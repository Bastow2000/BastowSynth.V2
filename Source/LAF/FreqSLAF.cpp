#include "FreqSLAF.h"

FreqSliderLookAndFeel::FreqSliderLookAndFeel()
{
}

FreqSliderLookAndFeel::~FreqSliderLookAndFeel()
{
}

void FreqSliderLookAndFeel::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle /*style*/, juce::Slider& slider)
{
    b_ = (slider.getValue() - slider.getMinimum()) / static_cast<double> (slider.getMaximum());

    //=============================================================================
    // Creates BackgroundTrack for Slider
    //=============================================================================

    g.setColour (juce::Colours::ghostwhite.darker (0.075f));

    auto trackWidthGainBackground1 = juce::jmin (10.5f, static_cast<float> (height));

    juce::Point<float> pos1 (slider.isHorizontal() ? static_cast<float> (x) : static_cast<float> (x) + static_cast<float> (width) * 0.5f,
                             slider.isHorizontal() ? static_cast<float> (y) + static_cast<float> (height) * 0.5f : static_cast<float> (height + y));

    juce::Point<float> pos2 (slider.isHorizontal() ? static_cast<float> (width + x) : pos1.x,
                             slider.isHorizontal() ? pos1.y : static_cast<float> (y));

    juce::Path backgroundTrack;
    backgroundTrack.startNewSubPath (pos1);
    backgroundTrack.lineTo (pos2);

    g.setColour (juce::Colours::ghostwhite.darker (0.1f));
    g.strokePath (backgroundTrack, { trackWidthGainBackground1, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

    //==============================================================================
    // Creates Track for Slider
    //==============================================================================

    auto trackWidthGain1 = juce::jmin (5.5f, static_cast<float> (height));

    juce::Point<float> startPoint (slider.isHorizontal() ? static_cast<float> (x) : static_cast<float> (x) + static_cast<float> (width) * 0.5f,
                                   slider.isHorizontal() ? static_cast<float> (y) + static_cast<float> (height) * 0.5f : static_cast<float> (height + y));

    juce::Point<float> endPoint (slider.isHorizontal() ? static_cast<float> (width + x) : startPoint.x,
                                 slider.isHorizontal() ? startPoint.y : static_cast<float> (y));

    juce::Path track;
    track.startNewSubPath (startPoint);
    track.lineTo (endPoint);

    g.setColour (mainSliderColour_);
    g.strokePath (track, { trackWidthGain1, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

    //==============================================================================
    // Creates dotted Track for slider
    //==============================================================================

    auto brightTrackWidthGain1 = juce::jmin (5.5f, static_cast<float> (height));
    juce::Path brightTrack;
    juce::Point<float> minPoint, maxPoint, thumbPoint;

    minPoint = { minSliderPos, static_cast<float> (height) * 0.5f };
    maxPoint = { maxSliderPos, static_cast<float> (height) * 0.5f };
    thumbPoint = { sliderPos, static_cast<float> (height) * 0.5f };

    auto kx = slider.isHorizontal() ? sliderPos : (static_cast<float> (x) + static_cast<float> (width) * 0.5f);
    auto ky = slider.isHorizontal() ? (static_cast<float> (y) + static_cast<float> (height) * 0.5f) : sliderPos;

    minPoint = startPoint;
    maxPoint = { kx, ky };

    auto minThumbWidth = getSliderThumbRadius (slider) - 7.8;
    auto maxThumbWidth = getSliderThumbRadius (slider) - 8.6f;

    brightTrack.startNewSubPath (minPoint);
    brightTrack.lineTo (maxPoint);

     if (mainSliderColour_ != C1::grey && mainSliderColour_ != C2::grey && mainSliderColour_ != C3::grey)
    {
       g.setColour (mainSliderColour_.withSaturation (b_ * 0.9f).darker (0.075f));
    }
    else
    {
        // Set a fallback color for grey
        g.setColour (mainSliderColour_);
    }
   
    g.strokePath (brightTrack, { brightTrackWidthGain1, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

    //==============================================================================
    // Draws Thumb Rest
    //==============================================================================

    g.setColour (juce::Colours::ghostwhite.darker (0.075f));
    g.drawEllipse (juce::Rectangle<float> (static_cast<float> (x ? maxThumbWidth : minThumbWidth),
                                           static_cast<float> (x ? maxThumbWidth : minThumbWidth))
                       .withCentre (x ? startPoint : minPoint),
                   9.5f);

    slider.setMouseCursor (juce::MouseCursor::NoCursor);

    //==============================================================================
    // Decoration for Thumb Rest
    //==============================================================================

    g.setColour (mainSliderColour_);
    g.drawEllipse (juce::Rectangle<float> (static_cast<float> (x ? 0.5 * maxThumbWidth : 0.5 * minThumbWidth),
                                           static_cast<float> (x ? 0.5 * maxThumbWidth : 0.5 * minThumbWidth))
                       .withCentre (x ? startPoint : minPoint),
                   6.25f);

    if (mainSliderColour_ != C1::grey && mainSliderColour_ != C2::grey && mainSliderColour_ != C3::grey)
    {
       // g.setColour (mainSliderColour_.withSaturation (b_ * 0.7f).darker (0.075f));
    }
    else
    {
        // Set a fallback color for grey
        g.setColour (mainSliderColour_);
    }
    g.drawEllipse (juce::Rectangle<float> (static_cast<float> (x ? maxThumbWidth : minThumbWidth),
                                           static_cast<float> (x ? maxThumbWidth : minThumbWidth))
                       .withCentre (x ? startPoint : minPoint),
                   2.75f);

    g.setColour (mainSliderColour_.darker (2.5f));
    g.drawEllipse (juce::Rectangle<float> (static_cast<float> (x ? 0.5 * maxThumbWidth : 0.5 * minThumbWidth),
                                           static_cast<float> (x ? 0.5 * maxThumbWidth : 0.5 * minThumbWidth))
                       .withCentre (x ? maxPoint : minPoint),
                   9.25);

    if (mainSliderColour_ != C1::grey && mainSliderColour_ != C2::grey && mainSliderColour_ != C3::grey)
    {
        g.setColour (mainSliderColour_.withSaturation (b_ * 0.7f));
    }
    else
    {
        // Set a fallback color for grey
        g.setColour (mainSliderColour_.brighter (5.5f));
    }
    g.drawEllipse (juce::Rectangle<float> (static_cast<float> (x ? 0.5 * maxThumbWidth : 0.5 * minThumbWidth),
                                           static_cast<float> (x ? 0.5 * maxThumbWidth : 0.5 * minThumbWidth))
                       .withCentre (x ? maxPoint : minPoint),
                   3.75);

    slider.setMouseCursor (juce::MouseCursor::NormalCursor);
}

float FreqSliderLookAndFeel::thumbPosition (int x)
{
    return x ? maxPoint_ : maxPoint_;
}
