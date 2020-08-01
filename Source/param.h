/*
  ==============================================================================

	param.h
	Created: 22 Jul 2020 10:55:59am
	Author:  callu

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "lfo.h" 


class Param : public AudioProcessorParameter
{
public:
	Param() { };
	~Param() {};

	void initialise(AudioProcessorParameter* param, AudioPlayHead::CurrentPositionInfo* play)
	{
		soulParameter = param;
		playHead = play;
		//lfo = std::make_unique<LFO>(soulParameter, &value, playHead);
		value = soulParameter->getValue();
	}

	AudioPlayHead::CurrentPositionInfo* playHead;
	AudioProcessorParameter* soulParameter;
	double value = 0;
	std::unique_ptr<LFO> lfo;

	float getValue() const override { return soulParameter->getValue(); };
	void setValue(float newValue) override { value = newValue; sendValueChangedMessageToListeners(value); };
	float getDefaultValue() const override { return soulParameter->getDefaultValue(); };
	juce::String getName(int maximumStringLength) const override { return soulParameter->getName(100); };
	juce::String getLabel() const override { return soulParameter->getLabel(); };
	float getValueForText(const juce::String& text) const override { return soulParameter->getValueForText(text); };
	juce::String getText(float newValue, int i)const override { return soulParameter->getText(newValue, i); };
	//NormalisableRange<float>& getNormalisableRange() const override { return *new NormalisableRange(float(0.0), float(1.0)); };
};

