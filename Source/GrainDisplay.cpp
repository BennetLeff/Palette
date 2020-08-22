//#include "GrainDisplay.h"
//
//namespace Palette
//{
//	template <typename SampleType>
//	GrainDisplay<SampleType>::GrainDisplay()
//	{
//
//	}
//
//	template <typename SampleType>
//	void GrainDisplay<SampleType>::paint(juce::Graphics& g)
//	{
//		g.fillAll(juce::Colours::darkgrey);
//
//		for (const auto& grain : grains)
//		{
//			const auto xAxisValue = grain[xAxis];
//			const auto yAxisValue = grain[yAxis];
//
//			const auto point = Rectangle(xAxisValue, yAxisValue, 20, 20);
//
//			g.setColour(juce::Colours::orange);
//
//			g.fillRect(point);
//		}
//
//		const auto test_point = Rectangle(200, 200, 20, 20);
//
//		g.setColour(juce::Colours::orange);
//
//		g.fillRect(test_point);
//	}
//
//	template <typename SampleType>
//	void GrainDisplay<SampleType>::resized()
//	{
//
//	}
//}