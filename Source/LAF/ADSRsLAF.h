#pragma once
#include "../../Source/ParamId.h"
#include <JuceHeader.h>
class ADSRSliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ADSRSliderLookAndFeel();
    ~ADSRSliderLookAndFeel()override;
        
    juce::Slider::SliderLayout getSliderLayout (juce::Slider& slider) override;
            
    void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider&) override;
    
    void setSliderColour(juce::Colour colourToUse)
    {
        mainSliderColour_ = colourToUse;
    }
    
private:
   float b_;
    juce::Colour mainSliderColour_ = juce::Colours::blue;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRSliderLookAndFeel);
};
