#include <llulu/lu_time.h>
#include <llulu/lu_defs.h>
#include "bench_sstr_data.h"
#include <stdio.h>
#include <stdint.h> 
#include <stdlib.h>

#include <string>


static const size_t ITERATIONS = 2048UL << 22UL;

static void lu_sstr_bench()
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
    printf("Elapsed test 1: %llu ns. (%s, %d)\n", lapse, str.str, cnd);

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
    printf("Elapsed test 2: %llu ns. (%s, %d)\n", lapse, str.str, cnd);

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
    printf("Elapsed test 3: %llu ns. (%s, %d)\n", lapse, str.str, cnd);
}

static void cpp_string_bench()
{
    srand(1);
    int idx = rand() % LU_ARR_SIZE(bench_data);
    std::string str;
    bool cnd;
    uint64_t start = lu_time_get();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        std::string a = bench_data[idx].str;
        if (a != str) {
            a = str;
        }

        idx = (idx + 1) % LU_ARR_SIZE(bench_data);
        std::string b = bench_data[idx].str;


        if (a != b) {
            a = b;
        } else  {
            a.clear();
        }

        cnd = (a == b);
        if (cnd || a.empty()) {
            str = a;
        } else {
            str = b;
        }
    }

    uint64_t lapse = lu_time_elapsed(start);
    printf("Elapsed test 4: %llu ns. (%s, %d)\n", lapse, str.c_str(), cnd);

    start = lu_time_get();
    for (size_t i = 0; i < ITERATIONS; ++i) {
        std::string a = bench_data[idx].str;
        if (a != str) {
            a = str;
        }

        idx = (idx + 1) % LU_ARR_SIZE(bench_data);
        std::string b = bench_data[idx].str;


        if (a != b) {
            a = b;
        } else  {
            a.clear();
        }

        cnd = (a ==  b);
        if (cnd || a.empty()) {
            str = a;
        } else {
            str = b;
        }
    }

    lapse = lu_time_elapsed(start);
    printf("Elapsed test 5: %llu ns. (%s, %d)\n", lapse, str.c_str(), cnd);

    start = lu_time_get();
    for (size_t i = 0; i < ITERATIONS; ++i) {
        std::string a = bench_data[idx].str;
        if (a != str) {
            a = str;
        }

        idx = (idx + 1) % LU_ARR_SIZE(bench_data);
        std::string b = bench_data[idx].str;


        if (a != b) {
            a = b;
        } else  {
            a.clear();
        }

        cnd = (a == b);
        if (cnd || a.empty()) {
            str = a;
        } else {
            str = b;
        }
    }

    lapse = lu_time_elapsed(start);
    printf("Elapsed test 6: %llu ns. (%s, %d)\n", lapse, str.c_str(), cnd);
}

int main(void)
{
    cpp_string_bench();

    lu_sstr_bench();

    return 0;
}
