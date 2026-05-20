
#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_processors_headless/juce_audio_processors_headless.h>
#include "ui/gui.h"
#include "service/PresetManager.h"
#include "dsp/Processor.h"
#include "dsp/ParameterSetup.h"
#include "dsp/ParameterIDs.hpp"
#include "dsp/paramsDeclaration.h"
#include "dsp/faustParameterMappers/faustParameterMap.h"
#include "dsp/faustParameterMappers/hpLpFaustParameterMap.h"
#include "dsp/faustParameterMappers/DuckingEngineParameterMap.h"


class PluginAudioProcessor final : public juce::AudioProcessor,
                                   public juce::AudioProcessorValueTreeState::Listener
{
public:

    PluginAudioProcessor();
    ~PluginAudioProcessor() override;



void addFilterLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout,
                     juce::String prefix,
                     parametersDeclaration::Parameters::Lp& lp,
                     parametersDeclaration::Parameters::Hp& hp)
{
    auto lpParam = std::make_unique<juce::AudioParameterFloat>(

        juce::ParameterID{prefix + "_LP_FILTER_FREQ", 1}, "LP Freq Head "+prefix, 20.f, 20000.f, 20000.f);
    lp.freq = lpParam.get();
    layout.add(std::move(lpParam));

    auto hpParam = std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{prefix + "_HP_FILTER_FREQ", 1}, "HP Freq Head "+prefix, 20.f, 20000.f, 20.f);
    hp.freq = hpParam.get();
    layout.add(std::move(hpParam));

    auto hpBpParam = std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{prefix + "_HP_FILTER_BYPASS", 1}, "HP Bp Head "+prefix,1);
    hp.bypass = hpBpParam.get();
    layout.add(std::move(hpBpParam));
    auto lpBpParam = std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{prefix + "_LP_FILTER_BYPASS", 1}, "LP Bp Head "+prefix, 1);
    hp.bypass = lpBpParam.get();
    layout.add(std::move(lpBpParam));
}

// Helper for Movement Function structures
void addMovementLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout,
                       juce::String prefix,
                       parametersDeclaration::Parameters::MovementFunction& move)
{
    auto on = std::make_unique<juce::AudioParameterBool>(juce::ParameterID{prefix + "_MOVEMENT_ON", 1}, "Movement On Head "+prefix, false);
    move.movementOn = on.get();
    layout.add(std::move(on));
    auto period =  std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{prefix + "_MOVEMENT_PERIOD_DURATION", 1},
        "Movement Period Duration "+prefix, juce::StringArray{"1/64", "1/32T", "1/64D", "1/32", "1/16T", "1/32D", "1/16", "1/8T", "1/16D", "1/8", "1/4T", "1/8D", "1/4", "1/2T", "1/4D", "1/2", "1T", "1/2D", "1"},0);
    move.periodDuration = period.get();
    layout.add(std::move(period));

    auto periodNoSync = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{prefix + "_MOVEMENT_PERIOD_DURATION_NO_SYNC", 1}, "Period"+prefix, 0.1f, 10.f, 1.f);
    move.periodDurationNoSync = periodNoSync.get();
    layout.add(std::move(periodNoSync));

    auto isPeriodSync = std::make_unique<juce::AudioParameterBool>(juce::ParameterID{prefix + "_MOVEMENT_BPM_SYNC", 1}, "Period Sync "+prefix, true);
    move.periodSync = isPeriodSync.get();
    layout.add(std::move(isPeriodSync));

    auto width = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{prefix + "_MOVEMENT_WIDTH", 1}, "Width "+prefix, 0.f,1.f, 0.f);
    move.width = width.get();
    layout.add(std::move(width));

    auto widthSlave = std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ prefix + "_MOVEMENT_WIDTH_SLAVE", 1}, "Width Slave "+prefix, false);
    move.widthSlave = widthSlave.get();
    layout.add(std::move(widthSlave));

    auto periodStart = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{prefix + "_MOVEMENT_PERIOD_STARTING_POINT", 1}, "Period Start "+prefix, 0.f, 1.f, 0.f);
    move.periodStart = width.get();
    layout.add(std::move(periodStart));

    auto func = std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{prefix + "_MOVEMENT_FUNCTION", 1}, "Function"+prefix, juce::StringArray{"Sine", "Square", "Triangle"}, 0);
    move.function = func.get();
    layout.add(std::move(func));
}

void writeFaustParametersToFile() {

    std::vector<juce::String> params;

    DuckingEngine::DuckingEngine* tempDsp = new DuckingEngine::DuckingEngine ();
    DuckingEngine::MapUI* tempUI = new DuckingEngine::MapUI();

    tempDsp->buildUserInterface(tempUI);
    auto fullPathMap = tempUI->getFullpathMap();

    juce::File outputFile = juce::File::getSpecialLocation(juce::File::userDesktopDirectory)
                                .getChildFile("faust_params.txt");
    juce::FileOutputStream stream(outputFile);

    if (stream.openedOk())
    {
        int i = 0;
        for (const auto& [key, value] : fullPathMap)
        {
            auto name = key;
            params.push_back(name);

            stream.writeText(juce::String(i) + ": " + name + "\n", false, false, nullptr);
            i++;
        }
        stream.flush();
    }
    delete tempDsp;
    delete tempUI;

}

// Helper for Head structures
void addHeadLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout,
                  int headIndex,
                  parametersDeclaration::Parameters::Head& head)
{
     juce::String prefix = "HEAD_" +  juce::String(headIndex);

    auto bypass = std::make_unique<juce::AudioParameterBool>(juce::ParameterID{prefix + "_ON", 1}, "Head "+prefix+" on", true);
    head.bypass = bypass.get();
    layout.add(std::move(bypass));

    auto headGain = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{prefix + "_GAIN", 1}, "Gain Head "+prefix, -12.f, 12.f, 0.f);
    head.gain = headGain.get();
    layout.add(std::move(headGain));
    auto headPan = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{prefix + "_PAN", 1}, "Pan Head "+prefix, 0.f, 1.f, 0.5f);
    head.pan = headPan.get();
    layout.add(std::move(headPan));
     auto headFeed = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{prefix + "_FEEDBACK", 1}, "Feedback Head "+prefix, 0.f, 0.95f, 0.3f);
    head.feedBack = headFeed.get();
    layout.add(std::move(headFeed));

    auto feedbackSlave = std::make_unique<juce::AudioParameterBool>(juce::ParameterID{prefix + "_FEEDBACK_SLAVE"}, "Feedback Slave " + prefix,0 );
    head.feedbackSlave = feedbackSlave.get();
    layout.add(std::move(feedbackSlave));

    auto headTime = std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{prefix + "_TIME", 1},
        "Time Head "+prefix, juce::StringArray{"1/64", "1/32T", "1/64D", "1/32", "1/16T", "1/32D", "1/16", "1/8T", "1/16D", "1/8", "1/4T", "1/8D", "1/4", "1/2T", "1/4D", "1/2", "1T", "1/2D", "1"},0);
    head.time = headTime.get();
    layout.add(std::move(headTime));

    auto timeSlave = std::make_unique<juce::AudioParameterBool>(juce::ParameterID{prefix + "_TIME_SLAVE"}, "Time Slave " + prefix,0 );
    head.timeSlave = timeSlave.get();
    layout.add(std::move(timeSlave));

    auto headTimeNoSync = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{prefix + "_TIME_NO_SYNC", 1}, "Time Head "+prefix+" no sync",0.f, 4.f, 0.2f*(headIndex+1));
    head.timeNoSync = headTimeNoSync.get();
    layout.add(std::move(headTimeNoSync));

    // Nested calls
    addFilterLayout(layout, prefix, head.lpFilter, head.hpFilter);
    addMovementLayout(layout, prefix, head.movementFunction);
}

    void addMainParametersLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout, parametersDeclaration::Parameters& parameters) {

    // 1. Global Parameters
//    auto gain = std::make_unique<juce::AudioParameterFloat>(id::GAIN, "Gain", 0.f, 1.f, 1.f);
 //   parameters.gain = gain.get();
  //  layout.add(std::move(gain));

    auto bypass = std::make_unique<juce::AudioParameterBool>(id::BYPASS, "Bypass", false);
    parameters.bypass = bypass.get();
    layout.add(std::move(bypass));

    auto mix = std::make_unique<juce::AudioParameterFloat>(id::MIX, "Mix", 0.f, 100.f, 100.f);
    parameters.mix = mix.get();
    layout.add(std::move(mix));

    auto feedBack = std::make_unique<juce::AudioParameterFloat>(id::FEEDBACK,"Global Feedback",0.f,1.f,0.f);
    parameters.feedback = feedBack.get();
    layout.add(std::move(feedBack));

    auto inputGain = std::make_unique<juce::AudioParameterFloat>(id::INPUT_GAIN,"Input Gain",-12.f,12.f,0.f);
    parameters.inputGain = inputGain.get();
    layout.add(std::move(inputGain));

    auto outputGain = std::make_unique<juce::AudioParameterFloat>(id::OUTPUT_GAIN,"Output Gain",-12.f,12.f,0.f);
    parameters.outputGain = outputGain.get();
    layout.add(std::move(outputGain));

    auto syncTempo = std::make_unique<juce::AudioParameterBool>(id::SYNC_TEMPO,"Sync to bpm",false);
    parameters.syncTempo = syncTempo.get();
    layout.add(std::move(syncTempo));

    auto ducking = std::make_unique<juce::AudioParameterBool>(id::DUCKING,"Ducking",false);
    parameters.ducking = ducking.get();
    layout.add(std::move(ducking));
    auto duckingAttac = std::make_unique<juce::AudioParameterFloat>(id::DUCKING_ATTACK,"Ducking attack time",0.f,500.f,10.f);
    parameters.duckingAttack = duckingAttac.get();
    layout.add(std::move(duckingAttac));

    auto duckthres = std::make_unique<juce::AudioParameterFloat>(id::DUCKING_THRESHOLD,"Ducking Threshold",-60, 0,-20);
    parameters.duckingThreshold = duckthres.get();
    layout.add(std::move(duckthres));

    auto duckingRelease = std::make_unique<juce::AudioParameterFloat>(id::DUCKING_RELEASE,"Ducking release time",5.f,2000.f,100.f);
    parameters.duckingRelease = duckingRelease.get();
    layout.add(std::move(duckingRelease));

    auto duckingRatio = std::make_unique<juce::AudioParameterFloat>(id::DUCKING_RATIO,"Ducking Ratio",1.f,20.f,4.f);
    parameters.duckingRatio = duckingRatio.get();
    layout.add(std::move(duckingRatio));

    auto WIDTH = std::make_unique<juce::AudioParameterFloat>(id::WIDTH,"Width",0.f,1.f,0.f);
    parameters.width = WIDTH.get(); 
    layout.add(std::move(WIDTH));

    auto lp = std::make_unique<juce::AudioParameterFloat>(id::LP_FILTER_FREQ,"LP Freq",20.f,20000.f,20000.f);
    parameters.lpFilter.freq = lp.get();
    layout.add(std::move(lp));

    auto hp = std::make_unique<juce::AudioParameterFloat>(id::HP_FILTER_FREQ,"HP Freq",20.f,20000.f,20.f);
    parameters.hpFilter.freq = hp.get();
    layout.add(std::move(hp));

    auto hpBp = std::make_unique<juce::AudioParameterBool>(id::HP_FILTER_BYPASS,"HP Bp",1);
    parameters.hpFilter.bypass = hpBp.get();
    layout.add(std::move(hpBp));
    auto lpBp = std::make_unique<juce::AudioParameterBool>(id::LP_FILTER_BYPASS,"LP Bp",1);
    parameters.lpFilter.bypass = lpBp.get();
    layout.add(std::move(lpBp));

    auto bpm = std::make_unique<juce::AudioParameterFloat>(id::USER_BPM, "BPM", 20.f, 300.f, 120.f);
    parameters.bpm = bpm.get();
    layout.add(std::move(bpm));

    auto bpmFromHost = std::make_unique<juce::AudioParameterBool>(id::BPM_FROM_HOST, "BPM value from host", 1);
    parameters.bpmFromHost = bpmFromHost.get();
    layout.add(std::move(bpmFromHost));

    auto globalTime = std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{id::GLOBAL_TIME.getParamID(), 1},
           "Global Time Sync", juce::StringArray{"1/64", "1/32T", "1/64D", "1/32", "1/16T", "1/32D", "1/16", "1/8T", "1/16D", "1/8", "1/4T", "1/8D", "1/4", "1/2T", "1/4D", "1/2", "1T", "1/2D", "1"},0);
    parameters.globalTime = globalTime.get();
    layout.add(std::move(globalTime));


    auto globalTimeNoSync = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{id::GLOBAL_TIME_NO_SYNC.getParamID(), 1}, "Global Time no sync",0.f, 4.f, 0.2f);
    parameters.globalTimeNoSync = globalTimeNoSync.get();
    layout.add(std::move(globalTimeNoSync));

}

juce::AudioProcessorValueTreeState::ParameterLayout
createParameterLayout(parametersDeclaration::Parameters& parameters)
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    //writeFaustParametersToFile();
    addMainParametersLayout(layout, parameters);


    addFilterLayout(layout, "MAIN", parameters.lpFilter, parameters.hpFilter);

    addHeadLayout(layout, 1, parameters.head1);
    addHeadLayout(layout, 2, parameters.head2);
    addHeadLayout(layout, 3, parameters.head3);
    addHeadLayout(layout, 4, parameters.head4);

    return layout;
}
float getTimeFromIndex(float index);

    bool setTempoSync(const juce::String& parameterID, float newValue) {
        if (parameterID.contains("TIME") && !parameterID.contains("NO_SYNC")) {

            mFaustUI->setParamValue(FaustParameterMapping::getFaustPath(parameterID), getTimeFromIndex(newValue));
            return true;
        }
        return false;
    }

    void syncTempoToggled(bool isActive) {
        if (isActive) {
            mFaustUI->setParamValue(FaustParameterMapping::getFaustPath(id::HEAD_1_TIME.getParamID()), getTimeFromIndex(mParameters.getRawParameterValue(id::HEAD_1_TIME.getParamID())->load()));
            mFaustUI->setParamValue(FaustParameterMapping::getFaustPath(id::HEAD_2_TIME.getParamID()), getTimeFromIndex(mParameters.getRawParameterValue(id::HEAD_2_TIME.getParamID())->load()));
            mFaustUI->setParamValue(FaustParameterMapping::getFaustPath(id::HEAD_3_TIME.getParamID()), getTimeFromIndex(mParameters.getRawParameterValue(id::HEAD_3_TIME.getParamID())->load()));
            mFaustUI->setParamValue(FaustParameterMapping::getFaustPath(id::HEAD_4_TIME.getParamID()), getTimeFromIndex(mParameters.getRawParameterValue(id::HEAD_4_TIME.getParamID())->load()));

        } else {
            mFaustUI->setParamValue(FaustParameterMapping::getFaustPath(id::HEAD_1_TIME_NO_SYNC.getParamID()), mParameters.getRawParameterValue(id::HEAD_1_TIME_NO_SYNC.getParamID())->load());
            mFaustUI->setParamValue(FaustParameterMapping::getFaustPath(id::HEAD_2_TIME_NO_SYNC.getParamID()), mParameters.getRawParameterValue(id::HEAD_2_TIME_NO_SYNC.getParamID())->load());
            mFaustUI->setParamValue(FaustParameterMapping::getFaustPath(id::HEAD_3_TIME_NO_SYNC.getParamID()), mParameters.getRawParameterValue(id::HEAD_3_TIME_NO_SYNC.getParamID())->load());
            mFaustUI->setParamValue(FaustParameterMapping::getFaustPath(id::HEAD_4_TIME_NO_SYNC.getParamID()), mParameters.getRawParameterValue(id::HEAD_4_TIME_NO_SYNC.getParamID())->load());

        }
    }

    bool updateGlobalTimeParameters(const juce::String& parameterID, float newValue) ;
    bool updateGlobalFeedBackParameters(const juce::String& parameterID, float newValue);

    bool updateGlobalWidthParameters(const juce::String& parameterID, float newValue);

void parameterChanged(const juce::String& parameterID, float newValue) override
{
        if (parameterID == "SYNC_TEMPO") {
        if (mFaustUI != nullptr && newValue > 0.5f) {
            syncTempoToggled(true);
        } else {
            syncTempoToggled(false);
        }
        return;
    }

    if (updateGlobalFeedBackParameters(parameterID, newValue)) return;
    updateGlobalTimeParameters(parameterID, newValue);

    if (updateGlobalWidthParameters(parameterID, newValue)) return;

    if (parameterID == "BPM_FROM_HOST") {
        if (newValue > 0.5f) {
            // 1. Get the actual parameter object
            if (auto* bpmParam = mParameters.getParameter(id::USER_BPM.getParamID())) {

                // 2. Convert the "real" BPM (e.g. 120) to normalized (0.0 -> 1.0)
                float hostBpm = static_cast<float>(currentBpm.load());
                float normalizedBpm = mParameters.getParameterRange(id::USER_BPM.getParamID())
                                                 .convertTo0to1(hostBpm);

                // 3. Update it so the UI and Host both know
                bpmParam->setValueNotifyingHost(normalizedBpm);
            }
        }
    }

    updateSecondaryEngines(parameterID, newValue);

    if (mFaustUI == nullptr) return;

    bool isSynced = mParameters.getRawParameterValue("SYNC_TEMPO")->load() > 0.5f;
    if (isSynced && setTempoSync(parameterID, newValue)) {
        return;
    }

    auto faustPath = FaustParameterMapping::getFaustPath(parameterID);
    if (!faustPath.empty()) {
        float finalValue = newValue;

        if (parameterID.endsWith("_GAIN")) {
            finalValue = juce::Decibels::decibelsToGain(newValue);
        }

        mFaustUI->setParamValue(faustPath, finalValue);
    }
}

void updateSecondaryEngines(const juce::String& parameterID, float newValue) {
    if (mFaustHpLpUI != nullptr) {
        auto path = FaustParameterMapping::getHpLpPath(parameterID);
        if (!path.empty()) mFaustHpLpUI->setParamValue(path, newValue);
    }

    if (mFaustDuckingUI != nullptr) {
        auto path = FaustParameterMapping::getDuckingEnginePath(parameterID);
        if (!path.empty()) mFaustDuckingUI->setParamValue(path, newValue);
    }
}

void prepareToPlay (double sampleRate, int blockSize) override
{
    if (!mFaustUI) mFaustUI = std::make_unique<MapUI>();
    if (!mFaustHpLpUI) mFaustHpLpUI = std::make_unique<HpLpFilter::MapUI>();
    if (!mFaustDuckingUI) mFaustDuckingUI = std::make_unique<DuckingEngine::MapUI>();

    mSkeletonProcessor.setMapUI(mFaustUI.get());
    mSkeletonProcessor.setMapUIHpLp(mFaustHpLpUI.get());
    mSkeletonProcessor.setMapUIDucking(mFaustDuckingUI.get());

    mSkeletonProcessor.prepareToPlay(sampleRate, blockSize);

    mParameterSetup.setFaustUI(getFaustUI());
    for (auto* param : getParameters())
    {
        if (auto* p = dynamic_cast<juce::AudioProcessorParameterWithID*>(param))
        {
           juce::String paramID = p->getParameterID();

            float currentValue = p->getValue();

            std::string faustPath = FaustParameterMapping::getFaustPath(paramID);
            if (!faustPath.empty())
            {
                mFaustUI->setParamValue(faustPath, currentValue);
            }
            // TODO : faire pour tous les faust engines
        }
    }
}

    void releaseResources() override {
    }

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override {
        auto editor = new VueProcessorEditor(mSkeletonProcessor);
        return editor;
    }
    bool hasEditor() const override                        { return true;   }

    const juce::String getName() const override                  { return "Template"; }
    bool acceptsMidi() const override                      { return false; }
    bool producesMidi() const override                     { return false; }
    double getTailLengthSeconds() const override           { return 0; }

    int getNumPrograms() override                          { return 1; }
    int getCurrentProgram() override                       { return 0; }
    void setCurrentProgram (int) override                  {}
    const juce::String getProgramName (int) override             { return "None"; }
    void changeProgramName (int, const juce::String&) override   {}


    void getStateInformation (juce::MemoryBlock& destData) override
    {
        juce::MemoryOutputStream stream(destData, true);
        mParameters.state.writeToStream(stream);  
    }

    void PluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes) override
    {
        const auto xml = getXmlFromBinary(data, sizeInBytes); // JUCE helper
        if (xml == nullptr) return;

        const auto newState = juce::ValueTree::fromXml(*xml);
        if (!newState.isValid()) return;

        // Restore the preset name if present
        if (newState.hasProperty("currentPreset"))
            mPresetManager->loadPreset(newState["currentPreset"].toString());
        else
            mParameters.replaceState(newState);
    }

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
    {
        const auto& mainInLayout  = layouts.getChannelSet (true,  0);
        const auto& mainOutLayout = layouts.getChannelSet (false, 0);

        return (mainInLayout == mainOutLayout && (! mainInLayout.isDisabled()));
    }

    juce::AudioProcessorValueTreeState& getCustomParameterTree()
    {
        return mParameters;
    }
    MapUI* getFaustUI() {
        return mFaustUI.get();
    }

private:
    void updateMovmentHeadPosition(int inHeadNumber, juce::AudioPlayHead* playHead);
    void updateMovmentPosition() ;

private:
    parametersDeclaration::Parameters parametersDeclaration;
    std::atomic<double> currentBpm { 120.0 };
    // Use unique_ptr for automatic memory management
    std::unique_ptr<MapUI> mFaustUI;
    std::unique_ptr<HpLpFilter::MapUI> mFaustHpLpUI;
    std::unique_ptr<DuckingEngine::MapUI> mFaustDuckingUI;

    juce::AudioProcessorValueTreeState mParameters;
    std::unique_ptr<PresetManager> mPresetManager;
    SkeletonAudioProcessor mSkeletonProcessor;
    ParameterSetup mParameterSetup;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginAudioProcessor)
};
