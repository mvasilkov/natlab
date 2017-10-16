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
   BigCrush, though it passes SmallCrush and Crush without any failures.
   I haven't been able to build the TestU01 suite on Windows, so I need to
   run TestU01 on the much-slower VM, and have only been able to try one
   seed so far. On an earlier version, there was one failure on the one
   tested seed, but some key parts have since improved regarding quality.
   There may be more information soon if someone can run the BigCrush tests
   on Thrust with the full 100 seeds, and also in reverse. My VM does not
   seem capable of completing that this year.
 */

uint64_t x; /* The state can be seeded with any value. */
 
/* Call next() to get 64 pseudo-random bits, call it again to get more bits. */
// ASIDE: Edited to remove "static inline" modifiers; that is appropriate for the
// speed test this was brought in from, but not normal code. "inline" may be
// appropriate, but you can be the judge of that.
uint64_t next(void) {
	x += 0x9E3779B97F4A7C15ULL; // golden-ratio-related
	const uint64_t z = (x ^ (x >> 26)) * 0x2545F4914F6CDD1DULL; // L'Ecuyer
	return z ^ (z >> 28);
}

/* The first "magic number" is common in hashing-related code, and is related to
   the golden ratio (or its inverse) and either 2 to the 64 or 2 to the 63; I've
   seen it often, including in splitmix64.c , and the various shift amounts were
   adjusted to fit this particular increment and the next magic number.
   
   The second "magic number" is from L'Ecuyer's 1999 paper
   https://www.researchgate.net/publication/220577404_Tables_of_linear_congruential_generators_of_different_sizes_and_good_lattice_structure
   and was selected after evaluating several constants from that paper. This is not
   a linear congruential generator, but some steps act like one. The number is shown
   in base 10 in the paper as 2685821657736338717, and is in Table 5 under the section
   where m is 2 to the 64.
   
   You can check the revision history on this Gist to see previous versions.
   Until October 16, this generator used a different multiplier and a different
   shift value in one place. What is currently:
   const uint64_t z = (x ^ (x >> 26)) * 0x2545F4914F6CDD1DULL;
   was:
   const uint64_t z = (x ^ (x >> 30)) * 0x5851F42D4C957F2DULL;
   The changes were made to improve quality and to ensure the constant multiplier
   is what I thought it was. The multiplier was from some other source before, but
   now is confirmed from L'Ecuyer's 1999 paper.
 */
