/*
  ==============================================================================

    ColourPaletteWindow.cpp
    Created: 14 Dec 2020 11:32:03pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ColourPaletteWindow.h"
#include "Main.h"

//==============================================================================
// ColourPaletteWindow Definitions

ColourPaletteWindow::ColourPaletteWindow(Array<LumatoneEditorColourPalette>& colourPalettesIn)
    : lookAndFeel(TerpstraSysExApplication::getApp().getLookAndFeel()),
      colourPalettes(colourPalettesIn)
{
    setLookAndFeel(&lookAndFeel);
    
    setName("ColourPaletteWindow");

    colourSelectorGroup.reset(new ColourSelectionGroup());
    
    palettePanel.reset(new ColourPalettesPanel(colourPalettes, colourSelectorGroup.get()));
    palettePanel->addListener(this);

    palettePanelViewport.reset(new Viewport("PalettePanelViewport"));
    palettePanelViewport->setViewedComponent(palettePanel.get(), false);
    palettePanelViewport->setScrollBarsShown(true, false, true, false);
    palettePanelViewport->getVerticalScrollBar().setColour(ScrollBar::ColourIds::thumbColourId, Colour(0xff2d3135));

    customPickerPanel.reset(new CustomPickerPanel());
    colourSelectorGroup->addSelector(customPickerPanel.get());
    colourSelectorGroup->addColourSelectionListener(customPickerPanel.get());

    colourToolTabs.reset(new TabbedComponent(TabbedButtonBar::Orientation::TabsAtTop));
    colourToolTabs->setName("ColourSelectionToolTabs");
    colourToolTabs->addTab(translate("ColourPalettes"), Colour(), palettePanelViewport.get(), false);
    colourToolTabs->addTab(translate("CustomPicker"), Colour(), customPickerPanel.get(), false);
    colourToolTabs->setColour(TabbedComponent::ColourIds::outlineColourId, Colour());
    colourToolTabs->getTabbedButtonBar().getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, 0.9f);
    addAndMakeVisible(*colourToolTabs);
    
    const int firstTabIndex = TerpstraSysExApplication::getApp().getPropertiesFile()->getIntValue("LastColourPopupTabIndex");
    colourToolTabs->setCurrentTabIndex(firstTabIndex);
    colourToolTabs->getTabbedButtonBar().addChangeListener(this);
}

ColourPaletteWindow::~ColourPaletteWindow()
{ 
    paletteEditPanel        = nullptr;
    colourToolTabs          = nullptr;
    
    colourSelectorGroup->removeSelector(customPickerPanel.get());
    customPickerPanel       = nullptr;
    
    palettePanelViewport    = nullptr;
    palettePanel            = nullptr;
    colourSelectorGroup     = nullptr;
    
    setLookAndFeel(nullptr);
}

void ColourPaletteWindow::resized()
{
    palettePanelViewport->setScrollBarThickness(proportionOfWidth(viewportScrollbarWidthScalar));
    colourToolTabs->setBounds(getLocalBounds());

    if (paletteEditPanel.get())
        paletteEditPanel->setBounds(getLocalBounds());

    palettePanel->rebuildPanel(colourPalettes, palettePanelViewport->getMaximumVisibleWidth());
}

void ColourPaletteWindow::startEditingPalette(int paletteIndexIn, int selectedSwatchIndex)
{
    paletteIndexEditing = paletteIndexIn;
    paletteEditPanel.reset(new PaletteEditPanel(colourPalettes[paletteIndexIn]));
    paletteEditPanel->setBounds(getLocalBounds());
    paletteEditPanel->setLookAndFeel(&getLookAndFeel());
    addAndMakeVisible(*paletteEditPanel);
    paletteEditPanel->addChangeListener(this);

    // Retain selected swatch
    if (selectedSwatchIndex >= 0)
        paletteEditPanel->setSelectedSwatch(selectedSwatchIndex);
}

void ColourPaletteWindow::duplicatePalette(int paletteIndexIn)
{
    auto copiedPalette = colourPalettes[paletteIndexIn].clone();
    colourPalettes.insert(paletteIndexIn + 1, copiedPalette);
    TerpstraSysExApplication::getApp().saveColourPalette(copiedPalette);
    palettePanel->rebuildPanel(colourPalettes);
}

void ColourPaletteWindow::removePalette(int paletteIndexToRemove)
{
    // Remove loaded colour palette
    String deletedPalette = colourPalettes[paletteIndexToRemove].getPathToFile();
    colourPalettes.remove(paletteIndexToRemove);

    TerpstraSysExApplication::getApp().deletePaletteFile(deletedPalette);

    palettePanel->rebuildPanel(colourPalettes);
}

void ColourPaletteWindow::editPaletteRequested(int paletteIndex, int selectedSwatchIndex)
{
    if (paletteIndex >= 0 && paletteIndex < colourPalettes.size())
    {
        startEditingPalette(paletteIndex, selectedSwatchIndex);
    }
    else
        jassert(true); // Something bad happened!   
}

void ColourPaletteWindow::clonePaletteRequested(int paletteIndex)
{
    if (paletteIndex >= 0 && paletteIndex < colourPalettes.size())
    {
        duplicatePalette(paletteIndex);
    }
    else
        jassert(true); // Something bad happened!
}

void ColourPaletteWindow::deletePaletteRequested(int paletteIndex)
{
    if (paletteIndex >= 0 && paletteIndex < colourPalettes.size())
    {
        removePalette(paletteIndex);
    }
    else
        jassert(true); // Something bad happened!
}

void ColourPaletteWindow::newPaletteRequested()
{
    paletteEditingIsNew = true;
    colourPalettes.insert(0, LumatoneEditorColourPalette());
    startEditingPalette(0, 0);
}

void ColourPaletteWindow::changeListenerCallback(ChangeBroadcaster* source)
{
    // Palette editing finished
    if (source == paletteEditPanel.get())
    {
        if (paletteEditPanel->wasSaveRequested())
        {
            if (paletteIndexEditing >= 0 && paletteIndexEditing < colourPalettes.size())
            {
                auto palette = colourPalettes.getReference(paletteIndexEditing);
                palette.setColours(paletteEditPanel->getCurrentPalette());

                String newName = paletteEditPanel->getPaletteName();
                palette.setName(newName);

                TerpstraSysExApplication::getApp().saveColourPalette(palette);
            }
            else
                jassert(true); // Something bad happened!


            palettePanel->rebuildPanel(colourPalettes);
        }
        else if (paletteEditingIsNew)
            removePalette(paletteIndexEditing);

        paletteIndexEditing = -1;
        paletteEditingIsNew = false;
        paletteEditPanel = nullptr;
    }
    
    else if (source == &colourToolTabs->getTabbedButtonBar())
    {
        const int newTab = colourToolTabs->getCurrentTabIndex();
        TerpstraSysExApplication::getApp().getPropertiesFile()->setValue("LastColourPopupTabIndex", newTab);
    }
}
