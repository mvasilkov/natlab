/*  Written in 2017 by Tommy Ettinger (tommy.ettinger@gmail.com)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include <stdint.h>

/* This is a modified (stripped-down) fixed-increment version of Java 8's
   SplittableRandom generator, with significantly less "cipher-like" work
   done but with the parameters adjusted to fit the fixed increment well.
   It is meant for speed. See http://dx.doi.org/10.1145/2714064.2660195 and
   http://docs.oracle.com/javase/8/docs/api/java/util/SplittableRandom.html .
   It should have a period of 2 to the 64, exactly, since the state changes
   to be each possible 64-bit unsigned integer before cycling.

   Testing on MinGW64 on Windows 7, with an i7-6700HQ mobile processor, it
   outperforms Xoroshiro128+ (narrowly) at 7.038 GB/s for Thrust vs. 6.820
   GB/s for Xoroshiro128+. This seems dependent on how well it is allowed
   to use the processor; on a virtual machine permitted two cores of the
   same processor, Thrust performed miserably compared to Xoroshiro, though
   it was roughly 1.5x faster than SplitMix64. I'm not sure if it "passes"
   BigCrush; I haven't been able to build the TestU01 suite on Windows, so
   I need to run it on the much-slower VM, and have only been able to try
   one seed so far. With that seed, it has one failure, but I'm not sure if
   the test is the one tolerable-to-fail test that detects only of a PRNG
   is an LFSR in some bits. If anyone understands TestU01's results, the
   failed test is "ClosePairs mNP, t = 16", with p-value 7.7e-4 . */

uint64_t x; /* The state can be seeded with any value. */
 
// Call next() to get 64 pseudo-random bits, call it again to get more bits.
static inline uint64_t next(void) {
	x += 0x9E3779B97F4A7C15ULL; // golden-ratio-related
	const uint64_t z = (x ^ (x >> 30)) * 0x5851F42D4C957F2DULL; // L'Ecuyer
	return z ^ (z >> 28);
}

/* The first "magic number" is common in hashing-related code, and is related to
   the golden ratio (or its inverse) and either 2 to the 64 or 2 to the 63; I've
   seen it often, including in splitmix64.c , and the various shift amounts were
   adjusted to fit this particular increment.
   
   The second "magic number" is from L'Ecuyer's 1999 paper
   https://www.researchgate.net/publication/220577404_Tables_of_linear_congruential_generators_of_different_sizes_and_good_lattice_structure
   and was selected after evaluating several constants from that paper. This is not
   a linear congruential generator, but some steps act like one.
 */
