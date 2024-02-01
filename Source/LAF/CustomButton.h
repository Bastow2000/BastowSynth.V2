#pragma once
#include "Osc.h"
#include <JuceHeader.h>

class CustomButton : public juce::Button
{
public:
    CustomButton (WavetableSynthesiserVoice& voice, int oscillatorIndex);
    ~CustomButton()override;

    void paint (juce::Graphics& g) override;
    void mouseDown (const juce::MouseEvent& event) override;
    void setWaveType (int waveType);
    void paintButton (juce::Graphics& g, bool isMouseOver, bool isButtonDown) override;
    void drawSineWave (juce::Graphics& g, int startX, int startY, int amplitude, int wavelength, int period, juce::Colour colour);
    void drawSawWave (juce::Graphics& g, int startX, int startY, juce::Colour colour);
    void drawSquareWave (juce::Graphics& g, int startX, int startY, juce::Colour colour);
    void drawTriangleWave (juce::Graphics& g, int startX, int startY, int amplitude, int wavelength, int period, juce::Colour colour);
    void setButtonColour (juce::Colour colourToUse);

    int counter;

private:
    // Counter to keep track of the number of clicks
    std::vector<int> waveType_;
    // Amplitude of the sine wave
    const int amplitude_ = 5;
    //WavetableSynthesiserVoice voice_;
    WavetableSynthesiserVoice* voice_;
    int oscillatorIndex_;

    // Wavelength of the sine wave
    const int wavelength_ = 13;
    static constexpr int kNumOscillators_ = 33;
    //std::vector<int> wavelength {13};

    const int period_ = 2;

    juce::Colour mainSliderColour_ = juce::Colours::red;
};