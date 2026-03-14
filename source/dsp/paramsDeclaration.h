#pragma once
#include <juce_audio_processors_headless/juce_audio_processors_headless.h>

namespace parametersDeclaration{
    struct Parameters {
        juce::AudioParameterFloat* gain{nullptr};
        juce::AudioParameterBool* bypass{nullptr};
        juce::AudioParameterChoice* distortionType{nullptr};
    };
}
