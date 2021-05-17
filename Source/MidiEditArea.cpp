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
LumatoneController::sysExSendingMode editModeTabIndexToMidiSysExSendingMode(int tabIndex) { return static_cast<LumatoneController::sysExSendingMode>(tabIndex); }

//[/MiscUserDefs]

//==============================================================================
MidiEditArea::MidiEditArea (LumatoneEditorLookAndFeel& lookAndFeelIn)
    : lookAndFeel(lookAndFeelIn)
{
    //[Constructor_pre] You can add your own custom stuff here..
	setName("MidiEditArea");

	lumatoneLabel.reset(new Label("LumatoneLabel", "lumatone"));
	lumatoneLabel->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::UniviaProBold));
	lumatoneLabel->setColour(Label::ColourIds::textColourId, lookAndFeel.findColour(LumatoneEditorColourIDs::LabelPink));
	lumatoneLabel->setJustificationType(Justification::centred);
	addAndMakeVisible(lumatoneLabel.get());

	liveEditorBtn.reset(new TextButton("LiveEditorButton"));
	lookAndFeel.setupRadioTextButton(*liveEditorBtn, 10, true);
	liveEditorBtn->setButtonText(translate("LiveEditor"));
	liveEditorBtn->setConnectedEdges(Button::ConnectedOnRight);
	liveEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontOverride, LumatoneEditorFont::UniviaProBold);
	liveEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, editModeFontScalar);
	addChildComponent(liveEditorBtn.get());
	liveEditorBtn->addListener(this);

	offlineEditorBtn.reset(new TextButton("OfflineEditorButton"));
	lookAndFeel.setupRadioTextButton(*offlineEditorBtn, 10, false);
	offlineEditorBtn->setButtonText(translate("OfflineEditor"));
	offlineEditorBtn->setConnectedEdges(Button::ConnectedOnLeft);
	offlineEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontOverride, LumatoneEditorFont::UniviaProBold);
	offlineEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, editModeFontScalar);
	addChildComponent(offlineEditorBtn.get());
	offlineEditorBtn->addListener(this);

	pleaseConnectLabel.reset(new Label("PleaseConnectLabel", translate("PleaseConnect")));
	pleaseConnectLabel->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::UniviaProBold));
	pleaseConnectLabel->setColour(Label::ColourIds::textColourId, lookAndFeel.findColour(LumatoneEditorColourIDs::ActiveText));
	addAndMakeVisible(*pleaseConnectLabel);

	offlineMsgLabel.reset(new Label("DirectionsLabel", translate("OfflineMessage")));
	offlineMsgLabel->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::FranklinGothic));
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

    btnAutoConnect.reset (new juce::TextButton ("btnAutoConnect"));
    addAndMakeVisible (btnAutoConnect.get());
    btnAutoConnect->setTooltip (TRANS("Toggle between automatic or manual connection to Lumatone"));
    btnAutoConnect->setButtonText (TRANS("auto"));
    btnAutoConnect->addListener (this);

    btnAutoConnect->setBounds (184, 8, 39, 24);


    //[UserPreSize]
	cbMidiInput->setVisible(false);
	cbMidiOutput->setVisible(false);

	lblConnectionState->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::UniviaProBold));
	lblConnectionState->setColour(Label::ColourIds::textColourId, connectedColours[isConnected]);
    lblConnectionState->getProperties().set(LumatoneEditorStyleIDs::labelMaximumLineCount, 2);

	lblEditMode->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::UniviaProBold));
	lblEditMode->setColour(Label::ColourIds::textColourId, lookAndFeel.findColour(LumatoneEditorColourIDs::LabelPink));

	offlineEditorBtn->setClickingTogglesState(true);
	offlineEditorBtn->setRadioGroupId(10, dontSendNotification);
	liveEditorBtn->setClickingTogglesState(true);
	liveEditorBtn->setRadioGroupId(10, dontSendNotification);
	liveEditorBtn->setToggleState(true, dontSendNotification);
    liveEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, 0.6f);
    offlineEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, 0.6f);

	btnAutoConnect->setClickingTogglesState(true);

	ioAreaFlexBox.alignContent = FlexBox::AlignContent::center;
	ioAreaFlexBox.alignItems = FlexBox::AlignItems::center;
	ioAreaFlexBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
    //[/UserPreSize]

    setSize (1024, 48);


    //[Constructor] You can add your own custom stuff here..
	TerpstraSysExApplication::getApp().getLumatoneController().addStatusListener(this);
	auto inputs = TerpstraSysExApplication::getApp().getLumatoneController().getMidiInputList();
	auto outputs = TerpstraSysExApplication::getApp().getLumatoneController().getMidiOutputList();
	refreshInputMenuAndSetSelected(0, dontSendNotification);
	refreshOutputMenuAndSetSelected(0, dontSendNotification);
	setConnectivity(false);

    btnAutoConnect->setToggleState(TerpstraSysExApplication::getApp().getLumatoneController().isDetectingLumatone(), sendNotificationSync);
	
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
    btnAutoConnect = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	//deviceMonitor.stopThread(100);
	TerpstraSysExApplication::getApp().getLumatoneController().removeStatusListener(this);
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
		round(w * controlBoundsX), round(h * controlBoundsY),
		round(w * controlBoundsWidth), round(h * controlBoundsHeight)
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
			lblEditMode->getRight(),
			round((h - liveEditorBtn->getHeight()) * 0.5f)
		);

		offlineEditorBtn->setBounds(
			liveEditorBtn->getRight(), liveEditorBtn->getY(), round(w * offlineEditButtonWidth), liveEditorBtn->getHeight()
		);


		connectivityArea = getBounds().toFloat().withLeft(round(w * connectedAreaX));

        lblConnectionState->setJustificationType (juce::Justification::centredLeft);
		resizeLabelWithHeight(lblConnectionState.get(), round(h * connectivityHeight));
		lblConnectionState->setTopLeftPosition(
			round(w * connectedX),
			round((h - lblConnectionState->getHeight()) * 0.5f)
		);
	}
	else
	{
		int controlHeight = roundToInt(ioBounds.getHeight() * midiDeviceControlBoundsHeight);

		connectivityArea = getBounds().toFloat().withLeft(round(w * disconnectedAreaX));

		int lblMarginX = round(ioBounds.getWidth() * controlBoundsMarginScalar);
		int lblMarginY = round((ioBounds.getHeight() - h * connectivityHeight) * 0.5f);

		ioAreaFlexBox.items.clear();
		ioAreaFlexBox.items.add(FlexItem(*btnAutoConnect).withFlex(0).withWidth(controlHeight * 1.6f).withHeight(controlHeight));

		if (btnAutoConnect->getToggleState())
		{
			lblConnectionState->setJustificationType(juce::Justification::centredLeft);
		}
		else
		{
			int deviceBoxWidth = roundToInt(ioBounds.getWidth() * midiDeviceControlBoundsWidth);
			ioAreaFlexBox.items.add(FlexItem(*cbMidiInput).withFlex(0).withWidth(deviceBoxWidth).withHeight(controlHeight));
			ioAreaFlexBox.items.add(FlexItem(*cbMidiOutput).withFlex(0).withWidth(deviceBoxWidth).withHeight(controlHeight));

			lblConnectionState->setJustificationType(juce::Justification::centredRight);
		}

		// Not sure why AlignSelf is necessary...
		ioAreaFlexBox.items.add(FlexItem(*lblConnectionState).withFlex(1).withHeight(h * connectivityHeight).withAlignSelf(FlexItem::AlignSelf::center));

		float itemMargin = lblMarginX * 0.5f;
		for (int i = 0; i < ioAreaFlexBox.items.size(); i++)
		{
			ioAreaFlexBox.items.getReference(i).margin = FlexItem::Margin(0, itemMargin, 0, 0);
		}

		ioAreaFlexBox.performLayout(
			ioBounds.reduced(lblMarginX, lblMarginY)
		);

		pleaseConnectLabel->setTopLeftPosition(round(w * pleaseConnectX), round(h * pleaseConnectY));
		resizeLabelWithHeight(pleaseConnectLabel.get(), round(h * pleaseConnectHeight));

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
			TerpstraSysExApplication::getApp().getLumatoneController().setMidiInput(cbMidiInput->getSelectedItemIndex());

		if (cbMidiInput->getSelectedItemIndex() < 0 || cbMidiOutput->getSelectedItemIndex() < 0)
		{
			setConnectivity(false);
			//errorVisualizer.setErrorLevel(
			//	*lblConnectionState.get(),
			//	HajuErrorVisualizer::ErrorLevel::error,
			//	translate("Select both a MIDI input and output"));
		}
		else
		{
			attemptDeviceConnection();
		}
        //[/UserComboBoxCode_cbMidiInput]
    }
    else if (comboBoxThatHasChanged == cbMidiOutput.get())
    {
        //[UserComboBoxCode_cbMidiOutput] -- add your combo box handling code here..
		if (cbMidiOutput->getSelectedItemIndex() >= 0)
			TerpstraSysExApplication::getApp().getLumatoneController().setMidiOutput(cbMidiOutput->getSelectedItemIndex());

		if (cbMidiInput->getSelectedItemIndex() < 0 || cbMidiOutput->getSelectedItemIndex() < 0)
		{
			setConnectivity(false);
			//errorVisualizer.setErrorLevel(
			//	*lblConnectionState.get(),
			//	HajuErrorVisualizer::ErrorLevel::error,
			//	translate("Select both a MIDI input and output"));
		}
		else
		{
			attemptDeviceConnection();
		}
        //[/UserComboBoxCode_cbMidiOutput]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void MidiEditArea::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]

    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnAutoConnect.get())
    {
        //[UserButtonCode_btnAutoConnect] -- add your button handler code here..
		cbMidiInput->setVisible(!btnAutoConnect->getToggleState());
		cbMidiOutput->setVisible(!btnAutoConnect->getToggleState());

        if (btnAutoConnect->getToggleState())
		{
			TerpstraSysExApplication::getApp().getLumatoneController().detectAndConnectToLumatone();
			lblConnectionState->setText(translate("Searching for Lumatone..."), dontSendNotification);
			//errorVisualizer.setErrorLevel(
			//	*lblConnectionState.get(),
			//	HajuErrorVisualizer::ErrorLevel::error,
			//	translate("Waiting for response from connected devices..."));
		}
		else
		{
			TerpstraSysExApplication::getApp().getLumatoneController().stopAutoConnection();
			lblConnectionState->setText(translate("Disconnected"), dontSendNotification);
			startTimer(deviceRefreshTimeoutMs);
		}

		resized();
        //[/UserButtonCode_btnAutoConnect]
    }

    //[UserbuttonClicked_Post]
	else if (buttonThatWasClicked == liveEditorBtn.get())
	{
		auto sysExSendingMode = editModeTabIndexToMidiSysExSendingMode((int)!liveEditorBtn->getToggleState());

		TerpstraSysExApplication::getApp().getLumatoneController().setSysExSendingMode(sysExSendingMode);

		switch (sysExSendingMode)
		{
		case LumatoneController::sysExSendingMode::liveEditor:
			onOpenConnectionToDevice();
			break;

		case LumatoneController::sysExSendingMode::offlineEditor:
			lblConnectionState->setText(translate("Offline"), NotificationType::dontSendNotification);
			//errorVisualizer.setErrorLevel(
			//	*lblConnectionState.get(),
			//	HajuErrorVisualizer::ErrorLevel::noError,
			//	"Offline");
			break;

		default:
			jassertfalse;
			break;
		}
	}
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MidiEditArea::lookAndFeelChanged()
{
	connectedColours.clear();
	connectedColours.add(getLookAndFeel().findColour(LumatoneEditorColourIDs::DisconnectedRed));
	connectedColours.add(getLookAndFeel().findColour(LumatoneEditorColourIDs::ConnectedGreen));
}

void MidiEditArea::setConnectivity(bool isConnectedIn)
{
	bool isNotConnected = !isConnectedIn;

	btnAutoConnect->setVisible(isNotConnected);
	cbMidiInput->setVisible(isNotConnected && !btnAutoConnect->getToggleState());
	cbMidiOutput->setVisible(isNotConnected && !btnAutoConnect->getToggleState());
	pleaseConnectLabel->setVisible(isNotConnected);
	offlineMsgLabel->setVisible(isNotConnected);

	lblEditMode->setVisible(isConnectedIn);
	liveEditorBtn->setVisible(isConnectedIn);
	offlineEditorBtn->setVisible(isConnectedIn);

	isConnected = isConnectedIn;
	btnAutoConnect->setEnabled(!isConnected);

	if (isConnected)
	{
		stopTimer();

		if (liveEditorBtn->getToggleState())
			lblConnectionState->setText(translate("Connected"), dontSendNotification);
		else
			lblConnectionState->setText(translate("Offline"), dontSendNotification);
	}
	else
	{	
		if (btnAutoConnect->getToggleState())
			lblConnectionState->setText(translate("Searching for Lumatone..."), dontSendNotification);
		else
		{
			lblConnectionState->setText(translate("Disconnected"), dontSendNotification);
			startTimer(deviceRefreshTimeoutMs);
		}
	}

	lblConnectionState->setColour(Label::ColourIds::textColourId, connectedColours[isConnectedIn]);

	resized();
	repaint();
}

// Called when DeviceActivityMonitor detects a change in devices

//void MidiEditArea::connectionChanged(bool hasConnection)
//{
//	if (hasConnection && btnAutoConnect->getToggleState() == false)
//	{
//		DBG("MIDIAREA SETTING DEVICES");
//		TerpstraSysExApplication::getApp().getLumatoneController().setMidiInput(cbMidiInput->getSelectedId() - 1);
//		TerpstraSysExApplication::getApp().getLumatoneController().setMidiOutput(cbMidiOutput->getSelectedId() - 1);
//	}
//}

void MidiEditArea::connectionEstablished(int inputDevice, int outputDevice)
{
    if (inputDevice >= 0 && outputDevice >= 0)
    {
		refreshInputMenuAndSetSelected(inputDevice + 1, dontSendNotification);
		refreshOutputMenuAndSetSelected(outputDevice + 1, dontSendNotification);
        setConnectivity(true);

		onOpenConnectionToDevice();
    }
    else
    {
		jassertfalse;
    }

	//deviceMonitor.intializeConnectionLossDetection();
}

void MidiEditArea::connectionLost()
{
	if (!btnAutoConnect->getToggleState())
		startTimer(deviceRefreshTimeoutMs);

	if (isWaitingForConnectionTest)
	{
		lblConnectionState->setText(translate("No answer"), NotificationType::dontSendNotification);
		//errorVisualizer.setErrorLevel(
		//    *lblConnectionState.get(),
		//    HajuErrorVisualizer::ErrorLevel::error,
		//    "No answer");
	}
	else
	{
		setConnectivity(false);
	}
}

void MidiEditArea::attemptDeviceConnection()
{
    jassert(!isConnected);

    lblConnectionState->setText("Connecting...", NotificationType::dontSendNotification);
    //errorVisualizer.setErrorLevel(
    //    *lblConnectionState.get(),
    //    HajuErrorVisualizer::ErrorLevel::noError,
    //    "Connecting...");

	if (TerpstraSysExApplication::getApp().getLumatoneController().isConnected())
	{
		onOpenConnectionToDevice();
	}
	else
	{
		//isWaitingForConnectionTest = true;
		//TerpstraSysExApplication::getApp().getLumatoneController().testCurrentDeviceConnection();
		//jassert(isWaitingForConnectionTest); // Triggered if a test is requested before opening any devices
	}
}

void MidiEditArea::onOpenConnectionToDevice()
{
	jassert(cbMidiInput->getSelectedItemIndex() >= 0 && cbMidiOutput->getSelectedItemIndex() >= 0);

	jassert(alert == nullptr);
	// This can get spammed, and needs a real solution, but for now this will prevent it in releases - Vito
	if (alert == nullptr)
	{
		// Get firmware version so we can use the correct commands
		TerpstraSysExApplication::getApp().getLumatoneController().sendGetFirmwareRevisionRequest();

		alert.reset(new AlertWindow("Connection established!", translate("Do you want to send the current setup to your Lumatone?"), AlertWindow::AlertIconType::QuestionIcon, getParentComponent()));
		alert->addButton("Send Editor layout", 1);
		alert->addButton("Keep Editing Offline", 0);
		alert->addButton("Import From Lumatone", 2);
		alert->setLookAndFeel(&lookAndFeel);

		auto retc = alert->runModalLoop();

		if (retc == 1)
		{
			TerpstraSysExApplication::getApp().sendCurrentConfigurationToDevice();
			liveEditorBtn->setToggleState(true, dontSendNotification);
			lblConnectionState->setText("Connected", NotificationType::dontSendNotification);
		}
		else if (retc == 2)
		{
			TerpstraSysExApplication::getApp().requestConfigurationFromDevice();
			liveEditorBtn->setToggleState(true, NotificationType::dontSendNotification);
			lblConnectionState->setText("Connected", NotificationType::dontSendNotification);
		}
		else
		{
			offlineEditorBtn->setToggleState(true, NotificationType::sendNotification);
			lblConnectionState->setText("Offline", NotificationType::dontSendNotification);
		}

		alert = nullptr;
	}
}

void MidiEditArea::refreshInputMenuAndSetSelected(int inputDeviceIndex, juce::NotificationType notificationType)
{
	cbMidiInput->clear(NotificationType::dontSendNotification);
	int i = 1;
	for (auto device : TerpstraSysExApplication::getApp().getLumatoneController().getMidiInputList())
		cbMidiInput->addItem(device.name, i++);

	if (inputDeviceIndex > 0)
		cbMidiInput->setSelectedId(inputDeviceIndex, notificationType);
}

void MidiEditArea::refreshOutputMenuAndSetSelected(int outputDeviceIndex, juce::NotificationType notificationType)
{
	cbMidiOutput->clear(NotificationType::dontSendNotification);
	int i = 1;
	for (auto device : TerpstraSysExApplication::getApp().getLumatoneController().getMidiOutputList())
		cbMidiOutput->addItem(device.name, i++);

	if (outputDeviceIndex > 0)
		cbMidiOutput->setSelectedId(outputDeviceIndex, notificationType);
}

void MidiEditArea::timerCallback()
{
	if (isConnected)
	{
		stopTimer();
	}
	else
	{
		TerpstraSysExApplication::getApp().getLumatoneController().refreshAvailableMidiDevices();

		refreshInputMenuAndSetSelected(
			TerpstraSysExApplication::getApp().getLumatoneController().getMidiInputIndex() + 1,
			juce::NotificationType::dontSendNotification
		);

		refreshOutputMenuAndSetSelected(
			TerpstraSysExApplication::getApp().getLumatoneController().getMidiOutputIndex() + 1,
			juce::NotificationType::dontSendNotification
		);
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
                 parentClasses="public Component, public TerpstraMidiDriver::Listener, public juce::ChangeListener, public Timer"
                 constructorParams="LumatoneEditorLookAndFeel&amp; lookAndFeelIn, DeviceActivityMonitor&amp; deviceMonitorIn"
                 variableInitialisers="lookAndFeel(lookAndFeelIn),errorVisualizer(lookAndFeelIn), deviceMonitor(deviceMonitorIn)"
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
  <TEXTBUTTON name="btnAutoConnect" id="381f46e171b5df06" memberName="btnAutoConnect"
              virtualName="" explicitFocusOrder="0" pos="184 8 39 24" tooltip="Toggle between automatic or manual connection to Lumatone"
              buttonText="auto" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

