/*
  ==============================================================================

    ColourPaletteComponent.cpp
    Created: 18 Dec 2020 1:48:01am
    Author:  Vincenzo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ColourPaletteComponent.h"

//==============================================================================
// ColourPaletteComponent Definitions

ColourPaletteComponent::ColourPaletteComponent(String name)
    : PolygonPalette(COLOURPALETTESIZE)
{
    setName(name);
    setColourPalette(Array<Colour>());
}

ColourPaletteComponent::ColourPaletteComponent(String name, Array<Colour>& colours)
    : PolygonPalette(COLOURPALETTESIZE), referencedPalette(&colours)
{
    setName(name);
    setColourPalette(colours);
}

ColourPaletteComponent::~ColourPaletteComponent()
{

}

//==========================================================================

void ColourPaletteComponent::setSelectedSwatchNumber(int swatchIndex)
{
    PolygonPalette::setSelectedSwatchNumber(swatchIndex);
    selectorListeners.call(&ColourSelectionListener::colourChangedCallback, this, getSelectedSwatchColour());
}

void ColourPaletteComponent::setColourPalette(Array<Colour> colourPaletteIn)
{
    if (colourPaletteIn.size() == 0)
    {
        for (int i = 0; i < getNumberOfSwatches(); i++)
            colourPaletteIn.add(Colour(0xff1b1b1b));

        setEnabled(false);
    }
    else
    {
        setEnabled(true);
    }

    PolygonPalette::setColourPalette(colourPaletteIn);

    if (referencedPalette)
        *referencedPalette = colourPaletteIn;

    if (isEnabled())
    {
        int selectedSwatch = getSelectedSwatchNumber();
        if (selectedSwatch >= 0 && selectedSwatch < getNumberOfSwatches())
            selectorListeners.call(&ColourSelectionListener::colourChangedCallback, this, getSelectedSwatchColour());
    }
    else
    {
        deselectColour();
    }
}

void ColourPaletteComponent::setSwatchColour(int swatchNumber, Colour newColour)
{
    PolygonPalette::setSwatchColour(swatchNumber, newColour);
    
    // Edit referenced palette
    if (referencedPalette)
        referencedPalette->set(swatchNumber, newColour);

    if (getSelectedSwatchNumber() == swatchNumber)
        selectorListeners.call(&ColourSelectionListener::colourChangedCallback, this, getSelectedSwatchColour());
}

//==========================================================================

Colour ColourPaletteComponent::getSelectedColour()
{
    return PolygonPalette::getSelectedSwatchColour();
}

void ColourPaletteComponent::deselectColour()
{
    PolygonPalette::setSelectedSwatchNumber(-1);
}
