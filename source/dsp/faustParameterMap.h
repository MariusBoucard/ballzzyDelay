#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>
#include <string>

namespace FaustParameterMapping {
    // Static map linking JUCE ParameterIDs to Faust parameter paths
      inline const std::unordered_map<juce::String, std::string>& getParameterMap() {
        static const std::unordered_map<juce::String, std::string> map = {
            // ============================================================================
            // Global Parameters
            // ============================================================================
            {"MIX", "/FaustEffect/Global_Settings/Global_Mix"},

            // ============================================================================
            // Head 1 Parameters (JUCE HEAD_1 -> Faust Head_0)
            // ============================================================================
            {"HEAD_1_FEEDBACK", "/FaustEffect/Delay_Multi-Tap/Head_0/Feedback"},
            {"HEAD_1_GAIN",     "/FaustEffect/Delay_Multi-Tap/Head_0/Level"},
            {"HEAD_1_PAN",      "/FaustEffect/Delay_Multi-Tap/Head_0/Pan"},
            // Note: Head_0/Time has no JUCE equivalent in your parameter list

            // ============================================================================
            // Head 2 Parameters (JUCE HEAD_2 -> Faust Head_1)
            // ============================================================================
            {"HEAD_2_FEEDBACK", "/FaustEffect/Delay_Multi-Tap/Head_1/Feedback"},
            {"HEAD_2_GAIN",     "/FaustEffect/Delay_Multi-Tap/Head_1/Level"},
            {"HEAD_2_PAN",      "/FaustEffect/Delay_Multi-Tap/Head_1/Pan"},

            // ============================================================================
            // Head 3 Parameters (JUCE HEAD_3 -> Faust Head_2)
            // ============================================================================
            {"HEAD_3_FEEDBACK", "/FaustEffect/Delay_Multi-Tap/Head_2/Feedback"},
            {"HEAD_3_GAIN",     "/FaustEffect/Delay_Multi-Tap/Head_2/Level"},
            {"HEAD_3_PAN",      "/FaustEffect/Delay_Multi-Tap/Head_2/Pan"},

            // ============================================================================
            // Head 4 Parameters (JUCE HEAD_4 -> Faust Head_3)
            // ============================================================================
            {"HEAD_4_FEEDBACK", "/FaustEffect/Delay_Multi-Tap/Head_3/Feedback"},
            {"HEAD_4_GAIN",     "/FaustEffect/Delay_Multi-Tap/Head_3/Level"},
            {"HEAD_4_PAN",      "/FaustEffect/Delay_Multi-Tap/Head_3/Pan"}

            // NOTE: The following JUCE parameters have NO Faust equivalents:
            // - GAIN, BYPASS, DISTORTION_TYPE, INPUT_GAIN, OUTPUT_GAIN, FEEDBACK
            // - SYNC_TEMPO, DUCKING, DUCKING_ATTACK, DUCKING_RELEASE, WIDTH
            // - LP_FILTER_FREQ, HP_FILTER_FREQ
            // - All HEAD_*_BYPASS, HEAD_*_MOVEMENT_*, HEAD_*_*_SLAVE, HEAD_*_LP/HP_FILTER_FREQ
            // These must be handled separately in your JUCE code if needed
        };
        return map;
    }
    
    // Helper function to get Faust path from JUCE parameter ID
    inline std::string getFaustPath(const juce::String& parameterID) {
        const auto& map = getParameterMap();
        auto it = map.find(parameterID);
        if (it != map.end()) {
            return it->second;
        }
        // Return empty string or throw exception if not found
        jassertfalse; // Debug assertion
        return "";
    }
}