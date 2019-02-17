#include <dblam.h>
#include <experiments.h>
#include <log.h>
#include <dbstat.h>
#include <common.h>
#include <time.h>
#include <stdlib.h>
#include <genrand.h>

static ___unused___ void __debug_lam(DB_LAM *lam);

static ___unused___ void __debug_lam(DB_LAM *lam)
{
    size_t i;
    size_t j = 0;
    Partition *p;

    printf("LAM table entries = %zu, entry size = %zu bs = %zu, mt = %zu\n\n",
           lam->num_entries, lam->entry_size, lam->sort_buffer_size, lam->merge_treshold);

    printf("INDEX entries = %zu\n", lam->index->num_entries);
    printf("PART entries = %zu\n", lam->set.num_entries);
    for_each_data(lam->set.partitions, Darray, p)
    {
        printf("PARTITON[%zu] entries = %zu, blocks = %zu\n", j, p->num_entries, p->num_blocks);
        for (i = 0; i < p->num_blocks; ++i)
            if (p->blocks[i] != NULL)
                printf("\tBLOCK[%zu] valid = %zu, invalid = %zu\n", i, p->blocks[i]->valid_entries, p->blocks[i]->invalid_entries);
        ++j;
    }
}

void db_lam_experiment_workload(size_t querries)
{
    DB_LAM *lam;
    SSD *ssd;
    size_t i;

    TRACE();

    sgenrand((unsigned long)time(NULL));

    ssd = ssd_create_samsung840();
    lam = db_lam_create(ssd, querries, sizeof(int), 140, 10, 4);
    db_stat_reset();

    db_stat_start_querry();
    db_lam_search(lam, QUERRY_RANDOM, 100);
    db_stat_finish_querry();

    for (i = 0; i < 100; ++i)
    {
        db_stat_start_querry();
        db_lam_search(lam, QUERRY_RANDOM, querries / 1000);
        db_stat_finish_querry();
    }

    for (i = 0; i < 100; ++i)
    {
        db_stat_start_querry();
        db_lam_insert(lam, querries / 1000);
        db_stat_finish_querry();
    }

    for (i = 0; i < 100; ++i)
    {
        db_stat_start_querry();
        db_lam_delete(lam, querries / 1000);
        db_stat_finish_querry();
    }

    for (i = 0; i < 100; ++i)
    {
        db_stat_start_querry();
        db_lam_update(lam, querries / 1000);
        db_stat_finish_querry();
    }

    for (i = 0; i < 100; ++i)
    {
        db_stat_start_querry();
        db_lam_search(lam, QUERRY_RANDOM, querries / 1000);
        db_stat_finish_querry();
    }

    db_stat_summary_print();

    db_lam_destroy(lam);
}