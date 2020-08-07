#include "Processor.h"
#include "Editor.h"
#include "soulpatch.cpp"

struct Processor::NoteHandler
{
	void addNote(int note, int voice)
	{
		noteMap[note] = voice;
	}

	int getVoicePlaying(int note)
	{
		return noteMap[note];
	}

	std::array<int, 128> noteMap;
};

Processor::Processor() :
	voices(initVoices()),
	editor(new Editor(*this)),
	noteHandler(new NoteHandler()),
	AudioProcessor(createBuses())
{
	initParams();
}

Processor::~Processor() {}



std::array<Voice*, MAXVOICES> Processor::initVoices()
{
	std::array<Voice*, MAXVOICES> voices;
	for (int i = 0; i < MAXVOICES; ++i)
	{
		voices[i] = new Voice();
	}
	return voices;
}

void Processor::initParams()
{
	for (AudioProcessorParameter* p : voices[0]->getParameters())
	{
		juce::String name = p->getName(100);
		Param* param = new Param(getEndPointGroup(name));
		params.set(name, param);
		addParameter(param);
		param->addListener(dynamic_cast<Editor*>(editor));
		param->setValue(param->getDefaultValue());
	}
}

void connectParamsToUI()
{

}

void Processor::prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock)
{
	for (int i = 0; i < MAXVOICES; ++i)
	{
		voices[i]->prepareToPlay(sampleRate, maximumExpectedSamplesPerBlock);
	}
}

void Processor::processBlock(AudioBuffer<float>& audio, MidiBuffer& midi)
{

	if (playHead.load() && playHead.load()->getCurrentPosition(playheadPosition))
	{
		bpm = playheadPosition.bpm;
	}

	std::array<MidiBuffer, MAXVOICES> midiOut;
	std::array<AudioBuffer<float>, MAXVOICES> audioOut;
	int numSamples = audio.getNumSamples();
	int numChannels = audio.getNumChannels();
	for (int i = 0; i < MAXVOICES; ++i)
	{
		audioOut[i].setSize(numChannels, numSamples, false, false, true);
	}

	for (MidiMessageMetadata data : midi)
	{
		int msgCount = 0;
		if (data.numBytes < 4)
		{
			MidiMessage message = data.getMessage();
			if (message.isNoteOn())
			{
				int selection = noteCount % MAXVOICES;
				midiOut[selection].addEvent(message, data.samplePosition);
				//voices[selection]->noteOn = message.getNoteNumber();
				noteHandler->addNote(message.getNoteNumber(), selection);
				noteCount++;
				triggerLFOs(selection, message);
			}
			else if (message.isNoteOff())
			{
				//int selection = getVoicePlayingNote(message.getNoteNumber());
				int selection = noteHandler->getVoicePlaying(message.getNoteNumber());
				midiOut[selection].addEvent(message, data.samplePosition);
				voices[selection]->noteOn = -1;
				noteCount--;
				triggerLFOs(selection, message);
			}
			else
			{
				for (int i = 0; i < MAXVOICES; ++i)
				{
					midiOut[i].addEvent(message, data.samplePosition);
				}
			}
		}
	}


	audio.clear();
	for (int i = 0; i < MAXVOICES; ++i)
	{
		voices[i]->processBlock(audioOut[i], midiOut[i]);
		for (int j = 0; j < numChannels; ++j)
			audio.addFrom(j, 0, audioOut[i], j, 0, numSamples);
	}
}

void Processor::triggerLFOs(int v, juce::MidiMessage message)
{
	for (Param* p : params)
	{
		p->triggerLFO(v, message);
	}
}

int Processor::getVoicePlayingNote(int note)
{
	for (int i = 0; i < MAXVOICES; ++i)
	{
		if (voices[i]->noteOn == note)
		{
			return i;
		}
	}
	return 0;
}

Processor::BusesProperties Processor::createBuses()
{
	BusesProperties buses;
	if constexpr (SOUL_SineSynth::numAudioInputChannels > 0)
		buses.addBus(true, "Input", juce::AudioChannelSet::canonicalChannelSet(SOUL_SineSynth::numAudioInputChannels));
	if constexpr (SOUL_SineSynth::numAudioOutputChannels > 0)
		buses.addBus(false, "Output", juce::AudioChannelSet::canonicalChannelSet(SOUL_SineSynth::numAudioOutputChannels));
	return buses;
}

std::array<AudioProcessorParameter*, MAXVOICES> Processor::getEndPointGroup(juce::String paramName)
{
	std::array<AudioProcessorParameter*, MAXVOICES> endPoints;
	for (int i = 0; i < MAXVOICES; ++i)
	{
		endPoints[i] = voices[i]->params.getReference((paramName));
	}
	return endPoints;
}


AudioProcessorEditor* Processor::createEditor() { return editor; }

const juce::String Processor::getName() const { return SOUL_SineSynth::name; }

void Processor::releaseResources() { }
double Processor::getTailLengthSeconds() const { return 0; }
bool Processor::acceptsMidi() const { return true; }
bool Processor::producesMidi() const { return false; }
bool Processor::hasEditor() const { return true; }
int Processor::getNumPrograms() { return 1; }
int Processor::getCurrentProgram() { return 0; }
void Processor::setCurrentProgram(int index) { }
const juce::String Processor::getProgramName(int index) { return{}; }
void Processor::changeProgramName(int index, const juce::String& newName) { }
void Processor::getStateInformation(juce::MemoryBlock& destData) { }
void Processor::setStateInformation(const void* data, int sizeInBytes) { }



AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	Processor* p = new Processor();
	return p;
}

