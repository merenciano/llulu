#ifndef LLULU_LU_TEST_H
#define LLULU_LU_TEST_H

#include <llulu/lu_log.h>
#include <stdint.h>
#include <stdlib.h>

#define LU_TEST_FAIL "\e[1:51:97:191mFAIL\e[0m"
#define LU_TEST_PASS "\e[1:51:97:102mPASS\e[0m"

#define LU_TEST_GROUP_BEGIN(NAME) lu_test_group_begin(NAME, __FILE__, __LINE__)
#define LU_TEST_UNIT(NAME, RES) lu_test_runit(NAME, RES, __PRETTY_FUNCTION__, __LINE__)

#define lu_log_test(...) LU_INTERNAL_LOG(LU_LOG_PANIC, __VA_ARGS__)

typedef void (*lu_bench_func)(void *buf, size_t bufsize);

static inline void lu_bench_preventopt(const void *p) {
    __asm__ __volatile__("" : : "g"(p) : "memory");
}

static inline void lu_test_group_begin(const char *name, const char *file, int line)
{
    lu_log_test("%s $$ %s(%d)", name, file, line);
}

static inline void lu_test_runit(const char *name, bool result, const char *func, int line)
{
    static const char * const res_to_str[] = {LU_TEST_FAIL, LU_TEST_PASS};

    lu_log_test("[%s] - %s", res_to_str[result], name);
    if (!result) {
        lu_log_test("  -> %s:%s(%d)", __FILE__, func, line);
        exit(1);
    }
}

#endif /* LLULU_LU_TEST_H */
