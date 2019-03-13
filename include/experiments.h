#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

/*
    Implementation of all experiment needed for this project

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE GPL 3.0
*/

#include <stddef.h>
#include <compiler.h>
#include <dbutils.h>
#include <ssd.h>

/*
    Normal workload experiment

        1. Bulkload N /2,
        2. sqrt(N) point search,
        3. N / 2 x Insert,
        4. Nlog(N) range search with 10% selectivity,
        5. sqrt(N) delete,
        6. sqrt(N) point search,
        7. sqrt(N) update,
        8. Nlog(N) range search with 5% selectivity

    PARAMS
    @IN queries - number of queries in batch (N)

    RETURN
    This is a void function
*/
void db_index_experiment_workload(size_t queries);


/*
    Normal workload experiment

        1. Bulkload N /2,
        2. sqrt(N) point search,
        3. N / 2 x Insert,
        4. Nlog(N) range search with 10% selectivity,
        5. sqrt(N) delete,
        6. sqrt(N) point search,
        7. sqrt(N) update,
        8. Nlog(N) range search with 5% selectivity

    PARAMS
    @IN queries - number of queries in batch (N)

    RETURN
    This is a void function
*/
void db_raw_experiment_workload(size_t queries);


/*
    This is only test workload for db lam to check all of functions

    PARAMS
    @IN queries - number od queries in batch (N)

    RETURN
    This is a void function
*/
void db_lam_experiment_workload(size_t queries);

/*
    This is only test workload for db la to check all of functions

    PARAMS
    @IN queries - number od queries in batch (N)

    RETURN
    This is a void function
*/
void db_am_experiment_workload(size_t queries);

void experiment1(const char const *file, SSD *ssd, size_t key_size, size_t data_size, size_t entries, query_t type, double selectivity);
void experiment2(const char const *file, SSD *ssd, size_t key_size, size_t data_size, size_t entries, query_t type, double selectivity);
void experiment3(const char const *file, SSD *ssd, size_t key_size, size_t data_size, size_t entries, query_t type, double selectivity, size_t queries);
void experiment4(const char const *file, SSD *ssd, size_t key_size, size_t data_size, size_t entries, query_t type, double selectivity, size_t queries);
void experiment5(const char const *file, SSD *ssd, size_t key_size, size_t data_size, size_t entries, query_t type, double selectivity[], size_t selectivity_len, size_t queries);
#endif