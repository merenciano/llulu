#include <llulu/lu_str.h>
#include <llulu/lu_time.h>
#include <llulu/lu_test.h>

void test_group_sstr(void)
{
    LU_TEST_GROUP_BEGIN("sstr");
    lu_timestamp start = lu_time_get();

    const lu_sstr SSTR_EMPTY = {.str = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }};
    lu_sstr s1 = {"kokoko"};
    lu_sstr s2 = {"HOLA"};

    LU_TEST_UNIT("NotEmpty", !lu_sstr_empty(&s1));

    lu_sstr_clear(&s1);
    LU_TEST_UNIT("Clear", !memcmp(s1.str, SSTR_EMPTY.str, LU_SSTRLEN));
    LU_TEST_UNIT("Empty", lu_sstr_empty(&s1));
    LU_TEST_UNIT("NotEqual", !lu_sstr_equal(&s1, &s2));
    LU_TEST_UNIT("NotEqual_0", !lu_sstr_equal_0(&s1, &s2));
    s1 = s2;
    LU_TEST_UNIT("Equal", lu_sstr_equal(&s1, &s2));
    LU_TEST_UNIT("Equal_0", lu_sstr_equal_0(&s1, &s2));
    const char *largestr = "SFEFHOWEINFOIWEOFNWEOIFNOWIENGOIWENFOIWENFIOWEOGINWOEING";
    lu_sstr_copy(&s1, largestr);
    LU_TEST_UNIT("Assign string literal longer than buffer", s1.str[30] && !s1.str[31]);
    s2 = s1;
    lu_sstr_copy_0(&s2, s1.str);
    lu_sstr_copy(&s1, "pepa");
    LU_TEST_UNIT("StringCopy", !strcmp(s1.str, "pepa"));
    LU_TEST_UNIT("StringCopy 2", !memcmp(s1.str, (char[32]){"pepa"}, 32));
    LU_TEST_UNIT("StringCopy_0", (bool)strcmp(s2.str, "pepa"));
    lu_sstr_copy_0(&s2, s1.str);
    LU_TEST_UNIT("StringCopy_0 + Equal", lu_sstr_equal(&s1, &s2));
    lu_sstr s3 = {"aowdnoqwndo"};
    lu_sstr_memcpy(&s1, &s3, 235);
    lu_sstr_memcpy_0(&s2, &s3, LU_SSTRLEN);
    LU_TEST_UNIT("MemCopy", lu_sstr_equal(&s1, &s3));
    LU_TEST_UNIT("MemCopy_0", lu_sstr_equal(&s2, &s3));
    LU_TEST_UNIT("MemCopy 2", lu_sstr_equal(&s2, &s1));
    s1 = (lu_sstr){"teteteplplp"};
    s2 = (lu_sstr){"tetetetasfve"};
    LU_TEST_UNIT("StringCompare", lu_sstr_cmpstr(&s1, s2.str) == strcmp(s1.str, s2.str));
    LU_TEST_UNIT("StringCompare_0", lu_sstr_cmpstr_0(&s1, s2.str) == strcmp(s1.str, s2.str));
    LU_TEST_UNIT("MemoryCompare", lu_sstr_cmpmem(&s1, s2.str, 3) == strncmp(s1.str, s2.str, 3));
    LU_TEST_UNIT("MemoryCompare_0", lu_sstr_cmpmem_0(&s1, s2.str, 3) == strncmp(s1.str, s2.str, 3));

    lu_sstr names[32];
    uint64_t hashes[32];
    int count[4] = {0, 0, 0, 0};
    for (int i = 0; i < 32; ++i) {
        names[i] = (lu_sstr){"0"};
        _itoa_s(i, names[i].str, LU_SSTRLEN, 10);
        hashes[i] = lu_sstr_hash(&names[i]);
        count[hashes[i] % 4]++;
    }

    LU_TEST_UNIT("Hash", count[0] > 3 && count[1] > 3 && count[2] > 3 && count[3] > 3);

    int64_t duration = lu_time_elapsed(start);
    lu_log_test("%s test finished in %f seconds.", "sstr", lu_time_sec(duration));
}

int main(int argc, char **argv)
{
    lu_log_setopt(LU_LOG_COLOR | LU_LOG_DATE | LU_LOG_TIME | LU_LOG_FUNC, true);
    lu_timestamp suite_start = lu_time_get();
    lu_log_test("**************************");
    lu_log_test("**  Test Suite Started  **");
    lu_log_test("**  file: %s", __FILE__);
    lu_log_test("**************************");

    test_group_sstr();

    float suite_duration_sec = lu_time_sec(lu_time_elapsed(suite_start));
    lu_log_test("************************");
    lu_log_test("**  Test Suite Ended  **");
    lu_log_test("**  file: %s", __FILE__);
    lu_log_test("**  duration: %0.2fs", suite_duration_sec);
    lu_log_test("************************");

    return 0;
}
