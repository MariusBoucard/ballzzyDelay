#include "PluginAudioProcessor.h"
//#include "dsp/ParameterSetup.h"



PluginAudioProcessor::PluginAudioProcessor()
    : juce::AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo())
          .withOutput("Output", juce::AudioChannelSet::stereo()))
      , mParameters{*this, nullptr, "PARAMETERS", createParameterLayout(parametersDeclaration)}
      , mParameterSetup(mParameters)
      , mSkeletonProcessor(mParameters, mParameterSetup, parametersDeclaration)
{
    for (auto* param : mParameters.processor.getParameters())
    {
        auto paramID =static_cast<juce::AudioProcessorParameterWithID*>(param)->paramID;
        mParameters.addParameterListener(paramID, this);
    }
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
    if (auto* playHead = getPlayHead()) {
        auto postion = playHead->getPosition();
        if (postion.hasValue() && postion->getBpm().hasValue()) {
            // Store it for the UI/other methods to use
            currentBpm.store(*postion->getBpm());
        }
    }

    mSkeletonProcessor.processBlock(buffer, a);
}
