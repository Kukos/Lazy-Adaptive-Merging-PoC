#ifndef DBINDEX_H
#define DBINDEX_H

/*
    Simualtion of index optimal for SSD memory

    Assumptions:
    0. P(N) = N / (page_size / entry_size)
    1. Hight H = log_{page_size / (sizeof(void *) + key_size}(n)
    2. Insert time = H
    3. Bulkload Time = P(N) + H
    4. Search Time = H
    5. Seq Search Time = H + P(N)
    6. Delete Time = H
    7. Update Time =  Search + Insert Time

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE GPL 3.0
*/

#include <compiler.h>
#include <stddef.h>
#include <sys/types.h>
#include <ssd.h>

typedef struct DB_index
{
    size_t num_entries;
    size_t entry_size; /* in bytes */
    size_t key_size; /* in bytes */
    size_t hight;

    SSD *ssd;

} DB_index;

/*
    Create empty index

    PARAMS
    @IN SSD - ssd
    @IN key_size - size of key in Bytes
    @IN entry_size - size of entry in Bytes

    RETURN
    Pointer to new index
*/
DB_index *db_index_create(SSD *ssd, size_t key_size, size_t entry_size);

/*
    Destroy index

    PARAMS
    @IN index - pointer to index

    RETURN
    This is a void function
*/
void db_index_destroy(DB_index *index);

/*
    Insert entries to index

    PARAMS
    @IN index - pointer to index
    @IN entries - entries to insert

    RETURN
    Insert time
*/
double db_index_insert(DB_index *index, size_t entries);

/*
    Insert entries via bulkload method

    PARAMS
    @IN index - pointer to index
    @IN entries - entries to insert

    RETURN
    Insert time
*/
double db_index_bulkload(DB_index *index, size_t entries);

/*
    Find entries by point search

    PARAMS
    @IN index - pointer to index
    @IN entries - entries to find

    RETURN
    Search time
*/
double db_index_point_search(DB_index *index, size_t entries);

/*
    Find entries by range search

    PARAMS
    @IN index - pointer to index
    @IN entries - entries to find

    RETURN
    Search time
*/
double db_index_range_search(DB_index *index, size_t entries);

/*
    Delete entries from index

    PARAMS
    @IN index - pointer to index
    @IN entries - entries to delete

    RETURN
    Delete time
*/
double db_index_delete(DB_index *index, size_t entries);

/*
    Update entries in index

    PARAMS
    @IN index - pointer to index
    @IN entries - entries to update

    RETURN
    Update Time
*/
double db_index_update(DB_index *index, size_t entries);

#endif