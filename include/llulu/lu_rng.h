#ifndef LLULU_LU_RNG_H
#define LLULU_LU_RNG_H

#include <stdint.h>

/*
 *  PRNG algorithms copied from STC's crand64.
 *  This file may have nothing to do with the original and does not intend to be a faithful representation.
 *  The correct implementation can be found in random.h from https://github.com/stclib/STC.
 */


typedef struct lu_rng {
    uint64_t state[4];
} lu_rng;

static inline void
lu_rng_seed(lu_rng *rng, uint64_t seed)
{
    uint64_t* s = rng->state;
    s[0] = seed * 0x9E3779B97F4A7C15;
    s[0] ^= s[0] >> 30;
    s[1] = s[0] * 0xBF58476D1CE4E5B9;
    s[1] ^= s[1] >> 27;
    s[2] = s[1] * 0x94D049BB133111EB;
    s[2] ^= s[2] >> 31;
    s[3] = seed;
}

static inline uint64_t 
lu_rng_get(lu_rng *rng, uint64_t stream)
{
    uint64_t* s = rng->state;
    const uint64_t result = (s[0] ^ (s[3] += stream)) + s[1];
    s[0] = s[1] ^ (s[1] >> 11);
    s[1] = s[2] + (s[2] << 3);
    s[2] = ((s[2] << 24) | (s[2] >> 40)) + result;
    return result;
}

static inline double
lu_rng_getf(lu_rng *rng, uint64_t stream)
{
    return (double)(lu_rng_get(rng, stream) >> 11) * 0x1.0p-53;
}

/**
 * Fill the buffer with random characters.
 * @return One past the last written char, useful when null terminated text is desired.
 *         e.g. *lu_rng_get_text(&rng, buf, 14) = '\0';
 */
static inline char *lu_rng_get_text(lu_rng *rng, uint64_t stream, char *buf, size_t count)
{
    const char *lut = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    uint64_t rand = 0;

    while (count--) {
        rand = rand < 4 ? lu_rng_get(rng, stream) : rand >> 6;
        *buf++ = lut[rand & 0x3F];
    }
    return buf;
}

/* Global single sequence generator functions */

static inline lu_rng *lu_rng_g(void)
{
    static lu_rng rng = {{0X9E3779BB07979AF0,0X6F682616BAE3641A,0XE220A8397B1DCDAF,0X1}};
    return &rng;
}

static inline void
lu_rng_seedg(uint64_t seed)
{
    lu_rng_seed(lu_rng_g(), seed);
}

static inline uint64_t lu_rng_getg(void)
{
    return lu_rng_get(lu_rng_g(), 1);
}

static inline uint64_t lu_rng_getfg(void)
{
    return lu_rng_getf(lu_rng_g(), 1);
}

static inline char *lu_rng_textg(char *buf, size_t count)
{
    return lu_rng_get_text(lu_rng_g(), 1, buf, count);
}

#endif /* LLULU_LU_RNG_H */
