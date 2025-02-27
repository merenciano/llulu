#ifndef __LLULU_DEFS_H__
#define __LLULU_DEFS_H__

#if defined(LU_STRINGIFY_EX)
#undef LU_STRINGIFY_EX
#endif
#define LU_STRINGIFY_EX(x) #x

#if defined(LU_STRINGIFY)
#undef LU_STRINGIFY
#endif
#define LU_STRINGIFY(x) LU_STRINGIFY_EX(x)

#ifdef LU_RESTRICT
#undef LU_RESTRICT
#endif
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) &&              \
    !defined(__cplusplus)
#define LU_RESTRICT restrict
#elif defined(__clang__)
#define LU_RESTRICT __restrict
#else
#define LU_RESTRICT
#endif

#define LU_ARR_SIZE(ARR) (sizeof(ARR) / sizeof(ARR[0]))
#define LU_IS_POW2(X) ((X > 0) && !((X) & ((X) - 1)))

#endif /* __LLULU_DEFS_H__ */
