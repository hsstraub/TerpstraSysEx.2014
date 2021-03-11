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
    : colourPalettes(colourPalettesIn)
{
    setName("ColourPaletteWindow");

    paletteGroup.reset(new ColourSelectionGroup());
    
    palettePanel.reset(new ColourPalettesPanel(colourPalettes, paletteGroup.get()));
    palettePanel->addListener(this);

    palettePanelViewport.reset(new Viewport("PalettePanelViewport"));
    palettePanelViewport->setViewedComponent(palettePanel.get(), false);
    palettePanelViewport->setScrollBarsShown(true, false, true, false);
    palettePanelViewport->getVerticalScrollBar().setColour(ScrollBar::ColourIds::thumbColourId, Colour(0xff2d3135));

    customPickerPanel.reset(new CustomPickerPanel());
    paletteGroup->addSelector(customPickerPanel.get());

    colourToolTabs.reset(new TabbedComponent(TabbedButtonBar::Orientation::TabsAtTop));
    colourToolTabs->setName("ColourSelectionToolTabs");
    colourToolTabs->addTab(translate("ColourPalettes"), Colour(), palettePanelViewport.get(), false);
    colourToolTabs->addTab(translate("CustomPicker"), Colour(), customPickerPanel.get(), false);
    colourToolTabs->setColour(TabbedComponent::ColourIds::outlineColourId, Colour());
    addAndMakeVisible(*colourToolTabs);

    colourToolTabs->getTabbedButtonBar().addChangeListener(this);
}

ColourPaletteWindow::~ColourPaletteWindow()
{ 
    paletteGroup->removeSelector(customPickerPanel.get());

    palettePanelViewport    = nullptr;
    colourToolTabs          = nullptr;
    palettePanel            = nullptr;
    customPickerPanel       = nullptr;
    paletteEditPanel        = nullptr;
}

void ColourPaletteWindow::resized()
{
    palettePanelViewport->setScrollBarThickness(proportionOfWidth(viewportScrollbarWidthScalar));
    colourToolTabs->setBounds(getLocalBounds());

    if (paletteEditPanel.get())
        paletteEditPanel->setBounds(getLocalBounds());

    palettePanel->setViewUnits(
        palettePanelViewport->getMaximumVisibleWidth(),
        palettePanelViewport->getMaximumVisibleHeight()
    );
    palettePanel->rebuildPanel(colourPalettes);
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
    startEditingPalette(0);
}

void ColourPaletteWindow::changeListenerCallback(ChangeBroadcaster* source)
{
    // Custom picker colour changed
    if (source == &colourToolTabs->getTabbedButtonBar())
    {
        customPickerPanel->setCurrentColour(paletteGroup->getSelectedColour());
    }

    // Palette editing finished
    else if (source == paletteEditPanel.get())
    {
        if (paletteEditPanel->wasSaveRequested())
        {
            if (paletteIndexEditing >= 0 && paletteIndexEditing < colourPalettes.size())
            {
                auto palette = colourPalettes.getReference(paletteIndexEditing);
                palette.setColours(paletteEditPanel->getCurrentPalette());

                String newName = paletteEditPanel->getPaletteName();

                // TODO: Move this elsewhere?
                String paletteFilePath = palette.getPathToFile();
                File paletteFile;
                // File name handling if palette already exists
                if (File::isAbsolutePath(paletteFilePath))
                {
                    paletteFile = paletteFilePath;

                    if (palette.getName() != newName)
                    {
                        // Delete previous version to ensure name is up to date.
                        // This is optional - palettes can retain file name and have palette 
                        // name changed if that's preferred behavior
                        if (paletteFile.existsAsFile())
                        {
                            paletteFile.deleteFile();

                            // Rename file with new palette name
                            paletteFile = paletteFile.getSiblingFile(newName).withFileExtension(PALETTEFILEEXTENSION);
                        }
                    }
                }

                palette.setName(newName);

                // Save to properties
                TerpstraSysExApplication::getApp().saveColourPalette(palette, paletteFile);
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
}
