/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "Main.h"
//[/Headers]

#include "VelocityCurveDlg.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VelocityCurveDlg::VelocityCurveDlg (TerpstraMidiDriver::VelocityCurveType typeValue)
    : freeDrawingStrategy(beamTableFrame, velocityBeamTable),
      linearDrawingStrategy(beamTableFrame, velocityBeamTable),
      quadraticDrawingStrategy(beamTableFrame, velocityBeamTable)
{
    //[Constructor_pre] You can add your own custom stuff here..
	velocityCurveType = typeValue;
	currentCurveEditStrategy = nullptr;
    //[/Constructor_pre]

    lblDescription.reset (new Label ("lblDescription",
                                     TRANS("Click with the mouse in the graphics to draw the velocity curve.")));
    addAndMakeVisible (lblDescription.get());
    lblDescription->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    lblDescription->setJustificationType (Justification::centredLeft);
    lblDescription->setEditable (false, false, false);
    lblDescription->setColour (TextEditor::textColourId, Colours::black);
    lblDescription->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lblDescription->setBounds (32, 104, 608, 32);

    buttonSendAll.reset (new TextButton ("buttonSendAll"));
    addAndMakeVisible (buttonSendAll.get());
    buttonSendAll->setTooltip (TRANS("Send whole velocity curve map to controller and save it there."));
    buttonSendAll->setButtonText (TRANS("Send & Save All"));
    buttonSendAll->addListener (this);

    buttonSendAll->setBounds (312, 392, 144, 24);

    buttonDiscard.reset (new TextButton ("buttonDiscard"));
    addAndMakeVisible (buttonDiscard.get());
    buttonDiscard->setTooltip (TRANS("Discard velocity curve edits on controller."));
    buttonDiscard->setButtonText (TRANS("Discard Edits"));
    buttonDiscard->addListener (this);

    buttonDiscard->setBounds (464, 368, 144, 24);

    buttonSaveEdits.reset (new TextButton ("buttonSaveEdits"));
    addAndMakeVisible (buttonSaveEdits.get());
    buttonSaveEdits->setTooltip (TRANS("Save velocity curve edits that have been sent on controller"));
    buttonSaveEdits->setButtonText (TRANS("Save Edits"));
    buttonSaveEdits->addListener (this);

    buttonSaveEdits->setBounds (160, 392, 144, 24);

    cbEditMode.reset (new ComboBox ("cbEditMode"));
    addAndMakeVisible (cbEditMode.get());
    cbEditMode->setEditableText (false);
    cbEditMode->setJustificationType (Justification::centredLeft);
    cbEditMode->setTextWhenNothingSelected (String());
    cbEditMode->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbEditMode->addItem (TRANS("Free drawing"), 1);
    cbEditMode->addItem (TRANS("Linear"), 2);
    cbEditMode->addItem (TRANS("Quadratic"), 3);
    cbEditMode->addListener (this);

    cbEditMode->setBounds (144, 72, 296, 24);

    labelEditMode.reset (new Label ("labelEditMode",
                                    TRANS("Edit Function:")));
    addAndMakeVisible (labelEditMode.get());
    labelEditMode->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelEditMode->setJustificationType (Justification::centredLeft);
    labelEditMode->setEditable (false, false, false);
    labelEditMode->setColour (TextEditor::textColourId, Colours::black);
    labelEditMode->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelEditMode->setBounds (32, 72, 103, 24);

    cbPreset.reset (new ComboBox ("cbPreset"));
    addAndMakeVisible (cbPreset.get());
    cbPreset->setEditableText (true);
    cbPreset->setJustificationType (Justification::centredLeft);
    cbPreset->setTextWhenNothingSelected (String());
    cbPreset->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbPreset->addItem (TRANS("One to one"), 1);
    cbPreset->addListener (this);

    cbPreset->setBounds (144, 32, 296, 24);

    labelPresets.reset (new Label ("labelPresets",
                                   TRANS("Presets:")));
    addAndMakeVisible (labelPresets.get());
    labelPresets->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelPresets->setJustificationType (Justification::centredLeft);
    labelPresets->setEditable (false, false, false);
    labelPresets->setColour (TextEditor::textColourId, Colours::black);
    labelPresets->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelPresets->setBounds (32, 32, 107, 24);

    labelCurrentBeamValue.reset (new Label ("labelCurrentBeamValue",
                                            TRANS("127")));
    addAndMakeVisible (labelCurrentBeamValue.get());
    labelCurrentBeamValue->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelCurrentBeamValue->setJustificationType (Justification::centredLeft);
    labelCurrentBeamValue->setEditable (false, false, false);
    labelCurrentBeamValue->setColour (TextEditor::textColourId, Colours::black);
    labelCurrentBeamValue->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelCurrentBeamValue->setBounds (0, 152, 31, 24);

    buttonReceive.reset (new TextButton ("buttonReceive"));
    addAndMakeVisible (buttonReceive.get());
    buttonReceive->setTooltip (TRANS("Receive the current configurartion from controller"));
    buttonReceive->setButtonText (TRANS("Receive"));
    buttonReceive->addListener (this);

    buttonReceive->setBounds (8, 392, 144, 24);

    buttonCalibrate.reset (new TextButton ("buttonCalibrate"));
    addAndMakeVisible (buttonCalibrate.get());
    buttonCalibrate->setTooltip (TRANS("Calibrate aftertouch"));
    buttonCalibrate->setButtonText (TRANS("Calibrate"));
    buttonCalibrate->addListener (this);

    buttonCalibrate->setBounds (616, 368, 144, 24);

    buttonAfterTouchActive.reset (new ToggleButton ("buttonAfterTouchActive"));
    addAndMakeVisible (buttonAfterTouchActive.get());
    buttonAfterTouchActive->setButtonText (TRANS("Aftertouch active"));
    buttonAfterTouchActive->addListener (this);

    buttonAfterTouchActive->setBounds (144, 0, 144, 24);


    //[UserPreSize]

	for (int x = 0; x < 128; x++)
	{
		velocityBeamTable[x] = new  VelocityCurveBeam();
		addAndMakeVisible(velocityBeamTable[x]);
		velocityBeamTable[x]->addMouseListener(this, true);
	}

	TerpstraSysExApplication::getApp().getMidiDriver().addListener(this);

    //[/UserPreSize]

    setSize (768, 424);


    //[Constructor] You can add your own custom stuff here..
	labelCurrentBeamValue->setVisible(false);

	// Calibrate button: only for aftertouch
	buttonAfterTouchActive->setVisible(velocityCurveType == TerpstraMidiDriver::VelocityCurveType::afterTouch);
	buttonCalibrate->setVisible(velocityCurveType == TerpstraMidiDriver::VelocityCurveType::afterTouch);

	// Set values according to the properties files
	restoreStateFromPropertiesFile(TerpstraSysExApplication::getApp().getPropertiesFile());
    //[/Constructor]
}

VelocityCurveDlg::~VelocityCurveDlg()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	// Save values to properties file
	saveStateToPropertiesFile(TerpstraSysExApplication::getApp().getPropertiesFile());
    //[/Destructor_pre]

    lblDescription = nullptr;
    buttonSendAll = nullptr;
    buttonDiscard = nullptr;
    buttonSaveEdits = nullptr;
    cbEditMode = nullptr;
    labelEditMode = nullptr;
    cbPreset = nullptr;
    labelPresets = nullptr;
    labelCurrentBeamValue = nullptr;
    buttonReceive = nullptr;
    buttonCalibrate = nullptr;
    buttonAfterTouchActive = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	deleteAllChildren();
    //[/Destructor]
}

//==============================================================================
void VelocityCurveDlg::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(findColour(ResizableWindow::backgroundColourId));

	g.setColour(findColour(VelocityCurveBeam::outlineColourId));
	g.strokePath(beamTableFrame, PathStrokeType(1.000f));

	if (currentCurveEditStrategy != nullptr)
	{
		lblDescription->setText(currentCurveEditStrategy->getDescriptionText(), juce::NotificationType::dontSendNotification);
		currentCurveEditStrategy->paint(g, getLookAndFeel());
	}
    //[/UserPaint]
}

void VelocityCurveDlg::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	int w = this->getWidth();
	int h = this->getHeight();
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..

	float graphicsXPadding =  buttonSaveEdits->getX();

	beamTableFrame.clear();
	beamTableFrame.startNewSubPath(graphicsXPadding, graphicsYPadding);
	beamTableFrame.lineTo(graphicsXPadding, h - pushButtonAreaHeight);
	beamTableFrame.lineTo(w - graphicsXPadding, h - pushButtonAreaHeight);
	beamTableFrame.lineTo(w - graphicsXPadding, graphicsYPadding);
	beamTableFrame.closeSubPath();

	if (currentCurveEditStrategy != nullptr)
		currentCurveEditStrategy->resized();

	float velocityBeamWidth = (w - 2 * graphicsXPadding) / 128;
	float velocityGraphicsHeight = h - graphicsYPadding - pushButtonAreaHeight;

	for (int x = 0; x < 128; x++)
	{
		velocityBeamTable[x]->setBounds(
			graphicsXPadding + x*velocityBeamWidth,
			graphicsYPadding,
			velocityBeamWidth,
			velocityGraphicsHeight);
	}

	int buttonYPos = h - pushButtonAreaHeight + (pushButtonAreaHeight - buttonSendAll->getHeight())/2;

	buttonReceive->setBounds(buttonReceive->getX(), buttonYPos,
		buttonReceive->getWidth(), buttonReceive->getHeight());
	buttonSendAll->setBounds(buttonSendAll->getX(), buttonYPos,
		buttonSendAll->getWidth(), buttonSendAll->getHeight());
	buttonDiscard->setBounds(buttonDiscard->getX(), buttonYPos,
		buttonDiscard->getWidth(), buttonDiscard->getHeight());
	buttonSaveEdits->setBounds(buttonSaveEdits->getX(), buttonYPos,
		buttonSaveEdits->getWidth(), buttonSaveEdits->getHeight());
	buttonCalibrate->setBounds(buttonCalibrate->getX(), buttonYPos,
		buttonCalibrate->getWidth(), buttonCalibrate->getHeight());

    //[/UserResized]
}

void VelocityCurveDlg::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonSendAll.get())
    {
        //[UserButtonCode_buttonSendAll] -- add your button handler code here..
		// Send all
		sendVelocityTableToController();

		if (velocityCurveType == TerpstraMidiDriver::VelocityCurveType::afterTouch)
        {
            TerpstraSysExApplication::getApp().getMidiDriver().sendAfterTouchActivation(buttonAfterTouchActive->getToggleState());
        }

		// Save
		TerpstraSysExApplication::getApp().getMidiDriver().saveVelocityConfig(velocityCurveType);
        //[/UserButtonCode_buttonSendAll]
    }
    else if (buttonThatWasClicked == buttonDiscard.get())
    {
        //[UserButtonCode_buttonDiscard] -- add your button handler code here..
		TerpstraSysExApplication::getApp().getMidiDriver().resetVelocityConfig(velocityCurveType);
        //[/UserButtonCode_buttonDiscard]
    }
    else if (buttonThatWasClicked == buttonSaveEdits.get())
    {
        //[UserButtonCode_buttonSaveEdits] -- add your button handler code here..
		TerpstraSysExApplication::getApp().getMidiDriver().saveVelocityConfig(velocityCurveType);
        //[/UserButtonCode_buttonSaveEdits]
    }
    else if (buttonThatWasClicked == buttonReceive.get())
    {
        //[UserButtonCode_buttonReceive] -- add your button handler code here..
        TerpstraSysExApplication::getApp().getMidiDriver().sendVelocityConfigurationRequest(velocityCurveType);

        // ToDo Handle MIDI answer

        //[/UserButtonCode_buttonReceive]
    }
    else if (buttonThatWasClicked == buttonCalibrate.get())
    {
        //[UserButtonCode_buttonCalibrate] -- add your button handler code here..
        TerpstraSysExApplication::getApp().getMidiDriver().sendCalibrateAfterTouch();
        //[/UserButtonCode_buttonCalibrate]
    }
    else if (buttonThatWasClicked == buttonAfterTouchActive.get())
    {
        //[UserButtonCode_buttonAfterTouchActive] -- add your button handler code here..
        TerpstraSysExApplication::getApp().getMidiDriver().sendAfterTouchActivation(buttonAfterTouchActive->getToggleState());
        //[/UserButtonCode_buttonAfterTouchActive]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void VelocityCurveDlg::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cbEditMode.get())
    {
        //[UserComboBoxCode_cbEditMode] -- add your combo box handling code here..
		int editModeIndex = cbEditMode->getSelectedItemIndex();

		switch (editModeIndex)
		{
		case EDITSTRATEGYINDEX::freeDrawing:
			currentCurveEditStrategy = &freeDrawingStrategy;
			break;

		case EDITSTRATEGYINDEX::linearSegments:
			currentCurveEditStrategy = &linearDrawingStrategy;
			break;

		case EDITSTRATEGYINDEX::quadraticCurves:
			currentCurveEditStrategy = &quadraticDrawingStrategy;
			break;

		default:
			currentCurveEditStrategy = nullptr;
			break;
		}

		// Set edit config according to current values of velocity table, of possible
		if (currentCurveEditStrategy != nullptr)
			currentCurveEditStrategy->setEditConfigFromVelocityTable();

		repaint();
        //[/UserComboBoxCode_cbEditMode]
    }
    else if (comboBoxThatHasChanged == cbPreset.get())
    {
        //[UserComboBoxCode_cbPreset] -- add your combo box handling code here..

		int presetIndex = cbPreset->getSelectedItemIndex();

		// Show sub window corresponding to selected edit mode
		switch (presetIndex)
		{
		case 0:
			// Currently only preset is "one to one"
			for (int x = 0; x < 128; x++)
				velocityBeamTable[x]->setValue(x);
			sendVelocityTableToController();
			break;
		}

		if (currentCurveEditStrategy != nullptr)
			currentCurveEditStrategy->setEditConfigFromVelocityTable();

		repaint();

        //[/UserComboBoxCode_cbPreset]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void VelocityCurveDlg::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	String keyName;
    switch(velocityCurveType)
    {
        case TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff:
            keyName = "NoteOnOffVelocityCurveTable";
            break;
        case TerpstraMidiDriver::VelocityCurveType::fader:
            keyName = "FaderVelocityCurveTable";
            break;
        case TerpstraMidiDriver::VelocityCurveType::afterTouch:
            keyName = "AfterTouchCurveTable";
            break;
        default:
            jassert(false);
            break;
    }

	String propertiesString = propertiesFile->getValue(keyName);

	// Format of properties string includes edit strategy. Parse values incl. edit strategy. Free drawing (default) last

	if (linearDrawingStrategy.setEditConfigFromSavedString(propertiesString))
	{
		cbEditMode->setSelectedItemIndex(EDITSTRATEGYINDEX::linearSegments, juce::NotificationType::dontSendNotification);
		currentCurveEditStrategy = &linearDrawingStrategy;
	}
	else if (quadraticDrawingStrategy.setEditConfigFromSavedString(propertiesString))
	{
		cbEditMode->setSelectedItemIndex(EDITSTRATEGYINDEX::quadraticCurves, juce::NotificationType::dontSendNotification);
		currentCurveEditStrategy = &quadraticDrawingStrategy;
	}
	else if (freeDrawingStrategy.setEditConfigFromSavedString(propertiesString))
	{
		cbEditMode->setSelectedItemIndex(EDITSTRATEGYINDEX::freeDrawing, juce::NotificationType::dontSendNotification);
		currentCurveEditStrategy = &freeDrawingStrategy;
	}
	else
	{
		// Initialize velocity lookup table
		for (int x = 0; x < 128; x++)
			velocityBeamTable[x]->setValue(x);

		cbEditMode->setSelectedItemIndex(EDITSTRATEGYINDEX::none, juce::NotificationType::dontSendNotification);
		currentCurveEditStrategy = nullptr;
	}

	if (currentCurveEditStrategy != nullptr)
		currentCurveEditStrategy->setVelocityTableValuesFromEditConfig();

    switch(velocityCurveType)
    {
        case TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff:
            setSize(
                propertiesFile->getIntValue("VelocityCurveWindowWidth", 648),
                propertiesFile->getIntValue("VelocityCurveWindowHeight", 320));
            break;
        case TerpstraMidiDriver::VelocityCurveType::fader:
            setSize(
                propertiesFile->getIntValue("VelocityCurveWindowWidth", 648),
                propertiesFile->getIntValue("VelocityCurveWindowHeight", 320));
            break;
        case TerpstraMidiDriver::VelocityCurveType::afterTouch:
            buttonAfterTouchActive->setToggleState(
                propertiesFile->getBoolValue("AfterTouchActive", false),
                juce::NotificationType::sendNotification);
            setSize(
                propertiesFile->getIntValue("AftertochVelocityCurveWindowWidth", 768),
                propertiesFile->getIntValue("AftertouchVelocityCurveWindowHeight", 320));
            break;
        default:
            jassert(false);
            break;
    }

}

void VelocityCurveDlg::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	String velocityCurveString;

	if (currentCurveEditStrategy != nullptr)
	{
		velocityCurveString = currentCurveEditStrategy->createPropertiesStringForSaving();
	}
	else
	{
		jassertfalse;
		velocityCurveString = String();
	}

	String keyName;
    switch(velocityCurveType)
    {
        case TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff:
            keyName = "NoteOnOffVelocityCurveTable";
            break;
        case TerpstraMidiDriver::VelocityCurveType::fader:
            keyName = "FaderVelocityCurveTable";
            break;
        case TerpstraMidiDriver::VelocityCurveType::afterTouch:
            keyName = "AfterTouchCurveTable";
            break;
        default:
            jassert(false);
            break;
    }

	propertiesFile->setValue(keyName, velocityCurveString);

    switch(velocityCurveType)
    {
        case TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff:
            propertiesFile->setValue("VelocityCurveWindowWidth", getWidth());
            propertiesFile->setValue("VelocityCurveWindowHeight", getHeight());
            break;
        case TerpstraMidiDriver::VelocityCurveType::fader:
            propertiesFile->setValue("VelocityCurveWindowWidth", getWidth());
            propertiesFile->setValue("VelocityCurveWindowHeight", getHeight());
            break;
        case TerpstraMidiDriver::VelocityCurveType::afterTouch:
            propertiesFile->setValue("AfterTouchActive", buttonAfterTouchActive->getToggleState());
            propertiesFile->setValue("AftertouchVelocityCurveWindowWidth", getWidth());
            propertiesFile->setValue("AftertouchVelocityCurveWindowHeight", getHeight());
            break;
        default:
            jassert(false);
            break;
    }
}

void VelocityCurveDlg::sendVelocityTableToController()
{
	unsigned char velocityValues[128];

	for (int x = 0; x < 128; x++)
	{
		velocityValues[x] = velocityBeamTable[x]->getValue();
	}

	TerpstraSysExApplication::getApp().getMidiDriver().sendVelocityConfig(velocityCurveType, velocityValues);
}

void VelocityCurveDlg::showBeamValueOfMousePosition(juce::Point<float> localPoint)
{
	if (beamTableFrame.contains(localPoint))
	{
		// Show the field with the current beam value
		labelCurrentBeamValue->setVisible(true);
		labelCurrentBeamValue->setBounds(
			localPoint.x, localPoint.y - labelCurrentBeamValue->getHeight(), labelCurrentBeamValue->getWidth(), labelCurrentBeamValue->getHeight());

		// Value
		labelCurrentBeamValue->setText(String(velocityBeamTable[0]->getBeamValueFromLocalPoint(localPoint)), juce::NotificationType::sendNotification);
	}
	else
		// Hide field
		labelCurrentBeamValue->setVisible(false);
}

void VelocityCurveDlg::mouseMove(const MouseEvent &event)
{
	juce::Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	showBeamValueOfMousePosition(localPoint);

	if (currentCurveEditStrategy != nullptr)
	{
		if (currentCurveEditStrategy->mouseMove(event, localPoint))
			repaint();
	}
}

void VelocityCurveDlg::mouseDown(const MouseEvent &event)
{
	juce::Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	showBeamValueOfMousePosition(localPoint);

	if (currentCurveEditStrategy != nullptr)
	{
		if (currentCurveEditStrategy->mouseDown(event, localPoint))
		{
			repaint();

			// If something was edited: Unselect preset
			cbPreset->setSelectedItemIndex(-1, juce::NotificationType::dontSendNotification);
		}
	}
}

void VelocityCurveDlg::mouseDrag(const MouseEvent &event)
{
	juce::Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	showBeamValueOfMousePosition(localPoint);

	if (currentCurveEditStrategy != nullptr)
	{
		if (currentCurveEditStrategy->mouseDrag(event, localPoint))
		{
			repaint();

			// If something was edited: Unselect preset
			cbPreset->setSelectedItemIndex(-1, juce::NotificationType::dontSendNotification);
		}
	}
}

void VelocityCurveDlg::mouseUp(const MouseEvent &event)
{
	juce::Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	// Send velocity table to controller
	sendVelocityTableToController();

	if (currentCurveEditStrategy != nullptr)
		currentCurveEditStrategy->mouseUp(event, localPoint);

	repaint();
}


void VelocityCurveDlg::midiMessageReceived(const MidiMessage& message)
{
    // ToDo
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VelocityCurveDlg" componentName=""
                 parentClasses="public Component, public TerpstraMidiDriver::Listener"
                 constructorParams="TerpstraMidiDriver::VelocityCurveType typeValue"
                 variableInitialisers="freeDrawingStrategy(beamTableFrame, velocityBeamTable)&#10;linearDrawingStrategy(beamTableFrame, velocityBeamTable)&#10;quadraticDrawingStrategy(beamTableFrame, velocityBeamTable)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="768" initialHeight="424">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <LABEL name="lblDescription" id="e1affcc7a142cab2" memberName="lblDescription"
         virtualName="" explicitFocusOrder="0" pos="32 104 608 32" edTextCol="ff000000"
         edBkgCol="0" labelText="Click with the mouse in the graphics to draw the velocity curve."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <TEXTBUTTON name="buttonSendAll" id="71e432722656a5b7" memberName="buttonSendAll"
              virtualName="" explicitFocusOrder="0" pos="312 392 144 24" tooltip="Send whole velocity curve map to controller and save it there."
              buttonText="Send &amp; Save All" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="buttonDiscard" id="8943d46ddc434616" memberName="buttonDiscard"
              virtualName="" explicitFocusOrder="0" pos="464 368 144 24" tooltip="Discard velocity curve edits on controller."
              buttonText="Discard Edits" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="buttonSaveEdits" id="b3ed9064acdde93" memberName="buttonSaveEdits"
              virtualName="" explicitFocusOrder="0" pos="160 392 144 24" tooltip="Save velocity curve edits that have been sent on controller"
              buttonText="Save Edits" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <COMBOBOX name="cbEditMode" id="1f22301dd42b968e" memberName="cbEditMode"
            virtualName="" explicitFocusOrder="0" pos="144 72 296 24" editable="0"
            layout="33" items="Free drawing&#10;Linear&#10;Quadratic" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <LABEL name="labelEditMode" id="55d538af27203498" memberName="labelEditMode"
         virtualName="" explicitFocusOrder="0" pos="32 72 103 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Edit Function:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="cbPreset" id="e5845a95b8b0cb19" memberName="cbPreset" virtualName=""
            explicitFocusOrder="0" pos="144 32 296 24" editable="1" layout="33"
            items="One to one" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="labelPresets" id="aa3a0484f33857d9" memberName="labelPresets"
         virtualName="" explicitFocusOrder="0" pos="32 32 107 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Presets:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="labelCurrentBeamValue" id="5ddce68a8155d39e" memberName="labelCurrentBeamValue"
         virtualName="" explicitFocusOrder="0" pos="0 152 31 24" edTextCol="ff000000"
         edBkgCol="0" labelText="127" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="buttonReceive" id="5545cd9fc9bd20cb" memberName="buttonReceive"
              virtualName="" explicitFocusOrder="0" pos="8 392 144 24" tooltip="Receive the current configurartion from controller"
              buttonText="Receive" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonCalibrate" id="47242594c34a8de9" memberName="buttonCalibrate"
              virtualName="" explicitFocusOrder="0" pos="616 368 144 24" tooltip="Calibrate aftertouch"
              buttonText="Calibrate" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="buttonAfterTouchActive" id="3f2eba6027c4f2f5" memberName="buttonAfterTouchActive"
                virtualName="" explicitFocusOrder="0" pos="144 0 144 24" buttonText="Aftertouch active"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

