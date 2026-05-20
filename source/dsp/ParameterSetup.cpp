#include "ParameterSetup.h"
#include "faustParameterMappers/faustParameterMap.h"

ParameterSetup::ParameterSetup(juce::AudioProcessorValueTreeState &inApvts)
    : juce::Thread("MappersProcessingThread")
      , mParameters(inApvts)
      , mFaustUI(nullptr)
{

    startTimerHz(60);
}

ParameterSetup::~ParameterSetup() {
    stopTimer();
    stopThread(10);
}

void ParameterSetup::setFaustUI(MapUI* faustUI) {
    mFaustUI = faustUI;
}

void ParameterSetup::parameterChanged(const juce::String& parameterID, float newValue) {

}

void ParameterSetup::initParametersListener(juce::AudioProcessor& inProcessor) {

    startThread();
}


void ParameterSetup::setPlayTime(double timeInSeconds, double bpm) {
    mCurrentBpm.store(bpm, std::memory_order_relaxed);
    mPlayTimeInSeconds.store(timeInSeconds, std::memory_order_relaxed);
}

void ParameterSetup::timerCallback() {
    if (mFaustUI == nullptr)
        return;
    const double playTime = mPlayTimeInSeconds.load(std::memory_order_relaxed);

    for (int i = 0; i < 4; ++i) {
        updateFaustHeadPan(i, playTime);
    }
}

void ParameterSetup::run() {
    // Pas de run ici ?
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
        duration = getTimeFromIndex(getTimeFromIndex(mParameters.getRawParameterValue(headPrefix + "MOVEMENT_PERIOD_DURATION")->load()));
    } else {
        duration = mParameters.getRawParameterValue(headPrefix + "MOVEMENT_PERIOD_DURATION_NO_SYNC")->load();
    }
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
    const float panCentered  = pan * 2;
    const float finalPan     = juce::jlimit(0.0f, 1.0f, panCentered + width * functionResult);

    // --- Push to Faust ---
    juce::String paramID = "HEAD_" + juce::String(headIndex + 1) + "_PAN";
    auto path = FaustParameterMapping::getFaustPath(paramID);
    if (!path.empty()) {
        mFaustUI->setParamValue(path, finalPan);
    }
}
