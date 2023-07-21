/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "BASPluginProcessor.h"
//==============================================================================
BASAudioProcessorEditor::BASAudioProcessorEditor (BASAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),

      // Initalises components with the audio processor
      resizableBorderComponent (&vComponent, nullptr),
      vComponent1 (audioProcessor),
      vComponent2 (audioProcessor),
      vComponent3 (audioProcessor),
      mvComponent1 (audioProcessor),
      ADSRComponent (audioProcessor),
      adsrCompartments (),
      keyboardComponent (p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)

      // Initalising Component Volume Buttons
      ,
      cvb1 (cvb1Id),
      cvb2 (cvb2Id),
      cvb3 (cvb3Id),
      cvbA1 (audioProcessor.tree, cvb1Id, cvb1),
      cvbA2 (audioProcessor.tree, cvb2Id, cvb2),
      cvbA3 (audioProcessor.tree, cvb2Id, cvb2)

{
    //==============================================================================
    //Keyboard Component
    //==============================================================================

    keyboardComponent.setAvailableRange (36, 120);
    keyboardComponent.setKeyWidth (11.0f);
    keyboardComponent.setBlackNoteWidthProportion (.5);
    keyboardComponent.setColour (keyboardComponent.blackNoteColourId, C4::lightGrey.darker (100.5));
    keyboardComponent.setColour (keyboardComponent.whiteNoteColourId, C4::lightBlack.darker (0.05f));
    keyboardComponent.setColour (keyboardComponent.textLabelColourId, C4::lightGrey);
    keyboardComponent.setColour (keyboardComponent.keySeparatorLineColourId, C4::darkGrey.darker (0.5));
    keyboardComponent.setColour (keyboardComponent.mouseOverKeyOverlayColourId, C4::lightGrey.darker (1.05f));
    keyboardComponent.setColour (keyboardComponent.keyDownOverlayColourId, C4::lightGrey.darker (5.2f));
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (929, 571);

    addMouseListener (&resizableBorderComponent, true);
    addAndMakeVisible (&vComponent1);
    addAndMakeVisible (&vComponent2);
    addAndMakeVisible (&vComponent3);
    addAndMakeVisible (&cvb1);
    addAndMakeVisible (&cvb2);
    addAndMakeVisible (&cvb3);
    addAndMakeVisible (&mvComponent1);
    addAndMakeVisible (&keyboardComponent);
    addAndMakeVisible (ADSRComponent);
    addAndMakeVisible (vComponent);

   /* for (int i = 0; i < adsrCompartments.size(); ++i)
    {
        adsrCompartments[i] = std::make_unique<ADSRCompartment>(audioProcessor);
        addAndMakeVisible(*adsrCompartments[i]);
    }*/

    // Component button volume Switch between Volume Components
    /*setButtonImages (cvb1, overlayImage1, normalImage1, downImage1);
    setButtonImages (cvb2, overlayImage2, normalImage2, downImage2);
    setButtonImages (cvb3, overlayImage3, normalImage3, downImage3);*/

    setButtonImages (cvb1);
    setButtonImages (cvb2);
    setButtonImages (cvb3);

    // Roman Book Black
    auto black = juce::Typeface::createSystemTypefaceFor (BinaryData::AvenirBook_ttf, BinaryData::AvenirBook_ttfSize);
    juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface (black);
}

BASAudioProcessorEditor::~BASAudioProcessorEditor()
{
    //delete draggableResizableComponent;
    //delete componentToManipulate;
}

void BASAudioProcessorEditor::setButtonImages(juce::TextButton& button)
{
    //button.setImages (true, true, true, overlayImage, 1.f, {}, normalImage, 1.f, {}, downImage, 1.f, {});
  
    cvb1.setButtonText("1");
    
    cvb2.setButtonText("2");
   
    cvb3.setButtonText("3");
    button.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    button.setColour(juce::TextButton::buttonOnColourId, C4::lightBlack);
    button.setColour(juce::TextButton::textColourOnId, C4::white);
    button.setColour(juce::TextButton::textColourOffId, C4::shadeGrey);
    button.setColour(juce::ComboBox::outlineColourId , juce::Colours::transparentBlack);
     
    button.setClickingTogglesState (true);
    button.isAlwaysOnTop();
    button.setAlwaysOnTop (true);

    vComponent2.setVisible (false);
    vComponent3.setVisible (false);
    button.onClick = [this, &button]()
    {
        // Sets component visiblity based on which button was clicked
        vComponent1.setVisible (&button == &cvb1);
        vComponent2.setVisible (&button == &cvb2);
        vComponent3.setVisible (&button == &cvb3);
    };
}

//==============================================================================
void BASAudioProcessorEditor::paint (juce::Graphics& g)
{
    background = juce::ImageCache::getFromMemory (BinaryData::Skeleton_png, BinaryData::Skeleton_pngSize);
    g.drawImageWithin (background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
    // g.fillAll(C4::lightBlack);
}

void BASAudioProcessorEditor::resized()
{
    // vComponent.setBounds(getLocalBounds());
    //resizableBorderComponent.setBounds(getLocalBounds());
    auto a = getLocalBounds().withWidth (123).withHeight (418).withX (10.f);
    vComponent1.setBounds (a.withY (47));
    auto b = getLocalBounds().withWidth (123).withHeight (418).withX (10.f);
    vComponent2.setBounds (b.withY (47));
    auto c = getLocalBounds().withWidth (123).withHeight (418).withX (10.f);
    vComponent3.setBounds (c.withY (47));
    auto d = getLocalBounds().withWidth (593).withHeight (473).withX (168.f);
    mvComponent1.setBounds (d.withY (0));
    auto e = getLocalBounds().withWidth (550).withHeight (60).withX (183);
    keyboardComponent.setBounds (e.withY (490));
    auto f = getLocalBounds().withWidth (20).withHeight (20).withX (30.f);
    cvb1.setBounds (f.withY (437));
    auto g = getLocalBounds().withWidth (20).withHeight (20).withX (60.f);
    cvb2.setBounds (g.withY (437));
    auto h = getLocalBounds().withWidth (20).withHeight (20).withX (90.f);
    cvb3.setBounds (h.withY (437));
    auto i = getLocalBounds().withWidth (123).withHeight (420).withX (785.f);
    ADSRComponent.setBounds (i.withY (47));
    /* for (unsigned int i = 0; i < adsrCompartments.size(); ++i)
    {
        auto compartmentBounds = getLocalBounds().withWidth (80).withHeight (80).withX (785.f);
        adsrCompartments[i]->setBounds (compartmentBounds.withY (47));
    }*/
    
}
