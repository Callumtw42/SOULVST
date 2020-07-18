/*
  ==============================================================================

	lfo.h
	Created: 12 Jul 2020 11:03:20pm
	Author:  callu

  ==============================================================================
*/

#define _USE_MATH_DEFINES
#include <JuceHeader.h>
#include <math.h>
using namespace juce;

class LFO
{
public:
	LFO(AudioProcessorParameter* param, double* lfoPlot) : parameter(param), table(lfoPlot)
	{
	};

	~LFO() {};

	void process()
	{
		float lfoValue = table[position % 128];
		parameter->setValue(lfoValue);
		position++;
	}

	int position = 0;
	double* table;
	//AudioProcessor* processor;
	AudioProcessorParameter* parameter;
	static constexpr size_t lfoUpdateRate = 100;
	size_t lfoUpdateCounter = lfoUpdateRate;
};

#pragma once
