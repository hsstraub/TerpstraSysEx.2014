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

ColourPaletteComponent::ColourPaletteComponent(String nameIn)
    : TenHexagonPalette()
{
    setName(nameIn);
    setColourPalette(Array<Colour>());
}

ColourPaletteComponent::ColourPaletteComponent(LumatoneEditorColourPalette paletteIn)
    : TenHexagonPalette()
{
    setName(paletteIn.getName());
    setColourPalette(*paletteIn.getColours());
}

ColourPaletteComponent::ColourPaletteComponent(const ColourPaletteComponent& paletteToCopy)
    : TenHexagonPalette()
{
    setName(paletteToCopy.getName());
    setColourPalette(paletteToCopy.getColourPalette());
}

ColourPaletteComponent::~ColourPaletteComponent()
{

}

//==========================================================================

void ColourPaletteComponent::setSelectedSwatchNumber(int swatchIndex)
{
    Palette::setSelectedSwatchNumber(swatchIndex);
    selectorListeners.call(&ColourSelectionListener::colourChangedCallback, this, getSelectedSwatchColour());
}

void ColourPaletteComponent::setColourPalette(Array<Colour> colourPaletteIn)
{
    if (colourPaletteIn.size() == 0)
    {
        setEnabled(false);
    }
    else
    {
        setEnabled(true);
    }

    while (colourPaletteIn.size() < getNumberOfSwatches())
    {
        colourPaletteIn.add(Colour(0xff1b1b1b));
    }

    Palette::setColourPalette(colourPaletteIn);

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
    Palette::setSwatchColour(swatchNumber, newColour);
    
    if (getSelectedSwatchNumber() == swatchNumber)
        selectorListeners.call(&ColourSelectionListener::colourChangedCallback, this, getSelectedSwatchColour());
}

//==========================================================================

Colour ColourPaletteComponent::getSelectedColour()
{
    return Palette::getSelectedSwatchColour();
}

void ColourPaletteComponent::deselectColour()
{
    Palette::setSelectedSwatchNumber(-1);
}

//==============================================================================
// PaletteControlGroup Definitions

PaletteControlGroup::PaletteControlGroup(LumatoneEditorColourPalette newPaletteIn)
    : palette(ColourPaletteComponent(newPaletteIn)),
      editButton("EditButton_" + newPaletteIn.getName(), translate("EditButtonTip")),
      cloneButton("CloneButton_" + newPaletteIn.getName()),
      trashButton("TrashButton_" + newPaletteIn.getName())
{
    editButton.setButtonText("Edit");
    editButton.getProperties().set(LumatoneEditorStyleIDs::textButtonHyperlinkFlag, 1);

    const Image cloneIcon = ImageCache::getFromHashCode(LumatoneEditorAssets::DuplicateIcon);
    cloneButton.setImages(false, true, true,
        cloneIcon, 1.0f, Colour(),
        cloneIcon, 1.0f, Colours::white.withAlpha(0.4f),
        cloneIcon, 1.0f, Colour()
    );

    const Image trashIcon = ImageCache::getFromHashCode(LumatoneEditorAssets::TrashCanIcon);
    trashButton.setImages(false, true, true,
        trashIcon, 1.0f, Colour(),
        trashIcon, 1.0f, Colours::white.withAlpha(0.4f),
        trashIcon, 1.0f, Colour()
    );
}
