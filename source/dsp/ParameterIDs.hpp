#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace id {
    // ============================================================================
    // Global Parameters
    // ============================================================================
    const juce::ParameterID GAIN                  {"GAIN", 1};
    const juce::ParameterID BYPASS                {"BYPASS", 1};
    const juce::ParameterID INPUT_GAIN            {"INPUT_GAIN", 1};
    const juce::ParameterID OUTPUT_GAIN           {"OUTPUT_GAIN", 1};
    const juce::ParameterID MIX                   {"MIX", 1};
    const juce::ParameterID FEEDBACK              {"FEEDBACK", 1};
    const juce::ParameterID TIME                  {"TIME", 1};
    const juce::ParameterID TIME_NO_SYNC          {"TIME_NO_SYNC", 1};
    const juce::ParameterID SYNC_TEMPO            {"SYNC_TEMPO", 1};

    const juce::ParameterID DUCKING               {"DUCKING", 1};
    const juce::ParameterID DUCKING_ATTACK        {"DUCKING_ATTACK", 1};
    const juce::ParameterID DUCKING_RELEASE       {"DUCKING_RELEASE", 1};
    const juce::ParameterID DUCKING_RATIO         {"DUCKING_RATIO", 1};
    const juce::ParameterID DUCKING_THRESHOLD     {"DUCKING_THRESHOLD", 1};

    const juce::ParameterID WIDTH                 {"WIDTH", 1};
    const juce::ParameterID LP_FILTER_FREQ        {"LP_FILTER_FREQ", 1};
    const juce::ParameterID HP_FILTER_FREQ        {"HP_FILTER_FREQ", 1};
    // TODO pourquoi pas mis ?
  //  const juce::ParameterID LP_FILTER_BYPASS      {"LP_FILTER_BYPASS", 1};
   // const juce::ParameterID HP_FILTER_BYPASS      {"HP_FILTER_BYPASS", 1};

    // ============================================================================
    // Helper function for nested Head IDs
    // ============================================================================
    inline juce::String getHeadPrefix(int index) { return "HEAD_" + juce::String(index) + "_"; }

    struct ParameterIDs {
        static const std::array<juce::ParameterID, 10>& getIDs() {
            static const std::array<juce::ParameterID, 10> ids = {
                GAIN, BYPASS, INPUT_GAIN, OUTPUT_GAIN,
                MIX,TIME, FEEDBACK, SYNC_TEMPO, DUCKING, DUCKING_ATTACK
            };
            return ids;
        }
    };



    // ============================================================================
    // Head 1 Parameters
    // ============================================================================
    const juce::ParameterID HEAD_1_ON                    {"HEAD_1_ON", 1};
    const juce::ParameterID HEAD_1_FEEDBACK                  {"HEAD_1_FEEDBACK", 1};
    const juce::ParameterID HEAD_1_PAN                       {"HEAD_1_PAN", 1};
    const juce::ParameterID HEAD_1_TIME                      {"HEAD_1_TIME", 1};
    const juce::ParameterID HEAD_1_TIME_NO_SYNC               {"HEAD_1_TIME_NO_SYNC", 1};
    const juce::ParameterID HEAD_1_GAIN                      {"HEAD_1_GAIN", 1};
    const juce::ParameterID HEAD_1_MOVEMENT_PERIOD_DURATION  {"HEAD_1_MOVEMENT_PERIOD_DURATION", 1};
    const juce::ParameterID HEAD_1_MOVEMENT_PERIOD_DURATION_NO_SYNC  {"HEAD_1_MOVEMENT_PERIOD_DURATION_NO_SYNC", 1};
    const juce::ParameterID HEAD_1_MOVEMENT_FUNCTION         {"HEAD_1_MOVEMENT_FUNCTION", 1};
    const juce::ParameterID HEAD_1_MOVEMENT_WIDTH            {"HEAD_1_MOVEMENT_WIDTH", 1};
    const juce::ParameterID HEAD_1_MOVEMENT_ON               {"HEAD_1_MOVEMENT_ON", 1};
    const juce::ParameterID HEAD_1_MOVEMENT_WIDTH_SLAVE      {"HEAD_1_MOVEMENT_WIDTH_SLAVE", 1};
    const juce::ParameterID HEAD_1_LP_FILTER_FREQ            {"HEAD_1_LP_FILTER_FREQ", 1};
    const juce::ParameterID HEAD_1_HP_FILTER_FREQ            {"HEAD_1_HP_FILTER_FREQ", 1};
    const juce::ParameterID HEAD_1_HP_FILTER_BYPASS          {"HEAD_1_HP_FILTER_BYPASS", 1};
    const juce::ParameterID HEAD_1_LP_FILTER_BYPASS          {"HEAD_1_LP_FILTER_BYPASS", 1};
    const juce::ParameterID HEAD_1_FEEDBACK_SLAVE            {"HEAD_1_FEEDBACK_SLAVE", 1};
    const juce::ParameterID HEAD_1_GAIN_SLAVE                {"HEAD_1_GAIN_SLAVE", 1};
    
    // ============================================================================
    // Head 2 Parameters
    // ============================================================================
    const juce::ParameterID HEAD_2_ON                    {"HEAD_2_ON", 1};
    const juce::ParameterID HEAD_2_FEEDBACK                  {"HEAD_2_FEEDBACK", 1};
    const juce::ParameterID HEAD_2_PAN                       {"HEAD_2_PAN", 1};
    const juce::ParameterID HEAD_2_TIME                      {"HEAD_2_TIME", 1};
    const juce::ParameterID HEAD_2_TIME_NO_SYNC                      {"HEAD_2_TIME_NO_SYNC", 1};
    const juce::ParameterID HEAD_2_GAIN                      {"HEAD_2_GAIN", 1};
    const juce::ParameterID HEAD_2_MOVEMENT_PERIOD_DURATION  {"HEAD_2_MOVEMENT_PERIOD_DURATION", 1};
    const juce::ParameterID HEAD_2_MOVEMENT_PERIOD_DURATION_NO_SYNC  {"HEAD_2_MOVEMENT_PERIOD_DURATION_NO_SYNC", 1};
    const juce::ParameterID HEAD_2_MOVEMENT_FUNCTION         {"HEAD_2_MOVEMENT_FUNCTION", 1};
    const juce::ParameterID HEAD_2_MOVEMENT_WIDTH            {"HEAD_2_MOVEMENT_WIDTH", 1};
    const juce::ParameterID HEAD_2_MOVEMENT_ON               {"HEAD_2_MOVEMENT_ON", 1};
    const juce::ParameterID HEAD_2_MOVEMENT_WIDTH_SLAVE      {"HEAD_2_MOVEMENT_WIDTH_SLAVE", 1};
    const juce::ParameterID HEAD_2_LP_FILTER_FREQ            {"HEAD_2_LP_FILTER_FREQ", 1};
    const juce::ParameterID HEAD_2_HP_FILTER_FREQ            {"HEAD_2_HP_FILTER_FREQ", 1};
    const juce::ParameterID HEAD_2_FEEDBACK_SLAVE            {"HEAD_2_FEEDBACK_SLAVE", 1};
    const juce::ParameterID HEAD_2_GAIN_SLAVE                {"HEAD_2_GAIN_SLAVE", 1};
    const juce::ParameterID HEAD_2_LP_FILTER_BYPASS          {"HEAD_2_LP_FILTER_BYPASS", 1};
    const juce::ParameterID HEAD_2_HP_FILTER_BYPASS          {"HEAD_2_HP_FILTER_BYPASS", 1};

    
    // ============================================================================
    // Head 3 Parameters
    // ============================================================================
    const juce::ParameterID HEAD_3_ON                    {"HEAD_3_ON", 1};
    const juce::ParameterID HEAD_3_FEEDBACK                  {"HEAD_3_FEEDBACK", 1};
    const juce::ParameterID HEAD_3_PAN                       {"HEAD_3_PAN", 1};
    const juce::ParameterID HEAD_3_TIME                      {"HEAD_3_TIME", 1};
    const juce::ParameterID HEAD_3_TIME_NO_SYNC                      {"HEAD_3_TIME_NO_SYNC", 1};
    const juce::ParameterID HEAD_3_GAIN                      {"HEAD_3_GAIN", 1};
    const juce::ParameterID HEAD_3_MOVEMENT_PERIOD_DURATION  {"HEAD_3_MOVEMENT_PERIOD_DURATION", 1};
    const juce::ParameterID HEAD_3_MOVEMENT_PERIOD_DURATION_NO_SYNC  {"HEAD_3_MOVEMENT_PERIOD_DURATION_NO_SYNC", 1};
    const juce::ParameterID HEAD_3_MOVEMENT_FUNCTION         {"HEAD_3_MOVEMENT_FUNCTION", 1};
    const juce::ParameterID HEAD_3_MOVEMENT_WIDTH            {"HEAD_3_MOVEMENT_WIDTH", 1};
    const juce::ParameterID HEAD_3_MOVEMENT_ON               {"HEAD_3_MOVEMENT_ON", 1};
    const juce::ParameterID HEAD_3_MOVEMENT_WIDTH_SLAVE      {"HEAD_3_MOVEMENT_WIDTH_SLAVE", 1};
    const juce::ParameterID HEAD_3_LP_FILTER_FREQ            {"HEAD_3_LP_FILTER_FREQ", 1};
    const juce::ParameterID HEAD_3_HP_FILTER_FREQ            {"HEAD_3_HP_FILTER_FREQ", 1};
    const juce::ParameterID HEAD_3_FEEDBACK_SLAVE            {"HEAD_3_FEEDBACK_SLAVE", 1};
    const juce::ParameterID HEAD_3_GAIN_SLAVE                {"HEAD_3_GAIN_SLAVE", 1};
    const juce::ParameterID HEAD_3_LP_FILTER_BYPASS          {"HEAD_3_LP_FILTER_BYPASS", 1};
    const juce::ParameterID HEAD_3_HP_FILTER_BYPASS          {"HEAD_3_HP_FILTER_BYPASS", 1};

    // ============================================================================
    // Head 4 Parameters
    // ============================================================================
    const juce::ParameterID HEAD_4_ON                    {"HEAD_4_ON", 1};
    const juce::ParameterID HEAD_4_FEEDBACK                  {"HEAD_4_FEEDBACK", 1};
    const juce::ParameterID HEAD_4_PAN                       {"HEAD_4_PAN", 1};
    const juce::ParameterID HEAD_4_TIME                      {"HEAD_4_TIME", 1};
    const juce::ParameterID HEAD_4_TIME_NO_SYNC                      {"HEAD_4_TIME_NO_SYNC", 1};
    const juce::ParameterID HEAD_4_GAIN                      {"HEAD_4_GAIN", 1};
    const juce::ParameterID HEAD_4_MOVEMENT_PERIOD_DURATION  {"HEAD_4_MOVEMENT_PERIOD_DURATION", 1};
    const juce::ParameterID HEAD_4_MOVEMENT_PERIOD_DURATION_NO_SYNC  {"HEAD_4_MOVEMENT_PERIOD_DURATION_NO_SYNC", 1};
    const juce::ParameterID HEAD_4_MOVEMENT_FUNCTION         {"HEAD_4_MOVEMENT_FUNCTION", 1};
    const juce::ParameterID HEAD_4_MOVEMENT_WIDTH            {"HEAD_4_MOVEMENT_WIDTH", 1};
    const juce::ParameterID HEAD_4_MOVEMENT_ON               {"HEAD_4_MOVEMENT_ON", 1};
    const juce::ParameterID HEAD_4_MOVEMENT_WIDTH_SLAVE      {"HEAD_4_MOVEMENT_WIDTH_SLAVE", 1};
    const juce::ParameterID HEAD_4_LP_FILTER_FREQ            {"HEAD_4_LP_FILTER_FREQ", 1};
    const juce::ParameterID HEAD_4_HP_FILTER_FREQ            {"HEAD_4_HP_FILTER_FREQ", 1};
    const juce::ParameterID HEAD_4_FEEDBACK_SLAVE            {"HEAD_4_FEEDBACK_SLAVE", 1};
    const juce::ParameterID HEAD_4_GAIN_SLAVE                {"HEAD_4_GAIN_SLAVE", 1};
    const juce::ParameterID HEAD_4_LP_FILTER_BYPASS          {"HEAD_4_LP_FILTER_BYPASS", 1};
    const juce::ParameterID HEAD_4_HP_FILTER_BYPASS          {"HEAD_4_HP_FILTER_BYPASS", 1};

}