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
    : lookAndFeel(static_cast<LumatoneEditorLookAndFeel&>(TerpstraSysExApplication::getApp().getLookAndFeel())),errorVisualizer(TerpstraSysExApplication::getApp().getLookAndFeel())
{
    //[Constructor_pre] You can add your own custom stuff here..
	setName("MidiEditArea");

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
	//lookAndFeel.setupComboBox(*cbMidiInput.get());
	//lookAndFeel.setupComboBox(*cbMidiOutput.get());
	cbMidiInput->setVisible(false);
	cbMidiOutput->setVisible(false);

	lblConnectionState->setFont(LumatoneEditorFonts::UniviaProBold());
	lblConnectionState->setColour(Label::ColourIds::textColourId, connectedColours[isConnected]);

	lblEditMode->setFont(LumatoneEditorFonts::UniviaProBold());
	lblEditMode->setColour(Label::ColourIds::textColourId, lookAndFeel.findColour(LumatoneEditorColourIDs::LabelPink));

	//cbMidiInput->addItemList(TerpstraSysExApplication::getApp().getMidiDriver().getMidiInputList(), 1);
	//cbMidiOutput->addItemList(TerpstraSysExApplication::getApp().getMidiDriver().getMidiOutputList(), 1);

    //[/UserPreSize]

    setSize (1024, 48);


    //[Constructor] You can add your own custom stuff here..
	TerpstraSysExApplication::getApp().getMidiDriver().addListener(this);

	setConnectivity(false);
	errorVisualizer.setErrorLevel(
		*lblConnectionState.get(),
		HajuErrorVisualizer::ErrorLevel::error,
		"Select both a MIDI input and a MIDI output");

	initializeDeviceDetection();
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

    g.fillAll (juce::Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(lookAndFeel.findColour(LumatoneEditorColourIDs::LightBackground));

	// Dark backrgound for title and logomark
	g.setColour(lookAndFeel.findColour(LumatoneEditorColourIDs::DarkBackground));
	g.fillRect(lumatoneLabelBounds);
	g.fillRect(connectivityArea);

	// Rounded rect for device boxes
	if (!isConnected)
	{
		g.setColour(lookAndFeel.findColour(LumatoneEditorColourIDs::LightBackground));
		g.fillRoundedRectangle(ioBounds, round(getHeight() * controlBoundsCornerRadius));
	}

	g.setColour(connectedColours[isConnected]);
	drawPathToFillBounds(g, logomarkPath, logomarkBounds);
    //[/UserPaint]
}

void MidiEditArea::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	float w = getWidth();
	float h = getHeight();
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..

	lumatoneLabelBounds = getBounds().withRight(round(w * lumatoneLabelAreaWidth));
	resizeLabelWithWidth(lumatoneLabel.get(), lumatoneLabelBounds.proportionOfWidth(lumatoneLabelWidthInArea));
	lumatoneLabel->setCentrePosition(lumatoneLabelBounds.getCentre());

	// Also used to position logomark
	ioBounds.setBounds(
		round(w * controlBoundsX), round(h* controlBoundsY),
		round(w * controlBoundsWidth), round(h* controlBoundsHeight)
	);

	if (isConnected)
	{
		resizeLabelWithHeight(lblEditMode.get(), round(h* editModeHeight));
		lblEditMode->setTopLeftPosition(
			lumatoneLabelBounds.getRight() + round(w * editModeX),
			round((h - lblEditMode->getHeight()) * 0.5f)
		);

		liveEditorBtn->setSize(round(w * liveEditButtonWidth), round(h* editModeButtonHeight));
		liveEditorBtn->setTopLeftPosition(
			lumatoneLabelBounds.getRight() + round(w * editModeButtonX),
			round((h - liveEditorBtn->getHeight()) * 0.5f)
		);

		offlineEditorBtn->setBounds(
			liveEditorBtn->getRight(), liveEditorBtn->getY(), round(w * offlineEditButtonWidth), liveEditorBtn->getHeight()
		);

		connectivityArea = getBounds().toFloat().withLeft(round(w * connectedAreaX));

		resizeLabelWithHeight(lblConnectionState.get(), round(h * connectivityHeight));
		lblConnectionState->setTopLeftPosition(
			round(w * connectedX),
			round((h - lblConnectionState->getHeight()) * 0.5f)
		);
	}
	else
	{
		connectivityArea = getBounds().toFloat().withLeft(round(w * disconnectedAreaX));

		//lblConnectionState->setSize(ioBounds.getWidth() - ioBoundsLblMargin, lblHeight);
		//lblConnectionState->setTopLeftPosition(
		//	ioBounds.getX() + ioBoundsLblMargin,
		//	round((float)ioBounds.getY() + (ioBounds.getHeight() - lblConnectionState->getHeight()) * 0.5f)
		//);

		int lblMarginX = round(ioBounds.getWidth() * errorVizualizerControlBoundsX);
		int lblMarginY = round((ioBounds.getHeight() - h * connectivityHeight) * 0.5);
		lblConnectionState->setBounds(ioBounds.reduced(lblMarginX, lblMarginY).toNearestInt());

		//cbMidiInput->setBounds(ioBounds.getProportion(Rectangle<float>(
		//	{ midiInputControlBoundsX, midiDeviceControlBoundsHeight / 4.0f, midiDeviceControlBoundsWidth, midiDeviceControlBoundsHeight }
		//)).toNearestInt());

		//cbMidiOutput->setBounds(cbMidiInput->getBounds().withX(
		//	ioBounds.getX() + ioBounds.proportionOfWidth(midiOutputControlBoundsX))
		//);

		pleaseConnectLabel->setTopLeftPosition(round(w * pleaseConnectX), round(h * pleaseConnectY));
		pleaseConnectLabel->setSize(connectivityArea.getX() - pleaseConnectLabel->getX(), round(h * pleaseConnectHeight));

		offlineMsgLabel->setTopLeftPosition(round(w * connectionDirectionsX), round(h * connectionDirectionsY));
		offlineMsgLabel->setSize(connectivityArea.getX() - pleaseConnectLabel->getX(), round(h * connectionDirectionsHeight));
		offlineMsgLabel->setFont(offlineMsgLabel->getFont().withHeight(offlineMsgLabel->getHeight()));
	}

	int logomarkSize = round(h * logomarkHeight);
	logomarkBounds.setSize(logomarkSize, logomarkSize);
	logomarkBounds.setCentre(ioBounds.getRight() + roundToInt((getWidth() - ioBounds.getRight()) * 0.5f), round(h* 0.5f));
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
			TerpstraSysExApplication::getApp().getMidiDriver().setMidiInput(cbMidiInput->getSelectedItemIndex());

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

void MidiEditArea::setConnectivity(bool isConnectedIn)
{
	//cbMidiInput->setVisible(!isConnectedIn);
	//cbMidiOutput->setVisible(!isConnectedIn);
	pleaseConnectLabel->setVisible(!isConnectedIn);
	offlineMsgLabel->setVisible(!isConnectedIn);

	lblConnectionState->setText(connectedText[isConnectedIn], dontSendNotification);
	lblConnectionState->setColour(Label::ColourIds::textColourId, connectedColours[isConnectedIn]);

	lblEditMode->setVisible(isConnectedIn);
	liveEditorBtn->setVisible(isConnectedIn);
	offlineEditorBtn->setVisible(isConnectedIn);

	isConnected = isConnectedIn;

	resized();
	repaint();
}

void MidiEditArea::initializeDeviceDetection()
{
	deviceDetectInProgress = true;
	deviceConnectionMode = MidiEditArea::DetectConnectionMode::lookingForDevice;

	// Refresh available devices
	TerpstraMidiDriver& midiDriver = TerpstraSysExApplication::getApp().getMidiDriver();
	midiDriver.refreshDeviceLists();

	StringArray availableInputs = midiDriver.getMidiInputList();
	StringArray availableOutputs = midiDriver.getMidiOutputList();

	devicePairIndex = 0;

	// Only resume detection if there was a change
	if (availableInputs != inputDeviceCache || availableOutputs != outputDeviceCache)
	{
		inputDeviceCache = availableInputs;
		outputDeviceCache = availableOutputs;

		cbMidiInput->clear(NotificationType::dontSendNotification);
		cbMidiInput->addItemList(availableInputs, 1);

		cbMidiOutput->clear(NotificationType::dontSendNotification);
		cbMidiOutput->addItemList(availableOutputs, 1);

		detectedDevicePairs.clear();

		// Iterate through lists and find devices with matching names
		for (int inputIndex = 0; inputIndex < availableInputs.size(); inputIndex++)
		{
			for (int outputIndex = 0; outputIndex < availableOutputs.size(); outputIndex++)
			{
				// TODO: Improve
				// Not robust if the same device input and output have slightly different names
				if (availableOutputs[outputIndex] == availableInputs[inputIndex])
				{
					detectedDevicePairs.add({ inputIndex, outputIndex });
				}
			}
		}

		tryDevicePairAndIncrement(devicePairIndex);
	}
	else
	{
		deviceDetectInProgress = false;
		//DBG("No new devices found");
		startTimer(deviceChangeTimeoutMs);
	}
}

void MidiEditArea::tryToConnectToDevices(int inputDeviceIndex, int outputDeviceIndex)
{
	TerpstraMidiDriver& midiDriver = TerpstraSysExApplication::getApp().getMidiDriver();

	jassert(inputDeviceIndex >= 0 && inputDeviceIndex < midiDriver.getMidiInputList().size());
	jassert(outputDeviceIndex >= 0 && outputDeviceIndex < midiDriver.getMidiOutputList().size());
	
	midiDriver.setMidiInput(inputDeviceIndex);
	midiDriver.setMidiOutput(outputDeviceIndex);

	//DBG("Attempting to connect to input " + inputDeviceCache[inputDeviceIndex] + " and output " + outputDeviceCache[outputDeviceIndex]);

	midiDriver.sendSerialIdentityRequest();
	startTimer(waitForResponseTimeoutMs);
}

void MidiEditArea::tryDevicePairAndIncrement(int& devicePairIndexIn)
{
	tryToConnectToDevices(detectedDevicePairs[devicePairIndexIn].first, detectedDevicePairs[devicePairIndexIn].second);
	devicePairIndexIn++;
}

void MidiEditArea::intializeConnectionLossDetection()
{
	deviceConnectionMode = MidiEditArea::DetectConnectionMode::waitingForConnectionLoss;

	// TODO
}

void MidiEditArea::onOpenConnectionToDevice()
{
	jassert(cbMidiInput->getSelectedItemIndex() >= 0 && cbMidiOutput->getSelectedItemIndex() >= 0 && liveEditorBtn->getToggleState());

	lblConnectionState->setText("Connecting", NotificationType::dontSendNotification);
	errorVisualizer.setErrorLevel(
		*lblConnectionState.get(),
		HajuErrorVisualizer::ErrorLevel::noError,
		"Connecting");

	// Send current configuration to device, if desired
	auto retc = AlertWindow::showOkCancelBox(
		AlertWindow::AlertIconType::QuestionIcon,
		"Establishing connection to controller",
		"Do you want to send the current configuration to the controller?");

	if (retc)
		TerpstraSysExApplication::getApp().sendCurrentConfigurationToDevice();
	
	// else if (!waitingForConnectionResponse)
		// start request for finding lumatone connection / serial identification key

	setConnectivity(true);
}

void MidiEditArea::midiMessageReceived(const MidiMessage& midiMessage)
{
	if (midiMessage.getSysExDataSize() < 6)
	{
		errorVisualizer.setErrorLevel(
			*lblConnectionState.get(),
			HajuErrorVisualizer::ErrorLevel::error,
			"Message too short");
		
		return;
	}

	bool isConfigurationReception = TerpstraSysExApplication::getApp().getMidiDriver().messageIsTerpstraConfigurationDataReceptionMessage(midiMessage);
	bool isSerialIdentityReception = TerpstraSysExApplication::getApp().getMidiDriver().messageIsGetSerialIdentityMessage(midiMessage);
	
	if (isConfigurationReception || isSerialIdentityReception)
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

		// TODO: revise - is the the proper return code / condition?
		if (answerState == TerpstraMidiDriver::TerpstraMIDIAnswerReturnCode::ACK)
		{
			if (isConfigurationReception || isSerialIdentityReception || !isConnected)
			{
				onOpenConnectionToDevice();
			}
		}
	}
}

void MidiEditArea::generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel)
{
	lblConnectionState->setText(textMessage, NotificationType::dontSendNotification);
	errorVisualizer.setErrorLevel(*lblConnectionState.get(), errorLevel, textMessage);
}

void MidiEditArea::timerCallback()
{
	// Under valid conditions this will call functions that start the timer again.
	stopTimer();

	if (deviceConnectionMode == MidiEditArea::DetectConnectionMode::lookingForDevice)
	{
		if (isConnected)
		{
			deviceDetectInProgress = false;

			if (checkConnectionOnInactivity)
			{
				intializeConnectionLossDetection(); // TODO
			}
		}
		else if (deviceDetectInProgress)
		{
			// Successful
			if (isConnected)
			{
				intializeConnectionLossDetection();
			}

			else if (detectDevicesIfDisconnected)
			{
				auto lastPair = detectedDevicePairs[devicePairIndex - 1];
				//DBG("No response from pair of input " + inputDeviceCache[lastPair.first] + " and output " + outputDeviceCache[lastPair.second]);

				// Still have device pairs to test
				if (devicePairIndex >= 0 && devicePairIndex < detectedDevicePairs.size())
				{
					tryDevicePairAndIncrement(devicePairIndex);
				}

				// Tried all devices, set timeout
				else
				{
					//DBG("Detect device timeout.");
					deviceDetectInProgress = false;
					startTimer(deviceChangeTimeoutMs);
				}
			}
		}
		// Start detection
		else if (detectDevicesIfDisconnected)
		{
			initializeDeviceDetection();
		}

		if (!detectDevicesIfDisconnected)
		{
			deviceDetectInProgress = false;
		}
			
	}
	else if (deviceConnectionMode == MidiEditArea::DetectConnectionMode::waitingForConnectionLoss)
	{
		// TODO
		// Maybe look at the last message time stamp and only check if there hasn't been activity
		// Then stop if there is
	}
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiEditArea" componentName=""
                 parentClasses="public Component, public TerpstraMidiDriver::Listener, public juce::Button::Listener, public Timer"
                 constructorParams="" variableInitialisers="lookAndFeel(static_cast&lt;LumatoneEditorLookAndFeel&amp;&gt;(TerpstraSysExApplication::getApp().getLookAndFeel())),errorVisualizer(TerpstraSysExApplication::getApp().getLookAndFeel())"
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

