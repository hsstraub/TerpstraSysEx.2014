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

#include "VelocityCurveSubDlg.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VelocityCurveSubDlg::VelocityCurveSubDlg (TerpstraMidiDriver::VelocityCurveType typeValue, int newMaxBeamValue)
    : velocityCurveType(typeValue),
      maxBeamValue(newMaxBeamValue),
      freeDrawingStrategy(beamTableFrame, velocityBeamTable), linearDrawingStrategy(beamTableFrame, velocityBeamTable), quadraticDrawingStrategy(beamTableFrame, velocityBeamTable)
{
    //[Constructor_pre] You can add your own custom stuff here..
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

    lblDescription->setBounds (11, 81, 608, 32);

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

    cbEditMode->setBounds (123, 49, 296, 24);

    labelEditMode.reset (new Label ("labelEditMode",
                                    TRANS("Edit Function:")));
    addAndMakeVisible (labelEditMode.get());
    labelEditMode->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelEditMode->setJustificationType (Justification::centredLeft);
    labelEditMode->setEditable (false, false, false);
    labelEditMode->setColour (TextEditor::textColourId, Colours::black);
    labelEditMode->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelEditMode->setBounds (11, 49, 103, 24);

    cbPreset.reset (new ComboBox ("cbPreset"));
    addAndMakeVisible (cbPreset.get());
    cbPreset->setEditableText (true);
    cbPreset->setJustificationType (Justification::centredLeft);
    cbPreset->setTextWhenNothingSelected (String());
    cbPreset->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbPreset->addItem (TRANS("One to one"), 1);
    cbPreset->addListener (this);

    cbPreset->setBounds (123, 9, 296, 24);

    labelPresets.reset (new Label ("labelPresets",
                                   TRANS("Presets:")));
    addAndMakeVisible (labelPresets.get());
    labelPresets->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelPresets->setJustificationType (Justification::centredLeft);
    labelPresets->setEditable (false, false, false);
    labelPresets->setColour (TextEditor::textColourId, Colours::black);
    labelPresets->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelPresets->setBounds (11, 9, 107, 24);

    labelCurrentBeamValue.reset (new Label ("labelCurrentBeamValue",
                                            TRANS("127")));
    addAndMakeVisible (labelCurrentBeamValue.get());
    labelCurrentBeamValue->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelCurrentBeamValue->setJustificationType (Justification::centredLeft);
    labelCurrentBeamValue->setEditable (false, false, false);
    labelCurrentBeamValue->setColour (TextEditor::textColourId, Colours::black);
    labelCurrentBeamValue->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelCurrentBeamValue->setBounds (16, 112, 31, 24);


    //[UserPreSize]
	for (int x = 0; x < 128; x++)
	{
		velocityBeamTable[x] = new  VelocityCurveBeam(maxBeamValue);
		addAndMakeVisible(velocityBeamTable[x]);
		velocityBeamTable[x]->addMouseListener(this, true);
	}
    //[/UserPreSize]

    setSize (768, 212);


    //[Constructor] You can add your own custom stuff here..
    TerpstraSysExApplication::getApp().getMidiDriver().addListener(this);

	labelCurrentBeamValue->setVisible(false);
    //[/Constructor]
}

VelocityCurveSubDlg::~VelocityCurveSubDlg()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    TerpstraSysExApplication::getApp().getMidiDriver().removeListener(this);
    //[/Destructor_pre]

    lblDescription = nullptr;
    cbEditMode = nullptr;
    labelEditMode = nullptr;
    cbPreset = nullptr;
    labelPresets = nullptr;
    labelCurrentBeamValue = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void VelocityCurveSubDlg::paint (Graphics& g)
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

void VelocityCurveSubDlg::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	int w = this->getWidth();
	int h = this->getHeight();
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
  	float graphicsXPos = lblDescription->getX();
  	float graphicsYPos = lblDescription->getBottom() + graphicsYPadding;

	beamTableFrame.clear();
	beamTableFrame.startNewSubPath(graphicsXPos, graphicsYPos);
	beamTableFrame.lineTo(graphicsXPos, h - graphicsYPadding);
	beamTableFrame.lineTo(w - graphicsXPos, h - graphicsYPadding);
	beamTableFrame.lineTo(w - graphicsXPos, graphicsYPos);
	beamTableFrame.closeSubPath();

	if (currentCurveEditStrategy != nullptr)
		currentCurveEditStrategy->resized();

	float velocityBeamWidth = (w - 2 * graphicsXPos) / 128;
	float velocityGraphicsHeight = h - graphicsYPadding - graphicsYPos;

	for (int x = 0; x < 128; x++)
	{
		velocityBeamTable[x]->setBounds(
			graphicsXPos + x*velocityBeamWidth,
			graphicsYPos,
			velocityBeamWidth,
			velocityGraphicsHeight);
	}
    //[/UserResized]
}

void VelocityCurveSubDlg::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
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
				velocityBeamTable[x]->setValue(x*(velocityBeamTable[x]->getMaxValue()+1)/128);
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

void VelocityCurveSubDlg::restoreStateFromPropertiesString(const String& propertiesString)
{
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
			velocityBeamTable[x]->setValue(x*(velocityBeamTable[x]->getMaxValue()+1)/128);

		cbEditMode->setSelectedItemIndex(EDITSTRATEGYINDEX::none, juce::NotificationType::dontSendNotification);
		currentCurveEditStrategy = nullptr;
	}

	if (currentCurveEditStrategy != nullptr)
		currentCurveEditStrategy->setVelocityTableValuesFromEditConfig();
}

String VelocityCurveSubDlg::saveStateToPropertiesString()
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

	return velocityCurveString;
}

void VelocityCurveSubDlg::sendVelocityTableToController()
{
	unsigned char velocityValues[128];

	for (int x = 0; x < 128; x++)
	{
		velocityValues[x] = velocityBeamTable[x]->getValue();
	}

	TerpstraSysExApplication::getApp().getMidiDriver().sendVelocityConfig(velocityCurveType, velocityValues);
}

void VelocityCurveSubDlg::showBeamValueOfMousePosition(juce::Point<float> localPoint)
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

void VelocityCurveSubDlg::mouseMove(const MouseEvent &event)
{
	juce::Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	showBeamValueOfMousePosition(localPoint);

	if (currentCurveEditStrategy != nullptr)
	{
		if (currentCurveEditStrategy->mouseMove(event, localPoint))
			repaint();
	}
}

void VelocityCurveSubDlg::mouseDown(const MouseEvent &event)
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

void VelocityCurveSubDlg::mouseDrag(const MouseEvent &event)
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

void VelocityCurveSubDlg::mouseUp(const MouseEvent &event)
{
	juce::Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	// Send velocity table to controller
	sendVelocityTableToController();

	if (currentCurveEditStrategy != nullptr)
		currentCurveEditStrategy->mouseUp(event, localPoint);

	repaint();
}

void VelocityCurveSubDlg::midiMessageReceived(const MidiMessage& message)
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

<JUCER_COMPONENT documentType="Component" className="VelocityCurveSubDlg" componentName=""
                 parentClasses="public Component, public TerpstraMidiDriver::Listener"
                 constructorParams="TerpstraMidiDriver::VelocityCurveType typeValue, int newMaxBeamValue"
                 variableInitialisers="velocityCurveType(typeValue),&#10;maxBeamValue(newMaxBeamValue),&#10;freeDrawingStrategy(beamTableFrame, velocityBeamTable), linearDrawingStrategy(beamTableFrame, velocityBeamTable), quadraticDrawingStrategy(beamTableFrame, velocityBeamTable)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="768" initialHeight="212">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <LABEL name="lblDescription" id="e1affcc7a142cab2" memberName="lblDescription"
         virtualName="" explicitFocusOrder="0" pos="11 81 608 32" edTextCol="ff000000"
         edBkgCol="0" labelText="Click with the mouse in the graphics to draw the velocity curve."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <COMBOBOX name="cbEditMode" id="1f22301dd42b968e" memberName="cbEditMode"
            virtualName="" explicitFocusOrder="0" pos="123 49 296 24" editable="0"
            layout="33" items="Free drawing&#10;Linear&#10;Quadratic" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <LABEL name="labelEditMode" id="55d538af27203498" memberName="labelEditMode"
         virtualName="" explicitFocusOrder="0" pos="11 49 103 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Edit Function:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="cbPreset" id="e5845a95b8b0cb19" memberName="cbPreset" virtualName=""
            explicitFocusOrder="0" pos="123 9 296 24" editable="1" layout="33"
            items="One to one" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="labelPresets" id="aa3a0484f33857d9" memberName="labelPresets"
         virtualName="" explicitFocusOrder="0" pos="11 9 107 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Presets:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="labelCurrentBeamValue" id="5ddce68a8155d39e" memberName="labelCurrentBeamValue"
         virtualName="" explicitFocusOrder="0" pos="16 112 31 24" edTextCol="ff000000"
         edBkgCol="0" labelText="127" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

