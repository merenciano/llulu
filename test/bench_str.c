#include "lu_bench.h"
#include <llulu/lu_str.h>
#include <llulu/lu_time.h>
#include <llulu/lu_log.h>
#include <llulu/lu_rng.h>
#include <stdint.h> 

int main(int argc, char **argv)
{
    uint64_t start = lu_time_get();

    lu_rng_seedg(14);

    lu_sstr bench_data[3];
    for (int i = 0; i < 3; ++i) {
        lu_sstr_clear(&bench_data[i]);
        lu_rng_textg(bench_data[i].str, 31);
    }

    lu_sstr str = bench_data[0];
    lu_sstr a = bench_data[1];
    int cmp = lu_sstr_cmpmem(&a, str.str, 17);
    lu_sstr_copy(&a, str.str);
    lu_sstr b = bench_data[2];

    if (!lu_sstr_equal(&a, &b)) {
        a = b;
    } else  {
        lu_sstr_clear(&a);
    }

    if (lu_sstr_cmpstr(&a, b.str) || lu_sstr_empty(&a)) {
        str = a;
    } else {
        str = b;
    }

    uint64_t lapse = lu_time_elapsed(start);
    lu_log("Elapsed test lu_str: %zu.\n", lapse);
    lu_bench_preventopt(&str);
    lu_bench_preventopt(&a);
    lu_bench_preventopt(&b);
    lu_bench_preventopt(&cmp);

    return 0;
}
