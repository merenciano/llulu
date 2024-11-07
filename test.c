#include "lu_fixedstr.h"
#include "lu_time.h"

#include <stdio.h>

int main(void) {
    lu_str32 str = lu_str32_from_literal("FromLiteral");
    printf("FromLiteral: %s\n", str.str);
    lu_str32_cpymem(&str, "Test1");
    printf("Test1: %s\n", str.str);
    lu_str32 str2 = lu_str32_from_literal("Test2");
    lu_str32_cpy(&str, &str2);
    printf("Test2: %s\n", str.str);
    int cmpres = lu_str32_cmp(&str, &str2);
    printf("Equal: %s\n", !cmpres ? "Equal" : "Not equal");
    lu_str32_clear(&str);
    printf("Empty: %s\n", str.str);

    lu_timestamp chrono = lu_time_get();

    str = lu_str32_from_literal("1");
    printf("Hash (%s): %lu\n", str.str, lu_str32_hash(&str));

    str = lu_str32_from_literal("2");
    printf("Hash (%s): %lu\n", str.str, lu_str32_hash(&str));

    str = lu_str32_from_literal("HOLAHOLA");
    printf("Hash (%s): %lu\n", str.str, lu_str32_hash(&str));

    str = lu_str32_from_literal("_LUCAS_");
    printf("Hash (%s): %lu\n", str.str, lu_str32_hash(&str));

    str = lu_str32_from_literal("JejeProbandoHolamigo");
    printf("Hash (%s): %lu\n", str.str, lu_str32_hash(&str));

    int64_t elapsed_ns = lu_time_elapsed(chrono);
    printf("Elapsed hash: %ld ns\n", elapsed_ns);

    return 0;
}
