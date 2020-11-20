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
    cbMidiInput->setTextWhenNothingSelected (TRANS("Select MIDI Input"));
    cbMidiInput->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbMidiInput->addListener (this);

    cbMidiInput->setBounds (232, 8, 184, 24);

    cbMidiOutput.reset (new juce::ComboBox ("cbMidiOutput"));
    addAndMakeVisible (cbMidiOutput.get());
    cbMidiOutput->setTooltip (TRANS("Key mappings are sent to this port. This happens automatically if a valid MIDI port is selected."));
    cbMidiOutput->setEditableText (false);
    cbMidiOutput->setJustificationType (juce::Justification::centredLeft);
    cbMidiOutput->setTextWhenNothingSelected (TRANS("Select MIDI Output"));
    cbMidiOutput->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbMidiOutput->addListener (this);

    cbMidiOutput->setBounds (432, 8, 184, 24);

    lblConnectionState.reset (new juce::Label ("lblConnectionState",
                                               TRANS("Disconnected")));
    addAndMakeVisible (lblConnectionState.get());
    lblConnectionState->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblConnectionState->setJustificationType (juce::Justification::centredLeft);
    lblConnectionState->setEditable (false, false, false);
    lblConnectionState->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblConnectionState->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblConnectionState->setBounds (624, 8, 150, 24);

    lblEditMode.reset (new juce::Label ("lblEditMode",
                                        TRANS("Edit Mode:")));
    addAndMakeVisible (lblEditMode.get());
    lblEditMode->setFont (juce::Font (18.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblEditMode->setJustificationType (juce::Justification::centredLeft);
    lblEditMode->setEditable (false, false, false);
    lblEditMode->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblEditMode->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblEditMode->setBounds (8, 8, 96, 24);


    //[UserPreSize]

	// Edit mode selector
	editModeSelector.reset(new TabbedButtonBar(TabbedButtonBar::Orientation::TabsAtTop));
	addAndMakeVisible(editModeSelector.get());
	editModeSelector->addTab("Live Editor", juce::Colours::lightgrey, 0);
	editModeSelector->addTab("Offline Editor", juce::Colours::lightgrey, 1);
	editModeSelector->addChangeListener(this);
	editModeSelector->setBounds(110, 8, 240, OCTAVEBOARDTABHEIGHT);

	lblEditMode->setVisible(false);
	editModeSelector->setVisible(false);

	cbMidiInput->addItemList(TerpstraSysExApplication::getApp().getMidiDriver().getMidiInputList(), 1);
	cbMidiOutput->addItemList(TerpstraSysExApplication::getApp().getMidiDriver().getMidiOutputList(), 1);

    //[/UserPreSize]

    setSize (928, 48);


    //[Constructor] You can add your own custom stuff here..
	TerpstraSysExApplication::getApp().getMidiDriver().addListener(this);

	lblConnectionState->setText("Disconnected", NotificationType::dontSendNotification);
	errorVisualizer.setErrorLevel(
		*lblConnectionState.get(),
		HajuErrorVisualizer::ErrorLevel::error,
		"Select both a MIDI input and a MIDI output");
    //[/Constructor]
}

MidiEditArea::~MidiEditArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    cbMidiInput = nullptr;
    cbMidiOutput = nullptr;
    lblConnectionState = nullptr;
    lblEditMode = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	TerpstraSysExApplication::getApp().getMidiDriver().removeListener(this);

	editModeSelector = nullptr;
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
	juce::FlexBox fb;
	fb.flexWrap = juce::FlexBox::Wrap::wrap;
	fb.justifyContent = juce::FlexBox::JustifyContent::center;
	fb.alignContent = juce::FlexBox::AlignContent::center;

	// ToDO Logo
	fb.items.add(juce::FlexItem(*lblEditMode).withMinHeight(lblEditMode->getHeight()).withMinWidth(lblEditMode->getWidth()).withFlex(1));
	fb.items.add(juce::FlexItem(*editModeSelector).withMinHeight(editModeSelector->getHeight()).withMinWidth(editModeSelector->getWidth()).withFlex(1));
	fb.items.add(juce::FlexItem(*cbMidiInput).withMinHeight(cbMidiInput->getHeight()).withMinWidth(cbMidiInput->getWidth()).withFlex(1));
	fb.items.add(juce::FlexItem(*cbMidiOutput).withMinHeight(cbMidiOutput->getHeight()).withMinWidth(cbMidiOutput->getWidth()).withFlex(1));
	fb.items.add(juce::FlexItem(*lblConnectionState).withMinHeight(lblConnectionState->getHeight()).withMinWidth(lblConnectionState->getWidth()).withFlex(1));


	fb.performLayout(getLocalBounds().toFloat());
    //[/UserResized]
}

void MidiEditArea::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cbMidiInput.get())
    {
        //[UserComboBoxCode_cbMidiInput] -- add your combo box handling code here..
		if (cbMidiInput->getSelectedItemIndex() >= 0)
			TerpstraSysExApplication::getApp().getMidiDriver().setMidiInput(cbMidiInput->getSelectedItemIndex()+1);

		if (cbMidiInput->getSelectedItemIndex() < 0 || cbMidiOutput->getSelectedItemIndex() < 0)
		{
			lblConnectionState->setText("Disconnected", NotificationType::dontSendNotification);
			errorVisualizer.setErrorLevel(
				*lblConnectionState.get(),
				HajuErrorVisualizer::ErrorLevel::error,
				"Select both a MIDI input and a MIDI output");
		}
		else
		{
			lblEditMode->setVisible(true);
			editModeSelector->setVisible(true);

			onOpenConnectionToDevice();
		}
        //[/UserComboBoxCode_cbMidiInput]
    }
    else if (comboBoxThatHasChanged == cbMidiOutput.get())
    {
        //[UserComboBoxCode_cbMidiOutput] -- add your combo box handling code here..
		if (cbMidiOutput->getSelectedItemIndex() >= 0)
			TerpstraSysExApplication::getApp().getMidiDriver().setMidiOutput(cbMidiOutput->getSelectedItemIndex());

		if (cbMidiInput->getSelectedItemIndex() < 0 || cbMidiOutput->getSelectedItemIndex() < 0)
		{
			lblConnectionState->setText("Disconnected", NotificationType::dontSendNotification);
			errorVisualizer.setErrorLevel(
				*lblConnectionState.get(),
				HajuErrorVisualizer::ErrorLevel::error,
				"Select both a MIDI input and a MIDI output");
		}
		else
		{
			lblEditMode->setVisible(true);
			editModeSelector->setVisible(true);

			onOpenConnectionToDevice();
		}
        //[/UserComboBoxCode_cbMidiOutput]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MidiEditArea::onOpenConnectionToDevice()
{
	jassert(cbMidiInput->getSelectedItemIndex() >= 0 && cbMidiOutput->getSelectedItemIndex() >= 0 && editModeSelector->getCurrentTabIndex() == midiEditMode::liveEditor);

	// if editing operations were done that have not been saved, warn that edits will be overwritten when configuration is read from device
	if (TerpstraSysExApplication::getApp().getHasChangesToSave())
	{
		auto retc = AlertWindow::showOkCancelBox(
			AlertWindow::AlertIconType::QuestionIcon, 
			"Establishing connection to controller",
			"The controller's current configuration will be received now. This will overwrite all edits you have done, Do you want to continue?");

		if (retc == false)
		{
			editModeSelector->setCurrentTabIndex(midiEditMode::offlineEditor, true);
			return;
		}
	}

	TerpstraSysExApplication::getApp().resetSysExMapping();

	lblConnectionState->setText("Connecting", NotificationType::dontSendNotification);
	errorVisualizer.setErrorLevel(
		*lblConnectionState.get(),
		HajuErrorVisualizer::ErrorLevel::noError,
		"Connecting");

	requestConfigurationFromDevice();
}

void MidiEditArea::requestConfigurationFromDevice()
{
	// Request MIDI channel, MIDI note, colour and key type config for all keys
	TerpstraSysExApplication::getApp().getMidiDriver().sendGetCompleteMappingRequest();	
	
	// ToDo Request values for general options
	// ToDo Request velocity curve config
}


void MidiEditArea::changeListenerCallback(ChangeBroadcaster *source)
{
	if (source == editModeSelector.get())
	{
		auto editMode = editModeSelector->getCurrentTabIndex();

		switch (editMode)
		{
		case midiEditMode::liveEditor:
			onOpenConnectionToDevice();
			break;

		case midiEditMode::offlineEditor:
			lblConnectionState->setText("Disconnected", NotificationType::dontSendNotification);
			errorVisualizer.setErrorLevel(
				*lblConnectionState.get(),
				HajuErrorVisualizer::ErrorLevel::error,
				"Offline mode");

			// Remove all MIDI messages in queue waiting to be sent
			TerpstraSysExApplication::getApp().getMidiDriver().clearMIDIMessageBuffer();
			break;

		default:
			jassertfalse;
			break;
		}
	}
}

void MidiEditArea::midiMessageReceived(const MidiMessage& midiMessage)
{
	if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsTerpstraConfigurationDataReceptionMessage(midiMessage))
	{
		lblConnectionState->setText("Connected", NotificationType::dontSendNotification);

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
                 parentClasses="public Component, public TerpstraMidiDriver::Listener, public ChangeListener"
                 constructorParams="" variableInitialisers="errorVisualizer(TerpstraSysExApplication::getApp().getLookAndFeel())"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="928" initialHeight="48">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <COMBOBOX name="cbMidiInput" id="aad3610c2aac943b" memberName="cbMidiInput"
            virtualName="" explicitFocusOrder="0" pos="232 8 184 24" tooltip="Receives answers to sent SysEx commands and the current configuration from controller "
            editable="0" layout="33" items="" textWhenNonSelected="Select MIDI Input"
            textWhenNoItems="(no choices)"/>
  <COMBOBOX name="cbMidiOutput" id="f3f3544c4916f527" memberName="cbMidiOutput"
            virtualName="" explicitFocusOrder="0" pos="432 8 184 24" tooltip="Key mappings are sent to this port. This happens automatically if a valid MIDI port is selected."
            editable="0" layout="33" items="" textWhenNonSelected="Select MIDI Output"
            textWhenNoItems="(no choices)"/>
  <LABEL name="lblConnectionState" id="132031ea466ea84f" memberName="lblConnectionState"
         virtualName="" explicitFocusOrder="0" pos="624 8 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Disconnected" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblEditMode" id="ea4ef68f1765a3d4" memberName="lblEditMode"
         virtualName="" explicitFocusOrder="0" pos="8 8 96 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Edit Mode:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="18.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

