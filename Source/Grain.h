/*
  ==============================================================================

    Grain.h
    Created: 28 Jul 2020 10:59:46am
    Author:  bennet

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

/*
 * Grains are the fundamental building block of concatenative synthesis.
 * Each grain is some buffer of audio samples.
 */
template <typename SampleType>
struct Grain
{
	Grain(const juce::AudioBuffer<SampleType>& data) : sampleData(data) { }
	// The collection of samples which will be played back.
	juce::AudioBuffer<SampleType> sampleData;
};