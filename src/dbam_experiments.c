#include <dbam.h>
#include <experiments.h>
#include <log.h>
#include <dbstat.h>
#include <common.h>
#include <time.h>
#include <stdlib.h>
#include <genrand.h>

static ___unused___ void __debug_am(DB_AM *am);

static ___unused___ void __debug_am(DB_AM *am)
{
    size_t i;
    size_t j = 0;
    Partition *p;

    printf("AM table entries = %zu, entry size = %zu bs = %zu\n\n",
           am->num_entries, am->entry_size, am->sort_buffer_size);

    printf("INDEX entries = %zu\n", am->index->num_entries);
    printf("PART entries = %zu\n", am->set.num_entries);
    for_each_data(am->set.partitions, Darray, p)
    {
        printf("PARTITON[%zu] entries = %zu, blocks = %zu\n", j, p->num_entries, p->num_blocks);
        for (i = 0; i < p->num_blocks; ++i)
            if (p->blocks[i] != NULL)
                printf("\tBLOCK[%zu] valid = %zu\n", i, p->blocks[i]->valid_entries);
        ++j;
    }
}

void db_am_experiment_workload(size_t queries)
{
    DB_AM *am;
    SSD *ssd;
    size_t i;

    TRACE();

    sgenrand((unsigned long)time(NULL));

    ssd = ssd_create_samsung840();
    am = db_am_create(ssd, queries, sizeof(int), 140, 10);
    db_stat_reset();

    db_stat_start_query();
    db_am_search(am, QUERY_RANDOM, 100);
    db_stat_finish_query();

    for (i = 0; i < 10; ++i)
    {
        __debug_am(am);
        db_stat_start_query();
        db_am_search(am, QUERY_RANDOM, (size_t)((double)queries * 0.1));
        db_stat_current_print();
        db_stat_finish_query();
    }

    db_stat_summary_print();

    db_am_destroy(am);
    ssd_destroy(ssd);
}