#pragma once

#include "JuceHeader.h"

#include "Grain.h"

#include <vector>
#include <memory>

namespace Palette
{
    template <typename SampleType>
    class GrainDisplay : public juce::Component
    {
    public:
        GrainDisplay() { }

        void paint(juce::Graphics& g) override
        {
            g.fillAll(juce::Colours::darkgrey);

            for (const auto& grain : grains)
            {
                auto xAxisValue = static_cast<int>(g.getClipBounds().getWidth() * grain.extractedFeatures.at(Feature::RMS));
                auto yAxisValue = static_cast<int>(g.getClipBounds().getHeight() * grain.extractedFeatures.at(Feature::SpectralCentroid));

                auto point = juce::Rectangle<int>(xAxisValue, yAxisValue, 20, 20);

                g.setColour(juce::Colours::orange);

                g.fillRect(point);
            }
        }

        void resized() override { }

        void setAxis(const Feature& xAxis, const Feature& yAxis) { xAxis = xAxis; yAxis = yAxis; }

        // TODO: This makes a copy - use a shared_ptr
        void setGrains(std::vector<Grain<SampleType>>& grains) { grains = grains; }
    private:
        std::vector<Grain<SampleType>> grains;

        Feature xAxis;
        Feature yAxis;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainDisplay)
    };


}
