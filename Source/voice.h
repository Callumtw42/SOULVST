/*
  ==============================================================================

	voice.h
	Created: 13 Jul 2020 8:51:34am
	Author:  callu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <functional>

#include "../../SOUL/source/API/soul_patch/API/soul_patch.h"
#include "../../SOUL/source/API/soul_patch/helper_classes/soul_patch_AudioPluginFormat.h"
#include "../../SOUL/source/API/soul_patch/helper_classes/soul_patch_Utilities.h"
#include "../../SOUL/source/API/soul_patch/helper_classes/soul_patch_CompilerCacheFolder.h"
#include "../../SOUL/source/API/soul_patch/helper_classes/soul_patch_AudioProcessor.h"
#include "../../SOUL/examples/SOULPatchHostDemo/Source/PatchLoaderComponent.h"
#include "lfo.h";

using namespace juce;
using namespace soul::patch;

class Voice : public juce::AudioProcessor
{
public:
	//==============================================================================
	Voice();
	~Voice() override;

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	//==============================================================================
	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	//==============================================================================
	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	void initialiseGraph();

	std::unique_ptr<juce::AudioPluginInstance> plugin;

private:
	AudioProcessorPlayer* player;
	AudioDeviceManager* manager;
	SOULPatchAudioPluginFormat* patchFormat;
	bool isPlayable;
	AudioProcessorEditor* editor;
	PluginDescription* desc;
	LFO* lfo;
	AudioProcessorGraph graph;
	std::unique_ptr<SOULPatchAudioProcessor> soulProcessor;
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Voice)
};
