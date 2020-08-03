#pragma once

#include <fmt/format.h>

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
		
		std::vector<Grain<SampleType>> grains;

		/*
		 * If there are not enough samples in the audio file to fit in one grain,
		 * just return a single grain the AudioBuffer in audioData. This grain
		 * will thus be shorter in length than samplesPerGrain but this behaviour
		 * seems to be the best default. It's conceivable we may want to later adjust the
		 * API to create a grain padded with zeros to fit the requested grain size.
		 */
		if (audioData.getNumSamples() < samplesPerGrain)
		{
			// Create buffer the size of audioData.getNumSamples()
			auto buffer = juce::AudioBuffer<SampleType>(audioData.getNumChannels(), audioData.getNumSamples());

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
			// Create the buffer empty, with 2 channels, and samplesPerGrain space for samples.
			auto buffer = juce::AudioBuffer<SampleType>(audioData.getNumChannels(), samplesPerGrain);

			// Copy the data from the audioData AudioBuffer into a new buffer to be stored in a Grain.
			// Copy into channel ch, starting at sample 0, from audioData, from channel ch, starting at
			// an offset of partitionedSamples into audioData, and copy samplesPerGrain samples.
			for (auto ch = 0; ch < audioData.getNumChannels(); ch++)
				buffer.copyFrom(ch, 0, audioData, ch, partitionedSamples, samplesPerGrain);

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

			auto buffer = juce::AudioBuffer<SampleType>(2, samplesPerGrain);
			for (auto ch = 0; ch < audioData.getNumChannels(); ch++)
				buffer.copyFrom(ch, 0, audioData, ch, partitionedSamples, remainingSamples);

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
	const auto snareFile = "snare.wav";
	const auto springFile = "spring.wav";
	const auto bangFile = "loudanime.wav";
	
	auto grainLength = 100;
	auto sampleRate = 44100;
	
	auto [snareGrains100, snareLengthInSamples] = fileToGrains(resourcesLoc + snareFile, grainLength, sampleRate);

	SUBCASE(fmt::format("{} contains {} samples at a sampleRate of {} hz with a grainLength of {} ms",
		snareFile, snareLengthInSamples, sampleRate, grainLength).c_str())
	{
		CHECK(snareGrains100.size() == numGrains(snareLengthInSamples, sampleRate, grainLength));
	}

	grainLength = 1000;

	auto [snareGrains1000, _] = fileToGrains(resourcesLoc + snareFile, grainLength, sampleRate);
	SUBCASE(fmt::format("{} contains {} samples at a sampleRate of {} hz with a grainLength of {} ms",
		snareFile, snareLengthInSamples, sampleRate, grainLength).c_str())
	{
		CHECK(snareGrains1000.size() == numGrains(snareLengthInSamples, sampleRate, grainLength));
	}
	
	grainLength = 0;

	auto [snareGrains0, __] = fileToGrains(resourcesLoc + snareFile, grainLength, sampleRate);
	SUBCASE(fmt::format("{} contains {} samples at a sampleRate of {} hz with a grainLength of {} ms",
		snareFile, snareLengthInSamples, sampleRate, grainLength).c_str())
	{
		CHECK(snareGrains0.size() == 0);
	}

	// Cases for other files 
	// Set sample rate and change grainLength back
	sampleRate = 48000;
	grainLength = 100;

	auto [springGrains, springLengthInSamples] = fileToGrains(resourcesLoc + springFile, grainLength, sampleRate);

	SUBCASE(fmt::format("{} contains {} samples at a sampleRate of {} hz with a grainLength of {} ms",
		springFile, springLengthInSamples, sampleRate, grainLength).c_str())
	{
		CHECK(springGrains.size() == numGrains(springLengthInSamples, sampleRate, grainLength));
	}

	auto [bangGrains, bangLengthInSamples] = fileToGrains(resourcesLoc + bangFile, grainLength, sampleRate);

	SUBCASE(fmt::format("{} contains {} samples at a sampleRate of {} hz with a grainLength of {} ms",
		bangFile, bangLengthInSamples, sampleRate, grainLength).c_str())
	{
		CHECK(bangGrains.size() == numGrains(bangLengthInSamples, sampleRate, grainLength));
	}
}