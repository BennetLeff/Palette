#pragma once

#include "JuceHeader.h"

#include "Grain.h"

#include <vector>
#include <memory>
#include <algorithm>

namespace Palette
{
    /*
     * The GrainDisplay plots each grain according to extracted audio features.
     * In other words, each grain contains a map which stores certain types of
     * audio analysis methods and their corresponding values when computed on
     * each grain. These values are what are used to plot a grain.
     */
    template <typename SampleType>
    class GrainDisplay : public juce::Component
    {
    public:
        GrainDisplay() { }

        void paint(juce::Graphics& g) override
        {
            // Set background to dark gray.
            g.fillAll(juce::Colours::darkgrey);

            // Paint each grain as an orange square.
            for (auto& grain : grains)
            {
                // features is a reference to an unordered_map which maps Features to values
                auto& features = grain.extractedFeatures;

                // We set the x-axis and y-axis value to the corresponding feature
                // and scale it to the width and height of the drawing space.
                auto xAxisValue = static_cast<int>(getBounds().getWidth() * features.at(xAxis));
                auto yAxisValue = static_cast<int>(getBounds().getHeight() * features.at(yAxis));

                // Clamp the values to keep them in bounds. JUCE does this internally
                // but it's best to maintain consistency in case we need this value later.
                xAxisValue = std::clamp((int)xAxisValue, 0, getBounds().getWidth());
                yAxisValue = std::clamp((int)yAxisValue, 0, getBounds().getHeight());

                // Right now each point should be a 20 by 20 pixel square.
                auto point = juce::Rectangle<int>(xAxisValue, yAxisValue, 20, 20);

                // We set the square to orange
                g.setColour(juce::Colours::orange);

                // And we fill the square which fills the space to be drawn by JUCE.
                g.fillRect(point);
            }
        }

        void resized() override { }

        void setAxis(const Feature& xAxis, const Feature& yAxis) { xAxis = xAxis; yAxis = yAxis; }

        // TODO: This makes a copy - use a shared_ptr
        void setGrains(std::vector<Grain<SampleType>>& grains) { this->grains = grains; }
    private:
        std::vector<Grain<SampleType>> grains;

        Feature xAxis = Feature::RMS;
        Feature yAxis = Feature::SpectralCentroid;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainDisplay)
    };


}
