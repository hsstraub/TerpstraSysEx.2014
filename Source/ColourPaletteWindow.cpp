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

ColourPaletteWindow::ColourPaletteWindow(Array<LumatoneColourPalette>& colourPalettesIn)
    : colourPalettes(colourPalettesIn)
{
    setName("ColourPaletteWindow");

    // Generate palettes that are filled with saved palettes, or filler palettes to preserve layout
    for (int p = 0; p < colourPalettes.size(); p++)
    {
        LumatoneColourPalette& palette = colourPalettes.getReference(p);
        createAndListenToPaletteControls(palette);
    }

    newPaletteVisual.reset(new ColourPaletteComponent("EmptyPalette"));
    newPaletteButton.reset(new TextButton("NewButton", translate("NewPaletteTip")));
    newPaletteButton->setButtonText(translate("NewPalette"));
    newPaletteButton->getProperties().set(LumatoneEditorStyleIDs::textButtonHyperlinkFlag, 1);
    newPaletteButton->addListener(this);

    palettePanel.reset(new ColourPalettesPanel(filledPalettes, newPaletteVisual.get(), newPaletteButton.get())); 

    palettePanelViewport.reset(new Viewport("PalettePanelViewport"));
    palettePanelViewport->setViewedComponent(palettePanel.get(), false);
    palettePanelViewport->setScrollBarsShown(true, false, true, false);
    palettePanelViewport->getVerticalScrollBar().setColour(ScrollBar::ColourIds::thumbColourId, Colour(0xff2d3135));

    customPickerPanel.reset(new CustomPickerPanel());

    colourToolTabs.reset(new TabbedComponent(TabbedButtonBar::Orientation::TabsAtTop));
    colourToolTabs->setName("ColourSelectionToolTabs");
    colourToolTabs->addTab(translate("ColourPalettes"), Colour(), palettePanelViewport.get(), false);
    colourToolTabs->addTab(translate("CustomPicker"), Colour(), customPickerPanel.get(), false);
    colourToolTabs->setColour(TabbedComponent::ColourIds::outlineColourId, Colour());
    addAndMakeVisible(*colourToolTabs);

    paletteGroup.addSelector(customPickerPanel.get()); 

    colourToolTabs->getTabbedButtonBar().addChangeListener(this);
}

ColourPaletteWindow::~ColourPaletteWindow()
{
    paletteGroup.removeSelector(customPickerPanel.get());
    for (auto p : filledPalettes)
    {
        paletteGroup.removeSelector(&p->palette);
    }
    
    filledPalettes.clear();

    palettePanelViewport    = nullptr;
    colourToolTabs          = nullptr;
    palettePanel            = nullptr;
    customPickerPanel       = nullptr;
    paletteEditPanel        = nullptr;
    newPaletteVisual        = nullptr;
    newPaletteButton        = nullptr;
}

int ColourPaletteWindow::createAndListenToPaletteControls(LumatoneColourPalette& paletteIn)
{
    auto group = filledPalettes.add(new PaletteControlGroup(paletteIn));
    group->editButton.addListener(this);
    group->trashButton.addListener(this);

    paletteGroup.addSelector(&group->palette);

    return filledPalettes.size() - 1;
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
    palettePanel->rebuildPanel();
}

void ColourPaletteWindow::startEditingPalette(int paletteIndexIn)
{
    paletteIndexEditing = paletteIndexIn;
    auto group = filledPalettes[paletteIndexEditing];

    paletteEditPanel.reset(new PaletteEditPanel(group->palette.getColourPalette()));
    paletteEditPanel->setBounds(getLocalBounds());
    paletteEditPanel->setLookAndFeel(&getLookAndFeel());
    addAndMakeVisible(*paletteEditPanel);
    paletteEditPanel->addChangeListener(this);

    // Retain selected swatch
    int selectedSwatch = group->palette.getSelectedSwatchNumber();
    if (selectedSwatch >= 0)
        paletteEditPanel->setSelectedSwatch(selectedSwatch);
}

void ColourPaletteWindow::removePalette(int paletteIndexToRemove)
{
    paletteGroup.removeSelector(&filledPalettes[paletteIndexToRemove]->palette);

    filledPalettes.remove(paletteIndexToRemove);
    colourPalettes.remove(paletteIndexToRemove);
    
    TerpstraSysExApplication::getApp().getPropertiesFile()->setValue("ColourPalettes", LumatoneColourPalette::paletteArrayToString(colourPalettes));
    palettePanel->rebuildPanel();
}

int ColourPaletteWindow::findEditButtonIndex(Button* buttonIn)
{
    for (int i = 0; i < filledPalettes.size(); i++)
        if (&filledPalettes[i]->editButton == buttonIn)
            return i;
    return -1;
}

int ColourPaletteWindow::findTrashButtonIndex(Button* buttonIn)
{
    for (int i = 0; i < filledPalettes.size(); i++)
        if (&filledPalettes[i]->trashButton == buttonIn)
            return i;
    return -1;
}

void ColourPaletteWindow::buttonClicked(Button* btn)
{
    // Begin editing a palette
    if (btn->getName().startsWith("Edit"))
    {
        int paletteIndex = findEditButtonIndex(btn);
        if (paletteIndex >= 0 && paletteIndex < filledPalettes.size())
        {
            startEditingPalette(paletteIndex);
        }
        else
            jassert(true); // Something bad happened!   
    }

    // Create new palette and edit
    else if (btn->getName().startsWith("New"))
    {
        paletteEditingIsNew = true;

        colourPalettes.add(LumatoneColourPalette());

        startEditingPalette(
            createAndListenToPaletteControls(colourPalettes.getReference(colourPalettes.size() - 1))
        );
    }

    // Delete palette
    else if (btn->getName().startsWith("Trash"))
    {
        int paletteIndex = findTrashButtonIndex(btn);
        if (paletteIndex >= 0 && paletteIndex < filledPalettes.size())
        {
            removePalette(paletteIndex);
        }
        else
            jassert(true); // Something bad happened!
    }
}

void ColourPaletteWindow::changeListenerCallback(ChangeBroadcaster* source)
{
    // Custom picker colour changed
    if (source == &colourToolTabs->getTabbedButtonBar())
    {
        customPickerPanel->setCurrentColour(paletteGroup.getSelectedColour());
    }

    // Palette editting finished
    else if (source == paletteEditPanel.get())
    {
        if (paletteEditPanel->wasSaveRequested())
        {
            if (paletteIndexEditing >= 0 && paletteIndexEditing < filledPalettes.size())
            {
                filledPalettes[paletteIndexEditing]->palette.setColourPalette(paletteEditPanel->getCurrentPalette());

                if (paletteEditingIsNew)
                    palettePanel->rebuildPanel();
            }
            else
                jassert(true); // Something bad happened!

            // Save to properties
            TerpstraSysExApplication::getApp().getPropertiesFile()->setValue("ColourPalettes", LumatoneColourPalette::paletteArrayToString(colourPalettes));
        }
        else if (paletteEditingIsNew)
            removePalette(paletteIndexEditing);

        paletteIndexEditing = -1;
        paletteEditingIsNew = false;
        paletteEditPanel = nullptr;
    }
}
