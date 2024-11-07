#ifndef __LUC_HASH_H__
#define __LUC_HASH_H__

#include <stdint.h>

/* xxHash implementation */

static inline uint64_t lu_hash_avalanche(uint64_t hash) {
    static const uint64_t PRIME = 0x165667919E3779F9ULL;
    hash = hash ^ (hash >> 37);
    hash *= PRIME;
    return hash ^ (hash >> 32);
}

static inline uint64_t lu_hash_mul128_fold64(uint64_t lhs, uint64_t rhs) {
    __uint128_t const v = (__uint128_t)lhs * (__uint128_t)rhs;
    return v ^ (v >> 64);
}

static inline uint64_t lu_hash_mix16(const uint8_t *data, const uint8_t *sec,
                                     uint64_t seed) {
    uint64_t lo = *(uint64_t *)data;
    uint64_t hi = *(uint64_t *)(data + 8);
    return lu_hash_mul128_fold64(lo ^ (*(uint64_t *)sec + seed),
                                 hi ^ (*(uint64_t *)(sec + 8) - seed));
}

static inline uint64_t lu_hash_str32(const uint8_t *data) {
    static const uint64_t PRIME = 0x9E3779B185EBCA87ULL;
    static const uint8_t SECRET[] = {
        0xb8, 0xfe, 0x6c, 0x39, 0x23, 0xa4, 0x4b, 0xbe, 0x7c, 0x01, 0x81,
        0x2c, 0xf7, 0x21, 0xad, 0x1c, 0xde, 0xd4, 0x6d, 0xe9, 0x83, 0x90,
        0x97, 0xdb, 0x72, 0x40, 0xa4, 0xa4, 0xb7, 0xb3, 0x67, 0x1f};

    uint64_t acc = 32 * PRIME;
    acc += lu_hash_mix16(data, SECRET, 0);
    acc += lu_hash_mix16((data + 16), SECRET + 16, 0);
    return lu_hash_avalanche(acc);
}

#endif /* __LUC_HASH_H__ */
