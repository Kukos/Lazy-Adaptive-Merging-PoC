#include <log.h>
#include <dbindex.h>
#include <ssd.h>
#include <math.h>
#include <stdlib.h>

#define DB_INDEX_INT_CEIL_DIV(n, k) (((n) + (k) - 1) / (k))

#define DB_INDEX_LOG(n, k) (log(n) / log(k))

/*
    Get Index hight

    PARAMS
    @IN index - pointer to index

    RETURN
    Index hight
*/
static ___inline___ size_t db_index_get_hight(DB_index *index);

/*
    PARAMS
    @IN index - pointer to index

    RETURN
    Maximum number of entries that can be written on one page
*/
static ___inline___ size_t db_index_entries_per_page(DB_index *index);

/*
    PARAMS
    @IN index - pointer to index
    @IN entries - number of entries

    RETURN
    Number of pages used by entries
*/
static ___inline___ size_t db_index_pages_for_entries(DB_index *index, size_t entries);

static ___inline___ size_t db_index_get_hight(DB_index *index)
{
    const double hight = ceil(DB_INDEX_LOG((double)index->num_entries, (double)((double)index->ssd->page_size / (double)(index->key_size + sizeof(void *)))));

    return (size_t)hight;
}

static ___inline___ size_t db_index_entries_per_page(DB_index *index)
{
    return DB_INDEX_INT_CEIL_DIV(index->ssd->page_size, index->entry_size);
}

static ___inline___ size_t db_index_pages_for_entries(DB_index *index, size_t entries)
{
    return DB_INDEX_INT_CEIL_DIV(entries, db_index_entries_per_page(index));
}

DB_index *db_index_create(SSD *ssd, size_t key_size, size_t entry_size)
{
    DB_index *index;

    TRACE();

    index = (DB_index *)calloc(1, sizeof(DB_index));
    if (index == NULL)
        ERROR("calloc error\n", NULL);

    index->ssd = ssd;
    index->key_size = key_size;
    index->entry_size = entry_size;

    return index;
}

void db_index_destroy(DB_index *index)
{
    TRACE();

    if (index == NULL)
        return;

    ssd_destroy(index->ssd);
    FREE(index);
}

double db_index_insert(DB_index *index, size_t entries)
{
    double time = 0.0;
    size_t i;

    size_t pages;

    TRACE();

    pages = db_index_pages_for_entries(index, index->num_entries);

    for (i = 0; i < entries; ++i)
    {
        /* find proper place for this key */
        time += ssd_rread_pages(index->ssd, index->hight);

        /* write it down */
        time += ssd_update_pages(index->ssd, 1);

        ++index->num_entries;
        index->hight = db_index_get_hight(index);
    }

    /* diff in pages */
    pages = db_index_pages_for_entries(index, index->num_entries) - pages;
    db_stat_update_mem((ssize_t)(pages * (sizeof(void *) + index->key_size) + (index->entry_size * entries)));

    return time;
}

double db_index_bulkload(DB_index *index, size_t entries)
{
    double time = 0.0;
    size_t pages;

    TRACE();

    pages = db_index_pages_for_entries(index, index->num_entries);

    /* build subtree */
    time += ssd_swrite_pages(index->ssd, db_index_pages_for_entries(index, entries));

    /* insert pointer to subtree */
    time += db_index_insert(index, 1);

    index->num_entries += entries;
    index->hight = db_index_get_hight(index);

    /* diff in pages */
    pages = db_index_pages_for_entries(index, index->num_entries) - pages;
    db_stat_update_mem((ssize_t)(pages * (sizeof(void *) + index->key_size) + (index->entry_size * entries)));

    return time;
}

double db_index_point_search(DB_index *index, size_t entries)
{
    double time = 0.0;

    TRACE();

    time += ssd_rread_pages(index->ssd, index->hight) * (double)entries;

    return time;
}

double db_index_range_search(DB_index *index, size_t entries)
{
    double time = 0.0;

    TRACE();

    /* find start point */
    time += db_index_point_search(index, 1);

    /* read all entries */
    time += ssd_sread_pages(index->ssd, db_index_pages_for_entries(index, entries) - 1);

    return time;
}

double db_index_delete(DB_index *index, size_t entries)
{
    double time = 0.0;
    size_t pages;

    TRACE();

    pages = db_index_pages_for_entries(index, index->num_entries);

    if (index->hight > 1)
    {
        /* find key */
        time += ssd_rread_pages(index->ssd, index->hight);
    }

    /* delete */
    time += ssd_update_pages(index->ssd, db_index_pages_for_entries(index, entries));

    index->num_entries -= entries;
    index->hight = db_index_get_hight(index);

    /* diff in pages */
    pages = pages - db_index_pages_for_entries(index, index->num_entries);
    db_stat_update_mem(-1 * (ssize_t)(pages * (sizeof(void *) + index->key_size) + (index->entry_size * entries)));

    return time;
}

double db_index_update(DB_index *index, size_t entries)
{
    double time = 0.0;

    TRACE();

    if (index->hight > 1)
    {
        /* find proper place for this key */
        time += ssd_rread_pages(index->ssd, index->hight);
    }

    /* update */
    time += ssd_update_pages(index->ssd, db_index_pages_for_entries(index, entries));

    return time;
}