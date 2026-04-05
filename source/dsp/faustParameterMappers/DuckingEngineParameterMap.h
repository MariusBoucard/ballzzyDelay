#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>
#include <string>

namespace FaustParameterMapping {
    inline const std::unordered_map<juce::String, std::string>& getDuckingEngineParameterMap() {
        // TODO
        static const std::unordered_map<juce::String, std::string> map = {
            {"DUCKING_ATTACK" , "/duckingEngine/Attack"},
            {"DUCKING_ATTACK", "/duckingEngine/Gain_Reduction"},
            {"DUCKING_RATIO", "/duckingEngine/Ratio"},
            {"DUCKING_RELEASE", "/duckingEngine/Release"},
            {"DUCKING_THRESHOLD", "/duckingEngine/Threshold"},
        };
        return map;
    }

    inline std::string getDuckingEnginePath(const juce::String& parameterID) {
        const auto& map = getDuckingEngineParameterMap();
        auto it = map.find(parameterID);
        return (it != map.end()) ? it->second : "";
    }

}