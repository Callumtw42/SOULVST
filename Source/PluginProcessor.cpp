#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "SineSynth.cpp"


DefaultpluginAudioProcessor::DefaultpluginAudioProcessor()
	: AudioProcessor(BusesProperties()
		.withInput("Input", AudioChannelSet::stereo(), true)
		.withOutput("Output", AudioChannelSet::stereo(), true)),
	player(new AudioProcessorPlayer()),
	manager(new AudioDeviceManager()),
	isPlayable(false),
	desc(new PluginDescription()),
	pluginInstance(std::make_unique<_SineSynth>()),
	editor(new DefaultpluginAudioProcessorEditor(*this))
{
}

void DefaultpluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	AudioPlayHead* p = getPlayHead();
	if (p != nullptr) p->getCurrentPosition(playHead);
	pluginInstance->processBlock(buffer, midiMessages);
}

void DefaultpluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	pluginInstance->prepareToPlay(sampleRate, samplesPerBlock);
}

juce::AudioProcessorEditor* DefaultpluginAudioProcessor::createEditor() { return editor; }

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	DefaultpluginAudioProcessor* p = new DefaultpluginAudioProcessor();
	return p;
}

//#################################################################################################################################

bool DefaultpluginAudioProcessor::hasEditor() const { return true; }
void DefaultpluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData) { }
void DefaultpluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes) { }
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
double DefaultpluginAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int DefaultpluginAudioProcessor::getNumPrograms() { return 1; }
int DefaultpluginAudioProcessor::getCurrentProgram() { return 0; }
void DefaultpluginAudioProcessor::setCurrentProgram(int index) { }
const juce::String DefaultpluginAudioProcessor::getProgramName(int index) { return {}; }
void DefaultpluginAudioProcessor::changeProgramName(int index, const juce::String& newName) { }
void DefaultpluginAudioProcessor::releaseResources() {}
#ifndef JucePlugin_PreferredChannelConfigurations
bool DefaultpluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
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
