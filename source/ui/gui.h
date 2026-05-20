#pragma once
#include "../dsp/ParameterIDs.hpp"
#include "juce_gui_basics/juce_gui_basics.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include "../dsp/Processor.h"
#include "webSliderAttachments.h"


class VueProcessorEditor : public juce::AudioProcessorEditor,
                           private juce::Timer {
public:
  explicit VueProcessorEditor(SkeletonAudioProcessor&);
  ~VueProcessorEditor() override;

  void resized() override;
    std::string LOCAL_DEV_SERVER_ADDRESS = "http://127.0.0.1:5173";

  void timerCallback() override;
    juce::WebBrowserComponent::Options createWebBrowserOptions() {
       juce::WebBrowserComponent::Options opts = juce::WebBrowserComponent::Options{}
              .withBackend(
                  juce::WebBrowserComponent::Options::Backend::webview2)
              .withWinWebView2Options(
                  juce::WebBrowserComponent::Options::WinWebView2{}
                      .withBackgroundColour(juce::Colours::white)
                      .withUserDataFolder(juce::File::getSpecialLocation(
                          juce::File::SpecialLocationType::tempDirectory)))
              .withNativeIntegrationEnabled()
              .withResourceProvider(
                  [this](const auto& url) { return getResource(url); },
                  // allowedOriginIn parameter is necessary to
                  // retrieve resources from the C++ backend even if
                  // on live server
                  juce::URL{LOCAL_DEV_SERVER_ADDRESS}.getOrigin())
              .withInitialisationData("vendor", JUCE_COMPANY_NAME)
              .withInitialisationData("pluginName", JUCE_PRODUCT_NAME)
              .withInitialisationData("pluginVersion", JUCE_PRODUCT_VERSION)
              .withUserScript("console.log(\"C++ backend here: This is run before any other loading happens\");"
                             )
     .withNativeFunction(
        juce::Identifier{"getPresetList"},
        [this](const juce::Array<juce::var>& args,
        juce::WebBrowserComponent::NativeFunctionCompletion completion) {
        juce::StringArray presets = processorRef.getPresetManager().getPresetList();
        juce::var result;
        juce::Array<juce::var> arr;
        for (const auto& name : presets)
            arr.add(name);
        completion(juce::var{ arr });
        })
        .withNativeFunction(
        juce::Identifier{"savePreset"},
        [this](const juce::Array<juce::var>& args,
           juce::WebBrowserComponent::NativeFunctionCompletion completion) {
        if (args.isEmpty()) { completion("error: no name provided"); return; }
        processorRef.getPresetManager().savePreset(args[0].toString());
        completion("ok");
        })
        .withNativeFunction(
        juce::Identifier{"loadPreset"},
        [this](const juce::Array<juce::var>& args,
           juce::WebBrowserComponent::NativeFunctionCompletion completion) {
        if (args.isEmpty()) { completion("error: no name provided"); return; }
        processorRef.getPresetManager().loadPreset(args[0].toString());
        completion("ok");
        })
        .withNativeFunction(
        juce::Identifier{"deletePreset"},
        [this](const juce::Array<juce::var>& args,
           juce::WebBrowserComponent::NativeFunctionCompletion completion) {
        if (args.isEmpty()) { completion("error: no name provided"); return; }
        processorRef.getPresetManager().deletePreset(args[0].toString());
        completion("ok");
        });
       return webSliderAtt.addSlidersOptions(opts);
    }
private:
  using Resource = juce::WebBrowserComponent::Resource;
  std::optional<Resource> getResource(const juce::String& url) const;

  SkeletonAudioProcessor& processorRef;

  WebSliderAttachments webSliderAtt;
  juce::WebBrowserComponent webView;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VueProcessorEditor)
};
