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

//[Headers] You can add your own extra header files here...
#include "ViewConstants.h"
#include "Main.h"
#include "MainComponent.h"
//[/Headers]

#include "IsomorphicMassAssign.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
IsomorphicMassAssign::IsomorphicMassAssign ()
{
    //[Constructor_pre] You can add your own custom stuff here..
	incrMidiNotesMapping = new IncrMidiNotesMapping();
	addAndMakeVisible(incrMidiNotesMapping);
	incrMidiNotesMapping->setVisible(false);

	mappingLogic = nullptr;
	//[/Constructor_pre]

    addAndMakeVisible (startingPointBox = new ComboBox ("startingPointBox"));
    startingPointBox->setEditableText (false);
    startingPointBox->setJustificationType (Justification::centredLeft);
    startingPointBox->setTextWhenNothingSelected (String());
    startingPointBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    startingPointBox->addListener (this);

    addAndMakeVisible (labelStartingPoint = new Label ("labelStartingPoint",
                                                       TRANS("Starting value")));
    labelStartingPoint->setTooltip (TRANS("Value that will be assigned to the key at mouse pposition when clicking"));
    labelStartingPoint->setFont (Font (15.00f, Font::plain));
    labelStartingPoint->setJustificationType (Justification::centredLeft);
    labelStartingPoint->setEditable (false, false, false);
    labelStartingPoint->setColour (TextEditor::textColourId, Colours::black);
    labelStartingPoint->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (labelHorizontalSteps = new Label ("labelHorizontalSteps",
                                                         TRANS("Horizontal steps")));
    labelHorizontalSteps->setFont (Font (15.00f, Font::plain));
    labelHorizontalSteps->setJustificationType (Justification::centredLeft);
    labelHorizontalSteps->setEditable (false, false, false);
    labelHorizontalSteps->setColour (TextEditor::textColourId, Colours::black);
    labelHorizontalSteps->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (editHorizontalSteps = new TextEditor ("editHorizontalSteps"));
    editHorizontalSteps->setMultiLine (false);
    editHorizontalSteps->setReturnKeyStartsNewLine (false);
    editHorizontalSteps->setReadOnly (false);
    editHorizontalSteps->setScrollbarsShown (true);
    editHorizontalSteps->setCaretVisible (true);
    editHorizontalSteps->setPopupMenuEnabled (true);
    editHorizontalSteps->setText (String());

    addAndMakeVisible (labelRightUpwardSteps = new Label ("labelRightUpwardSteps",
                                                          TRANS("Right upward steps")));
    labelRightUpwardSteps->setFont (Font (15.00f, Font::plain));
    labelRightUpwardSteps->setJustificationType (Justification::centredLeft);
    labelRightUpwardSteps->setEditable (false, false, false);
    labelRightUpwardSteps->setColour (TextEditor::textColourId, Colours::black);
    labelRightUpwardSteps->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (editRightUpwardSteps = new TextEditor ("editRightUpwardSteps"));
    editRightUpwardSteps->setMultiLine (false);
    editRightUpwardSteps->setReturnKeyStartsNewLine (false);
    editRightUpwardSteps->setReadOnly (false);
    editRightUpwardSteps->setScrollbarsShown (true);
    editRightUpwardSteps->setCaretVisible (true);
    editRightUpwardSteps->setPopupMenuEnabled (true);
    editRightUpwardSteps->setText (String());

    addAndMakeVisible (editInstructionText = new Label ("editInstructionText",
                                                        TRANS("Fill a line or the whole field with constant step distances. \n"
                                                        "Click on desired key field to start.")));
    editInstructionText->setFont (Font (15.00f, Font::plain));
    editInstructionText->setJustificationType (Justification::topLeft);
    editInstructionText->setEditable (false, false, false);
    editInstructionText->setColour (TextEditor::textColourId, Colours::black);
    editInstructionText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (groupMapping = new GroupComponent ("groupMapping",
                                                          TRANS("Mapping")));

    addAndMakeVisible (cbMappingStyle = new ComboBox ("cbMappingStyle"));
    cbMappingStyle->setEditableText (false);
    cbMappingStyle->setJustificationType (Justification::centredLeft);
    cbMappingStyle->setTextWhenNothingSelected (String());
    cbMappingStyle->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbMappingStyle->addItem (TRANS("MIDI notes, increasing order"), 1);
    cbMappingStyle->addListener (this);

    addAndMakeVisible (labelMappingStyle = new Label ("labelMappingStyle",
                                                      TRANS("Style:")));
    labelMappingStyle->setFont (Font (15.00f, Font::plain));
    labelMappingStyle->setJustificationType (Justification::centredLeft);
    labelMappingStyle->setEditable (false, false, false);
    labelMappingStyle->setColour (TextEditor::textColourId, Colours::black);
    labelMappingStyle->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (428, 220);


    //[Constructor] You can add your own custom stuff here..

	incrMidiNotesMapping->getMappingLogic()->addListener(this);


	// Default selection
	// Todo: read from user settings
	cbMappingStyle->setSelectedItemIndex(0, juce::NotificationType::sendNotification);

    //[/Constructor]
}

IsomorphicMassAssign::~IsomorphicMassAssign()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	incrMidiNotesMapping = nullptr;
    //[/Destructor_pre]

    startingPointBox = nullptr;
    labelStartingPoint = nullptr;
    labelHorizontalSteps = nullptr;
    editHorizontalSteps = nullptr;
    labelRightUpwardSteps = nullptr;
    editRightUpwardSteps = nullptr;
    editInstructionText = nullptr;
    groupMapping = nullptr;
    cbMappingStyle = nullptr;
    labelMappingStyle = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void IsomorphicMassAssign::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void IsomorphicMassAssign::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	incrMidiNotesMapping->setBounds(16, MAPPINGSUBWINTOP, EDITSUBWINWIDTH, MAPPINGSUBWINHEIGHT);
    //[/UserPreResize]

    startingPointBox->setBounds (12, 291, 150, 24);
    labelStartingPoint->setBounds (12, 267, 150, 24);
    labelHorizontalSteps->setBounds (220, 259, 150, 24);
    editHorizontalSteps->setBounds (220, 283, 40, 24);
    labelRightUpwardSteps->setBounds (148, 211, 150, 24);
    editRightUpwardSteps->setBounds (148, 235, 39, 24);
    editInstructionText->setBounds (0, 168, 416, 40);
    groupMapping->setBounds (0, 8, 416, 152);
    cbMappingStyle->setBounds (105, 31, 296, 24);
    labelMappingStyle->setBounds (12, 32, 88, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void IsomorphicMassAssign::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == startingPointBox)
    {
        //[UserComboBoxCode_startingPointBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_startingPointBox]
    }
    else if (comboBoxThatHasChanged == cbMappingStyle)
    {
        //[UserComboBoxCode_cbMappingStyle] -- add your combo box handling code here..
		int mappingStyle = cbMappingStyle->getSelectedItemIndex();

		// Show sub window corresponding to selected mapping style. Currently there is only one
		if (mappingStyle == 0)
			incrMidiNotesMapping->setVisible(true);
		else
			incrMidiNotesMapping->setVisible(false);
        //[/UserComboBoxCode_cbMappingStyle]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void IsomorphicMassAssign::setSaveSend(int setSelection, int keySelection, int noteIndex)
{
	// XXX This could be in a common base class for all assign edit components
	TerpstraKey keyData = this->mappingLogic->indexToTerpstraKey(noteIndex);

	// Save data
	((MainContentComponent*)(getParentComponent()->getParentComponent()))->getMappingInEdit().sets[setSelection].theKeys[keySelection] = keyData;

	// Send to device
	// XXX TerpstraSysExApplication::getApp().getMidiDriver().sendAndMaybeSaveKeyParam(setSelection + 1, keySelection, keyData);
}

// Fill a line. Starting point is assumed to have been set
void IsomorphicMassAssign::fillLine(int setSelection, TerpstraBoardGeometry::StraightLine& line, int startPos, int startNoteIndex, int stepSize)
{
	jassert(stepSize != 0);

	int pos, noteIndex;
	// Forward
	for (pos = startPos + 1, noteIndex = startNoteIndex + stepSize;
		pos < line.size() && noteIndex < this->mappingLogic->globalMappingSize();
		pos++, noteIndex += stepSize)
	{
		setSaveSend(setSelection, line[pos], noteIndex);
	}

	// Backward
	for (pos = startPos - 1, noteIndex = startNoteIndex - stepSize;
		pos >= 0 && noteIndex >= 0;
		pos--, noteIndex -= stepSize)
	{
		setSaveSend(setSelection, line[pos], noteIndex);
	}
}

// Fill a horizontal line and its cutting upwards lines, recursively. Fill only those that have not been filled yet. Starting point is assumed to have been set.
void IsomorphicMassAssign::fill2DHorizLineRecursive(int setSelection, TerpstraBoardGeometry::StraightLine& horizLine, int startPos, int startNoteIndex,
	int horizStepSize, int rUpwStepSize,
	TerpstraBoardGeometry::StraightLineSet& finishedLines)
{
	// Only if not done yet
	if (!finishedLines.contains(horizLine))
	{
		// Fill the line itself
		fillLine(setSelection, horizLine, startPos, startNoteIndex, horizStepSize);

		// Add to list of finished lines
		finishedLines.add(horizLine);

		// Find cutting lines and fill them
		for (int linepos = 0; linepos < horizLine.size(); linepos++)
		{
			// Find the vertical line at this position
			TerpstraBoardGeometry::StraightLine rUpLine = boardGeometry.rightUpwardLineOfField(horizLine[linepos]);
			int rUpStartPos = rUpLine.indexOf(horizLine[linepos]);

			// Start note index: the value that has been set to the horizontal line element
			int rUpStartNoteIndex = this->mappingLogic->terpstraKeyToIndex(
				((MainContentComponent*)(getParentComponent()->getParentComponent()))->getMappingInEdit().sets[setSelection].theKeys[horizLine[linepos]]);

			// Fill it and its cutting lines, if it has not been done before. Check of the latter is done inside.
			fill2DRUpwLineRecursive(setSelection, rUpLine, rUpStartPos, rUpStartNoteIndex, horizStepSize, rUpwStepSize, finishedLines);
		}
	}
}

// Fill a right upward line and its cutting horizontal lines, recursively. Fill only those that have not been filled yet. Starting point is assumed to have been set.
void IsomorphicMassAssign::fill2DRUpwLineRecursive(int setSelection, TerpstraBoardGeometry::StraightLine& rUpwLine, int startPos, int startNoteIndex,
	int horizStepSize, int rUpwStepSize,
	TerpstraBoardGeometry::StraightLineSet& finishedLines)
{
	// Only if not done yet
	if (!finishedLines.contains(rUpwLine))
	{
		// Fill the line itself
		fillLine(setSelection, rUpwLine, startPos, startNoteIndex, rUpwStepSize);

		// Add to list of finished lines
		finishedLines.add(rUpwLine);

		// Find cutting lines and fill them
		for (int linepos = 0; linepos < rUpwLine.size(); linepos++)
		{
			// Find the vertical line at this position
			TerpstraBoardGeometry::StraightLine horizLine = boardGeometry.horizontalLineOfField(rUpwLine[linepos]);
			int horizStartPos = horizLine.indexOf(rUpwLine[linepos]);

			// Start note index: the value that has been set to the horizontal line element
			int horizStartNoteIndex = this->mappingLogic->terpstraKeyToIndex(
				((MainContentComponent*)(getParentComponent()->getParentComponent()))->getMappingInEdit().sets[setSelection].theKeys[rUpwLine[linepos]]);

			// Fill it and its cutting lines, if it has not been done before. Check of the latter is done inside.
			fill2DHorizLineRecursive(setSelection, horizLine, horizStartPos, horizStartNoteIndex, horizStepSize, rUpwStepSize, finishedLines);
		}
	}
}

// Implementation of MappingLogicListener
void IsomorphicMassAssign::mappingLogicChanged(MappingLogicBase* mappingLogicThatChanged)
{
	this->mappingLogic = mappingLogicThatChanged;

	// Fill note combo with values according to mapping logic
	startingPointBox->clear(juce::NotificationType::dontSendNotification);

	for (int i = 0; i < mappingLogicThatChanged->globalMappingSize(); i++)
	{
		TerpstraKey keyData = mappingLogicThatChanged->indexToTerpstraKey(i);
		// XXX format text
		startingPointBox->addItem(String(i) + ": Key_" + String(keyData.noteNumber) + ", Chan_" + String(keyData.channelNumber), i + 1);
	}
}

// Called from MainComponent when one of the keys is clicked
void IsomorphicMassAssign::PerformMouseClickEdit(int setSelection, int keySelection)
{
	jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS && keySelection >= 0 && keySelection < TERPSTRABOARDSIZE);

	int startNoteIndex = this->startingPointBox->getSelectedItemIndex();
	if (this->mappingLogic != nullptr && startNoteIndex >= 0)
	{
		// Set value of starting point
		setSaveSend(setSelection, keySelection, startNoteIndex);

		int horizStepSize = editHorizontalSteps->getText().getIntValue();
		int rUpwStepSize = editRightUpwardSteps->getText().getIntValue();

		// Horizontal line
		if (horizStepSize != 0 && rUpwStepSize == 0)
		{
			TerpstraBoardGeometry::StraightLine horizLine = boardGeometry.horizontalLineOfField(keySelection);
			int startPos = horizLine.indexOf(keySelection);
			fillLine(setSelection, horizLine, startPos, startNoteIndex, horizStepSize);
		}

		// Right vertical line
		else if (horizStepSize == 0 && rUpwStepSize != 0)
		{
			TerpstraBoardGeometry::StraightLine rUpLine = boardGeometry.rightUpwardLineOfField(keySelection);
			int startPos = rUpLine.indexOf(keySelection);
			fillLine(setSelection, rUpLine, startPos, startNoteIndex, rUpwStepSize);
		}

		// Two dimensional: fill whole subset
		else if (horizStepSize != 0 && rUpwStepSize != 0)
		{
			TerpstraBoardGeometry::StraightLineSet finishedLines;	// List of lines that have been finished, so the recursion ends

			// Find the horizontal line
			TerpstraBoardGeometry::StraightLine horizLine = boardGeometry.horizontalLineOfField(keySelection);
			int startPos = horizLine.indexOf(keySelection);

			// Fill the board, starting from this line
			fill2DHorizLineRecursive(setSelection, horizLine, startPos, startNoteIndex, horizStepSize, rUpwStepSize, finishedLines);
		}
	}
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="IsomorphicMassAssign" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="428" initialHeight="220">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <COMBOBOX name="startingPointBox" id="d526f69bdc196fea" memberName="startingPointBox"
            virtualName="" explicitFocusOrder="0" pos="12 291 150 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="labelStartingPoint" id="5401a3246c13771e" memberName="labelStartingPoint"
         virtualName="" explicitFocusOrder="0" pos="12 267 150 24" tooltip="Value that will be assigned to the key at mouse pposition when clicking"
         edTextCol="ff000000" edBkgCol="0" labelText="Starting value"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="labelHorizontalSteps" id="3e6663aecf1474c8" memberName="labelHorizontalSteps"
         virtualName="" explicitFocusOrder="0" pos="220 259 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Horizontal steps" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="editHorizontalSteps" id="8d2f5f07f337b9ef" memberName="editHorizontalSteps"
              virtualName="" explicitFocusOrder="0" pos="220 283 40 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="labelRightUpwardSteps" id="43530804741d9cb7" memberName="labelRightUpwardSteps"
         virtualName="" explicitFocusOrder="0" pos="148 211 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Right upward steps" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="editRightUpwardSteps" id="3a1cf8588366e0ca" memberName="editRightUpwardSteps"
              virtualName="" explicitFocusOrder="0" pos="148 235 39 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="editInstructionText" id="c03ef432c2b4599" memberName="editInstructionText"
         virtualName="" explicitFocusOrder="0" pos="0 168 416 40" edTextCol="ff000000"
         edBkgCol="0" labelText="Fill a line or the whole field with constant step distances. &#10;Click on desired key field to start."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="9"/>
  <GROUPCOMPONENT name="groupMapping" id="fbb69100a7b12118" memberName="groupMapping"
                  virtualName="" explicitFocusOrder="0" pos="0 8 416 152" title="Mapping"/>
  <COMBOBOX name="cbMappingStyle" id="a7825b65cfb78392" memberName="cbMappingStyle"
            virtualName="" explicitFocusOrder="0" pos="105 31 296 24" editable="0"
            layout="33" items="MIDI notes, increasing order" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <LABEL name="labelMappingStyle" id="d77d8a4b80130afc" memberName="labelMappingStyle"
         virtualName="" explicitFocusOrder="0" pos="12 32 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Style:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
