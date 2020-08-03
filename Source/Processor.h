//      _____ _____ _____ __
//     |   __|     |  |  |  |        Auto-generated C++
//     |__   |  |  |  |  |  |__      SOUL Version 0.9.0
//     |_____|_____|_____|_____|     https://soul.dev
//

#pragma once
#include <JuceHeader.h>
#include "soulpatch.h"

//#ifndef JUCE_AUDIO_PROCESSORS_H_INCLUDED
//#error "This file is designed to be included inside a file in a JUCE project, so that the module headers have already been included before it"
//#endif

const int MAXVOICES = 3;
const int LFORES = 128;


struct Voice : SineSynth
{
	int noteOn = -1;
};

class Processor : public AudioProcessor
{
public:
	Processor();
	~Processor() override;
	BusesProperties createBuses();
	const juce::String getName() const override;
	void prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) override;
	void releaseResources() override;
	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;
	double getTailLengthSeconds() const override;
	bool acceptsMidi() const override;
	bool producesMidi() const override;
	AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;
	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;
	std::array<Voice*, MAXVOICES> initVoices();
	int noteCount = 0;
	int getVoicePlayingNote(int note);

	std::array<Voice*, MAXVOICES> voices;
	HashMap<juce::String, AudioProcessorParameter*> params;
};

#define SOUL_HEADER_INCLUDED_167054764 1


