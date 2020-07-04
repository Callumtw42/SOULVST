#include "PluginProcessor.h"
#include "PluginEditor.h"
//#include "../../SOUL/source/API/soul_patch/helper_classes/soul_patch_AudioProcessor.h"

using namespace juce;
using namespace soul::patch;

DefaultpluginAudioProcessor::DefaultpluginAudioProcessor()
	: AudioProcessor(BusesProperties()
		.withInput("Input", AudioChannelSet::stereo(), true)
		.withOutput("Output", AudioChannelSet::stereo(), true)),
	mainProcessor(new juce::AudioProcessorGraph()),
	player(new AudioProcessorPlayer()),
	manager(new AudioDeviceManager())

{

	manager->initialiseWithDefaultDevices(2, 2);
	manager->addAudioCallback(player);
	AudioIODevice* device = manager->getCurrentAudioDevice();

	juce::File patchPath("C:\\Users\\callu\\SOUL\\examples\\patches\\ClassicRingtone\\ClassicRingtone.soulpatch");
	jassert(patchPath.existsAsFile());

	juce::String dll("C:\\Users\\callu\\SOUL_PatchLoader.dll");
	auto reinitialiseCallback = [this](soul::patch::SOULPatchAudioProcessor& patch)
	{
		player->setProcessor(nullptr);
		patch.reinitialise();
		player->setProcessor(plugin.get());
	};
	SOULPatchAudioPluginFormat* patchFormat = new SOULPatchAudioPluginFormat(dll, reinitialiseCallback);

	juce::PluginDescription* desc = new PluginDescription();
	desc->pluginFormatName = soul::patch::SOULPatchAudioProcessor::getPluginFormatName();
	desc->fileOrIdentifier = patchPath.getFullPathName();

	auto setPlugin = [this](std::unique_ptr<juce::AudioPluginInstance> newPlugin,
		const juce::String& error)
	{
		player->setProcessor(nullptr);
		plugin = std::move(newPlugin);
		player->setProcessor(plugin.get());
	};
	patchFormat->createPluginInstance(*desc, device->getCurrentSampleRate(), device->getCurrentBufferSizeSamples(), setPlugin);

}


DefaultpluginAudioProcessor::~DefaultpluginAudioProcessor() {}

const juce::String DefaultpluginAudioProcessor::getName() const { return JucePlugin_Name; }

bool DefaultpluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool DefaultpluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool DefaultpluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double DefaultpluginAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int DefaultpluginAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int DefaultpluginAudioProcessor::getCurrentProgram()
{
	return 0;
}

void DefaultpluginAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String DefaultpluginAudioProcessor::getProgramName(int index)
{
	return {};
}

void DefaultpluginAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void DefaultpluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	mainProcessor->setPlayConfigDetails(getMainBusNumInputChannels(),
		getMainBusNumOutputChannels(),
		sampleRate, samplesPerBlock);

	mainProcessor->prepareToPlay(sampleRate, samplesPerBlock);

	initialiseGraph();
}

void DefaultpluginAudioProcessor::initialiseGraph()
{
	mainProcessor->clear();

	////manager->initialiseWithDefaultDevices(2, 2);
	////manager->addAudioCallback(player);
	////AudioIODevice* device = manager->getCurrentAudioDevice();

	//juce::File patchPath("C:\\Users\\callu\\SOUL\\examples\\patches\\ClassicRingtone\\ClassicRingtone.soulpatch");
	//jassert(patchPath.existsAsFile());

	//juce::String dll("C:\\Users\\callu\\SOUL_PatchLoader.dll");
	//auto reinitialiseCallback = [this](soul::patch::SOULPatchAudioProcessor& patch)
	//{
	//	//player->setProcessor(nullptr);
	//	patch.reinitialise();
	//	//player->setProcessor(plugin.get());
	//};
	//patchFormat = new SOULPatchAudioPluginFormat(dll, reinitialiseCallback);

	//juce::PluginDescription* desc = new PluginDescription();
	//desc->pluginFormatName = soul::patch::SOULPatchAudioProcessor::getPluginFormatName();
	//desc->fileOrIdentifier = patchPath.getFullPathName();

	//auto setPlugin = [this](std::unique_ptr<juce::AudioPluginInstance> newPlugin,
	//	const juce::String& error)
	//{
	//	//player->setProcessor(nullptr);
	//	plugin = std::move(newPlugin);
	//	//player->setProcessor(plugin.get());
	//};
	//patchFormat->createPluginInstance(*desc, getSampleRate(), getBlockSize(), setPlugin);

	////soulPatchNode = mainProcessor->addNode(std::move(plugin));
	////Logger::writeToLog(soulPatchNode->getProcessor()->getName());

	osc = mainProcessor->addNode(std::make_unique<OscillatorProcessor>());
	//osc = mainProcessor->addNode(std::move(plugin));

	osc->getProcessor()->setPlayConfigDetails(getMainBusNumInputChannels(),
		getMainBusNumOutputChannels(),
		getSampleRate(), getBlockSize());




	audioInputNode = mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioInputNode));
	audioOutputNode = mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode));

	midiInputNode = mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiInputNode));
	midiOutputNode = mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiOutputNode));

	for (auto node : mainProcessor->getNodes())                 // [10]
		node->getProcessor()->enableAllBuses();

	connectAudioNodes();
	//connectMidiNodes();
}

void DefaultpluginAudioProcessor::connectAudioNodes()
{
	for (int channel = 0; channel < 2; ++channel)
		mainProcessor->addConnection({ { osc->nodeID,  channel },
											{ audioOutputNode->nodeID, channel } });
}

void DefaultpluginAudioProcessor::connectMidiNodes()
{
	mainProcessor->addConnection({ { midiInputNode->nodeID,  juce::AudioProcessorGraph::midiChannelIndex },
									{ midiOutputNode->nodeID, juce::AudioProcessorGraph::midiChannelIndex } });
}

void DefaultpluginAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	 //spare memory, etc.
	mainProcessor->releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DefaultpluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
	//if (layouts.getMainInputChannelSet() == juce::AudioChannelSet::disabled()
	//	|| layouts.getMainOutputChannelSet() == juce::AudioChannelSet::disabled())
	//	return false;

	//if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
	//	&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
	//	return false;

	//return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet();
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void DefaultpluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	Logger::writeToLog(juce::String(plugin.get()->getNumOutputChannels()));
		mainProcessor->processBlock(buffer, midiMessages);
}

//==============================================================================
bool DefaultpluginAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DefaultpluginAudioProcessor::createEditor()
{
	return new DefaultpluginAudioProcessorEditor(*this);
}

//==============================================================================
void DefaultpluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void DefaultpluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,

	// whose contents will have been created by the getStateInformation() call.
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{


	DefaultpluginAudioProcessor* p = new DefaultpluginAudioProcessor();
	return p;

	//OscillatorProcessor* p = new OscillatorProcessor();
	//return p;
}
