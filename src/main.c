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
    // db_am_experiment_workload(100000);
    experiment1("experiment1_1.txt", sizeof(int), 140, 10000000, QUERRY_RANDOM, 0.05);
    experiment1("experiment1_2.txt", sizeof(int), 140, 10000000, QUERRY_ALWAYS_NEW, 0.05);
    experiment1("experiment1_3.txt", sizeof(int), 140, 10000000, QUERRY_SEQUENTIAL_PATTERN, 0.05);
    experiment2("experiment2_1.txt", sizeof(int), 140, 10000000, QUERRY_RANDOM, 0.05);
    experiment2("experiment2_2.txt", sizeof(int), 140, 10000000, QUERRY_ALWAYS_NEW, 0.05);
    experiment2("experiment2_3.txt", sizeof(int), 140, 10000000, QUERRY_SEQUENTIAL_PATTERN, 0.05);

    return 0;
}