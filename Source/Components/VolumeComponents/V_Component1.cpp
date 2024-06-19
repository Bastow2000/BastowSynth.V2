#include "V_Component1.h"

// //https://corecoding.com/utilities/rgb-or-hex-to-float.php

FirstVCompartment::FirstVCompartment(BASAudioProcessor& p) : audioProcessor_(p),
                                                              voice_(std::make_unique<WavetableSynthesiserVoice>().release()),
                                                              waveTypeB(numButtons),
                                                              sliderArray_(numSliders),
                                                              frequencyAttachments_(numSliders),
                                                              buttonArray_(numButtons),
                                                              waveTypeAttachments_(numButtons),
                                                              frequencySliders(numSliders)
{
    // Set up slider array
    for (std::size_t i = 0; i < numSliders; ++i)
    {
        sliderArray_[i] = &frequencySliders[i];
    }

    // Lambda function to get frequency slider color
    const auto getFreqSliderColour = [&](int index)
    {
        return colour_scheme_[index];
    };

    // Lambda function to create slider attachment
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    static const auto createSliderAttachment = [](juce::AudioProcessorValueTreeState& tree, std::unique_ptr<SliderAttachment>& attachment, juce::String paramID, juce::Slider& slider)
    {
        attachment = std::make_unique<SliderAttachment>(tree, paramID, slider);
    };

    setAlwaysOnTop(false);

    // Setup each frequency slider
    for (auto i = 0; i < numSliders; ++i)
    {
        frequencySliders[static_cast<std::vector<BastowFreqSlider>::size_type>(i)].addListener(this);
        frequencySliders[static_cast<std::vector<BastowFreqSlider>::size_type>(i)].setColour(getFreqSliderColour(static_cast<int>(i)));
        addAndMakeVisible(frequencySliders[static_cast<std::vector<BastowFreqSlider>::size_type>(i)]);
        createSliderAttachment(audioProcessor_.tree, frequencyAttachments_[static_cast<std::vector<BastowFreqSlider>::size_type>(i)], getFreqId(i), frequencySliders[static_cast<std::vector<BastowFreqSlider>::size_type>(i)]);
    }

    // Lambda function to create button attachment
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    static const auto createButtonAttachment = [](juce::AudioProcessorValueTreeState& tree, std::unique_ptr<ButtonAttachment>& attachment, juce::String paramID, juce::Button& button)
    {
        attachment = std::make_unique<ButtonAttachment>(tree, paramID, button);
    };

    // Setup each wave type button
    for (std::size_t i = 0; i < numButtons; ++i)
    {
        std::vector<std::unique_ptr<CustomButton>> buttons(numButtons);
        buttons[i] = std::make_unique<CustomButton>(*voice_, i);

        buttons[i]->addListener(this);
        buttons[i]->setClickingTogglesState(true);
        buttons[i]->setButtonColour(getFreqSliderColour(static_cast<int>(i)));
        addAndMakeVisible(*buttons[i]);
        waveTypeB[i] = std::move(buttons[i]);

        createButtonAttachment(audioProcessor_.tree, waveTypeAttachments_[i], getWaveTypeId(i), *waveTypeB[i]);
        buttonArray_[i] = waveTypeB[i].get();
    }
}

FirstVCompartment::~FirstVCompartment()
{
}

void FirstVCompartment::drawSineWave(juce::Graphics& g, const int startX, const int startY, int amplitude, int wavelength, int period, const juce::Colour& colour)
{
    g.setColour(colour);

    // Compute waveLengthPixels at compile-time
    const int waveLengthPixels = wavelength * period;
    const int sineSize = 1;

    const int waveHeight = sineSize * 2;

    // Draw the sine wave
    for (int i = 4; i < waveLengthPixels - 4; i++)
    {
        float x = startX + i;
        float sine = amplitude * std::sin(2 * juce::MathConstants<float>::pi * i / wavelength);
        float y = startY + sine;

        g.drawLine(static_cast<int>(x), static_cast<int>(y), static_cast<int>(x), static_cast<int>(y + 1 + waveHeight));
    }
}

void FirstVCompartment::drawCross(juce::Graphics& g, const int crossX, const int crossY, int crossSize, const juce::Colour& colour)
{
    g.setColour(colour);

    juce::Rectangle<int> crossBounds(crossX - crossSize, crossY - crossSize, crossSize * 2, crossSize * 2);

    // Draw vertical line
    g.drawLine(crossBounds.getX() + crossBounds.getWidth() / 2, crossBounds.getY(), crossBounds.getX() + crossBounds.getWidth() / 2, crossBounds.getBottom(), 1);

    // Draw horizontal line
    g.drawLine(crossBounds.getX(), crossBounds.getY() + crossBounds.getHeight() / 2, crossBounds.getRight(), crossBounds.getY() + crossBounds.getHeight() / 2, 1);
}

void FirstVCompartment::paint(juce::Graphics& g)
{
    // Get background image
    background_ = juce::ImageCache::getFromMemory(BinaryData::V_C1_png, BinaryData::V_C1_pngSize);
    g.drawImageWithin(background_, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);

    // Set up font and color
    static auto black = juce::Typeface::createSystemTypefaceFor(BinaryData::AvenirBlack_ttf, BinaryData::AvenirBlack_ttfSize);
    g.setColour(juce::Colours::white);
    g.setFont(black);
    g.setFont(15);

    juce::Rectangle<int> s = getLocalBounds().withPosition({ 12, 59 });
    g.drawFittedText("Pitch Bend", s, juce::Justification::topLeft, false);

    static auto book = juce::Typeface::createSystemTypefaceFor(BinaryData::AvenirBook_ttf, BinaryData::AvenirBook_ttfSize);
    g.setColour(juce::Colours::white);
    g.setFont(book);
    g.setFont(10);

    // Draw pitch labels
    for (int i = 0; i < num_harms_; i++)
    {
        juce::Rectangle<int> harm = getLocalBounds().withPosition({ text_x_, text_y_[static_cast<std::vector<int>::size_type>(i)] });
        std::string text = "Pitch " + std::to_string(0 + i);
        g.drawFittedText(text, harm, juce::Justification::topLeft, false);
    }
}

void FirstVCompartment::resized()
{
    // Set bounds for frequency sliders
    for (unsigned int i = 0; i < sine_y_.size(); ++i)
    {
        auto a = getLocalBounds().withWidth(60).withHeight(140).withX(0);
        frequencySliders[i].setBounds(a.withY(slider_y_[i]));

        auto b = getLocalBounds().withWidth(60).withHeight(140).withX(55);
        waveTypeB[i]->setBounds(b.withY(button_y_[i]));
    }
}

void FirstVCompartment::buttonClicked(juce::Button* /*button*/)
{
}

void FirstVCompartment::sliderValueChanged(juce::Slider* /*slider*/)
{
}
