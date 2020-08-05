#pragma once

#include <range/v3/all.hpp>
#include "doctest.h"

/*
 * The following are a collection of pure functions which perform audio analysis.
 * These are meant to be called on the data in Grains for use in comparison of
 * Grains. Many of the functions are named or presented in literature such as 
 * http://recherche.ircam.fr/equipes/analyse-synthese/peeters/ARTICLES/Peeters_2003_cuidadoaudiofeatures.pdf
 * or in open source such as in the Gist Audio Analysis library.
 *
 * Until the arrival of full support of Ranges accross the major C++ compilers,
 * we will be using Ranges-v3.
 */
namespace Palette
{
	/*
	 * Computes the root mean square of range. Note that the RMS is defined
	 * differently in the frequency domain. This is expected to operate
	 * in the time domain. 
	 */
	template <typename R>
	constexpr double rootMeanSquare(const R& data);
	
	/*
	 * Computes the peak energy of the range. This is the max
	 * sample value of any sample in a range.
	 */
	template <typename R>
	constexpr double peakEnergy(const R& data);
	
	/*
	 * Computes the zero crossing rate of the range. This is the number of times
	 * the samples cross the zero axis. Noisier sound tend to have a higher
	 * zero crossing rate.
	 */
	template <typename R>
	constexpr double zeroCrossingRate(const R& data);

	/*
	 * Computes the spectral centroid of the range. The spectral centroid can be 
	 * used to quantify some measure of brightness of a signal.
	 */
	template <typename R>
	constexpr double spectralCentroid(const R& data);

	/*
	 * Computes the spectral flatness of the range. The spectral flatness of a
	 * signal can be used to measure how tone-like or how noise-like a signal is.
	 */
	template <typename R>
	constexpr double spectralFlatness(const R& data);

	/*
	 * Computes the spectral crest of the range. This is a measure of the flatness
	 * which is computed by Crest = max(data) / rms(data). White noise should 
	 * produce a flat spectrum, therefore a low spectral crest.
	 */
	template <typename R>
	constexpr double spectralCrest(const R& data);

	/*
	 * Spectral rolloff is the frequency below which a specified percentage of 
	 * the total spectral energy, e.g. 85%, lies.
	 */
	template <typename R>
	constexpr double spectralRolloff(const R& data, const float percentile);

	/*
	 * Computes the spectral kurtosis of the range. The Spectral Kurtosis
	 * can be used to quantify the occurence of transients in a signal.
	 */
	template <typename R>
	constexpr double spectralKurtosis(const R& data);
}

/*
 * Tests
 */
TEST_CASE("Palette::rootMeanSqure(const R)")
{
	SUBCASE("RMS for empty range")
	{
		CHECK(Palette::rootMeanSquare(std::vector<float>{}) == 0.0);
	}

	SUBCASE("RMS for 1 element")
	{
		CHECK(Palette::rootMeanSquare(std::vector<float>{1}) == 1.0);
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
		CHECK(Palette::peakEnergy(std::vector<float>{1}) == 1.0);
	}

	SUBCASE("Peak energy for range")
	{
		CHECK(Palette::peakEnergy(std::vector<float>{-2, 5, -8, 9, -4}) == 9.0);
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
		CHECK(Palette::zeroCrossingRate(std::vector<float>{1}) == 0.0);
	}

	SUBCASE("Zero crossing rate for range")
	{
		CHECK(Palette::zeroCrossingRate(std::vector<float>{-2, 5, -8, 9, -4}) == 4.0);
	}
}

TEST_CASE("Palette::spectralCentroid(const R&);")
{
	SUBCASE("Spectral centroid of empty range")
	{

	}
	
	SUBCASE("Spectral centroid of 1 element")
	{

	}
	
	SUBCASE("Spectral centroid of range")
	{

	}
}

TEST_CASE("Palette::spectralFlatness(const R&);")
{
	SUBCASE("Spectral flatness of empty range")
	{

	}
	
	SUBCASE("Spectral flatness of 1 element")
	{

	}
	
	SUBCASE("Spectral flatness of range")
	{

	}
}

TEST_CASE("Palette::spectralCrest(const R&)")
{
	SUBCASE("Spectral crest of empty range")
	{

	}
	
	SUBCASE("Spectral crest of 1 element")
	{

	}
	
	SUBCASE("Spectral crest of range")
	{

	}
}

TEST_CASE("Palette::spectralRolloff(const R&, const float)")
{
	SUBCASE("Spectral rolloff of empty range")
	{

	}
	
	SUBCASE("Spectral rolloff of 1 element")
	{

	}
	
	SUBCASE("Spectral rolloff of range, 0th percentile")
	{

	}

	SUBCASE("Spectral rolloff of range, 85th percentile")
	{

	}

	SUBCASE("Spectral rolloff of range, 100th percentile")
	{

	}

	SUBCASE("Spectral rolloff of range, 110th percentile")
	{

	}
}

TEST_CASE("Palette::spectralKurtosis(const R&)")
{
	SUBCASE("Spectral kurtosis of empty range")
	{

	}
	
	SUBCASE("Spectral kurtosis of 1 element")
	{

	}
	
	SUBCASE("Spectral kurtosis of range")
	{

	}
}