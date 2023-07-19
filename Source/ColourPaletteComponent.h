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
    ColourPaletteComponent(String nameIn);
    ColourPaletteComponent(LumatoneEditorColourPalette paletteIn);
    ColourPaletteComponent(const ColourPaletteComponent& paletteToCopy);
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

};

//==============================================================================
/*
    Container object for controlling colour palettes
*/
class PaletteControlGroup
{
public:

    PaletteControlGroup(LumatoneEditorColourPalette newPaletteIn);

    ~PaletteControlGroup() {}

    ColourPaletteComponent* getPaletteComponent() { return &palette; }

    TextButton* getEditButton() { return &editButton; }

    ImageButton* getCloneButton() { return &cloneButton; }

    ImageButton* getTrashButton() { return &trashButton; }

private:

    ColourPaletteComponent palette;
    TextButton editButton;
    ImageButton cloneButton;
    ImageButton trashButton;
};
