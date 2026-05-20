#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <atomic>
#include <mutex>
#include <deque>
#include <functional>

#include "ParameterIDs.hpp"
#include "Bones/FaustMultiheadFeedback.h"

struct FilterCoefficients
{
    double cutoff = 2000.0;
    double resonance = 0.707;
};

struct ParameterSetupData
{
    FilterCoefficients lowPassFilterCoeffs;
    FilterCoefficients highPassFilterCoeffs;
    double gain = 0.0;
    uint64_t version = 0;
};

// Forward declaration
class MapUI;

class ParameterSetup : public juce::Thread,
                       public juce::AudioProcessorValueTreeState::Listener,
                       public juce::Timer
{
public:
    using MapperTask = std::function<void()>;

    ParameterSetup(juce::AudioProcessorValueTreeState& inApvts);
    ~ParameterSetup() override;

    // Pass the Faust UI to enable parameter updates
    void setFaustUI(MapUI* faustUI);

    void parameterChanged(const juce::String& parameterID, float newValue) override;

    void initParametersListener(juce::AudioProcessor& inProcessor);

    void setPlayTime(double playTime, double bpm);

    float getTimeFromIndex(float index) {
    double bpm =  100;
    bool hostTempoBpm = mParameters.getRawParameterValue(id::BPM_FROM_HOST.getParamID())->load();
    if (hostTempoBpm) {
        bpm = mCurrentBpm.load();
    } else {
        mParameters.getRawParameterValue(id::USER_BPM.getParamID())->load();
    }

    const float beatDuration = 60.0f / bpm;
    float beatMultiplier = 0.0f;

    int idx = static_cast<int>(index);

    switch (idx) {
        case 0:  beatMultiplier = 4.0f / 64.0f;             break; // 1/64
        case 1:  beatMultiplier = (4.0f / 32.0f) * (2.f/3.f); break; // 1/32T
        case 2:  beatMultiplier = (4.0f / 64.0f) * 1.5f;      break; // 1/64D
        case 3:  beatMultiplier = 4.0f / 32.0f;             break; // 1/32
        case 4:  beatMultiplier = (4.0f / 16.0f) * (2.f/3.f); break; // 1/16T
        case 5:  beatMultiplier = (4.0f / 32.0f) * 1.5f;      break; // 1/32D
        case 6:  beatMultiplier = 4.0f / 16.0f;             break; // 1/16
        case 7:  beatMultiplier = (4.0f / 8.0f)  * (2.f/3.f); break; // 1/8T
        case 8:  beatMultiplier = (4.0f / 16.0f) * 1.5f;      break; // 1/16D
        case 9:  beatMultiplier = 4.0f / 8.0f;              break; // 1/8
        case 10: beatMultiplier = (4.0f / 4.0f)  * (2.f/3.f); break; // 1/4T
        case 11: beatMultiplier = (4.0f / 8.0f)  * 1.5f;      break; // 1/8D
        case 12: beatMultiplier = 4.0f / 4.0f;              break; // 1/4 (1 beat)
        case 13: beatMultiplier = (4.0f / 2.0f)  * (2.f/3.f); break; // 1/2T
        case 14: beatMultiplier = (4.0f / 4.0f)  * 1.5f;      break; // 1/4D
        case 15: beatMultiplier = 4.0f / 2.0f;              break; // 1/2
        case 16: beatMultiplier = 4.0f * (2.f/3.f);           break; // 1T (Whole Triplet)
        case 17: beatMultiplier = (4.0f / 2.0f)  * 1.5f;      break; // 1/2D
        case 18: beatMultiplier = 4.0f;                     break; // 1 (Whole note)
        default: beatMultiplier = 1.0f;                     break;
    }

    return beatMultiplier * beatDuration;
}

private:
    void run() override;
    void timerCallback() override;

    void updateFaustHeadPan(int headIndex, double playTime);

private:
    juce::AudioProcessorValueTreeState& mParameters;

    MapUI* mFaustUI;

    std::atomic<double> mPlayTimeInSeconds { 0.0 };
    std::atomic<double> mCurrentBpm { 100.0 };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterSetup)
};