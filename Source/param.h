/*
  ==============================================================================

	parameter.h
	Created: 2 Aug 2020 10:48:07am
	Author:  callu

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
//#include "voice.h"
#include "soulpatch.cpp"

using GeneratedClass = SineSynth;
class Param : public juce::AudioProcessorParameterWithID
{
public:
	Param(std::array < Voice*, MAXVOICES> voices);
	float currentFullRangeValue = 0;
	bool needsUpdate = false;
	//_SineSynth* owner;
	std::array < Voice*, MAXVOICES> voices;
	Voice::GeneratedClass::ParameterProperties* properties;
	//std::array<MainProcessor::EndPointParameter*, MAXVOICES>* endPoints;
	juce::StringArray textValues;
	const juce::NormalisableRange<float>* range;
	int numDecimalPlaces;
	std::atomic<uint32_t>* numParametersNeedingUpdate;

	juce::String getName(int maximumStringLength) const override { return name.substring(0, maximumStringLength); }
	juce::String getLabel() const override { return properties->unit; }
	Category getCategory() const override { return genericParameter; }
	bool isDiscrete() const override { return range->interval != 0; }
	bool isBoolean() const override { return properties->isBoolean; }
	bool isAutomatable() const override { return properties->isAutomatable; }
	bool isMetaParameter() const override { return false; }
	juce::StringArray getAllValueStrings() const override { return textValues; }
	float getDefaultValue() const override { return convertTo0to1(properties->initialValue); }
	float getValue() const override { return convertTo0to1(currentFullRangeValue); }
	void setValue(float newValue) override { setFullRangeValue(convertFrom0to1(newValue)); }

	void setFullRangeValue(float newValue);

	void sendUpdate(int index);

	bool sendUpdateIfNeeded();
	juce::String getText(float v, int length) const override;

	float getValueForText(const juce::String& text) const override;

	int getNumSteps() const override;

private:
	float convertTo0to1(float v) const { return range->convertTo0to1(range->snapToLegalValue(v)); }
	float convertFrom0to1(float v) const { return range->snapToLegalValue(range->convertFrom0to1(juce::jlimit(0.0f, 1.0f, v))); }

	static int getNumDecimalPlaces(juce::NormalisableRange<float> r);

	static juce::StringArray parseTextValues(const juce::String& text);

	static juce::String preprocessText(juce::CharPointer_UTF8 text, float value);
};
