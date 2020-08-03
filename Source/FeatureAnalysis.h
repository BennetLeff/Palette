#pragma once

#include <range/v3/all.hpp>
#include "doctest.h"

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
}


TEST_CASE("Palette::rootMeanSqure(const R&&)")
{
	std::vector<float> testData;

	SUBCASE("RMS for empty range")
	{
		CHECK(Palette::rootMeanSquare(testData) == 0);
	}

	testData.push_back(1.0f);

	SUBCASE("RMS for 1 element")
	{
		CHECK(Palette::rootMeanSquare(testData) == 1);
	}

	SUBCASE("RMS for range of 1-10")
	{
		CHECK(Palette::rootMeanSquare(std::vector<float>{-2, 5, -8, 9, -4}) == doctest::Approx(6.16).epsilon(0.01));
	}
}