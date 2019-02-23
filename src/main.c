#include <log.h>
#include <common.h>
#include <experiments.h>
#include <stdbool.h>
___before_main___(0) void init(void);
___after_main___(0) void deinit(void);

___before_main___(0) void init(void)
{
	(void)log_init(stdout, LOG_TO_FILE);
}

___after_main___(0) void deinit(void)
{
	log_deinit();
}

int main(void)
{
    // db_index_experiment_workload(1000000);
    // db_raw_experiment_workload(1000000);
    // db_lam_experiment_workload(1000000);
    experiment1(sizeof(int), 140, 10000000, QUERRY_RANDOM, 0.1);
    return 0;
}