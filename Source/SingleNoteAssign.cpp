/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "SingleNoteAssign.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SingleNoteAssign::SingleNoteAssign ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (editInstructionText = new Label ("editInstructionText",
                                                        TRANS("Assign value to a key by clicking on the desired key-face.")));
    editInstructionText->setFont (Font (15.00f, Font::plain));
    editInstructionText->setJustificationType (Justification::topLeft);
    editInstructionText->setEditable (false, false, false);
    editInstructionText->setColour (TextEditor::textColourId, Colours::black);
    editInstructionText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (noteAutoIncrButton = new ToggleButton ("noteAutoIncrButton"));
    noteAutoIncrButton->setButtonText (TRANS("Auto Increment after click"));
    noteAutoIncrButton->addListener (this);

    addAndMakeVisible (nextValueBox = new ComboBox ("nextValueBox"));
    nextValueBox->setEditableText (false);
    nextValueBox->setJustificationType (Justification::centredLeft);
    nextValueBox->setTextWhenNothingSelected (String());
    nextValueBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    nextValueBox->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (428, 220);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SingleNoteAssign::~SingleNoteAssign()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    editInstructionText = nullptr;
    noteAutoIncrButton = nullptr;
    nextValueBox = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SingleNoteAssign::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SingleNoteAssign::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    editInstructionText->setBounds (8, 8, 416, 32);
    noteAutoIncrButton->setBounds (8, 32, 200, 24);
    nextValueBox->setBounds (16, 72, 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SingleNoteAssign::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == noteAutoIncrButton)
    {
        //[UserButtonCode_noteAutoIncrButton] -- add your button handler code here..
        //[/UserButtonCode_noteAutoIncrButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void SingleNoteAssign::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == nextValueBox)
    {
        //[UserComboBoxCode_nextValueBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_nextValueBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

// Implementation of MappingLogicListener
void SingleNoteAssign::mappingLogicChanged(MappingLogicBase* mappingLogicThatChanged)
{
	// Fill note combo with values according to mapping logic
	nextValueBox->clear(juce::NotificationType::dontSendNotification);

	for (int i = 0; i < mappingLogicThatChanged->globalMappingSize(); i++)
	{
		TerpstraKey keyData = mappingLogicThatChanged->indexToTerpstraKey(i);
		// XXX format text
		nextValueBox->addItem(String(i) + ": Key_" + String(keyData.noteNumber) + ", Chan_" + String(keyData.channelNumber), i + 1);
	}
}

// Called from MainComponent when one of the keys  is clicked
TerpstraKey SingleNoteAssign::createKeyMapping()
{
	TerpstraKey keyData;
	//keyData.noteNumber = noteBox->getSelectedItemIndex(); //-1 for no selection or 0-127
	//if (keyData.noteNumber < 0) keyData.noteNumber = 0;
	//keyData.channelNumber = channelBox->getSelectedId();	// 0 for no selection or 1-16

	// Auto increment note
	// XXX
	//if (noteAutoIncrButton->getToggleState())
	//{
	//	int newNote = keyData.noteNumber + 1;

	//	// Auto increment channel
	//	if (channelAutoIncrButton->getToggleState() && channelAutoIncrNoteBox->getSelectedItemIndex() > 0 &&
	//		newNote > channelAutoIncrNoteBox->getSelectedItemIndex())
	//	{
	//		newNote = 0;
	//		int newChannel = keyData.channelNumber + 1;
	//		if (newChannel > 16)
	//			newChannel = 1;
	//		channelBox->setSelectedId(newChannel);
	//	}

	//	if (newNote > 127)
	//		newNote = 0;

	//	noteBox->setSelectedItemIndex(newNote);
	//}

	return keyData;
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SingleNoteAssign" componentName=""
                 parentClasses="public Component, public MappingLogicListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="428"
                 initialHeight="220">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <LABEL name="editInstructionText" id="c03ef432c2b4599" memberName="editInstructionText"
         virtualName="" explicitFocusOrder="0" pos="8 8 416 32" edTextCol="ff000000"
         edBkgCol="0" labelText="Assign value to a key by clicking on the desired key-face."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="9"/>
  <TOGGLEBUTTON name="noteAutoIncrButton" id="49829699593b11f7" memberName="noteAutoIncrButton"
                virtualName="" explicitFocusOrder="0" pos="8 32 200 24" buttonText="Auto Increment after click"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="nextValueBox" id="d526f69bdc196fea" memberName="nextValueBox"
            virtualName="" explicitFocusOrder="0" pos="16 72 150 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
