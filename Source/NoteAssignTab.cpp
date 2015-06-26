/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "NoteAssignTab.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
NoteAssignTab::NoteAssignTab ()
{
    addAndMakeVisible (noteAndChannelAssGroup = new GroupComponent ("noteAndChannelAssGroup",
                                                                    TRANS("Key Note and Channel Assignments")));

    addAndMakeVisible (editInstructionText = new Label ("editInstructionText",
                                                        TRANS("Assign these values to a key by clicking on the desired key-face.")));
    editInstructionText->setFont (Font (15.00f, Font::plain));
    editInstructionText->setJustificationType (Justification::centredLeft);
    editInstructionText->setEditable (false, false, false);
    editInstructionText->setColour (TextEditor::textColourId, Colours::black);
    editInstructionText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (noteLabel = new Label ("noteLabel",
                                              TRANS("Note (0-127):")));
    noteLabel->setFont (Font (15.00f, Font::plain));
    noteLabel->setJustificationType (Justification::centredLeft);
    noteLabel->setEditable (false, false, false);
    noteLabel->setColour (TextEditor::textColourId, Colours::black);
    noteLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (noteBox = new ComboBox ("noteBox"));
    noteBox->setEditableText (false);
    noteBox->setJustificationType (Justification::centredLeft);
    noteBox->setTextWhenNothingSelected (String::empty);
    noteBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    noteBox->addListener (this);

    addAndMakeVisible (noteAutoIncrButton = new ToggleButton ("noteAutoIncrButton"));
    noteAutoIncrButton->setButtonText (TRANS("Auto Increment"));
    noteAutoIncrButton->addListener (this);

    addAndMakeVisible (channelLabel = new Label ("channelLabel",
                                                 TRANS("Channel (1-16):")));
    channelLabel->setFont (Font (15.00f, Font::plain));
    channelLabel->setJustificationType (Justification::centredLeft);
    channelLabel->setEditable (false, false, false);
    channelLabel->setColour (TextEditor::textColourId, Colours::black);
    channelLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (channelBox = new ComboBox ("channelBox"));
    channelBox->setEditableText (false);
    channelBox->setJustificationType (Justification::centredLeft);
    channelBox->setTextWhenNothingSelected (String::empty);
    channelBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    channelBox->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (380, 320);


    //[Constructor] You can add your own custom stuff here..
	for (int i = 0; i <= 127; i++)
		noteBox->addItem(String(i), i + 1);

	for (int i = 1; i <= 16; i++)
		channelBox->addItem(String(i), i);

	noteLabel->attachToComponent(noteBox, true);
	channelLabel->attachToComponent(channelBox, true);
    //[/Constructor]
}

NoteAssignTab::~NoteAssignTab()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    noteAndChannelAssGroup = nullptr;
    editInstructionText = nullptr;
    noteLabel = nullptr;
    noteBox = nullptr;
    noteAutoIncrButton = nullptr;
    channelLabel = nullptr;
    channelBox = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void NoteAssignTab::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void NoteAssignTab::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    noteAndChannelAssGroup->setBounds (8, 8, 368, 200);
    editInstructionText->setBounds (16, 32, 352, 24);
    noteLabel->setBounds (24, 64, 96, 24);
    noteBox->setBounds (128, 64, 55, 24);
    noteAutoIncrButton->setBounds (200, 64, 160, 24);
    channelLabel->setBounds (24, 96, 96, 24);
    channelBox->setBounds (128, 96, 56, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void NoteAssignTab::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == noteBox)
    {
        //[UserComboBoxCode_noteBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_noteBox]
    }
    else if (comboBoxThatHasChanged == channelBox)
    {
        //[UserComboBoxCode_channelBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_channelBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void NoteAssignTab::buttonClicked (Button* buttonThatWasClicked)
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



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

TerpstraKey NoteAssignTab::createKeyMapping()
{
	TerpstraKey keyData;
	keyData.noteNumber = noteBox->getSelectedItemIndex(); //-1 for no selection or 0-127
	if (keyData.noteNumber < 0) keyData.noteNumber = 0;
	keyData.channelNumber = channelBox->getSelectedId();	// 0 for no selection or 1-16

	// Auto increment
	if (noteAutoIncrButton->getToggleState())
		noteBox->setSelectedItemIndex(noteBox->getSelectedItemIndex() + 1);

	return keyData;
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="NoteAssignTab" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="380" initialHeight="320">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <GROUPCOMPONENT name="noteAndChannelAssGroup" id="cd12ee7381d485a1" memberName="noteAndChannelAssGroup"
                  virtualName="" explicitFocusOrder="0" pos="8 8 368 200" title="Key Note and Channel Assignments"/>
  <LABEL name="editInstructionText" id="c03ef432c2b4599" memberName="editInstructionText"
         virtualName="" explicitFocusOrder="0" pos="16 32 352 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Assign these values to a key by clicking on the desired key-face."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="noteLabel" id="86c397362e81fdd" memberName="noteLabel"
         virtualName="" explicitFocusOrder="0" pos="24 64 96 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Note (0-127):" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="noteBox" id="123cacc6155f964" memberName="noteBox" virtualName=""
            explicitFocusOrder="0" pos="128 64 55 24" editable="0" layout="33"
            items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TOGGLEBUTTON name="noteAutoIncrButton" id="49829699593b11f7" memberName="noteAutoIncrButton"
                virtualName="" explicitFocusOrder="0" pos="200 64 160 24" buttonText="Auto Increment"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="channelLabel" id="13a60ce682923955" memberName="channelLabel"
         virtualName="" explicitFocusOrder="0" pos="24 96 96 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Channel (1-16):" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="channelBox" id="208bbc8901c22319" memberName="channelBox"
            virtualName="" explicitFocusOrder="0" pos="128 96 56 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
