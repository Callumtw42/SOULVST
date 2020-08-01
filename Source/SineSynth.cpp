#ifndef SOUL_CPP_ASSERT
#define SOUL_CPP_ASSERT(x)
#endif

#include "main_processor.cpp"
#include "parameter.cpp"
#include "editor_component.cpp"
#include "PluginEditor.h"

_SineSynth::_SineSynth()
	: juce::AudioPluginInstance(MainProcessor::createBuses()),
	editor(new Editor(*this))
{
	pimpl = std::make_unique<MainProcessor>(*this);
	refreshParameterList();
}

_SineSynth::~_SineSynth() = default;

//==============================================================================
void _SineSynth::fillInPluginDescription(juce::PluginDescription & d) const
{
	d.name = MainProcessor::GeneratedClass::name;
	d.descriptiveName = MainProcessor::GeneratedClass::description;
	d.pluginFormatName = "Compiled SOUL Patch";
	d.category = MainProcessor::GeneratedClass::category;
	d.manufacturerName = MainProcessor::GeneratedClass::manufacturer;
	d.version = MainProcessor::GeneratedClass::version;
	d.fileOrIdentifier = {};
	d.lastFileModTime = {};
	d.lastInfoUpdateTime = {};
	d.uid = (int)juce::String(MainProcessor::GeneratedClass::UID).hash();
	d.isInstrument = MainProcessor::GeneratedClass::isInstrument;
}

const juce::String _SineSynth::getName() const { return MainProcessor::GeneratedClass::name; }

juce::StringArray _SineSynth::getAlternateDisplayNames() const
{
	juce::StringArray s;
	s.add(MainProcessor::GeneratedClass::name);

	if (MainProcessor::GeneratedClass::description[0] != 0)
		s.add(MainProcessor::GeneratedClass::description);

	return s;
}

bool _SineSynth::isBusesLayoutSupported(const BusesLayout & layout) const
{
	auto processorInputBuses = pimpl->voices[0]->processor.getInputBuses();
	auto processorOutputBuses = pimpl->voices[0]->processor.getOutputBuses();

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
		pimpl->voices[i]->processor.reset();
	}
}
void _SineSynth::prepareToPlay(double sampleRate, int maxBlockSize) { pimpl->prepareToPlay(sampleRate, maxBlockSize); }
void _SineSynth::releaseResources() { midiKeyboardState.reset(); }

void _SineSynth::processBlock(juce::AudioBuffer<float> & audio, juce::MidiBuffer & midi)
{
	if (isSuspended())
	{
		for (int i = 0; i < (int)MainProcessor::GeneratedClass::numAudioOutputChannels; ++i)
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
bool _SineSynth::acceptsMidi() const { return MainProcessor::GeneratedClass::hasMIDIInput; }
bool _SineSynth::producesMidi() const { return false; }
bool _SineSynth::supportsMPE() const { return MainProcessor::GeneratedClass::hasMIDIInput; }
bool _SineSynth::isMidiEffect() const { return acceptsMidi() && producesMidi(); }
void _SineSynth::setNonRealtime(bool) noexcept {}

//==============================================================================

void _SineSynth::MainProcessor::updateAllParameters()
{
	for (auto& p : owner.allParameters)
		for (size_t i = 0; i < MAXVOICES; i++)
		{
			p->sendUpdate(i);
		}
}

void _SineSynth::MainProcessor::updateAnyChangedParameters()
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
		for (auto& p : pimpl->voices[i]->processor.getParameterProperties())
		{
			if (i == 0)
			{
				auto mainParam = std::make_unique<Parameter>(*this, p);
				pimpl->voices[i]->endPointParameters[i]->setParamValue(&mainParam->currentFullRangeValue);
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
	generateVoices();
}

void _SineSynth::generateVoices()
{
	//Might Get Deleted and cause error
	const int numParams = MainProcessor::GeneratedClass::numParameters;
	std::array<float*, numParams> paramValues;

	int count = 0;
	for (auto& p : allParameters)
	{
		paramValues[count] = &p->currentFullRangeValue;
		count++;
	}
	//for (size_t i = 0; i < MAXVOICES; i++)
	//{
	//	pimpl->voices[i].setParameters
	//}
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
	state.setProperty(pimpl->ids.version, MainProcessor::GeneratedClass::version, nullptr);

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

bool _SineSynth::hasEditor() const { return true; }
juce::AudioProcessorEditor* _SineSynth::createEditor()
{
	/*return new EditorComponent(*this);*/ return editor;
}

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


void _SineSynth::linkParams()
{
	Logger::writeToLog("Loaded: " + getName());
	for (AudioProcessorParameter* p : allParameters)
	{
		juce::String name = p->getName(100);
		//params.set(name, new Param());
		Param* mainParam = params.getReference(name);
		mainParam->initialise(p, &playHead);
		mainParam->addListener(dynamic_cast<AudioProcessorParameter::Listener*> (editor));
		addParameter(mainParam);
		mainParam->sendValueChangedMessageToListeners(mainParam->getValue());
	}
}


juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	_SineSynth* p = new _SineSynth();
	//static_cast<Editor*>(p->editor)->updateParams();
	return p;
}
