#include <log.h>
#include <dbraw.h>
#include <dbstat.h>
#include <common.h>
#include <stdlib.h>
#include <dbutils.h>

/*
    PARAMS
    @IN raw - pointer to raw table

    RETURN
    Maximum number of entries that can be written on one page
*/
static ___inline___ size_t db_raw_entries_per_page(DB_raw *raw);

/*
    PARAMS
    @IN raw - pointer to raw table
    @IN entries - number of entries

    RETURN
    Number of pages used by entries
*/
static ___inline___ size_t db_raw_pages_for_entries(DB_raw *raw, size_t entries);

static ___inline___ size_t db_raw_entries_per_page(DB_raw *raw)
{
    return db_utils_entries_per_page(raw->ssd->page_size, raw->entry_size);
}

static ___inline___ size_t db_raw_pages_for_entries(DB_raw *raw, size_t entries)
{
    return db_utils_pages_for_entries(raw->ssd->page_size, raw->entry_size, entries);
}

DB_raw *db_raw_create(SSD *ssd, size_t entry_size)
{
    DB_raw *raw;

    TRACE();

    raw = (DB_raw *)malloc(sizeof(DB_raw));
    if (raw == NULL)
        ERROR("malloc error\n", NULL);

    raw->entry_size = entry_size;
    raw->ssd = ssd;
    raw->num_entries = 0;

    return raw;
}

void db_raw_destroy(DB_raw *raw)
{
    TRACE();

    if (raw == NULL)
        return;

    FREE(raw);
}

double db_raw_insert(DB_raw *raw, size_t entries)
{
    double time = 0.0;
    size_t i;
    size_t pages;

    TRACE();

    pages = db_raw_pages_for_entries(raw, raw->num_entries);

    for (i = 0; i < entries; ++i)
    {
        /* write at the end */
        time += ssd_update_pages(raw->ssd, 1);

        ++raw->num_entries;
    }

    pages = db_raw_pages_for_entries(raw, raw->num_entries) - pages;
    db_stat_update_mem((ssize_t)(pages * raw->ssd->page_size));

    return time;
}

double db_raw_bulkload(DB_raw *raw, size_t entries)
{
    double time = 0.0;
    size_t pages;

    TRACE();

    pages = db_raw_pages_for_entries(raw, raw->num_entries);

    /* insert at the end */
    time += ssd_swrite_pages(raw->ssd, db_raw_pages_for_entries(raw, entries));
    raw->num_entries += entries;

    pages = db_raw_pages_for_entries(raw, raw->num_entries) - pages;
    db_stat_update_mem((ssize_t)(pages * raw->ssd->page_size));

    return time;
}

double db_raw_point_search(DB_raw *raw, size_t entries)
{
    double time = 0.0;

    TRACE();

    (void)entries;

    /* data are unsorted, so scan all */
    time += ssd_sread_pages(raw->ssd, db_raw_pages_for_entries(raw, raw->num_entries));

    return time;
}

double db_raw_range_search(DB_raw *raw, size_t entries)
{
    double time = 0.0;

    TRACE();

    (void)entries;

    /* data are unsorted, so scan all */
    time += ssd_sread_pages(raw->ssd, db_raw_pages_for_entries(raw, raw->num_entries));

    return time;
}

double db_raw_delete(DB_raw *raw, size_t entries)
{
    double time = 0.0;
    size_t pages;

    TRACE();

    pages = db_raw_pages_for_entries(raw, raw->num_entries);

    /* data are unsorted, so scan all */
    time += ssd_sread_pages(raw->ssd, db_raw_pages_for_entries(raw, raw->num_entries) - db_raw_pages_for_entries(raw, entries));

    /* delete */
    time += ssd_update_pages(raw->ssd, db_raw_pages_for_entries(raw, entries));

    raw->num_entries -= entries;

    pages = db_raw_pages_for_entries(raw, raw->num_entries) - pages;
    db_stat_update_mem(-1 * (ssize_t)(pages * raw->ssd->page_size));

    return time;
}

double db_raw_update(DB_raw *raw, size_t entries)
{
    double time = 0.0;

    TRACE();

    /* data are unsorted, so scan all */
    time += ssd_sread_pages(raw->ssd, db_raw_pages_for_entries(raw, raw->num_entries) - db_raw_pages_for_entries(raw, entries));

    /* update */
    time += ssd_update_pages(raw->ssd, db_raw_pages_for_entries(raw, entries));

    return time;
}