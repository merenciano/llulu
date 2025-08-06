#include "lu_bench.h"
#include <llulu/lu_str.h>
#include <llulu/lu_time.h>
#include <llulu/lu_log.h>
#include <llulu/lu_rng.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char **argv)
{


    lu_bench_state bs = {
        .name = "sstr generic bench",
        .iterations = 1000,
        .seed = 14
    };

    lu_rng_seedg(bs.seed);
    int64_t *results = malloc(bs.iterations * sizeof(int64_t));
    assert(results);

    for (int i = 0; i < bs.iterations; i++) {
        char bench_data[6 * 32] = {0};
        for (int j = 0; j < 6; ++j) {
            lu_rng_textg(&bench_data[j * 32], 31);
        }

        int64_t start = lu_time_get();
        lu_bench_barrier();
        lu_sstr s1;
        lu_sstr s2;
        lu_sstr s3;
        lu_sstr_copy(&s1, bench_data);
        lu_sstr_copy(&s2, bench_data + 32 * 1);
        lu_sstr_copy(&s3, bench_data + 32 * 2);
        int cmp = lu_sstr_cmpstr(&s1, bench_data + 32 * 3);
        cmp += lu_sstr_cmpstr(&s2, bench_data + 32 * 4);
        cmp += lu_sstr_cmpstr(&s3, bench_data + 32 * 5);

        lu_bench_barrier();
        results[i] = lu_time_elapsed(start);
        if (results[i] == 0) printf("EPERPRPERPR\n");
        lu_bench_preventopt(&s1);
        lu_bench_preventopt(&s2);
        lu_bench_preventopt(&s3);
        lu_bench_preventopt(&cmp);
    }

    double mean = 0.0;
    int64_t min = INT64_MAX;
    int64_t max = INT64_MIN;
    for (int i = 0; i < bs.iterations; i++) {
        if (results[i] < min) {
            min = results[i];
        }
        if (results[i] > max) {
            max = results[i];
        }

        mean += (double)results[i];
    }
    mean /= bs.iterations;

    double stddev = 0.0;

    for (int i = 0; i < bs.iterations; i++) {
        stddev += ((double)results[i] - mean) * ((double)results[i] - mean);
    }

    stddev = sqrt(stddev / (bs.iterations - 1));

    bs.min_ns = min;
    bs.max_ns = max;
    bs.mean_ns = (int64_t)mean;
    bs.stddev_ns = (int64_t)stddev;

    char buf[1024 * 8];
    lu_bench_export_json(&bs, buf, sizeof(buf));

    printf("%s\n", buf);

    return 0;
}
