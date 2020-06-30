#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../../SOUL/source/API/soul_patch/API/soul_patch.h"
#include "../../SOUL/source/API/soul_patch/helper_classes/soul_patch_AudioPluginFormat.h"
#include "../../SOUL/source/API/soul_patch/helper_classes/soul_patch_Utilities.h"
#include "../../SOUL/source/API/soul_patch/helper_classes/soul_patch_CompilerCacheFolder.h"
#include "../../SOUL/examples/SOULPatchHostDemo/Source/PatchLoaderComponent.h"

class DefaultpluginAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::AudioProcessorParameter::Listener
{
public:
	DefaultpluginAudioProcessorEditor(DefaultpluginAudioProcessor&);
	~DefaultpluginAudioProcessorEditor() override;
	void resized() override;
	void parameterValueChanged(int parameterIndex, float newValue) override;
	void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}
	void onPatchLoad();
	void bindNativeCallbacks();
	DefaultpluginAudioProcessor& audioProcessor;
	blueprint::ReactApplicationRoot appRoot;
	std::map<juce::String, juce::AudioProcessorParameter*> params;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DefaultpluginAudioProcessorEditor);
	PatchLoaderComponent* patchLoader;
};
