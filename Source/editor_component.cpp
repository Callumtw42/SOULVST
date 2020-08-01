/*
  ==============================================================================

	editor_component.cpp
	Created: 31 Jul 2020 11:47:26pm
	Author:  callu

  ==============================================================================
*/

//#include "soulpatch.cpp"
//#include "SineSynth.h"


struct _SineSynth::EditorComponent : public juce::AudioProcessorEditor
{
	EditorComponent(_SineSynth& p)
		: juce::AudioProcessorEditor(p), owner(p), editor(p),
		midiKeyboard(p.midiKeyboardState, juce::MidiKeyboardComponent::Orientation::horizontalKeyboard)
	{
		setLookAndFeel(&lookAndFeel);

		if constexpr (MainProcessor::GeneratedClass::numParameters != 0)
			addAndMakeVisible(editor);

		if (MainProcessor::GeneratedClass::hasMIDIInput)
			addAndMakeVisible(midiKeyboard);

		auto size = owner.getStoredEditorSize({ 600, 400 });
		setResizeLimits(400, 150, 2000, 2000);
		setSize(size.width, size.height);
	}

	~EditorComponent() override
	{
		owner.editorBeingDeleted(this);
		setLookAndFeel(nullptr);
	}

	void paint(juce::Graphics& g) override
	{
		auto background = lookAndFeel.findColour(juce::ResizableWindow::backgroundColourId);
		g.fillAll(background);

		if (getNumChildComponents() == 0)
		{
			g.setColour(background.contrasting());
			g.setFont(16.0f);
			g.drawFittedText(owner.getName(), getLocalBounds().reduced(6), juce::Justification::centred, 2);
		}
	}

	void resized() override
	{
		auto r = getLocalBounds().reduced(6);

		if (midiKeyboard.isVisible())
			midiKeyboard.setBounds(r.removeFromBottom(std::min(80, r.getHeight() / 4)));

		editor.setBounds(r);
		owner.storeEditorSize({ getWidth(), getHeight() });
	}

	_SineSynth& owner;
	juce::LookAndFeel_V4 lookAndFeel;
	juce::GenericAudioProcessorEditor editor;
	juce::MidiKeyboardComponent midiKeyboard;
};
