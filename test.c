#include "lu_fixedstr.h"
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

    str = lu_str32_from_literal("1");
    printf("Hash (%s): %u\n", str.str, lu_str32_hash(&str, 0));

    str = lu_str32_from_literal("2");
    printf("Hash (%s): %u\n", str.str, lu_str32_hash(&str, 0));

    str = lu_str32_from_literal("HOLAHOLA");
    printf("Hash (%s): %u\n", str.str, lu_str32_hash(&str, 0));

    str = lu_str32_from_literal("_LUCAS_");
    printf("Hash (%s): %u\n", str.str, lu_str32_hash(&str, 0));

    str = lu_str32_from_literal("JejeProbandoHolamigo");
    printf("Hash (%s): %u\n", str.str, lu_str32_hash(&str, 0));

    return 0;
}
