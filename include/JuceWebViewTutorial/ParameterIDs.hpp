#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
// TODO : to remove

namespace webview_plugin::id {
    const juce::ParameterID GAIN{"GAIN", 1};
    const juce::ParameterID BYPASS{"BYPASS", 1};
    const juce::ParameterID DISTORTION_TYPE{"DISTORTION_TYPE", 1};
    const juce::ParameterID MIX{"MIX", 1};
    const juce::ParameterID INPUT_GAIN{"INPUT_GAIN", 1};
    const juce::ParameterID OUTPUT_GAIN{"OUTPUT_GAIN", 1};
    const juce::ParameterID FEEDBACK{"FEEDBACK", 1};
    const juce::ParameterID SYNC_TEMPO{"SYNC_TEMPO", 1};
    const juce::ParameterID DUCKING{"DUCKING", 1};
    const juce::ParameterID DUCKING_ATTACK{"DUCKING_ATTACK", 1};
    const juce::ParameterID DUCKING_RELEASE{"DUCKING_RELEASE", 1};
    const juce::ParameterID WIDTH{"WIDTH", 1};
}  // namespace webview_plugin::id
