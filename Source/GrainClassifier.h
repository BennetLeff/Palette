#pragma once

#include "doctest.h"
#include "../dependencies/Gist/src/Gist.h"

#include "Grain.h"

namespace Palette 
{
	template<typename SampleType>
	class GrainClassifier
	{
	public:
		/*
		 * GrainClassifier is a class which helps call methods in the Gist library.
		 * GrainClassifier fits the API to our application more conviently.
		 * frameSize is the size a grain in samples i.e. 441 samples for a
		 * 10 ms grain at 44.1 kHz sample rate.
		 */
		GrainClassifier(int frameSize, int sampleRate) : gist(frameSize, sampleRate) { }

		const auto spectralCentroid(const Grain<SampleType>& grain) 
        { 
			auto ptr = grain.sampleData.getReadPointer(0);
            
			// We must construct a vector from the internal pointer inside the 
			// juce::AudioBuffer inside the Grain because that's the only type
			// Gist accepts.
            return gist.spectralCentroid(std::vector<SampleType>(ptr, ptr + grain.sampleData.getNumSamples()));
        }

		const auto rootMeanSquare(const Grain<SampleType>& grain)
		{
			auto ptr = grain.sampleData.getReadPointer(0);

			return gist.rootMeanSquare(std::vector<SampleType>(ptr, ptr + grain.sampleData.getNumSamples()));
		}

		const auto peakEnergy(const Grain<SampleType>& grain)
		{
			auto ptr = grain.sampleData.getReadPointer(0);

			return gist.peakEnergy(std::vector<SampleType>(ptr, ptr + grain.sampleData.getNumSamples()));
		}

	private:
		Gist<SampleType> gist;
	};
}