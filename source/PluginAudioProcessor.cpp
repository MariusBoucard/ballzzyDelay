#include "PluginAudioProcessor.h"


PluginAudioProcessor::PluginAudioProcessor()
    : juce::AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo())
          .withOutput("Output", juce::AudioChannelSet::stereo()))
      , mParameters{*this, nullptr, "PARAMETERS", createParameterLayout(parametersDeclaration)}
      , mPresetManager{std::make_unique<PresetManager>(mParameters)}
      , mParameterSetup(mParameters)
      , mSkeletonProcessor(mParameters, mParameterSetup, parametersDeclaration, *mPresetManager) {
    for (auto *param: mParameters.processor.getParameters()) {
        auto paramID = static_cast<juce::AudioProcessorParameterWithID *>(param)->paramID;
        mParameters.addParameterListener(paramID, this);
    }

    mParameterSetup.initParametersListener(*this);
}


PluginAudioProcessor::~PluginAudioProcessor() {
    for (auto *param: getParameters()) {
        auto paramID = static_cast<juce::AudioProcessorParameterWithID *>(param)->paramID;
        mParameters.removeParameterListener(paramID, this);;
    }
}

void PluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &a) {
    auto *playHead = getPlayHead();
    if (playHead) {
        auto postion = playHead->getPosition();
        if (postion.hasValue() && postion->getBpm().hasValue()) {
            currentBpm.store(*postion->getBpm());
        }
    }
    updateMovmentPosition();
    mSkeletonProcessor.processBlock(buffer, a);
}

void PluginAudioProcessor::updateMovmentPosition() {
    juce::AudioPlayHead *playHead = getPlayHead();


    for (int i = 0; i < 4; ++i) {
        updateMovmentHeadPosition(i, playHead);
    }
}

void PluginAudioProcessor::updateMovmentHeadPosition(int inHeadNumber, juce::AudioPlayHead *playHead) {
    if (playHead == nullptr)
        return;

    auto position = playHead->getPosition();
    if (!position.hasValue())
        return;

    if (position->getTimeInSeconds().hasValue()) {
        mParameterSetup.setPlayTime(*position->getTimeInSeconds(), currentBpm.load());
    }
}

float PluginAudioProcessor::getTimeFromIndex(float index) {
    double bpm = mParameters.getRawParameterValue(id::USER_BPM.getParamID())->load();
    bool hostTempoBpm = mParameters.getRawParameterValue(id::BPM_FROM_HOST.getParamID())->load();
    if (hostTempoBpm) {
        bpm = currentBpm.load();
    }

    const float beatDuration = 60.0f / bpm;
    float beatMultiplier = 0.0f;

    int idx = static_cast<int>(index);

    switch (idx) {
        case 0: beatMultiplier = 4.0f / 64.0f;
            break; // 1/64
        case 1: beatMultiplier = (4.0f / 32.0f) * (2.f / 3.f);
            break; // 1/32T
        case 2: beatMultiplier = (4.0f / 64.0f) * 1.5f;
            break; // 1/64D
        case 3: beatMultiplier = 4.0f / 32.0f;
            break; // 1/32
        case 4: beatMultiplier = (4.0f / 16.0f) * (2.f / 3.f);
            break; // 1/16T
        case 5: beatMultiplier = (4.0f / 32.0f) * 1.5f;
            break; // 1/32D
        case 6: beatMultiplier = 4.0f / 16.0f;
            break; // 1/16
        case 7: beatMultiplier = (4.0f / 8.0f) * (2.f / 3.f);
            break; // 1/8T
        case 8: beatMultiplier = (4.0f / 16.0f) * 1.5f;
            break; // 1/16D
        case 9: beatMultiplier = 4.0f / 8.0f;
            break; // 1/8
        case 10: beatMultiplier = (4.0f / 4.0f) * (2.f / 3.f);
            break; // 1/4T
        case 11: beatMultiplier = (4.0f / 8.0f) * 1.5f;
            break; // 1/8D
        case 12: beatMultiplier = 4.0f / 4.0f;
            break; // 1/4 (1 beat)
        case 13: beatMultiplier = (4.0f / 2.0f) * (2.f / 3.f);
            break; // 1/2T
        case 14: beatMultiplier = (4.0f / 4.0f) * 1.5f;
            break; // 1/4D
        case 15: beatMultiplier = 4.0f / 2.0f;
            break; // 1/2
        case 16: beatMultiplier = 4.0f * (2.f / 3.f);
            break; // 1T (Whole Triplet)
        case 17: beatMultiplier = (4.0f / 2.0f) * 1.5f;
            break; // 1/2D
        case 18: beatMultiplier = 4.0f;
            break; // 1 (Whole note)
        default: beatMultiplier = 1.0f;
            break;
    }

    return beatMultiplier * beatDuration;
}

bool PluginAudioProcessor::updateGlobalTimeParameters(const juce::String &parameterID, float newValue) {
    const bool isNoSync = parameterID.equalsIgnoreCase(id::GLOBAL_TIME_NO_SYNC.getParamID());
    const bool isGlobal = parameterID.equalsIgnoreCase(id::GLOBAL_TIME.getParamID());

    if (isGlobal || isNoSync) {
        if (mFaustUI == nullptr) return true;

        auto *sourceParam = mParameters.getParameter(parameterID);
        if (sourceParam == nullptr) return true;
        const float normalizedValue = sourceParam->convertTo0to1(newValue);

        struct HeadParams {
            juce::String slaveID;
            juce::String timeID;
            juce::String timeNoSyncID;
        };

        const HeadParams heads[] = {
            {id::HEAD_1_TIME_SLAVE.getParamID(), id::HEAD_1_TIME.getParamID(), id::HEAD_1_TIME_NO_SYNC.getParamID()},
            {id::HEAD_2_TIME_SLAVE.getParamID(), id::HEAD_2_TIME.getParamID(), id::HEAD_2_TIME_NO_SYNC.getParamID()},
            {id::HEAD_3_TIME_SLAVE.getParamID(), id::HEAD_3_TIME.getParamID(), id::HEAD_3_TIME_NO_SYNC.getParamID()},
            {id::HEAD_4_TIME_SLAVE.getParamID(), id::HEAD_4_TIME.getParamID(), id::HEAD_4_TIME_NO_SYNC.getParamID()},
        };

        for (const auto &head: heads) {
            if (mParameters.getRawParameterValue(head.slaveID)->load() <= 0.5f)
                continue;

            const juce::String &targetID = isNoSync ? head.timeNoSyncID : head.timeID;
            if (auto *headParam = mParameters.getParameter(targetID))
                headParam->setValueNotifyingHost(normalizedValue);
        }
        return true;
    }

    if (parameterID.contains("TIME_SLAVE") && newValue > 0.5f) {
        const juce::String baseParam = parameterID.dropLastCharacters(6);

        auto *globalTime = mParameters.getParameter(id::GLOBAL_TIME.getParamID());
        auto *globalTimeNoSync = mParameters.getParameter(id::GLOBAL_TIME_NO_SYNC.getParamID());

        if (globalTime != nullptr)
            if (auto *headTime = mParameters.getParameter(baseParam))
                headTime->setValueNotifyingHost(globalTime->getValue());

        if (globalTimeNoSync != nullptr)
            if (auto *headTimeNoSync = mParameters.getParameter(baseParam + "_NO_SYNC"))
                headTimeNoSync->setValueNotifyingHost(globalTimeNoSync->getValue());

        return true;
    }

    return false;
}

bool PluginAudioProcessor::updateGlobalFeedBackParameters(const juce::String &parameterID, float newValue) {
    // If global feedback, update all the feedback slave.
    // if feedback slave, alors on update la tete courante avec global

    if (parameterID.equalsIgnoreCase("FEEDBACK")) {
        if (mFaustUI != nullptr && mParameters.getRawParameterValue(id::HEAD_1_FEEDBACK_SLAVE.getParamID())->load() >
            0.5f) {
            if (auto *headFeedBack = mParameters.getParameter(id::HEAD_1_FEEDBACK.getParamID())) {
                headFeedBack->setValueNotifyingHost(newValue);
            }
        }
        if (mFaustUI != nullptr && mParameters.getRawParameterValue(id::HEAD_2_FEEDBACK_SLAVE.getParamID())->load() >
            0.5f) {
            if (auto *headFeedBack = mParameters.getParameter(id::HEAD_2_FEEDBACK.getParamID())) {
                headFeedBack->setValueNotifyingHost(newValue);
            }
        }
        if (mFaustUI != nullptr && mParameters.getRawParameterValue(id::HEAD_3_FEEDBACK_SLAVE.getParamID())->load() >
            0.5f) {
            if (auto *headFeedBack = mParameters.getParameter(id::HEAD_3_FEEDBACK.getParamID())) {
                headFeedBack->setValueNotifyingHost(newValue);
            }
        }
        if (mFaustUI != nullptr && mParameters.getRawParameterValue(id::HEAD_4_FEEDBACK_SLAVE.getParamID())->load() >
            0.5f) {
            if (auto *headFeedBack = mParameters.getParameter(id::HEAD_1_FEEDBACK.getParamID())) {
                headFeedBack->setValueNotifyingHost(newValue);
            }
        }
        return true;
    }

    if (parameterID.contains("FEEDBACK_SLAVE")) {
        juce::String param = parameterID.substring(0, parameterID.length() - 6); // on enleve le slave
        if (auto *headFeedBack = mParameters.getParameter(param)) {
            headFeedBack->setValueNotifyingHost(mParameters.getRawParameterValue(id::FEEDBACK.getParamID())->load());
        }
        return true;
    }

    // TODO : ce call fait crasher car peut faire une loop de call en cas de valeur bien diff entre fedback et feedback head
    // test si on est en mode slave, mais on essaye de update une head ?
    if (parameterID.contains("FEEDBACK") && mParameters.getRawParameterValue(parameterID + "_SLAVE")->load() > 0.5f) {
        if (auto *headFeedBack = mParameters.getParameter(parameterID)) {
            if (mParameters.getRawParameterValue(parameterID)->load() != mParameters.getRawParameterValue(
                    id::FEEDBACK.getParamID())->load()) {
                //    headFeedBack->setValueNotifyingHost(mParameters.getRawParameterValue(id::FEEDBACK.getParamID())->load());
            }
        }
    }
    return false;
}

bool PluginAudioProcessor::updateGlobalWidthParameters(const juce::String &parameterID, float newValue) {
    // If global width, update all the width slave.
    // if feedback slave, alors on update la tete courante avec global

    if (parameterID.equalsIgnoreCase("WIDTH")) {
        if (mFaustUI != nullptr && mParameters.getRawParameterValue(id::HEAD_1_MOVEMENT_WIDTH_SLAVE.getParamID())->
            load() > 0.5f) {
            if (auto *headFeedBack = mParameters.getParameter(id::HEAD_1_MOVEMENT_WIDTH.getParamID())) {
                headFeedBack->setValueNotifyingHost(newValue);
            }
        }
        if (mFaustUI != nullptr && mParameters.getRawParameterValue(id::HEAD_2_MOVEMENT_WIDTH_SLAVE.getParamID())->
            load() > 0.5f) {
            if (auto *headFeedBack = mParameters.getParameter(id::HEAD_2_MOVEMENT_WIDTH.getParamID())) {
                headFeedBack->setValueNotifyingHost(newValue);
            }
        }
        if (mFaustUI != nullptr && mParameters.getRawParameterValue(id::HEAD_3_MOVEMENT_WIDTH_SLAVE.getParamID())->
            load() > 0.5f) {
            if (auto *headFeedBack = mParameters.getParameter(id::HEAD_3_MOVEMENT_WIDTH.getParamID())) {
                headFeedBack->setValueNotifyingHost(newValue);
            }
        }
        if (mFaustUI != nullptr && mParameters.getRawParameterValue(id::HEAD_4_MOVEMENT_WIDTH_SLAVE.getParamID())->
            load() > 0.5f) {
            if (auto *headFeedBack = mParameters.getParameter(id::HEAD_4_MOVEMENT_WIDTH.getParamID())) {
                headFeedBack->setValueNotifyingHost(newValue);
            }
        }
        return true;
    }

    if (parameterID.contains("WIDTH_SLAVE")) {
        juce::String param = parameterID.substring(0, parameterID.length() - 6); // on enleve le slave
        if (auto *headFeedBack = mParameters.getParameter(param)) {
            headFeedBack->setValueNotifyingHost(mParameters.getRawParameterValue(id::WIDTH.getParamID())->load());
        }
        return true;
    }

    // test si on est en mode slave, mais on essaye de update une head ? ICI Cette condition casse tout !
    // Risque de boucle reentrante non négligeable
    if (parameterID.contains("WIDTH") && mParameters.getRawParameterValue(parameterID + "_SLAVE")->load() > 0.5f) {
        if (auto *headFeedBack = mParameters.getParameter(parameterID)) {
            if (mParameters.getRawParameterValue(parameterID)->load() != mParameters.getRawParameterValue(
                    id::WIDTH.getParamID())->load()) {
                //  headFeedBack->setValueNotifyingHost(mParameters.getRawParameterValue(id::WIDTH.getParamID())->load());
            }
        }
    }
    return false;
}
