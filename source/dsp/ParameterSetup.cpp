#include "ParameterSetup.h"

#include "ParameterIDs.hpp"
#include "faustParameterMappers/faustParameterMap.h"

ParameterSetup::ParameterSetup(juce::AudioProcessorValueTreeState &inApvts)
    : juce::Thread("MappersProcessingThread")
      , mParameters(inApvts)
      , mFaustUI(nullptr)
      , mSetupData1(createSetupData())
      , mSetupData2(createSetupData())
{
    mCurrentParamsForAudio.store(&mSetupData1, std::memory_order_relaxed);
    mNextParamsForProcessing = &mSetupData2;
    initializeParameters();
    *mCurrentParamsForAudio.load(std::memory_order_relaxed) = *mNextParamsForProcessing;

    // Start timer for periodic Faust updates (60 Hz is smooth enough for movement)
    startTimerHz(60);
}

ParameterSetup::~ParameterSetup() {
    stopTimer();
    mTasksEvent.signal();
    mParameters.removeParameterListener("gain", this);
    mParameters.removeParameterListener("lowPassCutoff", this);
    mParameters.removeParameterListener("highPassResonance", this);

    stopThread(10);
}

void ParameterSetup::setFaustUI(MapUI* faustUI) {
    mFaustUI = faustUI;
}

ParameterSetupData ParameterSetup::createSetupData() {
    ParameterSetupData data;
    data.lowPassFilterCoeffs = {0.0, 0.0};
    data.highPassFilterCoeffs = {0.0, 0.0};
    data.gain = 0.0;

    return data;
}

void ParameterSetup::initParametersListener(juce::AudioProcessor& inProcessor) {

    startThread();
}

void ParameterSetup::initializeParameters() {
    if (auto *lpCutoffParam = mParameters.getRawParameterValue("lowPassCutoff"))
        mNextParamsForProcessing->lowPassFilterCoeffs.cutoff = static_cast<double>(lpCutoffParam->load());

    if (auto *hpResonanceParam = mParameters.getRawParameterValue("highPassResonance"))
        mNextParamsForProcessing->highPassFilterCoeffs.resonance = static_cast<double>(hpResonanceParam->load());

    mNextParamsForProcessing->version = 0;
}

const ParameterSetupData *ParameterSetup::getAudioThreadParams() const {
    return mCurrentParamsForAudio.load(std::memory_order_acquire);
}

void ParameterSetup::setHeadPanPosition(int headIndex, float panPosition) {
    if (headIndex >= 0 && headIndex < 4) {
        mHeadPanPositions[headIndex].store(panPosition, std::memory_order_relaxed);
    }
}

void ParameterSetup::parameterChanged(const juce::String &parameterID, float newValue) {
    juce::String paramIDCopy = parameterID;
/*
    MapperTask task = [this, paramIDCopy, newValue]() {
        ParameterSetupData *paramsToUpdate = mNextParamsForProcessing;
        {
            std::lock_guard<std::mutex> lock(mUpdateMutex);
            *paramsToUpdate = *mCurrentParamsForAudio.load(std::memory_order_relaxed);
        }

        bool calculationPerformed = false;

        if (paramIDCopy == "gain") {
            Mappers::setGain(paramsToUpdate->gain, static_cast<double>(newValue));
            calculationPerformed = true;
        }

        // Handle movement parameters for each head
        for (int headNum = 0; headNum < 4; ++headNum) {
            juce::String headPrefix = "head" + juce::String(headNum + 1) + "_";

            if (paramIDCopy == headPrefix + "movement_width") {
                paramsToUpdate->headMovement[headNum].width = newValue;
                calculationPerformed = true;
            }
            else if (paramIDCopy == headPrefix + "movement_function") {
                paramsToUpdate->headMovement[headNum].function = newValue;
                calculationPerformed = true;
            }
            else if (paramIDCopy == headPrefix + "movement_period_duration") {
                paramsToUpdate->headMovement[headNum].duration = newValue;
                calculationPerformed = true;
            }
            else if (paramIDCopy == headPrefix + "movement_period_starting_point") {
                paramsToUpdate->headMovement[headNum].startingPoint = newValue;
                calculationPerformed = true;
            }
            else if (paramIDCopy == headPrefix + "pan") {
                paramsToUpdate->headMovement[headNum].basePan = newValue;
                calculationPerformed = true;
            }
        }

        if (calculationPerformed) {
            std::lock_guard<std::mutex> lock(mUpdateMutex);
            paramsToUpdate->version++;
            performSwap();
        }
    };

    {
        std::lock_guard<std::mutex> lock(mTasksQueueMutex);
        mTaskQueue.push_back(std::move(task));
    }
    mTasksEvent.signal();
    */
}

void ParameterSetup::timerCallback() {
    // Update Faust with the latest pan positions from audio thread
    // This runs at 60 Hz on the message thread, avoiding audio thread blocking
    if (mFaustUI == nullptr)
        return;

    for (int i = 0; i < 4; ++i) {
        float panPosition = mHeadPanPositions[i].load(std::memory_order_relaxed);
        updateFaustHeadPan(i, panPosition);
    }
}

void ParameterSetup::updateFaustHeadPan(int headIndex, float panPosition) {
    if (mFaustUI == nullptr)
        return;

    // Map head index to parameter ID (adjust based on your actual parameter IDs)
    juce::String paramID = "HEAD_" + juce::String(headIndex + 1) + "_PAN";
    auto path = FaustParameterMapping::getFaustPath(paramID);
    if (!path.empty()) {
        mFaustUI->setParamValue(path, panPosition);
    }
}

void ParameterSetup::run() {
    while (!threadShouldExit()) {
        mTasksEvent.wait(-1);

        if (threadShouldExit())
            break;

        // Process all tasks currently in the queue.
        while (true) {
            MapperTask taskToExecute;
            {
                std::lock_guard<std::mutex> lock(mTasksQueueMutex);
                if (mTaskQueue.empty())
                    break;

                taskToExecute = std::move(mTaskQueue.front());
                mTaskQueue.pop_front();
            }

            if (taskToExecute) {
                taskToExecute();
            }
        }
    }
}

void ParameterSetup::performSwap() {
    ParameterSetupData *tempOldCurrentAudioParams = mCurrentParamsForAudio.load(std::memory_order_relaxed);
    mCurrentParamsForAudio.store(mNextParamsForProcessing, std::memory_order_release);
    mNextParamsForProcessing = tempOldCurrentAudioParams;
}