#pragma once
#include "../../Source/BASPluginProcessor.h"
#include "../ParamId.h"
#include "Component_Placement.h"
#include "CustomButton.h"
#include "FreqAProp.h"
#include "FreqSLAF.h"
#include "Osc.h"
#include <JuceHeader.h>

class ThirdVCompartment : public juce::Component,
                          public juce::Slider::Listener,
                          public juce::Button::Listener
{
public:
    ThirdVCompartment (BASAudioProcessor& p);

    ~ThirdVCompartment() override;

    void drawSineWave (juce::Graphics& g, const int startX, const int startY, const int amplitude, const int wavelength, const int period, const juce::Colour& colour);
    void drawCross (juce::Graphics& g, const int crossX, const int crossY, const int crossSize, const juce::Colour& colour);
    void paint (juce::Graphics&) override;
    void resized() override;

    /* The variable is static, if there is an instance of the class called it will
    have the same value among all instances*/

    /* The variable is constexpr as the variables are not used in any
     calculation*/
    static constexpr int numSliders = 11;
    std::vector<BastowFreqSlider> frequencySliders;

    static constexpr int numButtons = 11;
    //std::vector<CustomButton> waveTypeB;
    std::vector<std::unique_ptr<CustomButton>> waveTypeB;

private:
    //==============================================================================
    //Private Functions & variables
    //==============================================================================
    void sliderValueChanged (juce::Slider* slider) override;
    void buttonClicked (juce::Button* button) override;

    BASAudioProcessor& audioProcessor_;
    juce::Image background_;
    MyDraggableComponent vComponent_;
    MyResizableBorderComponent resizableBorderComponent_;
    // ParameterNames parameterNames_;

    WavetableSynthesiserVoice* voice_;

    //-------------------------------------------------------------------------------
    std::vector<BastowFreqSlider*> sliderArray_;

    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> frequencyAttachments_;

    std::vector<int> slider_y_ { 24, 52, 81, 108, 138, 167, 195, 221, 251, 276, 300 };

    //-------------------------------------------------------------------------------

    std::vector<CustomButton*> buttonArray_;

    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>> waveTypeAttachments_;

    std::vector<int> button_y_ { 90, 115, 144, 171, 201, 230, 258, 286, 314, 339, 363 };

    //-------------------------------------------------------------------------------
    const juce::Colour colour_scheme_[11] = { C3::red, C3::orange, C3::yellow, C3::brightGreen, C3::limeGreen, C3::brightBlue, C3::blue, C3::violet, C3::magenta, C3::cosmic, C3::grey };
    //-------------------------------------------------------------------------------
    // Used for Text for loop
    static constexpr int num_harms_ = 11;

    // X Location
    const int text_x_ = 80;

    // Y Location
    std::vector<int> text_y_ { 91, 118, 147, 174, 204, 233, 261, 287, 317, 342, 366 };
    //-------------------------------------------------------------------------------
    // Used for paint Sine for loop
    // Amplitude of the sine wave
    int amplitude_ = 5;

    // Wavelength of the sine wave
    int wavelength_ = 13;

    // Number of cycles of the sine wave
    int period_ = 2;

    // X Location
    const int sine_x_ = 55;

    // Y Location
    std::vector<int> sine_y_ { 94, 122, 151, 178, 208, 237, 265, 291, 321, 346, 370 };
    //-------------------------------------------------------------------------------
    // Used for paint cross for loop
    // X Location
    const int cross_x_ = 70;

    // Y Location
    std::vector<int> cross_y_ { 94, 122, 151, 178, 208, 237, 265, 291, 321, 346, 370 };

    const int crossSize_ = 5;
    //-------------------------------------------------------------------------------

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThirdVCompartment)
};
