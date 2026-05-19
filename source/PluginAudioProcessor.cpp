#include "PluginAudioProcessor.h"
//#include "dsp/ParameterSetup.h"



PluginAudioProcessor::PluginAudioProcessor()
    : juce::AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo())
          .withOutput("Output", juce::AudioChannelSet::stereo()))
      , mParameters{*this, nullptr, "PARAMETERS", createParameterLayout(parametersDeclaration)}
      , mPresetManager { std::make_unique<PresetManager>(mParameters) }
      , mParameterSetup(mParameters)
      , mSkeletonProcessor(mParameters, mParameterSetup, parametersDeclaration, *mPresetManager)

{
    for (auto* param : mParameters.processor.getParameters())
    {
        auto paramID = static_cast<juce::AudioProcessorParameterWithID*>(param)->paramID;
        mParameters.addParameterListener(paramID, this);
    }

    // Initialize the parameter listeners for movement params
    mParameterSetup.initParametersListener(*this);
}


PluginAudioProcessor::~PluginAudioProcessor() {
    for (auto* param : getParameters())
    {
        auto paramID =static_cast<juce::AudioProcessorParameterWithID*>(param)->paramID;
        mParameters.removeParameterListener(paramID, this);;
    }
    // Carefull we should delete the editor before the processor
}

void PluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &a) {
    auto* playHead = getPlayHead();
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
    // Get the position of the playhead
    juce::AudioPlayHead* playHead = getPlayHead();


    // do the update for each heads
    for (int i = 0; i < 4; ++i) {
        updateMovmentHeadPosition(i, playHead);
    }
}

void PluginAudioProcessor::updateMovmentHeadPosition(int inHeadNumber, juce::AudioPlayHead* playHead) {
    if (playHead == nullptr)
        return;

    auto position = playHead->getPosition();
    if (!position.hasValue())
        return;

    if (position->getTimeInSeconds().hasValue()) {
        mParameterSetup.setPlayTime(*position->getTimeInSeconds(), currentBpm.load());
    }
}