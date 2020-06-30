#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace juce;

DefaultpluginAudioProcessor::DefaultpluginAudioProcessor()
	: AudioProcessor(BusesProperties()
		.withInput("Input", AudioChannelSet::stereo(), true)
		.withOutput("Output", AudioChannelSet::stereo(), true))
{
	//patchLoader = new PatchLoaderComponent();
	//patchMidiCollector = &patchLoader->player.getMidiMessageCollector();
	//patchLoader->player.getCurrentProcessor()->a
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
	//auto patchDLL = lookForSOULPatchDLL();
// Use this method as the place to do any pre-playback
// initialisation that you need..
	//patchLoader->currentPlugin->prepareToPlay(sampleRate, samplesPerBlock);
}

void DefaultpluginAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	 //spare memory, etc.
	//patchLoader->currentPlugin->releaseResources();
}

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

void DefaultpluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	//Logger::writeToLog(String(midiMessages.getNumEvents()));
	//for (auto m : midiMessages)
	//	patchMidiCollector->addMessageToQueue(m.getMessage());
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
	//auto* p = new DefaultpluginAudioProcessor();
	PatchLoaderComponent* patchLoader = new PatchLoaderComponent();
	juce::File patchPath("C:\\Users\\callu\\SOUL\\examples\\patches\\SineSynth\\SineSynth.soulpatch");
	jassert(patchPath.existsAsFile());
	//patchLoader->onPatchLoad = [&]() {onPatchLoad(); };
	patchLoader->load(patchPath);
	//return patchLoader->currentPlugin.get();
	return new DefaultpluginAudioProcessor();

}
