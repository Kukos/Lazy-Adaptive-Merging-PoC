#include <log.h>
#include <ssd.h>
#include <stdlib.h>

#define SSD_MICROSEC(n) ((n) / 1000000.0)

SSD *ssd_create_samsung840(void)
{
    SSD *ssd;

    TRACE();

    ssd = (SSD *)malloc(sizeof(SSD));
    if (ssd == NULL)
        ERROR("malloc error\n", NULL);

    ssd->block_size = 64 * 8192;
    ssd->dirty_pages = 0;
    ssd->page_size = 8192;
    ssd->r_read_time = SSD_MICROSEC(20);
    ssd->r_write_time = SSD_MICROSEC(22);
    ssd->s_read_time = SSD_MICROSEC(14);
    ssd->s_write_time = SSD_MICROSEC(16);
    ssd->erase_time = SSD_MICROSEC(220) * 64;

    return ssd;
}

SSD *ssd_create_intelDCP4511(void)
{
    SSD *ssd;

    TRACE();

    ssd = (SSD *)malloc(sizeof(SSD));
    if (ssd == NULL)
        ERROR("malloc error\n", NULL);

    ssd->block_size = 64 * 4096;
    ssd->dirty_pages = 0;
    ssd->page_size = 4096;
    ssd->r_read_time = SSD_MICROSEC(3.9);
    ssd->r_write_time = SSD_MICROSEC(32.3);
    ssd->s_read_time = SSD_MICROSEC(2.2);
    ssd->s_write_time = SSD_MICROSEC(4);
    ssd->erase_time = SSD_MICROSEC(300) * 64;

    return ssd;
}

SSD *ssd_create_toshibaVX500(void)
{
    SSD *ssd;

    TRACE();

    ssd = (SSD *)malloc(sizeof(SSD));
    if (ssd == NULL)
        ERROR("malloc error\n", NULL);

    ssd->block_size = 64 * 4096;
    ssd->dirty_pages = 0;
    ssd->page_size = 4096;
    ssd->r_read_time = SSD_MICROSEC(10.8);
    ssd->r_write_time = SSD_MICROSEC(15.3);
    ssd->s_read_time = SSD_MICROSEC(7.2);
    ssd->s_write_time = SSD_MICROSEC(7.8);
    ssd->erase_time = SSD_MICROSEC(280) * 64;

    return ssd;
}