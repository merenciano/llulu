#include <llulu/lu_time.h>
#include <llulu/lu_defs.h>
#include "bench_sstr_data.h"
#include <stdio.h>
#include <stdint.h> 
#include <stdlib.h>

static const size_t ITERATIONS = 1UL << 25UL;

static void lu_sstr_bench()
{
    int idx = 0;
    lu_sstr str;
    bool cnd;
    uint64_t start = lu_time_get();

    for (size_t i = 0; i < ITERATIONS; ++i) {
        lu_sstr a = bench_data[idx];
        if (lu_sstr_cmpmem(&a, str.str, 17) < 0) {
            lu_sstr_copy(&a, str.str);
        }

	++idx;
	idx = idx & 15;

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
    printf("Elapsed test lu_str: %f sec. (%s, %d)\n", lu_time_sec(lapse), str.str, cnd);
}

int main(int argc, char **argv)
{
    lu_sstr_bench();
    return 0;
}
