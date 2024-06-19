#include "ADSR_Component1.h"

ADSRCompartment::ADSRCompartment(BASAudioProcessor& p) : audioProcessor_(p),
                                                         tree(p.tree),
                                                         ADSRSliders(numSliders),
                                                         sliderArray_(numSliders),
                                                         ADSRAttachments_(numSliders)
{
    startTimer(1);
    for (std::size_t i = 0; i < numSliders; ++i)
    {
        sliderArray_[i] = &ADSRSliders[i];
    }
    setAlwaysOnTop(false);
    setupSliders();
    changeADSROsc(osc);
}

void ADSRCompartment::setupSliders()
{
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    for (auto i = 0; i < numSliders; ++i)
    {
        ADSRSliders[static_cast<std::vector<BastowADSRSlider>::size_type>(i)].addListener(this);
        ADSRSliders[static_cast<std::vector<BastowADSRSlider>::size_type>(i)].setColour(colour_scheme_[i]);
        addAndMakeVisible(ADSRSliders[static_cast<std::vector<BastowADSRSlider>::size_type>(i)]);
        addAndMakeVisible(osc);
        std::unique_ptr<SliderAttachment> attachment = std::make_unique<SliderAttachment>(audioProcessor_.tree, paramId_[static_cast<std::vector<juce::String>::size_type>(i)], ADSRSliders[static_cast<std::vector<BastowADSRSlider>::size_type>(i)]);
        ADSRAttachments_[static_cast<std::vector<BastowADSRSlider>::size_type>(i)] = std::move(attachment);
    }
}

ADSRCompartment::~ADSRCompartment()
{
    stopTimer();
}

void ADSRCompartment::timerCallback()
{
    // Repaint the component
    repaint();
}

void ADSRCompartment::changeADSROsc(juce::TextButton& button)
{
    // TODO: Implement the logic for changing ADSR oscillator
}

void ADSRCompartment::paint(juce::Graphics& g)
{
    // Gets background for this component
    background_ = juce::ImageCache::getFromMemory(BinaryData::ADSR_C_png, BinaryData::ADSR_C_pngSize);
    g.drawImageWithin(background_, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);

    // Set up font and color
    static auto black = juce::Typeface::createSystemTypefaceFor(BinaryData::AvenirBlack_ttf, BinaryData::AvenirBlack_ttfSize);
    g.setColour(juce::Colours::white);
    g.setFont(black);
    g.setFont(15);

    juce::Rectangle<int> s = getLocalBounds().withPosition({40, 59});
    g.drawFittedText("ADSR", s, juce::Justification::topLeft, false);

    static auto book = juce::Typeface::createSystemTypefaceFor(BinaryData::AvenirBook_ttf, BinaryData::AvenirBook_ttfSize);
    g.setColour(juce::Colours::white);
    g.setFont(book);
    g.setFont(10);

    // Draw the text for each envelope stage
    for (int i = 0; i < num_harms_; i++)
    {
        juce::Rectangle<int> harm = getLocalBounds().withPosition({text_x_[static_cast<std::vector<int>::size_type>(i)], text_y_[static_cast<std::vector<int>::size_type>(i)]});
        std::string text = envelopeStages_[i];
        g.drawFittedText(text, harm, juce::Justification::topLeft, false);
    }

    repaintComponent(g);
}

void ADSRCompartment::drawADSRSegment(juce::Graphics& g, int x1, int y1, int x2, int y2, const juce::Colour& colour, int thickness)
{
    g.setColour(colour);
    g.drawLine(x1, y1, x2, y2, thickness);
}

void ADSRCompartment::drawFilledADSRPoints(juce::Graphics& g, int x, int y, int size)
{
    g.setColour(juce::Colours::white);
    g.fillEllipse(x - size, y - size, size * 2, size * 2);
}

void ADSRCompartment::repaintComponent(juce::Graphics& g)
{
    // Calculates the positions and movement room of each line segment
    int attackPos = static_cast<int>(ADSRSliders[0].getValue() * sliderAWidth);
    int holdPos = static_cast<int>(ADSRSliders[1].getValue() * sliderHWidth);
    int decayPos = static_cast<int>(ADSRSliders[2].getValue() * sliderDWidth);
    int sustainPos = static_cast<int>(ADSRSliders[3].getValue() * sliderSWidth);
    int decay2Pos = static_cast<int>(ADSRSliders[4].getValue() * sliderD2Width);
    int releasePos = static_cast<int>(ADSRSliders[5].getValue() * sliderRWidth);

    // Calculate the heights of the ADSR segments while respecting the bounds
    int attackHeight = std::min(static_cast<int>(ADSRSliders[0].getValue() * sliderAWidth), maxY - yLocation);
    attackHeight += 90; // shortening line length

    // Attack segment
    int aX1 = xLocation;
    int aY1 = yLocation;
    int aX2 = xLocation + attackPos;
    int aY2 = yLocation + attackHeight;

    // Hold segment
    int hX1 = aX2;
    int hY1 = aY2;
    int hX2 = xLocation + attackPos + holdPos;
    int hY2 = aY2;

    // Decay segment
    int dX1 = hX2;
    int dY1 = hY2;
    int dX2 = xLocation + attackPos + holdPos + decayPos;
    int dY2 = yLocation - attackHeight - 165;

    // Sustain segment
    int sX1 = dX2;
    int sY1 = dY2;
    int sX2 = xLocation + attackPos + holdPos + decayPos + sustainPos;
    int sY2 = dY2;

    // Decay2 segment
    int d2X1 = sX2;
    int d2Y1 = sY2;
    int d2X2 = xLocation + attackPos + holdPos + decayPos + sustainPos + decay2Pos;
    int d2Y2 = dY2 - attackHeight - 60;

    // Release segment
    int rX1 = d2X2;
    int rY1 = d2Y2;
    int rX2 = xLocation + attackPos + holdPos + decayPos + sustainPos + decay2Pos + releasePos;
    int rY2 = d2Y2 - attackHeight - 60;

    std::array<int, numSliders> adsrLineX1{aX1, hX1, dX1, sX1, d2X1, rX1};
    std::array<int, numSliders> adsrLineY1{aY1, hY1, dY1, sY1, d2Y1, rY1};
    std::array<int, numSliders> adsrLineX2{aX2, hX2, dX2, sX2, d2X2, rX2};
    std::array<int, numSliders> adsrLineY2{aY2, hY2, dY2, sY2, d2Y2, rY2};
    for (unsigned int i = 0; i < numSliders; ++i)
    {
        drawADSRSegment(g, adsrLineX1[i], adsrLineY1[i], adsrLineX2[i], adsrLineY2[i], colour_scheme_[i].brighter(0.75f), segmentThickness);
        drawFilledADSRPoints(g, adsrLineX1[i], adsrLineY1[i], segmentThickness);
        drawFilledADSRPoints(g, adsrLineX2[i], adsrLineY2[i], segmentThickness);
    }
}

void ADSRCompartment::resized()
{
    for (unsigned int i = 0; i < numSliders; ++i)
    {
        auto a = getLocalBounds().withWidth(80).withHeight(80).withX(slider_x_[i]);
        ADSRSliders[i].setBounds(a.withY(slider_y_[i]));
    }
    auto a = getLocalBounds().withWidth(30).withHeight(30).withX(50);
    osc.setBounds(a.withY(390));
}

void ADSRCompartment::sliderValueChanged(juce::Slider* /*slider*/)
{
    // TODO: Implement the logic for handling slider value changes
}
