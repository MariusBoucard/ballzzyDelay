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

void PluginAudioProcessor::updateMovmentHeadPosition(int inHeadNumber,juce::AudioPlayHead* playHead) {
    // get the parameters
    const bool isSync = true;

    // TODO : attention il va falloir faire un toggle pour le mouvement si il est linear ou sync
    float duration;
    if (true) {
        // sync
        duration = mParameters.getRawParameterValue(id::HEAD_1_MOVEMENT_PERIOD_DURATION.getParamID())->load();
    } else {
       duration = mParameters.getRawParameterValue(id::HEAD_1_MOVEMENT_PERIOD_DURATION_NO_SYNC.getParamID())->load();
    }
    const float width = mParameters.getRawParameterValue(id::HEAD_1_MOVEMENT_WIDTH.getParamID())->load();
    const float startingPoint = mParameters.getRawParameterValue(id::HEAD_1_MOVEMENT_PERIOD_STARTING_POINT.getParamID())->load();
    const float function = mParameters.getRawParameterValue(id::HEAD_1_MOVEMENT_FUNCTION.getParamID())->load();
    const float pan = mParameters.getRawParameterValue(id::HEAD_1_PAN.getParamID())->load();

    float phase = 0.;

    if (playHead != nullptr) {
        juce::AudioPlayHead::CurrentPositionInfo posInfo;
        auto position = playHead->getPosition();
        // Phase va dépendre de si on est en sync ou pas
        // pour l'instant osef. Est ce qu'on essaye de sync sur les temps meme ? peut etre chiant a partir de ppqPositon ??
        if (position->getPpqPosition()) {
            auto ppqPosition = position->getPpqPosition();
            auto ppqPositionLastBar = position->getPpqPositionOfLastBarStart();
            phase = fmod(*ppqPosition + (startingPoint * duration), duration);
        }

        if (position->getTimeInSeconds()) {
            auto timeInSeconds = position->getTimeInSeconds();
            if (auto timeInSeconds = position->getTimeInSeconds()) {
                // Dereference the Optional with * to get the actual double value
                phase = fmod(*timeInSeconds  + (startingPoint * duration), duration);
            }
        }
    }

    float functionResult = 0;
    int funcIndex = (int)function;
    switch (funcIndex) {
        case 0: // Sine
            // 2 pi ???
            functionResult = std::sin(phase * 2.0f * juce::MathConstants<float>::pi);
            break;

        case 1: // Rectangle/Square
            functionResult = (phase < 0.5f) ? 1.0f : -1.0f;
            break;

        case 2: // Triangle
            if (phase < 0.25f) {
                // Rising 0 to 1
                functionResult = phase * 4.0f;
            } else if (phase < 0.75f) {
                // Falling 1 to -1
                functionResult = 1.0f - ((phase - 0.25f) * 4.0f);
            } else {
                // Rising -1 to 0
                functionResult = -1.0f + ((phase - 0.75f) * 4.0f);
            }
            break;

        default:
            functionResult = 0.0f;
            break;
    }
    const float panMinusOnePlus1 = pan*2 - 1.0f;
    const float finalMovment = width * functionResult + panMinusOnePlus1; // attention au pan !


    mParameterSetup.setHeadPanPosition(inHeadNumber, finalMovment);
}

