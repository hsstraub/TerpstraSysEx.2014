/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "MappingLogic.h"
#include "IncrMidiNotesMapping.h"
#include "KBMMappingDlg.h"
#include "BoardGeometry.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class IsomorphicMassAssign  : public Component,
                              public MappingLogicBase::Listener,
                              public ComboBox::Listener
{
public:
    //==============================================================================
    IsomorphicMassAssign ();
    ~IsomorphicMassAssign() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	// Implementation of MappingLogicListener
	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);

	// Set, save and maybe send data of one key
	void setSaveSend(int setSelection, int keySelection, int noteIndex);

	// Fill a line, Starting point is assumed to have been set
	void fillLine(int setSelection, TerpstraBoardGeometry::StraightLine& line, int startPos, int startNoteIndex, int stepSize);

	// Fill a line over all octave boards. Starting point is assumed to have been set.
	void fillGlobalLine(int setSelection, TerpstraBoardGeometry::StraightLineSet& globalLine, int startPos, int startNoteIndex, int stepSize);

	// Fill a horizontal line and its cutting upwards lines, recursively
	void fill2DHorizLineRecursive(int setSelection, TerpstraBoardGeometry::StraightLine& horizLine, int startPos, int startNoteIndex,
		int horizStepSize, int rUpwStepSize,
		TerpstraBoardGeometry::StraightLineSet& finishedLines);

	// Fill a right upward line and its cutting horizontal lines, recursively
	void fill2DRUpwLineRecursive(int setSelection, TerpstraBoardGeometry::StraightLine& rUpwLine, int startPos, int startNoteIndex,
		int horizStepSize, int rUpwStepSize,
		TerpstraBoardGeometry::StraightLineSet& finishedLines);

	// Implementation of MappingLogicListener
	void mappingLogicChanged(MappingLogicBase* mappingLogicThatChanged) override;

	bool performMouseDown(int setSelection, int keySelection);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	std::unique_ptr<IncrMidiNotesMapping>	incrMidiNotesMapping;
	std::unique_ptr<KBMMappingDlg>          kbmMappingDlg;

	MappingLogicBase*		mappingLogic;
	TerpstraBoardGeometry	boardGeometry;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ComboBox> startingPointBox;
    std::unique_ptr<Label> labelStartingPoint;
    std::unique_ptr<Label> labelHorizontalSteps;
    std::unique_ptr<TextEditor> editHorizontalSteps;
    std::unique_ptr<Label> labelRightUpwardSteps;
    std::unique_ptr<TextEditor> editRightUpwardSteps;
    std::unique_ptr<Label> editInstructionText;
    std::unique_ptr<GroupComponent> groupMapping;
    std::unique_ptr<ComboBox> cbMappingType;
    std::unique_ptr<Label> labelMappingType;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IsomorphicMassAssign)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

