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
    // File extension used for preset files
    static constexpr const char* PRESET_EXTENSION = ".preset";

    // Returns the directory where presets are stored (created if missing)
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

    // Save the current APVTS state to a named preset file
    void savePreset(const juce::String& presetName);

    // Load a preset by name (without extension) into the APVTS
    void loadPreset(const juce::String& presetName);

    // Delete a preset file by name
    void deletePreset(const juce::String& presetName);

    // Returns all preset names (no extension) found in the preset directory
    juce::StringArray getPresetList() const;

    // Navigate through presets
    void loadNextPreset();
    void loadPreviousPreset();

    // Current preset name ("" if none loaded)
    juce::String getCurrentPresetName() const { return mCurrentPresetName; }

private:
    juce::File getPresetFile(const juce::String& presetName) const;

    juce::AudioProcessorValueTreeState& mApvts;
    juce::String mCurrentPresetName;
};