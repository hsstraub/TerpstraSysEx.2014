/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"

#include "ViewComponents.h"
#include "KeyboardDataStructure.h"

#include "HexagonTilingGeometry.h"
#include "LumatoneEditorStyleCommon.h"

#include "BoardGeometry.h"

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class NoteEditArea  : public Component,
                      public ChangeListener
{
public:
    //==============================================================================
    NoteEditArea ();
    ~NoteEditArea() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);

	// Implementation of ChangeListener
	void changeListenerCallback(ChangeBroadcaster *source) override;

	// Things to be done when a new mapping is loaded. E. g. fill the colour combo box with the colours appearing in the mapping.
	void onSetData(TerpstraKeyMapping& newData);

	// Fill key fields with values from a certain octaveboard subset
	void setKeyFieldValues(const TerpstraKeys& keySet);

	TabbedButtonBar* getOctaveBoardSelectorTab() { return octaveBoardSelectorTab.get(); }

	void changeSingleKeySelection(int newSelection);

	void refreshKeyFields();

    void lookAndFeelChanged() override;

    // Helper method for aligning the Octave Section TabbedButtonBar
    void setControlsTopLeftPosition(int controlsAreaX, int controlsAreaY);

    // Register listener for key colour edit popup
    void registerPaletteWindowRequestListener(TextButton::Listener* listenerIn);
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& e) override;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    enum noteEditMode
    {
        SingleNoteAssignMode = 0,
        IsomorphicMassAssignMode = 1
    };

	bool showIsomorphicMassAssign = false;

	// Selector for octave boards
	std::unique_ptr<TabbedButtonBar> octaveBoardSelectorTab;

	// Editing single keys (of the selected 56-key set)
	std::unique_ptr<TerpstraKeyEdit>	terpstraKeyFields[56];

    int					currentSingleKeySelection;

    // Key edit positioning
    HexagonTilingGeometry tilingGeometry;

    //===========================================================================
    // Style Helpers
    Colour backgroundColour = Colours::darkgrey;

    Rectangle<float> octaveTabsArea;
    Rectangle<float> contentBackground;
    Rectangle<float> assignControlsBounds;
    Rectangle<float> keyEditBounds;

    int roundedCornerLayout;

    //===========================================================================
    // Size & position constants relative to parent unless otherwise noted

    const float contentMarginY                  = 1.0f / 11.0f;

    const float assignControlsWidth             = 0.3846f;
    const float assignControlsHeightInContent   = 0.9f;

    const float assignMarginX                   = 0.05f;
    const float assignMarginYInContent          = 2.0f / 45.0f;
    const float assignTabDepthInContent         = 0.0851f;

    const float assignLabelMarginX              = 0.01f;
    const float assignLabelTabDepthHeight       = 0.9f;

    const float keyEditWidth                    = 0.48f;
    const float keyEditMarginX                  = 0.48f;
    const float keyEditMarginYInContent         = 0.1f;

    //===========================================================================
    // Key fields relative to KeyEditBounds

    const float singleKeyFieldRimAbove = 0.075;// = 0.05106f;
    const float singleKeyFieldRimLeft           = 0.02105f;
    const float singleKeyFieldSize              = 0.11f;
    const float singleKeyFieldLineWidth         = 0.003f;
    const float singleKeyFieldLineWidthSelected = 0.005f;

    const float singleKeyMarginFromWidth = 0.0164f;

	// Geometry settings
	TerpstraBoardGeometry	boardGeometry;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TabbedComponent> editFunctionsTab;
    std::unique_ptr<juce::Label> labelWindowTitle;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteEditArea)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

