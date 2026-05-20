#include "PresetManager.h"

PresetManager::PresetManager(juce::AudioProcessorValueTreeState& apvts)
    : mParameters(apvts)
{}

void PresetManager::savePreset(const juce::String& presetName)
{
    if (presetName.isEmpty()) return;

    const auto state = mParameters.copyState();
    const auto xml = state.createXml();

    if (xml == nullptr) return;

    const auto file = getPresetFile(presetName);
    if (!xml->writeTo(file))
    {
        jassertfalse; // Failed to write — check permissions or path
        return;
    }

    mCurrentPresetName = presetName;
}

void PresetManager::loadPreset(const juce::String& presetName)
{
    if (presetName.isEmpty()) return;

    const auto file = getPresetFile(presetName);
    if (!file.existsAsFile())
    {
        jassertfalse; // Preset file not found
        return;
    }

    const auto xml = juce::XmlDocument::parse(file);
    if (xml == nullptr) return;

    // Restore the state: APVTS will fire all parameterChanged callbacks automatically
    const auto newState = juce::ValueTree::fromXml(*xml);
    if (newState.isValid())
    {
        mParameters.replaceState(newState);
        mCurrentPresetName = presetName;
    }
}

void PresetManager::deletePreset(const juce::String& presetName)
{
    const auto file = getPresetFile(presetName);
    if (file.existsAsFile())
        file.deleteFile();

    if (mCurrentPresetName == presetName)
        mCurrentPresetName = {};
}

juce::StringArray PresetManager::getPresetList() const
{
    juce::StringArray presets;
    for (const auto& file : getPresetDirectory().findChildFiles(
             juce::File::findFiles, false, "*" + juce::String(PRESET_EXTENSION)))
    {
        presets.add(file.getFileNameWithoutExtension());
    }
    presets.sort(true); // Alphabetical
    return presets;
}

void PresetManager::loadNextPreset()
{
    const auto list = getPresetList();
    if (list.isEmpty()) return;

    const int currentIndex = list.indexOf(mCurrentPresetName);
    const int nextIndex = (currentIndex + 1) % list.size();
    loadPreset(list[nextIndex]);
}

void PresetManager::loadPreviousPreset()
{
    const auto list = getPresetList();
    if (list.isEmpty()) return;

    const int currentIndex = list.indexOf(mCurrentPresetName);
    const int prevIndex = (currentIndex <= 0) ? list.size() - 1 : currentIndex - 1;
    loadPreset(list[prevIndex]);
}

juce::File PresetManager::getPresetFile(const juce::String& presetName) const
{
    return getPresetDirectory().getChildFile(presetName + PRESET_EXTENSION);
}