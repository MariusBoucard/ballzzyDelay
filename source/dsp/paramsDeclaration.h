#pragma once
#include <juce_audio_processors_headless/juce_audio_processors_headless.h>

namespace parametersDeclaration{
    struct Parameters {
        struct Hp {
            juce::AudioParameterFloat* freq{nullptr};
            juce::AudioParameterBool* bypass{nullptr};
        };
        struct Lp {
            juce::AudioParameterFloat* freq{nullptr};
            juce::AudioParameterBool* bypass{nullptr};
        };
        struct MovementFunction {
            juce::AudioParameterFloat* periodDuration{nullptr};
            juce::AudioParameterChoice* function{nullptr};
            juce::AudioParameterFloat* width{nullptr};
            juce::AudioParameterBool* movementOn{nullptr};
            juce::AudioParameterBool* widthSlave{nullptr};
        };
        struct Head{
            juce::AudioParameterBool* bypass{nullptr};
            juce::AudioParameterFloat* feedBack{nullptr};
            juce::AudioParameterFloat* time{nullptr};
            juce::AudioParameterFloat* timeNoSync{nullptr};
            juce::AudioParameterFloat* pan{nullptr};
            juce::AudioParameterFloat* gain{nullptr};
            MovementFunction movementFunction;

            Lp lpFilter;
            Hp hpFilter;
            juce::AudioParameterBool* feedbackSlave{nullptr};
            juce::AudioParameterChoice * gainSlave{nullptr};

        };

        juce::AudioParameterFloat* gain{nullptr};
        juce::AudioParameterBool* bypass{nullptr};
        juce::AudioParameterFloat* inputGain{nullptr};
        juce::AudioParameterFloat* outputGain{nullptr};
        juce::AudioParameterFloat* mix{nullptr};
        juce::AudioParameterFloat* feedback{nullptr};
        juce::AudioParameterFloat* time{nullptr};
        juce::AudioParameterFloat* timeNoSync{nullptr};
        juce::AudioParameterBool* syncTempo{nullptr};
        juce::AudioParameterBool* ducking{nullptr};
        juce::AudioParameterFloat* duckingAttack{nullptr};
        juce::AudioParameterFloat* duckingRelease{nullptr};
        juce::AudioParameterFloat* duckingRatio{nullptr};
        juce::AudioParameterFloat* duckingThreshold{nullptr};
        juce::AudioParameterFloat* width{nullptr};

        Lp lpFilter;
        Hp hpFilter;

        Head head1;
        Head head2;
        Head head3;
        Head head4;

    };
}
