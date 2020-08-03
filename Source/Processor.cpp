#include "Processor.h"
#include "Editor.h"
#include "soulpatch.cpp"

Processor::Processor() :
	voices(initVoices()),
	AudioProcessor(createBuses()) {}

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

void Processor::prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock)
{
	for (int i = 0; i < MAXVOICES; ++i)
	{
		voices[i]->prepareToPlay(sampleRate, maximumExpectedSamplesPerBlock);
	}
}



//juce::MidiMessage midiBlank(int samplePosition)
//{
//
//	return  { static_cast<uint32_t> (samplePosition),
//							  static_cast<uint8_t> (0),
//							  static_cast<uint8_t> (0),
//							  static_cast<uint8_t> (0) };
//}
//
//juce::MIDIMessage midiBytes(int samplePosition)
//{
//
//	return  { static_cast<uint32_t> (message.samplePosition),
//							  static_cast<uint8_t> (message.data[0]),
//							  static_cast<uint8_t> (message.data[1]),
//							  static_cast<uint8_t> (message.data[2]) };
//}

void Processor::processBlock(AudioBuffer<float>& audio, MidiBuffer& midi)
{
	//Populate 3 arrays with blank midimessages
	std::array<MidiBuffer, MAXVOICES> midiOut;
	std::array<AudioBuffer<float>, MAXVOICES> audioOut;
	int numSamples = audio.getNumSamples();
	int numChannels = audio.getNumChannels();
	for (int i = 0; i < MAXVOICES; ++i)
	{
		audioOut[i].setSize(numChannels, numSamples, false, false, true);
	}
	//for (int i = 0; i < MAXVOICES; ++i)
	//{
		//midiOut[i].resize(getBlockSize());
		//int msgCount = 0;
		//for (MidiMessageMetadata data : midi)
		//{
		//	if (data.numBytes < 4)
		//		midiOut[i][msgCount] = midiBlank(data.samplePosition);
		//}
	//}

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
				voices[selection]->noteOn = message.getNoteNumber();
				noteCount++;
			}
			else if (message.isNoteOff())
			{
				int selection = getVoicePlayingNote(message.getNoteNumber());
				midiOut[selection].addEvent(message, data.samplePosition);
				voices[selection]->noteOn = -1;
				noteCount--;
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


AudioProcessorEditor* Processor::createEditor() { return new Editor(*this); }

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

