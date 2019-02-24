#ifndef DBAM_H
#define DBAM_H

/*
    Simulation of Adaptive Merging

    (To make this simpler, simulation works as LAM but without lazy erase)

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE GPL 3.0
*/

#include <ssd.h>
#include <dbindex.h>
#include <stddef.h>
#include <darray.h>
#include <dbutils.h>
#include <dblam.h>

typedef struct DB_AM
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
} DB_AM;

/*
    Create instance of AM system

    PARAMS
    @IN SSD - ssd
    @IN num_entries - number of entries in table (process works on those entries)
    @IN key_size - key size in Bytes
    @IN entry_size - size of entry in Bytes
    @IN bs - buffer size (size in blocks)

    RETURN
    Pointer to new raw Table
*/
DB_AM *db_am_create(SSD *ssd, size_t num_entries, size_t key_size, size_t entry_size, size_t bs);

/*
    Destroy AM system

    PARAMS
    @IN am - pointer to AM system

    RETURN
    This is a void function
*/
void db_am_destroy(DB_AM *am);

/*
    Find entries by key

    PARMAS
    @IN am - pointer to AM system
    @IN type - querry type
    @IN entries - number of entries to find

    RETURN
    Querry time
*/
double db_am_search(DB_AM *am, querry_t type, size_t entries);

#endif