#pragma once
#include "../dsp/ParameterIDs.hpp"
#include "juce_gui_basics/juce_gui_basics.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include "../dsp/Processor.h"


class VueProcessorEditor : public juce::AudioProcessorEditor,
                           private juce::Timer {
public:
  explicit VueProcessorEditor(SkeletonAudioProcessor&);
  ~VueProcessorEditor() override;

  void resized() override;
    std::string LOCAL_DEV_SERVER_ADDRESS = "http://127.0.0.1:5173";

  void timerCallback() override;
    juce::WebBrowserComponent::Options createWebBrowserOptions() {
      
       return juce::WebBrowserComponent::Options{}
              .withBackend(
                  juce::WebBrowserComponent::Options::Backend::webview2)
              .withWinWebView2Options(
                  juce::WebBrowserComponent::Options::WinWebView2{}
                      .withBackgroundColour(juce::Colours::white)
                      // this may be necessary for some DAWs; include for safety
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
              .withEventListener(
                  "exampleJavaScriptEvent",
                  [this](juce::var objectFromFrontend) {
                    labelUpdatedFromJavaScript.setText(
                        "example JavaScript event occurred with value " +
                            objectFromFrontend.getProperty("emittedCount", 0)
                                .toString(),
                        juce::dontSendNotification);
                  })
              .withNativeFunction(
                  juce::Identifier{"nativeFunction"},
                  [this](const juce::Array<juce::var>& args,
                         juce::WebBrowserComponent::NativeFunctionCompletion
                             completion) {
                    nativeFunction(args, std::move(completion));
                  })
              .withNativeFunction(
                  juce::Identifier{"getLogs"},
                  [this](const juce::Array<juce::var>& args,
                         juce::WebBrowserComponent::NativeFunctionCompletion
                             completion) {
                    webView.evaluateJavascript(
                        "JSON.stringify(window.__allLogs || [])",
                        [completion](juce::WebBrowserComponent::EvaluationResult result) {
                          if (const auto* resultPtr = result.getResult()) {
                            const auto logsJson = resultPtr->toString();
                            std::cout << "\n[BROWSER LOGS]\n" << logsJson << "\n" << std::endl;
                            completion(logsJson);
                          } else {
                            completion("Error retrieving logs");
                          }
                        });
                  })
              .withOptionsFrom(webGainRelay)
              .withOptionsFrom(webBypassRelay)
              .withOptionsFrom(webDistortionTypeRelay)
              .withOptionsFrom(webMixRelay);
    }
private:
  using Resource = juce::WebBrowserComponent::Resource;
  std::optional<Resource> getResource(const juce::String& url) const;
  void nativeFunction(
      const juce::Array<juce::var>& args,
      juce::WebBrowserComponent::NativeFunctionCompletion completion);

  juce::TextButton runJavaScriptButton{"Run some JavaScript"};
  juce::TextButton emitJavaScriptEventButton{"Emit JavaScript event"};
  juce::Label labelUpdatedFromJavaScript{"label",
                                         "To be updated from JavaScript"};

  SkeletonAudioProcessor& processorRef;

  juce::Slider gainSlider{"gain slider"};
  juce::SliderParameterAttachment gainSliderAttachment;

  juce::ToggleButton bypassButton{"Bypass"};
  juce::ButtonParameterAttachment bypassButtonAttachment;

  juce::Label distortionTypeLabel{"distortion type label", "Distortion"};
  juce::ComboBox distortionTypeComboBox{"distortion type combo box"};
  juce::ComboBoxParameterAttachment distortionTypeComboBoxAttachment;

  juce::WebSliderRelay webGainRelay;
  juce::WebToggleButtonRelay webBypassRelay;
  juce::WebComboBoxRelay webDistortionTypeRelay;
    juce::WebSliderRelay webMixRelay;

  juce::WebBrowserComponent webView;

  juce::WebSliderParameterAttachment webGainSliderAttachment;
  juce::WebToggleButtonParameterAttachment webBypassToggleAttachment;
  juce::WebComboBoxParameterAttachment webDistortionTypeComboBoxAttachment;
 juce::WebSliderParameterAttachment webMixSliderAttachment;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VueProcessorEditor)
};
