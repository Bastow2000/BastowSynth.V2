/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "BASPluginProcessor.h"
#include "Component_Placement.h"
#include "M_Component1.h"
#include "V_Component1.h"
#include "V_Component2.h"
#include "V_Component3.h"
#include "ADSR_Component1.h"
#include <JuceHeader.h>

//==============================================================================
/**
 * Change the slider and harmonic colour so that its grey non in use
*/
class BASAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    BASAudioProcessorEditor (BASAudioProcessor&);
    ~BASAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    //void setButtonImages (juce::ImageButton& button, const juce::Image& overlayImage, const juce::Image& normalImage, const juce::Image& downImage);
    void setButtonImages (juce::TextButton& button);
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BASAudioProcessor& audioProcessor;

    // Sets the main background for the synth
    juce::Image background;

    // Creates a draggable component used for finding the size values faster
    MyDraggableComponent vComponent;
    MyResizableBorderComponent resizableBorderComponent;

    // Sets Components for different Volume Sliders (1)1-10,(2)11-21,(3)22-32
    ADSRCompartment ADSRComponent;
    constexpr static const int numAdsr = 11;
    std::array<std::unique_ptr<ADSRCompartment>,numAdsr> adsrCompartments;
    
    FirstVCompartment vComponent1;
    SecondVCompartment vComponent2;
    ThirdVCompartment vComponent3;
    MainVCompartment mvComponent1;

    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;
    // Component Volume Button, Changes Volume Component
    /*juce::ImageButton cvb1;
    juce::ImageButton cvb2;
    juce::ImageButton cvb3;*/
    juce::TextButton cvb1;
    juce::TextButton cvb2;
    juce::TextButton cvb3;  

   // std::array<std::unique_ptr<juce::TextButton>,numAdsr> adsrButtons;
    using tree = juce::AudioProcessorValueTreeState;
    using ButtonAttachment = tree::ButtonAttachment;
    ButtonAttachment cvbA1, cvbA2, cvbA3;

/*
    juce::Image downImage1 = juce::ImageCache::getFromMemory (BinaryData::B1_Off_png,
                                                              BinaryData::B1_Off_pngSize);
    juce::Image overlayImage1 = juce::ImageCache::getFromMemory (BinaryData::B1_Over_png,
                                                                 BinaryData::B1_Over_pngSize);
    juce::Image normalImage1 = juce::ImageCache::getFromMemory (BinaryData::B1_Norm_png,
                                                                BinaryData::B1_Norm_pngSize);

    juce::Image downImage2 = juce::ImageCache::getFromMemory (BinaryData::B2_Off_png,
                                                              BinaryData::B2_Off_pngSize);
    juce::Image overlayImage2 = juce::ImageCache::getFromMemory (BinaryData::B2_Over_png,
                                                                 BinaryData::B2_Over_pngSize);
    juce::Image normalImage2 = juce::ImageCache::getFromMemory (BinaryData::B2_Norm_png,
                                                                BinaryData::B2_Norm_pngSize);

    juce::Image downImage3 = juce::ImageCache::getFromMemory (BinaryData::B3_Off_png,
                                                              BinaryData::B3_Off_pngSize);
    juce::Image overlayImage3 = juce::ImageCache::getFromMemory (BinaryData::B3_Over_png,
                                                                 BinaryData::B3_Over_pngSize);
    juce::Image normalImage3 = juce::ImageCache::getFromMemory (BinaryData::B3_Norm_png,
                                                   BinaryData::B3_Norm_pngSize);*/

    // ThirdVCompartment* componentToManipulate = new ThirdVCompartment(audioProcessor);
    // ResizableComponent* draggableResizableComponent = new ResizableComponent(componentToManipulate);
    //MyDraggableComp draggableComponent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BASAudioProcessorEditor)
};
