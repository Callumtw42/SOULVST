/*
  ==============================================================================

	lfo.h
	Created: 12 Jul 2020 11:03:20pm
	Author:  callu

  ==============================================================================
*/

//static const int DEFAULT_TEMPO = 128;

#pragma once
#define _USE_MATH_DEFINES
#include <JuceHeader.h>
#include <math.h>

inline AudioPlayHead::CurrentPositionInfo playheadPosition;
inline double bpm = 128;

class LFO
{
public:
	LFO(AudioProcessorParameter* endPoint, float* paramValue, double* speed, std::array<double, LFORES>& plot, float& modAmt)
		:endPoint(endPoint), paramSliderValue(paramValue), speed(speed), plot(plot), modAmt(modAmt)
	{
		plot.fill(0);

	};

	~LFO() {};

	void start()
	{
		if (!isOn) {
			isOn = true;
			std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();
			std::thread proc([this, startTime]()
				{
					while (isOn)
					{
						process(startTime);
						std::this_thread::sleep_for(std::chrono::microseconds(minInterval));
					}
				});
			proc.detach();
		}
	}

	void stop()
	{
		isOn = false;
	}



	void process(std::chrono::steady_clock::time_point startTime)
	{
		//double bpm = *this->bpm;
		//if (bpm < 0)
			//bpm = DEFAULT_TEMPO;
		double microSecondsPerBeat = (60.0 / bpm) * 1000000;
		double barLength = microSecondsPerBeat * 4;
		std::chrono::duration<float> elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
		double microsecondsElapsed = elapsedTime.count() * 1000000;
		double barsElapsed = microsecondsElapsed / barLength;
		double intervalsElapsed = barsElapsed / *speed;
		int index = std::round(intervalsElapsed * LFORES);
		double lfoVal = plot[index % LFORES];
		double outVal = std::clamp(*paramSliderValue + lfoVal * modAmt, 0.0, 1.0);
		//std::function<void(float)> endPoint = *this->endP;
		//callBack(outVal);
		endPoint->setValue(outVal);

		//soulParam->setValue(outVal);
	}

	//AudioPlayHead::CurrentPositionInfo* playHead;
	int minInterval = 1000;
	//double* bpm;
	double* speed;
	std::array<double, LFORES>& plot;
	float* paramSliderValue;
	float& modAmt;
	//void(*callBack)(double);
	//std::function<void(float)>* callBack;
	AudioProcessorParameter* endPoint;
	bool isOn = false;

	//AudioProcessorParameter* soulParam;
};
