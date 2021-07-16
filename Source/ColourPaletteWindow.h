/*
  ==============================================================================

    ColourPaletteWindow.h
    Created: 14 Dec 2020 11:32:03pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LumatoneEditorStyleCommon.h"
#include "ColourPaletteComponent.h"
#include "ColourSelectionPanels.h"
#include "ColourPaletteDataStructure.h"

//==============================================================================
/*
*/
class ColourPaletteWindow  :    public juce::Component,
                                public ChangeListener,
                                public ColourPalettesPanel::Listener
{
public:
    ColourPaletteWindow(Array<LumatoneEditorColourPalette>& colourPalettesIn);
    ~ColourPaletteWindow() override;

    void resized() override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

    //====================================================================

    /// <summary>
    /// Registers a listener to receive the colour the user's input resolves to
    /// </summary>
    /// <param name="listenerIn"></param>
    void listenToColourSelection(ColourSelectionListener* listenerIn) { colourSelectorGroup->addColourSelectionListener(listenerIn); }

    /// <summary>
    /// Adds a colour selector component to the colour selection group
    /// </summary>
    /// <param name="broadcaster"></param>
    void addColourSelectorToGroup(ColourSelectionBroadcaster* broadcaster) { colourSelectorGroup->addSelector(broadcaster); }

    /// <summary>
    /// Force a colour selector (added to the group) to be selected
    /// </summary>
    /// <param name="newSelector"></param>
    void setCurrentColourSelector(ColourSelectionBroadcaster* newSelector) 
    { 
        // This statement is kind of a kludge - vsicurella
        if (colourSelectorGroup->getIndexOfSelector(newSelector) < 0)
            colourSelectorGroup->addSelector(newSelector);

        colourSelectorGroup->setCurrentSelector(newSelector); 
    }

private:

    /// <summary>
    /// Launches the Palette Edit panel and listens for user interaction.
    /// </summary>
    /// <param name="paletteIn"></param>
    void startEditingPalette(int paletteIndexIn, int selectedSwatchIndex = -1);

    /// <summary>
    /// Removes a palette and associated buttons
    /// </summary>
    /// <param name="paletteIndexToRemove"></param>
    void removePalette(int paletteIndexToRemove);

protected:

    void editPaletteRequested(int paletteIndex, int selectedSwatchIndex) override;

    void deletePaletteRequested(int paletteIndex) override;

    void newPaletteRequested() override;

private:

    Array<LumatoneEditorColourPalette>& colourPalettes;

    std::unique_ptr<TabbedComponent> colourToolTabs;
    std::unique_ptr<ColourPalettesPanel> palettePanel;
    std::unique_ptr<CustomPickerPanel> customPickerPanel;
    std::unique_ptr<PaletteEditPanel> paletteEditPanel;

    std::unique_ptr<Viewport> palettePanelViewport;

    std::unique_ptr<ColourSelectionGroup> colourSelectorGroup;

    int paletteIndexEditing = -1;
    bool paletteEditingIsNew = false;

    const float viewportScrollbarWidthScalar = 0.020833f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourPaletteWindow)
};
