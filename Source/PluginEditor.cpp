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
	appRoot.evaluate(UIPath);
}

DefaultpluginAudioProcessorEditor::~DefaultpluginAudioProcessorEditor() { }

void DefaultpluginAudioProcessorEditor::updateParams(juce::String* error, int index)
{
	removeChildComponent(getIndexOfChildComponent(&appRoot));
	removeChildComponent(getIndexOfChildComponent(&errorText));
	// Now our React application is up and running, so we can start dispatching events, such as current parameter values.
	Logger::writeToLog("Loaded: " + processor.pluginInstances[index]->getName());

	//if (processor.params) processor.params->clear();
	if (error->isEmpty()) {
		for (AudioProcessorParameter* p : processor.pluginInstances[index]->getParameters())
		{
			processor.params.set(p->getName(100), new Param(p, &processor.playHead));
			Param* mainParam = processor.params.getReference(p->getName(100));
			processor.addParameter(mainParam);

			//processor.LFOPlots.insert_or_assign(p->getName(100), defaultPlot);
			//juce::Logger::writeToLog(p->getName(100));
			mainParam->addListener(this);
			//mainParam->sendValueChangedMessageToListeners(p->getValue());
		}
		bindNativeCallbacks();
		addAndMakeVisible(appRoot);
	}
	else {
		errorText.setText(*error, NotificationType());
		addAndMakeVisible(errorText);
		errorText.setBounds(getLocalBounds());
		errorText.centreWithSize(getParentHeight(), getParentWidth());
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
	if (processor.isPlayable) {
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
}

void DefaultpluginAudioProcessorEditor::resized()
{
	appRoot.setBounds(getLocalBounds());
	errorText.setBounds(getLocalBounds());
	errorText.centreWithSize(getParentHeight(), getParentWidth());
}


