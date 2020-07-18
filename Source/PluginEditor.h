#pragma once

#include "PluginProcessor.h"

static const int WIDTH = 700;
static const int HEIGHT = 500;

class DefaultpluginAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::AudioProcessorParameter::Listener
{
public:
	DefaultpluginAudioProcessorEditor(DefaultpluginAudioProcessor&);
	~DefaultpluginAudioProcessorEditor() override;
	void resized() override;
	void parameterValueChanged(int parameterIndex, float newValue) override;
	void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}
	void updateParams(juce::String* error, int index);
	void bindNativeCallbacks();
	DefaultpluginAudioProcessor& processor;
	blueprint::ReactApplicationRoot appRoot;
	Label errorText;
	ComponentBoundsConstrainer constrainer;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DefaultpluginAudioProcessorEditor);
};
