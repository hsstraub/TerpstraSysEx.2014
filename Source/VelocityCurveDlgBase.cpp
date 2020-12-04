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
#include "Main.h"
//[/Headers]

#include "VelocityCurveDlgBase.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VelocityCurveDlgBase::VelocityCurveDlgBase (TerpstraMidiDriver::VelocityCurveType typeValue)
    : freeDrawingStrategy(beamTableFrame, velocityBeamTable),
      linearDrawingStrategy(beamTableFrame, velocityBeamTable),
      quadraticDrawingStrategy(beamTableFrame, velocityBeamTable)
{
    //[Constructor_pre] You can add your own custom stuff here..
	velocityCurveType = typeValue;
	currentCurveEditStrategy = nullptr;
    //[/Constructor_pre]

    cbEditMode.reset (new juce::ComboBox ("cbEditMode"));
    addAndMakeVisible (cbEditMode.get());
    cbEditMode->setEditableText (false);
    cbEditMode->setJustificationType (juce::Justification::centredLeft);
    cbEditMode->setTextWhenNothingSelected (TRANS("Select drawing mode"));
    cbEditMode->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbEditMode->addItem (TRANS("Free drawing"), 1);
    cbEditMode->addItem (TRANS("Linear"), 2);
    cbEditMode->addItem (TRANS("Quadratic"), 3);
    cbEditMode->addListener (this);

    cbEditMode->setBounds (8, 8, 296, 24);

    labelCurrentBeamValue.reset (new juce::Label ("labelCurrentBeamValue",
                                                  TRANS("127")));
    addAndMakeVisible (labelCurrentBeamValue.get());
    labelCurrentBeamValue->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelCurrentBeamValue->setJustificationType (juce::Justification::centredLeft);
    labelCurrentBeamValue->setEditable (false, false, false);
    labelCurrentBeamValue->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelCurrentBeamValue->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelCurrentBeamValue->setBounds (0, 40, 31, 16);

    labelCurrentXPos.reset (new juce::Label ("labelCurrentXPos",
                                             TRANS("127")));
    addAndMakeVisible (labelCurrentXPos.get());
    labelCurrentXPos->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelCurrentXPos->setJustificationType (juce::Justification::centredLeft);
    labelCurrentXPos->setEditable (false, false, false);
    labelCurrentXPos->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelCurrentXPos->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelCurrentXPos->setBounds (40, 40, 31, 16);


    //[UserPreSize]

	for (int x = 0; x < 128; x++)
	{
		velocityBeamTable[x].reset(new  VelocityCurveBeam());
		addAndMakeVisible(velocityBeamTable[x].get());
		velocityBeamTable[x]->addMouseListener(this, true);
	}

    //[/UserPreSize]

    setSize (320, 160);


    //[Constructor] You can add your own custom stuff here..
    TerpstraSysExApplication::getApp().getMidiDriver().addListener(this);

	labelCurrentBeamValue->setVisible(false);
	labelCurrentXPos->setVisible(false);
    //[/Constructor]
}

VelocityCurveDlgBase::~VelocityCurveDlgBase()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    TerpstraSysExApplication::getApp().getMidiDriver().removeListener(this);

    for (int x = 0; x < 128; x++) {
		velocityBeamTable[x] = nullptr;
	}
    //[/Destructor_pre]

    cbEditMode = nullptr;
    labelCurrentBeamValue = nullptr;
    labelCurrentXPos = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	deleteAllChildren();
    //[/Destructor]
}

//==============================================================================
void VelocityCurveDlgBase::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(findColour(ResizableWindow::backgroundColourId));

	g.setColour(findColour(VelocityCurveBeam::outlineColourId));
	g.strokePath(beamTableFrame, PathStrokeType(1.000f));

	if (currentCurveEditStrategy != nullptr)
	{
		//lblDescription->setText(currentCurveEditStrategy->getDescriptionText(), juce::NotificationType::dontSendNotification);
		currentCurveEditStrategy->paint(g, getLookAndFeel());
	}
    //[/UserPaint]
}

void VelocityCurveDlgBase::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	int w = this->getWidth();
	int h = this->getHeight();
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..

	float graphicsXPadding = cbEditMode->getX();
	float graphicsYPos = cbEditMode->getBottom() + BEAMTABLERIMABOVE;
	float graphicsBottom = h - labelCurrentXPos->getHeight();

	beamTableFrame.clear();
	beamTableFrame.startNewSubPath(graphicsXPadding, graphicsYPos);
	beamTableFrame.lineTo(graphicsXPadding, graphicsBottom);
	beamTableFrame.lineTo(w - graphicsXPadding, graphicsBottom);
	beamTableFrame.lineTo(w - graphicsXPadding, graphicsYPos);
	beamTableFrame.closeSubPath();

	if (currentCurveEditStrategy != nullptr)
		currentCurveEditStrategy->resized();

	float velocityGraphicsHeight = graphicsBottom - graphicsYPos;
	float velocityBeamXPos = graphicsXPadding;
	for (int x = 0; x < 128; x++)
	{
		auto velocityBeamWidth = beamWidth(x);
		velocityBeamTable[x]->setBounds(velocityBeamXPos, graphicsYPos, velocityBeamWidth, velocityGraphicsHeight);
		velocityBeamXPos += velocityBeamWidth;
	}

    //[/UserResized]
}

void VelocityCurveDlgBase::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
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

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

// ToDo Read from and write to *.LMT file

//void VelocityCurveDlgBase::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
//{
//	String keyName;
//    switch(velocityCurveType)
//    {
//        case TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff:
//            keyName = "NoteOnOffVelocityCurveTable";
//            break;
//        case TerpstraMidiDriver::VelocityCurveType::fader:
//            keyName = "FaderVelocityCurveTable";
//            break;
//        case TerpstraMidiDriver::VelocityCurveType::afterTouch:
//            keyName = "AfterTouchCurveTable";
//            break;
//        default:
//            jassert(false);
//            break;
//    }
//
//	String propertiesString = propertiesFile->getValue(keyName);
//
//	// Format of properties string includes edit strategy. Parse values incl. edit strategy. Free drawing (default) last
//
//	if (linearDrawingStrategy.setEditConfigFromSavedString(propertiesString))
//	{
//		cbEditMode->setSelectedItemIndex(EDITSTRATEGYINDEX::linearSegments, juce::NotificationType::dontSendNotification);
//		currentCurveEditStrategy = &linearDrawingStrategy;
//	}
//	else if (quadraticDrawingStrategy.setEditConfigFromSavedString(propertiesString))
//	{
//		cbEditMode->setSelectedItemIndex(EDITSTRATEGYINDEX::quadraticCurves, juce::NotificationType::dontSendNotification);
//		currentCurveEditStrategy = &quadraticDrawingStrategy;
//	}
//	else if (freeDrawingStrategy.setEditConfigFromSavedString(propertiesString))
//	{
//		cbEditMode->setSelectedItemIndex(EDITSTRATEGYINDEX::freeDrawing, juce::NotificationType::dontSendNotification);
//		currentCurveEditStrategy = &freeDrawingStrategy;
//	}
//	else
//	{
//		// Initialize velocity lookup table
//		for (int x = 0; x < 128; x++)
//			velocityBeamTable[x]->setValue(x);
//
//		cbEditMode->setSelectedItemIndex(EDITSTRATEGYINDEX::none, juce::NotificationType::dontSendNotification);
//		currentCurveEditStrategy = nullptr;
//	}
//
//	if (currentCurveEditStrategy != nullptr)
//		currentCurveEditStrategy->setVelocityTableValuesFromEditConfig();
//
//    // Window size: read in calling function when creating this window
//
//    if(velocityCurveType == TerpstraMidiDriver::VelocityCurveType::afterTouch)
//    {
//        buttonAfterTouchActive->setToggleState(
//            propertiesFile->getBoolValue("AfterTouchActive", false),
//            juce::NotificationType::sendNotification);
//    }
//}
//
//void VelocityCurveDlgBase::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
//{
//	String velocityCurveString;
//
//	if (currentCurveEditStrategy != nullptr)
//	{
//		velocityCurveString = currentCurveEditStrategy->createPropertiesStringForSaving();
//	}
//	else
//	{
//		jassertfalse;
//		velocityCurveString = String();
//	}
//
//	String keyName;
//    switch(velocityCurveType)
//    {
//        case TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff:
//            keyName = "NoteOnOffVelocityCurveTable";
//            break;
//        case TerpstraMidiDriver::VelocityCurveType::fader:
//            keyName = "FaderVelocityCurveTable";
//            break;
//        case TerpstraMidiDriver::VelocityCurveType::afterTouch:
//            keyName = "AfterTouchCurveTable";
//            break;
//        default:
//            jassert(false);
//            break;
//    }
//
//	propertiesFile->setValue(keyName, velocityCurveString);
//
//    switch(velocityCurveType)
//    {
//        case TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff:
//            propertiesFile->setValue("VelocityCurveWindowWidth", getParentComponent()->getWidth());
//            propertiesFile->setValue("VelocityCurveWindowHeight", getParentComponent()->getHeight());
//            break;
//        case TerpstraMidiDriver::VelocityCurveType::fader:
//            propertiesFile->setValue("FaderVelocityCurveWindowWidth", getParentComponent()->getWidth());
//            propertiesFile->setValue("FaderVelocityCurveWindowHeight", getParentComponent()->getHeight());
//            break;
//        case TerpstraMidiDriver::VelocityCurveType::afterTouch:
//            propertiesFile->setValue("AfterTouchActive", buttonAfterTouchActive->getToggleState());
//            propertiesFile->setValue("AftertouchVelocityCurveWindowWidth", getParentComponent()->getWidth());
//            propertiesFile->setValue("AftertouchVelocityCurveWindowHeight", getParentComponent()->getHeight());
//            break;
//        default:
//            jassert(false);
//            break;
//    }
//}

void VelocityCurveDlgBase::sendVelocityTableToController()
{
	unsigned char velocityValues[128];

	for (int x = 0; x < 128; x++)
	{
		velocityValues[x] = velocityBeamTable[x]->getValue();
	}

	TerpstraSysExApplication::getApp().getMidiDriver().sendVelocityConfig(velocityCurveType, velocityValues);
}

void VelocityCurveDlgBase::sendVelocityConfigurationRequest()
{
	TerpstraSysExApplication::getApp().getMidiDriver().sendVelocityConfigurationRequest(velocityCurveType);
}

bool VelocityCurveDlgBase::showBeamValueOfMousePosition(juce::Point<float> localPoint)
{
	if (beamTableFrame.contains(localPoint))
	{
        int beamTableLeft = velocityBeamTable[0]->getX();
        int beamTableRight = velocityBeamTable[127]->getRight();
        int xpos = (localPoint.x - beamTableLeft) * 128 / (beamTableRight - beamTableLeft);
        if (xpos >= 0 && xpos < 128)
        {
			// Show the field with the current beam value
			labelCurrentBeamValue->setVisible(true);
			labelCurrentBeamValue->setBounds(
				jmin(roundToInt(localPoint.x), getWidth() - labelCurrentBeamValue->getWidth()),
				localPoint.y - labelCurrentBeamValue->getHeight(),
				labelCurrentBeamValue->getWidth(),
				labelCurrentBeamValue->getHeight());

			// Value
			labelCurrentBeamValue->setText(beamValueText(velocityBeamTable[0]->getBeamValueFromLocalPoint(localPoint)), juce::NotificationType::sendNotification);

			// Show x value (beam position)
			labelCurrentXPos->setVisible(true);
			labelCurrentXPos->setBounds(
				jmin(roundToInt(localPoint.x), getWidth() - labelCurrentXPos->getWidth()),
				velocityBeamTable[0]->getBottom(),
                labelCurrentXPos->getWidth(),
                labelCurrentXPos->getHeight());
            labelCurrentXPos->setText(beamXPosText(xpos), juce::NotificationType::sendNotification);

			return true;
        }
        else
        {
            labelCurrentBeamValue->setVisible(false);
            labelCurrentXPos->setVisible(false);

			return false;
        }
	}
	else
    {
		// Hide fields
		labelCurrentBeamValue->setVisible(false);
		labelCurrentXPos->setVisible(false);

		return false;
    }
}

void VelocityCurveDlgBase::mouseMove(const MouseEvent &event)
{
	juce::Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	bool doRepaint = showBeamValueOfMousePosition(localPoint);

	if (currentCurveEditStrategy != nullptr)
		doRepaint |= currentCurveEditStrategy->mouseMove(event, localPoint);

	if (doRepaint)
		repaint();
}

void VelocityCurveDlgBase::mouseDown(const MouseEvent &event)
{
	juce::Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	showBeamValueOfMousePosition(localPoint);

	if (currentCurveEditStrategy != nullptr)
	{
		if (currentCurveEditStrategy->mouseDown(event, localPoint))
		{
			repaint();
		}
	}
}

void VelocityCurveDlgBase::mouseDrag(const MouseEvent &event)
{
	juce::Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	showBeamValueOfMousePosition(localPoint);

	if (currentCurveEditStrategy != nullptr)
	{
		if (currentCurveEditStrategy->mouseDrag(event, localPoint))
		{
			repaint();
		}
	}
}

void VelocityCurveDlgBase::mouseUp(const MouseEvent &event)
{
	juce::Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	if (currentCurveEditStrategy != nullptr)
		currentCurveEditStrategy->mouseUp(event, localPoint);

    // Send velocity table to controller
	sendVelocityTableToController();

	repaint();
}


void VelocityCurveDlgBase::midiMessageReceived(const MidiMessage& message)
{
    if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsTerpstraVelocityConfigReceptionMessage(message, velocityCurveType))
    {
        auto sysExData = message.getSysExData();
        auto answerState = sysExData[5];

        if (answerState == TerpstraMidiDriver::ACK)
        {
            // After the answer state byte there must be 128 bytes of data
            jassert(message.getSysExDataSize() >= 134); // ToDo display error otherwise

            cbEditMode->setSelectedItemIndex(EDITSTRATEGYINDEX::freeDrawing, juce::NotificationType::dontSendNotification);
            currentCurveEditStrategy = &freeDrawingStrategy;

            for (int x = 0; x < 128; x++)
                velocityBeamTable[x]->setValue(sysExData[6+x]);
        }
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VelocityCurveDlgBase" componentName=""
                 parentClasses="public Component, public TerpstraMidiDriver::Listener"
                 constructorParams="TerpstraMidiDriver::VelocityCurveType typeValue"
                 variableInitialisers="freeDrawingStrategy(beamTableFrame, velocityBeamTable)&#10;linearDrawingStrategy(beamTableFrame, velocityBeamTable)&#10;quadraticDrawingStrategy(beamTableFrame, velocityBeamTable)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="320" initialHeight="160">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <COMBOBOX name="cbEditMode" id="1f22301dd42b968e" memberName="cbEditMode"
            virtualName="" explicitFocusOrder="0" pos="8 8 296 24" editable="0"
            layout="33" items="Free drawing&#10;Linear&#10;Quadratic" textWhenNonSelected="Select drawing mode"
            textWhenNoItems="(no choices)"/>
  <LABEL name="labelCurrentBeamValue" id="5ddce68a8155d39e" memberName="labelCurrentBeamValue"
         virtualName="" explicitFocusOrder="0" pos="0 40 31 16" edTextCol="ff000000"
         edBkgCol="0" labelText="127" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="labelCurrentXPos" id="f2fe328e83d5b29" memberName="labelCurrentXPos"
         virtualName="" explicitFocusOrder="0" pos="40 40 31 16" edTextCol="ff000000"
         edBkgCol="0" labelText="127" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

