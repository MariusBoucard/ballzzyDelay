

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
    
    // Get parameters
    auto* bypassParam = mParameters.getRawParameterValue(id::BYPASS.getParamID());
    auto* inputGainParam = mParameters.getRawParameterValue(id::INPUT_GAIN.getParamID());
    auto* outputGainParam = mParameters.getRawParameterValue(id::OUTPUT_GAIN.getParamID());
    auto* mixParam = mParameters.getRawParameterValue(id::MIX.getParamID());
    auto* duckingParam = mParameters.getRawParameterValue(id::DUCKING.getParamID());
    
    // 1. Safety check
    if (numSamples > mBlockSize) {
        return;
    }
    
    // 2. Check bypass
    if (bypassParam && *bypassParam > 0.5f) {
        return; // Pass audio through unchanged
    }
    
    // 3. Store dry signal for mix (before processing)
    std::vector<std::vector<float>> drySignal(numIn);
    for (int ch = 0; ch < numIn; ++ch) {
        drySignal[ch].resize(numSamples);
        auto* channelData = inBuffer.getReadPointer(ch);
        std::copy(channelData, channelData + numSamples, drySignal[ch].begin());
    }
    
    // 4. Apply input gain and fill Faust inputs
    float inputGain = inputGainParam ? inputGainParam->load() : 1.0f;
    for (int ch = 0; ch < numIn; ++ch) {
        auto* channelData = inBuffer.getReadPointer(ch);
        for (int i = 0; i < numSamples; ++i) {
            inputs[ch][i] = channelData[i] * inputGain;
        }
    }
    
    // 5. Faust processing
    mFaustProcessor->compute(numSamples, inputs, outputs);
    
    // 6. Apply ducking if enabled
    bool duckingEnabled = duckingParam && (*duckingParam > 0.5f);
    if (duckingEnabled && mFaustDuckingProcessor) {
        // Ducking processes the Faust output
        // Copy outputs to a temp buffer, then duck it
        for (int ch = 0; ch < numOut; ++ch) {
            for (int i = 0; i < numSamples; ++i) {
                inputs[ch][i] = outputs[ch][i];
            }
        }
        mFaustDuckingProcessor->compute(numSamples, inputs, outputs);
    }
    
    // 7. Apply mix (blend dry and wet signals)
    float mixValue = mixParam ? mixParam->load() : 1.0f; // 0.0 = full dry, 1.0 = full wet
    
    // 8. Apply output gain and write to buffer
    float outputGain = outputGainParam ? outputGainParam->load() : 1.0f;
    
    for (int ch = 0; ch < numOut; ++ch) {
        auto* channelData = inBuffer.getWritePointer(ch);
        for (int i = 0; i < numSamples; ++i) {
            float wet = outputs[ch][i] * outputGain;
            float dry = (ch < numIn) ? drySignal[ch][i] : 0.0f;
            channelData[i] = dry * (1.0f - mixValue) + wet * mixValue;
        }
    }
}