#ifndef PASTE_XOROSHIRO_128PLUS_H
#define PASTE_XOROSHIRO_128PLUS_H

#include <stdint.h>

/**
 * Makes random number under 0.72ns.
 * This algorithm is originally developed
 * by David Blackman and Sebastiano Vigna (vigna@acm.org)
 * @see http://xoroshiro.di.unimi.it/xoroshiro128plus.c
 */
struct xoroshiro_128plus{
	uint64_t s[2];
};

static uint64_t split_mix64(uint64_t seed) {
	uint64_t z = (seed += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
	return z ^ (z >> 31);
}

static struct xoroshiro_128plus init_xoroshiro_128plus(uint64_t seed){
	struct xoroshiro_128plus r;
	r.s[0] = seed;
	r.s[1] = split_mix64(seed);
	return r;
}

static inline uint64_t rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

static uint64_t next(struct xoroshiro_128plus *r) {
	const uint64_t s0 = r->s[0];
	uint64_t s1 = r->s[1];
	const uint64_t result = s0 + s1;

	s1 ^= s0;
	r->s[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16);  // a, b
	r->s[1] = rotl(s1, 37);                    // c

	return result;
}




#endif // PASTE_XOROSHIRO_128PLUS_H
