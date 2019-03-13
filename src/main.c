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
    SSD *samsung = ssd_create_samsung840();
    SSD *intel = ssd_create_intelDCP4511();
    SSD *toshiba = ssd_create_toshibaVX500();
    // db_index_experiment_workload(1000000);
    // db_raw_experiment_workload(1000000);
    // db_lam_experiment_workload(1000000);
    // db_am_experiment_workload(100000);
    experiment1("experiment1_1", samsung, sizeof(int), 140, 10000000, QUERY_RANDOM, 0.05);
    experiment1("experiment1_2", samsung, sizeof(int), 140, 10000000, QUERY_ALWAYS_NEW, 0.05);
    experiment1("experiment1_3", samsung, sizeof(int), 140, 10000000, QUERY_SEQUENTIAL_PATTERN, 0.05);
    experiment2("experiment2_1", samsung, sizeof(int), 140, 10000000, QUERY_RANDOM, 0.05);
    experiment2("experiment2_2", samsung, sizeof(int), 140, 10000000, QUERY_ALWAYS_NEW, 0.05);
    experiment2("experiment2_3", samsung, sizeof(int), 140, 10000000, QUERY_SEQUENTIAL_PATTERN, 0.05);

    experiment1("experiment1_1", intel, sizeof(int), 140, 10000000, QUERY_RANDOM, 0.05);
    experiment1("experiment1_2", intel, sizeof(int), 140, 10000000, QUERY_ALWAYS_NEW, 0.05);
    experiment1("experiment1_3", intel, sizeof(int), 140, 10000000, QUERY_SEQUENTIAL_PATTERN, 0.05);
    experiment2("experiment2_1", intel, sizeof(int), 140, 10000000, QUERY_RANDOM, 0.05);
    experiment2("experiment2_2", intel, sizeof(int), 140, 10000000, QUERY_ALWAYS_NEW, 0.05);
    experiment2("experiment2_3", intel, sizeof(int), 140, 10000000, QUERY_SEQUENTIAL_PATTERN, 0.05);

    experiment1("experiment1_1", toshiba, sizeof(int), 140, 10000000, QUERY_RANDOM, 0.05);
    experiment1("experiment1_2", toshiba, sizeof(int), 140, 10000000, QUERY_ALWAYS_NEW, 0.05);
    experiment1("experiment1_3", toshiba, sizeof(int), 140, 10000000, QUERY_SEQUENTIAL_PATTERN, 0.05);
    experiment2("experiment2_1", toshiba, sizeof(int), 140, 10000000, QUERY_RANDOM, 0.05);
    experiment2("experiment2_2", toshiba, sizeof(int), 140, 10000000, QUERY_ALWAYS_NEW, 0.05);
    experiment2("experiment2_3", toshiba, sizeof(int), 140, 10000000, QUERY_SEQUENTIAL_PATTERN, 0.05);

    experiment3("experiment3_1", samsung, sizeof(int), 140, 10000000, QUERY_RANDOM, 0.05, 100);
    experiment3("experiment3_1", intel, sizeof(int), 140, 10000000, QUERY_RANDOM, 0.05, 100);
    experiment3("experiment3_1", toshiba, sizeof(int), 140, 10000000, QUERY_RANDOM, 0.05, 100);

    experiment4("experiment4_1", samsung, sizeof(int), 140, 10000000, QUERY_RANDOM, 0.05, 100);
    experiment4("experiment4_1", intel, sizeof(int), 140, 10000000, QUERY_RANDOM, 0.05, 100);
    experiment4("experiment4_1", toshiba, sizeof(int), 140, 10000000, QUERY_RANDOM, 0.05, 100);

    experiment5("experiment5_1", samsung, sizeof(int), 140, 10000000, QUERY_RANDOM, (double[3]){0.05, 0.1, 0.2}, 3, 100);
    experiment5("experiment5_1", intel, sizeof(int), 140, 10000000, QUERY_RANDOM, (double[3]){0.05, 0.1, 0.2}, 3, 100);
    experiment5("experiment5_1", toshiba, sizeof(int), 140, 10000000, QUERY_RANDOM, (double[3]){0.05, 0.1, 0.2}, 3, 100);

    ssd_destroy(samsung);
    ssd_destroy(intel);
    ssd_destroy(toshiba);

    return 0;
}