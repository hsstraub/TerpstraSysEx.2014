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
//[/Headers]

#include "IncrMidiNotesMapping.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
IncrMidiNotesMapping::IncrMidiNotesMapping ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (channelAutoIncrButton = new ToggleButton ("channelAutoIncrButton"));
    channelAutoIncrButton->setTooltip (TRANS("Auto-increment channel after maximal note is reached"));
    channelAutoIncrButton->setButtonText (TRANS("Multichannel"));
    channelAutoIncrButton->setConnectedEdges (Button::ConnectedOnTop);
    channelAutoIncrButton->setRadioGroupId (1);
    channelAutoIncrButton->addListener (this);

    addAndMakeVisible (channelAutoIncrNoteBox = new ComboBox ("channelAutoIncrNoteBox"));
    channelAutoIncrNoteBox->setTooltip (TRANS("After reaching this note, the channel is incremented and the note is reset to 0 (in case of multichannel)"));
    channelAutoIncrNoteBox->setEditableText (false);
    channelAutoIncrNoteBox->setJustificationType (Justification::centredLeft);
    channelAutoIncrNoteBox->setTextWhenNothingSelected (String());
    channelAutoIncrNoteBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    channelAutoIncrNoteBox->addListener (this);

    addAndMakeVisible (labelMidiNotesUntil = new Label ("labelMidiNotesUntil",
                                                        TRANS("MIDI notes from 0 until")));
    labelMidiNotesUntil->setFont (Font (15.00f, Font::plain));
    labelMidiNotesUntil->setJustificationType (Justification::centredLeft);
    labelMidiNotesUntil->setEditable (false, false, false);
    labelMidiNotesUntil->setColour (TextEditor::textColourId, Colours::black);
    labelMidiNotesUntil->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (singleChannelButton = new ToggleButton ("singleChannelButton"));
    singleChannelButton->setTooltip (TRANS("Only the MIDI notes from a single channel"));
    singleChannelButton->setButtonText (TRANS("Single channel"));
    singleChannelButton->setConnectedEdges (Button::ConnectedOnTop);
    singleChannelButton->setRadioGroupId (1);
    singleChannelButton->addListener (this);

    addAndMakeVisible (channelBox = new ComboBox ("channelBox"));
    channelBox->setTooltip (TRANS("The MIDI channel (in case of single channel)"));
    channelBox->setEditableText (false);
    channelBox->setJustificationType (Justification::centredLeft);
    channelBox->setTextWhenNothingSelected (String());
    channelBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    channelBox->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (416, 220);


    //[Constructor] You can add your own custom stuff here..
	for (int i = 0; i <= 127; i++)
	{
		channelAutoIncrNoteBox->addItem(String(i), i + 1);
	}

	for (int i = 1; i <= 16; i++)
		channelBox->addItem(String(i), i);

	// Default: multi channel
	// Todo latest choice by user, from settings
	channelAutoIncrButton->setToggleState(true, juce::NotificationType::sendNotification);

	//noteLabel->attachToComponent(noteBox, true);
	//channelLabel->attachToComponent(channelBox, true);
    //[/Constructor]
}

IncrMidiNotesMapping::~IncrMidiNotesMapping()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    channelAutoIncrButton = nullptr;
    channelAutoIncrNoteBox = nullptr;
    labelMidiNotesUntil = nullptr;
    singleChannelButton = nullptr;
    channelBox = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void IncrMidiNotesMapping::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffb8d0de));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void IncrMidiNotesMapping::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    channelAutoIncrButton->setBounds (8, 64, 280, 24);
    channelAutoIncrNoteBox->setBounds (184, 8, 56, 24);
    labelMidiNotesUntil->setBounds (8, 8, 192, 24);
    singleChannelButton->setBounds (8, 40, 160, 24);
    channelBox->setBounds (184, 40, 56, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void IncrMidiNotesMapping::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == channelAutoIncrButton)
    {
        //[UserButtonCode_channelAutoIncrButton] -- add your button handler code here..
		if (channelAutoIncrButton->getToggleState())
		{
			channelBox->setVisible(false);
			this->mappingLogic.setChannelInCaseOfSingleChannel(0);	// 0 for no selection or 1-16
		}
        //[/UserButtonCode_channelAutoIncrButton]
    }
    else if (buttonThatWasClicked == singleChannelButton)
    {
        //[UserButtonCode_singleChannelButton] -- add your button handler code here..
		if (singleChannelButton->getToggleState())
		{
			channelBox->setVisible(true);
			// Make sure something is selected
			if (channelBox->getSelectedId() <= 0)
				channelBox->setSelectedId(1, juce::dontSendNotification);
			this->mappingLogic.setChannelInCaseOfSingleChannel(channelBox->getSelectedId());	// 0 for no selection or 1-16
		}
        //[/UserButtonCode_singleChannelButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void IncrMidiNotesMapping::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == channelAutoIncrNoteBox)
    {
        //[UserComboBoxCode_channelAutoIncrNoteBox] -- add your combo box handling code here..
		this->mappingLogic.setMaxMidiNote(channelAutoIncrNoteBox->getSelectedItemIndex());	 //-1 for no selection or 0-127
        //[/UserComboBoxCode_channelAutoIncrNoteBox]
    }
    else if (comboBoxThatHasChanged == channelBox)
    {
        //[UserComboBoxCode_channelBox] -- add your combo box handling code here..
		this->mappingLogic.setChannelInCaseOfSingleChannel(channelBox->getSelectedId());	// 0 for no selection or 1-16
        //[/UserComboBoxCode_channelBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="IncrMidiNotesMapping" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="416" initialHeight="220">
  <BACKGROUND backgroundColour="ffb8d0de"/>
  <TOGGLEBUTTON name="channelAutoIncrButton" id="1749290d10236ec3" memberName="channelAutoIncrButton"
                virtualName="" explicitFocusOrder="0" pos="8 64 280 24" tooltip="Auto-increment channel after maximal note is reached"
                buttonText="Multichannel" connectedEdges="4" needsCallback="1"
                radioGroupId="1" state="0"/>
  <COMBOBOX name="channelAutoIncrNoteBox" id="4560285c5e467e2f" memberName="channelAutoIncrNoteBox"
            virtualName="" explicitFocusOrder="0" pos="184 8 56 24" tooltip="After reaching this note, the channel is incremented and the note is reset to 0 (in case of multichannel)"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="labelMidiNotesUntil" id="c566693d4c0fdb01" memberName="labelMidiNotesUntil"
         virtualName="" explicitFocusOrder="0" pos="8 8 192 24" edTextCol="ff000000"
         edBkgCol="0" labelText="MIDI notes from 0 until" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="singleChannelButton" id="c96447a8b1eacf35" memberName="singleChannelButton"
                virtualName="" explicitFocusOrder="0" pos="8 40 160 24" tooltip="Only the MIDI notes from a single channel"
                buttonText="Single channel" connectedEdges="4" needsCallback="1"
                radioGroupId="1" state="0"/>
  <COMBOBOX name="channelBox" id="3ac4d54146311c84" memberName="channelBox"
            virtualName="" explicitFocusOrder="0" pos="184 40 56 24" tooltip="The MIDI channel (in case of single channel)"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
