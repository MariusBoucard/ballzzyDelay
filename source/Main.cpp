/*
  ==============================================================================

    This file was auto-generated and contains the startup code for a PIP.

  ==============================================================================
*/
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginAudioProcessor.h"

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    //return new FaustPlugInAudioProcessor();
    return new PluginAudioProcessor();
}
