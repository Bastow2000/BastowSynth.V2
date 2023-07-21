#pragma once
#include "FreqSLAF.h"
#include "GainSLAF.h"
#include "ADSRsLAF.h"
#include <JuceHeader.h>

class BastowFreqSlider : public juce::Slider
{
public:
    BastowFreqSlider();

    ~BastowFreqSlider();

    bool hitTest (int x, int y);

    void setColour (juce::Colour colourToSet);

    void mouseDoubleClick (const juce::MouseEvent&);

private:
    FreqSliderLookAndFeel lf_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BastowFreqSlider)
};

class BastowGainSlider : public juce::Slider
{
public:
    BastowGainSlider();

    ~BastowGainSlider();

    bool hitTest (int x, int y);

    void setColour (juce::Colour colourToSet);
    void setText (std::string textToSet);
    void setFont (juce::Font fontToSet);

private:
    GainSliderLookAndFeel glf_;
    int desiredSliderIndex_ = 1;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BastowGainSlider)
};

class BastowADSRSlider : public juce::Slider
{
public:
    BastowADSRSlider();

    ~BastowADSRSlider();

    bool hitTest (int x, int y);

    void setColour (juce::Colour colourToSet);

private:
    ADSRSliderLookAndFeel adsrlf_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BastowADSRSlider)
};