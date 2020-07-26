/*
  ==============================================================================

	soulvoice.h
	Created: 22 Jul 2020 10:56:16am
	Author:  callu

  ==============================================================================
*/

#pragma once

#include<JuceHeader.h>
#include "param.h"
using namespace juce;

class SoulVoice
{
public:
	SoulVoice() {};
	~SoulVoice() {};

	void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
	{
		processor->processBlock(buffer, midiMessages);
	}

	//juce::HashMap<juce::String, Param*>* mainParams;
	HashMap < juce::String, Param*>* mainParams;
	SOULPatchAudioProcessor* processor = nullptr;
};

