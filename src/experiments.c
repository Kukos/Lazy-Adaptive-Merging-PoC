#include <experiments.h>
#include <dbstat.h>
#include <dbraw.h>
#include <dbindex.h>
#include <dblam.h>
#include <ssd.h>
#include <common.h>
#include <log.h>

void experiment1(size_t key_size, size_t data_size, size_t entries, querry_t type, double selectivity)
{
    SSD *ssd;
    DB_index *index;
    DB_raw *raw;
    DB_LAM *lam;
    size_t i;

    TRACE();

    ssd = ssd_create_samsung840();
    index = db_index_create(ssd, key_size, data_size);
    raw = db_raw_create(ssd, data_size);
    lam = db_lam_create(ssd, entries, key_size, data_size, 50, 3, 0.05, 60);

    db_index_bulkload(index, entries);
    db_raw_bulkload(raw, entries);

    /* time for search from index */
    db_stat_reset();
    db_stat_start_querry();
    db_index_range_search(index, (size_t)((double)entries * selectivity));
    db_stat_finish_querry();
    db_stat_summary_print();

    /* time for search from raw */
    db_stat_reset();
    db_stat_start_querry();
    db_raw_range_search(raw, (size_t)((double)entries * selectivity));
    db_stat_finish_querry();
    db_stat_summary_print();

    db_stat_reset();
    i = 0;
    do
    {
        db_stat_start_querry();
        db_lam_search(lam, type, (size_t)((double)entries * selectivity));
        db_stat_finish_querry();

        ++i;
    } while (lam->set.num_entries > 0);

    printf("AFTER %zu querries we have full index\n", i);
    db_stat_summary_print();

    ssd_destroy(ssd);
    db_index_destroy(index);
    db_raw_destroy(raw);
    db_lam_destroy(lam);
}