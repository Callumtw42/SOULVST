
#pragma once
const int MAXVOICES = 12;
const int LFORES = 128;
#include <JuceHeader.h>
#include "soulpatch.h"
#include "param.h"


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
	void initParams();
	std::array<AudioProcessorParameter*, MAXVOICES> getEndPointGroup(juce::String paramName);
	int getVoicePlayingNote(int note);
	void triggerLFOs(int v, juce::MidiMessage message);

	int noteCount = 0;
	AudioProcessorEditor* editor;

	std::array<Voice*, MAXVOICES> voices;
	HashMap<juce::String, Param*> params;
private:
	struct NoteHandler;
	NoteHandler* noteHandler;
};

#define SOUL_HEADER_INCLUDED_167054764 1


