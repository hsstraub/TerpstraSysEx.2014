/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "ViewConstants.h"
#include "Main.h"
//[/Headers]

#include "MidiEditArea.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MidiEditArea::MidiEditArea ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    lblMidiInput.reset (new Label ("lblMidiInput",
                                   TRANS("MIDI Input Device")));
    addAndMakeVisible (lblMidiInput.get());
    lblMidiInput->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    lblMidiInput->setJustificationType (Justification::centredLeft);
    lblMidiInput->setEditable (false, false, false);
    lblMidiInput->setColour (TextEditor::textColourId, Colours::black);
    lblMidiInput->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lblMidiInput->setBounds (8, 8, 184, 16);

    cbMidiInput.reset (new ComboBox ("cbMidiInput"));
    addAndMakeVisible (cbMidiInput.get());
    cbMidiInput->setTooltip (TRANS("Receives macro button controller events to send key mapping files to the keyboard."));
    cbMidiInput->setEditableText (false);
    cbMidiInput->setJustificationType (Justification::centredLeft);
    cbMidiInput->setTextWhenNothingSelected (String());
    cbMidiInput->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbMidiInput->addListener (this);

    cbMidiInput->setBounds (8, 32, 184, 24);

    lblMidiOutput.reset (new Label ("lblMidiOutput",
                                    TRANS("MIDI Output Device")));
    addAndMakeVisible (lblMidiOutput.get());
    lblMidiOutput->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    lblMidiOutput->setJustificationType (Justification::centredLeft);
    lblMidiOutput->setEditable (false, false, false);
    lblMidiOutput->setColour (TextEditor::textColourId, Colours::black);
    lblMidiOutput->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lblMidiOutput->setBounds (200, 8, 184, 16);

    cbMidiOutput.reset (new ComboBox ("cbMidiOutput"));
    addAndMakeVisible (cbMidiOutput.get());
    cbMidiOutput->setTooltip (TRANS("Key mappings are sent to this port. This happens automatically if a valid MIDI port is selected."));
    cbMidiOutput->setEditableText (false);
    cbMidiOutput->setJustificationType (Justification::centredLeft);
    cbMidiOutput->setTextWhenNothingSelected (String());
    cbMidiOutput->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbMidiOutput->addListener (this);

    cbMidiOutput->setBounds (200, 32, 184, 24);

    toggleAutoSave.reset (new ToggleButton ("toggleAutoSave"));
    addAndMakeVisible (toggleAutoSave.get());
    toggleAutoSave->setTooltip (TRANS("If this is active, all edits are saved on controller automatically."));
    toggleAutoSave->setButtonText (TRANS("AutoSave"));
    toggleAutoSave->addListener (this);

    toggleAutoSave->setBounds (8, 64, 168, 24);

    buttonSendSaveEdits.reset (new TextButton ("buttonSendSaveEdits"));
    addAndMakeVisible (buttonSendSaveEdits.get());
    buttonSendSaveEdits->setTooltip (TRANS("Save edits that have been sent on controller"));
    buttonSendSaveEdits->setButtonText (TRANS("Send & Save Edits"));
    buttonSendSaveEdits->addListener (this);

    buttonSendSaveEdits->setBounds (8, 96, 150, 24);

    buttonSendAll.reset (new TextButton ("buttonSendAll"));
    addAndMakeVisible (buttonSendAll.get());
    buttonSendAll->setTooltip (TRANS("Send the all key mappings to controller and save them there."));
    buttonSendAll->setButtonText (TRANS("Send & Save All"));

    buttonSendAll->setBounds (200, 64, 150, 24);

    buttonDiscard.reset (new TextButton ("buttonDiscard"));
    addAndMakeVisible (buttonDiscard.get());
    buttonDiscard->setTooltip (TRANS("Discard edits on controller. This has the same effect as switching controller off and on again."));
    buttonDiscard->setButtonText (TRANS("Discard Edits"));
    buttonDiscard->addListener (this);

    buttonDiscard->setBounds (200, 96, 150, 24);


    //[UserPreSize]
	cbMidiInput->addItemList(TerpstraSysExApplication::getApp().getMidiDriver().getMidiInputList(), 1);
	cbMidiOutput->addItemList(TerpstraSysExApplication::getApp().getMidiDriver().getMidiOutputList(), 1);
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

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MidiEditArea::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cbMidiInput.get())
    {
        //[UserComboBoxCode_cbMidiInput] -- add your combo box handling code here..
		TerpstraSysExApplication::getApp().getMidiDriver().setMidiInput(cbMidiInput->getSelectedItemIndex(), &TerpstraSysExApplication::getApp());
        //[/UserComboBoxCode_cbMidiInput]
    }
    else if (comboBoxThatHasChanged == cbMidiOutput.get())
    {
        //[UserComboBoxCode_cbMidiOutput] -- add your combo box handling code here..
		TerpstraSysExApplication::getApp().getMidiDriver().setMidiOutput(cbMidiOutput->getSelectedItemIndex());
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

    if (buttonThatWasClicked == toggleAutoSave.get())
    {
        //[UserButtonCode_toggleAutoSave] -- add your button handler code here..
		TerpstraSysExApplication::getApp().getMidiDriver().setAutoSave(toggleAutoSave->getToggleState());
        //[/UserButtonCode_toggleAutoSave]
    }
    else if (buttonThatWasClicked == buttonSendSaveEdits.get())
    {
        //[UserButtonCode_buttonSendSaveEdits] -- add your button handler code here..
		TerpstraSysExApplication::getApp().getMidiDriver().storeAllToEEPROM();
        //[/UserButtonCode_buttonSendSaveEdits]
    }
    else if (buttonThatWasClicked == buttonDiscard.get())
    {
        //[UserButtonCode_buttonDiscard] -- add your button handler code here..
		TerpstraSysExApplication::getApp().getMidiDriver().recallAllFromEEPROM();
        //[/UserButtonCode_buttonDiscard]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
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
         fontsize="1.5e1" kerning="0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="cbMidiInput" id="aad3610c2aac943b" memberName="cbMidiInput"
            virtualName="" explicitFocusOrder="0" pos="8 32 184 24" tooltip="Receives macro button controller events to send key mapping files to the keyboard."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="lblMidiOutput" id="f5bc70386d951a4a" memberName="lblMidiOutput"
         virtualName="" explicitFocusOrder="0" pos="200 8 184 16" edTextCol="ff000000"
         edBkgCol="0" labelText="MIDI Output Device" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="1.5e1" kerning="0" bold="0" italic="0" justification="33"/>
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

