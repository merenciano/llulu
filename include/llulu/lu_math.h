#ifndef __LLULU_MATH_H__
#define __LLULU_MATH_H__

#include <stddef.h>
#include <stdbool.h>
#include <math.h>

#define LU_PI 3.14159265358979323846f
#define LU_EPSILON 1.19209290e-7f

#if defined(__STDC_VERSION__) && !defined(__TINYC__)
static inline float
lu_sin(float a) { return sinf(a);}
static inline float
lu_cos(float a) { return cosf(a);}
static inline float
lu_acos(float a) { return acosf(a);}
static inline float
lu_sqrt(float x) { return sqrtf(x);}
static inline float
lu_atan2(float y, float x) { return atan2f(y, x);}
static inline float
lu_fmod(float x, float y) { return fmodf(x, y);}
#else
static inline float
lu_sin(float a) { return sin(a);}
static inline float
lu_cos(float a) { return cos(a);}
static inline float
lu_acos(float a) { return acos(a);}
static inline float
lu_sqrt(float x) { return sqrt(x);}
static inline float
lu_atan2(float y, float x) { return atan2(y, x);}
static inline float
lu_fmod(float x, float y) { return fmod(x, y);}
#endif

static inline float
lu_minf(float a, float b) { return a < b ? a : b; }
static inline float
lu_maxf(float a, float b) { return a >= b ? a : b; }
static inline float
lu_clampf(float v, float min, float max) { return (v < min) ? min : (v > max) ? max : v; }
static inline float
lu_lerpf(float a, float b, float t) { return a + (b - a) * t; }

static inline int lu_mini(int a, int b) { return a < b ? a : b; }
static inline int lu_maxi(int a, int b) { return a >= b ? a : b; }

static inline size_t lu_minu(size_t a, size_t b) { return a < b ? a : b; }
static inline size_t lu_maxu(size_t a, size_t b) { return a >= b ? a : b; }

static inline float lu_radians(float degrees) { return degrees * LU_PI / 180.0f; }
static inline float lu_degrees(float radians) { return radians * 180.0f / LU_PI; }

typedef struct lu_vec2 {
	float x, y;
} lu_vec2;

typedef struct lu_vec3 {
    float x, y, z;
} lu_vec3;

typedef struct lu_vec4 {
	float x, y, z, w;
} lu_vec4;

typedef struct lu_mat3 {
    float m[9];
} lu_mat3;

typedef struct lu_mat4 {
    float m[16];
} lu_mat4;

static inline bool
lu_vec3_is_zero(float *v0)
{
	return fabsf(v0[0]) < LU_EPSILON && fabsf(v0[1]) < LU_EPSILON && fabsf(v0[2]) < LU_EPSILON;
}

static inline bool
lu_vec3_is_equal(float *v0, float *v1)
{
	return fabsf(v0[0] - v1[0]) < LU_EPSILON && fabsf(v0[1] - v1[1]) < LU_EPSILON && fabsf(v0[2] - v1[2]) < LU_EPSILON;
}

static inline float *
lu_vec3_assign(float *result, float *v0)
{
	result[0] = v0[0];
	result[1] = v0[1];
	result[2] = v0[2];
	return result;
}

static inline float *
lu_vec3_zero(float *result)
{
	result[0] = 0.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	return result;
}

static inline float *
lu_vec3_one(float *result)
{
	result[0] = 1.0f;
	result[1] = 1.0f;
	result[2] = 1.0f;
	return result;
}

static inline float *
lu_vec3_sign(float *result, float *v0)
{
	if (v0[0] > 0.0f) {
		result[0] = 1.0f;
	} else if (v0[0] < 0) {
		result[0] = -1.0f;
	} else {
		result[0] = 0.0f;
	}
	if (v0[1] > 0.0f) {
		result[1] = 1.0f;
	} else if (v0[1] < 0) {
		result[1] = -1.0f;
	} else {
		result[1] = 0.0f;
	}
	if (v0[2] > 0.0f) {
		result[2] = 1.0f;
	} else if (v0[2] < 0) {
		result[2] = -1.0f;
	} else {
		result[2] = 0.0f;
	}
	return result;
}

static inline float *
lu_vec3_add(float *result, float *v0, float *v1)
{
	result[0] = v0[0] + v1[0];
	result[1] = v0[1] + v1[1];
	result[2] = v0[2] + v1[2];
	return result;
}

static inline float *
lu_vec3_add_f(float *result, float *v0, float f)
{
	result[0] = v0[0] + f;
	result[1] = v0[1] + f;
	result[2] = v0[2] + f;
	return result;
}

static inline float *
lu_vec3_subtract(float *result, float *v0, float *v1)
{
	result[0] = v0[0] - v1[0];
	result[1] = v0[1] - v1[1];
	result[2] = v0[2] - v1[2];
	return result;
}

static inline float *
lu_vec3_subtract_f(float *result, float *v0, float f)
{
	result[0] = v0[0] - f;
	result[1] = v0[1] - f;
	result[2] = v0[2] - f;
	return result;
}

static inline float *
lu_vec3_multiply(float *result, float *v0, float *v1)
{
	result[0] = v0[0] * v1[0];
	result[1] = v0[1] * v1[1];
	result[2] = v0[2] * v1[2];
	return result;
}

static inline float *
lu_vec3_multiply_f(float *result, float *v0, float f)
{
	result[0] = v0[0] * f;
	result[1] = v0[1] * f;
	result[2] = v0[2] * f;
	return result;
}

static inline float *
lu_vec3_multiply_mat3(float *result, float *v0, float *m0)
{
	float x = v0[0];
	float y = v0[1];
	float z = v0[2];
	result[0] = m0[0] * x + m0[3] * y + m0[6] * z;
	result[1] = m0[1] * x + m0[4] * y + m0[7] * z;
	result[2] = m0[2] * x + m0[5] * y + m0[8] * z;
	return result;
}

static inline float *
lu_vec3_divide(float *result, float *v0, float *v1)
{
	result[0] = v0[0] / v1[0];
	result[1] = v0[1] / v1[1];
	result[2] = v0[2] / v1[2];
	return result;
}

static inline float *
lu_vec3_divide_f(float *result, float *v0, float f)
{
	result[0] = v0[0] / f;
	result[1] = v0[1] / f;
	result[2] = v0[2] / f;
	return result;
}

static inline float *
lu_vec3_snap(float *result, float *v0, float *v1)
{
	result[0] = floorf(v0[0] / v1[0]) * v1[0];
	result[1] = floorf(v0[1] / v1[1]) * v1[1];
	result[2] = floorf(v0[2] / v1[2]) * v1[2];
	return result;
}

static inline float *
lu_vec3_snap_f(float *result, float *v0, float f)
{
	result[0] = floorf(v0[0] / f) * f;
	result[1] = floorf(v0[1] / f) * f;
	result[2] = floorf(v0[2] / f) * f;
	return result;
}

static inline float *
lu_vec3_negative(float *result, float *v0)
{
	result[0] = -v0[0];
	result[1] = -v0[1];
	result[2] = -v0[2];
	return result;
}

static inline float *
lu_vec3_abs(float *result, float *v0)
{
	result[0] = fabsf(v0[0]);
	result[1] = fabsf(v0[1]);
	result[2] = fabsf(v0[2]);
	return result;
}

static inline float *
lu_vec3_floor(float *result, float *v0)
{
	result[0] = floorf(v0[0]);
	result[1] = floorf(v0[1]);
	result[2] = floorf(v0[2]);
	return result;
}

static inline float *
lu_vec3_ceil(float *result, float *v0)
{
	result[0] = ceilf(v0[0]);
	result[1] = ceilf(v0[1]);
	result[2] = ceilf(v0[2]);
	return result;
}

static inline float *
lu_vec3_round(float *result, float *v0)
{
	result[0] = roundf(v0[0]);
	result[1] = roundf(v0[1]);
	result[2] = roundf(v0[2]);
	return result;
}

static inline float *
lu_vec3_max(float *result, float *v0, float *v1)
{
	result[0] = lu_maxf(v0[0], v1[0]);
	result[1] = lu_maxf(v0[1], v1[1]);
	result[2] = lu_maxf(v0[2], v1[2]);
	return result;
}

static inline float *
lu_vec3_min(float *result, float *v0, float *v1)
{
	result[0] = lu_minf(v0[0], v1[0]);
	result[1] = lu_minf(v0[1], v1[1]);
	result[2] = lu_minf(v0[2], v1[2]);
	return result;
}

static inline float *
lu_vec3_clamp(float *result, float *v0, float *v1, float *v2)
{
	lu_vec3_min(result, v0, v1);
	lu_vec3_max(result, v0, v2);
	return result;
}

static inline float *
lu_vec3_cross(float *result, float *v0, float *v1)
{
	float cross[3];
	cross[0] = v0[1] * v1[2] - v0[2] * v1[1];
	cross[1] = v0[2] * v1[0] - v0[0] * v1[2];
	cross[2] = v0[0] * v1[1] - v0[1] * v1[0];
	result[0] = cross[0];
	result[1] = cross[1];
	result[2] = cross[2];
	return result;
}

static inline float *
lu_vec3_normalize(float *result, float *v0)
{
	float l = lu_sqrt(v0[0] * v0[0] + v0[1] * v0[1] + v0[2] * v0[2]);
	result[0] = v0[0] / l;
	result[1] = v0[1] / l;
	result[2] = v0[2] / l;
	return result;
}

static inline float
lu_vec3_dot(float *v0, float *v1)
{
	return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
}

static inline float *
lu_vec3_project(float *result, float *v0, float *v1)
{
	float d = lu_vec3_dot(v1, v1);
	float s = lu_vec3_dot(v0, v1) / d;
	result[0] = v1[0] * s;
	result[1] = v1[1] * s;
	result[2] = v1[2] * s;
	return result;
}

static inline float *
lu_vec3_slide(float *result, float *v0, float *normal)
{
	float d = lu_vec3_dot(v0, normal);
	result[0] = v0[0] - normal[0] * d;
	result[1] = v0[1] - normal[1] * d;
	result[2] = v0[2] - normal[2] * d;
	return result;
}

static inline float *
lu_vec3_reflect(float *result, float *v0, float *normal)
{
	float d = 2.0f * lu_vec3_dot(v0, normal);
	result[0] = normal[0] * d - v0[0];
	result[1] = normal[1] * d - v0[1];
	result[2] = normal[2] * d - v0[2];
	return result;
}

static inline float *
lu_vec3_rotate(float *result, float *v0, float *ra, float f)
{
	float cs;
	float sn;
	float x;
	float y;
	float z;
	float rx;
	float ry;
	float rz;
	cs = lu_cos(f);
	sn = lu_sin(f);
	x = v0[0];
	y = v0[1];
	z = v0[2];
	lu_vec3_normalize(ra, ra);
	rx = ra[0];
	ry = ra[1];
	rz = ra[2];
	result[0] = x * (cs + rx * rx * (1 - cs)) + y * (rx * ry * (1 - cs) - rz * sn) + z * (rx * rz * (1 - cs) + ry * sn);
	result[1] = x * (ry * rx * (1 - cs) + rz * sn) + y * (cs + ry * ry * (1 - cs)) + z * (ry * rz * (1 - cs) - rx * sn);
	result[2] = x * (rz * rx * (1 - cs) - ry * sn) + y * (rz * ry * (1 - cs) + rx * sn) + z * (cs + rz * rz * (1 - cs));
	return result;
}

static inline float *
lu_vec3_lerp(float *result, float *v0, float *v1, float f)
{
	result[0] = v0[0] + (v1[0] - v0[0]) * f;
	result[1] = v0[1] + (v1[1] - v0[1]) * f;
	result[2] = v0[2] + (v1[2] - v0[2]) * f;
	return result;
}

static inline float *
lu_vec3_bezier3(float *result, float *v0, float *v1, float *v2, float f)
{
	float tmp0[3];
	float tmp1[3];
	lu_vec3_lerp(tmp0, v0, v1, f);
	lu_vec3_lerp(tmp1, v1, v2, f);
	lu_vec3_lerp(result, tmp0, tmp1, f);
	return result;
}

static inline float *
lu_vec3_bezier4(float *result, float *v0, float *v1, float *v2, float *v3, float f)
{
	float tmp0[3];
	float tmp1[3];
	float tmp2[3];
	float tmp3[3];
	float tmp4[3];
	lu_vec3_lerp(tmp0, v0, v1, f);
	lu_vec3_lerp(tmp1, v1, v2, f);
	lu_vec3_lerp(tmp2, v2, v3, f);
	lu_vec3_lerp(tmp3, tmp0, tmp1, f);
	lu_vec3_lerp(tmp4, tmp1, tmp2, f);
	lu_vec3_lerp(result, tmp3, tmp4, f);
	return result;
}

static inline float
lu_vec3_length(float *v0)
{
	return lu_sqrt(v0[0] * v0[0] + v0[1] * v0[1] + v0[2] * v0[2]);
}

static inline float
lu_vec3_length_squared(float *v0)
{
	return v0[0] * v0[0] + v0[1] * v0[1] + v0[2] * v0[2];
}

static inline float
lu_vec3_distance(float *v0, float *v1)
{
	return lu_sqrt((v0[0] - v1[0]) * (v0[0] - v1[0]) + (v0[1] - v1[1]) * (v0[1] - v1[1]) + (v0[2] - v1[2]) * (v0[2] - v1[2]));
}

static inline float
lu_vec3_distance_squared(float *v0, float *v1)
{
	return (v0[0] - v1[0]) * (v0[0] - v1[0]) + (v0[1] - v1[1]) * (v0[1] - v1[1]) + (v0[2] - v1[2]) * (v0[2] - v1[2]);
}

static inline bool
lu_vec3_linear_independent(float *v0, float *v1, float *v2)
{
	return v0[0] * v1[1] * v2[2] + v0[1] * v1[2] * v2[0] + v0[2] * v1[0] * v2[1]
		- v0[2] * v1[1] * v2[0] - v0[1] * v1[0] * v2[2] - v0[0] * v1[2] * v2[1];
}

static inline float **
lu_vec3_orthonormalization(float result[3][3], float basis[3][3])
{
	float v0[3];
	float v1[3];
	float v2[3];
	
	for(int i = 0; i < 3; ++i) {
		v0[i] = basis[0][i];
		v1[i] = basis[1][i];
		v2[i] = basis[2][i];
	}

	if (!lu_vec3_linear_independent(v0, v1, v2)) {
		return (float**)result;
	}

	float proj[3];
	float u0[3];
	float u1[3];
	float u2[3];

	for(int i = 0; i < 3; ++i) {
		u0[i] = v0[i];
	}

	lu_vec3_project(proj, v1, u0);
	lu_vec3_subtract(u1, v1, proj);

	lu_vec3_project(proj, v2, u0);
	lu_vec3_subtract(u2, v2, proj);
	lu_vec3_project(proj, v2, u1);
	lu_vec3_subtract(u2, u2, proj);

	lu_vec3_normalize(result[0], u0);
	lu_vec3_normalize(result[1], u1);
	lu_vec3_normalize(result[2], u2);

	return (float**)result;
}

static inline float
lu_mat3_determinant(float *m0)
{
	float m11 = m0[0];
	float m21 = m0[1];
	float m31 = m0[2];
	float m12 = m0[3];
	float m22 = m0[4];
	float m32 = m0[5];
	float m13 = m0[6];
	float m23 = m0[7];
	float m33 = m0[8];
	float determinant = m11 * m22 * m33
		+ m12 * m23 * m31
		+ m13 * m21 * m32
		- m11 * m23 * m32
		- m12 * m21 * m33
		- m13 * m22 * m31;
	return determinant;
}

static inline float *
lu_mat4_zero(float *result)
{
	result[0] = 0.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	result[4] = 0.0f;
	result[5] = 0.0f;
	result[6] = 0.0f;
	result[7] = 0.0f;
	result[8] = 0.0f;
	result[9] = 0.0f;
	result[10] = 0.0f;
	result[11] = 0.0f;
	result[12] = 0.0f;
	result[13] = 0.0f;
	result[14] = 0.0f;
	result[15] = 0.0f;
	return result;
}

static inline lu_mat4
lu_mat4_get(void)
{
	return (lu_mat4) { .m = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f 
	}};
}

static inline float *
lu_mat4_identity(float *result)
{
	result[0] = 1.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	result[4] = 0.0f;
	result[5] = 1.0f;
	result[6] = 0.0f;
	result[7] = 0.0f;
	result[8] = 0.0f;
	result[9] = 0.0f;
	result[10] = 1.0f;
	result[11] = 0.0f;
	result[12] = 0.0f;
	result[13] = 0.0f;
	result[14] = 0.0f;
	result[15] = 1.0f;
	return result;
}

static inline float
lu_mat4_determinant(float *m0)
{
	float m11 = m0[0];
	float m21 = m0[1];
	float m31 = m0[2];
	float m41 = m0[3];
	float m12 = m0[4];
	float m22 = m0[5];
	float m32 = m0[6];
	float m42 = m0[7];
	float m13 = m0[8];
	float m23 = m0[9];
	float m33 = m0[10];
	float m43 = m0[11];
	float m14 = m0[12];
	float m24 = m0[13];
	float m34 = m0[14];
	float m44 = m0[15];
	float determinant = m14 * m23 * m32 * m41 - m13 * m24 * m32 * m41
		- m14 * m22 * m33 * m41 + m12 * m24 * m33 * m41
		+ m13 * m22 * m34 * m41 - m12 * m23 * m34 * m41
		- m14 * m23 * m31 * m42 + m13 * m24 * m31 * m42
		+ m14 * m21 * m33 * m42 - m11 * m24 * m33 * m42
		- m13 * m21 * m34 * m42 + m11 * m23 * m34 * m42
		+ m14 * m22 * m31 * m43 - m12 * m24 * m31 * m43
		- m14 * m21 * m32 * m43 + m11 * m24 * m32 * m43
		+ m12 * m21 * m34 * m43 - m11 * m22 * m34 * m43
		- m13 * m22 * m31 * m44 + m12 * m23 * m31 * m44
		+ m13 * m21 * m32 * m44 - m11 * m23 * m32 * m44
		- m12 * m21 * m33 * m44 + m11 * m22 * m33 * m44;
	return determinant;
}

static inline float *
lu_mat4_assign(float *result, float *m0)
{
	result[0] = m0[0];
	result[1] = m0[1];
	result[2] = m0[2];
	result[3] = m0[3];
	result[4] = m0[4];
	result[5] = m0[5];
	result[6] = m0[6];
	result[7] = m0[7];
	result[8] = m0[8];
	result[9] = m0[9];
	result[10] = m0[10];
	result[11] = m0[11];
	result[12] = m0[12];
	result[13] = m0[13];
	result[14] = m0[14];
	result[15] = m0[15];
	return result;
}

static inline float *
lu_mat4_negative(float *result, float *m0)
{
	result[0] = -m0[0];
	result[1] = -m0[1];
	result[2] = -m0[2];
	result[3] = -m0[3];
	result[4] = -m0[4];
	result[5] = -m0[5];
	result[6] = -m0[6];
	result[7] = -m0[7];
	result[8] = -m0[8];
	result[9] = -m0[9];
	result[10] = -m0[10];
	result[11] = -m0[11];
	result[12] = -m0[12];
	result[13] = -m0[13];
	result[14] = -m0[14];
	result[15] = -m0[15];
	return result;
}

static inline float *
lu_mat4_transpose(float *result, float *m0)
{
	float transposed[16];
	transposed[0] = m0[0];
	transposed[1] = m0[4];
	transposed[2] = m0[8];
	transposed[3] = m0[12];
	transposed[4] = m0[1];
	transposed[5] = m0[5];
	transposed[6] = m0[9];
	transposed[7] = m0[13];
	transposed[8] = m0[2];
	transposed[9] = m0[6];
	transposed[10] = m0[10];
	transposed[11] = m0[14];
	transposed[12] = m0[3];
	transposed[13] = m0[7];
	transposed[14] = m0[11];
	transposed[15] = m0[15];
	result[0] = transposed[0];
	result[1] = transposed[1];
	result[2] = transposed[2];
	result[3] = transposed[3];
	result[4] = transposed[4];
	result[5] = transposed[5];
	result[6] = transposed[6];
	result[7] = transposed[7];
	result[8] = transposed[8];
	result[9] = transposed[9];
	result[10] = transposed[10];
	result[11] = transposed[11];
	result[12] = transposed[12];
	result[13] = transposed[13];
	result[14] = transposed[14];
	result[15] = transposed[15];
	return result;
}

static inline float *
lu_mat4_cofactor(float *result, float *m0)
{
	float cofactor[16];
	float minor[9];
	minor[0] = m0[5];
	minor[1] = m0[6];
	minor[2] = m0[7];
	minor[3] = m0[9];
	minor[4] = m0[10];
	minor[5] = m0[11];
	minor[6] = m0[13];
	minor[7] = m0[14];
	minor[8] = m0[15];
	cofactor[0] = lu_mat3_determinant(minor);
	minor[0] = m0[4];
	minor[1] = m0[6];
	minor[2] = m0[7];
	minor[3] = m0[8];
	minor[4] = m0[10];
	minor[5] = m0[11];
	minor[6] = m0[12];
	minor[7] = m0[14];
	minor[8] = m0[15];
	cofactor[1] = -lu_mat3_determinant(minor);
	minor[0] = m0[4];
	minor[1] = m0[5];
	minor[2] = m0[7];
	minor[3] = m0[8];
	minor[4] = m0[9];
	minor[5] = m0[11];
	minor[6] = m0[12];
	minor[7] = m0[13];
	minor[8] = m0[15];
	cofactor[2] = lu_mat3_determinant(minor);
	minor[0] = m0[4];
	minor[1] = m0[5];
	minor[2] = m0[6];
	minor[3] = m0[8];
	minor[4] = m0[9];
	minor[5] = m0[10];
	minor[6] = m0[12];
	minor[7] = m0[13];
	minor[8] = m0[14];
	cofactor[3] = -lu_mat3_determinant(minor);
	minor[0] = m0[1];
	minor[1] = m0[2];
	minor[2] = m0[3];
	minor[3] = m0[9];
	minor[4] = m0[10];
	minor[5] = m0[11];
	minor[6] = m0[13];
	minor[7] = m0[14];
	minor[8] = m0[15];
	cofactor[4] = -lu_mat3_determinant(minor);
	minor[0] = m0[0];
	minor[1] = m0[2];
	minor[2] = m0[3];
	minor[3] = m0[8];
	minor[4] = m0[10];
	minor[5] = m0[11];
	minor[6] = m0[12];
	minor[7] = m0[14];
	minor[8] = m0[15];
	cofactor[5] = lu_mat3_determinant(minor);
	minor[0] = m0[0];
	minor[1] = m0[1];
	minor[2] = m0[3];
	minor[3] = m0[8];
	minor[4] = m0[9];
	minor[5] = m0[11];
	minor[6] = m0[12];
	minor[7] = m0[13];
	minor[8] = m0[15];
	cofactor[6] = -lu_mat3_determinant(minor);
	minor[0] = m0[0];
	minor[1] = m0[1];
	minor[2] = m0[2];
	minor[3] = m0[8];
	minor[4] = m0[9];
	minor[5] = m0[10];
	minor[6] = m0[12];
	minor[7] = m0[13];
	minor[8] = m0[14];
	cofactor[7] = lu_mat3_determinant(minor);
	minor[0] = m0[1];
	minor[1] = m0[2];
	minor[2] = m0[3];
	minor[3] = m0[5];
	minor[4] = m0[6];
	minor[5] = m0[7];
	minor[6] = m0[13];
	minor[7] = m0[14];
	minor[8] = m0[15];
	cofactor[8] = lu_mat3_determinant(minor);
	minor[0] = m0[0];
	minor[1] = m0[2];
	minor[2] = m0[3];
	minor[3] = m0[4];
	minor[4] = m0[6];
	minor[5] = m0[7];
	minor[6] = m0[12];
	minor[7] = m0[14];
	minor[8] = m0[15];
	cofactor[9] = -lu_mat3_determinant(minor);
	minor[0] = m0[0];
	minor[1] = m0[1];
	minor[2] = m0[3];
	minor[3] = m0[4];
	minor[4] = m0[5];
	minor[5] = m0[7];
	minor[6] = m0[12];
	minor[7] = m0[13];
	minor[8] = m0[15];
	cofactor[10] = lu_mat3_determinant(minor);
	minor[0] = m0[0];
	minor[1] = m0[1];
	minor[2] = m0[2];
	minor[3] = m0[4];
	minor[4] = m0[5];
	minor[5] = m0[6];
	minor[6] = m0[12];
	minor[7] = m0[13];
	minor[8] = m0[14];
	cofactor[11] = -lu_mat3_determinant(minor);
	minor[0] = m0[1];
	minor[1] = m0[2];
	minor[2] = m0[3];
	minor[3] = m0[5];
	minor[4] = m0[6];
	minor[5] = m0[7];
	minor[6] = m0[9];
	minor[7] = m0[10];
	minor[8] = m0[11];
	cofactor[12] = -lu_mat3_determinant(minor);
	minor[0] = m0[0];
	minor[1] = m0[2];
	minor[2] = m0[3];
	minor[3] = m0[4];
	minor[4] = m0[6];
	minor[5] = m0[7];
	minor[6] = m0[8];
	minor[7] = m0[10];
	minor[8] = m0[11];
	cofactor[13] = lu_mat3_determinant(minor);
	minor[0] = m0[0];
	minor[1] = m0[1];
	minor[2] = m0[3];
	minor[3] = m0[4];
	minor[4] = m0[5];
	minor[5] = m0[7];
	minor[6] = m0[8];
	minor[7] = m0[9];
	minor[8] = m0[11];
	cofactor[14] = -lu_mat3_determinant(minor);
	minor[0] = m0[0];
	minor[1] = m0[1];
	minor[2] = m0[2];
	minor[3] = m0[4];
	minor[4] = m0[5];
	minor[5] = m0[6];
	minor[6] = m0[8];
	minor[7] = m0[9];
	minor[8] = m0[10];
	cofactor[15] = lu_mat3_determinant(minor);
	result[0] = cofactor[0];
	result[1] = cofactor[1];
	result[2] = cofactor[2];
	result[3] = cofactor[3];
	result[4] = cofactor[4];
	result[5] = cofactor[5];
	result[6] = cofactor[6];
	result[7] = cofactor[7];
	result[8] = cofactor[8];
	result[9] = cofactor[9];
	result[10] = cofactor[10];
	result[11] = cofactor[11];
	result[12] = cofactor[12];
	result[13] = cofactor[13];
	result[14] = cofactor[14];
	result[15] = cofactor[15];
	return result;
}

static inline float *
lu_mat4_rotation_x(float *result, float f)
{
	float c = lu_cos(f);
	float s = lu_sin(f);
	result[5] = c;
	result[6] = s;
	result[9] = -s;
	result[10] = c;
	return result;
}

static inline float *
lu_mat4_rotation_y(float *result, float f)
{
	float c = lu_cos(f);
	float s = lu_sin(f);
	result[0] = c;
	result[2] = -s;
	result[8] = s;
	result[10] = c;
	return result;
}

static inline float *
lu_mat4_rotation_z(float *result, float f)
{
	float c = lu_cos(f);
	float s = lu_sin(f);
	result[0] = c;
	result[1] = s;
	result[4] = -s;
	result[5] = c;
	return result;
}

static inline float *
lu_mat4_rotation_axis(float *result, float *v0, float f)
{
	float c = lu_cos(f);
	float s = lu_sin(f);
	float one_c = 1.0f - c;
	float x = v0[0];
	float y = v0[1];
	float z = v0[2];
	float xx = x * x;
	float xy = x * y;
	float xz = x * z;
	float yy = y * y;
	float yz = y * z;
	float zz = z * z;
	float l = xx + yy + zz;
	float sqrt_l = sqrt(l);
	result[0] = (xx + (yy + zz) * c) / l;
	result[1] = (xy * one_c + v0[2] * sqrt_l * s) / l;
	result[2] = (xz * one_c - v0[1] * sqrt_l * s) / l;
	result[3] = 0.0f;
	result[4] = (xy * one_c - v0[2] * sqrt_l * s) / l;
	result[5] = (yy + (xx + zz) * c) / l;
	result[6] = (yz * one_c + v0[0] * sqrt_l * s) / l;
	result[7] = 0.0f;
	result[8] = (xz * one_c + v0[1] * sqrt_l * s) / l;
	result[9] = (yz * one_c - v0[0] * sqrt_l * s) / l;
	result[10] = (zz + (xx + yy) * c) / l;
	result[11] = 0.0f;
	result[12] = 0.0f;
	result[13] = 0.0f;
	result[14] = 0.0f;
	result[15] = 1.0f;
	return result;
}

static inline float *
lu_mat4_rotation_quat(float *result, float *q0)
{
	float xx = q0[0] * q0[0];
	float yy = q0[1] * q0[1];
	float zz = q0[2] * q0[2];
	float xy = q0[0] * q0[1];
	float zw = q0[2] * q0[3];
	float xz = q0[0] * q0[2];
	float yw = q0[1] * q0[3];
	float yz = q0[1] * q0[2];
	float xw = q0[0] * q0[3];
	result[0] = 1.0f - 2.0f * (yy + zz);
	result[1] = 2.0f * (xy + zw);
	result[2] = 2.0f * (xz - yw);
	result[3] = 0.0f;
	result[4] = 2.0f * (xy - zw);
	result[5] = 1.0f - 2.0f * (xx + zz);
	result[6] = 2.0f * (yz + xw);
	result[7] = 0.0f;
	result[8] = 2.0f * (xz + yw);
	result[9] = 2.0f * (yz - xw);
	result[10] = 1.0f - 2.0f * (xx + yy);
	result[11] = 0.0f;
	result[12] = 0.0f;
	result[13] = 0.0f;
	result[14] = 0.0f;
	result[15] = 1.0f;
	return result;
}

static inline float *
lu_mat4_translation(float *result, float *m0, float *v0)
{
	result[0] = m0[0];
	result[1] = m0[1];
	result[2] = m0[2];
	result[3] = m0[3];
	result[4] = m0[4];
	result[5] = m0[5];
	result[6] = m0[6];
	result[7] = m0[7];
	result[8] = m0[8];
	result[9] = m0[9];
	result[10] = m0[10];
	result[11] = m0[11];
	result[12] = v0[0];
	result[13] = v0[1];
	result[14] = v0[2];
	result[15] = m0[15];
	return result;
}

static inline float *
lu_mat4_translate(float *result, float *m0, float *v0)
{
	result[0] = m0[0];
	result[1] = m0[1];
	result[2] = m0[2];
	result[3] = m0[3];
	result[4] = m0[4];
	result[5] = m0[5];
	result[6] = m0[6];
	result[7] = m0[7];
	result[8] = m0[8];
	result[9] = m0[9];
	result[10] = m0[10];
	result[11] = m0[11];
	result[12] = m0[12] + v0[0];
	result[13] = m0[13] + v0[1];
	result[14] = m0[14] + v0[2];
	result[15] = m0[15];
	return result;
}

static inline float *
lu_mat4_scaling(float *result, float *m0, float *v0)
{
	result[0] = v0[0];
	result[1] = m0[1];
	result[2] = m0[2];
	result[3] = m0[3];
	result[4] = m0[4];
	result[5] = v0[1];
	result[6] = m0[6];
	result[7] = m0[7];
	result[8] = m0[8];
	result[9] = m0[9];
	result[10] = v0[2];
	result[11] = m0[11];
	result[12] = m0[12];
	result[13] = m0[13];
	result[14] = m0[14];
	result[15] = m0[15];
	return result;
}

static inline float *
lu_mat4_scale(float *result, float *m0, float *v0)
{
	result[0] = m0[0] * v0[0];
	result[1] = m0[1];
	result[2] = m0[2];
	result[3] = m0[3];
	result[4] = m0[4];
	result[5] = m0[5] * v0[1];
	result[6] = m0[6];
	result[7] = m0[7];
	result[8] = m0[8];
	result[9] = m0[9];
	result[10] = m0[10] * v0[2];
	result[11] = m0[11];
	result[12] = m0[12];
	result[13] = m0[13];
	result[14] = m0[14];
	result[15] = m0[15];
	return result;
}

static inline float *
lu_mat4_multiply(float *result, const float *m0, const float *m1)
{
	float multiplied[16];
	multiplied[0] = m0[0] * m1[0] + m0[4] * m1[1] + m0[8] * m1[2] + m0[12] * m1[3];
	multiplied[1] = m0[1] * m1[0] + m0[5] * m1[1] + m0[9] * m1[2] + m0[13] * m1[3];
	multiplied[2] = m0[2] * m1[0] + m0[6] * m1[1] + m0[10] * m1[2] + m0[14] * m1[3];
	multiplied[3] = m0[3] * m1[0] + m0[7] * m1[1] + m0[11] * m1[2] + m0[15] * m1[3];
	multiplied[4] = m0[0] * m1[4] + m0[4] * m1[5] + m0[8] * m1[6] + m0[12] * m1[7];
	multiplied[5] = m0[1] * m1[4] + m0[5] * m1[5] + m0[9] * m1[6] + m0[13] * m1[7];
	multiplied[6] = m0[2] * m1[4] + m0[6] * m1[5] + m0[10] * m1[6] + m0[14] * m1[7];
	multiplied[7] = m0[3] * m1[4] + m0[7] * m1[5] + m0[11] * m1[6] + m0[15] * m1[7];
	multiplied[8] = m0[0] * m1[8] + m0[4] * m1[9] + m0[8] * m1[10] + m0[12] * m1[11];
	multiplied[9] = m0[1] * m1[8] + m0[5] * m1[9] + m0[9] * m1[10] + m0[13] * m1[11];
	multiplied[10] = m0[2] * m1[8] + m0[6] * m1[9] + m0[10] * m1[10] + m0[14] * m1[11];
	multiplied[11] = m0[3] * m1[8] + m0[7] * m1[9] + m0[11] * m1[10] + m0[15] * m1[11];
	multiplied[12] = m0[0] * m1[12] + m0[4] * m1[13] + m0[8] * m1[14] + m0[12] * m1[15];
	multiplied[13] = m0[1] * m1[12] + m0[5] * m1[13] + m0[9] * m1[14] + m0[13] * m1[15];
	multiplied[14] = m0[2] * m1[12] + m0[6] * m1[13] + m0[10] * m1[14] + m0[14] * m1[15];
	multiplied[15] = m0[3] * m1[12] + m0[7] * m1[13] + m0[11] * m1[14] + m0[15] * m1[15];
	result[0] = multiplied[0];
	result[1] = multiplied[1];
	result[2] = multiplied[2];
	result[3] = multiplied[3];
	result[4] = multiplied[4];
	result[5] = multiplied[5];
	result[6] = multiplied[6];
	result[7] = multiplied[7];
	result[8] = multiplied[8];
	result[9] = multiplied[9];
	result[10] = multiplied[10];
	result[11] = multiplied[11];
	result[12] = multiplied[12];
	result[13] = multiplied[13];
	result[14] = multiplied[14];
	result[15] = multiplied[15];
	return result;
}

static inline float *
lu_mat4_multiply_f(float *result, float *m0, float f)
{
	result[0] = m0[0] * f;
	result[1] = m0[1] * f;
	result[2] = m0[2] * f;
	result[3] = m0[3] * f;
	result[4] = m0[4] * f;
	result[5] = m0[5] * f;
	result[6] = m0[6] * f;
	result[7] = m0[7] * f;
	result[8] = m0[8] * f;
	result[9] = m0[9] * f;
	result[10] = m0[10] * f;
	result[11] = m0[11] * f;
	result[12] = m0[12] * f;
	result[13] = m0[13] * f;
	result[14] = m0[14] * f;
	result[15] = m0[15] * f;
	return result;
}

static inline float *
lu_mat4_inverse(float *result, float *m0)
{
	float inverse[16];
	float inverted_determinant;
	float m11 = m0[0];
	float m21 = m0[1];
	float m31 = m0[2];
	float m41 = m0[3];
	float m12 = m0[4];
	float m22 = m0[5];
	float m32 = m0[6];
	float m42 = m0[7];
	float m13 = m0[8];
	float m23 = m0[9];
	float m33 = m0[10];
	float m43 = m0[11];
	float m14 = m0[12];
	float m24 = m0[13];
	float m34 = m0[14];
	float m44 = m0[15];
	inverse[0] = m22 * m33 * m44
		- m22 * m43 * m34
		- m23 * m32 * m44
		+ m23 * m42 * m34
		+ m24 * m32 * m43
		- m24 * m42 * m33;
	inverse[4] = -m12 * m33 * m44
		+ m12 * m43 * m34
		+ m13 * m32 * m44
		- m13 * m42 * m34
		- m14 * m32 * m43
		+ m14 * m42 * m33;
	inverse[8] = m12 * m23 * m44
		- m12 * m43 * m24
		- m13 * m22 * m44
		+ m13 * m42 * m24
		+ m14 * m22 * m43
		- m14 * m42 * m23;
	inverse[12] = -m12 * m23 * m34
		+ m12 * m33 * m24
		+ m13 * m22 * m34
		- m13 * m32 * m24
		- m14 * m22 * m33
		+ m14 * m32 * m23;
	inverse[1] = -m21 * m33 * m44
		+ m21 * m43 * m34
		+ m23 * m31 * m44
		- m23 * m41 * m34
		- m24 * m31 * m43
		+ m24 * m41 * m33;
	inverse[5] =m11 * m33 * m44
		-m11 * m43 * m34
		- m13 * m31 * m44
		+ m13 * m41 * m34
		+ m14 * m31 * m43
		- m14 * m41 * m33;
	inverse[9] = -m11 * m23 * m44
		+m11 * m43 * m24
		+ m13 * m21 * m44
		- m13 * m41 * m24
		- m14 * m21 * m43
		+ m14 * m41 * m23;
	inverse[13] =m11 * m23 * m34
		-m11 * m33 * m24
		- m13 * m21 * m34
		+ m13 * m31 * m24
		+ m14 * m21 * m33
		- m14 * m31 * m23;
	inverse[2] = m21 * m32 * m44
		- m21 * m42 * m34
		- m22 * m31 * m44
		+ m22 * m41 * m34
		+ m24 * m31 * m42
		- m24 * m41 * m32;
	inverse[6] = -m11 * m32 * m44
		+m11 * m42 * m34
		+ m12 * m31 * m44
		- m12 * m41 * m34
		- m14 * m31 * m42
		+ m14 * m41 * m32;
	inverse[10] =m11 * m22 * m44
		-m11 * m42 * m24
		- m12 * m21 * m44
		+ m12 * m41 * m24
		+ m14 * m21 * m42
		- m14 * m41 * m22;
	inverse[14] = -m11 * m22 * m34
		+m11 * m32 * m24
		+ m12 * m21 * m34
		- m12 * m31 * m24
		- m14 * m21 * m32
		+ m14 * m31 * m22;
	inverse[3] = -m21 * m32 * m43
		+ m21 * m42 * m33
		+ m22 * m31 * m43
		- m22 * m41 * m33
		- m23 * m31 * m42
		+ m23 * m41 * m32;
	inverse[7] =m11 * m32 * m43
		-m11 * m42 * m33
		- m12 * m31 * m43
		+ m12 * m41 * m33
		+ m13 * m31 * m42
		- m13 * m41 * m32;
	inverse[11] = -m11 * m22 * m43
		+m11 * m42 * m23
		+ m12 * m21 * m43
		- m12 * m41 * m23
		- m13 * m21 * m42
		+ m13 * m41 * m22;
	inverse[15] =m11 * m22 * m33
		-m11 * m32 * m23
		- m12 * m21 * m33
		+ m12 * m31 * m23
		+ m13 * m21 * m32
		- m13 * m31 * m22;
	inverted_determinant = 1.0f / (m11 * inverse[0] + m21 * inverse[4] + m31 * inverse[8] + m41 * inverse[12]);
	result[0] = inverse[0] * inverted_determinant;
	result[1] = inverse[1] * inverted_determinant;
	result[2] = inverse[2] * inverted_determinant;
	result[3] = inverse[3] * inverted_determinant;
	result[4] = inverse[4] * inverted_determinant;
	result[5] = inverse[5] * inverted_determinant;
	result[6] = inverse[6] * inverted_determinant;
	result[7] = inverse[7] * inverted_determinant;
	result[8] = inverse[8] * inverted_determinant;
	result[9] = inverse[9] * inverted_determinant;
	result[10] = inverse[10] * inverted_determinant;
	result[11] = inverse[11] * inverted_determinant;
	result[12] = inverse[12] * inverted_determinant;
	result[13] = inverse[13] * inverted_determinant;
	result[14] = inverse[14] * inverted_determinant;
	result[15] = inverse[15] * inverted_determinant;
	return result;
}

static inline float *
lu_mat4_lerp(float *result, float *m0, float *m1, float f)
{
	result[0] = m0[0] + (m1[0] - m0[0]) * f;
	result[1] = m0[1] + (m1[1] - m0[1]) * f;
	result[2] = m0[2] + (m1[2] - m0[2]) * f;
	result[3] = m0[3] + (m1[3] - m0[3]) * f;
	result[4] = m0[4] + (m1[4] - m0[4]) * f;
	result[5] = m0[5] + (m1[5] - m0[5]) * f;
	result[6] = m0[6] + (m1[6] - m0[6]) * f;
	result[7] = m0[7] + (m1[7] - m0[7]) * f;
	result[8] = m0[8] + (m1[8] - m0[8]) * f;
	result[9] = m0[9] + (m1[9] - m0[9]) * f;
	result[10] = m0[10] + (m1[10] - m0[10]) * f;
	result[11] = m0[11] + (m1[11] - m0[11]) * f;
	result[12] = m0[12] + (m1[12] - m0[12]) * f;
	result[13] = m0[13] + (m1[13] - m0[13]) * f;
	result[14] = m0[14] + (m1[14] - m0[14]) * f;
	result[15] = m0[15] + (m1[15] - m0[15]) * f;
	return result;
}

static inline float *
lu_mat4_look_at(float *result, float *position, float *target, float *up)
{
	float tmp_forward[3];
	float tmp_side[3];
	float tmp_up[3];
	lu_vec3_subtract(tmp_forward, target, position);
	lu_vec3_normalize(tmp_forward, tmp_forward);
	lu_vec3_cross(tmp_side, tmp_forward, up);
	lu_vec3_normalize(tmp_side, tmp_side);
	lu_vec3_cross(tmp_up, tmp_side, tmp_forward);
	result[0] = tmp_side[0];
	result[1] = tmp_up[0];
	result[2] = -tmp_forward[0];
	result[3] = 0.0f;
	result[4] = tmp_side[1];
	result[5] = tmp_up[1];
	result[6] = -tmp_forward[1];
	result[7] = 0.0f;
	result[8] = tmp_side[2];
	result[9] = tmp_up[2];
	result[10] = -tmp_forward[2];
	result[11] = 0.0f;
	result[12] = -lu_vec3_dot(tmp_side, position);
	result[13] = -lu_vec3_dot(tmp_up, position);
	result[14] = lu_vec3_dot(tmp_forward, position);
	result[15] = 1.0f;
	return result;
}

static inline float *
lu_mat4_ortho(float *result, float l, float r, float b, float t, float n, float f)
{
	result[0] = 2.0f / (r - l);
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	result[4] = 0.0f;
	result[5] = 2.0f / (t - b);
	result[6] = 0.0f;
	result[7] = 0.0f;
	result[8] = 0.0f;
	result[9] = 0.0f;
	result[10] = -2.0f / (f - n);
	result[11] = 0.0f;
	result[12] = -((r + l) / (r - l));
	result[13] = -((t + b) / (t - b));
	result[14] = -((f + n) / (f - n));
	result[15] = 1.0f;
	return result;
}

static inline float *
lu_mat4_perspective(float *result, float fov_y, float aspect, float n, float f)
{
	float tan_half_fov_y = 1.0f / tanf(fov_y * 0.5f);
	result[0] = 1.0f / aspect * tan_half_fov_y;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	result[4] = 0.0f;
	result[5] = 1.0f / tan_half_fov_y;
	result[6] = 0.0f;
	result[7] = 0.0f;
	result[8] = 0.0f;
	result[9] = 0.0f;
	result[10] = f / (n - f);
	result[11] = -1.0f;
	result[12] = 0.0f;
	result[13] = 0.0f;
	result[14] = -(f * n) / (f - n);
	result[15] = 0.0f;
	return result;
}

static inline float *
lu_mat4_perspective_fov(float *result, float fov, float w, float h, float n, float f)
{
	float h2 = lu_cos(fov * 0.5f) / lu_sin(fov * 0.5f);
	float w2 = h2 * h / w;
	result[0] = w2;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	result[4] = 0.0f;
	result[5] = h2;
	result[6] = 0.0f;
	result[7] = 0.0f;
	result[8] = 0.0f;
	result[9] = 0.0f;
	result[10] = f / (n - f);
	result[11] = -1.0f;
	result[12] = 0.0f;
	result[13] = 0.0f;
	result[14] = -(f * n) / (f - n);
	result[15] = 0.0f;
	return result;
}

static inline float *
lu_mat4_perspective_infinite(float *result, float fov_y, float aspect, float n)
{
	float range = tanf(fov_y * 0.5f) * n;
	float left = -range * aspect;
	float right = range * aspect;
	float top = range;
	float bottom = -range;
	result[0] = 2.0f * n / (right - left);
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;
	result[4] = 0.0f;
	result[5] = 2.0f * n / (top - bottom);
	result[6] = 0.0f;
	result[7] = 0.0f;
	result[8] = 0.0f;
	result[9] = 0.0f;
	result[10] = -1.0f;
	result[11] = -1.0f;
	result[12] = 0.0f;
	result[13] = 0.0f;
	result[14] = -2.0f * n;
	result[15] = 0.0f;
	return result;
}

#endif /* __LLULU_MATH_H__ */
