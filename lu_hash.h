#include <stdint.h>

/* xxHash */
#define LU_32PRIME1 0x9E3779B1U
#define LU_32PRIME2 0x85EBCA77U
#define LU_32PRIME3 0xC2B2AE3DU
#define LU_32PRIME4 0x27D4EB2FU
#define LU_32PRIME5 0x165667B1U
#define LU_64PRIME1 0x9E3779B185EBCA87ULL
#define LU_64PRIME2 0xC2B2AE3D27D4EB4FULL
#define LU_64PRIME3 0x165667B19E3779F9ULL
#define LU_64PRIME4 0x85EBCA77C2B2AE63ULL
#define LU_64PRIME5 0x27D4EB2F165667C5ULL
#define LU_PRIME_MX1 0x165667919E3779F9ULL
#define LU_PRIME_MX2 0x9FB21C651E98DF25ULL

static const uint8_t LU_SECRET[] = {
    0xb8, 0xfe, 0x6c, 0x39, 0x23, 0xa4, 0x4b, 0xbe, 0x7c, 0x01, 0x81,
    0x2c, 0xf7, 0x21, 0xad, 0x1c, 0xde, 0xd4, 0x6d, 0xe9, 0x83, 0x90,
    0x97, 0xdb, 0x72, 0x40, 0xa4, 0xa4, 0xb7, 0xb3, 0x67, 0x1f};

static inline uint32_t lu_hash_rotl32(uint32_t v, uint32_t r) {
    return (v << r) | (v >> (32 - r));
}

static inline uint64_t lu_hash_rotl64(uint64_t v, uint64_t r) {
    return (v << r) | (v >> (64 - r));
}

static inline uint64_t lu_hash_round64(uint64_t acc, uint64_t v) {
    acc += v * LU_64PRIME2;
    acc = lu_hash_rotl64(acc, 31);
    return acc * LU_64PRIME1;
}

static inline uint64_t lu_hash_mergeround64(uint64_t acc, uint64_t v) {
    v = lu_hash_round64(0, v);
    acc ^= v;
    return acc * LU_64PRIME1 + LU_64PRIME4;
}

static inline uint64_t lu_hash_avalanche(uint64_t hash) {
    hash ^= (hash >> 33);
    hash *= LU_64PRIME2;
    hash ^= (hash >> 29);
    hash *= LU_64PRIME3;
    return hash ^ (hash >> 32);
}

/* For partially mixed input bits. */
static inline uint64_t lu_hash_avalanche_fast(uint64_t hash) {
    hash = hash ^ (hash >> 37);
    hash *= LU_PRIME_MX1;
    return hash ^ (hash >> 32);
}

/* Pelle Evensen's rrmxmx. */
static inline uint64_t lu_hash_avalanche_strong(uint64_t hash, uint64_t len) {
    hash ^= lu_hash_rotl64(hash, 49) ^ lu_hash_rotl64(hash, 24);
    hash *= LU_PRIME_MX2;
    hash ^= (hash >> 35) + len;
    hash *= LU_PRIME_MX2;
    return hash ^ (hash >> 28);
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
