#ifndef __LLULU_MATH_H__
#define __LLULU_MATH_H__

#include <stddef.h>

static inline float lu_minf(float a, float b) { return a < b ? a : b; }
static inline float lu_maxf(float a, float b) { return a >= b ? a : b; }
static inline float lu_clampf(float v, float min, float max) { return (v < min) ? min : (v > max) ? max : v; }
static inline float lu_lerpf(float a, float b, float t) { return a + (b - a) * t; }

static inline int lu_mini(int a, int b) { return a < b ? a : b; }
static inline int lu_maxi(int a, int b) { return a >= b ? a : b; }

static inline size_t lu_minu(size_t a, size_t b) { return a < b ? a : b; }
static inline size_t lu_maxu(size_t a, size_t b) { return a >= b ? a : b; }

#endif /* __LLULU_MATH_H__ */
