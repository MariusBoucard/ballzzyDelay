#pragma once
#include <juce_audio_processors_headless/juce_audio_processors_headless.h>

#include "ParameterSetup.h"
#include "paramsDeclaration.h"
#include "Bones/GainBone.h"
#include "Processor.hpp"
#include "Bones/FaustMultiheadFeedback.h"

//==============================================================================
class SkeletonAudioProcessor final : public juce::AudioProcessor {
public:
    //==============================================================================
    SkeletonAudioProcessor(juce::AudioProcessorValueTreeState &inParameters, ParameterSetup &inParameterSetup, parametersDeclaration::Parameters inParametersDeclaration);


    ~SkeletonAudioProcessor() override;


    void prepareToPlay(double inSampleRate, int inBlockSize) override {
        mSampleRate = inSampleRate;
        mBlockSize = inBlockSize;
        mParameterSetup.initParametersListener(*this);

        Mappers::getMapperInstance().setSampleRate(mSampleRate);
        mProcessorGraph.prepareToPlay(mSampleRate, mBlockSize);

        mFaustProcessor = new mydsp();
        mFaustProcessor->init(mSampleRate);
        mFaustProcessor->buildUserInterface(fUI);

        // TODO CHANGE TO THE RIGHT DSP
        mFaustDuckingProcessor = new mydsp();
        mFaustDuckingProcessor->init(mSampleRate);
        mFaustDuckingProcessor->buildUserInterface(fDuckingUI);

        mFaustLPHpProcessor = new mydsp();
        mFaustLPHpProcessor->init(mSampleRate);
        mFaustLPHpProcessor->buildUserInterface(fLpHpUi);

        inputs = new float*[2];
        for (int channel = 0; channel < 2; ++channel) {
            inputs[channel] = new float[mBlockSize];
        }
        outputs = new float*[2];
        for (int channel = 0; channel < 2; ++channel) {
            outputs[channel] = new float[mBlockSize];
        }
    }

    void releaseResources() override {
        mProcessorGraph.releaseResources();
       // delete mFaustProcessor; // TODO THIS CAUSES CRASH ON EXIT
        delete fUI;
        delete fDuckingUI;
        delete fLpHpUi;
        delete mFaustDuckingProcessor;
        delete mFaustLPHpProcessor;
        delete mFaustProcessor;
        for (int channel = 0; channel < 2; ++channel) {
            delete[] outputs[channel];
        }
        delete [] outputs;
        for (int channel = 0; channel < 2; ++channel) {
            delete[] inputs[channel];
        }
        delete[] inputs;
    }
    juce::AudioProcessorValueTreeState& getState() { return mParameters;}
    void processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &) override;

    void updateMeter(bool isOutput, juce::AudioBuffer<float> &buffer, int numSamples, int numChannels);

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override {
        return nullptr;
    }

    bool hasEditor() const override { return false; }

    //==============================================================================
    const juce::String getName() const override { return "template"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0; }

    //==============================================================================
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }

    void setCurrentProgram(int) override {
    }

    const juce::String getProgramName(int) override { return "None"; }

    void changeProgramName(int, const juce::String &) override {
    }


    double getRmsLevelLeft() const { return mRmsLevelLeft.load(); }
    double getRmsLevelRight() const { return mRmsLevelRight.load(); }
    double getRmsOutputLevelLeft() const { return mRmsOutputLevelLeft.load(); }
    double getRmsOutputLevelRight() const { return mRmsOutputLevelRight.load(); }

    //==============================================================================
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        // TODO test, je rajoute ca mais jsp si c etait appelé ca peut faire peter le vst3 host
        const auto &mainInLayout = layouts.getChannelSet(true, 0);
        const auto &mainOutLayout = layouts.getChannelSet(false, 0);

        return (mainInLayout == mainOutLayout && (!mainInLayout.isDisabled()));
    }

    juce::AudioProcessorValueTreeState&  getCustomParameterTree() {
        return mParameters;
    }


    void initState() {
    }

    void getStateInformation(juce::MemoryBlock &destData) override {
    }

    void setStateInformation(const void *data, int sizeInBytes) override {
        juce::MemoryInputStream stream(data, static_cast<size_t>(sizeInBytes), false);
        auto newState = juce::ValueTree::readFromStream(stream);
    }

    //==============================================================================
    void initialiseGraph() {
        // TODO : Est ce qu'on peut mettre faust dans le graph ?
        mInputNode = mProcessorGraph.addNode(std::make_unique<AudioInputNode>(AudioInputNode::audioInputNode));
        mOutputNode = mProcessorGraph.addNode(std::make_unique<AudioOutputNode>(AudioOutputNode::audioOutputNode));
        mGainNode = mProcessorGraph.addNode(std::make_unique<GainProcessor>(mParameters));

        for (int channel = 0; channel < 2; ++channel)
        {
            mProcessorGraph.addConnection({
                {mInputNode->nodeID, channel},
                {mGainNode->nodeID, channel}
            });

            mProcessorGraph.addConnection({
                {mGainNode->nodeID, channel},
                {mOutputNode->nodeID, channel}
            });
        }
    }

    void setMapUI(MapUI *inUI) {
        fUI = inUI;
    }

    void setMapUIHpLp(MapUI *inUI) {
        fLpHpUi = inUI;
    }

    void setMapUIDucking(MapUI *inUI) {
        fDuckingUI = inUI;
    }
    void setRateAndBufferSizeDetails(double sampleRate, int bufferSize) {
        mSampleRate = sampleRate;
        mBlockSize = bufferSize;
    }
    std::atomic<float> outputLevelLeft;

private:

    //==============================================================================
    juce::AudioProcessorValueTreeState &mParameters;
    ParameterSetup &mParameterSetup;
    parametersDeclaration::Parameters mParametersDeclaration;

private:
    std::atomic<float> mRmsLevelLeft{0.0f};
    std::atomic<float> mRmsLevelRight{0.0f};
    std::atomic<float> mRmsOutputLevelLeft{0.0f};
    std::atomic<float> mRmsOutputLevelRight{0.0f};

    juce::AudioProcessorGraph mProcessorGraph;

    using AudioInputNode = juce::AudioProcessorGraph::AudioGraphIOProcessor;
    using AudioOutputNode = juce::AudioProcessorGraph::AudioGraphIOProcessor;

    mydsp* mFaustProcessor;
    MapUI* fUI;

    // TODO : faire des processors bien !
    mydsp* mFaustLPHpProcessor;
    MapUI* fLpHpUi;

    mydsp* mFaustDuckingProcessor;
    MapUI* fDuckingUI;

    float** inputs;
    float** outputs;
    juce::AudioProcessorGraph::Node::Ptr mInputNode;
    juce::AudioProcessorGraph::Node::Ptr mOutputNode;
    juce::AudioProcessorGraph::Node::Ptr mGainNode;
    int mBlockSize;
    double mSampleRate;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SkeletonAudioProcessor)
};
