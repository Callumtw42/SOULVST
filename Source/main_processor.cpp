/*
  ==============================================================================

	pimpl.cpp
	Created: 31 Jul 2020 11:45:53pm
	Author:  callu

  ==============================================================================
*/

#pragma once
#include "soulpatch.cpp"
#include "SineSynth.h"
#include <functional>


struct _SineSynth::MainProcessor
{
	MainProcessor(_SineSynth& p) : owner(p)
	{
		for (size_t i = 0; i < MAXVOICES; i++)
		{
			voices[i] = std::make_unique<Voice>(&owner.playHead.bpm);
		}
	}
	using GeneratedClass = SineSynth;

	_SineSynth& owner;
	int sessionID = 0;
	std::atomic<uint32_t> numParametersNeedingUpdate{ 0 };
	int notesOn = 0;
	static const int numParams = GeneratedClass::numParameters;

	struct EndPointParameter
	{
		EndPointParameter(GeneratedClass::ParameterProperties* endPoint, double* bpm) :
			endPoint(endPoint),
			//paramValue(paramValue),
			lfo(std::make_unique<LFO>(&setValue, paramValue, bpm))
		{ }

		void setParamValue(float* f)
		{
			paramValue = f;
			//if (!lfo->isOn)
			//{
			//	setValue(*f);
			//}
		}


		GeneratedClass::ParameterProperties* endPoint;
		std::unique_ptr<LFO> lfo;
		float* paramValue;
		std::function<void(float)> setValue = [&](float f) { endPoint->setValue(f); };
	};

	struct Voice
	{
		Voice(double* bpm)
		{
			for (size_t i = 0; i < numParams; i++)
			{
				endPointParameters[i] = std::make_unique<EndPointParameter>(&processor.getParameterProperties()[i], bpm);
			}
		};
		GeneratedClass processor;
		juce::AudioBuffer<float> outputBuffer;
		std::vector<GeneratedClass::MIDIMessage> incomingMIDIMessages;
		std::array<std::unique_ptr<EndPointParameter>, numParams> endPointParameters;
		int midiBufferSize = 0;
		int noteOn = -1;
		int numMessages = 0;

		void startLFOs()
		{
			for (size_t i = 0; i < GeneratedClass::numParameters; i++)
			{
				endPointParameters[i]->lfo->start();
			}
		}
	};

	std::array<std::unique_ptr<Voice>, MAXVOICES> voices;

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

	IDs ids;

	static BusesProperties createBuses()
	{
		BusesProperties buses;

		if constexpr (GeneratedClass::numAudioInputChannels > 0)
			buses.addBus(true, "Input", juce::AudioChannelSet::canonicalChannelSet(GeneratedClass::numAudioInputChannels));

		if constexpr (GeneratedClass::numAudioOutputChannels > 0)
			buses.addBus(false, "Output", juce::AudioChannelSet::canonicalChannelSet(GeneratedClass::numAudioOutputChannels));

		return buses;
	}

	void prepareToPlay(double sampleRate, int maxBlockSize)
	{
		for (size_t i = 0; i < MAXVOICES; i++)
		{
			voices[i]->processor.init(sampleRate, ++sessionID);
		}

		for (size_t i = 0; i < MAXVOICES; i++)
		{
			//voices[i].incomingMIDIMessages.resize((size_t)maxBlockSize);
			voices[i]->midiBufferSize = maxBlockSize;
		}
		owner.setRateAndBufferSizeDetails(sampleRate, maxBlockSize);
		owner.midiKeyboardState.reset();
		for (size_t i = 0; i < MAXVOICES; i++)
		{
			voices[i]->outputBuffer.setSize(GeneratedClass::numAudioOutputChannels, maxBlockSize, false, false, true);
		}
		updateAllParameters();
		owner.linkParams();

	}

	template <class RenderContext>
	static void populateInputChannels(juce::AudioBuffer<float>& audio, RenderContext& rc)
	{
		if constexpr (GeneratedClass::numAudioInputChannels > 0)
		{
			for (int i = 0; i < (int)GeneratedClass::numAudioInputChannels; ++i)
				rc.inputChannels[i] = audio.getReadPointer(i);
		}
	}

	SineSynth::MIDIMessage midiBlank(MidiBufferIterator::reference message)
	{

		return  { static_cast<uint32_t> (message.samplePosition),
								  static_cast<uint8_t> (0),
								  static_cast<uint8_t> (0),
								  static_cast<uint8_t> (0) };
	}

	SineSynth::MIDIMessage midiBytes(MidiBufferIterator::reference message)
	{

		return  { static_cast<uint32_t> (message.samplePosition),
								  static_cast<uint8_t> (message.data[0]),
								  static_cast<uint8_t> (message.data[1]),
								  static_cast<uint8_t> (message.data[2]) };
	}

	void processBlock(juce::AudioBuffer<float>& audio, juce::MidiBuffer& midi)
	{

		int numSamples = audio.getNumSamples();
		std::array<GeneratedClass::RenderContext<float>, MAXVOICES> rcs;

		for (size_t i = 0; i < MAXVOICES; i++)
		{

			voices[i]->incomingMIDIMessages = std::vector<GeneratedClass::MIDIMessage>();
			voices[i]->incomingMIDIMessages.resize(voices[i]->midiBufferSize);
			voices[i]->outputBuffer.setSize(GeneratedClass::numAudioOutputChannels, numSamples, false, false, true);
			voices[i]->outputBuffer.clear();
			voices[i]->numMessages = 0;

			populateInputChannels(audio, rcs[i]);

			for (int j = 0; j < (int)GeneratedClass::numAudioOutputChannels; ++j)
				rcs[i].outputChannels[j] = voices[i]->outputBuffer.getWritePointer(j);

			rcs[i].numFrames = (uint32_t)numSamples;
		}
		if (midi.isEmpty())
		{
			for (size_t i = 0; i < MAXVOICES; i++)
			{
				rcs[i].incomingMIDI.numMessages = 0;
			}
		}
		else
		{
			owner.midiKeyboardState.processNextMidiBuffer(midi, 0, numSamples, true);
			auto maxEvents = voices[0]->incomingMIDIMessages.size();
			auto iter = midi.cbegin();
			auto end = midi.cend();
			size_t j = 0;

			while (j < maxEvents && iter != end)
			{
				auto message = *iter++;
				if (message.numBytes < 4)
				{
					for (size_t i = 0; i < MAXVOICES; i++)
					{
						voices[i]->incomingMIDIMessages[j] = midiBlank(message);
						voices[i]->numMessages++;
					}
					if (message.getMessage().isNoteOn())
					{
						int note = message.getMessage().getNoteNumber();
						Voice* voice = voices[notesOn % MAXVOICES].get();
						std::vector<SineSynth::MIDIMessage>* incBuffer = &voice->incomingMIDIMessages;
						incBuffer->at(j) = midiBytes(message);
						voice->numMessages++;
						voice->noteOn = note;
						notesOn++;
					}
					else if (message.getMessage().isNoteOff())
					{
						int note = message.getMessage().getNoteNumber();
						std::vector<SineSynth::MIDIMessage>* incBuffer;
						Voice* voice;
						for (size_t i = 0; i < MAXVOICES; i++)
						{
							if (voices[i]->noteOn == note)
							{
								notesOn--;
								voice = voices[i].get();
								incBuffer = &voice->incomingMIDIMessages;
								incBuffer->at(j) = midiBytes(message);
								voice->noteOn = -1;
								voice->numMessages++;
								break;
							}
						}
					}
					else
					{
						for (size_t i = 0; i < MAXVOICES; i++)
						{
							voices[i]->incomingMIDIMessages[j] = midiBytes(message);
							voices[i]->numMessages++;
						}
					}
					j++;
				}
			}
			for (size_t i = 0; i < MAXVOICES; i++)
			{
				rcs[i].incomingMIDI.messages = std::addressof(voices[i]->incomingMIDIMessages[0]);
				rcs[i].incomingMIDI.numMessages = voices[i]->numMessages;
			}
		}

		midi.clear();
		updateAnyChangedParameters();
		for (size_t i = 0; i < MAXVOICES; i++)
		{
			voices[i]->processor.render(rcs[i]);
			for (int j = 0; j < voices[i]->outputBuffer.getNumChannels(); ++j)
				audio.addFrom(j, 0, voices[i]->outputBuffer, j, 0, numSamples);
		}
	}

	void updateAllParameters();
	void updateAnyChangedParameters();

};

