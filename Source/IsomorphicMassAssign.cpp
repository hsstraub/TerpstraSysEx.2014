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

    // Create the mapping sub components. Do not make them visible (when one becomes visible it will automatically update the mapping logic)
	incrMidiNotesMapping.reset(new IncrMidiNotesMapping());
	incrMidiNotesMapping->setVisible(false);
	addChildComponent(incrMidiNotesMapping.get());

	kbmMappingDlg.reset(new KBMMappingDlg());
	kbmMappingDlg->setVisible(false);
	addChildComponent(kbmMappingDlg.get());

	mappingLogic = nullptr;
    //[/Constructor_pre]

    startingPointBox.reset (new ComboBox ("startingPointBox"));
    addAndMakeVisible (startingPointBox.get());
    startingPointBox->setEditableText (false);
    startingPointBox->setJustificationType (Justification::centredLeft);
    startingPointBox->setTextWhenNothingSelected (String());
    startingPointBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    startingPointBox->addListener (this);

    startingPointBox->setBounds (12, 291, 150, 24);

    labelStartingPoint.reset (new Label ("labelStartingPoint",
                                         TRANS("Starting value")));
    addAndMakeVisible (labelStartingPoint.get());
    labelStartingPoint->setTooltip (TRANS("Value that will be assigned to the key at mouse pposition when clicking"));
    labelStartingPoint->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelStartingPoint->setJustificationType (Justification::centredLeft);
    labelStartingPoint->setEditable (false, false, false);
    labelStartingPoint->setColour (TextEditor::textColourId, Colours::black);
    labelStartingPoint->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelStartingPoint->setBounds (12, 267, 150, 24);

    labelHorizontalSteps.reset (new Label ("labelHorizontalSteps",
                                           TRANS("Horizontal steps")));
    addAndMakeVisible (labelHorizontalSteps.get());
    labelHorizontalSteps->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelHorizontalSteps->setJustificationType (Justification::centredLeft);
    labelHorizontalSteps->setEditable (false, false, false);
    labelHorizontalSteps->setColour (TextEditor::textColourId, Colours::black);
    labelHorizontalSteps->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelHorizontalSteps->setBounds (220, 259, 150, 24);

    editHorizontalSteps.reset (new TextEditor ("editHorizontalSteps"));
    addAndMakeVisible (editHorizontalSteps.get());
    editHorizontalSteps->setMultiLine (false);
    editHorizontalSteps->setReturnKeyStartsNewLine (false);
    editHorizontalSteps->setReadOnly (false);
    editHorizontalSteps->setScrollbarsShown (true);
    editHorizontalSteps->setCaretVisible (true);
    editHorizontalSteps->setPopupMenuEnabled (true);
    editHorizontalSteps->setText (String());

    editHorizontalSteps->setBounds (220, 283, 40, 24);

    labelRightUpwardSteps.reset (new Label ("labelRightUpwardSteps",
                                            TRANS("Right upward steps")));
    addAndMakeVisible (labelRightUpwardSteps.get());
    labelRightUpwardSteps->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelRightUpwardSteps->setJustificationType (Justification::centredLeft);
    labelRightUpwardSteps->setEditable (false, false, false);
    labelRightUpwardSteps->setColour (TextEditor::textColourId, Colours::black);
    labelRightUpwardSteps->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelRightUpwardSteps->setBounds (148, 211, 150, 24);

    editRightUpwardSteps.reset (new TextEditor ("editRightUpwardSteps"));
    addAndMakeVisible (editRightUpwardSteps.get());
    editRightUpwardSteps->setMultiLine (false);
    editRightUpwardSteps->setReturnKeyStartsNewLine (false);
    editRightUpwardSteps->setReadOnly (false);
    editRightUpwardSteps->setScrollbarsShown (true);
    editRightUpwardSteps->setCaretVisible (true);
    editRightUpwardSteps->setPopupMenuEnabled (true);
    editRightUpwardSteps->setText (String());

    editRightUpwardSteps->setBounds (148, 235, 39, 24);

    editInstructionText.reset (new Label ("editInstructionText",
                                          TRANS("Fill a line or the whole field with constant step distances. \n"
                                          "Click on desired key field to start.")));
    addAndMakeVisible (editInstructionText.get());
    editInstructionText->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    editInstructionText->setJustificationType (Justification::topLeft);
    editInstructionText->setEditable (false, false, false);
    editInstructionText->setColour (TextEditor::textColourId, Colours::black);
    editInstructionText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    editInstructionText->setBounds (0, 168, 416, 40);

    groupMapping.reset (new GroupComponent ("groupMapping",
                                            TRANS("Mapping")));
    addAndMakeVisible (groupMapping.get());

    groupMapping->setBounds (0, 8, 416, 152);

    cbMappingType.reset (new ComboBox ("cbMappingType"));
    addAndMakeVisible (cbMappingType.get());
    cbMappingType->setEditableText (false);
    cbMappingType->setJustificationType (Justification::centredLeft);
    cbMappingType->setTextWhenNothingSelected (String());
    cbMappingType->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbMappingType->addItem (TRANS("MIDI notes, increasing order"), 1);
    cbMappingType->addItem (TRANS("Scala KBM mappings"), 2);
    cbMappingType->addListener (this);

    cbMappingType->setBounds (105, 31, 296, 24);

    labelMappingType.reset (new Label ("labelMappingType",
                                       TRANS("Type:")));
    addAndMakeVisible (labelMappingType.get());
    labelMappingType->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelMappingType->setJustificationType (Justification::centredLeft);
    labelMappingType->setEditable (false, false, false);
    labelMappingType->setColour (TextEditor::textColourId, Colours::black);
    labelMappingType->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelMappingType->setBounds (12, 32, 88, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (428, 220);


    //[Constructor] You can add your own custom stuff here..

	incrMidiNotesMapping->getMappingLogic()->addListener(this);
	kbmMappingDlg->getMappingLogic()->addListener(this);

    // cbMappingStyle default selection: will be read from user settings

    //[/Constructor]
}

IsomorphicMassAssign::~IsomorphicMassAssign()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	incrMidiNotesMapping = nullptr;
	kbmMappingDlg = nullptr;
    //[/Destructor_pre]

    startingPointBox = nullptr;
    labelStartingPoint = nullptr;
    labelHorizontalSteps = nullptr;
    editHorizontalSteps = nullptr;
    labelRightUpwardSteps = nullptr;
    editRightUpwardSteps = nullptr;
    editInstructionText = nullptr;
    groupMapping = nullptr;
    cbMappingType = nullptr;
    labelMappingType = nullptr;


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
	g.fillAll(findColour(ResizableWindow::backgroundColourId));
    //[/UserPaint]
}

void IsomorphicMassAssign::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	incrMidiNotesMapping->setBounds(16, MAPPINGSUBWINTOP, EDITSUBWINWIDTH, MAPPINGSUBWINHEIGHT);
	kbmMappingDlg->setBounds(16, MAPPINGSUBWINTOP, EDITSUBWINWIDTH, MAPPINGSUBWINHEIGHT);
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void IsomorphicMassAssign::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == startingPointBox.get())
    {
        //[UserComboBoxCode_startingPointBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_startingPointBox]
    }
    else if (comboBoxThatHasChanged == cbMappingType.get())
    {
        //[UserComboBoxCode_cbMappingType] -- add your combo box handling code here..
		int mappingType = cbMappingType->getSelectedItemIndex();

		// Show sub window corresponding to selected mapping style
		switch(mappingType)
		{
        case 0:
            incrMidiNotesMapping->setVisible(true);
            kbmMappingDlg->setVisible(false);
            break;
        case 1:
            incrMidiNotesMapping->setVisible(false);
            kbmMappingDlg->setVisible(true);
            break;
        default:
            incrMidiNotesMapping->setVisible(false);
            kbmMappingDlg->setVisible(false);
		}
        //[/UserComboBoxCode_cbMappingType]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void IsomorphicMassAssign::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	cbMappingType->setSelectedItemIndex(
        propertiesFile->getIntValue("MappingType", 0),
        juce::NotificationType::sendNotification);

    kbmMappingDlg->restoreStateFromPropertiesFile(propertiesFile);
}

void IsomorphicMassAssign::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
    propertiesFile->setValue("MappingType", cbMappingType->getSelectedItemIndex());

    kbmMappingDlg->saveStateToPropertiesFile(propertiesFile);
}

void IsomorphicMassAssign::setSaveSend(int setSelection, int keySelection, int noteIndex)
{
	// XXX This could be in a common base class for all assign edit components
	TerpstraKey keyData = this->mappingLogic->indexToTerpstraKey(noteIndex);

	// Save data
	((MainContentComponent*)(getParentComponent()->getParentComponent()))->getMappingInEdit().sets[setSelection].theKeys[keySelection] = keyData;

	// Send to device
	TerpstraSysExApplication::getApp().getMidiDriver().sendKeyParam(setSelection + 1, keySelection, keyData);
}

// Fill a line in current octave board. Starting point is assumed to have been set
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

// Fill a horizontal line over all octave boards. Starting point is assumed to have been set.
void IsomorphicMassAssign::fillGlobalLine(int setSelection, TerpstraBoardGeometry::StraightLineSet& globalLine, int startPos, int startNoteIndex, int stepSize)
{
	jassert(stepSize != 0);

	int pos, octaveBoardIndex;

	// Forward
	// Line belonging to current octave board
	int noteIndex = startNoteIndex + stepSize;

	for (pos = startPos + 1;
		pos < globalLine[setSelection].size() && noteIndex < this->mappingLogic->globalMappingSize();
		pos++, noteIndex += stepSize)
	{
		setSaveSend(setSelection, globalLine[setSelection][pos], noteIndex);
	}

	// Following octave boards
	for (octaveBoardIndex = setSelection+1; octaveBoardIndex < NUMBEROFBOARDS; octaveBoardIndex++)
    {
        for (pos = 0;
            pos < globalLine[octaveBoardIndex].size() && noteIndex < this->mappingLogic->globalMappingSize();
            pos++, noteIndex += stepSize)
        {
            setSaveSend(octaveBoardIndex, globalLine[octaveBoardIndex][pos], noteIndex);
        }
    }

	// Backward
	// Current octave board
	noteIndex = startNoteIndex - stepSize;

	for (pos = startPos - 1; pos >= 0 && noteIndex >= 0; pos--, noteIndex -= stepSize)
	{
		setSaveSend(setSelection, globalLine[setSelection][pos], noteIndex);
	}

    // Preceding octave boards
	for (octaveBoardIndex = setSelection-1; octaveBoardIndex >= 0; octaveBoardIndex--)
    {
        for (pos = globalLine[octaveBoardIndex].size()-1;
            pos >=0  && noteIndex >= 0;
            pos--, noteIndex -= stepSize)
        {
            setSaveSend(octaveBoardIndex, globalLine[octaveBoardIndex][pos], noteIndex);
        }
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
		jassert(!keyData.isEmpty());

		startingPointBox->addItem(String(i) + ": Key_" + String(keyData.noteNumber) + ", Chan_" + String(keyData.channelNumber), i + 1);
	}
}

/// <summary>Called from MainComponent when one of the keys is clicked</summary>
/// <returns>Mapping was changed yes/no</returns>
bool IsomorphicMassAssign::performMouseDown(int setSelection, int keySelection)
{
	bool mappingChanged = false;
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
			TerpstraBoardGeometry::StraightLineSet globalHorizLine = boardGeometry.globalHorizontalLineOfField(setSelection, keySelection);
			int startPos = globalHorizLine[setSelection].indexOf(keySelection);
			fillGlobalLine(setSelection, globalHorizLine, startPos, startNoteIndex, horizStepSize);

			mappingChanged = true;
		}

		// Right vertical line
		else if (horizStepSize == 0 && rUpwStepSize != 0)
		{
			TerpstraBoardGeometry::StraightLineSet globalRUpLine = boardGeometry.globalRightUpwardLineOfField(setSelection, keySelection);
			int startPos = globalRUpLine[setSelection].indexOf(keySelection);
			fillGlobalLine(setSelection, globalRUpLine, startPos, startNoteIndex, rUpwStepSize);
			mappingChanged = true;
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

			mappingChanged = true;
		}
	}

	return mappingChanged;
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="IsomorphicMassAssign" componentName=""
                 parentClasses="public Component, public MappingLogicBase::Listener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="428"
                 initialHeight="220">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <COMBOBOX name="startingPointBox" id="d526f69bdc196fea" memberName="startingPointBox"
            virtualName="" explicitFocusOrder="0" pos="12 291 150 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="labelStartingPoint" id="5401a3246c13771e" memberName="labelStartingPoint"
         virtualName="" explicitFocusOrder="0" pos="12 267 150 24" tooltip="Value that will be assigned to the key at mouse pposition when clicking"
         edTextCol="ff000000" edBkgCol="0" labelText="Starting value"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="labelHorizontalSteps" id="3e6663aecf1474c8" memberName="labelHorizontalSteps"
         virtualName="" explicitFocusOrder="0" pos="220 259 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Horizontal steps" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="editHorizontalSteps" id="8d2f5f07f337b9ef" memberName="editHorizontalSteps"
              virtualName="" explicitFocusOrder="0" pos="220 283 40 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="labelRightUpwardSteps" id="43530804741d9cb7" memberName="labelRightUpwardSteps"
         virtualName="" explicitFocusOrder="0" pos="148 211 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Right upward steps" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="editRightUpwardSteps" id="3a1cf8588366e0ca" memberName="editRightUpwardSteps"
              virtualName="" explicitFocusOrder="0" pos="148 235 39 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="editInstructionText" id="c03ef432c2b4599" memberName="editInstructionText"
         virtualName="" explicitFocusOrder="0" pos="0 168 416 40" edTextCol="ff000000"
         edBkgCol="0" labelText="Fill a line or the whole field with constant step distances. &#10;Click on desired key field to start."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="9"/>
  <GROUPCOMPONENT name="groupMapping" id="fbb69100a7b12118" memberName="groupMapping"
                  virtualName="" explicitFocusOrder="0" pos="0 8 416 152" title="Mapping"/>
  <COMBOBOX name="cbMappingType" id="a7825b65cfb78392" memberName="cbMappingType"
            virtualName="" explicitFocusOrder="0" pos="105 31 296 24" editable="0"
            layout="33" items="MIDI notes, increasing order&#10;Scala KBM mappings"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="labelMappingType" id="d77d8a4b80130afc" memberName="labelMappingType"
         virtualName="" explicitFocusOrder="0" pos="12 32 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Type:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

