

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

// TODO
void SkeletonAudioProcessor::updateMeter(bool isOutput, juce::AudioBuffer<float>& buffer, int numChannels)
{
    if (isOutput)
    {
        if (numChannels > 0)
            mRmsOutputLevelLeft.store(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
        if (numChannels > 1)
            mRmsOutputLevelRight.store(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
    }
    else
    {
        if (numChannels > 0)
            mRmsLevelLeft.store(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
        if (numChannels > 1)
            mRmsLevelRight.store(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
    }
}

void SkeletonAudioProcessor::processBlock(juce::AudioBuffer<float>& inBuffer, juce::MidiBuffer& inMidiBuffer)
{
    juce::ScopedNoDenormals noDenormals;
    const int numSamples = inBuffer.getNumSamples();
    const int numIn = getTotalNumInputChannels();
    const int numOut = getTotalNumOutputChannels();

    if (numSamples > mBlockSize || mFaustProcessor == nullptr) {
        return;
    }

    const float inGain = mParameters.getRawParameterValue(id::INPUT_GAIN.getParamID())->load();
    const float outGain = mParameters.getRawParameterValue(id::OUTPUT_GAIN.getParamID())->load();
    const float mixAmount = mParameters.getRawParameterValue(id::MIX.getParamID())->load() / 100.0f;

    juce::AudioBuffer<float> dryBuffer;
    dryBuffer.makeCopyOf(inBuffer);

    inBuffer.applyGain(juce::Decibels::decibelsToGain ((float)inGain));

    updateMeter(false, inBuffer, numIn);

    for (int ch = 0; ch < numIn; ++ch) {
        auto* channelReadPtr = inBuffer.getReadPointer(ch);
        for (int i = 0; i < numSamples; ++i) {
            inputs[ch][i] = channelReadPtr[i];
        }
    }
    mFaustLPHpProcessor->compute(numSamples, inputs, postHpLp);
    mFaustProcessor->compute(numSamples, postHpLp, outputs);

    for (int i = 0; i < numSamples; ++i) {
        duckingInput[0][i] = outputs[0][i];
        duckingInput[1][i] = outputs[1][i];

        duckingInput[2][i] = inputs[0][i];
        duckingInput[3][i] = inputs[1][i];
    }
    mFaustDuckingProcessor->compute(numSamples, duckingInput, outputs);

    for (int ch = 0; ch < numOut; ++ch) {
        auto* channelWritePtr = inBuffer.getWritePointer(ch);
        auto* dryReadPtr = dryBuffer.getReadPointer(ch < numIn ? ch : 0);

        for (int i = 0; i < numSamples; ++i) {
            float wetSample = outputs[ch][i];
            channelWritePtr[i] = (dryReadPtr[i] * (1.0f - mixAmount)) + (wetSample * mixAmount);
        }
    }
    inBuffer.applyGain(juce::Decibels::decibelsToGain ((float)outGain));

    updateMeter(true, inBuffer, numOut);
}