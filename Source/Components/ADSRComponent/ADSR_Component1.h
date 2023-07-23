#pragma once
#include "../../Source/BASPluginProcessor.h"
#include "../ParamId.h"
#include "FreqAProp.h"
#include "ADSRsLAF.h"
#include "Osc.h"
#include <JuceHeader.h>

class ADSRCompartment : public juce::Component,
    public juce::Timer,
    public juce::Slider::Listener//, public juce::Button::Listener 
{
public:
    ADSRCompartment(BASAudioProcessor& p);
    ~ADSRCompartment() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;
    void setupSliders();
    void drawADSRSegment(juce::Graphics& g, int x1, int y1, int x2, int y2, const juce::Colour& colour, int thickness);
    void drawFilledADSRPoints(juce::Graphics& g, int x, int y, int size);
    void changeADSROsc(juce::TextButton& button);

    static constexpr int numSliders = 6;
    std::vector<BastowADSRSlider> ADSRSliders;

private:
    void sliderValueChanged(juce::Slider* slider) override;
    
   // void buttonClicked (juce::Button* button)override;
    void repaintComponent(juce::Graphics& g);
   

    BASAudioProcessor& audioProcessor_;
    juce::Image background_;

    juce::TextButton osc;
    CustomADSR amplitudeADSR;
    std::vector<CustomADSR> amplitudeADSRs_;
    std::vector<BastowADSRSlider*> sliderArray_;
    std::vector<int> slider_y_ { 108, 108, 108, 295, 295, 295 };
    std::vector<int> slider_x_ { -20, 20, 60, -20, 20, 60 };
    
    juce::AudioProcessorValueTreeState& tree;
    
    std::unique_ptr<WavetableSynthesiserVoice> osc_;

    int maxX = 60;

    int maxY = 100;

    std::vector<Wavetable*> oscillators_;

    int currentOscillatorIndex = 0;

    // The SliderWidth controls the amount of movement each slider has with the line
    int sliderAWidth = 1;

    int sliderHWidth = 1;

    int sliderDWidth = 1;

    int sliderSWidth = 1;

    int sliderD2Width = 1;

    int sliderRWidth = 1;

    // Define the position and height for each segment
    int yLocation = 285;

    int xLocation = 12;

    int segmentThickness = 6;

    int selectedOscillatorIndex = -1;

    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> ADSRAttachments_;

    static constexpr int num_harms_ = 6;
    const std::vector<int> text_x_ = { 8, 50, 87, 5, 45, 85 };
    const std::vector<int> text_y_ = { 93, 93, 93, 365, 365, 365 };
    const std::vector<std::string> envelopeStages_ = { "Attack", "Hold", "Decay", "Sustain", "Decay2", "Release" };
    const std::vector<juce::String> paramId_ = { attackId, holdId, decayId, sustainId, decay2Id, releaseId };
    std::array<juce::Colour, numSliders> colour_scheme_ { C1::yellow, C1::orange, C1::limeGreen, C1::blue, C1::violet, C1::magenta };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRCompartment)
};