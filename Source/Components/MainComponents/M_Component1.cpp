#include "M_Component1.h"
MainVCompartment::MainVCompartment (BASAudioProcessor& p)
    : gainSliders (numSliders), audioProcessor_ (p), sliderArray_ (numSliders), gainAttachments_ (numSliders)

{
    for (std::size_t i = 0; i < numSliders; ++i) // Assuming numSliders == number of oscillators
    {
        auto* comboBox = new juce::ComboBox("WaveTypeSelector" + juce::String(i));
        comboBox->addItem("Sine", 1);
        comboBox->addItem("Triangle", 2);
        comboBox->addItem("Square", 3);
        comboBox->addItem("Saw", 4);
        comboBox->addListener(this);
        comboBox->setSelectedId(1, juce::dontSendNotification); // Default to sine
        waveTypeSelectors.add(comboBox);
        addAndMakeVisible(comboBox);
    }
    for (int i = 0; i < 33; ++i)
    {
    }

    slider_x_[0] = 2.5f; // Starting value
    box_x_[0]=27.5f;

    for (int i = 1; i < 34; ++i)
    {
        slider_x_[i] = slider_x_[i - 1] + 16.0f; // Add 20 to the previous element
        box_x_[i]=box_x_[i-1]+16.0f;
    }

    for (std::size_t i = 0; i < numSliders; ++i)
    {
        sliderArray_[i] = &gainSliders[i];
    }

    const auto getGainSliderColour = [&] (int index)
    {
        return colour_scheme_[index];
    };

    static const auto getGainSliderText = [&] (int index)
    {
        return numbers_[static_cast<std::size_t> (index)];
    };

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    static const auto createSliderAttachment =
        [] (juce::AudioProcessorValueTreeState& tree,
            std::unique_ptr<SliderAttachment>& attachment,
            juce::String paramID,
            juce::Slider& slider)
    {
        attachment = std::make_unique<SliderAttachment> (tree, paramID, slider);
    };

    setAlwaysOnTop (false);
    static auto book = juce::Typeface::createSystemTypefaceFor (
        BinaryData::AvenirBook_ttf, BinaryData::AvenirBook_ttfSize);

    // Setup each slider with their colours etc.
    for (auto i = 0; i < numSliders; ++i)
    {
        gainSliders[static_cast<std::vector<BastowGainSlider>::size_type> (i)]
            .addListener (this);
        gainSliders[static_cast<std::vector<BastowGainSlider>::size_type> (i)]
            .setColour (getGainSliderColour (static_cast<int> (i)));
        gainSliders[static_cast<std::vector<BastowGainSlider>::size_type> (i)]
            .setText (getGainSliderText (static_cast<int> (i)));
        gainSliders[static_cast<std::vector<BastowGainSlider>::size_type> (i)]
            .setFont (book);
        addAndMakeVisible (
            gainSliders[static_cast<std::vector<BastowGainSlider>::size_type> (i)]);

        if (i == 0)
        {
            createSliderAttachment (audioProcessor_.tree,
                                    gainAttachments_[static_cast<std::size_t> (i)],
                                    gainIdM,
                                    gainSliders[static_cast<std::size_t> (i)]);
        }
        else
        {
            createSliderAttachment (
                audioProcessor_.tree, gainAttachments_[static_cast<std::size_t> (i)], getGainId (i - 1), gainSliders[static_cast<std::size_t> (i)]);
        }
    }
}

MainVCompartment::~MainVCompartment() {}

void MainVCompartment::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    int waveType = comboBoxThatHasChanged->getSelectedId();
    int oscillatorIndex = waveTypeSelectors.indexOf(comboBoxThatHasChanged);

    // Here, you would update the oscillator's wave type.
    // This might involve calling a method on your synthesizer voice or processor.
    DBG("Oscillator " << oscillatorIndex << " set to wave type " << waveType);
}
void MainVCompartment::paint (juce::Graphics& g)
{
    // Gets background for this component
    background_ = juce::ImageCache::getFromMemory (BinaryData::M_C_png,
                                                   BinaryData::M_C_pngSize);
    g.drawImageWithin (background_, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);

    static auto black = juce::Typeface::createSystemTypefaceFor (
        BinaryData::AvenirBlack_ttf, BinaryData::AvenirBlack_ttfSize);
    g.setColour (juce::Colours::white);
    g.setFont (black);
    g.setFont (30);

    juce::Rectangle<int> s = getLocalBounds().withPosition ({ 235, 20 });
    g.drawFittedText ("BASTOW", s, juce::Justification::topLeft, false);

    juce::Rectangle<int> k = getLocalBounds().withPosition ({ 245, 70 });
    g.drawFittedText ("SYNTH", k, juce::Justification::topLeft, false);

    juce::Rectangle<int> rectangleBounds =
        getLocalBounds().withPosition ({ 230, 50 }).withSize (155, 10);
    int totalSegments = 34;
    int segmentWidth = rectangleBounds.getWidth() / totalSegments;

    for (int i = 0; i < totalSegments; i++)
    {
        int startX = rectangleBounds.getX() + (i * segmentWidth);
        int endX = startX + segmentWidth;

        // Adjust the start and end positions to avoid overlapping black border
        if (i > 0)
        {
            startX -= 1;
        }
        if (i == totalSegments - 1)
        {
            endX += 1;
        }

        juce::Rectangle<int> segmentBounds (startX, rectangleBounds.getY(), endX - startX, rectangleBounds.getHeight());
        juce::Colour color =
            sColour_Scheme_[i].withAlpha (0.5f); // Adjust the alpha value as needed
        g.setColour (color);
        g.fillRect (segmentBounds);
    }
}

void MainVCompartment::resized()
{
    for (unsigned int i = 0; i < gainSliders.size(); ++i)
    {
        auto a = getLocalBounds().withWidth (60).withHeight (325).withX (
            static_cast<int> (slider_x_[i]));
        gainSliders[i].setBounds (a.withY (115));
         // Corrected to use '->' for accessing member function of ComboBox pointer
        if (i < waveTypeSelectors.size()) { // Make sure not to access beyond the size of waveTypeSelectors
            auto boundsForSelector = getLocalBounds().withWidth(10).withHeight(20).withX(static_cast<int>(box_x_[i]));
            waveTypeSelectors.getUnchecked(i)->setBounds(boundsForSelector.withY(443)); // Using '->' for pointer
        }
    }
}

void MainVCompartment::sliderValueChanged (juce::Slider* /*slider*/) {}
