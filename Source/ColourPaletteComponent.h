/*
  ==============================================================================

    ColourPaletteComponent.h
    Created: 18 Dec 2020 1:48:01am
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PolygonPalette.h"
#include "ColourSelectionGroup.h"

//==============================================================================
/*
* Inherits from PolygonPalette for Lumatone Editor specific functionality
*/
class ColourPaletteComponent  : public PolygonPalette, public ColourSelectionBroadcaster
{
public:
    ColourPaletteComponent(String name, Array<Colour> colours = Array<Colour>());
    ~ColourPaletteComponent() override;

    //==========================================================================
    // PolygonPalette overrides

    void setSelectedSwatchNumber(int swatchIndex) override;

    // Add disabled/new palette functionality
    void setColourPalette(Array<Colour> colourPaletteIn) override;

    void setSwatchColour(int swatchNumber, Colour newColour) override;

    //==========================================================================
    // ColourSelectionBroadcaster implementation

    Colour getSelectedColour() override;

    void deselectColour() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourPaletteComponent)
};
