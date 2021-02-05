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
#include "HexagonPalette.h"
#include "ColourSelectionGroup.h"
#include "ColourPaletteDataStructure.h"
#include "LumatoneEditorStyleCommon.h"

//==============================================================================
/*
* Inherits from PolygonPalette for Lumatone Editor specific functionality
*/
class ColourPaletteComponent  : public TenHexagonPalette, public ColourSelectionBroadcaster
{
public:
    ColourPaletteComponent(String name);
    ColourPaletteComponent(String name, Array<Colour>& colours);
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

    Array<Colour>* referencedPalette = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourPaletteComponent)
};

//==============================================================================
/*
    Container object for controlling colour palettes
*/
struct PaletteControlGroup
{
    PaletteControlGroup(LumatoneColourPalette& paletteIn);

    ColourPaletteComponent palette;
    TextButton editButton;
    ImageButton trashButton;
};