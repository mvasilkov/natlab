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
   It should have a period of 2 to the 63, exactly, since the state changes
   to be each odd 64-bit unsigned integer before cycling.

   Testing on MinGW64 on Windows 7, with an i7-6700HQ quad-core mobile
   processor, it outperforms Xoroshiro128+ (narrowly) at 7.570 GB/s for
   Thrust vs. 6.820 GB/s for Xoroshiro128+. This seems dependent on how
   well it is allowed to use the processor; on a virtual machine permitted
   two cores of the same processor, Thrust performed miserably compared to
   Xoroshiro, though it was roughly 1.5x faster than SplitMix64. It may
   currently pass BigCrush with the given constants, or may require some
   adjustments; only one failure has been found on a different (but close)
   constant increment.
   
   Thanks to Sebastiano Vigna, who helped find weak points in earlier
   iterations of the generator that led to the current version.
 */

uint64_t x; /* The state can be seeded with any odd value, so it's recommended
               that you run `x |= 1ULL` when setting the state with arbitrary
	       or user-entered data. */
 
/* Call next() to get 64 pseudo-random bits, call it again to get more bits. */
// It may help to make this inline, but you should see if it benefits your code.
uint64_t next(void) {
	const uint64_t s = x;
	const uint64_t z = (s ^ s >> 26) * (x += 0x6A5D39EAE116586AULL);
	return z ^ (z >> 28);
}

/* The one constant currently used is intentionally even, and is very close to
   a prime that I had empirically found good results for in a hashing function.
   Slight changes to the constant 0x6A5D39EAE116586A seem to affect BigCrush
   results somewhat; an earlier try using 0x6A5D39EAE1165866 had one failure
   (just barely) on one seed, but the current constant had fewer anomalies in
   PractRand testing. This passes PractRand with no failures and few anomalies
   up to at least 64GB, and based on similar attempts that were tested longer,
   it can probably pass much more (at least 1TB is expected to pass). If you
   have access to statistical tests for RNGs, you are encouraged to try other
   constants if you feel you can improve on the ones used, or have a
   theoretical foundation for a better choice.
   
   You can check the revision history on this Gist to see previous versions.
   This was changed to use a variable multiplier on October 19, 2017.
   Versions before that can be seen in the revision history, but they don't
   seem to hold out against PractRand for very long at all, failing at 32MB
   on Gap-16 tests (many thanks to Sebastiano Vigna for spotting this). The
   new version seems to be slightly faster and has higher quality as well.
 */
