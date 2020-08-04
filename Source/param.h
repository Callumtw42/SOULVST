/*
  ==============================================================================

	parameter.h
	Created: 2 Aug 2020 10:48:07am
	Author:  callu

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "lfo.h"

struct Voice : SineSynth
{
	Voice()
	{
		for (AudioProcessorParameter* p : getParameters())
		{
			params.set(p->getName(100), p);
		}
	}
	int noteOn = -1;
	HashMap <juce::String, AudioProcessorParameter* >params;

};

class Param : public AudioProcessorParameter
{
public:
	Param(std::array<AudioProcessorParameter*, MAXVOICES> endPoints) :
		endPoints(endPoints)
	{
		lfoPlot.fill(0.0f);
		initLFOs();
	}

	void initLFOs()
	{
		for (int i = 0; i < MAXVOICES; ++i)
		{
			lfos[i] = new LFO(endPoints[i], &value, &lfoSpeed, lfoPlot);
		}
	}

	float getValue() const override { return value; };

	void setValue(float newValue) override
	{
		value = newValue;
		sendValueChangedMessageToListeners(newValue);
		for (int i = 0; i < MAXVOICES; ++i)
		{
			if (!lfos[i]->isOn)	endPoints[i]->setValue(newValue);
		}
	};

	float getDefaultValue() const override { return endPoints[0]->getDefaultValue(); };

	float getValueForText(const juce::String& text) const override { return endPoints[0]->getValueForText(text); }

	juce::String getName(int maximumStringLength) const override { return endPoints[0]->getName(100); };

	juce::String getLabel() const override { return endPoints[0]->getLabel(); };

	void enableLFOs() { lfosEnabled = true; };
	void disableLFOs() { lfosEnabled = false; };
	void setLFOSpeed(double d) { lfoSpeed = d; };
	void triggerLFO(int i, MidiMessage message)
	{
		if (lfosEnabled) {
			if (message.isNoteOn())
				lfos[i]->start();
			else if (message.isNoteOff())
				lfos[i]->stop();
		}
	};
	
	std::array<double, LFORES> lfoPlot;

private:
	float value = 0.0;
	double lfoSpeed = 0.0;
	std::array<AudioProcessorParameter*, MAXVOICES> endPoints;
	std::array<LFO*, MAXVOICES> lfos;
	bool lfosEnabled = true;

};

