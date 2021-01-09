/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.5

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

// Index in edit mode tab coincides with sysExSendingMode. In case that changes in the future, modify this here.
TerpstraMidiDriver::sysExSendingMode editModeTabIndexToMidiSysExSendingMode(int tabIndex) { return static_cast<TerpstraMidiDriver::sysExSendingMode>(tabIndex); }

//[/MiscUserDefs]

//==============================================================================
MidiEditArea::MidiEditArea ()
    : Component("MidiEditArea"),
	  lookAndFeel(static_cast<LumatoneEditorLookAndFeel&>(TerpstraSysExApplication::getApp().getLookAndFeel())),
	  errorVisualizer(TerpstraSysExApplication::getApp().getLookAndFeel())
{
    //[Constructor_pre] You can add your own custom stuff here..
	lumatoneLabel.reset(new Label("LumatoneLabel", "lumatone"));
	lumatoneLabel->setFont(LumatoneEditorFonts::UniviaProBold());
	lumatoneLabel->setColour(Label::ColourIds::textColourId, lookAndFeel.findColour(LumatoneEditorColourIDs::LabelPink));
	lumatoneLabel->setJustificationType(Justification::centred);
	addAndMakeVisible(lumatoneLabel.get());

	liveEditorBtn.reset(new TextButton("LiveEditorButton"));
	lookAndFeel.setupRadioTextButton(*liveEditorBtn, 10, true);
	liveEditorBtn->setButtonText(translate("LiveEditor"));
	liveEditorBtn->setConnectedEdges(Button::ConnectedOnRight);
	liveEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontOverride, "Univia Pro");
	liveEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontOverrideTypefaceStyle, "Bold");
	liveEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, editModeFontScalar);
	addChildComponent(liveEditorBtn.get());
	liveEditorBtn->addListener(this);

	offlineEditorBtn.reset(new TextButton("OfflineEditorButton"));
	lookAndFeel.setupRadioTextButton(*offlineEditorBtn, 10, false);
	offlineEditorBtn->setButtonText(translate("OfflineEditor"));
	offlineEditorBtn->setConnectedEdges(Button::ConnectedOnLeft);
	offlineEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontOverride, "Univia Pro");
	offlineEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontOverrideTypefaceStyle, "Bold");
	offlineEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, editModeFontScalar);
	addChildComponent(offlineEditorBtn.get());
	offlineEditorBtn->addListener(this);

	pleaseConnectLabel.reset(new Label("PleaseConnectLabel", translate("PleaseConnect")));
	pleaseConnectLabel->setFont(LumatoneEditorFonts::UniviaProBold());
	pleaseConnectLabel->setColour(Label::ColourIds::textColourId, lookAndFeel.findColour(LumatoneEditorColourIDs::ActiveText));
	addAndMakeVisible(*pleaseConnectLabel);

	offlineMsgLabel.reset(new Label("DirectionsLabel", translate("OfflineMessage")));
	offlineMsgLabel->setFont(LumatoneEditorFonts::FranklinGothic());
	offlineMsgLabel->setColour(Label::ColourIds::textColourId, lookAndFeel.findColour(LumatoneEditorColourIDs::DescriptionText));
	addAndMakeVisible(*offlineMsgLabel);

	logomark.reset(new Component());
	addAndMakeVisible(logomark.get());

	logomarkPath = createLogomark();
    //[/Constructor_pre]

    cbMidiInput.reset (new juce::ComboBox ("cbMidiInput"));
    addAndMakeVisible (cbMidiInput.get());
	lookAndFeel.setupComboBox(*cbMidiInput.get());
    cbMidiInput->setTooltip (translate("InputTooltip"));
    cbMidiInput->setEditableText (false);
    cbMidiInput->setJustificationType (juce::Justification::centredLeft);
    cbMidiInput->setTextWhenNothingSelected (translate("SelectMIDIInput"));
    cbMidiInput->setTextWhenNoChoicesAvailable (translate("NoInputDevices"));
    cbMidiInput->addListener (this);

    cbMidiOutput.reset (new juce::ComboBox ("cbMidiOutput"));
    addAndMakeVisible (cbMidiOutput.get());
	lookAndFeel.setupComboBox(*cbMidiOutput.get());
    cbMidiOutput->setTooltip (translate("OutputToolTip"));
    cbMidiOutput->setEditableText (false);
    cbMidiOutput->setJustificationType (juce::Justification::centredLeft);
    cbMidiOutput->setTextWhenNothingSelected (translate("SelectMIDIOutput"));
    cbMidiOutput->setTextWhenNoChoicesAvailable (translate("NoOutputDevices"));
    cbMidiOutput->addListener (this);

    lblConnectionState.reset (new juce::Label ("lblConnectionState", connectedText[0]));
    addAndMakeVisible (lblConnectionState.get());
	lblConnectionState->setFont(LumatoneEditorFonts::UniviaProBold());
	lblConnectionState->setColour(Label::ColourIds::textColourId, connectedColours[connectedToLumatone]);

    lblEditMode.reset (new juce::Label ("lblEditMode", TRANS("EditMode")));
    addAndMakeVisible (lblEditMode.get());
	lblEditMode->setFont(LumatoneEditorFonts::UniviaProBold());
	lblEditMode->setColour(Label::ColourIds::textColourId, lookAndFeel.findColour(LumatoneEditorColourIDs::LabelPink));

    //[UserPreSize]

	cbMidiInput->addItemList(TerpstraSysExApplication::getApp().getMidiDriver().getMidiInputList(), 1);
	cbMidiOutput->addItemList(TerpstraSysExApplication::getApp().getMidiDriver().getMidiOutputList(), 1);

    //[/UserPreSize]


    //[Constructor] You can add your own custom stuff here..
	TerpstraSysExApplication::getApp().getMidiDriver().addListener(this);

	setConnectivity(false);
	errorVisualizer.setErrorLevel(
		*lblConnectionState.get(),
		HajuErrorVisualizer::ErrorLevel::error,
		"Select both a MIDI input and a MIDI output");
    //[/Constructor]
}

MidiEditArea::~MidiEditArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	liveEditorBtn = nullptr;
	offlineEditorBtn = nullptr;
	offlineMsgLabel = nullptr;
	pleaseConnectLabel = nullptr;
	logomark = nullptr;
    //[/Destructor_pre]

    cbMidiInput = nullptr;
    cbMidiOutput = nullptr;
    lblConnectionState = nullptr;
    lblEditMode = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	TerpstraSysExApplication::getApp().getMidiDriver().removeListener(this);

    //[/Destructor]
}

//==============================================================================
void MidiEditArea::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(lookAndFeel.findColour(LumatoneEditorColourIDs::LightBackground));

	// Dark backrgound for title and logomark
	g.setColour(lookAndFeel.findColour(LumatoneEditorColourIDs::DarkBackground));
	g.fillRect(lumatoneLabelBounds);
	g.fillRect(connectivityArea);

	// Rounded rect for device boxes
	if (!connectedToLumatone)
	{
		g.setColour(lookAndFeel.findColour(LumatoneEditorColourIDs::LightBackground));
		g.fillRoundedRectangle(ioBounds, proportionOfHeight(controlBoundsCornerRadius));
	}

	g.setColour(connectedColours[connectedToLumatone]);
	drawPathToFillBounds(g, logomarkPath, logomarkBounds);
    //[/UserPaint]
}

void MidiEditArea::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..

	lumatoneLabelBounds = getBounds().withRight(proportionOfWidth(lumatoneLabelAreaWidth));
	resizeLabelWithWidth(lumatoneLabel.get(), lumatoneLabelBounds.proportionOfWidth(lumatoneLabelWidthInArea));
	lumatoneLabel->setCentrePosition(lumatoneLabelBounds.getCentre());

	resizeLabelWithHeight(lblConnectionState.get(), proportionOfHeight(connectivityHeight));

	// Also used to position logomark
	ioBounds.setBounds(
		proportionOfWidth(controlBoundsX), proportionOfHeight(controlBoundsY),
		proportionOfWidth(controlBoundsWidth), proportionOfHeight(controlBoundsHeight)
	);

	if (connectedToLumatone)
	{
		resizeLabelWithHeight(lblEditMode.get(), proportionOfHeight(editModeHeight));
		lblEditMode->setTopLeftPosition(
			lumatoneLabelBounds.getRight() + proportionOfWidth(editModeX),
			round((getHeight() - lblEditMode->getHeight()) / 2.0f)
		);

		liveEditorBtn->setSize(proportionOfWidth(liveEditButtonWidth), proportionOfHeight(editModeButtonHeight));
		liveEditorBtn->setTopLeftPosition(
			lumatoneLabelBounds.getRight() + proportionOfWidth(editModeButtonX),
			round((getHeight() - liveEditorBtn->getHeight()) / 2.0f)
		);

		offlineEditorBtn->setBounds(
			liveEditorBtn->getRight(), liveEditorBtn->getY(), proportionOfWidth(offlineEditButtonWidth), liveEditorBtn->getHeight()
		);

		connectivityArea = getBounds().toFloat().withLeft(proportionOfWidth(connectedAreaX));

		lblConnectionState->setTopLeftPosition(
			proportionOfWidth(connectedX),
			round((getHeight() - lblConnectionState->getHeight()) / 2.0f)
		);
	}
	else
	{
		connectivityArea = getBounds().toFloat().withLeft(proportionOfWidth(disconnectedAreaX));

		lblConnectionState->setTopLeftPosition(
			ioBounds.getX() + ioBounds.proportionOfWidth(disconnectedControlBoundsX),
			round(ioBounds.getY() + (ioBounds.getHeight() - lblConnectionState->getHeight()) / 2.05f)
		);

		cbMidiInput->setBounds(ioBounds.getProportion(Rectangle<float>(
			{ midiInputControlBoundsX, midiDeviceControlBoundsHeight / 4.0f, midiDeviceControlBoundsWidth, midiDeviceControlBoundsHeight }
		)).toNearestInt());

		cbMidiOutput->setBounds(cbMidiInput->getBounds().withX(
			ioBounds.getX() + ioBounds.proportionOfWidth(midiOutputControlBoundsX))
		);

		resizeLabelWithHeight(pleaseConnectLabel.get(), proportionOfHeight(pleaseConnectHeight));
		pleaseConnectLabel->setTopLeftPosition(proportionOfWidth(pleaseConnectX), proportionOfHeight(pleaseConnectY));

		offlineMsgLabel->setBounds(
			proportionOfWidth(connectionDirectionsX), proportionOfHeight(connectionDirectionsY),
			proportionOfWidth(connectionDirectionsWidth), proportionOfHeight(connectionDirectionsHeight)
		);
		offlineMsgLabel->setFont(offlineMsgLabel->getFont().withHeight(offlineMsgLabel->getHeight()));
	}

	logomarkBounds.setSize(proportionOfHeight(logomarkHeight), proportionOfHeight(logomarkHeight));
	logomarkBounds.setCentre(ioBounds.getRight() + roundToInt((getWidth() - ioBounds.getRight()) / 2.0f), proportionOfHeight(0.5f));
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
			setConnectivity(false);
			errorVisualizer.setErrorLevel(
				*lblConnectionState.get(),
				HajuErrorVisualizer::ErrorLevel::error,
				"Select both a MIDI input and a MIDI output");
		}
		else
		{
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
			setConnectivity(false);
			errorVisualizer.setErrorLevel(
				*lblConnectionState.get(),
				HajuErrorVisualizer::ErrorLevel::error,
				"Select both a MIDI input and a MIDI output");
		}
		else
		{
			onOpenConnectionToDevice();
		}
        //[/UserComboBoxCode_cbMidiOutput]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MidiEditArea::lookAndFeelChanged()
{
	connectedColours.clear();
	connectedColours.add(getLookAndFeel().findColour(LumatoneEditorColourIDs::DisconnectedRed));
	connectedColours.add(getLookAndFeel().findColour(LumatoneEditorColourIDs::ConnectedGreen));
}

void MidiEditArea::buttonClicked(Button* btn)
{
	auto sysExSendingMode = editModeTabIndexToMidiSysExSendingMode((int)liveEditorBtn->getToggleState());

	TerpstraSysExApplication::getApp().getMidiDriver().setSysExSendingMode(sysExSendingMode);

	switch (sysExSendingMode)
	{
	case TerpstraMidiDriver::sysExSendingMode::liveEditor:
		onOpenConnectionToDevice();
		break;

	case TerpstraMidiDriver::sysExSendingMode::offlineEditor:
		lblConnectionState->setText(translate("OfflineMode"), NotificationType::dontSendNotification);
		errorVisualizer.setErrorLevel(
			*lblConnectionState.get(),
			HajuErrorVisualizer::ErrorLevel::noError,
			"Offline mode");
		break;

	default:
		jassertfalse;
		break;
	}
}

void MidiEditArea::setConnectivity(bool isConnected)
{
	if ((!connectedToLumatone && isConnected) || (connectedToLumatone && !isConnected))
	{
		cbMidiInput->setVisible(!isConnected);
		cbMidiOutput->setVisible(!isConnected);
		pleaseConnectLabel->setVisible(!isConnected);
		offlineMsgLabel->setVisible(!isConnected);

		lblConnectionState->setText(connectedText[isConnected], dontSendNotification);
		lblConnectionState->setColour(Label::ColourIds::textColourId, connectedColours[isConnected]);

		lblEditMode->setVisible(isConnected);
		liveEditorBtn->setVisible(isConnected);
		offlineEditorBtn->setVisible(isConnected);

		connectedToLumatone = isConnected;

		resized();
		repaint();
	}
}

void MidiEditArea::onOpenConnectionToDevice()
{
	jassert(cbMidiInput->getSelectedItemIndex() >= 0 && cbMidiOutput->getSelectedItemIndex() >= 0 && liveEditorBtn->getToggleState());

	// if editing operations were done that have not been saved, warn that edits will be overwritten when configuration is read from device
	if (TerpstraSysExApplication::getApp().getHasChangesToSave())
	{
		auto retc = AlertWindow::showOkCancelBox(
			AlertWindow::AlertIconType::QuestionIcon,
			"Establishing connection to controller",
			"The controller's current configuration will be received now. This will overwrite all edits you have done, Do you want to continue?");

		if (retc == false)
		{
			offlineEditorBtn->setToggleState(true, NotificationType::sendNotification);
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

	// General options
	// ToDo AfterTouchActive
	// ToDo LightOnKeyStrokes
	// ToDo invertFootController
	// ToDO expressionControllerSensivity

	// Velocity curve config
	TerpstraSysExApplication::getApp().getMidiDriver().sendVelocityIntervalConfigRequest();
	TerpstraSysExApplication::getApp().getMidiDriver().sendVelocityConfigurationRequest(TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff);
	TerpstraSysExApplication::getApp().getMidiDriver().sendVelocityConfigurationRequest(TerpstraMidiDriver::VelocityCurveType::fader);
	TerpstraSysExApplication::getApp().getMidiDriver().sendVelocityConfigurationRequest(TerpstraMidiDriver::VelocityCurveType::afterTouch);
}


void MidiEditArea::midiMessageReceived(const MidiMessage& midiMessage)
{
	if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsTerpstraConfigurationDataReceptionMessage(midiMessage))
	{
		setConnectivity(false);

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
                 fixedSize="0" initialWidth="1024" initialHeight="48">
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

