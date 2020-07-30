/*
  ==============================================================================

    GrainEngine.cpp
    Created: 28 Jul 2020 2:14:20pm
    Author:  bennet

  ==============================================================================
*/

#include "GrainEngine.h"

template <typename SampleType>
std::vector<Grain<SampleType>> GrainEngine<SampleType>::createGrains(const juce::AudioBuffer<SampleType>& audioData, const int samplesPerGrain)
{
	/*
	 * There must be a grain for each chunk of samples to be held. We round up because 
	 * we want to capture all samples of the audioData. The last grain will be padded with
	 * zeros if there is remaining space. 
	 */
	const auto numGrains = std::ceil(audioData.getNumSamples() / (double) samplesPerGrain);
	std::vector<Grain<SampleType>> grains(numGrains);

	jassert(audioData.getNumSamples() > samplesPerGrain);

	/*
	 * We walk through the data to be converted to grains one chunk at a time,
	 * where each chunk is the size of samplesPerGrain. This will loop
	 * enough times to fill each grain except possibly the last.
	 */
	auto partitionedSamples = 0;
	for (; partitionedSamples < (audioData.getNumSamples() - partitionedSamples); partitionedSamples += samplesPerGrain)
	{
		auto buffer = AudioBuffer<SampleType>(audioData, audioData.getNumChannels(), partitionedSamples, samplesPerGrain);
		grains.push_back(Grain{buffer})
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

		auto buffer = AudioBuffer<SampleType>(audioData,
			audioData.getNumChannels(),
			partitionedSamples,
			partitionedSamples + remainingSamples);

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

		grains.push_back(Grain{ buffer })
	}

	return grains;
}
