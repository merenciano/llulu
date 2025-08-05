#ifndef LLULU_LU_BENCH_H
#define LLULU_LU_BENCH_H

static inline void lu_bench_preventopt(const void *p) {
    __asm__ __volatile__("" : : "g"(p) : "memory");
}

#endif /* LLULU_LU_BENCH_H */
