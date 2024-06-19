#include "ADSRsLAF.h"

ADSRSliderLookAndFeel::ADSRSliderLookAndFeel() {}

ADSRSliderLookAndFeel::~ADSRSliderLookAndFeel() {}

juce::Slider::SliderLayout ADSRSliderLookAndFeel::getSliderLayout(juce::Slider& slider)
{
    auto localBounds = slider.getLocalBounds();
    
    juce::Slider::SliderLayout layout;
    layout.sliderBounds = localBounds;
    
    return layout;
}

void ADSRSliderLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                             const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    // Calculate changing value
    double b = (slider.getValue() - slider.getMinimum()) / static_cast<double>(slider.getMaximum());
    
    // Rotary Variables
    auto bounds = juce::Rectangle<float>(x, y, width, height).reduced(30.5f);
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto verySmallLine = radius * 0.0085f;
    auto smallLine = radius * 0.125f;
    auto SMLine = radius * 0.225f;
    auto mediumLine = radius * 0.325f;
    auto bigLine = radius * 0.55f;
    auto arcRadius = radius - verySmallLine * 1.5f;
    
    // Background Track
    g.setColour(C4::white.brighter(0.5f));
    juce::Path backgroundTrack;
    backgroundTrack.addCentredArc(bounds.getCentreX(),
                                  bounds.getCentreY(),
                                  arcRadius,
                                  arcRadius,
                                  0.0f,
                                  rotaryStartAngle,
                                  rotaryEndAngle,
                                  true);
    g.strokePath(backgroundTrack, juce::PathStrokeType(bigLine + 0.5, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    // Decoration Track
    juce::Path decorationTrack;
    decorationTrack.addCentredArc(bounds.getCentreX(),
                                  bounds.getCentreY(),
                                  arcRadius,
                                  arcRadius,
                                  0.0f,
                                  rotaryStartAngle,
                                  rotaryEndAngle,
                                  true);
    g.setColour(mainSliderColour_.darker(0.075f).withSaturation(b * 0.9f));
    g.strokePath(decorationTrack, juce::PathStrokeType(mediumLine, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    // Background Circle
    g.setColour(juce::Colours::ghostwhite.darker(0.15f));
    
    // Covers
    g.setColour(C4::offWhite.brighter(5.5f));
    juce::Path Cover1;
    Cover1.addCentredArc(bounds.getCentreX(),
                         bounds.getCentreY(),
                         arcRadius,
                         arcRadius,
                         0.0f,
                         rotaryStartAngle,
                         rotaryEndAngle * 0.5,
                         true);
    g.setColour(C4::offWhite.brighter(5.5f));
    g.strokePath(Cover1, juce::PathStrokeType(bigLine + 1.5, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    g.setColour(C4::offWhite.brighter(5.5f));
    juce::Path Cover2;
    Cover2.addCentredArc(bounds.getCentreX(),
                         bounds.getCentreY(),
                         arcRadius,
                         arcRadius,
                         0.0f,
                         rotaryStartAngle * 0.5,
                         rotaryEndAngle * 0.275,
                         true);
    g.strokePath(Cover2, juce::PathStrokeType(bigLine + 1.5, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    // Coloured Covers
    g.setColour(mainSliderColour_.withSaturation(b * 0.9f));
    juce::Path yellowCover;
    yellowCover.addCentredArc(bounds.getCentreX(),
                              bounds.getCentreY(),
                              arcRadius,
                              arcRadius,
                              0.0f,
                              rotaryStartAngle * 0.5,
                              rotaryEndAngle * 0.275,
                              true);
    g.strokePath(yellowCover, juce::PathStrokeType(bigLine - 1.5, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    g.setColour(mainSliderColour_);
    juce::Path yellowCover2;
    yellowCover2.addCentredArc(bounds.getCentreX(),
                               bounds.getCentreY(),
                               arcRadius,
                               arcRadius,
                               0.0f,
                               rotaryStartAngle,
                               rotaryEndAngle * 0.5,
                               true);
    g.strokePath(yellowCover2, juce::PathStrokeType(bigLine - 1.5, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    g.setColour(mainSliderColour_);
    juce::Path yellowCover3;
    yellowCover3.addCentredArc(bounds.getCentreX(),
                               bounds.getCentreY(),
                               arcRadius,
                               arcRadius,
                               0.0f,
                               rotaryStartAngle * 0.65,
                               rotaryEndAngle * 0.1,
                               true);
    
    // Thumb
    juce::Path thumb;
    auto thumbWidth = mediumLine * 2.0f;
    thumb.addEllipse(-thumbWidth / 2, -thumbWidth / 2, thumbWidth, radius + mediumLine);
    g.setColour(juce::Colours::ghostwhite.darker(0.075f));
    g.setColour(C4::darkGrey.darker(0.075f));
    g.fillPath(thumb, juce::AffineTransform::rotation(toAngle + 3.12f).translated(bounds.getCentre()));
    
    juce::Path thumb2;
    auto thumbWidth2 = verySmallLine * 15.0f;
    thumb2.addEllipse(-thumbWidth2 / 2, -thumbWidth2 / 2, thumbWidth2, radius + verySmallLine);
    g.setColour(juce::Colours::ghostwhite.darker(0.075f));
    g.setColour(mainSliderColour_.withSaturation(b * 0.5f));
    g.fillPath(thumb2, juce::AffineTransform::rotation(toAngle + 3.12f).translated(bounds.getCentre()));
}
