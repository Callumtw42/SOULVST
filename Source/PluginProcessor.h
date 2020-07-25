/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <functional>
#include "soulvoice.h"

static const int MAXVOICES = 1;
//static const int TEMPO = 128;

using namespace juce;
using namespace soul::patch;


class DefaultpluginAudioProcessor : public juce::AudioProcessor
{
public:
	//==============================================================================
	DefaultpluginAudioProcessor();
	~DefaultpluginAudioProcessor() override;

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


	std::unique_ptr<juce::AudioPluginInstance> pluginInstances[MAXVOICES];
	HashMap<juce::String, Param*> params;
	int voicesSet = 0;
	int voicesInitialised = 0;
	bool isPlayable;
	SoulVoice* soulVoices[MAXVOICES];

private:
	AudioProcessorPlayer* player;
	AudioDeviceManager* manager;
	SOULPatchAudioPluginFormat* patchFormat;
	AudioProcessorEditor* editor;
	PluginDescription* desc;
	int processCount = 0;
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DefaultpluginAudioProcessor)
};


