#pragma once

#include <range/v3/all.hpp>
#include "doctest.h"

namespace Palette
{
	template <typename R>
	constexpr auto rootMeanSquare(const R& data);
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

	for (auto i = 2; i <= 10; i++) testData.push_back((float)i);

	SUBCASE("RMS for range of 1-10")
	{
		CHECK(Palette::rootMeanSquare(testData) == 55.0f);
	}
}