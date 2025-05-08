#ifndef __LLULU_DEFS_H__
#define __LLULU_DEFS_H__

#ifdef LU_INTERNAL_STRINGIFY_EX
#undef LU_INTERNAL_STRINGIFY_EX
#endif
#define LU_INTERNAL_STRINGIFY_EX(X) #X

#ifdef LU_STRINGIFY
#undef LU_STRINGIFY
#endif
#define LU_STRINGIFY(X) LU_INTERNAL_STRINGIFY_EX(X)

#ifdef LU_RESTRICT
#undef LU_RESTRICT
#endif
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) && !defined(__cplusplus)
#define LU_RESTRICT restrict
#elif defined(__clang__)
#define LU_RESTRICT __restrict
#else
#define LU_RESTRICT
#endif

#ifdef __has_builtin
#define LU_HAS_BUILTIN(BUILTIN) __has_builtin(BUILTIN)
#else
#define LU_HAS_BUILTIN(BUILTIN) (0)
#endif

#ifdef LU_IS_CONSTANT
#undef LU_IS_CONSTANT
#endif
#ifdef LU_INTERNAL_IS_CONSTEXPR
#undef LU_INTERNAL_IS_CONSTEXPR
#endif
#if LU_HAS_BUILTIN(__builtin_constant_p)
#define LU_IS_CONSTANT(EXPR) __builtin_constant_p(EXPR)
#endif
#ifndef __cplusplus
#if LU_HAS_BUILTIN(__builtin_types_compatible_p)
#include <stdint.h>
#define LU_INTERNAL_IS_CONSTEXPR(EXPR) __builtin_types_compatible_p(__typeof__((1 ? (void*) ((intptr_t) ((EXPR) * 0)) : (int*) 0)), int*)
#endif
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
#include <stdint.h>
#define LU_INTERNAL_IS_CONSTEXPR(EXPR) _Generic((1 ? (void*) ((intptr_t) * 0) : (int*) 0), int*: 1, void*: 0)
#elif defined(__clang__)
#define LU_INTERNAL_IS_CONSTEXPR(EXPR) (sizeof(void) != sizeof(*(1 ? ((void*) ((EXPR) * 0L) ) : ((struct { char v[sizeof(void) * 2]; } *) 1))))
#endif
#ifdef LU_INTERNAL_IS_CONSTEXPR
#ifndef LU_IS_CONSTANT
#define LU_IS_CONSTANT(EXPR) LU_INTERNAL_IS_CONSTEXPR(EXPR)
#endif
#else
#ifndef LU_IS_CONSTANT
#define LU_IS_CONSTANT(EXPR) (0)
#endif
#endif

#define LU_ASSERT_ASSIGNABLE_TYPE(TYPE, VALUE) ((TYPE){ 0 } = (VALUE))

#define LU_ARR_SIZE(ARR) (sizeof(ARR) / sizeof(ARR[0]))
#define LU_IS_POW2(X) ((X > 0) && !((X) & ((X) - 1)))

#define LU_VEC4(X, Y, Z, W) (lu_vec4){(X), (Y), (Z), (W)}
#define LU_VEC3(X, Y, Z) (lu_vec3){(X), (Y), (Z)}
#define LU_VEC2(X, Y) (lu_vec2){(X), (Y)}
#define LU_GET_VEC_EX(X, Y, Z, W, NAME, ...) NAME
#define LU_EXPAND(X) X
#define LU_VEC(...)                                                           \
    LU_EXPAND(LU_GET_VEC_EX(__VA_ARGS__, LU_VEC4, LU_VEC3, LU_VEC2)(__VA_ARGS__))

#endif /* __LLULU_DEFS_H__ */
