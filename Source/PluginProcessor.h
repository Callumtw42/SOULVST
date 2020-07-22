/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

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

static const int MAXVOICES = 1;
static const int LFORES = 128;

using namespace juce;
using namespace soul::patch;

class Param : public AudioProcessorParameter
{
public:
	Param(AudioProcessorParameter* param) : soulParameter(param)
	{
		value = soulParameter->getValue();
		plot.fill(0.0);
	};
	~Param() {};

	AudioProcessorParameter* soulParameter;
	std::array<double, LFORES> plot;
	double modAmt = 0;
	double value = 0;

	float getValue() const override { return soulParameter->getValue(); };
	void setValue(float newValue) override { value = newValue; sendValueChangedMessageToListeners(value); };
	float getDefaultValue() const override { return soulParameter->getDefaultValue(); };
	juce::String getName(int maximumStringLength) const override { return soulParameter->getName(100); };
	juce::String getLabel() const override { return soulParameter->getLabel(); };
	float getValueForText(const juce::String& text) const override { return soulParameter->getValueForText(text); };
	juce::String getText(float newValue, int i)const override { return soulParameter->getText(newValue, i); };
};

class LFO
{
public:
	LFO(AudioProcessorParameter* soulParam, HashMap<juce::String, Param*>* params) : soulParam(soulParam), mainParams(params)
	{
		//startTimer(10);
		//if (soulParam->getName(100).contains("LFOIn"))
			//isLFOParam = true;
		std::thread proc([this]()
			{
				while (true) {
					//auto start = std::chrono::high_resolution_clock::now();
					process();
					std::this_thread::sleep_for(std::chrono::microseconds(interval));
					//auto end = std::chrono::high_resolution_clock::now();
					//std::chrono::duration<float>duration = end - start;
					//Logger::writeToLog(juce::String(duration.count() * 1000000));
				}
			});
		proc.detach();
	};

	~LFO() {};

	void process()
	{
		std::chrono::duration<float> elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
		float microseconds = elapsedTime.count() * 1000000;
		int index = std::ceil(microseconds / interval);

		Param* mainParam = mainParams->getReference(soulParam->getName(100));
		double lfoVal = mainParam->plot[index % LFORES];
		double mainVal = mainParam->value;
		double outVal = std::clamp(mainVal + lfoVal, 0.0, 1.0);;
		soulParam->setValue(outVal);
		position++;
		//});
	}
	int interval = 1000;
	std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();
	int position = 0;
	HashMap<juce::String, Param*>* mainParams;
	AudioProcessorParameter* soulParam;
	static constexpr size_t lfoUpdateRate = 100;
	size_t lfoUpdateCounter = lfoUpdateRate;
};

class SoulVoice
{
public:
	SoulVoice() {};
	~SoulVoice() {};

	void connectLFOs()
	{
		lfos.clear();
		for (AudioProcessorParameter* soulParam : processor->getParameters())
		{
			juce::String paramName = soulParam->getName(100);
			//Logger::writeToLog(params[paramName]->parameter->getName(100));
			//Logger::writeToLog(paramName);
			lfos.add(new LFO(soulParam, mainParams));
		}
	}

	void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
	{
		//for (auto lfo : lfos)lfo->process();
		processor->processBlock(buffer, midiMessages);
	}

	//std::map<juce::String, Param*> mainParams;
	juce::HashMap<juce::String, Param*>* mainParams;
	int processCount = 0;
	SOULPatchAudioProcessor* processor = nullptr;
	juce::Array<LFO*> lfos;
};

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
	//std::map<juce::String, juce::AudioProcessorParameter*> params;
	//const std::map < juce::String, Param*> params;
	HashMap<juce::String, Param*> params;
	int voicesSet = 0;
	int voicesInitialised = 0;
	bool isPlayable;
	SoulVoice* soulVoices[MAXVOICES];
	//std::map < juce::String, double* [LFORES] > LFOPlots;
	//double* defaultPlot[LFORES] = { 0 };

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


