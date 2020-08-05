#include <algorithm>

#include "FeatureAnalysis.h"
namespace Palette 
{
	template <typename R>
	constexpr double rootMeanSquare(const R& data)
	{
		if (data.size() == 0) return 0;

		const auto squaredTerms = ranges::view::transform(data, [](auto el) { return std::pow(el, 2); });
		const auto result = std::sqrt(ranges::accumulate(squaredTerms, 0.0) / static_cast<double>(data.size()));
		
		return result;
	}

	template <typename R>
	constexpr double peakEnergy(const R& data)
	{
		// As long as there's at least 1 sample, we can find the max.
		if (data.size() > 0)
			return *std::max_element(begin(data), end(data));
		else
			return 0.0;
	}

	template <typename R>
	constexpr double zeroCrossingRate(const R& data)
	{
		auto totalCrossings = 0;

		// If there are less than 2 samples, it would be impossible
		// for there for be a zero crossing.
		if (data.size() < 2)
			return totalCrossings;

		// Loop through each sample, checking if the next sample has
		// a different sign, which would mean the signal crossed the axis.
		for (auto sample = 0; sample < data.size() - 1; sample++)
		{
			if ((data[sample] < 0 && data[sample + 1] > 0)
				|| (data[sample] > 0 && data[sample + 1] < 0))
				totalCrossings++;
		}

		return totalCrossings;
	}
}
