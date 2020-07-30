/*
  ==============================================================================

    GrainEngine.h
    Created: 28 Jul 2020 2:14:20pm
    Author:  bennet

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "Grain.h"

/*
 * The GrainEngine splits an Audio file into Grains
 * and provides utilties for handling the data.
 */
template <typename SampleType>
class GrainEngine
{
public:
	std::vector<Grain<SampleType>> createGrains(const juce::AudioBuffer<SampleType>& audioData, const int samplesPerGrain);

	// this creates a pointer for reading from a const array of 16-bit little-endian packed samples.
	/*juce::AudioData::Pointer <juce::AudioData::Int16,
							  juce::AudioData::LittleEndian,
							  juce::AudioData::NonInterleaved,
							  juce::AudioData::Const> audioData;*/

	// std::vector<Grain<juce::AudioData::Int16>> grains;

};