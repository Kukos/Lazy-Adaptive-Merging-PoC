#include <log.h>
#include <dbstat.h>
#include <common.h>

DB_snapshot db_current_querry;
DB_snapshot db_maximum_snapshot;
DB_snapshot db_minimum_snapshot;
DB_snapshot db_total;

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

void db_stat_finish_querry(void)
{
    TRACE();

    /* update total */
    db_total.read_time += db_current_querry.read_time;
    db_total.write_time += db_current_querry.write_time;
    db_total.erase_time += db_current_querry.erase_time;

    /* current memory usage is in total usage */
    db_total.memory_usage = db_current_querry.memory_usage;

    /* update minimum snapshot */
    db_minimum_snapshot.read_time = MIN(db_minimum_snapshot.read_time, db_current_querry.read_time);
    db_minimum_snapshot.write_time = MIN(db_minimum_snapshot.write_time, db_current_querry.write_time);
    db_minimum_snapshot.erase_time = MIN(db_minimum_snapshot.erase_time, db_current_querry.erase_time);
    db_minimum_snapshot.memory_usage = MIN(db_minimum_snapshot.memory_usage, db_current_querry.memory_usage);

    /* update maximum snapshot */
    db_maximum_snapshot.read_time = MAX(db_maximum_snapshot.read_time, db_current_querry.read_time);
    db_maximum_snapshot.write_time = MAX(db_maximum_snapshot.write_time, db_current_querry.write_time);
    db_maximum_snapshot.erase_time = MAX(db_maximum_snapshot.erase_time, db_current_querry.erase_time);
    db_maximum_snapshot.memory_usage = MAX(db_maximum_snapshot.memory_usage, db_current_querry.memory_usage);
}