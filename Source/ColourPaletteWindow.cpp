/*
  ==============================================================================

    ColourPaletteWindow.cpp
    Created: 14 Dec 2020 11:32:03pm
    Author:  Vincenzo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ColourPaletteWindow.h"

//==============================================================================
// ColourPaletteWindow Definitions

ColourPaletteWindow::ColourPaletteWindow(const Array<Array<Colour>>& colourPalettesIn)
    : paletteColours(colourPalettesIn)
{
    setName("ColourPaletteWindow");

    // Generate palettes that are filled with saved palettes, or filler palettes to preserve layout
    for (auto p : paletteColours)
    {
        generateFilledPaletteComponents(p);
    }

    // Empty palette always in last index
    palettes.add(new ColourPaletteComponent("EmptyPalette"));
    auto newPaletteButton = editButtons.add(new TextButton("NewButton", translate("NewPaletteTip")));
    newPaletteButton->setButtonText(translate("NewPalette"));
    newPaletteButton->getProperties().set(LumatoneEditorStyleIDs::textButtonHyperlinkFlag, 1);
    newPaletteButton->addListener(this);

    palettePanel.reset(new ColourPalettesPanel(palettes, editButtons, trashButtons));

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

    paletteGroup.addColourSelectionListener(this);
    paletteGroup.addSelector((ColourSelectionBroadcaster*)customPickerPanel.get());

    colourToolTabs->getTabbedButtonBar().addChangeListener(this);
}

ColourPaletteWindow::~ColourPaletteWindow()
{
}

int ColourPaletteWindow::generateFilledPaletteComponents(Array<Colour> coloursIn)
{
    int newIndex = jmax(0, palettes.size() - 1);
    String iStr = String(newIndex);

    auto p = palettes.insert(newIndex, new ColourPaletteComponent("Palette_" + iStr, coloursIn));
    paletteGroup.addSelector(p);

    auto edit = editButtons.insert(newIndex, new TextButton("EditButton" + iStr, translate("EditButtonTip")));
    edit->setButtonText("Edit");
    edit->getProperties().set(LumatoneEditorStyleIDs::textButtonHyperlinkFlag, 1);
    edit->addListener(this);

    auto trash = trashButtons.insert(newIndex, new ImageButton("TrashButton_" + iStr));
    trash->setImages(false, true, true,
        trashCanIcon, 1.0f, Colour(),
        trashCanIcon, 1.0f, Colours::white.withAlpha(0.4f),
        trashCanIcon, 1.0f, Colour()
    );
    trash->addListener(this);

    return newIndex;
}

void ColourPaletteWindow::paint (juce::Graphics& g)
{
    //g.fillAll(Colours::black);
    
    //g.setColour(Colours::aqua);
    //g.drawRect(palettePanelViewport->getBounds()
    //     .withWidth(palettePanelViewport->getMaximumVisibleWidth())
    //    .withHeight(palettePanelViewport->getMaximumVisibleHeight())
    //);
}

void ColourPaletteWindow::resized()
{
    palettePanelViewport->setScrollBarThickness(proportionOfWidth(viewportScrollbarWidthScalar));
    colourToolTabs->setBounds(getLocalBounds());

    if (paletteEditPanel.get())
        paletteEditPanel->setBounds(getLocalBounds());

    // We shouldn't have to subtract vertical scrollbar width but it's 
    // the only way I could get the horizontal scrollbar to not appear
    palettePanel->setViewUnits(
        palettePanelViewport->getMaximumVisibleWidth(),// - palettePanelViewport->getScrollBarThickness(),
        palettePanelViewport->getMaximumVisibleHeight()// - palettePanelViewport->getScrollBarThickness()
    );
    palettePanel->rebuildPanel();
}

void ColourPaletteWindow::startEditingPalette(int paletteIndexIn)
{
    paletteIndexEditing = paletteIndexIn;
    paletteEditPanel.reset(new PaletteEditPanel(palettes[paletteIndexIn]->getColourPalette()));
    paletteEditPanel->setBounds(getLocalBounds());
    addAndMakeVisible(*paletteEditPanel);
    paletteEditPanel->addChangeListener(this);
}

void ColourPaletteWindow::removePalette(int paletteIndexToRemove)
{
    paletteGroup.removeSelector(palettes[paletteIndexToRemove]);
    palettes.remove(paletteIndexToRemove);
    editButtons.remove(paletteIndexToRemove);
    trashButtons.remove(paletteIndexToRemove);
    palettePanel->rebuildPanel();
}

void ColourPaletteWindow::buttonClicked(Button* btn)
{
    // Begin editing a palette
    if (btn->getName().startsWith("Edit"))
    {
        int paletteIndex = editButtons.indexOf((TextButton*)btn);
        if (paletteIndex >= 0 && paletteIndex < editButtons.size())
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
        startEditingPalette(
            generateFilledPaletteComponents()
        );
    }

    // Delete palette
    else if (btn->getName().startsWith("Trash"))
    {
        int paletteIndex = trashButtons.indexOf((ImageButton*)btn);
        if (paletteIndex >= 0 && paletteIndex < palettes.size() - 1)
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
            if (paletteIndexEditing >= 0 && paletteIndexEditing < palettes.size())
            {
                palettes.getUnchecked(paletteIndexEditing)->setColourPalette(paletteEditPanel->getCurrentPalette());

                if (paletteEditingIsNew)
                    palettePanel->rebuildPanel();
            }
            else
                jassert(true); // Something bad happened!
        }
        else if (paletteEditingIsNew)
            removePalette(paletteIndexEditing);

        paletteIndexEditing = -1;
        paletteEditingIsNew = false;
        paletteEditPanel = nullptr;
    }
}

void ColourPaletteWindow::colourChangedCallback(ColourSelectionBroadcaster* source, Colour newColour)
{
    // TODO
}

void ColourPaletteWindow::listenToColourSelection(ColourSelectionListener* listenerIn)
{
    paletteGroup.addColourSelectionListener(listenerIn);
}
