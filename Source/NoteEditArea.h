/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.4

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
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class NoteEditArea  : public Component
{
public:
    //==============================================================================
    NoteEditArea ();
    ~NoteEditArea() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);

	// Things to be done when a new mapping is loaded. E. g. fill the colour combo box with the colours appearing in the mapping.
	void onSetData(TerpstraKeyMapping& newData);

	// Fill key fields with values from a certain octaveboard subset
	void setKeyFieldValues(const TerpstraKeys& keySet);

	TabbedButtonBar* getOctaveBoardSelectorTab() { return octaveBoardSelectorTab.get(); }

private:
	void changeSingleKeySelection(int newSelection);

public:
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

	// Selector for octave boards
	std::unique_ptr<TabbedButtonBar> octaveBoardSelectorTab;

	// Editing single keys (of the selected 56-key set)
	std::unique_ptr<TerpstraKeyEdit>	terpstraKeyFields[TERPSTRABOARDSIZE];

	int					currentSingleKeySelection;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TabbedComponent> editFunctionsTab;
    std::unique_ptr<juce::Label> labelWindowTitle;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteEditArea)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

