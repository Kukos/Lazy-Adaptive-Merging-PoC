#ifndef DBSTAT_H
#define DBSTAT_H

/*
    Data base statistics included total time consumptions, memory usage and current query time

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE GPL 3.0
*/

#include <compiler.h>
#include <stddef.h>
#include <sys/types.h>

typedef struct DB_snapshot
{
    size_t memory_usage; /* in bytes */

    /* time in seconds */
    double read_time;
    double write_time;
    double erase_time;
} DB_snapshot;

extern DB_snapshot db_current_query;
extern DB_snapshot db_maximum_snapshot;
extern DB_snapshot db_minimum_snapshot;
extern DB_snapshot db_total;

/*
    Private function, do not use directly

    PARAMS
    @IN sh - pointer to snapshot

    RETURN
    Sum of read, write, erase time
*/
static ___inline___ double __db_stat_get_time(DB_snapshot *sh);

/*
    Private function, do not use directly

    PARAMS
    @IN sh - pointer to snapshot

    RETURN
    Memory usage
*/
static ___inline___ size_t __db_stat_get_mem_usage(DB_snapshot *sh);

static ___inline___ double __db_stat_get_time(DB_snapshot *sh)
{
    return  sh->read_time + sh->write_time + sh->erase_time;
}

static ___inline___ size_t __db_stat_get_mem_usage(DB_snapshot *sh)
{
    return sh->memory_usage;
}

/*
    Reset whole DB Stat

    PARAMS
    NO PARAMS

    RETURN
    This is a void function
*/
void db_stat_reset(void);

/*
    Reset only current query statictics

    PARAMS
    NO PARAMS

    RETURN
    This is a void function
*/
void db_stat_reset_query(void);

/*
    Start new query

    PARAMS
    NO PARAMS

    RETURN
    This is a void function
*/
void db_stat_start_query(void);

/*
    Print on stdout info about current query

    PARAMS
    NO PARAMS

    RETURN
    This is a void function
*/
void db_stat_current_print(void);

/*
    Print on stdout info about total, min, max query

    PARAMS
    NO PARAMS

    RETURN
    This is a void function
*/
void db_stat_summary_print(void);

/*
    Update statistics (read / write / erase) for current query

    PARAMS
    @IN s - seconds

    RETURN
    This is a void function
*/
static ___inline___ void db_stat_update_read(double s);
static ___inline___ void db_stat_update_write(double s);
static ___inline___ void db_stat_update_erase(double s);

/*
    Update memory usage for current query

    PARAMS
    IN b - bytes (positive if you increased usage, negaive in other case)

    RETURN
    This is a void function
*/
static ___inline___ void db_stat_update_mem(ssize_t b);

/*
    Set usage of memory for current query

    PARAMS
    @IN b - bytes

    RETURN
    This is a void function
*/
static ___inline___ void db_stat_set_mem(size_t b);

/*
    Finish current query and update total, max, min statistics

    PARAMS
    NO PARAMS

    RETURN
    This is a void function
*/
void db_stat_finish_query(void);

/*
    Get query time (sum of read, write, erase) for (min / max / current) query and total time

    PARAMS
    NO PARAMS

    RETURN
    Query time
*/
static ___inline___ double db_stat_get_min_time(void);
static ___inline___ double db_stat_get_max_time(void);
static ___inline___ double db_stat_get_current_time(void);
static ___inline___ double db_stat_get_total_time(void);

/*
    Get query memory usage for (min / max / current) query and total

    PARAMS
    NO PARAMS

    RETURN
    Query memory usage
*/
static ___inline___ size_t db_stat_get_min_mem_usage(void);
static ___inline___ size_t db_stat_get_max_mem_usage(void);
static ___inline___ size_t db_stat_get_current_mem_usage(void);
static ___inline___ size_t db_stat_get_total_mem_usage(void);

static ___inline___ void db_stat_update_read(double s)
{
    db_current_query.read_time += s;
}

static ___inline___ void db_stat_update_write(double s)
{
    db_current_query.write_time += s;
}

static ___inline___ void db_stat_update_erase(double s)
{
    db_current_query.erase_time += s;
}

static ___inline___ void db_stat_update_mem(ssize_t b)
{
    const ssize_t memory = (ssize_t)db_current_query.memory_usage + b;
    db_current_query.memory_usage = (size_t)memory;
}

static ___inline___ double db_stat_get_min_time(void)
{
    return __db_stat_get_time(&db_minimum_snapshot);
}

static ___inline___ double db_stat_get_max_time(void)
{
    return __db_stat_get_time(&db_maximum_snapshot);
}

static ___inline___ double db_stat_get_current_time(void)
{
    return __db_stat_get_time(&db_current_query);
}

static ___inline___ double db_stat_get_total_time(void)
{
    return __db_stat_get_time(&db_total);
}

static ___inline___ size_t db_stat_get_min_mem_usage(void)
{
    return __db_stat_get_mem_usage(&db_minimum_snapshot);
}

static ___inline___ size_t db_stat_get_max_mem_usage(void)
{
    return __db_stat_get_mem_usage(&db_maximum_snapshot);
}

static ___inline___ size_t db_stat_get_current_mem_usage(void)
{
    return __db_stat_get_mem_usage(&db_current_query);
}

static ___inline___ size_t db_stat_get_total_mem_usage(void)
{
    return __db_stat_get_mem_usage(&db_total);
}

static ___inline___ void db_stat_set_mem(size_t b)
{
    db_current_query.memory_usage = b;
}

#endif
