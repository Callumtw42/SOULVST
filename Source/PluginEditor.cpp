#include "PluginEditor.h"

juce::String message;

DefaultpluginAudioProcessorEditor::DefaultpluginAudioProcessorEditor(DefaultpluginAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p)
{
	setSize(400, 300);
}

DefaultpluginAudioProcessorEditor::~DefaultpluginAudioProcessorEditor() { }

void DefaultpluginAudioProcessorEditor::updateParams()
{
	setResizable(true, true);
	addAndMakeVisible(appRoot);
	juce::File UIPath("C:\\Users\\callu\\Desktop\\projects\\defaultplugin\\Source\\jsui\\build\\js\\main.js");
	jassert(UIPath.existsAsFile());
	appRoot.evaluate(UIPath);
	bindNativeCallbacks();

	// Now our React application is up and running, so we can start dispatching events, such as current parameter values.
	Logger::writeToLog("Loaded: " + audioProcessor.plugin->getName());
	for (AudioProcessorParameter* p : audioProcessor.plugin->getParameters())
	{
		params.insert_or_assign(p->getName(100), p);
		juce::Logger::writeToLog(p->getName(100));
		p->addListener(this);
		p->sendValueChangedMessageToListeners(p->getValue());
	}
}

void DefaultpluginAudioProcessorEditor::bindNativeCallbacks()
{
	//Bind some native callbacks
	appRoot.engine.registerNativeMethod(
		"beginParameterChangeGesture",
		[](void* stash, const juce::var::NativeFunctionArgs& args) {
			auto* self = reinterpret_cast<DefaultpluginAudioProcessorEditor*>(stash);
			const juce::String& paramId = args.arguments[0].toString();
			if (auto* parameter = self->params[paramId])
				parameter->beginChangeGesture();
			return juce::var::undefined();
		},
		(void*)this
			);

	appRoot.engine.registerNativeMethod(
		"setParameterValueNotifyingHost",
		[](void* stash, const juce::var::NativeFunctionArgs& args) {
			auto* self = reinterpret_cast<DefaultpluginAudioProcessorEditor*>(stash);
			const juce::String& paramId = args.arguments[0].toString();
			const double value = args.arguments[1];

			if (auto* parameter = self->params[paramId])
				parameter->setValueNotifyingHost(value);
			return juce::var::undefined();
		},
		(void*)this
			);

	appRoot.engine.registerNativeMethod(
		"endParameterChangeGesture",
		[](void* stash, const juce::var::NativeFunctionArgs& args) {
			auto* self = reinterpret_cast<DefaultpluginAudioProcessorEditor*>(stash);
			const juce::String& paramId = args.arguments[0].toString();
			Logger::writeToLog(paramId);
			if (auto* parameter = self->params[paramId])
				parameter->endChangeGesture();
			return juce::var::undefined();
		},
		(void*)this
			);
}

void DefaultpluginAudioProcessorEditor::parameterValueChanged(int parameterIndex, float newValue)
{
	const auto& p = audioProcessor.plugin->getParameters()[parameterIndex];
	juce::String id = p->getName(100);

	if (auto* x = dynamic_cast<AudioProcessorParameterWithID*>(p))
		id = x->paramID;

	float defaultValue = p->getDefaultValue();
	juce::String stringValue = p->getText(newValue, 0);

	Component::SafePointer<blueprint::ReactApplicationRoot> safeAppRoot(&appRoot);

	juce::MessageManager::callAsync([=]() {
		if (blueprint::ReactApplicationRoot* root = safeAppRoot.getComponent())
			root->dispatchEvent("parameterValueChange",
				parameterIndex,
				id,
				defaultValue,
				newValue,
				stringValue);
		});
}

void DefaultpluginAudioProcessorEditor::resized()
{
	appRoot.setBounds(getLocalBounds());
}


