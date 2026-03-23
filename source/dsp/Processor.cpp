

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

    // --- PARAMETER RETRIEVAL ---
    // Note: If these are 0.0 to 1.0, we use them as multipliers.
    // If MIX is 0 to 100, we divide by 100.
    const float inGain = mParameters.getRawParameterValue(id::INPUT_GAIN.getParamID())->load();
    const float outGain = mParameters.getRawParameterValue(id::OUTPUT_GAIN.getParamID())->load();
    const float mixAmount = mParameters.getRawParameterValue(id::MIX.getParamID())->load() / 100.0f; // 0.0 to 1.0

    // We create a copy of the dry input for the Mix at the end
    juce::AudioBuffer<float> dryBuffer;
    dryBuffer.makeCopyOf(inBuffer);

    // 2. Apply Input Gain & Fill Faust Inputs
    for (int ch = 0; ch < numIn; ++ch) {
        auto* channelReadPtr = inBuffer.getReadPointer(ch);
        for (int i = 0; i < numSamples; ++i) {
            // Apply Input Gain before Faust processing
            inputs[ch][i] = channelReadPtr[i] * inGain;
        }
    }

    // 3. Faust Processing
    mFaustProcessor->compute(numSamples, inputs, outputs);

    // 4. Combine: Output Gain + Dry/Wet Mix
    for (int ch = 0; ch < numOut; ++ch) {
        auto* channelWritePtr = inBuffer.getWritePointer(ch);
        auto* dryReadPtr = dryBuffer.getReadPointer(ch < numIn ? ch : 0);

        for (int i = 0; i < numSamples; ++i) {
            // A. Apply Output Gain to the Faust result
            float wetSample = outputs[ch][i] * outGain;

            // B. Linear Mix formula: Out = (Dry * (1 - Mix)) + (Wet * Mix)
            channelWritePtr[i] = (dryReadPtr[i] * (1.0f - mixAmount)) + (wetSample * mixAmount);
        }
    }
}