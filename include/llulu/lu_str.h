#ifndef LLULU_LU_STR_H
#define LLULU_LU_STR_H

#include "lu_hash.h"
#include "lu_math.h"
#include "lu_error.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Small string. */
#define LU_SSTRLEN 32

#ifdef __cplusplus
#define restrict 
extern "C" {
#endif

typedef struct lu_sstr {
    char str[LU_SSTRLEN];
} lu_sstr;

static inline void
lu_sstr_clear(lu_sstr *self)
{
    *self = (lu_sstr){0};
}

static inline bool
lu_sstr_is_empty(const lu_sstr *self)
{
    return !self->str[0];
}

static inline bool
lu_sstr_equals(const lu_sstr *self, const lu_sstr *other)
{ 
    return ((size_t*)self)[0] == ((size_t*)other)[0] &&
           ((size_t*)self)[1] == ((size_t*)other)[1] &&
           ((size_t*)self)[2] == ((size_t*)other)[2] &&
           ((size_t*)self)[3] == ((size_t*)other)[3];
}

static inline void
lu_sstr_copy(lu_sstr *dst, const char *src)
{
    lu_sstr_clear(dst);
    strncpy(dst->str, src, LU_SSTRLEN - 1);
}

static inline lu_sstr *
lu_sstr_memcpy(lu_sstr *dst, const void *src, size_t bytes)
{
    lu_sstr_clear(dst);
    memcpy(dst->str, src, lu_minu(bytes, LU_SSTRLEN - 1));
    return dst;
}

static inline int
lu_sstr_cmpstr(const lu_sstr *self, const char *other)
{
    return strncmp(self->str, other, LU_SSTRLEN);
}

static inline uint64_t
lu_sstr_hash(lu_sstr *self)
{
    return lu_hash_32bytes((const uint8_t*)self);
}

#ifdef __cplusplus
}
#endif
#endif /* LLULU_LU_STR_H */

