#include <compiler.h>
#include <common.h>
#include <stdlib.h>
#include <dbam.h>
#include <log.h>
#include <stdbool.h>
#include <darray.h>
#include <dbutils.h>
#include <genrand.h>

/* wrappers needed for darray */
static int __partition_cmp(const void *a, const void *b);
static void __partition_destroy(void *a);

/*
    Capacity calculator
*/
static ___inline___ size_t db_am_entries_per_page(DB_AM *am);
static ___inline___ size_t db_am_pages_for_entries(DB_AM *am, size_t entries);
static ___inline___ size_t db_am_entries_per_block(DB_AM *am);
static ___inline___ size_t db_am_blocks_for_entries(DB_AM *am, size_t entries);

/*
    Create partition with entries

    PARAMS
    @IN am - pointer to AM system
    @IN num_entries - number of entries in partition

    RETURN
    Pointer to new partition
*/
static Partition *partition_create(DB_AM *am, size_t num_entries);

/*
    Partition destructor

    PARAMS
    @IN p - pointer to Partition

    RETURN
    This is a void function
*/
static void partition_destroy(Partition *p);

/*
    Random Block position in partition
    position is correct iff from pos to end of p we can read @entries valid entries

    PARAMS
    @IN p - pointer to partition
    @IN entries - number of entries

    RETURN
    Random position
*/
static size_t rand_leb_position(Partition *p, size_t entries);

/*
    Based on query type calculate number of entries to load from index

    PARAMS
    @IN am - pointer to AM system
    @IN type - query type
    @IN entries - entries needed by query

    RETURN
    Number of entries to load from index
*/
static ___inline___ size_t get_num_entries_from_index(DB_AM *am, query_t type, size_t entries);

/*
    Load entries from index

    PARAMS
    @IN am - pointer to AM system
    @IN entries - number of entries to load

    RETURN
    Time consumed by this operation
*/
static ___inline___ double load_entries_from_index(DB_AM *am, size_t entries);

/*
    Copy entries from partition to index

    PARAMS
    @IN am - pointer to AM system
    @IN entries - number of entries to copy

    RETURN
    Time consumed by this operation
*/
static ___inline___ double copy_entries_to_index(DB_AM *am, size_t entries);

/*
    Copy all entries from am to index

    PARAMS
    @IN AM - pointer to AM system

    RETURN
    Consumed time
*/
static ___inline___ double db_am_write_all_to_index(DB_AM *am);

/*
    Calculate memory usage and set mem usage via db stat framework

    PARAMS
    @IN am - pointer to AM system

    RETURN
    This is a void function
*/
static ___inline___ void db_am_calculate_mem_usage(DB_AM *am);

static int __partition_cmp(const void *a, const void *b)
{
    Partition *_a = *(Partition **)a;
    Partition *_b = *(Partition **)b;

    if (_a->num_entries < _b->num_entries)
        return -1;

    if (_a->num_entries > _b->num_entries)
        return 1;

    return 0;
}

static void __partition_destroy(void *a)
{
    Partition *_a = *(Partition **)a;

    partition_destroy(_a);
}

static ___inline___ size_t db_am_entries_per_page(DB_AM *am)
{
   return db_utils_entries_per_page(am->ssd->page_size, am->entry_size);
}

static ___inline___ size_t db_am_pages_for_entries(DB_AM *am, size_t entries)
{
    return db_utils_pages_for_entries(am->ssd->page_size, am->entry_size, entries);
}

static ___inline___ size_t db_am_entries_per_block(DB_AM *am)
{
    return db_utils_entries_per_block(am->ssd->block_size, am->ssd->page_size, am->entry_size);
}

static ___inline___ size_t db_am_blocks_for_entries(DB_AM *am, size_t entries)
{
    return db_utils_blocks_for_entries(am->ssd->block_size, am->ssd->page_size, am->entry_size, entries);
}

static size_t rand_leb_position(Partition *p, size_t entries)
{
    ssize_t entries_to_load;
    ssize_t i;

    TRACE();

    entries_to_load = (ssize_t)entries;
    for (i = (ssize_t)p->num_blocks - 1; i >= 0; --i)
    {
        if (p->blocks[i] != NULL)
        {
            entries_to_load -= (ssize_t)p->blocks[i]->valid_entries;
            if (entries_to_load <= 0)
                break;
        }
    }

    return genrand() % ((unsigned long)i + 1);
}

static Partition *partition_create(DB_AM *am, size_t num_entries)
{
    Partition *p;
    size_t i;

    TRACE();

    p = (Partition *)malloc(sizeof(Partition));
    if (p == NULL)
        ERROR("malloc error\n", NULL);

    p->num_entries = num_entries;
    p->num_blocks = db_am_blocks_for_entries(am, num_entries);
    p->last_loaded_block = 0;
    p->blocks = (LEB **)calloc(p->num_blocks, sizeof(LEB *));
    if (p->blocks == NULL)
    {
        FREE(p);
        ERROR("calloc error\n", NULL);
    }

    for (i = 0; i < p->num_blocks; ++i)
    {
        p->blocks[i] = (LEB *)calloc(1, sizeof(LEB));

        /* rollback */
        if (p->blocks[i] == NULL)
        {
            partition_destroy(p);
            ERROR("malloc error\n", NULL);
        }

        p->blocks[i]->valid_entries = MIN(num_entries, db_am_entries_per_block(am));
        num_entries -= p->blocks[i]->valid_entries;
        p->blocks[i]->p = p;
    }

    return p;
}

static void partition_destroy(Partition *p)
{
    size_t i;

    TRACE();

    if (p == NULL)
        return;

    for (i = 0; i < p->num_blocks; ++i)
        FREE(p->blocks[i]);

    FREE(p->blocks);
    FREE(p);
}

static ___inline___ void db_am_calculate_mem_usage(DB_AM *am)
{
    size_t memory = 0;
    size_t entries = 0;
    size_t pages_for_index = 0;

    TRACE();

    pages_for_index = db_utils_pages_for_entries(am->ssd->page_size, am->index->entry_size, am->index->num_entries);
    pages_for_index += db_utils_pages_for_entries(am->ssd->page_size, am->index->key_size + sizeof(void *), pages_for_index);
    memory += pages_for_index * am->ssd->page_size;

    entries = am->set.num_entries;
    memory += db_am_pages_for_entries(am, entries) * am->ssd->page_size;
    db_stat_set_mem(memory);
}

static ___inline___ size_t get_num_entries_from_index(DB_AM *am, query_t type, size_t entries)
{
    size_t entries_from_index = 0;
    size_t i;
    unsigned long r;

    TRACE();

    switch (type)
    {
        case QUERY_ALWAYS_NEW:
        {
            if (am->set.num_entries >= entries)
                entries_from_index = 0;
            else
                entries_from_index = entries - am->set.num_entries;
            break;
        }
        case QUERY_RANDOM:
        {
            entries_from_index = 0;
            for (i = 0; i < entries; ++i)
            {
                r = genrand() % am->num_entries;
                if ((size_t)r >= am->set.num_entries)
                    ++entries_from_index;
            }
            break;
        }
        case QUERY_SEQUENTIAL_PATTERN:
        {
            if (am->num_entries == am->index->num_entries)
                entries_from_index = entries;
            else
                entries_from_index = entries / 2;

            break;
        }
        default:
        {
            ERROR("Incorrect query type\n", 0);
            break;
        }
    }

    /*
        from_index = distribution based on query
        from_part = entries - from_index

        from_index <= am->index->num_entries
        from_part <= am->set.num_entries

        after combining that we have:

        1. from_index <= am->index->num_entries
        2. from_index >= entries - am->set.num_entries;
    */

    entries_from_index = MIN(entries_from_index, am->index->num_entries);

    if (entries > am->set.num_entries)
        entries_from_index = MAX(entries_from_index, entries - am->set.num_entries);

    return entries_from_index;
}

static ___inline___ double db_am_write_all_to_index(DB_AM *am)
{
    double time = 0.0;
    Partition *p;

    TRACE();

    //time += ssd_erase_blocks(am->ssd, db_am_blocks_for_entries(am, am->set.num_entries));
    time += copy_entries_to_index(am, am->set.num_entries);
    //db_stat_update_mem(-1 * (ssize_t)(db_am_pages_for_entries(am, am->set.num_entries) * am->ssd->page_size));

    am->set.num_entries = 0;
    for_each_data(am->set.partitions, Darray, p)
        p->num_entries = p->num_blocks = 0;

    return time;
}

static ___inline___ double load_entries_from_index(DB_AM *am, size_t entries)
{
    if (entries > 0)
        return db_index_range_search(am->index, entries);

    return 0.0;
}

static ___inline___ double copy_entries_to_index(DB_AM *am, size_t entries)
{
    if (entries > 0)
        return db_index_bulkload(am->index, entries);

    return 0.0;
}

static double create_partitions_for_entries(DB_AM *am, size_t entries)
{
    double time = 0.0;
    size_t num_partitions;
    size_t i;
    Partition *p;

    TRACE();

    num_partitions = INT_CEIL_DIV(entries, db_am_entries_per_block(am) * am->sort_buffer_size);
    am->set.num_entries += entries;

    for (i = 0; i < num_partitions; ++i)
    {
        p = partition_create(am, MIN(entries, am->sort_buffer_size * db_am_entries_per_block(am)));
        if (p == NULL)
            ERROR("partition_create error\n", 0.0);
        entries -= p->num_entries;

        if (darray_insert(am->set.partitions, (void *)&p))
            ERROR("darray_insert error\n", 0.0);

        /* write partition */
        time += ssd_swrite_pages(am->ssd, db_am_pages_for_entries(am, p->num_entries));
        //db_stat_update_mem((ssize_t)(am->ssd->page_size * db_am_pages_for_entries(am, p->num_entries)));
    }

    return time;
}

static double db_am_init(DB_AM *am, size_t entries)
{
    double time = 0.0;
    size_t entries_to_sort;

    TRACE();

    /* read entries from table */
    time += ssd_sread_pages(am->ssd, db_am_pages_for_entries(am, am->num_entries));

    /* create index with entries */
    time += db_index_bulkload(am->index, entries);

    /* entries to sort and write into partitions */
    entries_to_sort = am->num_entries - entries;

    /* create partitions set */
    am->set.partitions = darray_create(DARRAY_UNSORTED, 0, sizeof(Partition *), __partition_cmp, __partition_destroy);
    if (am->set.partitions == NULL)
        ERROR("db_am_init error\n", 0.0);

    /* create new partitions */
    time += create_partitions_for_entries(am, entries_to_sort);

    return time;
}

static ___inline___ double load_entries_from_partition(DB_AM *am, query_t type, size_t entries)
{
    double time = 0.0;
    Partition *p;
    size_t entries_read_from_this_partition;
    size_t entries_from_block;
    size_t leb_num;
    size_t i;
    size_t loaded_pages = 0;
    size_t start_leb;
    size_t to_rewrite = 0;

    TRACE();

    /* to schedule entries from partition in fair way, sort in descending order */
    darray_sort(am->set.partitions);

    /* reading from partitions */
    i = 0;
    for_each_data(am->set.partitions, Darray, p)
    {
        entries_read_from_this_partition = MIN(p->num_entries, INT_CEIL_DIV(entries, ((size_t)darray_get_num_entries(am->set.partitions) - i)));
        entries_read_from_this_partition = MIN(entries_read_from_this_partition, entries);
        entries -= entries_read_from_this_partition;
        ++i;

        if (entries_read_from_this_partition == 0)
            continue;

        /* find block using binary search */
        loaded_pages = LOG2((p->num_blocks * (am->ssd->block_size / am->ssd->page_size))) - 1;

        if (type == QUERY_SEQUENTIAL_PATTERN)
            leb_num = p->last_loaded_block;
        else
            leb_num = rand_leb_position(p, entries_read_from_this_partition);

        p->num_entries -= entries_read_from_this_partition;
        am->set.num_entries -= entries_read_from_this_partition;

        start_leb = 0;
        to_rewrite = 0;
        while (entries_read_from_this_partition > 0)
        {
            if (p->blocks[leb_num] != NULL && p->blocks[leb_num]->valid_entries > 0)
            {
                if (start_leb == 0)
                    start_leb = leb_num;

                entries_from_block = MIN(p->blocks[leb_num]->valid_entries, entries_read_from_this_partition);
                loaded_pages += db_am_pages_for_entries(am, entries_from_block);

                /* read entries from this block */
                p->blocks[leb_num]->valid_entries -= entries_from_block;
                to_rewrite += p->blocks[leb_num]->valid_entries;
                entries_read_from_this_partition -= entries_from_block;

                /* delete read entries */
                ssd_erase_blocks(am->ssd, 1);

                /* write back entries */
                if (p->blocks[leb_num]->valid_entries == 0)
                    ++leb_num;
                else
                    ssd_swrite_pages(am->ssd, db_am_pages_for_entries(am, p->blocks[leb_num]->valid_entries));
            }
            else
                ++leb_num;
        }

        p->last_loaded_block = 0;

        loaded_pages = MIN(loaded_pages, p->num_blocks * (am->ssd->block_size / am->ssd->page_size));
        time += ssd_sread_pages(am->ssd, loaded_pages);

        /* entries have been write back, so rewrite from start leb  */
        while (to_rewrite > 0)
        {
            p->blocks[start_leb]->valid_entries = MIN(db_am_entries_per_block(am), to_rewrite);
            to_rewrite -= p->blocks[start_leb]->valid_entries;
            ++start_leb;
        }
    }

    return time;
}


DB_AM *db_am_create(SSD *ssd, size_t num_entries, size_t key_size, size_t entry_size, size_t bs)
{
    DB_AM *am;

    TRACE();

    am = (DB_AM *)malloc(sizeof(DB_AM));
    if (am == NULL)
        ERROR("malloc error\n", NULL);

    am->num_entries = num_entries;
    am->entry_size = entry_size;
    am->ssd = ssd;
    am->sort_buffer_size = bs;
    am->set.num_entries = 0;
    am->set.partitions = NULL;

    am->index = db_index_create(ssd, key_size, entry_size);
    if (am->index == NULL)
        ERROR("db_index_create error\n", NULL);

    return am;
}

void db_am_destroy(DB_AM *am)
{
    TRACE();

    if (am == NULL)
        return;

    db_index_destroy(am->index);

    if (am->set.partitions != NULL)
        darray_destroy_with_entries(am->set.partitions);

    FREE(am);
}

double db_am_search(DB_AM *am, query_t type, size_t entries)
{
    double time = 0.0;
    size_t entries_from_index;
    size_t entries_from_partition;

    TRACE();

    if (am->index->num_entries == 0 && am->set.partitions == NULL)
    {
        time += db_am_init(am, entries);
        db_am_calculate_mem_usage(am);
        return time;
    }

    entries_from_index = get_num_entries_from_index(am, type, entries);
    entries_from_partition = entries - entries_from_index;

    /* load from index */
     time += load_entries_from_index(am, entries_from_index);

    /* load from partition */
    time += load_entries_from_partition(am, type, entries_from_partition);

    /* insert loaded entries from partition into index */
    time += copy_entries_to_index(am, entries_from_partition);

    if (db_am_blocks_for_entries(am, am->set.num_entries) <= am->sort_buffer_size)
        time += db_am_write_all_to_index(am);

    db_am_calculate_mem_usage(am);

    return time;
}