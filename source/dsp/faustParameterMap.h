#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <unordered_map>
#include <string>

namespace FaustParameterMapping {
    inline const std::unordered_map<juce::String, std::string>& getParameterMap() {
        static const std::unordered_map<juce::String, std::string> map = {

            // ============================================================================
            // Head 1 (JUCE) -> Head 0 (Faust)
            // ============================================================================
            {"HEAD_1_BYPASS",           "/multiheadFeedback/Head_0_on"},
            {"HEAD_1_FEEDBACK",         "/multiheadFeedback/Head_0_Feedback"},
            {"HEAD_1_GAIN",             "/multiheadFeedback/Head_0_Level"},
            {"HEAD_1_PAN",              "/multiheadFeedback/Head_0_Pan"},
            {"HEAD_1_TIME",             "/multiheadFeedback/Head_0_Time"},
            {"HEAD_1_HP_FILTER_FREQ",   "/multiheadFeedback/Head_0_HPF"},
            {"HEAD_1_LP_FILTER_FREQ",   "/multiheadFeedback/Head_0_LPF"},

            // ============================================================================
            // Head 2 (JUCE) -> Head 1 (Faust)
            // ============================================================================
            {"HEAD_2_BYPASS",           "/multiheadFeedback/Head_1_on"},
            {"HEAD_2_FEEDBACK",         "/multiheadFeedback/Head_2_Feedback"},
            {"HEAD_2_GAIN",             "/multiheadFeedback/Head_1_Level"},
            {"HEAD_2_PAN",              "/multiheadFeedback/Head_1_Pan"},
            {"HEAD_2_TIME",             "/multiheadFeedback/Head_1_Time"},
            {"HEAD_2_HP_FILTER_FREQ",   "/multiheadFeedback/Head_1_HPF"},
            {"HEAD_2_LP_FILTER_FREQ",   "/multiheadFeedback/Head_1_LPF"},

            // ============================================================================
            // Head 3 (JUCE) -> Head 2 (Faust)
            // ============================================================================
            {"HEAD_3_BYPASS",           "/multiheadFeedback/Head_2_on"},
            {"HEAD_3_FEEDBACK",         "/multiheadFeedback/Head_2_Feedback"},
            {"HEAD_3_GAIN",             "/multiheadFeedback/Head_2_Level"},
            {"HEAD_3_PAN",              "/multiheadFeedback/Head_2_Pan"},
            {"HEAD_3_TIME",             "/multiheadFeedback/Head_2_Time"},
            {"HEAD_3_HP_FILTER_FREQ",   "/multiheadFeedback/Head_2_HPF"},
            {"HEAD_3_LP_FILTER_FREQ",   "/multiheadFeedback/Head_2_LPF"},

            // ============================================================================
            // Head 4 (JUCE) -> Head 3 (Faust)
            // ============================================================================
            {"HEAD_4_BYPASS",           "/multiheadFeedback/Head_3_on"},
            {"HEAD_4_FEEDBACK",         "/multiheadFeedback/Head_3_Feedback"},
            {"HEAD_4_GAIN",             "/multiheadFeedback/Head_3_Level"},
            {"HEAD_4_PAN",              "/multiheadFeedback/Head_3_Pan"},
            {"HEAD_4_TIME",             "/multiheadFeedback/Head_3_Time"},
            {"HEAD_4_HP_FILTER_FREQ",   "/multiheadFeedback/Head_3_HPF"},
            {"HEAD_4_LP_FILTER_FREQ",   "/multiheadFeedback/Head_3_LPF"}
        };
        return map;
    }

    inline std::string getFaustPath(const juce::String& parameterID) {
        const auto& map = getParameterMap();
        auto it = map.find(parameterID);
        return (it != map.end()) ? it->second : "";
    }
}