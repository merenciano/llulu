#include "lu_bench.h"
#include <llulu/lu_str.h>
#include <llulu/lu_rng.h>

void *bench_sstr(const void *ctx)
{
    const char *bench_data = ctx;
    lu_sstr s1;
    lu_sstr s2;
    lu_sstr s3;
    lu_sstr_copy(&s1, bench_data);
    lu_sstr_copy(&s2, bench_data + 32 * 1);
    lu_sstr_copy(&s3, bench_data + 32 * 2);
    int cmp = lu_sstr_cmpstr(&s1, bench_data + 32 * 3);
    cmp += lu_sstr_cmpstr(&s2, bench_data + 32 * 4);
    cmp += lu_sstr_cmpstr(&s3, bench_data + 32 * 5);

    bool eq = lu_sstr_equals(&s1, &s2);
    eq |= lu_sstr_equals(&s1, &s3);
    eq |= lu_sstr_equals(&s2, &s3);

    lu_bench_preventopt(&s1);
    lu_bench_preventopt(&s2);
    lu_bench_preventopt(&s3);
    lu_bench_preventopt(&cmp);
    lu_bench_preventopt(&eq);
    return NULL;
}

int bench_sstr_genctx(void* ctx)
{
    if (!ctx) {
        return 0;
    }

    char *bench_data = ctx;
    for (int j = 0; j < 6; ++j) {
        lu_rng_textg(&bench_data[j * 32], 31);
        bench_data[j * 32 + 31] = '\0';
    }
    return 1;
}

int main(int argc, char **argv)
{
    char bench_data[32 * 6];
    lu_bench_state strbs = {
        .name = "sstr generic bench",
        .notes = "mix of common string operations",
        .seed = 14,
        .iterations = 10000,
        .target_fn = bench_sstr,
        .ctx_generator_fn = bench_sstr_genctx,
        .ctx = bench_data,
    };

    lu_bench_run(&strbs);

    char buf[1024 * 8];
    if (lu_bench_export_json(&strbs, buf, sizeof(buf)) != LU_ERR_SUCCESS) {
        return LU_ERR_ERROR;
    }

    printf("%s\n", buf);
    return 0;
}
