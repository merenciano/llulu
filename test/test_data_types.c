#include "lu_test.h"

#include <llulu/lu_defs.h>
#include <llulu/lu_math.h>
#include <llulu/lu_str.h>
#include <llulu/lu_rng.h>
#include <llulu/lu_buffer.h>

#include <stdlib.h>
#include <stdio.h>

void test_group_rng(lu_test_state *s);
void test_group_mat4(lu_test_state *s);
void test_group_buff(lu_test_state *s);
void test_group_sstr(lu_test_state *s);

void test_failed_callback(lu_test_state *state, const char *failed_test_name)
{
    exit(1);
}

int main(int argc, char **argv)
{
    lu_test_state ts = {
        .name = "DataTypes",
        .test_fail_callback = test_failed_callback,
        .rng_seed = 0
    };

    lu_log_setopt(LU_LOG_COLOR, true);

    lu_test_begin(&ts);

    lu_test_group(&ts, "rng", test_group_rng);
    lu_test_group(&ts, "mat4", test_group_mat4);
    lu_test_group(&ts, "containers", test_group_buff);
    lu_test_group(&ts, "sstr", test_group_sstr);

    lu_test_end(&ts);

    return ts.failed;
}

void test_group_rng(lu_test_state *s)
{
    lu_rng_seedg(s->rng_seed);
    lu_rng rng1;
    lu_rng_seed(&rng1, s->rng_seed);

    uint64_t numg = lu_rng_getg();
    uint64_t num1 = lu_rng_get(&rng1, 1);
    uint64_t first = numg;
    lu_test_check(s, "Seed", numg == num1);
    numg = lu_rng_getg();
    num1 = lu_rng_get(&rng1, 1);
    lu_test_check(s, "Seed_2", numg == num1);
    numg = lu_rng_getg();
    num1 = lu_rng_get(&rng1, 1);
    lu_test_check(s, "Seed_3", numg == num1);

    lu_rng_seedg(s->rng_seed);
    lu_rng_seed(&rng1, s->rng_seed + 1);
    numg = lu_rng_getg();
    num1 = lu_rng_get(&rng1, 1);
    lu_test_check(s, "Seed_4", numg == first && num1 != first);

    lu_rng_seed(&rng1, s->rng_seed);
    num1 = lu_rng_get(&rng1, 1);
    lu_test_check(s, "Seed_5", numg == first && num1 == first);

    numg = lu_rng_getg();
    num1 = lu_rng_get(&rng1, 2);
    lu_test_check(s, "Stream", numg != num1);

    lu_rng_seedg(s->rng_seed);
    lu_rng_seed(&rng1, s->rng_seed);
    char buf[64];
    lu_rng_textg(buf, 64);
    bool in_range = true;
    for (int i = 0; i < 64; ++i) {
        if (buf[i] < 32 || buf[i] > 126) {
            in_range = false;
            break;
        }
    }
    lu_test_check(s, "Text", in_range);

    char buf2[64];
    lu_rng_get_text(&rng1, 1, buf2, 64);
    lu_rng_textg(buf, 64);

    lu_test_check(s, "Text Not Equal", memcmp(buf, buf2, 64));
    lu_rng_get_text(&rng1, 1, buf2, 64);
    lu_test_check(s, "Text Equal", !memcmp(buf, buf2, 64));

    double real1 = lu_rng_getf(&rng1, 1);
    double realg = lu_rng_getfg();
    lu_test_check(s, "Real", real1 == realg);

    real1 = lu_rng_getf(&rng1, 1);
    numg = lu_rng_getg();
    lu_test_check(s, "Real Eq int", real1 != *(double*)&numg);
}

static inline bool float_eq(float a, float b)
{
	return fabsf(a - b) < LU_EPSILON;
}

static inline bool vec4_eq(const float *a, const float *b)
{
    return float_eq(a[0], b[0]) && float_eq(a[1], b[1]) && float_eq(a[2], b[2]) && float_eq(a[3], b[3]);
}

static inline bool mat4_eq(const float *a, const float *b)
{
	bool eq = true;
	for (int i = 0; i < 16; ++i) {
		eq &= float_eq(a[i], b[i]);
	}
	return eq;
}

void test_group_mat4(lu_test_state *s)
{
    lu_mat4 mat;

    lu_mat4 zero;
    memset(zero.m, 0, sizeof(zero));
    lu_test_check(s, "Zero", mat4_eq(zero.m, lu_mat4_zero(mat.m)));

    lu_mat4 identity = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    lu_test_check(s, "Identity", mat4_eq(identity.m, lu_mat4_identity(mat.m)));

    lu_mat4 trans;
    lu_mat4_identity(mat.m);
    lu_mat4_transpose(trans.m, mat.m);
    lu_test_check(s, "Transpose", mat4_eq(trans.m, mat.m));

    mat.m[1] = 1.0f;
    mat.m[2] = 1.0f;
    mat.m[3] = 1.0f;

    lu_mat4_transpose(trans.m, mat.m);
    lu_mat4 expected = { .m = {
        1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f
    }};
    lu_test_check(s, "Transpose2", mat4_eq(trans.m, expected.m));

    lu_mat4 a;
    lu_mat4_identity(a.m);
    lu_mat4 e = { .m = {
        2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 2.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    }};

    lu_mat4_scale(a.m, a.m, &(LU_VEC(2.0f, 2.0f, 2.0f).x));
    lu_test_check(s, "Scale", mat4_eq(a.m, e.m));

    lu_mat4 b, r;
    lu_mat4_identity(a.m);
    lu_mat4_identity(b.m);
    lu_mat4_identity(e.m);

    lu_mat4_multiply(r.m, a.m, b.m);
    lu_test_check(s, "Mul", mat4_eq(r.m, e.m));

    a = (lu_mat4) { .m = {
        1.0f, 2.0f, 3.0f, 4.0f,
        4.0f, 3.0f, 2.0f, 1.0f,
        1.0f, 2.0f, 3.0f, 4.0f,
        4.0f, 3.0f, 2.0f, 1.0f
    }};

    b = (lu_mat4) { .m = {
        4.0f, 3.0f, 2.0f, 1.0f, 
        1.0f, 2.0f, 3.0f, 4.0f,
        4.0f, 3.0f, 2.0f, 1.0f,
        1.0f, 2.0f, 3.0f, 4.0f
    }};

    e = (lu_mat4) { .m = {
        22.0f, 24.0f, 26.0f, 28.0f,
        28.0f, 26.0f, 24.0f, 22.0f,
        22.0f, 24.0f, 26.0f, 28.0f,
        28.0f, 26.0f, 24.0f, 22.0f
    }};

    lu_mat4_multiply(r.m, a.m, b.m);
    lu_test_check(s, "Mul2", mat4_eq(r.m, e.m));

    lu_mat4_identity(a.m);
	lu_vec4 vb = { 1.0f, 1.0f, 1.0f, 1.0f };
	lu_vec4 ve = { 1.0f, 1.0f, 1.0f, 1.0f };

	lu_vec4 vr;
	lu_vec4_multiply_mat4(&vr.x, &vb.x, a.m);
    lu_test_check(s, "Mul vec4 mat4", vec4_eq(&vr.x, &ve.x));

    a = (lu_mat4) { .m = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        2.0f, 2.0f, 2.0f, 1.0f
    }};

    vb = (lu_vec4){ 0.0f, 0.0f, 0.0f, 1.0f };
    ve = (lu_vec4){ 2.0f, 2.0f, 2.0f, 1.0f };

    lu_vec4_multiply_mat4(&vr.x, &vb.x, a.m);
    lu_test_check(s, "Mul vec4 mat4 2", vec4_eq(&vr.x, &ve.x));

    {
        lu_mat4 o = { .m = {
            1.0f, 0.0f,  0.0f,          0.0f,
            0.0f, 1.0f,  0.0f,          0.0f,
            0.0f, 0.0f, -0.0200200193f, 0.0f,
        -0.0f,-0.0f, -1.002002f,     1.0f
        }};

        lu_mat4_ortho(r.m, -1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
        lu_test_check(s, "Ortho", mat4_eq(r.m, o.m));
    }

    {
        float zz = -1.001001f;
        float zw = -0.1001001f;

        lu_mat4 pers_expec = { .m = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, zz, -1.0f,
            0.0f, 0.0f, zw, 0.0f,
        }};

        lu_mat4_perspective(r.m, lu_radians(90.0f), 1.0f, 0.1f, 100.0f);
        lu_test_check(s, "Perspective", mat4_eq(r.m, pers_expec.m));
    }

    {
        lu_mat4 expected = { .m = {
            0.0f,  0.0f, -1.0f, 0.0f,
            0.0f, 1.0f, -0.0f, 0.0f,
            1.0f, 0.0f, -0.0f, 0.0f,
            -0.0f, -0.0f, -1.0f, 1.0f
        }};

        lu_vec3 p = { -1.0f, 0.0f, 0.0f };
        lu_vec3 t = {  0.0f, 0.0f, 0.0f };
        lu_vec3 u = {  0.0f, 1.0f, 0.0f };

        lu_mat4 r;
        lu_mat4_look_at(r.m, &p.x, &t.x, &u.x);
        lu_test_check(s, "LookAt", mat4_eq(r.m, expected.m));
    }

    {
        lu_mat4 a = lu_mat4_get();
        lu_mat4 e = lu_mat4_get();
        lu_mat4 r;
        lu_mat4_zero(r.m);

        lu_test_check(s, "Inverse", lu_mat4_inverse(r.m, a.m));
        lu_test_check(s, "Inverse_2", mat4_eq(r.m, e.m));

        a = (lu_mat4) { .m = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            2.0f, 2.0f, 2.0f, 1.0f
        }};

        e = (lu_mat4) { .m = {
            1.0f,  0.0f,  0.0f, 0.0f,
            0.0f,  1.0f,  0.0f, 0.0f,
            0.0f,  0.0f,  1.0f, 0.0f,
            -2.0f, -2.0f, -2.0f, 1.0f,
        }};

        lu_mat4_zero(r.m);
        lu_test_check(s, "Inverse_3", lu_mat4_inverse(r.m, a.m));
        lu_test_check(s, "Inverse_4", mat4_eq(r.m, e.m));

        a = (lu_mat4) { .m = {
            2.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 2.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 2.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }};

        e = (lu_mat4) { .m = {
            0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.5f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }};

        lu_mat4_zero(r.m);
        lu_test_check(s, "Inverse_5", lu_mat4_inverse(r.m, a.m));
        lu_test_check(s, "Inverse_6", mat4_eq(r.m, e.m));

        a = (lu_mat4) { .m = {
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }};

        e = (lu_mat4) { .m = {
            0.0f, 0.0f, -1.0f, 0.0f,
            0.0f, 1.0f,  0.0f, 0.0f,
            1.0f, 0.0f,  0.0f, 0.0f,
            0.0f, 0.0f,  0.0f, 1.0f
        }};

        lu_mat4_zero(r.m);
        lu_test_check(s, "Inverse_7", lu_mat4_inverse(r.m, a.m));
        lu_test_check(s, "Inverse_8", mat4_eq(r.m, e.m));
    }
}

void test_group_buff(lu_test_state *s)
{
    lu_array arr1;
    LU_ARR_ALLOCA(arr1, int, 32);
    lu_test_check(s, "Empty", lu_arr_is_empty(&arr1));
    lu_test_check(s, "NotFull", !lu_arr_is_full(&arr1));
    lu_test_check(s, "CountIsZero", !lu_arr_count(&arr1));

    *(int*)lu_arr_push(&arr1) = lu_rng_getg();

    lu_test_check(s, "NotEmpty", !lu_arr_is_empty(&arr1));
    lu_test_check(s, "NotFull", !lu_arr_is_full(&arr1));
    lu_test_check(s, "CountIsOne", lu_arr_count(&arr1) == 1);

}

void test_group_sstr(lu_test_state *s)
{
    const lu_sstr SSTR_EMPTY = {.str = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }};

    lu_sstr s1 = {"12345678"};
    lu_sstr s2 = {"12345678"};
    lu_rng_textg(s1.str, 8);
    lu_rng_textg(s2.str, 8);

    lu_test_check(s, "NotEmpty", !lu_sstr_is_empty(&s1));

    lu_sstr_clear(&s1);
    lu_test_check(s, "Clear", !memcmp(s1.str, SSTR_EMPTY.str, LU_SSTRLEN));
    lu_test_check(s, "Empty", lu_sstr_is_empty(&s1));
    lu_test_check(s, "NotEqual", !lu_sstr_equals(&s1, &s2));
    lu_test_check(s, "NotEqual_0", !lu_sstr_equals(&s1, &s2));
    s1 = s2;
    lu_test_check(s, "Equal", lu_sstr_equals(&s1, &s2));
    lu_test_check(s, "Equal_1", lu_sstr_equals(&s1, &s2));
    char largestr[64];
    lu_rng_textg(largestr, 64);
    largestr[63] = '\0';
    lu_sstr_copy(&s1, largestr);
    lu_test_check(s, "Assign string literal longer than buffer", s1.str[30] && !s1.str[31]);
    s2 = s1;
    lu_sstr_copy(&s2, s1.str);
    char bufcpy[5];
    lu_rng_textg(bufcpy, 4);
    bufcpy[4] = '\0';
    lu_sstr_copy(&s1, bufcpy);
    lu_test_check(s, "StringCopy", !strcmp(s1.str, bufcpy));
    //lu_test_check(s, "StringCopy 2", !memcmp(s1.str, (char[32]){"pepa"}, 32));
    lu_test_check(s, "StringCopy", (bool)strcmp(s2.str, bufcpy));
    lu_sstr_copy(&s2, s1.str);
    lu_test_check(s, "StringCopy + Equal", lu_sstr_equals(&s1, &s2));
    lu_sstr s3;
    lu_rng_textg(s3.str, 12);
    lu_sstr_memcpy(&s1, &s3, 235);
    lu_sstr_memcpy(&s2, &s3, LU_SSTRLEN);
    lu_test_check(s, "MemCopy", lu_sstr_equals(&s1, &s3));
    lu_test_check(s, "MemCopy 1", lu_sstr_equals(&s2, &s3));
    lu_test_check(s, "MemCopy 2", lu_sstr_equals(&s2, &s1));
    lu_sstr_clear(&s1);
    lu_rng_textg(s1.str, 16);
    s2 = s1;
    lu_rng_textg(s2.str + 7, 5);
    lu_test_check(s, "StringCompare", lu_sstr_cmpstr(&s1, s2.str) == strcmp(s1.str, s2.str));
    lu_test_check(s, "StringCompare2", lu_sstr_cmpstr(&s1, s2.str) == strcmp(s1.str, s2.str));

    lu_sstr names[32];
    uint64_t hashes[32];
    int count[4] = {0, 0, 0, 0};
    for (int i = 0; i < 32; ++i) {
        names[i] = (lu_sstr){"0"};
	sprintf(names[i].str, "%d", i);
        hashes[i] = lu_sstr_hash(&names[i]);
        count[hashes[i] % 4]++;
    }

    lu_test_check(s, "Hash", count[0] > 3 && count[1] > 3 && count[2] > 3 && count[3] > 3);
}
