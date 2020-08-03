#pragma once

#include <range/v3/all.hpp>
#include "doctest.h"

/*
 * The following are a collection of pure functions which perform
 * audio analysis. These are meant to be called on the data in Grains
 * for use in comparison of Grains. Many of the functions are named or
 * presented in literature such as 
 * http://recherche.ircam.fr/equipes/analyse-synthese/peeters/ARTICLES/Peeters_2003_cuidadoaudiofeatures.pdf
 * or in open source such as in the Gist Audio Analysis library.
 */
namespace Palette
{
	/*
	 * Computes the root mean square of range. Until the arrival 
	 * of full support of Ranges accross the major C++ compilers,
	 * we will be using Ranges-v3. Note that the RMS is defined
	 * differently in the frequency domain. This is expected to operate
	 * in the time domain. 
	 */
	template <typename R>
	constexpr typename R::value_type rootMeanSquare(const R& data);
	
	/*
	 * Computes the peak energy of the range. This is the max
	 * sample value of any sample in a range.
	 */
	template <typename R>
	constexpr typename R::value_type peakEnergy(const R& data);
	
	/*
	 * Computes the peak energy of the range. This is the max
	 * sample value of any sample in a range. Noisier sounds
	 * tend to have a higher zero crossing rate.
	 */
	template <typename R>
	constexpr typename R::value_type zeroCrossingRate(const R& data);
}


TEST_CASE("Palette::rootMeanSqure(const R)")
{
	SUBCASE("RMS for empty range")
	{
		CHECK(Palette::rootMeanSquare(std::vector<float>{}) == 0);
	}

	SUBCASE("RMS for 1 element")
	{
		CHECK(Palette::rootMeanSquare(std::vector<float>{1}) == 1);
	}

	SUBCASE("RMS for range")
	{
		CHECK(Palette::rootMeanSquare(std::vector<float>{-2, 5, -8, 9, -4}) == doctest::Approx(6.16).epsilon(0.01));
	}
}

TEST_CASE("Palette::peakEnergy(const R&)")
{
	SUBCASE("Peak energy for empty range")
	{
		CHECK(Palette::peakEnergy(std::vector<float>{}) == 0.0);
	}

	SUBCASE("Peak energy for 1 element")
	{
		CHECK(Palette::peakEnergy(std::vector<float>{1}) == 1);
	}

	SUBCASE("Peak energy for range")
	{
		CHECK(Palette::peakEnergy(std::vector<float>{-2, 5, -8, 9, -4}) == 9);
	}
}

TEST_CASE("Palette::zeroCrossingRate(const R&)")
{
	SUBCASE("Zero crossing rate for empty range")
	{
		CHECK(Palette::zeroCrossingRate(std::vector<float>{}) == 0.0);
	}

	SUBCASE("Zero crossing rate for 1 element")
	{
		CHECK(Palette::zeroCrossingRate(std::vector<float>{1}) == 0);
	}

	SUBCASE("Zero crossing rate for range")
	{
		CHECK(Palette::zeroCrossingRate(std::vector<float>{-2, 5, -8, 9, -4}) == 4);
	}
}