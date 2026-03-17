class WebSliderAttachments {
public:
    WebSliderAttachments(SkeletonAudioProcessor& p) :
        processorRef(p),
        webGainRelay{id::GAIN.getParamID()},
        webBypassRelay{id::BYPASS.getParamID()},
        webDistortionTypeRelay{id::DISTORTION_TYPE.getParamID()},
        webMixRelay{id::MIX.getParamID()}
    {}

    void createAttachments() {
        for (auto* param : processorRef.getParameters())
        {
            auto paramID =static_cast<juce::AudioProcessorParameterWithID*>(param)->paramID;
        }

        auto& state = processorRef.getState();
// On va pouvoir iterrer de la mm maniere que  pour les faust parameters // peut etre creation d attachments depuis main processor
        webGainSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *state.getParameter(id::GAIN.getParamID()), webGainRelay, nullptr);

        webBypassToggleAttachment = std::make_unique<juce::WebToggleButtonParameterAttachment>(
            *state.getParameter(id::BYPASS.getParamID()), webBypassRelay, nullptr);

        webMixSliderAttachment = std::make_unique<juce::WebSliderParameterAttachment>(
            *state.getParameter(id::MIX.getParamID()), webMixRelay, nullptr);

        webDistortionTypeComboBoxAttachment = std::make_unique<juce::WebComboBoxParameterAttachment>(
            *state.getParameter(id::DISTORTION_TYPE.getParamID()), webDistortionTypeRelay, nullptr);
    }

    juce::WebBrowserComponent::Options addSlidersOptions(juce::WebBrowserComponent::Options options) {
       return  options.withOptionsFrom(webGainRelay)
               .withOptionsFrom(webBypassRelay)
               .withOptionsFrom(webDistortionTypeRelay)
               .withOptionsFrom(webMixRelay);
    }

    SkeletonAudioProcessor& processorRef;

    juce::WebSliderRelay webGainRelay;
    juce::WebToggleButtonRelay webBypassRelay;
    juce::WebComboBoxRelay webDistortionTypeRelay;
    juce::WebSliderRelay webMixRelay;

    std::unique_ptr<juce::WebSliderParameterAttachment> webGainSliderAttachment;
    std::unique_ptr<juce::WebToggleButtonParameterAttachment> webBypassToggleAttachment;
    std::unique_ptr<juce::WebComboBoxParameterAttachment> webDistortionTypeComboBoxAttachment;
    std::unique_ptr<juce::WebSliderParameterAttachment> webMixSliderAttachment;
};