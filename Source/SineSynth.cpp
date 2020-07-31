//      _____ _____ _____ __
//     |   __|     |  |  |  |        Auto-generated C++
//     |__   |  |  |  |  |  |__      SOUL Version 0.9.0
//     |_____|_____|_____|_____|     https://soul.dev
//

#ifndef JUCE_AUDIO_PROCESSORS_H_INCLUDED
#error "This file is designed to be included inside a file in a JUCE project, so that the module headers have already been included before it"
#endif

#ifndef SOUL_HEADER_INCLUDED_167054764
#error "This file is designed to be included inside a file where its corresponding auto-generated header has already been included"
#endif

#ifndef SOUL_CPP_ASSERT
#define SOUL_CPP_ASSERT(x)
#endif

#include "soulpatch.cpp"
//#include "juce_soul_patch.cpp"

//==============================================================================
// Generated from graph 'SineSynth', source file: SineSynth.soulpatch
//

//==============================================================================
//==============================================================================
//
// The rest of this file is now the implementation of juce::AudioPluginInstance
//
//==============================================================================
//==============================================================================

struct _SineSynth::Pimpl
{
	Pimpl(_SineSynth& p) : owner(p) {}
	using GeneratedClass = SineSynth;

	_SineSynth& owner;
	int sessionID = 0;
	std::atomic<uint32_t> numParametersNeedingUpdate{ 0 };
	int counter = 0;
	int notesOn = 0;

	struct Voice
	{
		GeneratedClass processor;
		juce::AudioBuffer<float> outputBuffer;
		std::vector<GeneratedClass::MIDIMessage> incomingMIDIMessages;
		int midiBufferSize = 0;
		int noteOn = -1;
		int numMessages = 0;
	};

	std::array<Voice, MAXVOICES> voices;

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

	static juce::AudioProcessor::BusesProperties createBuses()
	{
		juce::AudioProcessor::BusesProperties buses;

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
			voices[i].processor.init(sampleRate, ++sessionID);
		}

		for (size_t i = 0; i < MAXVOICES; i++)
		{
			//voices[i].incomingMIDIMessages.resize((size_t)maxBlockSize);
			voices[i].midiBufferSize = maxBlockSize;
		}
		owner.setRateAndBufferSizeDetails(sampleRate, maxBlockSize);
		owner.midiKeyboardState.reset();
		for (size_t i = 0; i < MAXVOICES; i++)
		{
			voices[i].outputBuffer.setSize(GeneratedClass::numAudioOutputChannels, maxBlockSize, false, false, true);
		}
		updateAllParameters();
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
		//voices[0].incomingMIDIMessages[0] = MidiMessage::note
		//std::array<int, MAXVOICES> sizes;
		//sizes.fill(0);


		for (size_t i = 0; i < MAXVOICES; i++)
		{

			voices[i].incomingMIDIMessages = std::vector<GeneratedClass::MIDIMessage>();
			voices[i].incomingMIDIMessages.resize(voices[i].midiBufferSize);
			voices[i].outputBuffer.setSize(GeneratedClass::numAudioOutputChannels, numSamples, false, false, true);
			voices[i].outputBuffer.clear();
			voices[i].numMessages = 0;

			populateInputChannels(audio, rcs[i]);

			for (int j = 0; j < (int)GeneratedClass::numAudioOutputChannels; ++j)
				rcs[i].outputChannels[j] = voices[i].outputBuffer.getWritePointer(j);

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
			auto maxEvents = voices[0].incomingMIDIMessages.size();
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
						voices[i].incomingMIDIMessages[j] = midiBlank(message);
						voices[i].numMessages++;
					}
					if (message.getMessage().isNoteOn())
					{
						int note = message.getMessage().getNoteNumber();
						Voice* voice = &voices[counter % MAXVOICES];
						std::vector<SineSynth::MIDIMessage>* incBuffer = &voice->incomingMIDIMessages;
						incBuffer->at(j) = midiBytes(message);						//sizes[counter % MAXVOICES]++;
						voice->numMessages++;
						voice->noteOn = note;
						counter++;
						notesOn++;
						//notesOn.set(note, incBuffer);
					}
					else if (message.getMessage().isNoteOff())
					{
						int note = message.getMessage().getNoteNumber();
						std::vector<SineSynth::MIDIMessage>* incBuffer;
						Voice* voice;
						for (size_t i = 0; i < MAXVOICES; i++)
						{
							if (voices[i].noteOn == note)
							{
								voice = &voices[i];
								incBuffer = &voice->incomingMIDIMessages;
								incBuffer->at(j) = midiBytes(message);								//notesOn.remove(note);
								voice->noteOn = -1;
								voice->numMessages++;
								notesOn--;
								break;
							}
						}
					}
					else
					{
						for (size_t i = 0; i < MAXVOICES; i++)
						{
							voices[i].incomingMIDIMessages[j] = midiBytes(message);
							voices[i].numMessages++;
						}
					}
					j++;
				}

			}
			for (size_t i = 0; i < MAXVOICES; i++)
			{
				rcs[i].incomingMIDI.messages = std::addressof(voices[i].incomingMIDIMessages[0]);
				//rcs[i].incomingMIDI.numMessages = (uint32_t)j;
				rcs[i].incomingMIDI.numMessages = voices[i].numMessages;

			}

		}

		midi.clear();
		updateAnyChangedParameters();
		for (size_t i = 0; i < MAXVOICES; i++)
		{
			voices[i].processor.render(rcs[i]);
			for (int j = 0; j < voices[i].outputBuffer.getNumChannels(); ++j)
				audio.addFrom(j, 0, voices[i].outputBuffer, j, 0, numSamples);
		}
	}

	void updateAllParameters();
	void updateAnyChangedParameters();
};

//==============================================================================
_SineSynth::_SineSynth()
	: juce::AudioPluginInstance(Pimpl::createBuses())
{
	pimpl = std::make_unique<Pimpl>(*this);
	refreshParameterList();
}

_SineSynth::~_SineSynth() = default;

//==============================================================================
void _SineSynth::fillInPluginDescription(juce::PluginDescription & d) const
{
	d.name = Pimpl::GeneratedClass::name;
	d.descriptiveName = Pimpl::GeneratedClass::description;
	d.pluginFormatName = "Compiled SOUL Patch";
	d.category = Pimpl::GeneratedClass::category;
	d.manufacturerName = Pimpl::GeneratedClass::manufacturer;
	d.version = Pimpl::GeneratedClass::version;
	d.fileOrIdentifier = {};
	d.lastFileModTime = {};
	d.lastInfoUpdateTime = {};
	d.uid = (int)juce::String(Pimpl::GeneratedClass::UID).hash();
	d.isInstrument = Pimpl::GeneratedClass::isInstrument;
}

const juce::String _SineSynth::getName() const { return Pimpl::GeneratedClass::name; }

juce::StringArray _SineSynth::getAlternateDisplayNames() const
{
	juce::StringArray s;
	s.add(Pimpl::GeneratedClass::name);

	if (Pimpl::GeneratedClass::description[0] != 0)
		s.add(Pimpl::GeneratedClass::description);

	return s;
}

bool _SineSynth::isBusesLayoutSupported(const BusesLayout & layout) const
{
	auto processorInputBuses = pimpl->voices[0].processor.getInputBuses();
	auto processorOutputBuses = pimpl->voices[0].processor.getOutputBuses();

	if (layout.inputBuses.size() != (int)processorInputBuses.size())
		return false;

	if (layout.outputBuses.size() != (int)processorOutputBuses.size())
		return false;

	for (size_t i = 0; i < processorInputBuses.size(); ++i)
		if ((int)processorInputBuses[i].numChannels != layout.getNumChannels(true, (int)i))
			return false;

	for (size_t i = 0; i < processorOutputBuses.size(); ++i)
		if ((int)processorOutputBuses[i].numChannels != layout.getNumChannels(false, (int)i))
			return false;

	return true;
}

//==============================================================================
void _SineSynth::reset()
{
	for (size_t i = 0; i < MAXVOICES; i++)
	{
		pimpl->voices[i].processor.reset();
	}
}
void _SineSynth::prepareToPlay(double sampleRate, int maxBlockSize) { pimpl->prepareToPlay(sampleRate, maxBlockSize); }
void _SineSynth::releaseResources() { midiKeyboardState.reset(); }

void _SineSynth::processBlock(juce::AudioBuffer<float> & audio, juce::MidiBuffer & midi)
{
	if (isSuspended())
	{
		for (int i = 0; i < (int)Pimpl::GeneratedClass::numAudioOutputChannels; ++i)
			audio.clear(i, 0, audio.getNumSamples());
	}
	else
	{
		pimpl->processBlock(audio, midi);
	}
}

//==============================================================================
int _SineSynth::getNumPrograms() { return 1; }
int _SineSynth::getCurrentProgram() { return 0; }
void _SineSynth::setCurrentProgram(int) {}
const juce::String _SineSynth::getProgramName(int) { return {}; }
void _SineSynth::changeProgramName(int, const juce::String&) {}

double _SineSynth::getTailLengthSeconds() const { return 0; }
bool _SineSynth::acceptsMidi() const { return Pimpl::GeneratedClass::hasMIDIInput; }
bool _SineSynth::producesMidi() const { return false; }
bool _SineSynth::supportsMPE() const { return Pimpl::GeneratedClass::hasMIDIInput; }
bool _SineSynth::isMidiEffect() const { return acceptsMidi() && producesMidi(); }
void _SineSynth::setNonRealtime(bool) noexcept {}

//==============================================================================
struct _SineSynth::Parameter : public juce::AudioProcessorParameterWithID
{
	Parameter(_SineSynth& owner, const Pimpl::GeneratedClass::ParameterProperties& p)
		: AudioProcessorParameterWithID(p.UID, p.name),
		textValues(parseTextValues(p.textValues)),
		range(p.minValue, p.maxValue, p.step),
		numDecimalPlaces(getNumDecimalPlaces(range)),
		numParametersNeedingUpdate(owner.pimpl->numParametersNeedingUpdate)
	{
		properties[0] = p;
		currentFullRangeValue = properties[0].initialValue;
	}

	float currentFullRangeValue = 0;
	bool needsUpdate = false;

	std::array<Pimpl::GeneratedClass::ParameterProperties, MAXVOICES> properties;
	const juce::StringArray textValues;
	const juce::NormalisableRange<float> range;
	const int numDecimalPlaces;
	std::atomic<uint32_t>& numParametersNeedingUpdate;

	juce::String getName(int maximumStringLength) const override { return name.substring(0, maximumStringLength); }
	juce::String getLabel() const override { return properties[0].unit; }
	Category getCategory() const override { return genericParameter; }
	bool isDiscrete() const override { return range.interval != 0; }
	bool isBoolean() const override { return properties[0].isBoolean; }
	bool isAutomatable() const override { return properties[0].isAutomatable; }
	bool isMetaParameter() const override { return false; }
	juce::StringArray getAllValueStrings() const override { return textValues; }
	float getDefaultValue() const override { return convertTo0to1(properties[0].initialValue); }
	float getValue() const override { return convertTo0to1(currentFullRangeValue); }
	void setValue(float newValue) override { setFullRangeValue(convertFrom0to1(newValue)); }

	void setFullRangeValue(float newValue)
	{
		if (newValue != currentFullRangeValue)
		{
			currentFullRangeValue = newValue;

			if (!needsUpdate)
			{
				needsUpdate = true;
				++numParametersNeedingUpdate;
			}

			sendValueChangedMessageToListeners(convertTo0to1(newValue));
		}
	}

	void sendUpdate(int index)
	{
		properties[index].setValue(currentFullRangeValue);
	}

	bool sendUpdateIfNeeded()
	{
		if (!needsUpdate)
			return false;

		needsUpdate = false;
		for (size_t i = 0; i < MAXVOICES; i++)
		{
			sendUpdate(i);
		}
		return true;
	}

	juce::String getText(float v, int length) const override
	{
		if (length > 0)              return getText(v, 0).substring(0, length);
		if (textValues.isEmpty())    return juce::String(convertFrom0to1(v), numDecimalPlaces);
		if (textValues.size() == 1)  return preprocessText(textValues[0].toUTF8(), convertFrom0to1(v));

		return textValues[juce::jlimit(0, textValues.size() - 1, juce::roundToInt(v * (textValues.size() - 1.0f)))];
	}

	float getValueForText(const juce::String& text) const override
	{
		for (int i = 0; i < textValues.size(); ++i)
			if (textValues[i] == text)
				return i / (textValues.size() - 1.0f);

		return convertTo0to1(text.upToLastOccurrenceOf(text, false, false).getFloatValue());
	}

	int getNumSteps() const override
	{
		if (!textValues.isEmpty() && std::abs(textValues.size() - (range.end - range.start)) < 0.01f)
			return textValues.size() - 1;

		if (range.interval > 0)
			return static_cast<int> ((range.end - range.start) / range.interval) + 1;

		return AudioProcessor::getDefaultNumParameterSteps();
	}

private:
	float convertTo0to1(float v) const { return range.convertTo0to1(range.snapToLegalValue(v)); }
	float convertFrom0to1(float v) const { return range.snapToLegalValue(range.convertFrom0to1(juce::jlimit(0.0f, 1.0f, v))); }

	static int getNumDecimalPlaces(juce::NormalisableRange<float> r)
	{
		int places = 7;

		if (r.interval != 0.0f)
		{
			if (juce::approximatelyEqual(std::abs(r.interval - (int)r.interval), 0.0f))
				return 0;

			auto v = std::abs(juce::roundToInt(r.interval * pow(10, places)));

			while ((v % 10) == 0 && places > 0)
			{
				--places;
				v /= 10;
			}
		}

		return places;
	}

	static juce::StringArray parseTextValues(const juce::String& text)
	{
		if (text.isNotEmpty())
			return juce::StringArray::fromTokens(text.unquoted(), "|", {});

		return {};
	}

	static juce::String preprocessText(juce::CharPointer_UTF8 text, float value)
	{
		juce::MemoryOutputStream result;

		while (!text.isEmpty())
		{
			auto c = text.getAndAdvance();

			if (c != '%') { result << juce::String::charToString(c); continue; }

			auto format = text;
			bool addSignChar = (*format == '+');
			if (addSignChar) ++format;

			bool isPadded = (*format == '0');
			int numDigits = 0;

			while (format.isDigit())
				numDigits = numDigits * 10 + (format.getAndAdvance() - '0');

			bool isFloat = (*format == 'f');
			bool isInt = (*format == 'd');

			if (!(isInt || isFloat))
			{
				result << '%';
				continue;
			}

			if (addSignChar && value >= 0)
				result << '+';

			if (isInt)
			{
				juce::String s((int64_t)(value + 0.5f));
				result << (isPadded ? s.paddedLeft('0', numDigits) : s);
			}
			else if (numDigits <= 0)
			{
				result << value;
			}
			else if (isPadded)
			{
				result << juce::String(value, numDigits);
			}
			else
			{
				juce::String s(value);
				auto afterDot = s.fromLastOccurrenceOf(".", false, false);

				if (afterDot.containsOnly("0123456789"))
					if (afterDot.length() > numDigits)
						s = s.dropLastCharacters(afterDot.length() - numDigits);

				result << s;
			}

			text = ++format;
		}

		return result.toString();
	}
};

void _SineSynth::Pimpl::updateAllParameters()
{
	for (auto& p : owner.allParameters)
		for (size_t i = 0; i < MAXVOICES; i++)
		{
			p->sendUpdate(i);
		}
}

void _SineSynth::Pimpl::updateAnyChangedParameters()
{
	if (numParametersNeedingUpdate != 0)
		for (auto& p : owner.allParameters)
			if (p->sendUpdateIfNeeded())
				if (--numParametersNeedingUpdate == 0)
					break;
}

void _SineSynth::refreshParameterList()
{
	struct ParameterTreeGroupBuilder
	{
		std::map<juce::String, juce::AudioProcessorParameterGroup*> groups;
		juce::AudioProcessorParameterGroup tree;

		void addParam(std::unique_ptr<Parameter> newParam)
		{
			juce::String group(newParam->properties[0].group);

			if (group.isNotEmpty())
				getOrCreateGroup(tree, {}, group).addChild(std::move(newParam));
			else
				tree.addChild(std::move(newParam));
		}

		juce::AudioProcessorParameterGroup& getOrCreateGroup(juce::AudioProcessorParameterGroup& targetTree,
			const juce::String& parentPath,
			const juce::String& subPath)
		{
			auto fullPath = parentPath + "/" + subPath;
			auto& targetGroup = groups[fullPath];

			if (targetGroup != nullptr)
				return *targetGroup;

			auto slash = subPath.indexOfChar('/');

			if (slash < 0)
			{
				auto newGroup = std::make_unique<juce::AudioProcessorParameterGroup>(fullPath, subPath, "/");
				targetGroup = newGroup.get();
				targetTree.addChild(std::move(newGroup));
				return *targetGroup;
			}

			auto firstPathPart = subPath.substring(0, slash);
			auto& parentGroup = getOrCreateGroup(targetTree, parentPath, firstPathPart);
			return getOrCreateGroup(parentGroup, parentPath + "/" + firstPathPart, subPath.substring(slash + 1));
		}
	};

	ParameterTreeGroupBuilder treeBuilder;
	for (size_t i = 0; i < MAXVOICES; i++)
	{
		for (auto& p : pimpl->voices[i].processor.getParameterProperties())
		{
			if (i == 0)
			{
				auto mainParam = std::make_unique<Parameter>(*this, p);
				mainParams.set(p.name, mainParam.get());
				allParameters.push_back(mainParam.get());

				if (p.isHidden)
					hiddenParams.push_back(std::move(mainParam));
				else
					treeBuilder.addParam(std::move(mainParam));
			}
			else
			{
				mainParams.getReference(p.name)->properties[i] = p;
			}
		}
	}

	setParameterTree(std::move(treeBuilder.tree));
	pimpl->numParametersNeedingUpdate = static_cast<uint32_t> (allParameters.size());
}

//==============================================================================
void _SineSynth::getStateInformation(juce::MemoryBlock & data)
{
	updateLastState();
	juce::MemoryOutputStream out(data, false);
	lastValidState.writeToStream(out);
}

void _SineSynth::setStateInformation(const void* data, int size)
{
	auto newState = juce::ValueTree::readFromData(data, (size_t)size);

	if (newState.hasType(pimpl->ids.UID))
	{
		lastValidState = std::move(newState);
		applyLastState();
	}
}

juce::ValueTree _SineSynth::createCurrentState()
{
	juce::ValueTree state(pimpl->ids.UID);
	state.setProperty(pimpl->ids.version, Pimpl::GeneratedClass::version, nullptr);

	auto editorState = lastValidState.getChildWithName(pimpl->ids.EDITOR);

	if (editorState.isValid())
		state.addChild(editorState.createCopy(), -1, nullptr);

	for (auto& p : allParameters)
	{
		juce::ValueTree param(pimpl->ids.PARAM);
		for (size_t i = 0; i < MAXVOICES; i++)
		{
			param.setProperty(pimpl->ids.id, p->properties[i].UID, nullptr);
		}
		param.setProperty(pimpl->ids.value, p->currentFullRangeValue, nullptr);
		state.addChild(param, -1, nullptr);
	}

	return state;
}

void _SineSynth::updateLastState()
{
	lastValidState = createCurrentState();
}

void _SineSynth::ensureValidStateExists()
{
	if (!lastValidState.hasType(pimpl->ids.UID))
		updateLastState();
}

void _SineSynth::applyLastState()
{
	if (lastValidState.hasType(pimpl->ids.UID))
		for (auto& p : allParameters)
			if (auto* value = lastValidState.getChildWithProperty(pimpl->ids.id, p->properties[0].UID).getPropertyPointer(pimpl->ids.value))
				p->setFullRangeValue(*value);
}

//==============================================================================
struct _SineSynth::EditorComponent : public juce::AudioProcessorEditor
{
	EditorComponent(_SineSynth& p)
		: juce::AudioProcessorEditor(p), owner(p), editor(p),
		midiKeyboard(p.midiKeyboardState, juce::MidiKeyboardComponent::Orientation::horizontalKeyboard)
	{
		setLookAndFeel(&lookAndFeel);

		if constexpr (Pimpl::GeneratedClass::numParameters != 0)
			addAndMakeVisible(editor);

		if (Pimpl::GeneratedClass::hasMIDIInput)
			addAndMakeVisible(midiKeyboard);

		auto size = owner.getStoredEditorSize({ 600, 400 });
		setResizeLimits(400, 150, 2000, 2000);
		setSize(size.width, size.height);
	}

	~EditorComponent() override
	{
		owner.editorBeingDeleted(this);
		setLookAndFeel(nullptr);
	}

	void paint(juce::Graphics& g) override
	{
		auto background = lookAndFeel.findColour(juce::ResizableWindow::backgroundColourId);
		g.fillAll(background);

		if (getNumChildComponents() == 0)
		{
			g.setColour(background.contrasting());
			g.setFont(16.0f);
			g.drawFittedText(owner.getName(), getLocalBounds().reduced(6), juce::Justification::centred, 2);
		}
	}

	void resized() override
	{
		auto r = getLocalBounds().reduced(6);

		if (midiKeyboard.isVisible())
			midiKeyboard.setBounds(r.removeFromBottom(std::min(80, r.getHeight() / 4)));

		editor.setBounds(r);
		owner.storeEditorSize({ getWidth(), getHeight() });
	}

	_SineSynth& owner;
	juce::LookAndFeel_V4 lookAndFeel;
	juce::GenericAudioProcessorEditor editor;
	juce::MidiKeyboardComponent midiKeyboard;
};

bool _SineSynth::hasEditor() const { return true; }
juce::AudioProcessorEditor* _SineSynth::createEditor() { return new EditorComponent(*this); }

_SineSynth::EditorSize _SineSynth::getStoredEditorSize(EditorSize defaultSize)
{
	auto propertyValue = lastValidState.getChildWithName(pimpl->ids.EDITOR).getProperty(pimpl->ids.size);
	auto tokens = juce::StringArray::fromTokens(propertyValue.toString(), " ", {});

	if (tokens.size() == 2)
	{
		auto w = tokens[0].getIntValue();
		auto h = tokens[1].getIntValue();

		if (w > 0 && h > 0)
			return { w, h };
	}

	return defaultSize;
}

void _SineSynth::storeEditorSize(EditorSize newSize)
{
	ensureValidStateExists();
	auto state = lastValidState.getOrCreateChildWithName(pimpl->ids.EDITOR, nullptr);

	if (newSize.width > 0 || newSize.height > 0)
		state.setProperty(pimpl->ids.size, juce::String(newSize.width) + " " + juce::String(newSize.height), nullptr);
	else
		state.removeProperty(pimpl->ids.size, nullptr);
}

