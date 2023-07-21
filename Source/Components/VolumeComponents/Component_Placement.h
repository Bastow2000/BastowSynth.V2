#pragma once

#include <JuceHeader.h>

class MyResizableBorderComponent : public juce::ResizableBorderComponent
{
public:
    MyResizableBorderComponent (juce::Component* componentToResize,
                                juce::ComponentBoundsConstrainer* constrainer)
        : juce::ResizableBorderComponent (componentToResize, constrainer)
    {
        setSize (20, 20);
    }

    void resizeStarted (const juce::MouseEvent&)
    {
        originalBounds = getParentComponent()->getBounds();
    }

    void resizeEnded (const juce::MouseEvent&)
    {
        const juce::Rectangle<int> newBounds = originalBounds.withSizeKeepingCentre (getParentComponent()->getBounds().getWidth(), getParentComponent()->getBounds().getHeight());
        if (originalBounds != newBounds)
        {
            getParentComponent()->setBounds (newBounds);
        }
    }

private:
    juce::Rectangle<int> originalBounds;
};

class MyDraggableComponent : public juce::Component
{
public:
    MyDraggableComponent()
    {
        setSize (20, 20);

        addMouseListener (this, true);

        constrainer.setMinimumSize (50, 50); // Set minimum size for constraining
        constrainer.setMaximumSize (800, 600); // Set maximum size for constraining

        resizer = std::make_unique<MyResizableBorderComponent> (this, &constrainer); // Create a unique pointer to MyResizableBorderComponent
        addMouseListener (resizer.get(), true); // Add mouse listener to the resizer component
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::lightblue);
    }

    void mouseDown (const juce::MouseEvent& e) override
    {
        lastMouseDownPosition = e.getPosition();
    }

    void mouseDrag (const juce::MouseEvent& e) override
    {
        const juce::Point<int> delta (e.getPosition() - lastMouseDownPosition);
        setTopLeftPosition (getPosition() + delta);
        lastMouseDownPosition = e.getPosition();

        printComponentProperties();
    }

    void mouseUp (const juce::MouseEvent&) override
    {
        printComponentProperties();
    }

private:
    void printComponentProperties()
    {
        const juce::Rectangle<int> bounds = getBounds();
        std::cout << "X: " << bounds.getX() << std::endl;
        std::cout << "Y: " << bounds.getY() << std::endl;
        std::cout << "Width: " << bounds.getWidth() << std::endl;
        std::cout << "Height: " << bounds.getHeight() << std::endl;
    }

    juce::Point<int> lastMouseDownPosition;
    juce::ComponentBoundsConstrainer constrainer;
    std::unique_ptr<MyResizableBorderComponent> resizer;
};