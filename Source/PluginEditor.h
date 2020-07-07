#pragma once

#include "PluginProcessor.h"

class DefaultpluginAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::AudioProcessorParameter::Listener
{
public:
	DefaultpluginAudioProcessorEditor(DefaultpluginAudioProcessor&);
	~DefaultpluginAudioProcessorEditor() override;
	void resized() override;
	void parameterValueChanged(int parameterIndex, float newValue) override;
	void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}
	void updateParams();
	void bindNativeCallbacks();
	DefaultpluginAudioProcessor& audioProcessor;
	blueprint::ReactApplicationRoot appRoot;
	std::map<juce::String, juce::AudioProcessorParameter*> params;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DefaultpluginAudioProcessorEditor);
};
