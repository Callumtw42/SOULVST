# soul generate --juce-header --output=Source/SineSynth.h Source/soul/SineSynth.soulpatch
# soul generate --juce-cpp --output=Source/SineSynth.cpp Source/soul/SineSynth.soulpatch
soul generate --cpp --output=Source/soulpatch.cpp Source/soul/SineSynth.soulpatch;
# soul generate --graph --output=${workspaceRoot}\\Source\\soul\\graph\\graph.html ${workspaceRoot}\\Source\\soul\\SineSynth.soulpatch