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

    startTimerHz(60);
}

ParameterSetup::~ParameterSetup() {
    stopTimer();
    mTasksEvent.signal();

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

    mNextParamsForProcessing->version = 0;
}

const ParameterSetupData *ParameterSetup::getAudioThreadParams() const {
    return mCurrentParamsForAudio.load(std::memory_order_acquire);
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

void ParameterSetup::setPlayTime(double timeInSeconds, double bpm) {
    mCurrentBpm.store(bpm, std::memory_order_relaxed);
    mPlayTimeInSeconds.store(timeInSeconds, std::memory_order_relaxed);
}

void ParameterSetup::timerCallback() {
    // Update Faust with the latest pan positions from audio thread
    // This runs at 60 Hz on the message thread, avoiding audio thread blocking
    if (mFaustUI == nullptr)
        return;
    const double playTime = mPlayTimeInSeconds.load(std::memory_order_relaxed);

    for (int i = 0; i < 4; ++i) {
        updateFaustHeadPan(i, playTime);
    }
}

void ParameterSetup::updateFaustHeadPan(int headIndex, double playTime) {
    if (mFaustUI == nullptr)
        return;

    juce::String headPrefix = "HEAD_" + juce::String(headIndex + 1) + "_";

    const bool isMovementOnForThisHead = mParameters.getRawParameterValue(headPrefix + "MOVEMENT_ON" )->load();
    const bool isHeadOn = mParameters.getRawParameterValue(headPrefix+"ON")->load();

    if (!isMovementOnForThisHead || !isHeadOn) return;
    const bool isPeriodSync = mParameters.getRawParameterValue(headPrefix + "MOVEMENT_BPM_SYNC")->load();
    float duration = 0;

    if (isPeriodSync) {
        duration = getTimeFromIndex(mParameters.getRawParameterValue(headPrefix + "MOVEMENT_PERIOD_DURATION")->load());
    } else {
        duration = mParameters.getRawParameterValue(headPrefix + "MOVEMENT_PERIOD_DURATION_NO_SYNC")->load();
    }
    // TODO : attention que no sync pour le moment
    const float width         = mParameters.getRawParameterValue(headPrefix + "MOVEMENT_WIDTH")->load();
    const float startingPoint = mParameters.getRawParameterValue(headPrefix + "MOVEMENT_PERIOD_STARTING_POINT")->load();
    const float function      = mParameters.getRawParameterValue(headPrefix + "MOVEMENT_FUNCTION")->load();
    const float pan           = mParameters.getRawParameterValue(headPrefix + "PAN")->load();

    const double timeInSeconds = playTime;
    const float phase = duration > 0.0f
                        ? static_cast<float>(std::fmod(timeInSeconds / duration + startingPoint, 1.0))
                        : 0.0f;

    float functionResult = 0.0f;
    switch (static_cast<int>(function)) {
        case 0: // Sine
            functionResult = std::sin(phase * 2.0f * juce::MathConstants<float>::pi);
            break;

        case 1: // Square
            functionResult = (phase < 0.5f) ? 1.0f : -1.0f;
            break;

        case 2: // Triangle
            if (phase < 0.25f)
                functionResult = phase * 4.0f;
            else if (phase < 0.75f)
                functionResult = 1.0f - ((phase - 0.25f) * 4.0f);
            else
                functionResult = -1.0f + ((phase - 0.75f) * 4.0f);
            break;

        default:
            functionResult = 0.0f;
            break;
    }

    // --- Compute final pan (input is 0–1, Faust expects –1 to +1) ---
    const float panCentered  = pan * 2.0f;// - 1.0f; // TODO : y a un monde ou faust se prend du 0 : 1
    const float finalPan     = juce::jlimit(-1.0f, 1.0f, panCentered + width * functionResult);

    // --- Push to Faust ---
    juce::String paramID = "HEAD_" + juce::String(headIndex + 1) + "_PAN";
    auto path = FaustParameterMapping::getFaustPath(paramID);
    if (!path.empty()) {
        mFaustUI->setParamValue(path, finalPan);
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