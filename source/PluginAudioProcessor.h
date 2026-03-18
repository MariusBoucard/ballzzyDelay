
/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             GainPlugin
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Gain audio plugin.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_plugin_client, juce_audio_processors,
                   juce_audio_utils, juce_core, juce_data_structures,
                   juce_events, juce_graphics, juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2022

 moduleFlags:      JUCE_STRICT_REFCOUNTEDPOINTER=1

 type:             AudioProcessor
 mainClass:        GainProcessor

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_processors_headless/juce_audio_processors_headless.h>
#include "ui/gui.h"
#include "dsp/Processor.h"
#include "dsp/ParameterSetup.h"
#include "dsp/ParameterIDs.hpp"
#include "dsp/paramsDeclaration.h"
#include "dsp/faustParameterMap.h"

class PluginAudioProcessor final : public juce::AudioProcessor,
                                   public juce::AudioProcessorValueTreeState::Listener
{
public:

    PluginAudioProcessor();
    ~PluginAudioProcessor() override;
//#include "PluginAudioProcessor.hpp"



void addFilterLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout,
                     juce::String prefix,
                     parametersDeclaration::Parameters::Lp& lp,
                     parametersDeclaration::Parameters::Hp& hp)
{
    auto lpParam = std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{prefix + "_LP_FILTER_FREQ", 1}, "LP Freq Head "+prefix, juce::NormalisableRange<float>{20.f, 20000.f, 1.f, 0.3f}, 20000.f);
    lp.freq = lpParam.get();
    layout.add(std::move(lpParam));

    auto hpParam = std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{prefix + "_HP_FILTER_FREQ", 1}, "HP Freq Head "+prefix, juce::NormalisableRange<float>{20.f, 20000.f, 1.f, 0.3f}, 20.f);
    hp.freq = hpParam.get();
    layout.add(std::move(hpParam));
}

// Helper for Movement Function structures
void addMovementLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout,
                       juce::String prefix,
                       parametersDeclaration::Parameters::MovementFunction& move)
{
    auto on = std::make_unique<juce::AudioParameterBool>(juce::ParameterID{prefix + "_MOVEMENT_ON", 1}, "Movement On Head "+prefix, false);
    move.movementOn = on.get();
    layout.add(std::move(on));

    auto period = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{prefix + "_MOVEMENT_PERIOD_DURATION", 1}, "Period"+prefix, 0.1f, 10.f, 1.f);
    move.periodDuration = period.get();
    layout.add(std::move(period));
    auto width = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{prefix + "_MOVEMENT_WIDTH", 1}, "Width "+prefix, 0.f,1.f, 0.f);
    move.width = width.get();
    layout.add(std::move(width));

    auto func = std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{prefix + "_MOVEMENT_FUNCTION", 1}, "Function"+prefix, juce::StringArray{"Sine", "Saw", "Square"}, 0);
    move.function = func.get();
    layout.add(std::move(func));
}
void writeFaustParametersToFile() {

    std::vector<juce::String> params;

    mydsp* tempDsp = new mydsp();
    MapUI* tempUI = new MapUI();

    tempDsp->buildUserInterface(tempUI);
    auto fullPathMap = tempUI->getFullpathMap();

    // Write to file
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

    auto bypass = std::make_unique<juce::AudioParameterBool>(juce::ParameterID{prefix + "_BYPASS", 1}, "Bypass Head "+prefix, false);
    head.bypass = bypass.get();
    layout.add(std::move(bypass));

    auto headGain = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{prefix + "_GAIN", 1}, "Gain Head "+prefix, 0.f, 1.f, 1.f);
    head.gain = headGain.get();
    layout.add(std::move(headGain));

    auto headPan = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{prefix + "_PAN", 1}, "Pan Head "+prefix, 0.f, 1.f, 0.5f);
    head.pan = headPan.get();
    layout.add(std::move(headPan));

    auto headFeed = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{prefix + "_FEEDBACK", 1}, "Feedback Head "+prefix, 0.f, 1.f, 0.5f);
    head.feedBack = headFeed.get();
    layout.add(std::move(headFeed));

    auto headTime = std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{prefix + "_TIME", 1}, "Time Head "+prefix, 0.f, 1.f, 0.5f);
    head.time = headTime.get();
    layout.add(std::move(headTime));

    // Nested calls
    addFilterLayout(layout, prefix, head.lpFilter, head.hpFilter);
    addMovementLayout(layout, prefix + "_MV", head.movementFunction);
}

    void addMainParametersLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout, parametersDeclaration::Parameters& parameters) {

    // 1. Global Parameters
    auto gain = std::make_unique<juce::AudioParameterFloat>(id::GAIN, "Gain", 0.f, 1.f, 1.f);
    parameters.gain = gain.get();
    layout.add(std::move(gain));

    auto bypass = std::make_unique<juce::AudioParameterBool>(id::BYPASS, "Bypass", false);
    parameters.bypass = bypass.get();
    layout.add(std::move(bypass));

    auto mix = std::make_unique<juce::AudioParameterFloat>(id::MIX, "Mix", 0.f, 100.f, 100.f);
    parameters.mix = mix.get();
    layout.add(std::move(mix));

    auto feedBack = std::make_unique<juce::AudioParameterFloat>(id::FEEDBACK,"Global Feedback",0.f,1.f,0.f);
    parameters.feedback = feedBack.get();
    layout.add(std::move(feedBack));

    auto time = std::make_unique<juce::AudioParameterFloat>(id::TIME,"Global Feedback",0.f,1.f,0.f);
    parameters.time = time.get();
    layout.add(std::move(time));

    auto inputGain = std::make_unique<juce::AudioParameterFloat>(id::INPUT_GAIN,"Input Gain",0.f,1.f,0.f);
    parameters.inputGain = inputGain.get();
    layout.add(std::move(inputGain));

    auto outputGain = std::make_unique<juce::AudioParameterFloat>(id::OUTPUT_GAIN,"Output Gain",0.f,1.f,0.f);
    parameters.outputGain = outputGain.get();
    layout.add(std::move(outputGain));

    auto syncTempo = std::make_unique<juce::AudioParameterBool>(id::SYNC_TEMPO,"Sync to bpm",false);
    parameters.syncTempo = syncTempo.get();
    layout.add(std::move(syncTempo));

    auto ducking = std::make_unique<juce::AudioParameterFloat>(id::DUCKING,"Ducking",0.f,1.f,0.f);
    parameters.ducking = ducking.get();
    layout.add(std::move(ducking));

    auto duckingAttac = std::make_unique<juce::AudioParameterFloat>(id::DUCKING_ATTACK,"Ducking attack time",0.f,1.f,0.f);
    parameters.duckingAttack = duckingAttac.get();
    layout.add(std::move(duckingAttac));
    auto duckthres = std::make_unique<juce::AudioParameterFloat>(id::DUCKING_THRESHOLD,"Ducking Threshold",0.f,1.f,0.f);
    parameters.duckingThreshold = duckthres.get();
    layout.add(std::move(duckthres));

    auto duckingRelease = std::make_unique<juce::AudioParameterFloat>(id::DUCKING_RELEASE,"Ducking attack time",0.f,1.f,0.f);
    parameters.duckingRelease = duckingRelease.get();
    layout.add(std::move(duckingRelease));

    auto WIDTH = std::make_unique<juce::AudioParameterFloat>(id::WIDTH,"Width",0.f,1.f,0.f);
    parameters.width = WIDTH.get(); 
    layout.add(std::move(WIDTH));

    auto lp = std::make_unique<juce::AudioParameterFloat>(id::LP_FILTER_FREQ,"LP Freq",20.f,20000.f,20000.f);
    parameters.lpFilter.freq = lp.get();
    layout.add(std::move(lp));

    auto hp = std::make_unique<juce::AudioParameterFloat>(id::HP_FILTER_FREQ,"HP Freq",20.f,20000.f,20.f);
    parameters.hpFilter.freq = hp.get();
    layout.add(std::move(hp));

}

juce::AudioProcessorValueTreeState::ParameterLayout
createParameterLayout(parametersDeclaration::Parameters& parameters)
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    addMainParametersLayout(layout, parameters);


    // 2. Main Filters
    addFilterLayout(layout, "MAIN", parameters.lpFilter, parameters.hpFilter);

    // 3. Heads
    addHeadLayout(layout, 1, parameters.head1);
    addHeadLayout(layout, 2, parameters.head2);
    addHeadLayout(layout, 3, parameters.head3);
    addHeadLayout(layout, 4, parameters.head4);

    return layout;
}


    void parameterChanged (const juce::String& parameterID, float newValue) override
    {
        if (mFaustUI != nullptr)
            mFaustUI->setParamValue(FaustParameterMapping::getFaustPath(parameterID), newValue);
    }

    void prepareToPlay (double sampleRate, int blockSize) override {
        mFaustUI = new MapUI(); // Pas bon ca
        mSkeletonProcessor.setMapUI(mFaustUI);
        mSkeletonProcessor.prepareToPlay(sampleRate, blockSize);
    }

    void releaseResources() override {
      //  delete mFaustUI;
    }

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override {
        //auto editor = new RootViewComponent(mSkeletonProcessor);
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

    juce::AudioProcessorValueTreeState& getState() { return mParameters; }
    juce::AudioParameterChoice& getDistortionTypeParameter() const {
        return *dynamic_cast<juce::AudioParameterChoice*>(mParameters.getParameter(id::DISTORTION_TYPE.getParamID()));
    }
    void setStateInformation (const void* data, int sizeInBytes) override
    {

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
public:


private:
    parametersDeclaration::Parameters parametersDeclaration;
    MapUI* mFaustUI;
    juce::AudioProcessorValueTreeState mParameters;
    SkeletonAudioProcessor mSkeletonProcessor;
    ParameterSetup mParameterSetup;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginAudioProcessor)
};
