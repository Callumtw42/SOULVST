/*
  ==============================================================================

	soul_processor.h
	Created: 2 Aug 2020 10:49:44am
	Author:  callu

  ==============================================================================
*/


#pragma once
#include <atomic>
#include "soulpatch.cpp"
#include <JuceHeader.h>

using GeneratedClass = SineSynth;
const int MAXVOICES = 3;

struct IDs
{
	const juce::Identifier UID{ GeneratedClass::UID },
		id{ "id" },
		version{ "version" },
		value{ "value" },
		size{ "size" },
		PARAM{ "PARAM" },
		EDITOR{ "EDITOR" };
};

class SoulProcessor : AudioProcessor
{
public:
	//struct IDs;
	SoulProcessor();
	BusesProperties createBuses();
	void prepareToPlay(double sampleRate, int maxBlockSize);
	template <class RenderContext>
	static void populateInputChannels(juce::AudioBuffer<float>& audio, RenderContext& rc);
	SineSynth::MIDIMessage midiBlank(MidiBufferIterator::reference message);
	SineSynth::MIDIMessage midiBytes(MidiBufferIterator::reference message);
	void processBlock(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi);
	void updateAllParameters();
	void updateAnyChangedParameters();

	int sessionID = 0;
	std::atomic<uint32_t> numParametersNeedingUpdate{ 0 };
	int notesOn = 0;
	static const int numParams = GeneratedClass::numParameters;
	IDs ids;

};

