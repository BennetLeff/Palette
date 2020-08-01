/*
  ==============================================================================

    Grain.h
    Created: 28 Jul 2020 10:59:46am
    Author:  bennet

  ==============================================================================
*/

#pragma once

#include "doctest.h"


#include "JuceHeader.h"
namespace Palette 
{
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

	/*
	 * createGrains splits an Audio file into Grains
	 * and provides utilties for handling the data.
	 *
	 * grainLength in terms of miliseconds
	 * sampleRate in terms of samples per second.
	 */
	template <typename SampleType>
	std::vector<Palette::Grain<SampleType>> createGrains(const juce::AudioBuffer<SampleType>& audioData, const double grainLength, const double sampleRate)
	{
		/*
		 * We have samplesPerSecond as sampleRate and we have seconds as grainLength.
		 * Therefore, samplesPerGrain is samplesPerSecond * seconds in a grain
		 * or samplesPerSecond * grainLength. Since grainLength is ms we divide by 1000
		 */
		const int samplesPerGrain = static_cast<int>(sampleRate * (grainLength / 1000));

		/*
		 * There must be a grain for each chunk of samples to be held. We round up because
		 * we want to capture all samples of the audioData. The last grain will be padded with
		 * zeros if there is remaining space.
		 */
		const auto numGrains = std::ceil(audioData.getNumSamples() / (double)samplesPerGrain);

		DBG("Parsing " << audioData.getNumSamples() << " samples to partition into grains. ");
		DBG("With a grainLength of " << grainLength << " there should be " << numGrains << " grains");
		DBG("[ " << audioData.getNumSamples() << " / " << samplesPerGrain << " = " << numGrains << " ]");
		
		std::vector<Grain<SampleType>> grains;

		jassert(audioData.getNumSamples() > samplesPerGrain);

		const auto numChannels = audioData.getNumChannels();

		/*
		 * We walk through the data to be converted to grains one chunk at a time,
		 * where each chunk is the size of samplesPerGrain. This will loop
		 * enough times to fill each grain except possibly the last.
		 */
		auto partitionedSamples = 0;
		for (; partitionedSamples < (audioData.getNumSamples() - partitionedSamples); partitionedSamples += samplesPerGrain)
		{
			// auto buffer = juce::AudioBuffer<SampleType>( audioData.getArrayOfReadPointers(), audioData.getNumChannels(), partitionedSamples, samplesPerGrain );

			// Create the buffer empty, with 2 channels, and samplesPerGrain space for samples.
			auto buffer = juce::AudioBuffer<SampleType>(numChannels, samplesPerGrain);

			// Copy the data from the audioData AudioBuffer into a new buffer to be stored in a Grain.
			// Copy into channel ch, starting at sample 0, from audioData, from channel ch, starting at
			// an offset of partitionedSamples into audioData, and copy samplesPerGrain samples.
			for (auto ch = 0; ch < numChannels; ch++)
				buffer.copyFrom(ch, 0, audioData, ch, partitionedSamples, samplesPerGrain);

			grains.push_back(Palette::Grain<SampleType>(buffer));
		}

		/*
		 * Fill last grain by zeroing out a grain then filling it as much as possible
		 * with the remaining grains - only if there were samples left over from above.
		 */
		 // If partitionedSamples does not evenly divide by samplesPerGrain, there will be
		 // leftover samples to partition. 
		if (partitionedSamples % samplesPerGrain != 0)
		{
			const auto remainingSamples = partitionedSamples % samplesPerGrain;

			// There should not be more than one grains worth of samples leftover.
			jassert(partitionedSamples + remainingSamples <= samplesPerGrain);

			/*auto buffer = juce::AudioBuffer<SampleType>(audioData,
				audioData.getNumChannels(),
				partitionedSamples,
				partitionedSamples + remainingSamples);*/

			auto buffer = juce::AudioBuffer<SampleType>(2, samplesPerGrain);
			for (auto ch = 0; ch < numChannels; ch++)
				buffer.copyFrom(ch, 0, audioData, ch, partitionedSamples, samplesPerGrain);

			// Check if there's remaining space in the buffer which couldn't be filled
			// with samples. This space will need to be zeroed out to prevent audio artifacts.
			if (partitionedSamples + remainingSamples < samplesPerGrain)
			{
				// Zero out any remaining space for samples in the buffer.
				// 2 channels to the buffer
				// samplesPerGrain sized buffer
				// keepExistingContent = true
				// clearExtraSpace = true
				// avoidReallocating = false
				buffer.setSize(2,
					samplesPerGrain,
					true,
					true,
					false);
			}

			grains.push_back(Palette::Grain<SampleType>(buffer));
		}

		DBG("Partitioned " << grains.size() << " grains");

		return grains;
	}
}

TEST_CASE("Grain")
{
	CHECK(5 <= 5);
	CHECK(4 > 5);
}