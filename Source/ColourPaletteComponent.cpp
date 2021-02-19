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

ColourPaletteComponent::ColourPaletteComponent(LumatoneEditorColourPalette* paletteIn)
    : TenHexagonPalette()
{
    if (paletteIn)
    {
        referencedPalette = paletteIn;

        setName(referencedPalette->name);
        setColourPalette(*referencedPalette->palette);
    }
    else
    {
        setName("ColourPaletteComponent");
        setColourPalette(Array<Colour>());
    }
}

ColourPaletteComponent::ColourPaletteComponent(const ColourPaletteComponent& paletteToCopy)
    : TenHexagonPalette()
{
    setName(paletteToCopy.getName());
    referencedPalette = paletteToCopy.referencedPalette;
    setColourPalette(*referencedPalette->palette);
}

ColourPaletteComponent::~ColourPaletteComponent()
{
    
}

//==========================================================================

String ColourPaletteComponent::getPaletteName() const
{
    if (referencedPalette)
        return referencedPalette->name;
    else
        return String();
}

void ColourPaletteComponent::setPaletteName(String nameIn)
{
    if (referencedPalette)
        referencedPalette->name = nameIn;
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
        for (int i = 0; i < getNumberOfSwatches(); i++)
            colourPaletteIn.add(Colour(0xff1b1b1b));

        setEnabled(false);
    }
    else
    {
        setEnabled(true);
    }

    Palette::setColourPalette(colourPaletteIn);

    if (referencedPalette)
        *referencedPalette->palette = colourPaletteIn;

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
    
    // Edit referenced palette
    if (referencedPalette)
        referencedPalette->palette->set(swatchNumber, newColour);

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

PaletteControlGroup::PaletteControlGroup(LumatoneEditorColourPalette& newPaletteIn)
    : palette(new ColourPaletteComponent(&newPaletteIn)),
      editButton("EditButton_" + palette->getPaletteName(), translate("EditButtonTip")),
      trashButton("TrashButton_" + palette->getPaletteName())
{
    editButton.setButtonText("Edit");
    editButton.getProperties().set(LumatoneEditorStyleIDs::textButtonHyperlinkFlag, 1);

    const Image trashIcon = ImageCache::getFromHashCode(LumatoneEditorAssets::TrashCanIcon);
    trashButton.setImages(false, true, true,
        trashIcon, 1.0f, Colour(),
        trashIcon, 1.0f, Colours::white.withAlpha(0.4f),
        trashIcon, 1.0f, Colour()
    );
}