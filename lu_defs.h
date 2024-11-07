#ifndef __LU_DEFS_H__
#define __LU_DEFS_H__

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

#define LU_ARR_COUNT(ARR) (sizeof(ARR) / sizeof(*(ARR)))

#endif /* __LU_DEFS_H__ */
