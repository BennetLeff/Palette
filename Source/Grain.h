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

#include <unordered_map>

namespace Palette 
{
	enum class Feature
	{
		SpectralCentroid,
		RMS,
		PeakEnergy
	};


	/*
	 * Grains are the fundamental building block of concatenative synthesis.
	 * Each grain is some buffer of audio samples.
	 */
	template <typename SampleType>
	struct Grain
	{
		Grain(const juce::AudioBuffer<SampleType>& data) : sampleData(data) { }
		
		/* 
		 * The collection of samples which will be played back. This is assumed
		 * to be mono data. In the createGrains function, audio is summed to mono.
		 */
		juce::AudioBuffer<SampleType> sampleData;

		std::unordered_map < Feature, double > extractedFeatures;
	};

	/*
	 * createGrains splits an Audio file into Grains
	 * and provides utilties for handling the data.
	 *
	 * grainLength in terms of miliseconds
	 * sampleRate in terms of samples per second.
	 */
	template <typename SampleType>
	constexpr std::vector<Palette::Grain<SampleType>> createGrains(const juce::AudioBuffer<SampleType>& audioData, const double grainLength, const double sampleRate)
	{
		// Return an empty vector if grainLength is less than or equal to zero.
		if (grainLength <= 0)
			return std::vector<Palette::Grain<SampleType>>{};

		/*
		 * We have samplesPerSecond as sampleRate and we have seconds as grainLength.
		 * Therefore, samplesPerGrain is samplesPerSecond * seconds in a grain
		 * or samplesPerSecond * grainLength. Since grainLength is ms we divide by 1000
		 */
		const auto samplesPerGrain = static_cast<int>(sampleRate * (grainLength / 1000));

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

		/*
		 * If there are not enough samples in the audio file to fit in one grain,
		 * just return a single grain the AudioBuffer in audioData. This grain
		 * will thus be shorter in length than samplesPerGrain but this behaviour
		 * seems to be the best default. It's conceivable we may want to later adjust the
		 * API to create a grain padded with zeros to fit the requested grain size.
		 */
		const auto num = audioData.getNumSamples();

		if (audioData.getNumSamples() < samplesPerGrain)
		{
			// Create buffer the size of audioData.getNumSamples()
			auto buffer = juce::AudioBuffer<SampleType>(audioData.getNumChannels(), audioData.getNumSamples());

			buffer.clear();

			for (auto ch = 0; ch < audioData.getNumChannels(); ch++)
				buffer.copyFrom(ch, 0, audioData, ch, 0, audioData.getNumSamples());

			grains.push_back(Palette::Grain<SampleType>(buffer));

			return grains;
		}

		/*
		 * We walk through the data to be converted to grains one chunk at a time,
		 * where each chunk is the size of samplesPerGrain. This will loop
		 * enough times to fill each grain except possibly the last.
		 */
		auto partitionedSamples = 0;
		for (; partitionedSamples < (audioData.getNumSamples() - samplesPerGrain); partitionedSamples += samplesPerGrain)
		{
			// Create the buffer empty, with 1 channel, and samplesPerGrain space for samples.
			auto buffer = juce::AudioBuffer<SampleType>(1, samplesPerGrain);

			// We must clear the buffer as it can point to unexpectedly non-zero data on instantiation.
			buffer.clear();

			// Add the data from the audioData AudioBuffer into a new buffer to be stored in a Grain.
			// Add into channel 0, starting at sample 0, from audioData, from channel ch, starting at
			// an offset of partitionedSamples into audioData, and copy samplesPerGrain samples.
			for (auto ch = 0; ch < audioData.getNumChannels(); ch++)
				buffer.addFrom(0, 0, audioData, ch, partitionedSamples, samplesPerGrain);

			// Normalize the buffer since we've summed potentially multiple channels into one.
			buffer.applyGain(1.0f / audioData.getNumChannels());

			grains.push_back(Palette::Grain<SampleType>(buffer));
		}

		/*
		 * Fill last grain by zeroing out a grain then filling it as much as possible
		 * with the remaining grains - only if there were samples left over from above.
		 */
		 // If partitionedSamples does not evenly divide by samplesPerGrain, there will be
		 // leftover samples to partition. 
		if (audioData.getNumSamples() % samplesPerGrain != 0)
		{
			const auto remainingSamples = audioData.getNumSamples() % samplesPerGrain;

			// There should not be more than one grains worth of samples leftover.
			jassert(remainingSamples <= samplesPerGrain);

			auto buffer = juce::AudioBuffer<SampleType>(1, samplesPerGrain);

			buffer.clear();

			for (auto ch = 0; ch < audioData.getNumChannels(); ch++)
				buffer.addFrom(0, 0, audioData, ch, partitionedSamples, remainingSamples);

			// Check if there's remaining space in the buffer which couldn't be filled
			// with samples. This space will need to be zeroed out to prevent audio artifacts.
			if (partitionedSamples + remainingSamples < samplesPerGrain)
			{
				// Zero out any remaining space for samples in the buffer.
				// 1 channel to the buffer
				// samplesPerGrain sized buffer
				// keepExistingContent = true
				// clearExtraSpace = true
				// avoidReallocating = false
				buffer.setSize(1,
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

TEST_CASE("Palette::createGrains(const juce::AudioBuffer<SampleType>&, const double, const double)")
{
	/*
	 * Helper method which lets us test createGrains. This lambda calls
	 * createsGrains using a set grainLength and sampleRate on a file
	 * at a specified location. Once we've created the grains, we can perform
	 * tests. 
	 */
	auto fileToGrains = [](juce::String location, auto grainLength, auto sampleRate) {
		juce::AudioFormatManager formatManager;
		formatManager.registerBasicFormats();

		auto* reader = formatManager.createReaderFor(juce::File(location));

		juce::AudioBuffer<float> fileBuffer;

		if (reader != nullptr)
		{
			auto duration = reader->lengthInSamples / reader->sampleRate;

			fileBuffer.setSize(reader->numChannels, (int)reader->lengthInSamples);
			reader->read(&fileBuffer,
				0,
				(int)reader->lengthInSamples,
				0,
				true,
				true);
		}

		auto grainsAndSamples = std::make_tuple(
			Palette::createGrains(fileBuffer, grainLength, sampleRate),
			reader->lengthInSamples);

		delete reader;

		return grainsAndSamples;
	};

	const auto numGrains = [](const auto lengthInSamples, const auto sampleRate, const auto grainLength) { 
		return std::ceil(lengthInSamples / (sampleRate * ((double)grainLength / 1000)));
	};

	// We must find the resources dir location which I've copied to be next the executable created by Visual Studio. 
	// This helps to make the code work on other Windows machines but will need to be improved.
	const auto resourcesLoc = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile).getParentDirectory().getFullPathName() + "\\resources\\";
	const auto snareLoc = resourcesLoc + "snare.wav";
	const auto springLoc = resourcesLoc + "spring.wav";
	const auto bangLoc = resourcesLoc + "loudanime.wav";
	
	auto grainLength = 100;
	auto sampleRate = 44100;
	
	auto [snareGrains100, snareLengthInSamples] = fileToGrains(snareLoc, grainLength, sampleRate);

	SUBCASE("snare.wav contains 8113 samples at a sampleRate of 44100 hz with a grainLength of 100 ms")
	{
		CHECK(snareGrains100.size() == numGrains(snareLengthInSamples, sampleRate, grainLength));
	}

	grainLength = 1000;

	auto [snareGrains1000, _] = fileToGrains(snareLoc, grainLength, sampleRate);
	SUBCASE("snare.wav contains 8113 samples at a sampleRate of 44100 hz with a grainLength of 1000 ms")
	{
		CHECK(snareGrains1000.size() == numGrains(snareLengthInSamples, sampleRate, grainLength));
	}
	
	grainLength = 0;

	auto [snareGrains0, __] = fileToGrains(snareLoc, grainLength, sampleRate);
	SUBCASE("snare.wav contains 8113 samples at a sampleRate of 44100 hz with a grainLength of 0 ms")
	{
		CHECK(snareGrains0.size() == 0);
	}

	// Cases for other files 
	// Set sample rate and change grainLength back
	sampleRate = 48000;
	grainLength = 100;

	auto [springGrains, springLengthInSamples] = fileToGrains(springLoc, grainLength, sampleRate);

	SUBCASE("spring.wav contains 176400 samples at a sampleRate of 48000 hz with a grainLength of 100 ms")
	{
		CHECK(springGrains.size() == numGrains(springLengthInSamples, sampleRate, grainLength));
	}

	auto [bangGrains, bangLengthInSamples] = fileToGrains(bangLoc, grainLength, sampleRate);

	SUBCASE("loudanime.wav contains 229946 samples at a sampleRate of 48000 hz with a grainLength of 100 ms")
	{
		CHECK(bangGrains.size() == numGrains(bangLengthInSamples, sampleRate, grainLength));
	}
}