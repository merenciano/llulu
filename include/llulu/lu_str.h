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
    lu_err_expects(self);
    *self = (lu_sstr){0};
    lu_err_ensures(!(((size_t*)self)[0] | ((size_t*)self)[1] | ((size_t*)self)[2] | ((size_t*)self)[3]));
}

static inline bool
lu_sstr_empty(const lu_sstr *self)
{
    lu_err_expects(self);
    return !self->str[0];
}

static inline bool
lu_sstr_equal(const lu_sstr *self, const lu_sstr *other)
{ 
    lu_err_expects(self && other);
    return !memcmp(self, other, LU_SSTRLEN);
}

static inline bool
lu_sstr_equal_0(const lu_sstr *self, const lu_sstr *other)
{ 
    lu_err_expects(self && other);
    return ((size_t*)self)[0] == ((size_t*)other)[0] &&
           ((size_t*)self)[1] == ((size_t*)other)[1] &&
           ((size_t*)self)[2] == ((size_t*)other)[2] &&
           ((size_t*)self)[3] == ((size_t*)other)[3];
}

static inline void
lu_sstr_copy(lu_sstr *dst, const char *src)
{
    lu_err_expects(dst && src);
    lu_sstr_clear(dst);
    strncpy(dst->str, src, LU_SSTRLEN - 1);
}

static inline void
lu_sstr_copy_0(lu_sstr * restrict dst, const char * restrict src)
{
    lu_err_expects(dst && src);
    lu_sstr_clear(dst);

    for (int i = 0; i < (LU_SSTRLEN - 1) && *src; ++i) {
        dst->str[i] = *src++;
    }
}

static inline lu_sstr * 
lu_sstr_memcpy(lu_sstr *dst, const void *src, size_t bytes)
{
    lu_err_expects(dst && src);
    lu_sstr_clear(dst);
    memcpy(dst->str, src, lu_minu(bytes, LU_SSTRLEN - 1));
    return dst;
}

static inline lu_sstr *
lu_sstr_memcpy_0(lu_sstr * restrict dst, const void * restrict src, size_t bytes)
{
    lu_err_expects(dst && src);
    if (!bytes || &dst->str[0] == src) {
        return dst;
    }

    lu_sstr_clear(dst);
    bytes = lu_minu(bytes, LU_SSTRLEN - 1);
    unsigned int count = bytes >> 3;
    for (unsigned i = 0; i < count; ++i) {
        ((size_t*)dst)[i] = ((size_t*)src)[i];
    }

    unsigned int rem = bytes & 7;
    for (unsigned i = count << 3; i < rem; ++i) {
        dst->str[i] = ((char*)src)[i];
    }
    return dst;
}

static inline int
lu_sstr_cmpstr(const lu_sstr *self, const char *other)
{
    lu_err_expects(self && other);
    return strncmp(self->str, other, LU_SSTRLEN);
}

static inline int
lu_sstr_cmpstr_0(const lu_sstr *self, const char *other)
{
    lu_err_expects(self && other);
    const char *s = self->str;
    int d = *s - *other;

    for (int i = 0; i < LU_SSTRLEN && !d; ++i) {
        if (!*s) {
            return d;
        }
        d = *s++ - *other++;
    }
    return d;
}

static inline int
lu_sstr_cmpmem(const lu_sstr *self, const void *other, size_t bytes)
{
    lu_err_expects(self && other);
    return memcmp(self->str, other, lu_minu(bytes, LU_SSTRLEN));
}

static inline int
lu_sstr_cmpmem_0(const lu_sstr *self, const void *other, size_t bytes)
{
    lu_err_expects(self && other);
    const int count = lu_minu(bytes, LU_SSTRLEN);

    const char *s = self->str;
    const char *o = (const char*)other;
    int d = *s - *o;

    for (int i = 0; i < count && !d; ++i) {
        if (!*s) {
            return d;
        }
        d = *s++ - *o++;
    }
    return d;
}

static inline uint64_t
lu_sstr_hash(lu_sstr *self)
{
    lu_err_expects(self);
    return lu_hash_32bytes((const uint8_t*)self);
}

#ifdef __cplusplus
}
#endif
#endif /* LLULU_LU_STR_H */

