#include <log.h>
#include <dbstat.h>
#include <common.h>
#include <string.h>

DB_snapshot db_current_querry;
DB_snapshot db_maximum_snapshot;
DB_snapshot db_minimum_snapshot;
DB_snapshot db_total;

/*
    Print on stdout info about snapshot

    PARAMS
    @IN sh - pointer to snapshot

    RETURN
    This is a void function
*/
static ___inline___ void __db_stat_print(DB_snapshot *sh);

static ___inline___ void __db_stat_print(DB_snapshot *sh)
{
    printf("\tMEMORY USAGE   = %zuB\n", sh->memory_usage);
    printf("\tREAD  TIME     = %lfs\n", sh->read_time);
    printf("\tWRITE TIME     = %lfs\n", sh->write_time);
    printf("\tERASE TIME     = %lfs\n", sh->erase_time);
    printf("\tTOTAL TIME     = %lfs\n", __db_stat_get_time(sh));
}

void db_stat_reset(void)
{
    TRACE();

    LOG("Reseting db statistics\n");
    (void)memset(&db_current_querry, 0, sizeof(db_current_querry));
    (void)memset(&db_maximum_snapshot, 0, sizeof(db_maximum_snapshot));
    (void)memset(&db_minimum_snapshot, 0, sizeof(db_minimum_snapshot));
    (void)memset(&db_total, 0, sizeof(db_total));
}

void db_stat_reset_querry(void)
{
    TRACE();

    LOG("Reseting only current querry stat\n");
    (void)memset(&db_current_querry, 0, sizeof(db_current_querry));
}

void db_stat_start_querry(void)
{
    TRACE();

    LOG("Start new querry\n");
    db_stat_reset_querry();
    db_current_querry.memory_usage = db_total.memory_usage;
}

void db_stat_finish_querry(void)
{
    DB_snapshot sh = {0};

    TRACE();

    LOG("Finishing querry\n");

    /* update minimum snapshot */
    /* first cuerry so min = current */
    if (memcmp(&sh, &db_total, sizeof(sh)) == 0)
    {
        db_minimum_snapshot.read_time = db_current_querry.read_time;
        db_minimum_snapshot.write_time = db_current_querry.write_time;
        db_minimum_snapshot.erase_time = db_current_querry.erase_time;
        db_minimum_snapshot.memory_usage = db_current_querry.memory_usage;
    }
    else /* now min = min(min, current) */
    {
        if (db_stat_get_min_time() > db_stat_get_current_time())
        {
            db_minimum_snapshot.read_time = db_current_querry.read_time;
            db_minimum_snapshot.write_time = db_current_querry.write_time;
            db_minimum_snapshot.erase_time = db_current_querry.erase_time;
        }
        db_minimum_snapshot.memory_usage = MIN(db_minimum_snapshot.memory_usage, db_current_querry.memory_usage);
    }

    /* update total */
    db_total.read_time += db_current_querry.read_time;
    db_total.write_time += db_current_querry.write_time;
    db_total.erase_time += db_current_querry.erase_time;

    /* current memory usage is in total usage */
    db_total.memory_usage = db_current_querry.memory_usage;

    /* update maximum snapshot */
    if (db_stat_get_max_time() < db_stat_get_current_time())
    {
        db_maximum_snapshot.read_time = db_current_querry.read_time;
        db_maximum_snapshot.write_time = db_current_querry.write_time;
        db_maximum_snapshot.erase_time = db_current_querry.erase_time;
    }
    db_maximum_snapshot.memory_usage = MAX(db_maximum_snapshot.memory_usage, db_current_querry.memory_usage);
}

void db_stat_current_print(void)
{
    printf("CURRENT QUERRY\n\n");
    __db_stat_print(&db_current_querry);
}

void db_stat_summary_print(void)
{
    printf("TOTAL\n\n");
    __db_stat_print(&db_total);
    printf("\n\nMINIMUM\n\n");
    __db_stat_print(&db_minimum_snapshot);
    printf("\n\nMAXIMUM\n\n");
    __db_stat_print(&db_maximum_snapshot);
}