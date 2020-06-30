#include "PluginEditor.h"
using namespace juce;

void bindNativeCallbacks() {
	//
	////Bind some native callbacks
	//appRoot.engine.registerNativeMethod(
	//	"beginParameterChangeGesture",
	//	[](void* stash, const juce::var::NativeFunctionArgs& args) {
	//		auto* self = reinterpret_cast<DefaultpluginAudioProcessorEditor*>(stash);
	//		const juce::String& paramId = args.arguments[0].toString();
	//		if (auto* parameter = self->audioProcessor.patchLoader->currentPlugin->getParameters()[0])
	//			parameter->beginChangeGesture();

	//		return juce::var::undefined();
	//	},
	//	(void*)editor
	//		);

	//editor->appRoot.engine.registerNativeMethod(
	//	"setParameterValueNotifyingHost",
	//	[](void* stash, const juce::var::NativeFunctionArgs& args) {
	//		auto* self = reinterpret_cast<DefaultpluginAudioProcessorEditor*>(stash);
	//		const juce::String& paramId = args.arguments[0].toString();
	//		const double value = args.arguments[1];

	//		if (auto* parameter = self->audioProcessor.patchLoader->currentPlugin->getParameters()[0])
	//			parameter->setValueNotifyingHost(value);


	//		return juce::var::undefined();
	//	},
	//	(void*)editor
	//		);

	//editor->appRoot.engine.registerNativeMethod(
	//	"endParameterChangeGesture",
	//	[](void* stash, const juce::var::NativeFunctionArgs& args) {
	//		auto* self = reinterpret_cast<DefaultpluginAudioProcessorEditor*>(stash);
	//		const juce::String& paramId = args.arguments[0].toString();

	//		if (auto* parameter = self->audioProcessor.patchLoader->currentPlugin->getParameters()[0])
	//			parameter->endChangeGesture();

	//		return juce::var::undefined();
	//	},
	//	(void*)editor
	//		);
}
