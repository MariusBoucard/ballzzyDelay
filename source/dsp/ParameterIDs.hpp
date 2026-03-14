#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace id {
    // Global IDs
    const juce::ParameterID GAIN           {"GAIN", 1};
    const juce::ParameterID BYPASS         {"BYPASS", 1};
    const juce::ParameterID DISTORTION_TYPE{"DISTORTION_TYPE", 1};
    const juce::ParameterID MIX            {"MIX", 1};

    // Helpers for nested IDs
    inline juce::String getHeadPrefix(int index) { return "HEAD_" + juce::String(index) + "_"; }

    // Example sub-parameter suffixes
    const juce::String FREQ = "FREQ";
    const juce::String PAN  = "PAN";
}