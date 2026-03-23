

#include "Processor.h"

#include "ParameterIDs.hpp"


SkeletonAudioProcessor::SkeletonAudioProcessor(juce::AudioProcessorValueTreeState& inParameters, ParameterSetup& inParameterSetup, parametersDeclaration::Parameters inParametersDeclaration)
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo())
        .withOutput("Output", juce::AudioChannelSet::stereo()))
    , mParameters(inParameters)
    , mParameterSetup(inParameterSetup)
    , mBlockSize(256)
    , mSampleRate(44100)
    , mFaustProcessor()
    , mFaustLPHpProcessor()
    , mFaustDuckingProcessor()
    , mParametersDeclaration(inParametersDeclaration)
{
    setRateAndBufferSizeDetails(mSampleRate, mBlockSize);
  //  initialiseGraph();
}
SkeletonAudioProcessor::~SkeletonAudioProcessor()
{

}  
void SkeletonAudioProcessor::updateMeter(bool isOutput, juce::AudioBuffer<float>& buffer,int numSamples,int numChannels)
{
    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = buffer.getReadPointer(channel);
        float sum = 0.0f;

        for (int i = 0; i < numSamples; ++i)
        {
            sum += channelData[i] * channelData[i];
        }

        float rms = std::sqrt(sum / numSamples);

        if (channel == 0)
        {
            mRmsLevelLeft.store(rms);
            mRmsLevelRight.store(rms);
        }
    }
}

void SkeletonAudioProcessor::processBlock(juce::AudioBuffer<float>& inBuffer, juce::MidiBuffer& inMidiBuffer)
{
    juce::ScopedNoDenormals noDenormals;
    const int numSamples = inBuffer.getNumSamples();
    const int numIn = getTotalNumInputChannels();
    const int numOut = getTotalNumOutputChannels();
    
    // 1. Safety Check
    if (numSamples > mBlockSize || mFaustProcessor == nullptr) {
        return;
    }

    // 2. Fill Faust Inputs from JUCE Buffer
    // We manually copy to ensure the memory alignment is exactly what Faust expects
    for (int ch = 0; ch < numIn; ++ch) {
        auto* channelReadPtr = inBuffer.getReadPointer(ch);
        for (int i = 0; i < numSamples; ++i) {
            inputs[ch][i] = channelReadPtr[i];
        }
    }

    // 3. Faust Processing (The Core Logic)
    // This expects 2 inputs and produces 2 outputs based on your Faust 'process'
    mFaustProcessor->compute(numSamples, inputs, outputs);

    // 4. Overwrite JUCE Buffer with Faust Outputs (100% Wet)
    for (int ch = 0; ch < numOut; ++ch) {
        auto* channelWritePtr = inBuffer.getWritePointer(ch);
        for (int i = 0; i < numSamples; ++i) {
            // We replace the buffer content entirely with the processed signal
            channelWritePtr[i] = outputs[ch][i];
        }
    }
}