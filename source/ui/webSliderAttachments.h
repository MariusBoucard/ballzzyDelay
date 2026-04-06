#include "../dsp/ParameterIDs.hpp"

class WebSliderAttachments {
public:
    WebSliderAttachments(SkeletonAudioProcessor& p) :
        processorRef(p),
        // Global Parameters
        webGainRelay{id::GAIN.getParamID()},
        webBypassRelay{id::BYPASS.getParamID()},
        webInputGainRelay{id::INPUT_GAIN.getParamID()},
        webOutputGainRelay{id::OUTPUT_GAIN.getParamID()},
        webMixRelay{id::MIX.getParamID()},
        webFeedbackRelay{id::FEEDBACK.getParamID()},
        webTimeRelay{id::TIME.getParamID()},
        webSyncTempoRelay{id::SYNC_TEMPO.getParamID()},
        webDuckingRelay{id::DUCKING.getParamID()},
        webDuckingAttackRelay{id::DUCKING_ATTACK.getParamID()},
        webDuckingReleaseRelay{id::DUCKING_RELEASE.getParamID()},
        webDuckingThresholdRelay{id::DUCKING_THRESHOLD.getParamID()},
        webDuckingRatioRelay{id::DUCKING_RATIO.getParamID()},
        webWidthRelay{id::WIDTH.getParamID()},
        webLpFilterFreqRelay{id::LP_FILTER_FREQ.getParamID()},
        webHpFilterFreqRelay{id::HP_FILTER_FREQ.getParamID()},
        webHpBpRelay{id::HP_FILTER_BYPASS.getParamID()},
        webLpBpRelay{id::LP_FILTER_BYPASS.getParamID()},

        // Head 1 Parameters
        webHead1BypassRelay{id::HEAD_1_ON.getParamID()},
        webHead1FeedbackRelay{id::HEAD_1_FEEDBACK.getParamID()},
        webHead1PanRelay{id::HEAD_1_PAN.getParamID()},
        webHead1TimeRelay{id::HEAD_1_TIME.getParamID()},
        webHead1TimeNoSyncRelay{id::HEAD_1_TIME_NO_SYNC.getParamID()},
        webHead1GainRelay{id::HEAD_1_GAIN.getParamID()},
        webHead1MovementPeriodDurationRelay{id::HEAD_1_MOVEMENT_PERIOD_DURATION.getParamID()},
        webHead1MovementFunctionRelay{id::HEAD_1_MOVEMENT_FUNCTION.getParamID()},
        webHead1MovementWidthRelay{id::HEAD_1_MOVEMENT_WIDTH.getParamID()},
        webHead1MovementOnRelay{id::HEAD_1_MOVEMENT_ON.getParamID()},
        webHead1MovementWidthSlaveRelay{id::HEAD_1_MOVEMENT_WIDTH_SLAVE.getParamID()},
        webHead1LpFilterFreqRelay{id::HEAD_1_LP_FILTER_FREQ.getParamID()},
        webHead1HpFilterFreqRelay{id::HEAD_1_HP_FILTER_FREQ.getParamID()},
        webHead1FeedbackSlaveRelay{id::HEAD_1_FEEDBACK_SLAVE.getParamID()},
        webHead1GainSlaveRelay{id::HEAD_1_GAIN_SLAVE.getParamID()},
        webHead1HpBpRelay{id::HEAD_1_HP_FILTER_BYPASS.getParamID()},
        webHead1LpBpRelay{id::HEAD_1_LP_FILTER_BYPASS.getParamID()},

        // Head 2 Parameters
        webHead2BypassRelay{id::HEAD_2_ON.getParamID()},
        webHead2FeedbackRelay{id::HEAD_2_FEEDBACK.getParamID()},
        webHead2PanRelay{id::HEAD_2_PAN.getParamID()},
        webHead2TimeRelay{id::HEAD_2_TIME.getParamID()},
        webHead2TimeNoSyncRelay{id::HEAD_2_TIME_NO_SYNC.getParamID()},
        webHead2GainRelay{id::HEAD_2_GAIN.getParamID()},
        webHead2MovementPeriodDurationRelay{id::HEAD_2_MOVEMENT_PERIOD_DURATION.getParamID()},
        webHead2MovementFunctionRelay{id::HEAD_2_MOVEMENT_FUNCTION.getParamID()},
        webHead2MovementWidthRelay{id::HEAD_2_MOVEMENT_WIDTH.getParamID()},
        webHead2MovementOnRelay{id::HEAD_2_MOVEMENT_ON.getParamID()},
        webHead2MovementWidthSlaveRelay{id::HEAD_2_MOVEMENT_WIDTH_SLAVE.getParamID()},
        webHead2LpFilterFreqRelay{id::HEAD_2_LP_FILTER_FREQ.getParamID()},
        webHead2HpFilterFreqRelay{id::HEAD_2_HP_FILTER_FREQ.getParamID()},
        webHead2FeedbackSlaveRelay{id::HEAD_2_FEEDBACK_SLAVE.getParamID()},
        webHead2GainSlaveRelay{id::HEAD_2_GAIN_SLAVE.getParamID()},
        webHead2HpBpRelay{id::HEAD_2_HP_FILTER_BYPASS.getParamID()},
        webHead2LpBpRelay{id::HEAD_2_LP_FILTER_BYPASS.getParamID()},

        // Head 3 Parameters
        webHead3BypassRelay{id::HEAD_3_ON.getParamID()},
        webHead3FeedbackRelay{id::HEAD_3_FEEDBACK.getParamID()},
        webHead3PanRelay{id::HEAD_3_PAN.getParamID()},
        webHead3TimeRelay{id::HEAD_3_TIME.getParamID()},
        webHead3TimeNoSyncRelay{id::HEAD_3_TIME_NO_SYNC.getParamID()},
        webHead3GainRelay{id::HEAD_3_GAIN.getParamID()},
        webHead3MovementPeriodDurationRelay{id::HEAD_3_MOVEMENT_PERIOD_DURATION.getParamID()},
        webHead3MovementFunctionRelay{id::HEAD_3_MOVEMENT_FUNCTION.getParamID()},
        webHead3MovementWidthRelay{id::HEAD_3_MOVEMENT_WIDTH.getParamID()},
        webHead3MovementOnRelay{id::HEAD_3_MOVEMENT_ON.getParamID()},
        webHead3MovementWidthSlaveRelay{id::HEAD_3_MOVEMENT_WIDTH_SLAVE.getParamID()},
        webHead3LpFilterFreqRelay{id::HEAD_3_LP_FILTER_FREQ.getParamID()},
        webHead3HpFilterFreqRelay{id::HEAD_3_HP_FILTER_FREQ.getParamID()},
        webHead3FeedbackSlaveRelay{id::HEAD_3_FEEDBACK_SLAVE.getParamID()},
        webHead3GainSlaveRelay{id::HEAD_3_GAIN_SLAVE.getParamID()},
        webHead3HpBpRelay{id::HEAD_3_HP_FILTER_BYPASS.getParamID()},
        webHead3LpBpRelay{id::HEAD_3_LP_FILTER_BYPASS.getParamID()},

        // Head 4 Parameters
        webHead4BypassRelay{id::HEAD_4_ON.getParamID()},
        webHead4FeedbackRelay{id::HEAD_4_FEEDBACK.getParamID()},
        webHead4PanRelay{id::HEAD_4_PAN.getParamID()},
        webHead4TimeRelay{id::HEAD_4_TIME.getParamID()},
        webHead4TimeNoSyncRelay{id::HEAD_4_TIME_NO_SYNC.getParamID()},
        webHead4GainRelay{id::HEAD_4_GAIN.getParamID()},
        webHead4MovementPeriodDurationRelay{id::HEAD_4_MOVEMENT_PERIOD_DURATION.getParamID()},
        webHead4MovementFunctionRelay{id::HEAD_4_MOVEMENT_FUNCTION.getParamID()},
        webHead4MovementWidthRelay{id::HEAD_4_MOVEMENT_WIDTH.getParamID()},
        webHead4MovementOnRelay{id::HEAD_4_MOVEMENT_ON.getParamID()},
        webHead4MovementWidthSlaveRelay{id::HEAD_4_MOVEMENT_WIDTH_SLAVE.getParamID()},
        webHead4LpFilterFreqRelay{id::HEAD_4_LP_FILTER_FREQ.getParamID()},
        webHead4HpFilterFreqRelay{id::HEAD_4_HP_FILTER_FREQ.getParamID()},
        webHead4FeedbackSlaveRelay{id::HEAD_4_FEEDBACK_SLAVE.getParamID()},
        webHead4GainSlaveRelay{id::HEAD_4_GAIN_SLAVE.getParamID()},
        webHead4HpBpRelay{id::HEAD_4_HP_FILTER_BYPASS.getParamID()},
        webHead4LpBpRelay{id::HEAD_4_LP_FILTER_BYPASS.getParamID()}

    {}

   void createAttachments() {
    auto& state = processorRef.getState();

    // Global Parameters
    if (auto* param = state.getParameter(id::GAIN.getParamID()))
        webGainSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webGainRelay, nullptr);

    if (auto* param = state.getParameter(id::BYPASS.getParamID()))
        webBypassToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webBypassRelay, nullptr);

    if (auto* param = state.getParameter(id::INPUT_GAIN.getParamID()))
        webInputGainSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webInputGainRelay, nullptr);

    if (auto* param = state.getParameter(id::OUTPUT_GAIN.getParamID()))
        webOutputGainSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webOutputGainRelay, nullptr);

    if (auto* param = state.getParameter(id::MIX.getParamID()))
        webMixSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webMixRelay, nullptr);

    if (auto* param = state.getParameter(id::FEEDBACK.getParamID()))
        webFeedbackSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webFeedbackRelay, nullptr);

    if (auto* param = state.getParameter(id::SYNC_TEMPO.getParamID()))
        webSyncTempoToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webSyncTempoRelay, nullptr);

    if (auto* param = state.getParameter(id::DUCKING.getParamID()))
        webDuckingToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webDuckingRelay, nullptr);

    if (auto* param = state.getParameter(id::DUCKING_ATTACK.getParamID()))
        webDuckingAttackSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webDuckingAttackRelay, nullptr);

    if (auto* param = state.getParameter(id::DUCKING_RELEASE.getParamID()))
        webDuckingReleaseSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webDuckingReleaseRelay, nullptr);

    if (auto* param = state.getParameter(id::DUCKING_THRESHOLD.getParamID()))
        webDuckingThresholdSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webDuckingThresholdRelay, nullptr);
    if (auto* param = state.getParameter(id::DUCKING_RATIO.getParamID()))
        webDuckingRatioSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webDuckingRatioRelay, nullptr);

    if (auto* param = state.getParameter(id::WIDTH.getParamID()))
        webWidthSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webWidthRelay, nullptr);

    if (auto* param = state.getParameter(id::TIME.getParamID()))
            webTimeSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webTimeRelay, nullptr);


    if (auto* param = state.getParameter(id::LP_FILTER_FREQ.getParamID()))
        webLpFilterFreqSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webLpFilterFreqRelay, nullptr);

    if (auto* param = state.getParameter(id::HP_FILTER_FREQ.getParamID()))
        webHpFilterFreqSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHpFilterFreqRelay, nullptr);

    if (auto* param = state.getParameter(id::LP_FILTER_BYPASS.getParamID()))
        webLpBpToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webLpBpRelay, nullptr);
    if (auto* param = state.getParameter(id::HP_FILTER_BYPASS.getParamID()))
        webHpBpToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHpBpRelay, nullptr);

    // Head 1 Parameters
    if (auto* param = state.getParameter(id::HEAD_1_ON.getParamID()))
        webHead1BypassToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead1BypassRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_1_FEEDBACK.getParamID()))
        webHead1FeedbackSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead1FeedbackRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_1_PAN.getParamID()))
        webHead1PanSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead1PanRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_1_GAIN.getParamID()))
        webHead1GainSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead1GainRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_1_MOVEMENT_PERIOD_DURATION.getParamID()))
        webHead1MovementPeriodDurationSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead1MovementPeriodDurationRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_1_MOVEMENT_FUNCTION.getParamID()))
        webHead1MovementFunctionComboBoxAttachment = std::make_unique<juce::WebComboBoxParameterAttachment>(
            *param, webHead1MovementFunctionRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_1_MOVEMENT_WIDTH.getParamID()))
        webHead1MovementWidthSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead1MovementWidthRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_1_MOVEMENT_ON.getParamID()))
        webHead1MovementOnToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead1MovementOnRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_1_MOVEMENT_WIDTH_SLAVE.getParamID()))
        webHead1MovementWidthSlaveToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead1MovementWidthSlaveRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_1_LP_FILTER_FREQ.getParamID()))
        webHead1LpFilterFreqSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead1LpFilterFreqRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_1_HP_FILTER_FREQ.getParamID()))
        webHead1HpFilterFreqSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead1HpFilterFreqRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_1_FEEDBACK_SLAVE.getParamID()))
        webHead1FeedbackSlaveToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead1FeedbackSlaveRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_1_GAIN_SLAVE.getParamID()))
        webHead1GainSlaveToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead1GainSlaveRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_1_HP_FILTER_BYPASS.getParamID()))
        webHead1HpBpToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead1HpBpRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_1_LP_FILTER_BYPASS.getParamID()))
        webHead1LpBpToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead1LpBpRelay, nullptr);

    // Head 2 Parameters
    if (auto* param = state.getParameter(id::HEAD_2_ON.getParamID()))
        webHead2BypassToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead2BypassRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_2_FEEDBACK.getParamID()))
        webHead2FeedbackSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead2FeedbackRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_2_PAN.getParamID()))
        webHead2PanSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead2PanRelay, nullptr);
    if (auto* param = state.getParameter(id::HEAD_1_TIME.getParamID()))
            webHead1TimeSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webHead1TimeRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_2_TIME.getParamID()))
        webHead2TimeSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead2TimeRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_3_TIME.getParamID()))
        webHead3TimeSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead3TimeRelay, nullptr);
    if (auto* param = state.getParameter(id::HEAD_4_TIME.getParamID()))
        webHead4TimeSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead4TimeRelay, nullptr);

        if (auto* param = state.getParameter(id::HEAD_1_TIME_NO_SYNC.getParamID()))
            webHead1TimeNoSyncSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webHead1TimeNoSyncRelay, nullptr);

        if (auto* param = state.getParameter(id::HEAD_2_TIME_NO_SYNC.getParamID()))
            webHead2TimeNoSyncSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webHead2TimeNoSyncRelay, nullptr);

        if (auto* param = state.getParameter(id::HEAD_3_TIME_NO_SYNC.getParamID()))
            webHead3TimeNoSyncSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webHead3TimeNoSyncRelay, nullptr);
        if (auto* param = state.getParameter(id::HEAD_4_TIME_NO_SYNC.getParamID()))
            webHead4TimeNoSyncSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
                *param, webHead4TimeNoSyncRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_2_GAIN.getParamID()))
        webHead2GainSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead2GainRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_2_MOVEMENT_PERIOD_DURATION.getParamID()))
        webHead2MovementPeriodDurationSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead2MovementPeriodDurationRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_2_MOVEMENT_FUNCTION.getParamID()))
        webHead2MovementFunctionComboBoxAttachment = std::make_unique<juce::WebComboBoxParameterAttachment>(
            *param, webHead2MovementFunctionRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_2_MOVEMENT_WIDTH.getParamID()))
        webHead2MovementWidthSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead2MovementWidthRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_2_MOVEMENT_ON.getParamID()))
        webHead2MovementOnToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead2MovementOnRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_2_MOVEMENT_WIDTH_SLAVE.getParamID()))
        webHead2MovementWidthSlaveToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead2MovementWidthSlaveRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_2_LP_FILTER_FREQ.getParamID()))
        webHead2LpFilterFreqSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead2LpFilterFreqRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_2_HP_FILTER_FREQ.getParamID()))
        webHead2HpFilterFreqSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead2HpFilterFreqRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_2_FEEDBACK_SLAVE.getParamID()))
        webHead2FeedbackSlaveToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead2FeedbackSlaveRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_2_GAIN_SLAVE.getParamID()))
        webHead2GainSlaveToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead2GainSlaveRelay, nullptr);

        if (auto* param = state.getParameter(id::HEAD_2_HP_FILTER_BYPASS.getParamID()))
            webHead2HpBpToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
                *param, webHead2HpBpRelay, nullptr);

        if (auto* param = state.getParameter(id::HEAD_2_LP_FILTER_BYPASS.getParamID()))
            webHead2LpBpToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
                *param, webHead2LpBpRelay, nullptr);

    // Head 3 Parameters
    if (auto* param = state.getParameter(id::HEAD_3_ON.getParamID()))
        webHead3BypassToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead3BypassRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_3_FEEDBACK.getParamID()))
        webHead3FeedbackSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead3FeedbackRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_3_PAN.getParamID()))
        webHead3PanSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead3PanRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_3_GAIN.getParamID()))
        webHead3GainSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead3GainRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_3_MOVEMENT_PERIOD_DURATION.getParamID()))
        webHead3MovementPeriodDurationSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead3MovementPeriodDurationRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_3_MOVEMENT_FUNCTION.getParamID()))
        webHead3MovementFunctionComboBoxAttachment = std::make_unique<juce::WebComboBoxParameterAttachment>(
            *param, webHead3MovementFunctionRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_3_MOVEMENT_WIDTH.getParamID()))
        webHead3MovementWidthSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead3MovementWidthRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_3_MOVEMENT_ON.getParamID()))
        webHead3MovementOnToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead3MovementOnRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_3_MOVEMENT_WIDTH_SLAVE.getParamID()))
        webHead3MovementWidthSlaveToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead3MovementWidthSlaveRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_3_LP_FILTER_FREQ.getParamID()))
        webHead3LpFilterFreqSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead3LpFilterFreqRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_3_HP_FILTER_FREQ.getParamID()))
        webHead3HpFilterFreqSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead3HpFilterFreqRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_3_FEEDBACK_SLAVE.getParamID()))
        webHead3FeedbackSlaveToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead3FeedbackSlaveRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_3_GAIN_SLAVE.getParamID()))
        webHead3GainSlaveToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead3GainSlaveRelay, nullptr);


        if (auto* param = state.getParameter(id::HEAD_3_HP_FILTER_BYPASS.getParamID()))
            webHead3HpBpToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
                *param, webHead3HpBpRelay, nullptr);

        if (auto* param = state.getParameter(id::HEAD_3_LP_FILTER_BYPASS.getParamID()))
            webHead3LpBpToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
                *param, webHead3LpBpRelay, nullptr);
    // Head 4 Parameters
    if (auto* param = state.getParameter(id::HEAD_4_ON.getParamID()))
        webHead4BypassToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead4BypassRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_4_FEEDBACK.getParamID()))
        webHead4FeedbackSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead4FeedbackRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_4_PAN.getParamID()))
        webHead4PanSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead4PanRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_4_GAIN.getParamID()))
        webHead4GainSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead4GainRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_4_MOVEMENT_PERIOD_DURATION.getParamID()))
        webHead4MovementPeriodDurationSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead4MovementPeriodDurationRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_4_MOVEMENT_FUNCTION.getParamID()))
        webHead4MovementFunctionComboBoxAttachment = std::make_unique<juce::WebComboBoxParameterAttachment>(
            *param, webHead4MovementFunctionRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_4_MOVEMENT_WIDTH.getParamID()))
        webHead4MovementWidthSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead4MovementWidthRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_4_MOVEMENT_ON.getParamID()))
        webHead4MovementOnToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead4MovementOnRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_4_MOVEMENT_WIDTH_SLAVE.getParamID()))
        webHead4MovementWidthSlaveToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead4MovementWidthSlaveRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_4_LP_FILTER_FREQ.getParamID()))
        webHead4LpFilterFreqSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead4LpFilterFreqRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_4_HP_FILTER_FREQ.getParamID()))
        webHead4HpFilterFreqSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *param, webHead4HpFilterFreqRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_4_FEEDBACK_SLAVE.getParamID()))
        webHead4FeedbackSlaveToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead4FeedbackSlaveRelay, nullptr);

    if (auto* param = state.getParameter(id::HEAD_4_GAIN_SLAVE.getParamID()))
        webHead4GainSlaveToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *param, webHead4GainSlaveRelay, nullptr);


        if (auto* param = state.getParameter(id::HEAD_4_HP_FILTER_BYPASS.getParamID()))
            webHead4HpBpToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
                *param, webHead4HpBpRelay, nullptr);

        if (auto* param = state.getParameter(id::HEAD_4_LP_FILTER_BYPASS.getParamID()))
            webHead4LpBpToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
                *param, webHead4LpBpRelay, nullptr);
}

    juce::WebBrowserComponent::Options addSlidersOptions(juce::WebBrowserComponent::Options options) {
        return options
            // Global Parameters
            .withOptionsFrom(webGainRelay)
            .withOptionsFrom(webBypassRelay)
            .withOptionsFrom(webInputGainRelay)
            .withOptionsFrom(webOutputGainRelay)
            .withOptionsFrom(webMixRelay)
            .withOptionsFrom(webFeedbackRelay)
            .withOptionsFrom(webTimeRelay)
            .withOptionsFrom(webSyncTempoRelay)
            .withOptionsFrom(webDuckingRelay)
            .withOptionsFrom(webDuckingAttackRelay)
            .withOptionsFrom(webDuckingReleaseRelay)
            .withOptionsFrom(webDuckingThresholdRelay)
            .withOptionsFrom(webDuckingRatioRelay)
            .withOptionsFrom(webWidthRelay)
            .withOptionsFrom(webLpFilterFreqRelay)
            .withOptionsFrom(webHpFilterFreqRelay)
        .withOptionsFrom(webHpBpRelay)
        .withOptionsFrom(webLpBpRelay)

            // Head 1 Parameters
            .withOptionsFrom(webHead1BypassRelay)
            .withOptionsFrom(webHead1FeedbackRelay)
            .withOptionsFrom(webHead1PanRelay)
            .withOptionsFrom(webHead1TimeRelay)
        .withOptionsFrom(webHead1TimeNoSyncRelay)

            .withOptionsFrom(webHead1GainRelay)
            .withOptionsFrom(webHead1MovementPeriodDurationRelay)
            .withOptionsFrom(webHead1MovementFunctionRelay)
            .withOptionsFrom(webHead1MovementWidthRelay)
            .withOptionsFrom(webHead1MovementOnRelay)
            .withOptionsFrom(webHead1MovementWidthSlaveRelay)
            .withOptionsFrom(webHead1LpFilterFreqRelay)
            .withOptionsFrom(webHead1HpFilterFreqRelay)
            .withOptionsFrom(webHead1FeedbackSlaveRelay)
            .withOptionsFrom(webHead1GainSlaveRelay)
        .withOptionsFrom(webHead1HpBpRelay)
           .withOptionsFrom(webHead1LpBpRelay)
            // Head 2 Parameters
            .withOptionsFrom(webHead2BypassRelay)
            .withOptionsFrom(webHead2FeedbackRelay)
            .withOptionsFrom(webHead2PanRelay)
            .withOptionsFrom(webHead2TimeRelay)
        .withOptionsFrom(webHead2TimeNoSyncRelay)

            .withOptionsFrom(webHead2GainRelay)
            .withOptionsFrom(webHead2MovementPeriodDurationRelay)
            .withOptionsFrom(webHead2MovementFunctionRelay)
            .withOptionsFrom(webHead2MovementWidthRelay)
            .withOptionsFrom(webHead2MovementOnRelay)
            .withOptionsFrom(webHead2MovementWidthSlaveRelay)
            .withOptionsFrom(webHead2LpFilterFreqRelay)
            .withOptionsFrom(webHead2HpFilterFreqRelay)
            .withOptionsFrom(webHead2FeedbackSlaveRelay)
            .withOptionsFrom(webHead2GainSlaveRelay)
        .withOptionsFrom(webHead2HpBpRelay)
         .withOptionsFrom(webHead2LpBpRelay)

            // Head 3 Parameters
            .withOptionsFrom(webHead3BypassRelay)
            .withOptionsFrom(webHead3FeedbackRelay)
            .withOptionsFrom(webHead3PanRelay)
            .withOptionsFrom(webHead3TimeRelay)
            .withOptionsFrom(webHead3TimeNoSyncRelay)
            .withOptionsFrom(webHead3GainRelay)
            .withOptionsFrom(webHead3MovementPeriodDurationRelay)
            .withOptionsFrom(webHead3MovementFunctionRelay)
            .withOptionsFrom(webHead3MovementWidthRelay)
            .withOptionsFrom(webHead3MovementOnRelay)
            .withOptionsFrom(webHead3MovementWidthSlaveRelay)
            .withOptionsFrom(webHead3LpFilterFreqRelay)
            .withOptionsFrom(webHead3HpFilterFreqRelay)
            .withOptionsFrom(webHead3FeedbackSlaveRelay)
            .withOptionsFrom(webHead3GainSlaveRelay)
            .withOptionsFrom(webHead3HpBpRelay)
            .withOptionsFrom(webHead3LpBpRelay)

            // Head 4 Parameters
            .withOptionsFrom(webHead4BypassRelay)
            .withOptionsFrom(webHead4FeedbackRelay)
            .withOptionsFrom(webHead4PanRelay)
            .withOptionsFrom(webHead4TimeRelay)
            .withOptionsFrom(webHead4TimeNoSyncRelay)

            .withOptionsFrom(webHead4GainRelay)
            .withOptionsFrom(webHead4MovementPeriodDurationRelay)
            .withOptionsFrom(webHead4MovementFunctionRelay)
            .withOptionsFrom(webHead4MovementWidthRelay)
            .withOptionsFrom(webHead4MovementOnRelay)
            .withOptionsFrom(webHead4MovementWidthSlaveRelay)
            .withOptionsFrom(webHead4LpFilterFreqRelay)
            .withOptionsFrom(webHead4HpFilterFreqRelay)
            .withOptionsFrom(webHead4FeedbackSlaveRelay)
            .withOptionsFrom(webHead4GainSlaveRelay)
            .withOptionsFrom(webHead4HpBpRelay)
            .withOptionsFrom(webHead4LpBpRelay);
    }

private:
    SkeletonAudioProcessor& processorRef;

    // ============================================================================
    // Relays - Global Parameters
    // ============================================================================
    juce::WebSliderRelay webGainRelay;
    juce::WebToggleButtonRelay webBypassRelay;
    juce::WebSliderRelay webInputGainRelay;
    juce::WebSliderRelay webOutputGainRelay;
    juce::WebSliderRelay webMixRelay;
    juce::WebSliderRelay webFeedbackRelay;

    juce::WebSliderRelay webTimeRelay;

    juce::WebToggleButtonRelay webSyncTempoRelay;
    juce::WebToggleButtonRelay webDuckingRelay;
    juce::WebSliderRelay webDuckingAttackRelay;
    juce::WebSliderRelay webDuckingReleaseRelay;
    juce::WebSliderRelay webDuckingThresholdRelay;
    juce::WebSliderRelay webDuckingRatioRelay;


    juce::WebSliderRelay webWidthRelay;
    juce::WebSliderRelay webLpFilterFreqRelay;
    juce::WebSliderRelay webHpFilterFreqRelay;
    juce::WebToggleButtonRelay webHpBpRelay;
    juce::WebToggleButtonRelay webLpBpRelay;


    // ============================================================================
    // Relays - Head 1 Parameters
    // ============================================================================
    juce::WebToggleButtonRelay webHead1BypassRelay;
    juce::WebSliderRelay webHead1FeedbackRelay;
    juce::WebSliderRelay webHead1PanRelay;
    juce::WebSliderRelay webHead1TimeRelay;
    juce::WebSliderRelay webHead1TimeNoSyncRelay;
    juce::WebSliderRelay webHead1GainRelay;
    juce::WebSliderRelay webHead1MovementPeriodDurationRelay;
    juce::WebComboBoxRelay webHead1MovementFunctionRelay;
    juce::WebSliderRelay webHead1MovementWidthRelay;
    juce::WebToggleButtonRelay webHead1MovementOnRelay;
    juce::WebToggleButtonRelay webHead1MovementWidthSlaveRelay;
    juce::WebSliderRelay webHead1LpFilterFreqRelay;
    juce::WebSliderRelay webHead1HpFilterFreqRelay;
    juce::WebToggleButtonRelay webHead1FeedbackSlaveRelay;
    juce::WebToggleButtonRelay webHead1GainSlaveRelay;
    juce::WebToggleButtonRelay webHead1HpBpRelay;
    juce::WebToggleButtonRelay webHead1LpBpRelay;

    // ============================================================================
    // Relays - Head 2 Parameters
    // ============================================================================
    juce::WebToggleButtonRelay webHead2BypassRelay;
    juce::WebSliderRelay webHead2FeedbackRelay;
    juce::WebSliderRelay webHead2PanRelay;
    juce::WebSliderRelay webHead2TimeRelay;
    juce::WebSliderRelay webHead2TimeNoSyncRelay;
    juce::WebSliderRelay webHead2GainRelay;
    juce::WebSliderRelay webHead2MovementPeriodDurationRelay;
    juce::WebComboBoxRelay webHead2MovementFunctionRelay;
    juce::WebSliderRelay webHead2MovementWidthRelay;
    juce::WebToggleButtonRelay webHead2MovementOnRelay;
    juce::WebToggleButtonRelay webHead2MovementWidthSlaveRelay;
    juce::WebSliderRelay webHead2LpFilterFreqRelay;
    juce::WebSliderRelay webHead2HpFilterFreqRelay;
    juce::WebToggleButtonRelay webHead2FeedbackSlaveRelay;
    juce::WebToggleButtonRelay webHead2GainSlaveRelay;
    juce::WebToggleButtonRelay webHead2HpBpRelay;
    juce::WebToggleButtonRelay webHead2LpBpRelay;

    // ============================================================================
    // Relays - Head 3 Parameters
    // ============================================================================
    juce::WebToggleButtonRelay webHead3BypassRelay;
    juce::WebSliderRelay webHead3FeedbackRelay;
    juce::WebSliderRelay webHead3PanRelay;
    juce::WebSliderRelay webHead3TimeRelay;
    juce::WebSliderRelay webHead3TimeNoSyncRelay;
    juce::WebSliderRelay webHead3GainRelay;
    juce::WebSliderRelay webHead3MovementPeriodDurationRelay;
    juce::WebComboBoxRelay webHead3MovementFunctionRelay;
    juce::WebSliderRelay webHead3MovementWidthRelay;
    juce::WebToggleButtonRelay webHead3MovementOnRelay;
    juce::WebToggleButtonRelay webHead3MovementWidthSlaveRelay;
    juce::WebSliderRelay webHead3LpFilterFreqRelay;
    juce::WebSliderRelay webHead3HpFilterFreqRelay;
    juce::WebToggleButtonRelay webHead3FeedbackSlaveRelay;
    juce::WebToggleButtonRelay webHead3GainSlaveRelay;
    juce::WebToggleButtonRelay webHead3HpBpRelay;
    juce::WebToggleButtonRelay webHead3LpBpRelay;

    // ============================================================================
    // Relays - Head 4 Parameters
    // ============================================================================
    juce::WebToggleButtonRelay webHead4BypassRelay;
    juce::WebSliderRelay webHead4FeedbackRelay;
    juce::WebSliderRelay webHead4PanRelay;
    juce::WebSliderRelay webHead4TimeRelay;
    juce::WebSliderRelay webHead4TimeNoSyncRelay;
    juce::WebSliderRelay webHead4GainRelay;
    juce::WebSliderRelay webHead4MovementPeriodDurationRelay;
    juce::WebComboBoxRelay webHead4MovementFunctionRelay;
    juce::WebSliderRelay webHead4MovementWidthRelay;
    juce::WebToggleButtonRelay webHead4MovementOnRelay;
    juce::WebToggleButtonRelay webHead4MovementWidthSlaveRelay;
    juce::WebSliderRelay webHead4LpFilterFreqRelay;
    juce::WebSliderRelay webHead4HpFilterFreqRelay;
    juce::WebToggleButtonRelay webHead4FeedbackSlaveRelay;
    juce::WebToggleButtonRelay webHead4GainSlaveRelay;
    juce::WebToggleButtonRelay webHead4HpBpRelay;
    juce::WebToggleButtonRelay webHead4LpBpRelay;

    // ============================================================================
    // Attachments - Global Parameters
    // ============================================================================
    std::unique_ptr<juce::WebSliderParameterAttachment> webGainSliderAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webBypassToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webInputGainSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webOutputGainSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webMixSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webFeedbackSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webTimeSliderAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webSyncTempoToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webDuckingToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webDuckingAttackSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webDuckingReleaseSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webDuckingThresholdSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webDuckingRatioSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webWidthSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webLpFilterFreqSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHpFilterFreqSliderAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHpBpToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webLpBpToggleAttachment;



    // ============================================================================
    // Attachments - Head 1 Parameters
    // ============================================================================
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead1BypassToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead1FeedbackSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead1PanSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead1TimeSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead1TimeNoSyncSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead1GainSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead1MovementPeriodDurationSliderAttachment;
    std::unique_ptr<juce::WebComboBoxParameterAttachment> webHead1MovementFunctionComboBoxAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead1MovementWidthSliderAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead1MovementOnToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead1MovementWidthSlaveToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead1LpFilterFreqSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead1HpFilterFreqSliderAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead1FeedbackSlaveToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead1GainSlaveToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead1HpBpToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead1LpBpToggleAttachment;

    // ============================================================================
    // Attachments - Head 2 Parameters
    // ============================================================================
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead2BypassToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead2FeedbackSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead2PanSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead2TimeSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead2TimeNoSyncSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead2GainSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead2MovementPeriodDurationSliderAttachment;
    std::unique_ptr<juce::WebComboBoxParameterAttachment> webHead2MovementFunctionComboBoxAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead2MovementWidthSliderAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead2MovementOnToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead2MovementWidthSlaveToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead2LpFilterFreqSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead2HpFilterFreqSliderAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead2FeedbackSlaveToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead2GainSlaveToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead2HpBpToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead2LpBpToggleAttachment;

    // ============================================================================
    // Attachments - Head 3 Parameters
    // ============================================================================
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead3BypassToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead3FeedbackSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead3PanSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead3TimeSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead3TimeNoSyncSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead3GainSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead3MovementPeriodDurationSliderAttachment;
    std::unique_ptr<juce::WebComboBoxParameterAttachment> webHead3MovementFunctionComboBoxAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead3MovementWidthSliderAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead3MovementOnToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead3MovementWidthSlaveToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead3LpFilterFreqSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead3HpFilterFreqSliderAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead3FeedbackSlaveToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead3GainSlaveToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead3HpBpToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead3LpBpToggleAttachment;

    // ============================================================================
    // Attachments - Head 4 Parameters
    // ============================================================================
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead4BypassToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead4FeedbackSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead4PanSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead4TimeSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead4TimeNoSyncSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead4GainSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead4MovementPeriodDurationSliderAttachment;
    std::unique_ptr<juce::WebComboBoxParameterAttachment> webHead4MovementFunctionComboBoxAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead4MovementWidthSliderAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead4MovementOnToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead4MovementWidthSlaveToggleAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead4LpFilterFreqSliderAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webHead4HpFilterFreqSliderAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead4FeedbackSlaveToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead4GainSlaveToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead4HpBpToggleAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webHead4LpBpToggleAttachment;
};
