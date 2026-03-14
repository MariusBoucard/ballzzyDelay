

#include "Processor.h"




SkeletonAudioProcessor::SkeletonAudioProcessor(juce::AudioProcessorValueTreeState& inParameters, ParameterSetup& inParameterSetup, parametersDeclaration::Parameters inParametersDeclaration)
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo())
        .withOutput("Output", juce::AudioChannelSet::stereo()))
    , mParameters(inParameters)
    , mParameterSetup(inParameterSetup)
    , mBlockSize(256)
    , mSampleRate(44100)
    , mFaustProcessor()
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

    // 1. Sécurité : Vérifiez que vos buffers internes sont assez grands
    // Ces vérifications devraient idéalement être faites dans prepareToPlay
    if (numSamples > mBlockSize) {
        // Log d'erreur ou redimensionnement d'urgence (attention aux perfs)
        return;
    }

    // 2. Remplissage des entrées Faust
    for (int ch = 0; ch < numIn; ++ch) {
        auto* channelData = inBuffer.getReadPointer(ch);
        for (int i = 0; i < numSamples; ++i) {
            inputs[ch][i] = channelData[i];
        }
    }

    // 3. Calcul Faust (Vérifiez si Faust attend des entrées ET sorties)
    // Note : On passe souvent 'inputs' au lieu de NULL si Faust a des entrées
    mFaustProcessor->compute(numSamples, inputs, outputs);

    // 4. Récupération des sorties
    for (int ch = 0; ch < numOut; ++ch) {
        auto* channelData = inBuffer.getWritePointer(ch);
        for (int i = 0; i < numSamples; ++i) {
            channelData[i] = outputs[ch][i]; // Correction ici : On écrit la SORTIE Faust
        }
    }
}