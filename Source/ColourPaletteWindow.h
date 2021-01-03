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

//==============================================================================
/*
*/
class ColourPaletteWindow  :    public juce::Component,
                                public ColourSelectionListener,
                                public Button::Listener,
                                public ChangeListener
{
public:
    ColourPaletteWindow(/* TODO PALETTE COLOURS */const Array<Array<Colour>>& colourPalettesIn);
    ~ColourPaletteWindow() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(Button* btn) override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

    //====================================================================
    // ColourSelectionListener implementation

    void colourChangedCallback(ColourSelectionBroadcaster* source, Colour newColour) override;
    
    //====================================================================

    /// <summary>
    /// Registers a listener to receive the colour the user's input resolves to
    /// </summary>
    /// <param name="listenerIn"></param>
    void listenToColourSelection(ColourSelectionListener* listenerIn);

private:

    /// <summary>
    /// Creates a set of components for a filled palette and adds it to the arrays.
    /// </summary>
    /// <param name="coloursIn"></param>
    /// <returns>Index of new palette</returns>
    int generateFilledPaletteComponents(Array<Colour> coloursIn = Array<Colour>());

    /// <summary>
    /// Launches the Palette Edit panel and listens for user interaction.
    /// </summary>
    /// <param name="paletteIn"></param>
    void startEditingPalette(int paletteIndexIn);

    /// <summary>
    /// Removes a palette and associated buttons
    /// </summary>
    /// <param name="paletteIndexToRemove"></param>
    void removePalette(int paletteIndexToRemove);

private:

    std::unique_ptr<TabbedComponent> colourToolTabs;
    std::unique_ptr<ColourPalettesPanel> palettePanel;
    std::unique_ptr<CustomPickerPanel> customPickerPanel;
    std::unique_ptr<PaletteEditPanel> paletteEditPanel;

    std::unique_ptr<Viewport> palettePanelViewport;

    ColourSelectionGroup paletteGroup;

    OwnedArray<ColourPaletteComponent> palettes;
    OwnedArray<TextButton> editButtons;
    OwnedArray<ImageButton> trashButtons;

    int paletteIndexEditing = -1;
    bool paletteEditingIsNew = false;

    const Image trashCanIcon = ImageCache::getFromHashCode(LumatoneEditorAssets::TrashCanIcon);
    Array<Array<Colour>> paletteColours;

    const float viewportScrollbarWidthScalar = 1.0f / 48.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourPaletteWindow)
};
