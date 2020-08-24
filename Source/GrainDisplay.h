#pragma once

#include "JuceHeader.h"

#include "Grain.h"

#include <vector>
#include <memory>
#include <algorithm>

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

            for (auto& grain : grains)
            {
                auto& features = grain.extractedFeatures;

                auto xAxisValue = static_cast<int>(getBounds().getWidth() * features.at(Feature::RMS));
                auto yAxisValue = static_cast<int>(getBounds().getHeight() * features.at(Feature::SpectralCentroid));

                xAxisValue = std::clamp((int)xAxisValue, 0, getBounds().getWidth());
                yAxisValue = std::clamp((int)yAxisValue, 0, getBounds().getHeight());

                auto point = juce::Rectangle<int>(xAxisValue, yAxisValue, 20, 20);

                g.setColour(juce::Colours::orange);

                g.fillRect(point);
            }
        }

        void resized() override { }

        void setAxis(const Feature& xAxis, const Feature& yAxis) { xAxis = xAxis; yAxis = yAxis; }

        // TODO: This makes a copy - use a shared_ptr
        void setGrains(std::vector<Grain<SampleType>>& grains) { this->grains = grains; }
    private:
        std::vector<Grain<SampleType>> grains;

        Feature xAxis;
        Feature yAxis;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainDisplay)
    };


}
