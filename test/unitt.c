#include <llulu/lu_str.h>

#include <stdbool.h>
#include <stdio.h>

#define TEST_FAIL "\033[0;31mFAIL\033[0m"
#define TEST_PASS "\033[0;32mPASS\033[0m"

#define BEGIN_MODULE(NAME) set_test_group(NAME, __FILE__, __LINE__)
#define TEST(NAME, RES) do_test(NAME, RES, __PRETTY_FUNCTION__, __LINE__)

static const lu_sstr SSTR_EMPTY = {.str = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }};

static const char *g_test_module;

static void set_test_group(const char *name, const char *file, int line)
{
    g_test_module = name;
    printf("\n%s $$ %s(%d)\n", name, file, line);
}

static void do_test(const char *name, bool result, const char *func, int line)
{
    static const char * const res_to_str[] = {TEST_FAIL, TEST_PASS};

    printf("[%s] %s | %s\n", res_to_str[result], g_test_module, name);
    if (!result) {
        printf("  -> %s:%s(%d)\n", __FILE__, func, line);
    }
}

void short_string_tests(void)
{
    BEGIN_MODULE("lu_sstr");

    lu_sstr s1 = {"kokoko"};
    lu_sstr s2 = {"HOLA"};
    TEST("!empty", !lu_sstr_empty(&s1));
    lu_sstr_clear(&s1);
    TEST("clear", !memcmp(s1.str, SSTR_EMPTY.str, LU_SSTRLEN));
    TEST("empty", lu_sstr_empty(&s1));
    TEST("!equal", !lu_sstr_equal(&s1, &s2));
    TEST("!equal_0", !lu_sstr_equal_0(&s1, &s2));
    s1 = s2;
    TEST("equal", lu_sstr_equal(&s1, &s2));
    TEST("equal_0", lu_sstr_equal_0(&s1, &s2));
    const char *largestr = "SFEFHOWEINFOIWEOFNWEOIFNOWIENGOIWENFOIWENFIOWEOGINWOEING";
    lu_sstr_copy(&s1, largestr);
    TEST("asign string literal longer than buffer", s1.str[30] && !s1.str[31]);
    s2 = s1;
    lu_sstr_copy_0(&s2, s1.str);
    lu_sstr_copy(&s1, "pepa");
    TEST("strcpy", !strcmp(s1.str, "pepa"));
    TEST("strcpy 2", !memcmp(s1.str, (char[32]){"pepa"}, 32));
    TEST("strcpy_0", (bool)strcmp(s2.str, "pepa"));
    lu_sstr_copy_0(&s2, s1.str);
    TEST("strcpy_0 + equal", lu_sstr_equal(&s1, &s2));
    lu_sstr s3 = {"aowdnoqwndo"};
    lu_sstr_memcpy(&s1, &s3, 235);
    lu_sstr_memcpy_0(&s2, &s3, LU_SSTRLEN);
    TEST("memcpy", lu_sstr_equal(&s1, &s3));
    TEST("memcpy_0", lu_sstr_equal(&s2, &s3));
    TEST("memcpy 2", lu_sstr_equal(&s2, &s1));
    s1 = (lu_sstr){"teteteplplp"};
    s2 = (lu_sstr){"tetetetasfve"};
    TEST("cmpstr", lu_sstr_cmpstr(&s1, s2.str) == strcmp(s1.str, s2.str));
    TEST("cmpstr_0", lu_sstr_cmpstr_0(&s1, s2.str) == strcmp(s1.str, s2.str));
    TEST("cmpmem", lu_sstr_cmpmem(&s1, s2.str, 3) == strncmp(s1.str, s2.str, 3));
    TEST("cmpmem_0", lu_sstr_cmpmem_0(&s1, s2.str, 3) == strncmp(s1.str, s2.str, 3));

    lu_sstr names[32];
    uint64_t hashes[32];
    int count[4] = {0, 0, 0, 0};
    for (int i = 0; i < 32; ++i) {
        names[i] = (lu_sstr){"0"};
        sprintf(names[i].str, "%d", i);
        hashes[i] = lu_sstr_hash(&names[i]);
        count[hashes[i] % 4]++;
    }

    TEST("hash", count[0] > 3 && count[1] > 3 && count[2] > 3 && count[3] > 3);
}

int main(void) {


    
    printf("Unit test completed.\n");

    return 0;
}
