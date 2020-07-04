/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../SOUL/source/API/soul_patch/API/soul_patch.h"
#include "../../SOUL/source/API/soul_patch/helper_classes/soul_patch_AudioPluginFormat.h"
#include "../../SOUL/source/API/soul_patch/helper_classes/soul_patch_Utilities.h"
#include "../../SOUL/source/API/soul_patch/helper_classes/soul_patch_CompilerCacheFolder.h"
#include "../../SOUL/source/API/soul_patch/helper_classes/soul_patch_AudioProcessor.h"
#include "../../SOUL/examples/SOULPatchHostDemo/Source/PatchLoaderComponent.h"

using namespace juce;
using AudioGraphIOProcessor = juce::AudioProcessorGraph::AudioGraphIOProcessor;
using Node = juce::AudioProcessorGraph::Node;

//==============================================================================
/**
*/
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

	std::unique_ptr<juce::AudioProcessorGraph> mainProcessor;
	std::unique_ptr<juce::AudioPluginInstance> plugin;

private:
	void initialiseGraph();
	void connectMidiNodes();
	void connectAudioNodes();
	AudioProcessorGraph* g = new AudioProcessorGraph();
	Node::Ptr audioInputNode;
	Node::Ptr soulPatchNode;
	Node::Ptr audioOutputNode;
	Node::Ptr midiInputNode;
	Node::Ptr midiOutputNode;
	Node::Ptr osc;
	PatchLoaderComponent* patchLoader;
	AudioProcessorPlayer* player;
	AudioDeviceManager* manager;
	soul::patch::SOULPatchAudioPluginFormat* patchFormat;


	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DefaultpluginAudioProcessor)
};

class ProcessorBase : public juce::AudioProcessor
{
public:
	//==============================================================================
	ProcessorBase() {}

	//==============================================================================
	void prepareToPlay(double, int) override {}
	void releaseResources() override {}
	void processBlock(juce::AudioSampleBuffer&, juce::MidiBuffer&) override {}

	//==============================================================================
	juce::AudioProcessorEditor* createEditor() override { return nullptr; }
	bool hasEditor() const override { return false; }

	//==============================================================================
	const juce::String getName() const override { return {}; }
	bool acceptsMidi() const override { return false; }
	bool producesMidi() const override { return false; }
	double getTailLengthSeconds() const override { return 0; }

	//==============================================================================
	int getNumPrograms() override { return 0; }
	int getCurrentProgram() override { return 0; }
	void setCurrentProgram(int) override {}
	const juce::String getProgramName(int) override { return {}; }
	void changeProgramName(int, const juce::String&) override {}

	//==============================================================================
	void getStateInformation(juce::MemoryBlock&) override {}
	void setStateInformation(const void*, int) override {}

private:
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorBase)
};

class OscillatorProcessor : public ProcessorBase
{
public:
	OscillatorProcessor()
	{
		oscillator.setFrequency(880.0f);
		oscillator.initialise([](float x) { return std::sin(x); });
	}

	void prepareToPlay(double sampleRate, int samplesPerBlock) override
	{
		juce::dsp::ProcessSpec spec{ sampleRate, static_cast<juce::uint32> (samplesPerBlock) };
		oscillator.prepare(spec);
	}

	void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override
	{
		juce::dsp::AudioBlock<float> block(buffer);
		juce::dsp::ProcessContextReplacing<float> context(block);
		oscillator.process(context);
	}

	void reset() override
	{
		oscillator.reset();
	}

	const juce::String getName() const override { return "Oscillator"; }

private:
	juce::dsp::Oscillator<float> oscillator;
};
