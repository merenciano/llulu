#ifndef LLULU_LU_BENCH_H
#define LLULU_LU_BENCH_H

#include <stdint.h>
#include <stdio.h>

#define LU_DISABLE_ASSERTS
#define LU_DISABLE_CONTRACTS

typedef struct lu_bench_state {
    const char *name;
    char date[32];
    const char *commit_sha;
    uint64_t seed;
    int64_t iterations;
    int64_t min_ns;
    int64_t max_ns;
    int64_t mean_ns;
    int64_t stddev_ns;
    const char *notes;
} lu_bench_state;

static inline void lu_bench_export_json(lu_bench_state *bs, char *out, size_t out_size)
{
    const char *fmt = "{\n  \"benchmark_name\": \"%s\",\n  \"date\": \"%s\",\n  \"iterations\": %zu,\n  \"min\": %lld,\n  \"max\": %lld,\n  \"mean\": %lld,\n  \"stddev\": %lld\n}\n";
    int ret = snprintf(out, out_size, fmt, bs->name, bs->date, bs->iterations, bs->min_ns, bs->max_ns, bs->mean_ns, bs->stddev_ns);
    if (ret > out_size) {
        printf("ERROR");
    }
}

static inline void lu_bench_preventopt(const void *p) {
    __asm__ __volatile__("" : : "g"(p) : "memory");
}

static inline void lu_bench_barrier()
{
    __asm__ __volatile__("" : : : "memory");
}

#endif /* LLULU_LU_BENCH_H */
