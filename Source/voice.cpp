/*
  ==============================================================================

	voice.cpp
	Created: 13 Jul 2020 8:51:45am
	Author:  callu

  ==============================================================================
*/

#include "voice.h"

Voice::Voice()
	: AudioProcessor(BusesProperties()
		.withInput("Input", AudioChannelSet::stereo(), true)
		.withOutput("Output", AudioChannelSet::stereo(), true)),
	player(new AudioProcessorPlayer()),
	manager(new AudioDeviceManager()),
	isPlayable(false),
	desc(new PluginDescription)

{
	manager->initialiseWithDefaultDevices(2, 2);
	manager->addAudioCallback(player);
	juce::File patchPath("C:\\Users\\callu\\Desktop\\projects\\defaultplugin\\Source\\soul\\SineSynth.soulpatch");
	jassert(patchPath.existsAsFile());

	desc->pluginFormatName = soul::patch::SOULPatchAudioProcessor::getPluginFormatName();
	desc->fileOrIdentifier = patchPath.getFullPathName();

	auto setPlugin = [this](std::unique_ptr<juce::AudioPluginInstance> newPlugin,
		const juce::String& error)
	{
		player->setProcessor(nullptr);
		plugin = std::move(newPlugin); //new graph with array of newPlugin->processor's all connected up
		player->setProcessor(plugin.get());
	};

	auto reinitialiseCallback = [&setPlugin, this](soul::patch::SOULPatchAudioProcessor& patch)
	{
		isPlayable = false;
		player->setProcessor(nullptr);
		patch.reinitialise();
		juce::String error = patch.getCompileError();
		if (error.isEmpty()) {
			soulProcessor = std::unique_ptr<SOULPatchAudioProcessor>(&patch);
			plugin->prepareToPlay(getSampleRate(), getBlockSize());
			lfo = new LFO(plugin.get(), plugin.get()->getParameters()[0]);
			isPlayable = true;
		}
		//static_cast<Voice*>(editor)->updateParams(&error);
	};
	juce::String dll("C:\\Users\\callu\\SOUL_PatchLoader.dll");
	patchFormat = new SOULPatchAudioPluginFormat(dll, reinitialiseCallback);

	patchFormat->createPluginInstance(*desc, getSampleRate(), getBlockSize(), setPlugin);
}

void Voice::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	//if (isPlayable)lfo->process();

	if (isPlayable) { plugin->processBlock(buffer, midiMessages); }
}

void Voice::initialiseGraph()
{
	//graph.addNode(static_cast<std::unique_ptr<AudioProcessor>>(soulProcessor));
}

juce::AudioProcessorEditor* Voice::createEditor() { return nullptr; }


//#################################################################################################################################

bool Voice::hasEditor() const { return false; }
void Voice::getStateInformation(juce::MemoryBlock& destData) { }
void Voice::setStateInformation(const void* data, int sizeInBytes) { }
Voice::~Voice() {}
const juce::String Voice::getName() const { return JucePlugin_Name; }
bool Voice::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}
bool Voice::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}
bool Voice::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}
double Voice::getTailLengthSeconds() const { return 0.0; }
int Voice::getNumPrograms() { return 1; }
int Voice::getCurrentProgram() { return 0; }
void Voice::setCurrentProgram(int index) { }
const juce::String Voice::getProgramName(int index) { return {}; }
void Voice::changeProgramName(int index, const juce::String& newName) { }
void Voice::prepareToPlay(double sampleRate, int samplesPerBlock) {}
void Voice::releaseResources() {}
#ifndef JucePlugin_PreferredChannelConfigurations
bool Voice::isBusesLayoutSupported(const BusesLayout& layouts) const
{
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


