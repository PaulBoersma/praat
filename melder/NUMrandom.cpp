/* NUMrandom.cpp
 *
 * Copyright (C) 1992-2006,2008,2011,2012,2014-2018,2020,2025 Paul Boersma
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This code is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this work. If not, see <http://www.gnu.org/licenses/>.
 */

/* 
   A C-program for MT19937-64 (2014/2/23 version).
   Coded by Takuji Nishimura and Makoto Matsumoto.

   This is a 64-bit version of Mersenne Twister pseudorandom number
   generator.

   Before using, initialize the state by using init_genrand64(seed)  
   or init_by_array64(init_key, key_length).

   Copyright (C) 2004, 2014, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   References:
   T. Nishimura, ``Tables of 64-bit Mersenne Twisters''
     ACM Transactions on Modeling and 
     Computer Simulation 10. (2000) 348--357.
   M. Matsumoto and T. Nishimura,
     ``Mersenne Twister: a 623-dimensionally equidistributed
       uniform pseudorandom number generator''
     ACM Transactions on Modeling and 
     Computer Simulation 8. (Jan. 1998) 3--30.

   Any feedback is very welcome.
   http://www.math.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove spaces)
*/

#if defined (__MINGW32__) || defined (linux)
	#define UINT64_C(n)  n ## ULL
#endif
#include <unistd.h>
#include "melder.h"
#include <chrono>

#define NN  312
#define MM  156
#define MATRIX_A  UINT64_C (0xB5026F5AA96619E9)
#define UM  UINT64_C (0xFFFFFFFF80000000) /* Most significant 33 bits */
#define LM  UINT64_C (0x7FFFFFFF) /* Least significant 31 bits */

constexpr integer theNumberOfRandomGenerators = NUMrandom_maximumNumberOfParallelThreads + 3;

class NUMrandom_State { public:

	/** The state vector.
	 */
	uint64 array [NN];

	/** The pointer into the state vector.
		Equals NN + 1 iff the array has not been initialized.
	 */
	int index;
	NUMrandom_State () : index (NN + 1) {}
		// this initialization will lead to an immediate crash
		// when NUMrandomFraction() is called without NUMrandom_initXXX() having been called before;
		// without this initialization, it would be detected only after 312 calls to NUMrandomFraction()

	bool secondAvailable;
	double y;

	/**
		Initialize the whole array with one seed.
		This can be used for testing whether our implementation is correct (i.e. predicts the correct published sequence)
		and perhaps for generating reproducible sequences.
	 */
	uint64 init_genrand64 (uint64 seed) {
		array [0] = seed;
		for (index = 1; index < NN; index ++) {
			array [index] =
				(UINT64_C (6364136223846793005) * (array [index - 1] ^ (array [index - 1] >> 62))
				+ (uint64) index);
		}
		secondAvailable = false;   // ensure consistent starting state even after calling NUMrandomGauss an odd number of times
		return array [NN - 1];
	}

	/* initialize by an array with array-length */
	/* init_key is the array for initializing keys */
	/* key_length is its length */
	void init_by_array64 (uint64 init_key[], unsigned int key_length);

} states [theNumberOfRandomGenerators];

/* initialize the array with a number of seeds */
void NUMrandom_State :: init_by_array64 (uint64 init_key [], unsigned int key_length)
{
	init_genrand64 (UINT64_C (19650218));   // warm it up

	unsigned int i = 1, j = 0;
	unsigned int k = ( NN > key_length ? NN : key_length );
	for (; k; k --) {
		array [i] = (array [i] ^ ((array [i - 1] ^ (array [i - 1] >> 62)) * UINT64_C (3935559000370003845)))
				+ init_key [j] + (uint64) j;   // non-linear
		i ++;
		j ++;
		if (i >= NN) { array [0] = array [NN - 1]; i = 1; }
		if (j >= key_length) j = 0;
	}
	for (k = NN - 1; k; k --) {
		array [i] = (array [i] ^ ((array [i - 1] ^ (array [i - 1] >> 62)) * UINT64_C (2862933555777941757)))
				- (uint64) i;   // non-linear
		i ++;
		if (i >= NN) { array [0] = array [NN - 1]; i = 1; }
	}

	array [0] = UINT64_C (1) << 63;   // MSB is 1; assuring non-zero initial array
}

static uint64 getTicksSince1969 () {
	using namespace std::chrono;
	const auto timePoint = system_clock::now ();
	const auto duration = timePoint. time_since_epoch ();
	return uint64 (duration. count ());
	//Melder_casual (U"ticks since 1969: ", ticksSince1969);
}

static uint64 getTicksSinceBoot () {
	using namespace std::chrono;
	const auto timePoint = high_resolution_clock::now ();
	const auto duration = timePoint. time_since_epoch ();
	return uint64 (duration. count ());
	//Melder_casual (U"ticks since boot: ", ticksSinceBoot);
}

static bool theInited = false;
void NUMrandom_initializeSafelyAndUnpredictably () {
	const uint64 ticksSince1969 = getTicksSince1969 ();   // possibly microseconds
	const uint64 ticksSinceBoot = getTicksSinceBoot ();   // possibly nanoseconds
	for (int threadNumber = 0; threadNumber < theNumberOfRandomGenerators; threadNumber ++) {
		constexpr integer numberOfKeys = 7;
		uint64 keys [numberOfKeys];
		keys [0] = ticksSince1969;   // unique between boots of the same computer
		keys [1] = UINT64_C (7320321686725470078) + uint64 (threadNumber);   // unique between threads in the same process
		switch (threadNumber) {
			case  0: keys [2] = UINT64_C  (4492812493098689432); keys [3] = UINT64_C  (8902321878452586268); break;
			case  1: keys [2] = UINT64_C  (1875086582568685862); keys [3] = UINT64_C (12243257483652989599); break;
			case  2: keys [2] = UINT64_C  (9040925727554857487); keys [3] = UINT64_C  (8037578605604605534); break;
			case  3: keys [2] = UINT64_C (11168476768576857685); keys [3] = UINT64_C  (7862359785763816517); break;
			case  4: keys [2] = UINT64_C  (3878901748368466876); keys [3] = UINT64_C  (3563078257726526076); break;
			case  5: keys [2] = UINT64_C  (2185735817578415800); keys [3] = UINT64_C   (198502654671560756); break;
			case  6: keys [2] = UINT64_C (12248047509814562486); keys [3] = UINT64_C  (9836250167165762757); break;
			case  7: keys [2] = UINT64_C    (28362088588870143); keys [3] = UINT64_C  (8756376201767075602); break;
			case  8: keys [2] = UINT64_C  (5758130586486546775); keys [3] = UINT64_C  (4213784157469743413); break;
			case  9: keys [2] = UINT64_C  (8508416536565170756); keys [3] = UINT64_C  (2856175717654375656); break;
			case 10: keys [2] = UINT64_C  (2802356275260644756); keys [3] = UINT64_C  (2309872134087235167); break;
			case 11: keys [2] = UINT64_C   (230875784065064545); keys [3] = UINT64_C  (1209802371478023476); break;
			case 12: keys [2] = UINT64_C  (6520185868568714577); keys [3] = UINT64_C  (2173615001556504015); break;
			case 13: keys [2] = UINT64_C  (9082605608605765650); keys [3] = UINT64_C  (1204167447560475647); break;
			case 14: keys [2] = UINT64_C  (1238716515545475765); keys [3] = UINT64_C  (8435674023875847388); break;
			case 15: keys [2] = UINT64_C  (6127715675014756456); keys [3] = UINT64_C  (2435788450287508457); break;
			case 16: keys [2] = UINT64_C  (1081237546238975884); keys [3] = UINT64_C  (2939783238574293882); break;
			case 17: keys [2] = UINT64_C  (8375036306567876574); keys [3] = UINT64_C  (8540574357184754857); break;
			case 18: keys [2] = UINT64_C  (3850245655647557647); keys [3] = UINT64_C  (4364576692676547674); break;
			case 19: keys [2] = UINT64_C  (2057843567436577565); keys [3] = UINT64_C  (8438578345787893745); break;
			case 20: keys [2] = UINT64_C (10123890498732874894); keys [3] = UINT64_C (12091287549854854899); break;
			case 21: keys [2] = UINT64_C  (2318912478924387389); keys [3] = UINT64_C  (1209397927247838139); break;
			case 22: keys [2] = UINT64_C   (875843754798401029); keys [3] = UINT64_C (12030941286537547893); break;
			case 23: keys [2] = UINT64_C (10139892874834379012); keys [3] = UINT64_C  (9129821378428732498); break;
			case 24: keys [2] = UINT64_C   (919818490388575879); keys [3] = UINT64_C (10939848610941094810); break;
			case 25: keys [2] = UINT64_C  (9129839465819017879); keys [3] = UINT64_C  (9189102791272104190); break;
			case 26: keys [2] = UINT64_C  (4756874192190000982); keys [3] = UINT64_C  (3109570210921901080); break;
			case 27: keys [2] = UINT64_C (12183112903019492499); keys [3] = UINT64_C (12097659104981290001); break;
			case 28: keys [2] = UINT64_C  (9028903478201080929); keys [3] = UINT64_C  (4834934012959700481); break;
			case 29: keys [2] = UINT64_C  (2194569100499384010); keys [3] = UINT64_C  (1839749894208298429); break;
			case 30: keys [2] = UINT64_C  (3983209023438537234); keys [3] = UINT64_C  (7861159102090250989); break;
			case 31: keys [2] = UINT64_C  (2194849810939034937); keys [3] = UINT64_C  (3119014928483094870); break;
			case 32: keys [2] = UINT64_C  (3048135691040185393); keys [3] = UINT64_C   (192186502167157518); break;
			case 33: keys [2] = UINT64_C (12190902471094839890); keys [3] = UINT64_C  (8737946092598409589); break;
			case 34: keys [2] = UINT64_C  (3599358408609356095); keys [3] = UINT64_C  (8937295898493095840); break;
			case 35: keys [2] = UINT64_C    (45076835923445695); keys [3] = UINT64_C   (194634597229574509); break;
			case 36: keys [2] = UINT64_C   (836585748558470458); keys [3] = UINT64_C  (2289506565038756578); break;
			case 37: keys [2] = UINT64_C   (832563659023943438); keys [3] = UINT64_C  (1238264921469198284); break;
			case 38: keys [2] = UINT64_C (10393949889702093820); keys [3] = UINT64_C  (2198245910013109200); break;
			case 39: keys [2] = UINT64_C  (2001909384819394839); keys [3] = UINT64_C (13716710394194359394); break;
			case 40: keys [2] = UINT64_C (14966767595485485049); keys [3] = UINT64_C  (1889568829023490839); break;
			case 41: keys [2] = UINT64_C  (3856928438491014839); keys [3] = UINT64_C  (4411281012393465812); break;
			case 42: keys [2] = UINT64_C  (1093814878568456475); keys [3] = UINT64_C  (1938904975709295894); break;
			default: Melder_fatal (U"Thread number too high.");
		}
		keys [4] = (uint64) (int64) getpid ();   // unique between processes that run simultaneously on the same computer
		keys [5] = ticksSinceBoot;   // some extra randomness
		static uint64 callInstance = 0;
		keys [6] = UINT64_C (3642334578453) + (++ callInstance);
		states [threadNumber]. init_by_array64 (keys, numberOfKeys);
	}
	theInited = true;
}
void NUMrandom_initializeWithSeedUnsafelyButPredictably (uint64 seed) {
	for (int threadNumber = 0; threadNumber < theNumberOfRandomGenerators; threadNumber ++)
		seed = states [threadNumber]. init_genrand64 (seed + (uint64) threadNumber);
	theInited = true;
}

/* Throughout the years, several versions for "zero or magic" have been proposed. Choose the fastest. */

#define ZERO_OR_MAGIC_VERSION  3

#if ZERO_OR_MAGIC_VERSION == 1   // M&N 1999
	#define ZERO_OR_MAGIC  ( (x & UINT64_C (1)) ? MATRIX_A : UINT64_C (0) )
#elif ZERO_OR_MAGIC_VERSION == 2
	#define ZERO_OR_MAGIC  ( (x & UINT64_C (1)) * MATRIX_A )
#else   // M&N 2014
	constexpr uint64 mag01 [2] { UINT64_C (0), MATRIX_A };
	#define ZERO_OR_MAGIC  mag01 [(int) (x & UINT64_C (1))]
#endif

double NUMrandomFraction () {
	NUMrandom_State *me = & states [0];
	uint64 x;

	if (my index >= NN) {   // generate NN words at a time

		Melder_assert (theInited);   // if NUMrandom_initXXX() hasn't been called, we'll detect that here, probably in the first call

		int i;
		for (i = 0; i < NN - MM; i ++) {
			x = (my array [i] & UM) | (my array [i + 1] & LM);
			my array [i] = my array [i + MM] ^ (x >> 1) ^ ZERO_OR_MAGIC;
		}
		for (; i < NN - 1; i ++) {
			x = (my array [i] & UM) | (my array [i + 1] & LM);
			my array [i] = my array [i + (MM - NN)] ^ (x >> 1) ^ ZERO_OR_MAGIC;
		}
		x = (my array [NN - 1] & UM) | (my array [0] & LM);
		my array [NN - 1] = my array [MM - 1] ^ (x >> 1) ^ ZERO_OR_MAGIC;

		my index = 0;
	}

	x = my array [my index ++];

	x ^= (x >> 29) & UINT64_C (0x5555555555555555);
	x ^= (x << 17) & UINT64_C (0x71D67FFFEDA60000);
	x ^= (x << 37) & UINT64_C (0xFFF7EEE000000000);
	x ^= (x >> 43);

	return (x >> 11) * (1.0/9007199254740992.0);
}

double NUMrandomFraction_mt (int threadNumber) {
	NUMrandom_State *me = & states [threadNumber];
	uint64 x;

	if (my index >= NN) {   // generate NN words at a time

		Melder_assert (theInited);

		int i;
		for (i = 0; i < NN - MM; i ++) {
			x = (my array [i] & UM) | (my array [i + 1] & LM);
			my array [i] = my array [i + MM] ^ (x >> 1) ^ ZERO_OR_MAGIC;
		}
		for (; i < NN - 1; i ++) {
			x = (my array [i] & UM) | (my array [i + 1] & LM);
			my array [i] = my array [i + (MM - NN)] ^ (x >> 1) ^ ZERO_OR_MAGIC;
		}
		x = (my array [NN - 1] & UM) | (my array [0] & LM);
		my array [NN - 1] = my array [MM - 1] ^ (x >> 1) ^ ZERO_OR_MAGIC;

		my index = 0;
	}

	x = my array [my index ++];

	x ^= (x >> 29) & UINT64_C (0x5555555555555555);
	x ^= (x << 17) & UINT64_C (0x71D67FFFEDA60000);
	x ^= (x << 37) & UINT64_C (0xFFF7EEE000000000);
	x ^= (x >> 43);

	return (x >> 11) * (1.0/9007199254740992.0);
}

double NUMrandomUniform (double lowest, double highest) {
	return lowest + (highest - lowest) * NUMrandomFraction ();
}

integer NUMrandomInteger (integer lowest, integer highest) {
	return lowest + (integer) ((highest - lowest + 1) * NUMrandomFraction ());   // round down by truncation, because positive
}

bool NUMrandomBernoulli (double probability) {
	return NUMrandomFraction() < probability;
}

double NUMrandomBernoulli_real (double probability) {
	return (double) (NUMrandomFraction() < probability);
}

#define repeat  do
#define until(cond)  while (! (cond))
double NUMrandomGauss (double mean, double standardDeviation) {
	NUMrandom_State *me = & states [0];
	/*
		Knuth, p. 122.
	*/
	if (my secondAvailable) {
		my secondAvailable = false;
		return mean + standardDeviation * my y;
	} else {
		double s, x;
		repeat {
			x = 2.0 * NUMrandomFraction () - 1.0;   // inside the square [-1; 1] x [-1; 1]
			my y = 2.0 * NUMrandomFraction () - 1.0;
			s = x * x + my y * my y;
		} until (s < 1.0);   // inside the unit circle
		if (s == 0.0) {
			x = my y = 0.0;
		} else {
			const double factor = sqrt (-2.0 * log (s) / s);
			x *= factor;
			my y *= factor;
		}
		my secondAvailable = true;
		return mean + standardDeviation * x;
	}
}

double NUMrandomGauss_mt (int threadNumber, double mean, double standardDeviation) {
	NUMrandom_State *me = & states [threadNumber];
	/*
		Knuth, p. 122.
	*/
	if (my secondAvailable) {
		my secondAvailable = false;
		return mean + standardDeviation * my y;
	} else {
		double s, x;
		repeat {
			x = 2.0 * NUMrandomFraction_mt (threadNumber) - 1.0;   // inside the square [-1; 1] x [-1; 1]
			my y = 2.0 * NUMrandomFraction_mt (threadNumber) - 1.0;
			s = x * x + my y * my y;
		} until (s < 1.0);   // inside the unit circle
		if (s == 0.0) {
			x = my y = 0.0;
		} else {
			const double factor = sqrt (-2.0 * log (s) / s);
			x *= factor;
			my y *= factor;
		}
		my secondAvailable = true;
		return mean + standardDeviation * x;
	}
}

double NUMrandomPoisson (double mean) {
	/*
		The Poisson distribution is

			P(k) = mean^k * exp (- mean) / k!

		We have to find a function, with known primitive,
		that is always (a bit) greater than P (k).
		This function is based on the Lorentzian distribution,
		with a maximum of P(mean)/0.9 at k=mean:

			f (k) = mean^mean * exp (- mean) / mean! / (0.9 * (1 + (k - mean)^2 / (2 * mean)))

		The tangent is computed as the deviate

			tangent = tan (pi * unif (0, 1))

		This must equal the square root of (k - mean)^2 / (2 * mean),
		so that a trial value for k is given by

			k = floor (mean + tangent * sqrt (2 * mean))

		The probability that this is a good value is proportionate to the ratio of the Poisson
		distribution and the encapsulating function:

			probability = P (k) / f (k) = 0.9 * (1 + tangent^2) * mean ^ (k - mean) * mean! / k!

		The last two factors can be calculated as

			exp ((k - mean) * ln (mean) + lnGamma (mean + 1) - lnGamma (k + 1))
	*/
	static double previousMean = -1.0;   // this routine may well be called repeatedly with the same mean; optimize
	if (mean < 8.0) {
		static double expMean;
		double product = 1.0;
		integer result = -1;
		if (mean != previousMean) {
			previousMean = mean;
			expMean = exp (- mean);
		}
		repeat {
			product *= NUMrandomFraction ();
			result ++;
		} until (product <= expMean);
		return result;
	} else {
		static double sqrtTwoMean, lnMean, lnMeanFactorial;
		double result, probability, tangent;
		if (mean != previousMean) {
			previousMean = mean;
			sqrtTwoMean = sqrt (2.0 * mean);
			lnMean = log (mean);
			lnMeanFactorial = NUMlnGamma (mean + 1.0);
		}
		repeat {
			repeat {
				tangent = tan (NUMpi * NUMrandomFraction ());
				result = mean + tangent * sqrtTwoMean;
			} until (result >= 0.0);
			result = floor (result);
			probability = 0.9 * (1.0 + tangent * tangent) * exp ((result - mean) * lnMean + lnMeanFactorial - NUMlnGamma (result + 1.0));
		} until (NUMrandomFraction () <= probability);
		return result;
	}
}

uint32 NUMhashString (conststring32 string) {
	/*
	 * Jenkins' one-at-a-time hash.
	 */
	uint32 hash = 0;
	for (char32 kar = *string; kar != U'\0'; kar = * (++ string)) {
		hash += (kar >> 16) & 0xFF;   // highest five bits (a char32 actually has only 21 significant bits)
		hash += (hash << 10);
		hash ^= (hash >> 6);
		hash += (kar >> 8) & 0xFF;   // middle 8 bits
		hash += (hash << 10);
		hash ^= (hash >> 6);
		hash += kar & 0xFF;   // lowest 8 bits
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

/* End of file NUMrandom.cpp */
