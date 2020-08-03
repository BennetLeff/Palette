#include "FeatureAnalysis.h"
namespace Palette 
{
	template <typename R>
	constexpr typename R::value_type rootMeanSquare(const R& data)
	{
		if (data.size() == 0) return 0;

		const auto squaredTerms = ranges::view::transform(data, [](auto el) { return std::pow(el, 2); });
		const auto result = std::sqrt(ranges::accumulate(squaredTerms, 0.0) / static_cast<double>(data.size()));
		
		return result;
	}
}
