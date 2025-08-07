#ifndef LLULU_LU_BENCH_H
#define LLULU_LU_BENCH_H

#include <llulu/lu_defs.h>
#include <llulu/lu_rng.h>
#include <llulu/lu_error.h>
#include <llulu/lu_time.h>
#include <llulu/lu_math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static inline void lu_bench_preventopt(const void *p) {
    __asm__ __volatile__("" : : "g"(p) : "memory");
}

static inline void lu_bench_barrier()
{
    __asm__ __volatile__("" : : : "memory");
}

/* Use the return value if escaping data from function is needed.
 * The param ctx should be used for getting input values.
 * The function ctx_generator will be called before every iteration
 * (not benchmarked) in order to generate fresh input data for the
 * bench_target. It must return 0 if any error prevented the generation.
 */
typedef void* (*bench_target)(const void *ctx);
typedef int (*ctx_generator)(void *out_ctx);

typedef struct lu_bench_state {
    /* user config */
    const char *name;
    const char *notes;
    uint64_t seed;
    int64_t iterations;
    bench_target target_fn;
    ctx_generator ctx_generator_fn;
    void *ctx; /* bench_target input data */

    /* results */
    int64_t min_ns;
    int64_t max_ns;
    int64_t mean_ns;
    int64_t stddev_ns;
    char date[16];
    char time[16];
} lu_bench_state;

static inline void
lu_bench_process_results(lu_bench_state *bs, const int64_t *time_results)
{
    int64_t mean = 0;
    int64_t min = INT64_MAX;
    int64_t max = INT64_MIN;
    for (int i = 0; i < bs->iterations; i++) {
        int64_t ns = time_results[i];
        min = lu_minil(min, ns);
        max = lu_maxil(max, ns);
        mean += ns;
    }
    mean /= bs->iterations;

    double stddev = 0.0;
    for (int i = 0; i < bs->iterations; i++) {
        double dev = time_results[i] - mean;
        stddev += dev * dev;
    }

    stddev = lu_sqrtl(stddev / (bs->iterations - 1));

    bs->min_ns = min;
    bs->max_ns = max;
    bs->mean_ns = mean;
    bs->stddev_ns = (int64_t)stddev;

    lu_time_fmt_date(bs->date);
    lu_time_fmt_time(bs->time);
}

static inline lu_err
lu_bench_export_json(lu_bench_state *bs, char *out, size_t out_size)
{
    static const char fmt[] = {
      "{\n"
      "  \"name\": \"%s\",\n"
      "  \"notes\": \"%s\",\n"
      "  \"date\": \"%s %s\",\n"
      "  \"iterations\": %zu,\n"
      "  \"min\": %lld,\n"
      "  \"max\": %lld,\n"
      "  \"mean\": %lld,\n"
      "  \"stddev\": %lld\n"
      "}\n"
    };

    if (!out || !bs) {
        return LU_ERR_BADARG;
    }

    if (out_size < sizeof(fmt)) {
        return LU_ERR_MEM_BUF;
    }

    int ret = snprintf(out, out_size, fmt,
        bs->name, bs->notes, bs->date, bs->time, bs->iterations,
        (long long)bs->min_ns, (long long)bs->max_ns, (long long)bs->mean_ns, (long long)bs->stddev_ns);
    if (ret >= out_size) {
        return LU_ERR_MEM_BUF;
    }
    return ret >= out_size ? LU_ERR_MEM_BUF: LU_ERR_SUCCESS;
}

static inline lu_err
lu_bench_run(lu_bench_state *bs)
{
    lu_rng_seedg(bs->seed);
    int64_t *results = malloc(bs->iterations * sizeof(int64_t));
    if (!results) {
        return LU_ERR_MEM;
    }

    const int64_t n = bs->iterations;
    for (int i = 0; i < n; i++) {
        if (bs->ctx_generator_fn && bs->ctx) {
            if (!bs->ctx_generator_fn(bs->ctx)) {
                free(results);
                return LU_ERR_ERROR;
            }
        }

        const void *ctx = bs->ctx;
        int64_t start = lu_time_get();
        lu_bench_barrier();
        lu_bench_preventopt(bs->target_fn(ctx));
        lu_bench_barrier();
        results[i] = lu_time_elapsed(start);
    }

    lu_bench_process_results(bs, results);
    free(results);
    return LU_ERR_SUCCESS;
}

#endif /* LLULU_LU_BENCH_H */
