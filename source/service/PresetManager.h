#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

/**
 * Manages saving and loading of plugin presets.
 * Presets are stored as XML files in a fixed directory on the user's machine.
 * Integrates directly with AudioProcessorValueTreeState.
 */

class PresetManager
{
public:
    static constexpr const char* PRESET_EXTENSION = ".preset";

    static juce::File getPresetDirectory()
    {
        // ~/<UserDocuments>/YourPluginName/Presets/
        auto dir = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
                       .getChildFile("BallzzyDelay")
                       .getChildFile("Presets");
        if (!dir.exists())
            dir.createDirectory();
        return dir;
    }

    explicit PresetManager(juce::AudioProcessorValueTreeState& apvts);

    void savePreset(const juce::String& presetName);

    void loadPreset(const juce::String& presetName);

    void deletePreset(const juce::String& presetName);

    juce::StringArray getPresetList() const;

    void loadNextPreset();

    void loadPreviousPreset();

    juce::String getCurrentPresetName() const { return mCurrentPresetName; }

private:
    juce::File getPresetFile(const juce::String& presetName) const;

    juce::AudioProcessorValueTreeState& mParameters;
    juce::String mCurrentPresetName;
};