#include <compiler.h>
#include <common.h>
#include <stdlib.h>
#include <dblam.h>
#include <log.h>
#include <stdbool.h>
#include <darray.h>
#include <dbutils.h>
#include <genrand.h>
#include <sort.h>

/* to avoid reorganization with almost full leb */
#define USAGE_TRESHOLD 0.05
#define MAX_LEB_IN_REORGANIZATION 10

/* wrappers needed for darray */
static int __partition_cmp(const void *a, const void *b);
static void __partition_destroy(void *a);

static int __block_cmp(const void *a, const void *b);

/*
    Create partition with entries

    PARAMS
    @IN lam - pointer to LAM system
    @IN num_entries - number of entries in partition

    RETURN
    Pointer to new partition
*/
static Partition *partition_create(DB_LAM *lam, size_t num_entries);

/*
    Partition destructor

    PARAMS
    @IN p - pointer to Partition

    RETURN
    This is a void function
*/
static void partition_destroy(Partition *p);

/*
    Create partitions for entries and add into partition set

    PARAMS
    @IN lam - pointer to LAM system
    @IN entries - number of entries to insert into partitions

    RETURN
    Time consumed by this operation
*/
static double create_partitions_for_entries(DB_LAM *lam, size_t entries);

/*
    Random Block position in partition
    Posision is correct iff from pos to end of p we can read @entries valid entries

    PARAMS
    @IN p - pointer to partition
    @IN entries - number of entries

    RETURN
    Random posision
*/
static size_t rand_leb_posision(Partition *p, size_t entries);

/*
    Capacity calculator
*/
static ___inline___ size_t db_lam_entries_per_page(DB_LAM *lam);
static ___inline___ size_t db_lam_pages_for_entries(DB_LAM *lam, size_t entries);
static ___inline___ size_t db_lam_entries_per_block(DB_LAM *lam);
static ___inline___ size_t db_lam_blocks_for_entries(DB_LAM *lam, size_t entries);

/*
    Calculate leb usage

    PARAMS
    @IN lam - pointer to LAM system
    @IN leb - pointer to Block

    RETURN
    LEB usage
*/
static ___inline___ double leb_usage(DB_LAM *lam, LEB *leb);

/*
    Checking usage of leb

    PARAMS
    @IN lam - pointer to LAM system
    @IN leb - pointer to block

    RETURN
    True if LEB can be deleted
    False if not
*/
static ___inline___ bool leb_to_delete(DB_LAM *lam, LEB *leb);

/*
    Based on querry type calculate number of entries to load from index

    PARAMS
    @IN lam - pointer to LAM system
    @IN type - querry type
    @IN entries - entries needed by querry

    RETURN
    Number of entries to load from index
*/
static ___inline___ size_t get_num_entries_from_index(DB_LAM *lam, querry_t type, size_t entries);

/*
    Load entries from index

    PARAMS
    @IN lam - pointer to LAM system
    @IN entries - number of entries to load

    RETURN
    Time consumed by this operation
*/
static ___inline___ double load_entries_from_index(DB_LAM *lam, size_t entries);

/*
    Copy entries from partition to index

    PARAMS
    @IN lam - pointer to LAM system
    @IN entries - number of entries to copy

    RETURN
    Time consumed by this operation
*/
static ___inline___ double copy_entries_to_index(DB_LAM *lam, size_t entries);

/*
    Calculate how many blocks LAM is using

    PARAMS
    @IN lam - pointer to LAM system

    RETURN
    Number of blocks
*/
static ___inline___ size_t lam_num_blocks(DB_LAM *lam);

/*
    Unlink leb from partition

    PARAMS
    @IN leb - pointer to LEB

    RETURN
    This is a void function
*/
static ___inline___ void unlink_leb(LEB *leb);

/*
    Get blocks used by LAM

    PARAMS
    @IN lam - pointer to LAM system

    RETURN
    Array of pointers to blocks
*/
static ___inline___ LEB **lam_get_blocks(DB_LAM *lam);

/*
    Check if reorganization is needed

    PARAMS
    @IN lam - pointer to LAM system

    RETURN
    true if needed
    false if not
*/
static bool can_do_reorganization(DB_LAM *lam);

/*
    Perform reorganization

    PARAMS
    @IN lam - pointer to lam

    RETURN
    Time consumed by this operation
*/
static double db_lam_reorganization(DB_LAM *lam);

/*
    Copy all entries from lam to index

    PARAMS
    @IN LAM - pointer to LAM system

    RETURN
    Consumed time
*/
static ___inline___ double db_lam_write_all_to_index(DB_LAM *lam);

/*
    Calculate memory usage and set mem usage via db stat framework

    PARAMS
    @IN lam - pointer to LAM system

    RETURN
    This is a void function
*/
static ___inline___ void db_lam_calculate_mem_usage(DB_LAM *lam);

static size_t rand_leb_posision(Partition *p, size_t entries)
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

static ___inline___ double leb_usage(DB_LAM *lam, LEB *leb)
{
    const size_t entries_per_block = db_lam_entries_per_block(lam);
    return (double)leb->invalid_entries / (double)entries_per_block;
}

static ___inline___ bool leb_to_delete(DB_LAM *lam, LEB *leb)
{
    return leb_usage(lam, leb) >= lam->usage_treshold;
}

static ___inline___ size_t db_lam_entries_per_page(DB_LAM *lam)
{
   return db_utils_entries_per_page(lam->ssd->page_size, lam->entry_size);
}

static ___inline___ size_t db_lam_pages_for_entries(DB_LAM *lam, size_t entries)
{
    return db_utils_pages_for_entries(lam->ssd->page_size, lam->entry_size, entries);
}

static ___inline___ size_t db_lam_entries_per_block(DB_LAM *lam)
{
    return db_utils_entries_per_block(lam->ssd->block_size, lam->ssd->page_size, lam->entry_size);
}

static ___inline___ size_t db_lam_blocks_for_entries(DB_LAM *lam, size_t entries)
{
    return db_utils_blocks_for_entries(lam->ssd->block_size, lam->ssd->page_size, lam->entry_size, entries);
}

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

static int __block_cmp(const void *a, const void *b)
{
    LEB *_a = *(LEB **)a;
    LEB *_b = *(LEB **)b;

    if (_a->invalid_entries < _b->invalid_entries)
        return 1;

    if (_a->invalid_entries > _b->invalid_entries)
        return -1;

    return 0;
}

static Partition *partition_create(DB_LAM *lam, size_t num_entries)
{
    Partition *p;
    size_t i;

    TRACE();

    p = (Partition *)malloc(sizeof(Partition));
    if (p == NULL)
        ERROR("malloc error\n", NULL);

    p->num_entries = num_entries;
    p->num_blocks = db_lam_blocks_for_entries(lam, num_entries);
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

        p->blocks[i]->valid_entries = MIN(num_entries, db_lam_entries_per_block(lam));
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

static ___inline___ size_t lam_num_blocks(DB_LAM *lam)
{
    size_t blocks = 0;
    Partition *p;
    size_t i;

    TRACE();

    for_each_data(lam->set.partitions, Darray, p)
        for (i = 0; i < p->num_blocks; ++i)
            if (p->blocks[i] != NULL)
                ++blocks;

    return blocks;
}

static ___inline___ LEB **lam_get_blocks(DB_LAM *lam)
{
    LEB **blocks;
    size_t num_blocks;
    Partition *p;
    size_t i;
    size_t index = 0;

    TRACE();

    num_blocks = lam_num_blocks(lam);
    blocks = (LEB **)malloc(sizeof(LEB *) * num_blocks);
    if (blocks == NULL)
        ERROR("malloc error\n", NULL);

    for_each_data(lam->set.partitions, Darray, p)
        for (i = 0; i < p->num_blocks; ++i)
            if (p->blocks[i] != NULL)
                blocks[index++] = p->blocks[i];

    return blocks;
}

static double create_partitions_for_entries(DB_LAM *lam, size_t entries)
{
    double time = 0.0;
    size_t num_partitions;
    size_t i;
    Partition *p;

    TRACE();

    num_partitions = INT_CEIL_DIV(entries, db_lam_entries_per_block(lam) * lam->sort_buffer_size);
    lam->set.num_entries += entries;

    for (i = 0; i < num_partitions; ++i)
    {
        p = partition_create(lam, MIN(entries, lam->sort_buffer_size * db_lam_entries_per_block(lam)));
        if (p == NULL)
            ERROR("partition_create error\n", 0.0);
        entries -= p->num_entries;

        if (darray_insert(lam->set.partitions, (void *)&p))
            ERROR("darray_insert error\n", 0.0);

        /* write partition */
        time += ssd_swrite_pages(lam->ssd, db_lam_pages_for_entries(lam, p->num_entries));
        //db_stat_update_mem((ssize_t)(lam->ssd->page_size * db_lam_pages_for_entries(lam, p->num_entries)));
    }

    return time;
}

static double db_lam_init(DB_LAM *lam, size_t entries)
{
    double time = 0.0;
    size_t entries_to_sort;

    TRACE();

    /* read entries from table */
    time += ssd_sread_pages(lam->ssd, db_lam_pages_for_entries(lam, lam->num_entries));

    /* create index with entries */
    time += db_index_bulkload(lam->index, entries);

    /* entries to sort and write into partitions */
    entries_to_sort = lam->num_entries - entries;

    /* create partitions set */
    lam->set.partitions = darray_create(DARRAY_UNSORTED, 0, sizeof(Partition *), __partition_cmp, __partition_destroy);
    if (lam->set.partitions == NULL)
        ERROR("db_lam_init error\n", 0.0);

    /* create new partitions */
    time += create_partitions_for_entries(lam, entries_to_sort);

    return time;
}

static ___inline___ void unlink_leb(LEB *leb)
{
    size_t i;
    LEB **blocks;

    TRACE();

    leb->p->num_entries -= leb->valid_entries;

    blocks = leb->p->blocks;
    for (i = 0; i < leb->p->num_blocks; ++i)
        if (blocks[i] != NULL && blocks[i] == leb)
            leb->p->blocks[i] = NULL;
}

static bool can_do_reorganization(DB_LAM *lam)
{
    LEB **blocks;
    size_t num_blocks;
    size_t i;
    double trash;

    TRACE();

    if (lam->set.num_entries == 0)
        return false;

    blocks = lam_get_blocks(lam);
    num_blocks = lam_num_blocks(lam);

    insort(blocks, num_blocks, __block_cmp, sizeof(LEB *));

    trash = 0.0;
    for (i = 0; i < MIN(lam->max_blocks_in_reorganization, num_blocks); ++i)
        if (leb_to_delete(lam, blocks[i]))
            trash += leb_usage(lam, blocks[i]);


    FREE(blocks);
    return trash >= (double)lam->merge_treshold;
}

static double db_lam_reorganization(DB_LAM *lam)
{
    double time = 0.0;
    LEB **blocks;
    size_t num_blocks;
    size_t i;
    size_t entries;

    TRACE();

    blocks = lam_get_blocks(lam);
    num_blocks = lam_num_blocks(lam);
    insort(blocks, num_blocks, __block_cmp, sizeof(LEB *));

    entries = 0;
    for (i = 0; i < MIN(lam->max_blocks_in_reorganization, num_blocks); ++i)
        if (leb_to_delete(lam, blocks[i]))
        {
            /* load entries into buffer */
            entries += blocks[i]->valid_entries;

            /* unlink block */
            unlink_leb(blocks[i]);

            /* erase block */
            time += ssd_erase_blocks(lam->ssd, 1);

            lam->set.num_entries -= blocks[i]->valid_entries;

            //db_stat_update_mem(-1 * (ssize_t)(db_lam_pages_for_entries(lam, blocks[i]->invalid_entries + blocks[i]->valid_entries) * lam->ssd->page_size));

            FREE(blocks[i]);
        }

    FREE(blocks);

    /* create new partitions */
    create_partitions_for_entries(lam, entries);

    return time;
}

static ___inline___ size_t get_num_entries_from_index(DB_LAM *lam, querry_t type, size_t entries)
{
    size_t entries_from_index = 0;
    size_t i;
    unsigned long r;

    TRACE();

    switch (type)
    {
        case QUERRY_ALWAYS_NEW:
        {
            if (lam->set.num_entries >= entries)
                entries_from_index = 0;
            else
                entries_from_index = entries - lam->set.num_entries;
            break;
        }
        case QUERRY_RANDOM:
        {
            entries_from_index = 0;
            for (i = 0; i < entries; ++i)
            {
                r = genrand() % lam->num_entries;
                if ((size_t)r >= lam->set.num_entries)
                    ++entries_from_index;
            }
            break;
        }
        case QUERRY_SEQUENTIAL_PATTERN:
        {
            if (lam->num_entries == lam->index->num_entries)
                entries_from_index = entries;
            else
                entries_from_index = entries / 2;

            break;
        }
        default:
        {
            ERROR("Incorrect querry type\n", 0);
            break;
        }
    }

    /*
        from_index = distribution based on querry
        from_part = entries - from_index

        from_index <= lam->index->num_entries
        from_part <= lam->set.num_entries

        after combining that we have:

        1. from_index <= lam->index->num_entries
        2. from_index >= entries - lam->set.num_entries;
    */

    entries_from_index = MIN(entries_from_index, lam->index->num_entries);

    if (entries > lam->set.num_entries)
        entries_from_index = MAX(entries_from_index, entries - lam->set.num_entries);

    return entries_from_index;
}

static ___inline___ double load_entries_from_index(DB_LAM *lam, size_t entries)
{
    if (entries > 0)
        return db_index_range_search(lam->index, entries);

    return 0.0;
}

static ___inline___ double copy_entries_to_index(DB_LAM *lam, size_t entries)
{
    if (entries > 0)
        return db_index_bulkload(lam->index, entries);

    return 0.0;
}

static ___inline___ double load_entries_from_partition(DB_LAM *lam, querry_t type, size_t entries)
{
    double time = 0.0;
    Partition *p;
    size_t entries_read_from_this_partition;
    size_t entries_from_block;
    size_t leb_num;
    size_t i;
    size_t loaded_pages = 0;
    size_t c = 0;

    TRACE();

    /* to schedule entries from partition in fair way, sort in descending order */
    darray_sort(lam->set.partitions);

    /* reading from partitions */
    i = 0;
    for_each_data(lam->set.partitions, Darray, p)
    {
        entries_read_from_this_partition = MIN(p->num_entries, INT_CEIL_DIV(entries, ((size_t)darray_get_num_entries(lam->set.partitions) - i)));
        entries_read_from_this_partition = MIN(entries_read_from_this_partition, entries);
        entries -= entries_read_from_this_partition;
        c += entries_read_from_this_partition;
        ++i;

        if (entries_read_from_this_partition == 0)
            continue;

        /* find block using binary search */
        loaded_pages = LOG2((p->num_blocks * (lam->ssd->block_size / lam->ssd->page_size))) - 1;

        if (type == QUERRY_SEQUENTIAL_PATTERN)
            leb_num = p->last_loaded_block;
        else
            leb_num = rand_leb_posision(p, entries_read_from_this_partition);

        p->num_entries -= entries_read_from_this_partition;
        lam->set.num_entries -= entries_read_from_this_partition;

        while (entries_read_from_this_partition > 0)
        {
            if (p->blocks[leb_num] != NULL)
            {
                entries_from_block = MIN(p->blocks[leb_num]->valid_entries, entries_read_from_this_partition);
                loaded_pages += db_lam_pages_for_entries(lam, entries_from_block);

                p->blocks[leb_num]->valid_entries -= entries_from_block;
                p->blocks[leb_num]->invalid_entries += entries_from_block;

                entries_read_from_this_partition -= entries_from_block;

                if (p->blocks[leb_num]->valid_entries == 0)
                    ++leb_num;
            }
            else
                ++leb_num;
        }

        p->last_loaded_block = leb_num;

        loaded_pages = MIN(loaded_pages, p->num_blocks * (lam->ssd->block_size / lam->ssd->page_size));
        time += ssd_sread_pages(lam->ssd, loaded_pages);
    }

    return time;
}

static ___inline___ double db_lam_write_all_to_index(DB_LAM *lam)
{
    double time = 0.0;
    Partition *p;

    TRACE();

    time += ssd_erase_blocks(lam->ssd, db_lam_blocks_for_entries(lam, lam->set.num_entries));
    time += copy_entries_to_index(lam, lam->set.num_entries);
    //db_stat_update_mem(-1 * (ssize_t)(db_lam_pages_for_entries(lam, lam->set.num_entries) * lam->ssd->page_size));

    lam->set.num_entries = 0;
    for_each_data(lam->set.partitions, Darray, p)
        p->num_entries = p->num_blocks = 0;

    return time;
}

static ___inline___ void db_lam_calculate_mem_usage(DB_LAM *lam)
{
    size_t memory = 0;
    Partition *p;
    size_t i;
    size_t entries = 0;
    size_t pages_for_index = 0;

    TRACE();

    pages_for_index = db_utils_pages_for_entries(lam->ssd->page_size, lam->index->entry_size, lam->index->num_entries);
    pages_for_index += db_utils_pages_for_entries(lam->ssd->page_size, lam->index->key_size + sizeof(void *), pages_for_index);
    memory += pages_for_index * lam->ssd->page_size;

    entries = 0;
    for_each_data(lam->set.partitions, Darray, p)
        for (i = 0; i < p->num_blocks; ++i)
            if (p->blocks[i] != NULL)
                entries += p->blocks[i]->invalid_entries + p->blocks[i]->valid_entries;

    memory += db_lam_pages_for_entries(lam, entries) * lam->ssd->page_size;
    db_stat_set_mem(memory);
}

DB_LAM *db_lam_create(SSD *ssd, size_t num_entries, size_t key_size, size_t entry_size, size_t bs, size_t mt, double ut, size_t max_in_reorganization)
{
    DB_LAM *lam;

    TRACE();

    lam = (DB_LAM *)malloc(sizeof(DB_LAM));
    if (lam == NULL)
        ERROR("malloc error\n", NULL);

    lam->num_entries = num_entries;
    lam->entry_size = entry_size;
    lam->ssd = ssd;
    lam->sort_buffer_size = bs;
    lam->merge_treshold = mt;
    lam->set.num_entries = 0;
    lam->set.partitions = NULL;
    lam->usage_treshold = ut;
    lam->max_blocks_in_reorganization = max_in_reorganization;

    lam->index = db_index_create(ssd, key_size, entry_size);
    if (lam->index == NULL)
        ERROR("db_index_create error\n", NULL);

    return lam;
}

void db_lam_destroy(DB_LAM *lam)
{
    TRACE();

    if (lam == NULL)
        return;

    db_index_destroy(lam->index);

    if (lam->set.partitions != NULL)
        darray_destroy_with_entries(lam->set.partitions);

    FREE(lam);
}


double db_lam_search(DB_LAM *lam, querry_t type, size_t entries)
{
    double time = 0.0;
    size_t entries_from_index;
    size_t entries_from_partition;

    TRACE();

    if (lam->index->num_entries == 0 && lam->set.partitions == NULL)
    {
        time += db_lam_init(lam, entries);
        db_lam_calculate_mem_usage(lam);
        return time;
    }

    entries_from_index = get_num_entries_from_index(lam, type, entries);
    entries_from_partition = entries - entries_from_index;

    /* load from index */
    time += load_entries_from_index(lam, entries_from_index);

    /* load from partition */
    time += load_entries_from_partition(lam, type, entries_from_partition);

    /* insert loaded entries from partition into index */
    time += copy_entries_to_index(lam, entries_from_partition);

    /* check if reorganization is needed */
    if (can_do_reorganization(lam))
        time += db_lam_reorganization(lam);

    if (db_lam_blocks_for_entries(lam, lam->set.num_entries) <= lam->merge_treshold)
        time += db_lam_write_all_to_index(lam);

    db_lam_calculate_mem_usage(lam);

    return time;
}

double db_lam_insert(DB_LAM *lam, size_t entries)
{
    TRACE();

    return db_index_bulkload(lam->index, entries);
}

double db_lam_delete(DB_LAM *lam, size_t entries)
{
    double time = 0.0;
    size_t entries_from_index;
    size_t entries_from_partition;

    TRACE();

    if (lam->index->num_entries == 0 && lam->set.partitions == NULL)
    {
        time += db_lam_init(lam, entries);
        return time;
    }

    entries_from_index = get_num_entries_from_index(lam, QUERRY_RANDOM, entries);
    entries_from_partition = entries - entries_from_index;

    /* delete from index */
    time += db_index_delete(lam->index, entries_from_index);

    /* load from partition */
    time += load_entries_from_partition(lam, QUERRY_RANDOM, entries_from_partition);

    /* check if reorganization is needed */
    if (can_do_reorganization(lam))
        time += db_lam_reorganization(lam);

    if (db_lam_blocks_for_entries(lam, lam->set.num_entries) <= lam->merge_treshold)
        time += db_lam_write_all_to_index(lam);

    db_lam_calculate_mem_usage(lam);

    return time;
}

double db_lam_update(DB_LAM *lam, size_t entries)
{
    double time = 0.0;

    TRACE();

    time += db_lam_delete(lam, entries);
    time += db_lam_insert(lam, entries);

    db_lam_calculate_mem_usage(lam);

    return time;
}
