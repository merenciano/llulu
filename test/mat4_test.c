#include <llulu/lu_math.h>
#include <llulu/lu_error.h>
#include <llulu/lu_defs.h>

#include <string.h>
#include <stdbool.h>

static lu_mat4 mat;

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

static void
mat4_constants_test()
{
    lu_mat4 zero;
    memset(zero.m, 0, sizeof(zero));
    lu_err_expects(mat4_eq(zero.m, lu_mat4_zero(mat.m)));

    lu_mat4 identity = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    lu_err_expects(mat4_eq(identity.m, lu_mat4_identity(mat.m)));
}

static void
mat4_transpose_test()
{
    lu_mat4 trans;
    lu_mat4_identity(mat.m);
    lu_mat4_transpose(trans.m, mat.m);
    lu_err_expects(mat4_eq(trans.m, mat.m));

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
    lu_err_expects(mat4_eq(trans.m, expected.m));
}

static void
mat4_scale_test()
{
    lu_mat4 a;
    lu_mat4_identity(a.m);
    lu_mat4 e = { .m = {
        2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 2.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    }};

    lu_mat4_scale(a.m, a.m, &(LU_VEC(2.0f, 2.0f, 2.0f).x));
    lu_err_expects(mat4_eq(a.m, e.m));
}

static void
mat4_mul_test()
{
    lu_mat4 a, b, e, r;
    lu_mat4_identity(a.m);
    lu_mat4_identity(b.m);
    lu_mat4_identity(e.m);

    lu_mat4_multiply(r.m, a.m, b.m);
    lu_err_expects(mat4_eq(r.m, e.m));

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
    lu_err_expects(mat4_eq(r.m, e.m));
}

static void
mat4_mul_vec4_test()
{
    lu_mat4 a;
    lu_mat4_identity(a.m);
	lu_vec4 b = { 1.0f, 1.0f, 1.0f, 1.0f };
	lu_vec4 e = { 1.0f, 1.0f, 1.0f, 1.0f };

	lu_vec4 r;
	lu_vec4_multiply_mat4(&r.x, &b.x, a.m);
    lu_err_expects(vec4_eq(&r.x, &e.x));

    a = (lu_mat4) { .m = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        2.0f, 2.0f, 2.0f, 1.0f
    }};

    b = (lu_vec4){ 0.0f, 0.0f, 0.0f, 1.0f };
    e = (lu_vec4){ 2.0f, 2.0f, 2.0f, 1.0f };

    lu_vec4_multiply_mat4(&r.x, &b.x, a.m);
    lu_err_expects(vec4_eq(&r.x, &e.x));
}

static void
mat4_ortho_test()
{
    lu_mat4 e = { .m = {
        1.0f, 0.0f,  0.0f,          0.0f,
        0.0f, 1.0f,  0.0f,          0.0f,
        0.0f, 0.0f, -0.0200200193f, 0.0f,
       -0.0f,-0.0f, -1.002002f,     1.0f
    }};

    lu_mat4 r;
    lu_mat4_ortho(r.m, -1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    lu_err_expects(mat4_eq(r.m, e.m));
}

static void
mat4_perspective_test()
{
    float zz = -1.001001f;
    float zw = -0.1001001f;

    lu_mat4 expected = { .m = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, zz, -1.0f,
        0.0f, 0.0f, zw, 0.0f,
    }};

    lu_mat4 r;
    lu_mat4_perspective(r.m, lu_radians(90.0f), 1.0f, 0.1f, 100.0f);
    lu_err_expects(mat4_eq(r.m, expected.m));
}

static void
mat4_lookat_test()
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
    lu_err_expects(mat4_eq(r.m, expected.m));
}

static void
mat4_inverse_test()
{
    lu_mat4 a = lu_mat4_get();
    lu_mat4 e = lu_mat4_get();
    lu_mat4 r;
    lu_mat4_zero(r.m);

    lu_err_expects(lu_mat4_inverse(r.m, a.m));
    lu_err_expects(mat4_eq(r.m, e.m));

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
    lu_err_expects(lu_mat4_inverse(r.m, a.m));
    lu_err_expects(mat4_eq(r.m, e.m));

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
    lu_err_expects(lu_mat4_inverse(r.m, a.m));
    lu_err_expects(mat4_eq(r.m, e.m));

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
    lu_err_expects(lu_mat4_inverse(r.m, a.m));
    lu_err_expects(mat4_eq(r.m, e.m));
}

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
    mat4_constants_test();
    mat4_transpose_test();
    mat4_scale_test();
    mat4_mul_test();
    mat4_mul_vec4_test();
    mat4_ortho_test();
    mat4_perspective_test();
    mat4_lookat_test();
    mat4_inverse_test();

    return 0;
}
