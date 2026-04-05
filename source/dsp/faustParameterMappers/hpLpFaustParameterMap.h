#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>
#include <string>

namespace FaustParameterMapping {
    inline const std::unordered_map<juce::String, std::string>& getHpLpParameterMap() {
        static const std::unordered_map<juce::String, std::string> map = {
            {"HP_FILTER_BYPASS" , "/HpLp/Filters_BypassHp"},
            {"LP_FILTER_BYPASS", "/HpLp/Filters_BypassLp"},
            {"HP_FILTER_FREQ", "/HpLp/Filters_HPF"},
            {"LP_FILTER_FREQ", "/HpLp/Filters_LPF"},
        };
        return map;
    }

    inline std::string getHpLpPath(const juce::String& parameterID) {
        const auto& map = getHpLpParameterMap();
        auto it = map.find(parameterID);
        return (it != map.end()) ? it->second : "";
    }

}