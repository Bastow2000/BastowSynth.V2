#include "GainSLAF.h"

GainSliderLookAndFeel::GainSliderLookAndFeel()
{
    // Create system typeface for AvenirBook
    static auto book = juce::Typeface::createSystemTypefaceFor(BinaryData::AvenirBook_ttf, BinaryData::AvenirBook_ttfSize);
}

GainSliderLookAndFeel::~GainSliderLookAndFeel() {}

void GainSliderLookAndFeel::drawCross(juce::Graphics& g, int crossX, int crossY, int crossSize, juce::Colour colour)
{
    g.setColour(colour);

    juce::Rectangle<int> crossBounds(crossX - crossSize, crossY - crossSize, crossSize * 2, crossSize * 2);

    // Draw vertical line
    g.drawLine(crossBounds.getX() + crossBounds.getWidth() / 2, crossBounds.getY(), crossBounds.getX() + crossBounds.getWidth() / 2, crossBounds.getBottom(), 1);

    // Draw horizontal line
    g.drawLine(crossBounds.getX(), crossBounds.getY() + crossBounds.getHeight() / 2, crossBounds.getRight(), crossBounds.getY() + crossBounds.getHeight() / 2, 1);
}

void GainSliderLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle /*style*/, juce::Slider& slider)
{
    //=============================================================================
    // Create Background Track for Slider
    //=============================================================================
    auto trackWidthBackground = juce::jmin(12.5f, (float)height);

    juce::Point<float> startPoint(slider.isHorizontal() ? (float)x : (float)x + (float)width * 0.5f,
        slider.isHorizontal() ? (float)y + (float)height * 0.5f : (float)(height + y));

    juce::Point<float> endPoint(slider.isHorizontal() ? (float)(width + x) : startPoint.x,
        slider.isHorizontal() ? startPoint.y : (float)y);

    juce::Path track;
    track.startNewSubPath(startPoint);
    track.lineTo(endPoint);

    g.setColour(C4::darkGrey.withAlpha(0.01f));
    g.strokePath(track, { trackWidthBackground, juce::PathStrokeType::mitered, juce::PathStrokeType::butt });

    //==============================================================================
    // Create Border Track for Slider
    //==============================================================================
    auto trackWidthBorder = juce::jmin(15.5f, (float)height);

    g.setColour(C4::lightblack.darker(0.32f));
    juce::Path borderTrack;
    juce::Point<float> minPoint, maxPoint, thumbPoint;

    minPoint = { minSliderPos, (float)height * 0.5f };
    maxPoint = { maxSliderPos, (float)height * 0.5f };
    thumbPoint = { sliderPos, (float)height * 0.5f };

    auto kx = slider.isHorizontal() ? sliderPos : ((float)x + (float)width * 0.5f);
    auto ky = slider.isHorizontal() ? ((float)y + (float)height * 0.5f) : sliderPos;

    minPoint = startPoint;
    maxPoint = { kx, ky };

    auto minThumbWidth = getSliderThumbRadius(slider) + 5;
    auto maxThumbWidth = getSliderThumbRadius(slider);

    borderTrack.startNewSubPath(minPoint);
    borderTrack.lineTo(maxPoint);

    g.strokePath(borderTrack, { trackWidthBorder, juce::PathStrokeType::mitered, juce::PathStrokeType::butt });

    //==============================================================================
    // Create Track for Slider
    //==============================================================================
    auto trackWidthGain = juce::jmin(10.5f, (float)height);

    g.setColour(mainSliderColour_);
    juce::Path mainTrack;

    mainTrack.startNewSubPath(minPoint);
    mainTrack.lineTo(maxPoint);

    g.strokePath(mainTrack, { trackWidthGain, juce::PathStrokeType::mitered, juce::PathStrokeType::butt });

    //==============================================================================
    // Draw Thumb Rest
    //==============================================================================

    g.setColour(C4::lightblack.darker(0.32f));
    float rectHeight = 50.0f; // Adjust the desired height as needed
    float rectWidth = 14.5f;
    float rectHeight2 = 55.0f;
    float rectWidth2 = 16.0f;

    juce::Rectangle<float> backRect(static_cast<float>(x ? maxThumbWidth : minThumbWidth),
        static_cast<float>(x ? maxThumbWidth : minThumbWidth));
    backRect.setSize(rectWidth2, rectHeight2);
    backRect.setCentre(x ? startPoint : minPoint);

    g.drawRect(backRect, 18.5f);

    backRect.expand(8.0f, 8.0f);

    slider.setMouseCursor(juce::MouseCursor::NoCursor);

    //==============================================================================
    // Decoration for Thumb Rest
    //==============================================================================
    g.setColour(mainSliderColour_.withAlpha(0.3f));
    juce::Rectangle<float> rect(static_cast<float>(x ? maxThumbWidth : minThumbWidth),
        static_cast<float>(x ? maxThumbWidth : minThumbWidth));
    rect.setSize(rectWidth, rectHeight);
    rect.setCentre(x ? startPoint : minPoint);

    g.drawRect(rect, 18.5f);

    float crossX = rect.getCentreX();
    float crossY = rect.getCentreY() + 7.f;

    float text_x = rect.getCentreX() - 4;
    float text_y = rect.getCentreY() - 20.f;

    for (int i = 0; i < textSize_; i++)
    {
        g.setColour(juce::Colours::white);
        juce::Rectangle<float> harm(text_x, text_y, 10.0f, 10.0f);

        g.setFont(10.f);
        g.drawFittedText(mainText_, harm.toNearestInt(), juce::Justification::topLeft, false);
    }

    for (int i = 0; i < colourSchemeSize_; ++i)
    {
        drawCross(g, static_cast<int>(crossX), static_cast<int>(crossY), crossSize_, mainSliderColour_.brighter(0.5f));
    }
    slider.setMouseCursor(juce::MouseCursor::NormalCursor);
}

float GainSliderLookAndFeel::thumbPosition(int x)
{
    return x ? maxPoint_ : maxPoint_;
}
