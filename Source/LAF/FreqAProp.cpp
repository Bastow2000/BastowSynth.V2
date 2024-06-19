#include "FreqAProp.h"

BastowFreqSlider::BastowFreqSlider()
{
    // Set the look and feel
    setLookAndFeel(&lf_);

    // Set the text box style
    setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);

    // Enable popup display
    setPopupDisplayEnabled(true, false, this);

    // Set the text value suffix
    setTextValueSuffix(" dB");

    // Enable velocity-based mode
    setVelocityBasedMode(true);

    // Set velocity mode parameters
    setVelocityModeParameters(0.09, 1, 0.5, false);

    // Set the slider style
    setSliderStyle(SliderStyle::LinearHorizontal);
}

BastowFreqSlider::~BastowFreqSlider()
{
    // Reset the look and feel
    setLookAndFeel(nullptr);
}

bool BastowFreqSlider::hitTest(int x, int y)
{
    // Calculate the diameter based on the minimum dimension
    int dia = std::min(getWidth(), getHeight());

    // Check if the point (x, y) is within the circle centered at the bounds' center
    return getLocalBounds().getCentre().getDistanceFrom({ x, y }) < dia;
}

void BastowFreqSlider::mouseDoubleClick(const juce::MouseEvent&)
{
    // Set the slider value back to the default value
    setValue(0.0, juce::NotificationType::sendNotification);
}

void BastowFreqSlider::setColour(juce::Colour colourToSet)
{
    // Set the slider colour
    lf_.setSliderColour(colourToSet);
}

BastowGainSlider::BastowGainSlider()
{
    // Set the look and feel
    setLookAndFeel(&glf_);

    // Set the text box style
    setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);

    // Enable popup display
    setPopupDisplayEnabled(true, false, this);

    // Set the text value suffix
    setTextValueSuffix(" dB");

    // Enable velocity-based mode
    setVelocityBasedMode(true);

    // Set velocity mode parameters
    setVelocityModeParameters(0.09, 1, 0.5, false);

    // Set the slider style
    setSliderStyle(SliderStyle::LinearVertical);

    // Add mouse listener
    addMouseListener(this, true);
}

BastowGainSlider::~BastowGainSlider()
{
    // Reset the look and feel
    setLookAndFeel(nullptr);
}

bool BastowGainSlider::hitTest(int x, int y)
{
    // Adjust the scaling factor as needed
    int scalingFactor = 4;

    // Calculate the diameter based on the minimum dimension and scaling factor
    int dia = std::min(getWidth(), getHeight()) * scalingFactor;

    // Check if the point (x, y) is within the circle centered at the bounds' center
    return getLocalBounds().getCentre().getDistanceFrom({ x, y }) < dia;
}

void BastowGainSlider::setColour(juce::Colour colourToSet)
{
    // Set the slider colour
    glf_.setSliderColour(colourToSet);
}

void BastowGainSlider::setText(std::string textToSet)
{
    // Set the slider text
    glf_.setSliderText(textToSet);
}

void BastowGainSlider::setFont(juce::Font fontToSet)
{
    // Set the slider font
    glf_.setSliderFont(fontToSet);
}

BastowADSRSlider::BastowADSRSlider()
{
    // Set the look and feel
    setLookAndFeel(&adsrlf_);

    // Set the text box style
    setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);

    // Enable popup display
    setPopupDisplayEnabled(true, false, this);

    // Set the text value suffix
    setTextValueSuffix(" dB");

    // Enable velocity-based mode
    setVelocityBasedMode(true);

    // Set velocity mode parameters
    setVelocityModeParameters(0.09, 1, 0.5, false);

    // Set the slider style
    setSliderStyle(SliderStyle::RotaryHorizontalVerticalDrag);
}

BastowADSRSlider::~BastowADSRSlider()
{
    // Reset the look and feel
    setLookAndFeel(nullptr);
}

bool BastowADSRSlider::hitTest(int x, int y)
{
    // Adjust the scaling factor as needed
    int scalingFactor = 4;

    // Calculate the diameter based on the minimum dimension and scaling factor
    int dia = std::min(getWidth(), getHeight()) * scalingFactor;

    // Check if the point (x, y) is within the circle centered at the bounds' center
    return getLocalBounds().getCentre().getDistanceFrom({ x, y }) < dia;
}

void BastowADSRSlider::setColour(juce::Colour colourToSet)
{
    // Set the slider colour
    adsrlf_.setSliderColour(colourToSet);
}
