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
	LFO(AudioProcessor* proc, AudioProcessorParameter* param) : processor(proc), parameter(param)
	{
		double sampleRate = processor->getSampleRate();
		double blockSize = processor->getBlockSize();
		int numChannels = processor->getNumOutputChannels();

		//initialise([](float x) { return std::sin(x); }, 128);
		//setFrequency(3.0f);
		//prepare({ sampleRate / lfoUpdateRate, blockSize, numChannels });

		for (int i = 0; i < 128; i++)
		{
			table[i] = std::sin((2 * M_PI / 128) * i);
		}
	};

	~LFO() {};

	void process()
	{
		float lfoValue = table[position % 128] / 2 + 0.5;
		Logger::writeToLog(juce::String(lfoValue));
		parameter->setValue(lfoValue);
		position++;
	}

	int position = 0;
	float table[128];
	AudioProcessor* processor;
	AudioProcessorParameter* parameter;
	static constexpr size_t lfoUpdateRate = 100;
	size_t lfoUpdateCounter = lfoUpdateRate;
};

#pragma once
