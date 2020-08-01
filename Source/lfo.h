/*
  ==============================================================================

	lfo.h
	Created: 12 Jul 2020 11:03:20pm
	Author:  callu

  ==============================================================================
*/

static const int LFORES = 128;
static const int DEFAULT_TEMPO = 128;

#pragma once
#define _USE_MATH_DEFINES
#include <JuceHeader.h>
#include <math.h>
#include <functional>


class LFO
{
public:
	LFO(std::function<void(float)>* callback, float* paramValue, double* bpm) :callBack(callback), bpm(bpm), paramSliderValue(paramValue)
	{
		plot.fill(0);

	};

	~LFO() {};

	void start()
	{
		std::thread proc([this]()
			{
				while (true) {
					process();
					std::this_thread::sleep_for(std::chrono::microseconds(minInterval));
				}
			});
		proc.detach();
	}

	void process()
	{
		double bpm = *this->bpm;
		if (bpm < 0)
			bpm = DEFAULT_TEMPO;
		double microSecondsPerBeat = (60.0 / bpm) * 1000000;
		double barLength = microSecondsPerBeat * 4;
		std::chrono::duration<float> elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
		double microsecondsElapsed = elapsedTime.count() * 1000000;
		double barsElapsed = microsecondsElapsed / barLength;
		double intervalsElapsed = barsElapsed / speed;
		int index = std::round(intervalsElapsed * LFORES);
		double lfoVal = plot[index % LFORES];
		double outVal = std::clamp(*paramSliderValue + lfoVal, 0.0, 1.0);
		std::function<void(float)> callBack = *this->callBack;
		callBack(outVal);

		//soulParam->setValue(outVal);
	}

	//AudioPlayHead::CurrentPositionInfo* playHead;
	int minInterval = 1000;
	double* bpm;
	double speed = 0.0;
	std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();
	std::array<double, LFORES> plot;
	float* paramSliderValue;
	//void(*callBack)(double);
	std::function<void(float)>* callBack;

	//AudioProcessorParameter* soulParam;
};
