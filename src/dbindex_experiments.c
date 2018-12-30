#include <dbindex.h>
#include <experiments.h>
#include <log.h>
#include <dbstat.h>
#include <math.h>
#include <common.h>

void db_index_experiment_workload(size_t querries)
{
    DB_index *index;
    SSD *ssd;
    size_t i;
    double _sqrt_n = ceil(sqrt((double)querries));
    size_t sqrt_n = (size_t)_sqrt_n;
    size_t nlogn = querries * LOG2(querries);

    TRACE();

    ssd = ssd_create_samsung840();
    index = db_index_create(ssd, sizeof(long), 140);
    db_stat_reset();

    /* bukload N / 2 */
    db_stat_start_querry();
    db_index_bulkload(index, querries / 2);
    db_stat_finish_querry();

    /* sqrt(N) point search */
    for (i = 0; i < sqrt_n; ++i)
    {
        db_stat_start_querry();
        db_index_point_search(index, 1);
        db_stat_finish_querry();
    }

    /* Normal Insert N / 2 */
    for (i = 0; i < querries / 2; ++i)
    {
        db_stat_start_querry();
        db_index_insert(index, 1);
        db_stat_finish_querry();
    }

    /* NlogN range search with 10% selecivity */
    for (i = 0; i < nlogn; ++i)
    {
        db_stat_start_querry();
        db_index_range_search(index, (index->num_entries + 9) / 10);
        db_stat_finish_querry();
    }

    /* sqrt(N) delete */
    for (i = 0; i < sqrt_n; ++i)
    {
        db_stat_start_querry();
        db_index_delete(index, 1);
        db_stat_finish_querry();
    }

    /* sqrt(N) point search */
    for (i = 0; i < sqrt_n; ++i)
    {
        db_stat_start_querry();
        db_index_point_search(index, 1);
        db_stat_finish_querry();
    }

    /* sqrt(N) update */
    for (i = 0; i < sqrt_n; ++i)
    {
        db_stat_start_querry();
        db_index_update(index, 1);
        db_stat_finish_querry();
    }

    /* NlogN range search with 5% selectivity */
    for (i = 0; i < nlogn; ++i)
    {
        db_stat_start_querry();
        db_index_range_search(index, (index->num_entries + 19) / 20);
        db_stat_finish_querry();
    }

    db_stat_summary_print();
    db_index_destroy(index);
}