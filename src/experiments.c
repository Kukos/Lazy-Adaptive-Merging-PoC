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

void experiment1(const char const *file, size_t key_size, size_t data_size, size_t entries, querry_t type, double selectivity)
{
    SSD *ssd;
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

    TRACE();

    ssd = ssd_create_samsung840();
    index = db_index_create(ssd, key_size, data_size);
    raw = db_raw_create(ssd, data_size);

    bs = (size_t)((double)(data_size * entries) * 0.01) / ssd->block_size;
    mbr = bs * 2;
    mt = bs;
    ut = 0.05;

    lam = db_lam_create(ssd, entries, key_size, data_size, bs, mt, ut, mbr);
    printf("BS = %zu, MT = %zu, Ut = %lf, MBR = %zu\n", bs, mt, ut, mbr);

    db_index_bulkload(index, entries);
    db_raw_bulkload(raw, entries);

    /* time for search from index */
    db_stat_reset();
    db_stat_start_querry();
    db_index_range_search(index, (size_t)((double)entries * selectivity));
    db_stat_finish_querry();
    index_time = db_stat_get_current_time();
    db_stat_summary_print();

    /* time for search from raw */
    db_stat_reset();
    db_stat_start_querry();
    db_raw_range_search(raw, (size_t)((double)entries * selectivity));
    db_stat_finish_querry();
    raw_time = db_stat_get_current_time();
    db_stat_summary_print();

    db_stat_reset();
    fd = open(file, O_CREAT | O_TRUNC | O_RDWR | O_APPEND, 0644);
    dprintf(fd, "Querry\tFull index\tUnsorted table\tLazy Adaptive Merging\n");
    i = 0;
    do
    {
        db_stat_start_querry();
        db_lam_search(lam, type, (size_t)((double)entries * selectivity));
        db_stat_finish_querry();
        lam_time = db_stat_get_current_time();
        dprintf(fd, "%zu\t%lf\t%lf\t%lf\n", i, index_time, raw_time, lam_time);


        ++i;
    } while (lam->set.num_entries > 0);

    printf("AFTER %zu querries we have full index\n", i);
    db_stat_summary_print();

    ssd_destroy(ssd);
    db_index_destroy(index);
    db_raw_destroy(raw);
    db_lam_destroy(lam);
    close(fd);
}