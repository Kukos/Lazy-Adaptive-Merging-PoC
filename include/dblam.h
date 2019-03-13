#ifndef DBLAM_H
#define DBLAM_H

/*
    Simulation of Lazy Adaptive Merging system

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE GPL 3.0
*/

#include <ssd.h>
#include <dbindex.h>
#include <stddef.h>
#include <darray.h>
#include <dbutils.h>

typedef struct LEB
{
    size_t invalid_entries;
    size_t valid_entries;
    struct Partition *p;

} LEB;

typedef struct Partition
{
    size_t num_entries; /* real entries, still valid */
    size_t num_blocks;

    size_t last_loaded_block; /* needed for QUERY_SEQUENTIAL_PATTERN */

    LEB **blocks;
} Partition;

typedef struct Partition_set
{
    size_t num_entries; /* real entries, still valid */
    Darray *partitions;

} Partition_set;

typedef struct DB_LAM
{
    // table
    __extension__  struct
    {
        size_t num_entries; /* num entries in whole table (const value) */
        size_t entry_size;
    };

    Partition_set set;

    DB_index *index;
    SSD *ssd;

    /* sizes in blocks */
    size_t sort_buffer_size;
    size_t merge_treshold;

    double usage_treshold;
    size_t max_blocks_in_reorganization;

} DB_LAM;


/*
    Create instance of LAM system

    PARAMS
    @IN SSD - ssd
    @IN num_entries - number of entries in table (process works on those entries)
    @IN key_size - key size in Bytes
    @IN entry_size - size of entry in Bytes
    @IN bs - buffer size (size in blocks)
    @IN mt - merge treshold (size in blocks)
    @IN ut - usage treshold
    @IN max_in_reorganization - maximum number of blocks in one reorganization

    RETURN
    Pointer to new raw Table
*/
DB_LAM *db_lam_create(SSD *ssd, size_t num_entries, size_t key_size, size_t entry_size, size_t bs, size_t mt, double ut, size_t max_in_reorganization);
/*
    Destroy LAM system

    PARAMS
    @IN lam - pointer to LAM system

    RETURN
    This is a void function
*/
void db_lam_destroy(DB_LAM *lam);

/*
    Find entries by key

    PARMAS
    @IN lam - pointer to LAM system
    @IN type - query type
    @IN entries - number of entries to find

    RETURN
    Query time
*/
double db_lam_search(DB_LAM *lam, query_t type, size_t entries);

/*
    Insert new entries into LAM

    PARAMS
    @IN lam - pointer to LAM system
    @IN entries - entries to insert

    RETURN
    Query time
*/
double db_lam_insert(DB_LAM *lam, size_t entries);

/*
    Delete entries

    PARAMS
    @IN lam - pointer to LAM system
    @IN entries - entries to delete

    RETURN
    Query time
*/
double db_lam_delete(DB_LAM *lam, size_t entries);

/*
    Update entries value

    PARAMS
    @IN lam - pointer to LAM system
    @IN entries - entries to update

    RETURN
    Query time
*/
double db_lam_update(DB_LAM *lam, size_t entries);

#endif