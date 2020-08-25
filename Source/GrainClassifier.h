#pragma once

#include "doctest.h"
#include "../dependencies/Gist/src/Gist.h"

#include "Grain.h"

namespace Palette 
{
	// We need to forward declare Grain to resolve a few errors.
	template <typename>
	class Grain;

	/* We'll need to maintain an enumeration of methods for feature extraction.
	 * This is used in the Grain class so that each Grain can own data about
	 * its features which is then used in the GrainDisplay when plotting grains.
	 */

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

		const SampleType spectralCentroid(const Grain<SampleType>& grain) 
        { 
			auto ptr = grain.sampleData.getReadPointer(0);
            
			// We must construct a vector from the internal pointer inside the 
			// juce::AudioBuffer inside the Grain because that's the only type
			// Gist accepts.
			gist.processAudioFrame(std::vector<SampleType>(ptr, ptr + grain.sampleData.getNumSamples()));

            return gist.spectralCentroid();
        }

		const SampleType rootMeanSquare(const Grain<SampleType>& grain)
		{
			auto ptr = grain.sampleData.getReadPointer(0);

			gist.processAudioFrame(std::vector<SampleType>(ptr, ptr + grain.sampleData.getNumSamples()));

			return gist.rootMeanSquare();
		}

		const SampleType peakEnergy(const Grain<SampleType>& grain)
		{
			auto ptr = grain.sampleData.getReadPointer(0);

			gist.processAudioFrame(std::vector<SampleType>(ptr, ptr + grain.sampleData.getNumSamples()));

			return gist.peakEnergy();
		}

	private:
		Gist<SampleType> gist;
	};
}