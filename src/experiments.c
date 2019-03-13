#include <experiments.h>
#include <dbstat.h>
#include <dbraw.h>
#include <dbindex.h>
#include <dblam.h>
#include <ssd.h>
#include <common.h>
#include <log.h>
#include <unistd.h>
#include <fcntl.h>
#include <dbam.h>
#include <locale.h>

void experiment1(const char const *file, SSD *ssd, size_t key_size, size_t data_size, size_t entries, query_t type, double selectivity)
{
    DB_index *index;
    DB_raw *raw;
    DB_LAM *lam;
    size_t i;
    size_t bs;
    size_t mt;
    double ut;
    size_t mbr;
    int fd;
    double raw_time;
    double index_time;
    double lam_time;
    char file_name[1024];

    TRACE();

    index = db_index_create(ssd, key_size, data_size);
    raw = db_raw_create(ssd, data_size);

    bs = (size_t)((double)(data_size * entries) * 0.01) / ssd->block_size;
    mbr = bs * 3;
    mt = bs;
    ut = 0.05;

    lam = db_lam_create(ssd, entries, key_size, data_size, bs, mt, ut, mbr);
    printf("BS = %zu, MT = %zu, Ut = %lf, MBR = %zu\n", bs, mt, ut, mbr);

    db_index_bulkload(index, entries);
    db_raw_bulkload(raw, entries);

    /* time for search from index */
    db_stat_reset();
    db_stat_start_query();
    db_index_range_search(index, (size_t)((double)entries * selectivity));
    db_stat_finish_query();
    index_time = db_stat_get_current_time();
    db_stat_summary_print();

    /* time for search from raw */
    db_stat_reset();
    db_stat_start_query();
    db_raw_range_search(raw, (size_t)((double)entries * selectivity));
    db_stat_finish_query();
    raw_time = db_stat_get_current_time();
    db_stat_summary_print();

    db_stat_reset();
    snprintf(file_name, 1024, "%s_%s.txt", file, ssd->name);
    fd = open(file_name, O_CREAT | O_TRUNC | O_RDWR | O_APPEND, 0644);
    dprintf(fd, "Query\tIndex\tScan\tLAM\n");
    i = 0;
    do
    {
        db_stat_start_query();
        db_lam_search(lam, type, (size_t)((double)entries * selectivity));
        db_stat_finish_query();
        lam_time = db_stat_get_current_time();
        dprintf(fd, "%zu\t%lf\t%lf\t%lf\n", i, index_time, raw_time, lam_time);

        printf("QUERY %zu:\tLAM NUM = %zu\n", i, lam->set.num_entries);
        ++i;
    } while (lam->set.num_entries > 0);

    printf("AFTER %zu queries we have full index\n", i);
    db_stat_summary_print();

    db_index_destroy(index);
    db_raw_destroy(raw);
    db_lam_destroy(lam);
    close(fd);
}

void experiment2(const char const *file, SSD *ssd, size_t key_size, size_t data_size, size_t entries, query_t type, double selectivity)
{
    DB_index *index;
    DB_raw *raw;
    DB_LAM *lam;
    DB_AM *am;
    size_t i;
    size_t bs;
    size_t mt;
    double ut;
    size_t mbr;
    int fd;
    double raw_time;
    double index_time;
    double lam_time;
    double am_time;
    char file_name[1024];

    TRACE();

    index = db_index_create(ssd, key_size, data_size);
    raw = db_raw_create(ssd, data_size);

    bs = (size_t)((double)(data_size * entries) * 0.01) / ssd->block_size;
    mbr = bs * 3;
    mt = bs;
    ut = 0.05;

    lam = db_lam_create(ssd, entries, key_size, data_size, bs, mt, ut, mbr);
    printf("BS = %zu, MT = %zu, Ut = %lf, MBR = %zu\n", bs, mt, ut, mbr);

    am = db_am_create(ssd, entries, key_size, data_size, bs);

    db_index_bulkload(index, entries);
    db_raw_bulkload(raw, entries);

    /* time for search from index */
    db_stat_reset();
    db_stat_start_query();
    db_index_range_search(index, (size_t)((double)entries * selectivity));
    db_stat_finish_query();
    index_time = db_stat_get_current_time();
    db_stat_summary_print();

    /* time for search from raw */
    db_stat_reset();
    db_stat_start_query();
    db_raw_range_search(raw, (size_t)((double)entries * selectivity));
    db_stat_finish_query();
    raw_time = db_stat_get_current_time();
    db_stat_summary_print();

    db_stat_reset();
    snprintf(file_name, 1024, "%s_%s.txt", file, ssd->name);
    fd = open(file_name, O_CREAT | O_TRUNC | O_RDWR | O_APPEND, 0644);
    dprintf(fd, "Query\tIndex\tScan\tLAM\tAM\n");
    i = 0;
    do
    {
        db_stat_start_query();
        db_lam_search(lam, type, (size_t)((double)entries * selectivity));
        db_stat_finish_query();
        lam_time = db_stat_get_current_time();

        db_stat_start_query();
        db_am_search(am, type, (size_t)((double)entries * selectivity));
        db_stat_finish_query();
        am_time = db_stat_get_current_time();

        dprintf(fd, "%zu\t%lf\t%lf\t%lf\t%lf\n", i, index_time, raw_time, lam_time, am_time);
        printf("QUERY %zu:\tAM NUM = %zu\tLAM NUM = %zu\n", i, am->set.num_entries, lam->set.num_entries);

        ++i;
    } while (lam->set.num_entries > 0 || am->set.num_entries > 0);

    printf("AFTER %zu queries we have full index\n", i);
    db_stat_summary_print();

    db_index_destroy(index);
    db_raw_destroy(raw);
    db_lam_destroy(lam);
    db_am_destroy(am);
    close(fd);
}


void experiment3(const char const *file, SSD *ssd, size_t key_size, size_t data_size, size_t entries, query_t type, double selectivity, size_t queries)
{
    DB_LAM *lam;
    size_t bs;
    size_t mt;
    double ut;
    size_t mbr;
    int fd;
    char file_name[1024];
    double frac = 0.001;
    size_t i;

    TRACE();

    bs = (size_t)((double)(data_size * entries) * 0.01) / ssd->block_size;
    mbr = bs * 3;
    mt = bs;
    ut = 0.05;

    snprintf(file_name, 1024, "%s_%s.txt", file, ssd->name);
    fd = open(file_name, O_CREAT | O_TRUNC | O_RDWR | O_APPEND, 0644);
    setlocale(LC_ALL, "Polish");
    dprintf(fd, "Buffer size\tTotal time\n");
    while (frac < 0.1)
    {
        bs = (size_t)((double)(data_size * entries) * frac) / ssd->block_size;
        lam = db_lam_create(ssd, entries, key_size, data_size, bs, mt, ut, mbr);
        printf("BS = %zu, MT = %zu, Ut = %lf, MBR = %zu\n", bs, mt, ut, mbr);

        db_stat_reset();
        for (i = 0; i < queries; ++i)
        {
            db_stat_start_query();
            db_lam_search(lam, type, (size_t)((double)entries * selectivity));
            db_stat_finish_query();
        }

        dprintf(fd, "%.3lf\t%lf\n", frac, db_stat_get_total_time());
        db_lam_destroy(lam);
        frac += 0.001;
    }

    close(fd);
}

void experiment4(const char const *file, SSD *ssd, size_t key_size, size_t data_size, size_t entries, query_t type, double selectivity, size_t queries)
{
    DB_LAM *lam;
    size_t bs;
    size_t mt;
    double ut;
    size_t mbr;
    int fd_time;
    int fd_mem;
    char file_time_name[1024];
    char file_mem_name[1024];
    size_t i;
    double frac = 0.001;
    size_t mem;

    TRACE();

    bs = (size_t)((double)(data_size * entries) * 0.01) / ssd->block_size;
    ut = 0.00;

    snprintf(file_time_name, 1024, "%s_%s_time.txt", file, ssd->name);
    snprintf(file_mem_name, 1024, "%s_%s_memory.txt", file, ssd->name);
    fd_time = open(file_time_name, O_CREAT | O_TRUNC | O_RDWR | O_APPEND, 0644);
    fd_mem = open(file_mem_name, O_CREAT | O_TRUNC | O_RDWR | O_APPEND, 0644);
    setlocale(LC_ALL, "Polish");
    dprintf(fd_time, "Merge Treshold\tMax time\tTotal time\n");
    dprintf(fd_mem, "Merge Treshold\tMax memory\n");
    while (frac < 0.1)
    {
        mt = (size_t)((double)(data_size * entries) * frac) / ssd->block_size;
        mbr = (data_size * entries) / ssd->block_size;
        lam = db_lam_create(ssd, entries, key_size, data_size, bs, mt, ut, mbr);
        printf("BS = %zu, MT = %zu, Ut = %lf, MBR = %zu\n", bs, mt, ut, mbr);

        db_stat_reset();
        for (i = 0; i < queries; ++i)
        {
            db_stat_start_query();
            db_lam_search(lam, type, (size_t)((double)entries * selectivity));
            db_stat_finish_query();
        }

        mem = db_stat_get_max_mem_usage();
        dprintf(fd_time, "%.3lf\t%lf\t%lf\n", frac, db_stat_get_max_time(), db_stat_get_total_time());
        dprintf(fd_mem, "%.3lf\t%lf\n", frac, (double)mem / (double)(data_size * entries));
        db_lam_destroy(lam);
        frac += 0.001;
    }

    close(fd_time);
    close(fd_mem);
}

void experiment5(const char const *file, SSD *ssd, size_t key_size, size_t data_size, size_t entries, query_t type, double selectivity[], size_t selectivity_len, size_t queries)
{
    DB_LAM *lam;
    DB_AM *am;
    size_t i;
    size_t j;
    size_t bs;
    size_t mt;
    double ut;
    size_t mbr;
    int fd;
    double lam_total_time;
    double lam_max_time;
    char file_name[1024];

    TRACE();

    snprintf(file_name, 1024, "%s_%s.txt", file, ssd->name);
    fd = open(file_name, O_CREAT | O_TRUNC | O_RDWR | O_APPEND, 0644);
    setlocale(LC_ALL, "Polish");
    dprintf(fd, "Selectivity\tLAM Time\tAM Time\tLAM Max time\tAM Max time\n");
    for (i = 0; i < selectivity_len; ++i)
    {
        bs = (size_t)((double)(data_size * entries) * 0.01) / ssd->block_size;
        mbr = bs * 3;
        mt = bs;
        ut = 0.05;

        lam = db_lam_create(ssd, entries, key_size, data_size, bs, mt, ut, mbr);
        printf("BS = %zu, MT = %zu, Ut = %lf, MBR = %zu\n", bs, mt, ut, mbr);

        am = db_am_create(ssd, entries, key_size, data_size, bs);

        db_stat_reset();
        for (j = 0; j < queries; ++j)
        {
            db_stat_start_query();
            db_lam_search(lam, type, (size_t)((double)entries * selectivity[i]));
            db_stat_finish_query();
        }
        lam_max_time = db_stat_get_max_time();
        lam_total_time = db_stat_get_total_time();

        db_stat_reset();
        for (j = 0; j < queries; ++j)
        {
            db_stat_start_query();
            db_am_search(am, type, (size_t)((double)entries * selectivity[i]));
            db_stat_finish_query();
        }
        dprintf(fd, "%.2lf\t%lf\t%lf\t%lf\t%lf\n", selectivity[i], lam_total_time, db_stat_get_total_time(), lam_max_time, db_stat_get_max_time());


        db_am_destroy(am);
        db_lam_destroy(lam);
    }
    close(fd);
}