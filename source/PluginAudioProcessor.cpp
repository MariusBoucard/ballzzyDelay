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
void PluginAudioProcessor::updateMovmentHeadPosition(int inHeadNumber, juce::AudioPlayHead* playHead) {
    const bool isSync = false; // TODO: make this a parameter

    // Build parameter IDs dynamically based on head number
    juce::String headPrefix = "HEAD_" + juce::String(inHeadNumber + 1) + "_";

    float duration;
    if (isSync) {
        juce::String durationId = headPrefix + "MOVEMENT_PERIOD_DURATION";
        duration = mParameters.getRawParameterValue(durationId)->load();
    } else {
        juce::String durationId = headPrefix + "MOVEMENT_PERIOD_DURATION_NO_SYNC";
        duration = mParameters.getRawParameterValue(durationId)->load();
    }

    juce::String widthId = headPrefix + "MOVEMENT_WIDTH";
    juce::String startingPointId = headPrefix + "MOVEMENT_PERIOD_STARTING_POINT";
    juce::String functionId = headPrefix + "MOVEMENT_FUNCTION";
    juce::String panId = headPrefix + "PAN";

    const float width = mParameters.getRawParameterValue(widthId)->load();
    const float startingPoint = mParameters.getRawParameterValue(startingPointId)->load();
    const float function = mParameters.getRawParameterValue(functionId)->load();
    const float pan = mParameters.getRawParameterValue(panId)->load();

    float phase = 0.0f;

    if (playHead != nullptr) {
        auto position = playHead->getPosition();

        if (position.hasValue()) {
            if (isSync && position->getPpqPosition().hasValue()) {
                auto ppqPosition = *position->getPpqPosition();
                // Normalize phase to 0-1 range
                phase = fmod(ppqPosition / duration + startingPoint, 1.0);
            }
            else if (!isSync && position->getTimeInSeconds().hasValue()) {
                auto timeInSeconds = *position->getTimeInSeconds();
                // Normalize phase to 0-1 range
                phase = fmod(timeInSeconds / duration + startingPoint, 1.0);
            }
        }
    }
    // TODO : a partir d'ici envoyer que la phase dans le parameterSetup.
    // TODO : en fait mm plus haut, on get que la position in seconds !
    // Comme ca on pourra get les params de la bas, moins reactif mais on bloque moins
    // l'audioThread...

    float functionResult = 0.0f;
    int funcIndex = static_cast<int>(function);

    switch (funcIndex) {
        case 0: // Sine
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

    // Convert pan from 0-1 range to -1 to +1 range
    const float panMinusOnePlus1 = pan * 2.0f - 1.0f;

    // Calculate final pan position (base pan + movement offset)
    const float finalMovement = panMinusOnePlus1 + (width * functionResult);

    // TODO : en fait je crois que faust prend des valeurs entre 0 et 1 pour le pan non ???

    // Clamp to valid range and send to ParameterSetup
    const float clampedPan = juce::jlimit(-1.0f, 1.0f, finalMovement);
    mParameterSetup.setHeadPanPosition(inHeadNumber, clampedPan);
}

