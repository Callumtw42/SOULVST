#pragma once
#include "Processor.h"

static const int WIDTH = 700;
static const int HEIGHT = 500;

class Editor : public juce::AudioProcessorEditor, public juce::AudioProcessorParameter::Listener
{
public:
	Editor(Processor&);
	~Editor() override;
	void resized() override;
	void parameterValueChanged(int parameterIndex, float newValue) override;
	void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}
	void updateParams();
	void bindNativeCallbacks();
	Processor* processor;
	blueprint::ReactApplicationRoot appRoot;
	Label errorText;
	ComponentBoundsConstrainer constrainer;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Editor);
};
