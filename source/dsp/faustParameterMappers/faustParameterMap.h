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
            {"HEAD_1_ON",                "/multiheadFeedback/Head_0/On"},
            {"HEAD_1_FEEDBACK",          "/multiheadFeedback/Head_0/Feedback"},
            {"HEAD_1_GAIN",              "/multiheadFeedback/Head_0/Level"},
            {"HEAD_1_PAN",               "/multiheadFeedback/Head_0/Pan"},
            {"HEAD_1_TIME_NO_SYNC",              "/multiheadFeedback/Head_0/Time"},
            {"HEAD_1_TIME",              "/multiheadFeedback/Head_0/Time"},
            {"HEAD_1_HP_FILTER_FREQ",    "/multiheadFeedback/Head_0/Filters_HPF"},
            {"HEAD_1_HP_FILTER_BYPASS",  "/multiheadFeedback/Head_0/Filters_HP_Bp"},
            {"HEAD_1_LP_FILTER_FREQ",    "/multiheadFeedback/Head_0/Filters_LPF"},
            {"HEAD_1_LP_FILTER_BYPASS",  "/multiheadFeedback/Head_0/Filters_LP_Bp"},

            // ============================================================================
            // Head 2 (JUCE) -> Head 1 (Faust)
            // ============================================================================
            {"HEAD_2_ON",                "/multiheadFeedback/Head_1/On"},
            {"HEAD_2_FEEDBACK",          "/multiheadFeedback/Head_1/Feedback"},
            {"HEAD_2_GAIN",              "/multiheadFeedback/Head_1/Level"},
            {"HEAD_2_PAN",               "/multiheadFeedback/Head_1/Pan"},
            {"HEAD_2_TIME_NO_SYNC",              "/multiheadFeedback/Head_1/Time"},
            {"HEAD_2_TIME",              "/multiheadFeedback/Head_1/Time"},
            {"HEAD_2_HP_FILTER_FREQ",    "/multiheadFeedback/Head_1/Filters_HPF"},
            {"HEAD_2_HP_FILTER_BYPASS",  "/multiheadFeedback/Head_1/Filters_HP_Bp"},
            {"HEAD_2_LP_FILTER_FREQ",    "/multiheadFeedback/Head_1/Filters_LPF"},
            {"HEAD_2_LP_FILTER_BYPASS",  "/multiheadFeedback/Head_1/Filters_LP_Bp"},

            // ============================================================================
            // Head 3 (JUCE) -> Head 2 (Faust)
            // ============================================================================
            {"HEAD_3_ON",                "/multiheadFeedback/Head_2/On"},
            {"HEAD_3_FEEDBACK",          "/multiheadFeedback/Head_2/Feedback"},
            {"HEAD_3_GAIN",              "/multiheadFeedback/Head_2/Level"},
            {"HEAD_3_PAN",               "/multiheadFeedback/Head_2/Pan"},
            {"HEAD_3_TIME_NO_SYNC",              "/multiheadFeedback/Head_2/Time"},
            {"HEAD_3_TIME",              "/multiheadFeedback/Head_2/Time"},

            {"HEAD_3_HP_FILTER_FREQ",    "/multiheadFeedback/Head_2/Filters_HPF"},
            {"HEAD_3_HP_FILTER_BYPASS",  "/multiheadFeedback/Head_2/Filters_HP_Bp"},
            {"HEAD_3_LP_FILTER_FREQ",    "/multiheadFeedback/Head_2/Filters_LPF"},
            {"HEAD_3_LP_FILTER_BYPASS",  "/multiheadFeedback/Head_2/Filters_LP_Bp"},

            // ============================================================================
            // Head 4 (JUCE) -> Head 3 (Faust)
            // ============================================================================
            {"HEAD_4_ON",                "/multiheadFeedback/Head_3/On"},
            {"HEAD_4_FEEDBACK",          "/multiheadFeedback/Head_3/Feedback"},
            {"HEAD_4_GAIN",              "/multiheadFeedback/Head_3/Level"},
            {"HEAD_4_PAN",               "/multiheadFeedback/Head_3/Pan"},
            {"HEAD_4_TIME_NO_SYNC",      "/multiheadFeedback/Head_3/Time"},
            {"HEAD_4_TIME",              "/multiheadFeedback/Head_3/Time"},
            {"HEAD_4_HP_FILTER_FREQ",    "/multiheadFeedback/Head_3/Filters_HPF"},
            {"HEAD_4_HP_FILTER_BYPASS",  "/multiheadFeedback/Head_3/Filters_HP_Bp"},
            {"HEAD_4_LP_FILTER_FREQ",    "/multiheadFeedback/Head_3/Filters_LPF"},
            {"HEAD_4_LP_FILTER_BYPASS",  "/multiheadFeedback/Head_3/Filters_LP_Bp"}
        };
        return map;
    }

    inline std::string getFaustPath(const juce::String& parameterID) {
        const auto& map = getParameterMap();
        auto it = map.find(parameterID);
        return (it != map.end()) ? it->second : "";
    }

}