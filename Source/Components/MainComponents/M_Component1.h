#pragma once
#include "../../Source/BASPluginProcessor.h"
#include "../ParamId.h"
#include "CustomButton.h"
#include "FreqAProp.h"
#include "Osc.h"
#include <JuceHeader.h>

class MainVCompartment : public juce::Component, public juce::Slider::Listener
{
public:
    MainVCompartment (BASAudioProcessor& p);
    ~MainVCompartment() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    static const int numSliders = 34;
    std::vector<BastowGainSlider> gainSliders;

private:
    //==============================================================================
    //Private Functions & variables
    //==============================================================================
    void sliderValueChanged (juce::Slider* slider) override;

    BASAudioProcessor& audioProcessor_;
    juce::Image background_;

    std::vector<BastowGainSlider*> sliderArray_;
    // ParameterNames parameterNames_;

    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> gainAttachments_;

    //-------------------------------------------------------------------------------

    const juce::Colour sColour_Scheme_[34] = {
        C1::black,
        C1::grey,
        C2::grey,
        C3::grey,
        C1::red,
        C2::red,
        C3::red,
        C1::orange,
        C2::orange,
        C3::orange,
        C1::yellow,
        C2::yellow,
        C3::yellow,
        C1::brightGreen,
        C2::brightGreen,
        C3::brightGreen,
        C1::limeGreen,
        C2::limeGreen,
        C3::limeGreen,
        C1::brightBlue,
        C2::brightBlue,
        C3::brightBlue,
        C1::blue,
        C2::blue,
        C3::blue,
        C1::violet,
        C2::violet,
        C3::violet,
        C1::magenta,
        C2::magenta,
        C3::magenta,
        C1::cosmic,
        C2::cosmic,
        C3::cosmic
    };

    const std::vector<std::string> numbers_ = { "M", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32" };

    const juce::Colour colour_scheme_[34] {
        C1::black,
        C1::grey,
        C1::red,
        C1::orange,
        C1::yellow,
        C1::brightGreen,
        C1::limeGreen,
        C1::brightBlue,
        C1::blue,
        C1::violet,
        C1::magenta,
        C1::cosmic,
        C2::red,
        C2::orange,
        C2::yellow,
        C2::brightGreen,
        C2::limeGreen,
        C2::brightBlue,
        C2::blue,
        C2::violet,
        C2::magenta,
        C2::cosmic,
        C2::grey,
        C3::red,
        C3::orange,
        C3::yellow,
        C3::brightGreen,
        C3::limeGreen,
        C3::brightBlue,
        C3::blue,
        C3::violet,
        C3::magenta,
        C3::cosmic,
        C3::grey
    };

    float slider_x_[34];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainVCompartment)
};
