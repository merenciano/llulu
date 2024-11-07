#ifndef __LUC_STRING_32_H__
#define __LUC_STRING_32_H__

#include "luc_hash.h"
#include "luc_math.h"

#include <stdint.h>
#include <string.h>

#define LU_STRLEN 32

typedef struct lu_str32 {
    char str[LU_STRLEN];
} lu_str32;

#define lu_str32_from_literal(LITERAL)                                         \
    (lu_str32) { .str = LITERAL }

static inline void lu_str32_clear(lu_str32 *self) {
    memset(self->str, 0, LU_STRLEN);
}

static inline int lu_str32_isempty(lu_str32 *self) { return !self->str[0]; }

static inline void lu_str32_cpy(lu_str32 *dst, lu_str32 *src) { *dst = *src; }

static inline void lu_str32_cpystr(lu_str32 *dst, const char *src) {
    lu_str32_clear(dst);
    strncpy(dst->str, src, LU_STRLEN - 1);
}

static inline void lu_str32_cpymem(lu_str32 *dst, const void *src, size_t bytes) {
    lu_str32_clear(dst);
    memcpy(dst->str, src, lu_minu(bytes, LU_STRLEN - 1));
}

static inline int lu_str32_cmp(lu_str32 *self, lu_str32 *other) {
    return memcmp(self->str, other->str, LU_STRLEN);
}

static inline int lu_str32_cmpstr(lu_str32 *self, const char *other) {
    return strncmp(self->str, other, LU_STRLEN);
}

static inline int lu_str32_cmpmem(lu_str32 *self, const void *other, size_t bytes) {
    return memcmp(self->str, other, lu_minu(bytes, LU_STRLEN));
}

static inline uint64_t lu_str32_hash(lu_str32 *self) {
    return lu_hash_str32((const uint8_t *)self->str);
}

#endif // __LUC_STRING_32_H__
