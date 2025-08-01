#ifndef _melder_real_h_
#define _melder_real_h_
/* melder_real.h
 *
 * Copyright (C) 1992-2021,2023-2025 Paul Boersma
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
	The following is checked in praat.h.

	Ideally, "long double" is 80 bits ("extended") precision (but stored in 96 or 128 bits), or even really 96 or 128 bits.
	And ideally, long doubles should be fast.

	They *are* fast on Intel64 processors, which have hardware support for them.
	But they are *very* slow (i.e. software-emulated) on ARM64 processors, which is why Xcode on the Mac and Clang on Windows
	typically regard a "long double" as being 64 bits, i.e. identical to a "double".
	On ARM64 Linux, Gcc regards "long double" as 128 bits, but this is very slow (more than 100 times slower than "double"),
	so we give up and define "longdouble" as "double" on ARM64 Linux, just as was already the case on ARM64 Windows and ARM64 Mac.

	The condition could therefore be
		#if defined (linux) && (defined (__aarch64__) || defined (_M_ARM64_))
	but just to make sure that no other Windows or Mac compilers happen to do 128-bit long double emulation,
	we include Windows and Mac into our condition:
*/
#if defined (__aarch64__) || defined (_M_ARM64_)
	using longdouble = double;
#else
	using longdouble = long double;
#endif

static const double undefined = (0.0/0.0);   // NaN

/*
	isdefined() shall capture not only `undefined`, but all infinities and NaNs.
	This can be done with a single test for the set bits in 0x7FF0'0000'0000'0000,
	at least for 64-bit IEEE implementations. The correctness of this assumption is checked in sys/praat.cpp.
	The portable version of isdefined() involves both isinf() and isnan(), or perhaps just isfinite(),
	but that would be slower (as tested in fon/Praat_tests.cpp)
	and it would also run into problems on some platforms when both <cmath> and <math.h> are included,
	as in dwsys/NUMcomplex.cpp.
*/
//inline bool isdefined (double x) { return ! isinf (x) && ! isnan (x); }   /* portable */
//inline bool isdefined (double x) { return isfinite (x); }   /* portable */
inline bool isdefined (double x) { return ((* (uint64 *) & x) & 0x7FF0'0000'0000'0000) != 0x7FF0'0000'0000'0000; }
inline bool isundef (double x) { return ((* (uint64 *) & x) & 0x7FF0'0000'0000'0000) == 0x7FF0'0000'0000'0000; }

template <typename T>
constexpr T sqr (T x) {
	return x * x;
}

struct MelderPoint {
	double x = undefined, y = undefined;
};

struct MelderRealRange {
	double min = undefined, max = undefined;
	bool isundef () const { return ::isundef (min) || ::isundef (max); }
	bool isEmpty () const { return ! (max > min); }   // note edge case: will return true if min or max is NaN
	//double size () {
	//	const double result = max - min;
	//	return std::max (result, 0.0);
	//}
};

inline bool isundef (const MelderRealRange me) {
	return isundef (me.min) || isundef (me.max);
}

struct MelderExtremaWithInit {
	double min = std::numeric_limits<double>::max();
	double max = std::numeric_limits<double>::lowest();
	void update (const double val) {
		if (val < min)
			min = val;
		if (val > max)
			max = val;
	}
	bool isValid () const {
		return min <= max;
	}
};

struct MelderGaussianStats {
	double mean = undefined, stdev = undefined;
};

struct MelderFraction {
	double numerator = 0.0, denominator = 0.0;
	double get () const {
		return our denominator == 0.0 ? undefined : our numerator / our denominator;
	}
	bool isValid () const {
		return our denominator != 0.0;
	}
};

struct MelderCountAndFraction {
	integer count = 0;
	double numerator = 0.0, denominator = 0.0;
	double getFraction () const {
		return our denominator == 0.0 ? undefined : our numerator / our denominator;
	}
	bool isValid () const {
		return our denominator != 0.0;
	}
};

/* End of file melder_real.h */
#endif
