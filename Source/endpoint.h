/*
  ==============================================================================

	endpoint.h
	Created: 2 Aug 2020 4:57:02pm
	Author:  callu

  ==============================================================================
*/
#pragma once

using GeneratedClass = SineSynth;

struct EndPoint
{
	EndPoint(GeneratedClass::ParameterProperties* endPoint, double* bpm) :
		endPoint(endPoint),
		lfo(std::make_unique<LFO>(&setValue, paramValue, bpm))
	{ }

	void setParamValueAddress(float* f)
	{
		paramValue = f;
	}

	void setValueFromSlider()
	{
		if (!lfo->isOn)
		{
			endPoint->setValue(*paramValue);
		}
	}

	GeneratedClass::ParameterProperties* endPoint;
	std::unique_ptr<LFO> lfo;
	float* paramValue;
	std::function<void(float)> setValue = [&](float f) { endPoint->setValue(f); };
};
