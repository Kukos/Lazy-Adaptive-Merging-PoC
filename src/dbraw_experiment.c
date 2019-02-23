#include <dbraw.h>
#include <experiments.h>
#include <log.h>
#include <dbstat.h>
#include <math.h>
#include <common.h>

void db_raw_experiment_workload(size_t querries)
{
    DB_raw *raw;
    SSD *ssd;
    size_t i;
    double _sqrt_n = ceil(sqrt((double)querries));
    size_t sqrt_n = (size_t)_sqrt_n;
    size_t nlogn = querries * LOG2(querries);

    TRACE();

    ssd = ssd_create_samsung840();
    raw = db_raw_create(ssd, 140);
    db_stat_reset();

    /* bukload N / 2 */
    db_stat_start_querry();
    db_raw_bulkload(raw, querries / 2);
    db_stat_finish_querry();

    /* sqrt(N) point search */
    for (i = 0; i < sqrt_n; ++i)
    {
        db_stat_start_querry();
        db_raw_point_search(raw, 1);
        db_stat_finish_querry();
    }

    /* Normal Insert N / 2 */
    for (i = 0; i < querries / 2; ++i)
    {
        db_stat_start_querry();
        db_raw_insert(raw, 1);
        db_stat_finish_querry();
    }

    /* NlogN range search with 10% selecivity */
    for (i = 0; i < nlogn; ++i)
    {
        db_stat_start_querry();
        db_raw_range_search(raw, (raw->num_entries + 9) / 10);
        db_stat_finish_querry();
    }

    /* sqrt(N) delete */
    for (i = 0; i < sqrt_n; ++i)
    {
        db_stat_start_querry();
        db_raw_delete(raw, 1);
        db_stat_finish_querry();
    }

    /* sqrt(N) point search */
    for (i = 0; i < sqrt_n; ++i)
    {
        db_stat_start_querry();
        db_raw_point_search(raw, 1);
        db_stat_finish_querry();
    }

    /* sqrt(N) update */
    for (i = 0; i < sqrt_n; ++i)
    {
        db_stat_start_querry();
        db_raw_update(raw, 1);
        db_stat_finish_querry();
    }

    /* NlogN range search with 5% selectivity */
    for (i = 0; i < nlogn; ++i)
    {
        db_stat_start_querry();
        db_raw_range_search(raw, (raw->num_entries + 19) / 20);
        db_stat_finish_querry();
    }

    db_stat_summary_print();
    db_raw_destroy(raw);
    ssd_destroy(ssd);
}