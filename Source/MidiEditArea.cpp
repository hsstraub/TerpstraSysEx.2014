/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.4

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

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
    : errorVisualizer(TerpstraSysExApplication::getApp().getLookAndFeel())
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    cbMidiInput.reset (new juce::ComboBox ("cbMidiInput"));
    addAndMakeVisible (cbMidiInput.get());
    cbMidiInput->setTooltip (TRANS("Receives answers to sent SysEx commands and the current configuration from controller "));
    cbMidiInput->setEditableText (false);
    cbMidiInput->setJustificationType (juce::Justification::centredLeft);
    cbMidiInput->setTextWhenNothingSelected (juce::String());
    cbMidiInput->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbMidiInput->addItem (TRANS("Select MIDI Input"), 1);
    cbMidiInput->addListener (this);

    cbMidiInput->setBounds (8, 8, 184, 24);

    cbMidiOutput.reset (new juce::ComboBox ("cbMidiOutput"));
    addAndMakeVisible (cbMidiOutput.get());
    cbMidiOutput->setTooltip (TRANS("Key mappings are sent to this port. This happens automatically if a valid MIDI port is selected."));
    cbMidiOutput->setEditableText (false);
    cbMidiOutput->setJustificationType (juce::Justification::centredLeft);
    cbMidiOutput->setTextWhenNothingSelected (juce::String());
    cbMidiOutput->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbMidiOutput->addItem (TRANS("Select MIDI Output"), 1);
    cbMidiOutput->addListener (this);

    cbMidiOutput->setBounds (200, 8, 184, 24);

    lblConnectionState.reset (new juce::Label ("lblConnectionState",
                                               TRANS("Disconnected")));
    addAndMakeVisible (lblConnectionState.get());
    lblConnectionState->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblConnectionState->setJustificationType (juce::Justification::centredLeft);
    lblConnectionState->setEditable (false, false, false);
    lblConnectionState->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblConnectionState->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblConnectionState->setBounds (400, 8, 150, 24);


    //[UserPreSize]
	cbMidiInput->addItemList(TerpstraSysExApplication::getApp().getMidiDriver().getMidiInputList(), 1);
	cbMidiOutput->addItemList(TerpstraSysExApplication::getApp().getMidiDriver().getMidiOutputList(), 1);

    //[/UserPreSize]

    setSize (566, 40);


    //[Constructor] You can add your own custom stuff here..
	cbMidiInput->setSelectedItemIndex(0, juce::NotificationType::sendNotification);
	cbMidiOutput->setSelectedItemIndex(0, juce::NotificationType::sendNotification);

	TerpstraSysExApplication::getApp().getMidiDriver().addListener(this);
    //[/Constructor]
}

MidiEditArea::~MidiEditArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    cbMidiInput = nullptr;
    cbMidiOutput = nullptr;
    lblConnectionState = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	TerpstraSysExApplication::getApp().getMidiDriver().removeListener(this);
    //[/Destructor]
}

//==============================================================================
void MidiEditArea::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(findColour(ResizableWindow::backgroundColourId));
    //[/UserPaint]
}

void MidiEditArea::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MidiEditArea::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cbMidiInput.get())
    {
        //[UserComboBoxCode_cbMidiInput] -- add your combo box handling code here..
		TerpstraSysExApplication::getApp().getMidiDriver().setMidiInput(cbMidiInput->getSelectedItemIndex());
        //[/UserComboBoxCode_cbMidiInput]
    }
    else if (comboBoxThatHasChanged == cbMidiOutput.get())
    {
        //[UserComboBoxCode_cbMidiOutput] -- add your combo box handling code here..
		TerpstraSysExApplication::getApp().getMidiDriver().setMidiOutput(cbMidiOutput->getSelectedItemIndex());
        //[/UserComboBoxCode_cbMidiOutput]
    }

	// If both in- and output have a value: connection can be connecting, else disconnected
	// ToDo


    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MidiEditArea::midiMessageReceived(const MidiMessage& midiMessage)
{
	// Set connection state
	// ToDo

	// AD hoc: former MIDI infor farea functionality
	lblConnectionState->setText("<< " + midiMessage.getDescription(), NotificationType::dontSendNotification);

	if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsTerpstraConfigurationDataReceptionMessage(midiMessage))
	{
		if (midiMessage.getSysExDataSize() < 6)
		{
			errorVisualizer.setErrorLevel(
				*lblConnectionState.get(),
				HajuErrorVisualizer::ErrorLevel::error,
				"Message too short");
		}
		else
		{
			auto sysExData = midiMessage.getSysExData();
			auto answerState = sysExData[5];

			switch (answerState)
			{
			case TerpstraMidiDriver::TerpstraMIDIAnswerReturnCode::NACK:  // Not recognized
				errorVisualizer.setErrorLevel(
					*lblConnectionState.get(),
					HajuErrorVisualizer::ErrorLevel::error,
					"Not Recognized");
				break;

			case TerpstraMidiDriver::TerpstraMIDIAnswerReturnCode::ACK:  // Acknowledged, OK
				errorVisualizer.setErrorLevel(
					*lblConnectionState.get(),
					HajuErrorVisualizer::ErrorLevel::noError,
					"Ack");
				break;

			case TerpstraMidiDriver::TerpstraMIDIAnswerReturnCode::BUSY: // Controller busy
				errorVisualizer.setErrorLevel(
					*lblConnectionState.get(),
					HajuErrorVisualizer::ErrorLevel::warning,
					"Busy");
				break;

			case TerpstraMidiDriver::TerpstraMIDIAnswerReturnCode::ERROR:    // Error
				errorVisualizer.setErrorLevel(
					*lblConnectionState.get(),
					HajuErrorVisualizer::ErrorLevel::error,
					"Error from device");
				break;

			default:
				errorVisualizer.setErrorLevel(
					*lblConnectionState.get(),
					HajuErrorVisualizer::ErrorLevel::noError,
					"");
				break;
			}
		}
	}
	else
	{
		errorVisualizer.setErrorLevel(*lblConnectionState.get(), HajuErrorVisualizer::ErrorLevel::noError, "");
	}
}

void MidiEditArea::midiMessageSent(const MidiMessage& midiMessage)
{
	lblConnectionState->setText(">> " + midiMessage.getDescription(), NotificationType::dontSendNotification);
}

void MidiEditArea::midiSendQueueSize(int queueSize)
{
	// ToDo
}

void MidiEditArea::generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel)
{
	lblConnectionState->setText(textMessage, NotificationType::dontSendNotification);
	errorVisualizer.setErrorLevel(*lblConnectionState.get(), errorLevel, textMessage);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiEditArea" componentName=""
                 parentClasses="public Component, public TerpstraMidiDriver::Listener"
                 constructorParams="" variableInitialisers="errorVisualizer(TerpstraSysExApplication::getApp().getLookAndFeel())"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="566" initialHeight="40">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <COMBOBOX name="cbMidiInput" id="aad3610c2aac943b" memberName="cbMidiInput"
            virtualName="" explicitFocusOrder="0" pos="8 8 184 24" tooltip="Receives answers to sent SysEx commands and the current configuration from controller "
            editable="0" layout="33" items="Select MIDI Input" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <COMBOBOX name="cbMidiOutput" id="f3f3544c4916f527" memberName="cbMidiOutput"
            virtualName="" explicitFocusOrder="0" pos="200 8 184 24" tooltip="Key mappings are sent to this port. This happens automatically if a valid MIDI port is selected."
            editable="0" layout="33" items="Select MIDI Output" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <LABEL name="lblConnectionState" id="132031ea466ea84f" memberName="lblConnectionState"
         virtualName="" explicitFocusOrder="0" pos="400 8 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Disconnected" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

