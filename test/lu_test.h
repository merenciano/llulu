#ifndef LLULU_LU_TEST_H
#define LLULU_LU_TEST_H

#include <llulu/lu_log.h>
#include <llulu/lu_time.h>
#include <llulu/lu_rng.h>

#define LU_TEST_TAG "LU_TEST"
#define LU_TEST_FAIL "\e[1:51:97:101mFAIL\e[0m"
#define LU_TEST_PASS "\e[1:51:97:102mPASS\e[0m"

#define LU_TEST_UNIT(NAME, RES) lu_test_runit(NAME, RES, __FILE__, __LINE__)

#define lu_log_test(...) LU_INTERNAL_LOG(LU_LOG_CUSTOM, __VA_ARGS__)

typedef struct lu_test_state {
    const char *name;
    void (*test_fail_callback)(struct lu_test_state *s, const char *name);
    uint64_t rng_seed;
    lu_timestamp start_time;
    bool failed;
} lu_test_state;

static inline void lu_test_group(lu_test_state *state, const char *name, void (*test_group_fn)(lu_test_state*))
{
    lu_log_test("Group %s started", name);
    lu_log_set_custom_tag(name);
    lu_rng_seedg(state->rng_seed);
    lu_timestamp start = lu_time_get();

    test_group_fn(state);

    lu_log_set_custom_tag(LU_TEST_TAG);
    lu_log_test("Group %s finished in %zu ms.", name, lu_time_ms(lu_time_elapsed(start)));
}

static inline void lu_test_check(lu_test_state *state, const char *name, bool result)
{
    static const char * const res_to_str[] = {LU_TEST_FAIL, LU_TEST_PASS};

    lu_log_test("[%s] - %s", res_to_str[result], name);
    if (!result && state->test_fail_callback) {
        state->failed = true;
        state->test_fail_callback(state, name);
    }
}

static inline void lu_test_begin(lu_test_state *state)
{
    lu_log_set_custom_tag(LU_TEST_TAG);
    lu_log_setopt(LU_LOG_COLOR, true);
    state->start_time = lu_time_get();
    state->failed = false;
    char datestr[16];
    char timestr[16];
    lu_time_fmt_date(datestr);
    lu_time_fmt_time(timestr);
    lu_log_test("**************************");
    lu_log_test("**  Test Suite Started  **");
    lu_log_test("**  test: %s", state->name);
    lu_log_test("**  file: %s", __FILE__);
    lu_log_test("**  date: %s", datestr);
    lu_log_test("**  time: %s", timestr);
    lu_log_test("**************************");
    lu_log_test("");
}

static inline int lu_test_end(lu_test_state *state)
{
    float suite_duration_sec = lu_time_sec(lu_time_elapsed(state->start_time));
    lu_log_test("");
    lu_log_test("************************");
    lu_log_test("**  Test Suite Ended  **");
    lu_log_test("**  test: %s", state->name);
    lu_log_test("**  result: %s", state->failed ? LU_TEST_FAIL : LU_TEST_PASS);
    lu_log_test("**  duration: %0.2fs", suite_duration_sec);
    lu_log_test("************************");
    return state->failed;
}

#endif /* LLULU_LU_TEST_H */
