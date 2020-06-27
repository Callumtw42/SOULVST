/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "../../SOUL/source/API/soul_patch/API/soul_patch.h"
#include "../../SOUL/source/API/soul_patch/helper_classes/soul_patch_AudioPluginFormat.h"
#include "../../SOUL/source/API/soul_patch/helper_classes/soul_patch_Utilities.h"
#include "../../SOUL/source/API/soul_patch/helper_classes/soul_patch_CompilerCacheFolder.h"


//==============================================================================
/**
*/
class DefaultpluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
	DefaultpluginAudioProcessorEditor(DefaultpluginAudioProcessor&);
	~DefaultpluginAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	DefaultpluginAudioProcessor& audioProcessor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DefaultpluginAudioProcessorEditor)
};
