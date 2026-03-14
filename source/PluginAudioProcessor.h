
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

class PluginAudioProcessor final : public juce::AudioProcessor,
                                   public juce::AudioProcessorValueTreeState::Listener
{
public:

    PluginAudioProcessor();
    ~PluginAudioProcessor() override;
    juce::AudioProcessorValueTreeState::ParameterLayout
 createParameterLayout(
     parametersDeclaration::Parameters& parameters) {
        using namespace juce;
        AudioProcessorValueTreeState::ParameterLayout layout;

        {
            auto parameter = std::make_unique<AudioParameterFloat>(
                id::GAIN, "gain", NormalisableRange<float>{0.f, 1.f, 0.01f, 0.9f}, 1.f);
            parameters.gain = parameter.get();
            layout.add(std::move(parameter));
        }

        {
            auto parameter = std::make_unique<AudioParameterBool>(
                id::BYPASS, "bypass", false,
                AudioParameterBoolAttributes{}.withLabel("Bypass"));
            parameters.bypass = parameter.get();
            layout.add(std::move(parameter));
        }

        {
            auto parameter = std::make_unique<AudioParameterChoice>(
                id::DISTORTION_TYPE, "distortion type",
                StringArray{"none", "tanh(kx)/tanh(k)", "sigmoid"}, 0);
            parameters.distortionType = parameter.get();
            layout.add(std::move(parameter));
        }

        return layout;
    }

    void parameterChanged (const juce::String& parameterID, float newValue) override
    {
        if (mFaustUI != nullptr)
            mFaustUI->setParamValue(parameterID.toStdString(), newValue);
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
