#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <atomic>
#include <mutex>
#include <deque>
#include <functional>
#include "Mappers.h"
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

    ParameterSetupData createSetupData();

    void parameterChanged(const juce::String& parameterID, float newValue) override;

    const ParameterSetupData* getAudioThreadParams() const;
    void initParametersListener(juce::AudioProcessor& inProcessor);

    // Called from audio thread to update current playhead-based positions
    void setHeadPanPosition(int headIndex, float panPosition);

private:
    void run() override;
    void timerCallback() override;

    void initializeParameters();
    void performSwap();
    void updateFaustHeadPan(int headIndex, float panPosition);

    ParameterSetupData mSetupData1;
    ParameterSetupData mSetupData2;

    juce::AudioProcessorValueTreeState& mParameters;

    std::atomic<ParameterSetupData*> mCurrentParamsForAudio;
    ParameterSetupData* mNextParamsForProcessing;

    std::mutex mUpdateMutex;

    std::deque<MapperTask> mTaskQueue;
    std::mutex mTasksQueueMutex;
    juce::WaitableEvent mTasksEvent;

    MapUI* mFaustUI;

    std::atomic<float> mHeadPanPositions[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterSetup)
};