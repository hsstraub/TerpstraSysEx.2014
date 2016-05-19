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
#include "Main.h"
//[/Headers]

#include "IsomorphicMassAssign.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
IsomorphicMassAssign::IsomorphicMassAssign ()
{
    //[Constructor_pre] You can add your own custom stuff here..
	mappingLogic = nullptr;
    //[/Constructor_pre]

    addAndMakeVisible (startingPointeBox = new ComboBox ("startingPointBox"));
    startingPointeBox->setEditableText (false);
    startingPointeBox->setJustificationType (Justification::centredLeft);
    startingPointeBox->setTextWhenNothingSelected (String());
    startingPointeBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    startingPointeBox->addListener (this);

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


    //[UserPreSize]
    //[/UserPreSize]

    setSize (428, 220);


    //[Constructor] You can add your own custom stuff here..
	editHorizontalSteps->setInputRestrictions(0, "0123456789-");
	editRightUpwardSteps->setInputRestrictions(0, "0123456789-");
    //[/Constructor]
}

IsomorphicMassAssign::~IsomorphicMassAssign()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    startingPointeBox = nullptr;
    labelStartingPoint = nullptr;
    labelHorizontalSteps = nullptr;
    editHorizontalSteps = nullptr;
    labelRightUpwardSteps = nullptr;
    editRightUpwardSteps = nullptr;
    editInstructionText = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void IsomorphicMassAssign::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    g.setColour (Colour (0xff010e0c));
    g.fillPath (internalPath1);

    g.setColour (Colour (0xff02020e));
    g.fillPath (internalPath2);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void IsomorphicMassAssign::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    startingPointeBox->setBounds (24, 128, 150, 24);
    labelStartingPoint->setBounds (24, 104, 150, 24);
    labelHorizontalSteps->setBounds (232, 96, 150, 24);
    editHorizontalSteps->setBounds (232, 120, 40, 24);
    labelRightUpwardSteps->setBounds (160, 48, 150, 24);
    editRightUpwardSteps->setBounds (160, 72, 39, 24);
    editInstructionText->setBounds (8, 8, 416, 40);
    internalPath1.clear();
    internalPath1.startNewSubPath (188.0f, 132.0f);
    internalPath1.lineTo (212.0f, 136.0f);
    internalPath1.lineTo (192.0f, 144.0f);
    internalPath1.closeSubPath();

    internalPath2.clear();
    internalPath2.startNewSubPath (168.0f, 104.0f);
    internalPath2.lineTo (168.0f, 120.0f);
    internalPath2.lineTo (160.0f, 120.0f);
    internalPath2.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void IsomorphicMassAssign::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == startingPointeBox)
    {
        //[UserComboBoxCode_startingPointeBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_startingPointeBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void IsomorphicMassAssign::setSaveSend(TerpstraKeyMapping& mappingData, int setSelection, int keySelection, int noteIndex)
{
	// XXX This could be in a common base class for all assign edit components
	TerpstraKey keyData = this->mappingLogic->indexToTerpstraKey(noteIndex);

	// Save data
	mappingData.sets[setSelection].theKeys[keySelection] = keyData;

	// Send to device
	TerpstraSysExApplication::getApp().getMidiDriver().sendAndMaybeSaveKeyParam(setSelection + 1, keySelection, keyData);
}

// Fill a line, Starting point ios assumed to have been set
void IsomorphicMassAssign::fillLine(TerpstraKeyMapping& mappingData, int setSelection, TerpstraBoardGeometry::StraightLine& line, int startPos, int startNoteIndex, int stepSize)
{
	jassert(stepSize != 0);

	int pos, noteIndex;
	// Forward
	for (pos = startPos + 1, noteIndex = startNoteIndex + stepSize;
		pos < line.size() && noteIndex < this->mappingLogic->globalMappingSize();
		pos++, noteIndex += stepSize)
	{
		setSaveSend(mappingData, setSelection, line[pos], noteIndex);
	}

	// Backward
	for (pos = startPos - 1, noteIndex = startNoteIndex - stepSize;
		pos >= 0 && noteIndex >= 0;
		pos--, noteIndex -= stepSize)
	{
		setSaveSend(mappingData, setSelection, line[pos], noteIndex);
	}
}

// Implementation of MappingLogicListener
void IsomorphicMassAssign::mappingLogicChanged(MappingLogicBase* mappingLogicThatChanged)
{
	this->mappingLogic = mappingLogicThatChanged;

	// Fill note combo with values according to mapping logic
	// XXX Same code as in SingleNoteAssign
	startingPointeBox->clear(juce::NotificationType::dontSendNotification);

	for (int i = 0; i < mappingLogicThatChanged->globalMappingSize(); i++)
	{
		TerpstraKey keyData = mappingLogicThatChanged->indexToTerpstraKey(i);
		// XXX format text
		startingPointeBox->addItem(String(i) + ": Key_" + String(keyData.noteNumber) + ", Chan_" + String(keyData.channelNumber), i + 1);
	}
}

// Called from MainComponent when one of the keys is clicked
void IsomorphicMassAssign::PerformMouseClickEdit(TerpstraKeyMapping& mappingData, int setSelection, int keySelection)
{
	jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS && keySelection >= 0 && keySelection < TERPSTRABOARDSIZE);

	int startNoteIndex = this->startingPointeBox->getSelectedItemIndex();
	if (this->mappingLogic != nullptr && startNoteIndex >= 0)
	{
		// Set value of starting point
		setSaveSend(mappingData, setSelection, keySelection, startNoteIndex);

		int horizStepSize = editHorizontalSteps->getText().getIntValue();
		int rUpwStepSize = editRightUpwardSteps->getText().getIntValue();

		// Horizontal line
		if (horizStepSize != 0 && rUpwStepSize == 0)
		{
			TerpstraBoardGeometry::StraightLine horizLine = boardGeometry.horizontalLineOfField(keySelection);
			int startPos = horizLine.indexOf(keySelection);
			fillLine(mappingData, setSelection, horizLine, startPos, startNoteIndex, horizStepSize);
		}

		// Right vertical line
		else if (horizStepSize == 0 && rUpwStepSize != 0)
		{
			TerpstraBoardGeometry::StraightLine rUpLine = boardGeometry.rightUpwardLineOfField(keySelection);
			int startPos = rUpLine.indexOf(keySelection);
			fillLine(mappingData, setSelection, rUpLine, startPos, startNoteIndex, rUpwStepSize);
		}

		// Two dimensional: fill whole subset
		else if (horizStepSize != 0 && rUpwStepSize != 0)
		{
			// Fill the horizontal line
			TerpstraBoardGeometry::StraightLine horizLine = boardGeometry.horizontalLineOfField(keySelection);
			int startPos = horizLine.indexOf(keySelection);
			fillLine(mappingData, setSelection, horizLine, startPos, startNoteIndex, horizStepSize);

			for (int linepos = 0; linepos < horizLine.size(); linepos++)
			{
				// Fill the vertical line at this position
				TerpstraBoardGeometry::StraightLine rUpLine = boardGeometry.rightUpwardLineOfField(horizLine[linepos]);
				int rUpStartPos = rUpLine.indexOf(horizLine[linepos]);
				
				// Start note index: the value that has been set to the horizontal line element
				int rUpStartNoteIndex = this->mappingLogic->terpstraKeyToIndex(mappingData.sets[setSelection].theKeys[horizLine[linepos]]);

				fillLine(mappingData, setSelection, rUpLine, rUpStartPos, rUpStartNoteIndex, rUpwStepSize);
			}

			// XXX Not all fields have been filled yet - fill the rest now
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
                 parentClasses="public Component, public MappingLogicListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="428"
                 initialHeight="220">
  <BACKGROUND backgroundColour="ffbad0de">
    <PATH pos="0 0 100 100" fill="solid: ff010e0c" hasStroke="0" nonZeroWinding="1">s 188 132 l 212 136 l 192 144 x</PATH>
    <PATH pos="0 0 100 100" fill="solid: ff02020e" hasStroke="0" nonZeroWinding="1">s 168 104 l 168 120 l 160 120 x</PATH>
  </BACKGROUND>
  <COMBOBOX name="startingPointBox" id="d526f69bdc196fea" memberName="startingPointeBox"
            virtualName="" explicitFocusOrder="0" pos="24 128 150 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="labelStartingPoint" id="5401a3246c13771e" memberName="labelStartingPoint"
         virtualName="" explicitFocusOrder="0" pos="24 104 150 24" tooltip="Value that will be assigned to the key at mouse pposition when clicking"
         edTextCol="ff000000" edBkgCol="0" labelText="Starting value"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="labelHorizontalSteps" id="3e6663aecf1474c8" memberName="labelHorizontalSteps"
         virtualName="" explicitFocusOrder="0" pos="232 96 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Horizontal steps" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="editHorizontalSteps" id="8d2f5f07f337b9ef" memberName="editHorizontalSteps"
              virtualName="" explicitFocusOrder="0" pos="232 120 40 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="labelRightUpwardSteps" id="43530804741d9cb7" memberName="labelRightUpwardSteps"
         virtualName="" explicitFocusOrder="0" pos="160 48 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Right upward steps" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="editRightUpwardSteps" id="3a1cf8588366e0ca" memberName="editRightUpwardSteps"
              virtualName="" explicitFocusOrder="0" pos="160 72 39 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="editInstructionText" id="c03ef432c2b4599" memberName="editInstructionText"
         virtualName="" explicitFocusOrder="0" pos="8 8 416 40" edTextCol="ff000000"
         edBkgCol="0" labelText="Fill a line or the whole field with constant step distances. &#10;Click on desired key field to start."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="9"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
