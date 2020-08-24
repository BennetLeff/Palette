/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "GrainClassifier.h"

//==============================================================================
PaletteAudioProcessorEditor::PaletteAudioProcessorEditor (PaletteAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
	// Basic utility method to create grains from a file. 
	// Originally defined in a test in Grains.h. Only here temporarily.
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

		// print out data
		/*float* ptr = fileBuffer.getWritePointer(0);
		int count = fileBuffer.getNumSamples();
		while (count--)
		{
			if (fileBuffer.getNumSamples() - count == 0) { DBG("---------------- Grain 1 ----------------"); }
			else if (fileBuffer.getNumSamples() - count == 4410) { DBG("---------------- Grain 2 ----------------"); }
			DBG(*ptr);
			ptr++;
		}*/

		auto grains_ = Palette::createGrains(fileBuffer, grainLength, sampleRate);

		auto grainsAndSamples = std::make_tuple(
			grains_,
			reader->lengthInSamples);

		delete reader;

		return grainsAndSamples;
	};

	// Simply test data
	const auto resourcesLoc = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile).getParentDirectory().getFullPathName() + "\\resources\\";
	const auto snareLoc = resourcesLoc + "snare.wav";
	const auto springLoc = resourcesLoc + "spring.wav";
	const auto bangLoc = resourcesLoc + "loudanime.wav";

	auto grainLength = 100;
	auto sampleRate = 44100;

	auto [snareGrains100, snareLengthInSamples] = fileToGrains(snareLoc, grainLength, sampleRate);

	for (auto& grain : snareGrains100)
	{
		auto classifier = Palette::GrainClassifier<float>(4410, sampleRate);

		grain.extractedFeatures[Palette::Feature::RMS] = classifier.rootMeanSquare(grain);
		grain.extractedFeatures[Palette::Feature::SpectralCentroid] = classifier.spectralCentroid(grain) / 22000.f;
	}

	grainDisplay.setGrains(snareGrains100);

    addAndMakeVisible(&grainDisplay);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
}

PaletteAudioProcessorEditor::~PaletteAudioProcessorEditor()
{
}

//==============================================================================
void PaletteAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void PaletteAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    grainDisplay.setBounds(0, 0, 800, 600);
}
