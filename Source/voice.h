/*
  ==============================================================================

	voice.h
	Created: 2 Aug 2020 4:55:25pm
	Author:  callu

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
//#include "soul_processor.h"
#include "endpoint.h"

const int numParams = GeneratedClass::numParameters;

struct Voice
{
	Voice(double* bpm, int sessionID)
	{
		this->sessionID = sessionID;
		for (size_t i = 0; i < numParams; i++)
		{
			endPointParameters[i] = std::make_unique<EndPoint>(&processor.getParameterProperties()[i], bpm);
		}
	}
	;
	void startLFOs()
	{
		for (size_t i = 0; i < GeneratedClass::numParameters; i++)
		{
			endPointParameters[i]->lfo->start();
		}
	}

	void initialise(double sampleRate, int maxBlockSize)
	{
		processor.init(sampleRate, sessionID);
		outputBuffer.setSize(GeneratedClass::numAudioOutputChannels, maxBlockSize, false, false, true);
		midiBufferSize = maxBlockSize;
	}
	
	int sessionID;
	GeneratedClass processor;
	juce::AudioBuffer<float> outputBuffer;
	std::vector<GeneratedClass::MIDIMessage> incomingMIDIMessages;
	std::array<std::unique_ptr<EndPoint>, numParams> endPointParameters;
	int midiBufferSize = 0;
	int noteOn = -1;
	int numMessages = 0;

};

