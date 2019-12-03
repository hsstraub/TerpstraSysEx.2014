/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_730F20A4628CA114__
#define __JUCE_HEADER_730F20A4628CA114__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "MappingLogic.h"
#include "IncrMidiNotesMapping.h"
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
                              public MappingLogicListener,
                              public ComboBoxListener
{
public:
    //==============================================================================
    IsomorphicMassAssign ();
    ~IsomorphicMassAssign();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	// Implementation of MappingLogicListener
	// Set, save and maybe send data of one key
	void setSaveSend(int setSelection, int keySelection, int noteIndex);

	// Fill a line, Starting point is assumed to have been set
	void fillLine(int setSelection, TerpstraBoardGeometry::StraightLine& line, int startPos, int startNoteIndex, int stepSize);

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
	ScopedPointer<IncrMidiNotesMapping>	incrMidiNotesMapping;

	MappingLogicBase*		mappingLogic;
	TerpstraBoardGeometry	boardGeometry;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ComboBox> startingPointBox;
    ScopedPointer<Label> labelStartingPoint;
    ScopedPointer<Label> labelHorizontalSteps;
    ScopedPointer<TextEditor> editHorizontalSteps;
    ScopedPointer<Label> labelRightUpwardSteps;
    ScopedPointer<TextEditor> editRightUpwardSteps;
    ScopedPointer<Label> editInstructionText;
    ScopedPointer<GroupComponent> groupMapping;
    ScopedPointer<ComboBox> cbMappingStyle;
    ScopedPointer<Label> labelMappingStyle;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IsomorphicMassAssign)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_730F20A4628CA114__
