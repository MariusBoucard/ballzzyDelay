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
        mParameters.addParameterListener(param->getName(152), this);
    }
}

PluginAudioProcessor::~PluginAudioProcessor() {
    for (auto* param : getParameters())
    {
        mParameters.removeParameterListener(param->getName(152), this);
    }
}

void PluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &a) {
    juce::RangedAudioParameter *name = mParameters.getParameter("GAIN");
    mSkeletonProcessor.processBlock(buffer, a);
}
