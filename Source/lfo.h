/*
  ==============================================================================

	lfo.h
	Created: 12 Jul 2020 11:03:20pm
	Author:  callu

  ==============================================================================
*/

static const int LFORES = 128;
#pragma once
#define _USE_MATH_DEFINES
#include <JuceHeader.h>
#include <math.h>

class LFO
{
public:
	LFO(AudioProcessorParameter* soulParam, double* v) : soulParam(soulParam), paramSliderValue(v)
	{
		plot.fill(0);
		std::thread proc([this]()
			{
				while (true) {
					process();
					std::this_thread::sleep_for(std::chrono::microseconds(minInterval));
				}
			});
		proc.detach();
	};

	~LFO() {};

	void process()
	{
		float interval = (minInterval + ((maxInterval - minInterval) * (1 - speed)));
		std::chrono::duration<float> elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
		float microseconds = elapsedTime.count() * 1000000;
		int index = std::ceil(microseconds / interval);
		double lfoVal = plot[index % LFORES];
		double outVal = std::clamp(*paramSliderValue + lfoVal, 0.0, 1.0);;
		soulParam->setValue(outVal);
	}
	int minInterval = 1000;
	int maxInterval = 100000;
	double speed = 0.9;
	std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();
	std::array<double, LFORES> plot;
	double* paramSliderValue;
	AudioProcessorParameter* soulParam;
};
