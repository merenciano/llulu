#ifndef __LU_FIXED_STRING_H__
#define __LU_FIXED_STRING_H__

#include <stdint.h>
#include <string.h>

#define LU_STRLEN 32

typedef struct lu_str32 {
    char str[LU_STRLEN];
} lu_str32;

#define lu_str32_from_literal(LITERAL) (lu_str32){.str = LITERAL}

static inline void lu_str32_clear(lu_str32 *self) {
    memset(self->str, 0, LU_STRLEN);
}

static inline int lu_str32_isempty(lu_str32 *self) {
    return !self->str[0];
}

static inline void lu_str32_cpy(lu_str32 *dst, lu_str32 *src) {
    memcpy(dst, src, LU_STRLEN);
}

static inline void lu_str32_cpymem(lu_str32 *dst, const char *src) {
    lu_str32_clear(dst);
    strncpy(dst->str, src, LU_STRLEN);
}

static inline int lu_str32_cmp(lu_str32 *self, lu_str32 *other) {
    return memcmp(self->str, other->str, LU_STRLEN);
}

static inline int lu_str32_cmpmem(lu_str32 *self, const char *other) {
    return strncmp(self->str, other, LU_STRLEN);
}

static inline uint32_t lu_hash_rotleft(uint32_t v, uint8_t bits) {
    return (v << bits) | (v >> (32 - bits));
}

static inline uint32_t lu_str32_hash(lu_str32 *self, uint32_t seed) {
    static const uint32_t PRIMES[4] = {2654435761U, 2246822519U, 3266489917U};
    uint32_t state[4] = {
        seed + PRIMES[0] + PRIMES[1],
        seed + PRIMES[1],
        seed,
        seed - PRIMES[0]
    };

    const uint32_t *block = (const uint32_t*)self->str;
    state[0] = lu_hash_rotleft(state[0] + block[0] * PRIMES[1], 13) * PRIMES[0];
    state[1] = lu_hash_rotleft(state[1] + block[1] * PRIMES[1], 13) * PRIMES[0];
    state[2] = lu_hash_rotleft(state[2] + block[2] * PRIMES[1], 13) * PRIMES[0];
    state[3] = lu_hash_rotleft(state[3] + block[3] * PRIMES[1], 13) * PRIMES[0];

    state[0] = lu_hash_rotleft(state[0] + block[4] * PRIMES[1], 13) * PRIMES[0];
    state[1] = lu_hash_rotleft(state[1] + block[5] * PRIMES[1], 13) * PRIMES[0];
    state[2] = lu_hash_rotleft(state[2] + block[6] * PRIMES[1], 13) * PRIMES[0];
    state[3] = lu_hash_rotleft(state[3] + block[7] * PRIMES[1], 13) * PRIMES[0];

    uint32_t result = lu_hash_rotleft(state[0], 1) + lu_hash_rotleft(state[1], 7) + lu_hash_rotleft(state[2], 12) + lu_hash_rotleft(state[3], 18);
    result ^= result >> 15;
    result *= PRIMES[1];
    result ^= result >> 13;
    result *= PRIMES[2];
    result ^= result >> 16;
    return result;
}

#endif // __LU_FIXED_STRING_H__

