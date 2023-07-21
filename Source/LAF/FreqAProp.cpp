#include "FreqAProp.h"

BastowFreqSlider::BastowFreqSlider()
{
    setLookAndFeel (&lf_);
    setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    setPopupDisplayEnabled (true, false, this);
    setTextValueSuffix (" dB");
    setVelocityBasedMode (true);
    setVelocityModeParameters (0.09, 1, 0.5, false);
    setSliderStyle (SliderStyle::LinearHorizontal);
}

BastowFreqSlider::~BastowFreqSlider()
{
    setLookAndFeel (nullptr);
}

bool BastowFreqSlider::hitTest (int x, int y)
{
    // int scalingFactor = 0; // Adjust the scaling factor as needed
    int dia = std::min (getWidth(), getHeight());
    return getLocalBounds().getCentre().getDistanceFrom ({ x, y }) < dia;
}

void BastowFreqSlider::mouseDoubleClick (const juce::MouseEvent&)
{
    // Set the slider value back to the default value
    setValue (0.0, juce::NotificationType::sendNotification);
}

void BastowFreqSlider::setColour (juce::Colour colourToSet)
{
    lf_.setSliderColour (colourToSet);
}

BastowGainSlider::BastowGainSlider()
{
    setLookAndFeel (&glf_);
    setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    setPopupDisplayEnabled (true, false, this);
    setTextValueSuffix (" dB");
    setVelocityBasedMode (true);
    setVelocityModeParameters (0.09, 1, 0.5, false);
    setSliderStyle (SliderStyle::LinearVertical);
    addMouseListener (this, true);
}

BastowGainSlider::~BastowGainSlider()
{
    setLookAndFeel (nullptr);
}

bool BastowGainSlider::hitTest (int x, int y)
{
    int scalingFactor = 4; // Adjust the scaling factor as needed
    int dia = std::min (getWidth(), getHeight()) * scalingFactor;
    return getLocalBounds().getCentre().getDistanceFrom ({ x, y }) < dia;
    return true;
}

void BastowGainSlider::setColour (juce::Colour colourToSet)
{
    glf_.setSliderColour (colourToSet);
}
void BastowGainSlider::setText (std::string textToSet)
{
    glf_.setSliderText (textToSet);
}
void BastowGainSlider::setFont (juce::Font fontToSet)
{
    glf_.setSliderFont (fontToSet);
}


BastowADSRSlider::BastowADSRSlider()
{
    setLookAndFeel (&adsrlf_);
    setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    setPopupDisplayEnabled (true, false, this);
    setTextValueSuffix (" dB");
    setVelocityBasedMode (true);
    setVelocityModeParameters (0.09, 1, 0.5, false);
    //setSliderStyle (SliderStyle::RotaryHorizontalVerticalDrag);
    setSliderStyle (SliderStyle::RotaryHorizontalVerticalDrag);
  //  addMouseListener (this, true);
}

BastowADSRSlider::~BastowADSRSlider()
{
    setLookAndFeel (nullptr);
}

bool BastowADSRSlider::hitTest (int x, int y)
{
    int scalingFactor = 4; // Adjust the scaling factor as needed
    int dia = std::min (getWidth(), getHeight()) * scalingFactor;
    return getLocalBounds().getCentre().getDistanceFrom ({ x, y }) < dia;
    return true;
}

void BastowADSRSlider::setColour (juce::Colour colourToSet)
{
    adsrlf_.setSliderColour (colourToSet);
}
