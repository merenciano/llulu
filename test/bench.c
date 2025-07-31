#include <llulu/lu_time.h>
#include <llulu/lu_defs.h>
#include "bench_sstr_data.h"
#include <stdio.h>
#include <stdint.h> 
#include <stdlib.h>

static const size_t ITERATIONS = 2048UL << 22UL;

const char *g_commit_sha;

typedef struct bench_entry {
    char name[128];
    char date[32];
    int iterations;
    int64_t min_ns;
    int64_t max_ns;
    int64_t mean_ns;
    int64_t stddev_ns;
} bench_entry;

int main(void)
{
    srand(1);
    int idx = rand() % LU_ARR_SIZE(bench_data);
    lu_sstr str;
    bool cnd;
    uint64_t start = lu_time_get();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        lu_sstr a = bench_data[idx];
        if (lu_sstr_cmpmem(&a, str.str, 17) < 0) {
            lu_sstr_copy(&a, str.str);
        }

        idx = (idx + 1) % LU_ARR_SIZE(bench_data);
        lu_sstr b = bench_data[idx];


        if (!lu_sstr_equal(&a, &b)) {
            a = b;
        } else  {
            lu_sstr_clear(&a);
        }

        cnd = lu_sstr_cmpstr(&a, b.str);
        if (cnd || lu_sstr_empty(&a)) {
            str = a;
        } else {
            str = b;
        }
    }

    uint64_t lapse = lu_time_elapsed(start);
    printf("Elapsed test 1: %lu ns. (%s, %d)\n", lapse, str.str, cnd);

    start = lu_time_get();
    for (size_t i = 0; i < ITERATIONS; ++i) {
        lu_sstr a = bench_data[idx];
        if (lu_sstr_cmpmem_0(&a, str.str, 17) < 0) {
            lu_sstr_copy_0(&a, str.str);
        }

        idx = (idx + 1) % LU_ARR_SIZE(bench_data);
        lu_sstr b = bench_data[idx];


        if (!lu_sstr_equal_0(&a, &b)) {
            a = b;
        } else  {
            lu_sstr_clear(&a);
        }

        cnd = lu_sstr_cmpstr_0(&a, b.str);
        if (cnd || lu_sstr_empty(&a)) {
            str = a;
        } else {
            str = b;
        }
    }

    lapse = lu_time_elapsed(start);
    printf("Elapsed test 1: %lu ns. (%s, %d)\n", lapse, str.str, cnd);

    return 0;
}
