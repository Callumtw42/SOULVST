#include "PluginEditor.h"

juce::String message;

DefaultpluginAudioProcessorEditor::DefaultpluginAudioProcessorEditor(DefaultpluginAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p)
{
	constrainer.setMinimumWidth(WIDTH);
	constrainer.setMinimumHeight(HEIGHT);
	setConstrainer(&constrainer);
	setResizable(true, true);
	setSize(WIDTH, HEIGHT);

	juce::File UIPath("C:\\Users\\callu\\Desktop\\projects\\defaultplugin\\Source\\jsui\\build\\js\\main.js");
	jassert(UIPath.existsAsFile());
	appRoot.enableHotReloading();
	updateParams();
	appRoot.evaluate(UIPath);
	bindNativeCallbacks();
	addAndMakeVisible(appRoot);
}

DefaultpluginAudioProcessorEditor::~DefaultpluginAudioProcessorEditor() { }

void DefaultpluginAudioProcessorEditor::updateParams()
{
	Logger::writeToLog("Loaded: " + processor.pluginInstance->getName());
	for (AudioProcessorParameter* p : processor.pluginInstance->getParameters())
	{
		juce::String name = p->getName(100);
		processor.params.set(name, new Param());
		Param* mainParam = processor.params.getReference(name);
		mainParam->initialise(p, &processor.playHead);
		mainParam->addListener(this);
		processor.addParameter(mainParam);
		mainParam->sendValueChangedMessageToListeners(mainParam->getValue());
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
			if (Param* parameter = self->processor.params.getReference(paramId))
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

			if (Param* parameter = self->processor.params.getReference(paramId)) {
				parameter->setValueNotifyingHost(value);
				//parameter->sendValueChangedMessageToListeners(parameter->getValue());
			}
			return juce::var::undefined();
		},
		(void*)this
			);

	appRoot.engine.registerNativeMethod(
		"endParameterChangeGesture",
		[](void* stash, const juce::var::NativeFunctionArgs& args) {
			auto* self = reinterpret_cast<DefaultpluginAudioProcessorEditor*>(stash);
			const juce::String& paramId = args.arguments[0].toString();
			//Logger::writeToLog(paramId);
			if (Param* parameter = self->processor.params.getReference(paramId))
				parameter->endChangeGesture();
			return juce::var::undefined();
		},
		(void*)this
			);

	appRoot.engine.registerNativeMethod(
		"sendPlot",
		[](void* stash, const juce::var::NativeFunctionArgs& args) {
			auto* self = reinterpret_cast<DefaultpluginAudioProcessorEditor*>(stash);
			const juce::String& paramId = args.arguments[0].toString();
			Param* param = self->processor.params.getReference(paramId);
			for (int i = 0; i < LFORES; i++) {
				double inVal = static_cast<double>(args.arguments[1][i]);
				param->lfo->plot[i] = inVal;
			}
			return juce::var::undefined();
		},
		(void*)this
			);

	appRoot.engine.registerNativeMethod(
		"sendLFOSpeed",
		[](void* stash, const juce::var::NativeFunctionArgs& args) {
			auto* self = reinterpret_cast<DefaultpluginAudioProcessorEditor*>(stash);
			const juce::String& paramId = args.arguments[0].toString();
			Param* param = self->processor.params.getReference(paramId);

			param->lfo->speed = args.arguments[1];
			return juce::var::undefined();
		},
		(void*)this
			);

	appRoot.engine.registerNativeMethod(
		"log",
		[](void* stash, const juce::var::NativeFunctionArgs& args) {
			Logger::writeToLog(args.arguments[0].toString());
			return juce::var::undefined();
		},
		(void*)this
			);

	appRoot.engine.registerNativeMethod(
		"getMouseX",
		[](void* stash, const juce::var::NativeFunctionArgs& args) {
			auto* self = reinterpret_cast<DefaultpluginAudioProcessorEditor*>(stash);
			return var(self->getMouseXYRelative().getX());
		},
		(void*)this
			);

	appRoot.engine.registerNativeMethod(
		"getMouseY",
		[](void* stash, const juce::var::NativeFunctionArgs& args) {
			auto* self = reinterpret_cast<DefaultpluginAudioProcessorEditor*>(stash);
			return var(self->getMouseXYRelative().getY());
		},
		(void*)this
			);

}

void DefaultpluginAudioProcessorEditor::parameterValueChanged(int parameterIndex, float newValue)
{
	//const auto& p = processor.pluginInstances[MAXVOICES - 1]->getParameters()[parameterIndex];
	const auto& p = processor.getParameters()[parameterIndex];
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
	errorText.setBounds(getLocalBounds());
	errorText.centreWithSize(getParentHeight(), getParentWidth());
}


