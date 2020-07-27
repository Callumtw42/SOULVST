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
	editor(new DefaultpluginAudioProcessorEditor(*this)),
	desc(new PluginDescription())
{


	//for (int i = 0; i < MAXVOICES; i++)
	//{
	//	soulVoices[i] = new SoulVoice();
	//}

	//manager->initialiseWithDefaultDevices(2, 2);
	//manager->addAudioCallback(player);
	//juce::File patchPath("C:\\Users\\callu\\Desktop\\projects\\defaultplugin\\Source\\soul\\SineSynth.soulpatch");
	//jassert(patchPath.existsAsFile());

	//auto setPlugin = [this](std::unique_ptr<juce::AudioPluginInstance> newPlugin,
	//	const juce::String& error)
	//{
	//	int index = voicesSet % MAXVOICES;
	//	player->setProcessor(nullptr);
	//	pluginInstances[index] = std::move(newPlugin);
	//	player->setProcessor(pluginInstances[index].get());
	//	voicesSet++;
	//};

	//auto reinitialiseCallback = [this](soul::patch::SOULPatchAudioProcessor& patch)
	//{
	//	int index = voicesInitialised % MAXVOICES;
	//	isPlayable = false;
	//	player->setProcessor(nullptr);
	//	patch.reinitialise();
	//	juce::String error = patch.getCompileError();
	//	if (error.isEmpty()) {
	//		soulVoices[index]->processor = &patch;
	//		soulVoices[index]->mainParams = &params;
	//		soulVoices[index]->processor->prepareToPlay(getSampleRate(), getBlockSize());
	//		voicesInitialised++;
	//		if (index == MAXVOICES - 1)
	//		{
	//			static_cast<DefaultpluginAudioProcessorEditor*>(editor)->updateParams(&error, index);
	//			Logger::writeToLog(juce::String(getParameters().size()));
	//			isPlayable = true;
	//		}
	//	}
	//};

	//juce::String dll("C:\\Users\\callu\\SOUL_PatchLoader.dll");
	//patchFormat = new SOULPatchAudioPluginFormat(dll, reinitialiseCallback);
	//desc->pluginFormatName = soul::patch::SOULPatchAudioProcessor::getPluginFormatName();
	//desc->fileOrIdentifier = patchPath.getFullPathName();

	//for (int i = 0; i < MAXVOICES; i++) {
	//	patchFormat->createPluginInstance(*desc, getSampleRate(), getBlockSize(), setPlugin);
	//}
	pluginInstances[0] = std::make_unique<SineSynth>();
}

void DefaultpluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	//for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
	//	buffer.clear(i, 0, buffer.getNumSamples());

	pluginInstances[0]->processBlock(buffer, midiMessages);

	//if (isPlayable) {
	//	//setPlayHead();
	//	if (auto* p = getPlayHead()) {
	//		p->getCurrentPosition(playHead);
	//		//dbgLog(playHead.bpm);
	//	}
	//	soulVoices[MAXVOICES - 1]->processBlock(buffer, midiMessages);
	//}
}

void DefaultpluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{

	pluginInstances[0]->prepareToPlay(sampleRate, samplesPerBlock);

	//std::vector<juce::String> paramNames =
	//{
	//	"cutoff",
	//	"voiceCount",
	//	"detune",
	//	"ampRelease",
	//	"ampSustain",
	//	"ampDecay",
	//	"ampAttack",
	//	"volume",
	//};
	//for (juce::String name : paramNames)
	//{
	//	params.set(name, new Param());
	//	addParameter(params.getReference(name));

	//}
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
