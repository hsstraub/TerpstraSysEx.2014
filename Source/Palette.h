/*
  ==============================================================================

    PolygonPalette.h
    Created: 4 Feb 2021 11:42:00pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Palette : public juce::Component, protected juce::ChangeListener
{
public:
    Palette(int numSwatchesIn)
        : numSwatches(numSwatchesIn)
    {
        palette.resize(numSwatches);
        
        // Setup paths needed to draw swatches
    }

    virtual ~Palette() {}

    //======================================================================
    // Implementation of juce::Component

    virtual void paint(Graphics& g) = 0;
    
    virtual void resized() = 0;

    virtual void mouseDown(const MouseEvent& e) override    
    {
        if (isEnabled())
        {
            for (auto s : swatchPaths)
            {
                if (s.contains(e.position))
                {
                    setSelectedSwatchNumber(swatchPaths.indexOf(s));

                    if (selector)
                    {
                        selector->setCurrentColour(palette[selectedSwatch], dontSendNotification);
                    }

                    return;
                }
            }

            selectedSwatch = -1;
            repaint();
        }
    }

    //======================================================================
    // Getters

    int getNumberOfSwatches() const
    {
        return numSwatches;
    }

    // Returns index of selected swatch, -1 if none
    int getSelectedSwatchNumber() const
    {
        return selectedSwatch;
    } 

    Colour getSelectedSwatchColour() const
    {
        if (selectedSwatch >= 0 && selectedSwatch < numSwatches)
            return palette[selectedSwatch];

        return Colour();
    }

    Colour getSwatchColour(int swatchIndex) const
    {
        return palette[swatchIndex];
    }

    Array<Colour> getColourPalette() const
    {
        return palette;
    }

    //======================================================================
    // Setters

    virtual void setSelectedSwatchNumber(int swatchIndex)
    {
        selectedSwatch = swatchIndex;
        repaint();
    }

    virtual void setColourPalette(Array<Colour> colourPaletteIn)
    {
        palette = colourPaletteIn;
        repaint();
    }

    virtual void setSwatchColour(int swatchNumber, Colour newColour)
    {
        if (swatchNumber >= 0 && swatchNumber < numSwatches)
            palette.set(swatchNumber, newColour);
    }

    virtual void attachColourSelector(ColourSelector* selectorIn)
    {
        selector = selectorIn;
        
        if (selector)
        {
            if (selectedSwatch > -1)
            {
                selector->setCurrentColour(palette[selectedSwatch], dontSendNotification);
                //selector->setFocusserCallback([&](Component* c, var data) {
                //    palette.set(focussedSwatch, Colour::fromString(data.toString()));
                //    repaint();
                //});
            }

            selector->addChangeListener(this);
        }
    }

    //======================================================================
    // Implementation of juce::ChangeListener

    void changeListenerCallback(ChangeBroadcaster* source) override
    {
        if (source == selector && selectedSwatch > -1)
        {
            setSwatchColour(selectedSwatch, selector->getCurrentColour());
            onlyRepaintSelectedSwatch = true;
            repaint();
        }
    }

protected:

    Array<Path> swatchPaths;

    int selectedSwatch = -1;
    bool onlyRepaintSelectedSwatch = false;

private:

    const int numSwatches;

    ColourSelector* selector = nullptr;

    Array<Colour> palette =
    {
        Colours::orange,
        Colours::red,
        Colours::springgreen,
        Colours::lightsteelblue,
        Colours::yellowgreen,
        Colours::rebeccapurple
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Palette)
};
