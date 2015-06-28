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
#include "ViewConstants.h"
//[/Headers]

#include "MidiEditArea.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MidiEditArea::MidiEditArea ()
{
    addAndMakeVisible (lblMidiInput = new Label ("lblMidiInput",
                                                 TRANS("MIDI Input Device")));
    lblMidiInput->setFont (Font (15.00f, Font::plain));
    lblMidiInput->setJustificationType (Justification::centredLeft);
    lblMidiInput->setEditable (false, false, false);
    lblMidiInput->setColour (TextEditor::textColourId, Colours::black);
    lblMidiInput->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (cbMidiInput = new ComboBox ("cbMidiInput"));
    cbMidiInput->setTooltip (TRANS("Is currently not used"));
    cbMidiInput->setEditableText (false);
    cbMidiInput->setJustificationType (Justification::centredLeft);
    cbMidiInput->setTextWhenNothingSelected (String::empty);
    cbMidiInput->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbMidiInput->addListener (this);

    addAndMakeVisible (lblMidiOutput = new Label ("lblMidiOutput",
                                                  TRANS("MIDI Output Device")));
    lblMidiOutput->setFont (Font (15.00f, Font::plain));
    lblMidiOutput->setJustificationType (Justification::centredLeft);
    lblMidiOutput->setEditable (false, false, false);
    lblMidiOutput->setColour (TextEditor::textColourId, Colours::black);
    lblMidiOutput->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (cbMidiOutput = new ComboBox ("cbMidiOutput"));
    cbMidiOutput->setTooltip (TRANS("Key mappings are sent to this port. This happens automatically if a valid MIDI port is selected."));
    cbMidiOutput->setEditableText (false);
    cbMidiOutput->setJustificationType (Justification::centredLeft);
    cbMidiOutput->setTextWhenNothingSelected (String::empty);
    cbMidiOutput->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbMidiOutput->addListener (this);

    addAndMakeVisible (toggleAutoSave = new ToggleButton ("toggleAutoSave"));
    toggleAutoSave->setTooltip (TRANS("If this is active, all edits are saved on controller automatically."));
    toggleAutoSave->setButtonText (TRANS("AutoSave"));
    toggleAutoSave->addListener (this);

    addAndMakeVisible (buttonSendSaveEdits = new TextButton ("buttonSendSaveEdits"));
    buttonSendSaveEdits->setTooltip (TRANS("Save edits that have been sent on controller"));
    buttonSendSaveEdits->setButtonText (TRANS("Send & Save Edits"));
    buttonSendSaveEdits->addListener (this);

    addAndMakeVisible (buttonSendAll = new TextButton ("buttonSendAll"));
    buttonSendAll->setTooltip (TRANS("Send the all key mappings to controller and save them there."));
    buttonSendAll->setButtonText (TRANS("Send & Save All"));

    addAndMakeVisible (buttonDiscard = new TextButton ("buttonDiscard"));
    buttonDiscard->setTooltip (TRANS("Discard edits on controller. This has the same effect as switching controller off and on again."));
    buttonDiscard->setButtonText (TRANS("Discard Edits"));
    buttonDiscard->addListener (this);


    //[UserPreSize]
	cbMidiInput->addItemList(MidiInput::getDevices(), 1);
	cbMidiOutput->addItemList(MidiOutput::getDevices(), 1);
    //[/UserPreSize]

    setSize (380, 130);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MidiEditArea::~MidiEditArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    lblMidiInput = nullptr;
    cbMidiInput = nullptr;
    lblMidiOutput = nullptr;
    cbMidiOutput = nullptr;
    toggleAutoSave = nullptr;
    buttonSendSaveEdits = nullptr;
    buttonSendAll = nullptr;
    buttonDiscard = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MidiEditArea::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MidiEditArea::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    lblMidiInput->setBounds (8, 8, 184, 16);
    cbMidiInput->setBounds (8, 32, 184, 24);
    lblMidiOutput->setBounds (200, 8, 184, 16);
    cbMidiOutput->setBounds (200, 32, 184, 24);
    toggleAutoSave->setBounds (8, 64, 168, 24);
    buttonSendSaveEdits->setBounds (8, 96, 150, 24);
    buttonSendAll->setBounds (200, 64, 150, 24);
    buttonDiscard->setBounds (200, 96, 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MidiEditArea::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cbMidiInput)
    {
        //[UserComboBoxCode_cbMidiInput] -- add your combo box handling code here..
        //[/UserComboBoxCode_cbMidiInput]
    }
    else if (comboBoxThatHasChanged == cbMidiOutput)
    {
        //[UserComboBoxCode_cbMidiOutput] -- add your combo box handling code here..
		if (midiDriver != nullptr)
			midiDriver->setMidiOutput(cbMidiOutput->getSelectedItemIndex());
        //[/UserComboBoxCode_cbMidiOutput]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void MidiEditArea::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]

	// "Send and save all" is handled in MainComponent

    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleAutoSave)
    {
        //[UserButtonCode_toggleAutoSave] -- add your button handler code here..
		if (midiDriver != nullptr)
			midiDriver->setAutoSave(toggleAutoSave->getToggleState());
        //[/UserButtonCode_toggleAutoSave]
    }
    else if (buttonThatWasClicked == buttonSendSaveEdits)
    {
        //[UserButtonCode_buttonSendSaveEdits] -- add your button handler code here..
		if (midiDriver != nullptr)
			midiDriver->storeAllToEEPROM();
        //[/UserButtonCode_buttonSendSaveEdits]
    }
    else if (buttonThatWasClicked == buttonDiscard)
    {
        //[UserButtonCode_buttonDiscard] -- add your button handler code here..
		if (midiDriver != nullptr)
			midiDriver->recallAllFromEEPROM();
        //[/UserButtonCode_buttonDiscard]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiEditArea" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="380" initialHeight="130">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <LABEL name="lblMidiInput" id="717ebff5200810bd" memberName="lblMidiInput"
         virtualName="" explicitFocusOrder="0" pos="8 8 184 16" edTextCol="ff000000"
         edBkgCol="0" labelText="MIDI Input Device" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="cbMidiInput" id="aad3610c2aac943b" memberName="cbMidiInput"
            virtualName="" explicitFocusOrder="0" pos="8 32 184 24" tooltip="Is currently not used"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="lblMidiOutput" id="f5bc70386d951a4a" memberName="lblMidiOutput"
         virtualName="" explicitFocusOrder="0" pos="200 8 184 16" edTextCol="ff000000"
         edBkgCol="0" labelText="MIDI Output Device" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="cbMidiOutput" id="f3f3544c4916f527" memberName="cbMidiOutput"
            virtualName="" explicitFocusOrder="0" pos="200 32 184 24" tooltip="Key mappings are sent to this port. This happens automatically if a valid MIDI port is selected."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TOGGLEBUTTON name="toggleAutoSave" id="b5157b1eef5898bb" memberName="toggleAutoSave"
                virtualName="" explicitFocusOrder="0" pos="8 64 168 24" tooltip="If this is active, all edits are saved on controller automatically."
                buttonText="AutoSave" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TEXTBUTTON name="buttonSendSaveEdits" id="b3ed9064acdde93" memberName="buttonSendSaveEdits"
              virtualName="" explicitFocusOrder="0" pos="8 96 150 24" tooltip="Save edits that have been sent on controller"
              buttonText="Send &amp; Save Edits" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="buttonSendAll" id="71e432722656a5b7" memberName="buttonSendAll"
              virtualName="" explicitFocusOrder="0" pos="200 64 150 24" tooltip="Send the all key mappings to controller and save them there."
              buttonText="Send &amp; Save All" connectedEdges="0" needsCallback="0"
              radioGroupId="0"/>
  <TEXTBUTTON name="buttonDiscard" id="8943d46ddc434616" memberName="buttonDiscard"
              virtualName="" explicitFocusOrder="0" pos="200 96 150 24" tooltip="Discard edits on controller. This has the same effect as switching controller off and on again."
              buttonText="Discard Edits" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
