/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_730F20A4628CA114__
#define __JUCE_HEADER_730F20A4628CA114__

//[Headers]     -- You can add your own extra header files here --
#include "../../JuceLibraryCode/JuceHeader.h"
#include "MappingLogic.h"
#include "TerpstraMidiDriver.h"
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
	// Set, save and maybe send data of one key
	void setSaveSend(TerpstraKeyMapping& mappingData, int setSelection, int keySelection, int noteIndex, TerpstraMidiDriver& midiDriver);

	// Fill a line, Starting point ios assumed to have been set
	void fillLine(TerpstraKeyMapping& mappingData, int setSelection, TerpstraBoardGeometry::StraightLine& line, int startPos, int startNoteIndex, int stepSize, TerpstraMidiDriver& midiDriver);

	// Implementation of MappingLogicListener
	void mappingLogicChanged(MappingLogicBase* mappingLogicThatChanged) override;
	// Edit operation when a key field in MainComponent has been clicked
	void PerformMouseClickEdit(TerpstraKeyMapping& mappingData, int setSelection, int keySelection, TerpstraMidiDriver& midiDriver);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	MappingLogicBase*		mappingLogic;
	TerpstraBoardGeometry	boardGeometry;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ComboBox> startingPointeBox;
    ScopedPointer<Label> labelStartingPoint;
    ScopedPointer<Label> labelHorizontalSteps;
    ScopedPointer<TextEditor> editHorizontalSteps;
    ScopedPointer<Label> labelRightUpwardSteps;
    ScopedPointer<TextEditor> editRightUpwardSteps;
    ScopedPointer<Label> editInstructionText;
    Path internalPath1;
    Path internalPath2;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IsomorphicMassAssign)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_730F20A4628CA114__
