/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "Main.h"
//[/Headers]

#include "VelocityCurveDlg.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VelocityCurveDlg::VelocityCurveDlg ()
{
    //[Constructor_pre] You can add your own custom stuff here..
	keyType = TerpstraKey::noteOnNoteOff;
    //[/Constructor_pre]

    addAndMakeVisible (lblDescription = new Label ("lblDescription",
                                                   TRANS("Click with the mouse in the graphics to draw the velocity curve.")));
    lblDescription->setFont (Font (15.00f, Font::plain));
    lblDescription->setJustificationType (Justification::centredLeft);
    lblDescription->setEditable (false, false, false);
    lblDescription->setColour (TextEditor::textColourId, Colours::black);
    lblDescription->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (buttonSendAll = new TextButton ("buttonSendAll"));
    buttonSendAll->setTooltip (TRANS("Send whole velocity curve map to controller and save it there."));
    buttonSendAll->setButtonText (TRANS("Send & Save All"));
    buttonSendAll->addListener (this);

    addAndMakeVisible (buttonDiscard = new TextButton ("buttonDiscard"));
    buttonDiscard->setTooltip (TRANS("Discard velocity curve edits on controller."));
    buttonDiscard->setButtonText (TRANS("Discard Edits"));
    buttonDiscard->addListener (this);

    addAndMakeVisible (buttonSaveEdits = new TextButton ("buttonSaveEdits"));
    buttonSaveEdits->setTooltip (TRANS("Save velocity curve edits that have been sent on controller"));
    buttonSaveEdits->setButtonText (TRANS("Save Edits"));
    buttonSaveEdits->addListener (this);


    //[UserPreSize]

	for (int x = 0; x < 128; x++)
	{
		velocityBeamTable[x] = new  VelocityCurveBeam();
		addAndMakeVisible(velocityBeamTable[x]);
		velocityBeamTable[x]->addMouseListener(this, true);
	}

    //[/UserPreSize]

    setSize (640, 320);


    //[Constructor] You can add your own custom stuff here..
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
	g.setColour(Colours::black);
	g.strokePath(beamTableFrame, PathStrokeType(1.000f));

	if (!drawedLine.isEmpty() )
		g.strokePath(drawedLine, PathStrokeType(1.000f));
    //[/UserPaint]
}

void VelocityCurveDlg::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	int w = this->getWidth();
	int h = this->getHeight();
    //[/UserPreResize]

    lblDescription->setBounds (8, 8, 416, 32);
    buttonSendAll->setBounds (168, 288, 150, 24);
    buttonDiscard->setBounds (328, 288, 150, 24);
    buttonSaveEdits->setBounds (8, 288, 150, 24);
    //[UserResized] Add your own custom resize handling here..

	beamTableFrame.clear();
	beamTableFrame.startNewSubPath(graphicsXPadding, graphicsYPadding);
	beamTableFrame.lineTo(graphicsXPadding, h - graphicsYPadding);
	beamTableFrame.lineTo(w - graphicsXPadding, h - graphicsYPadding);
	beamTableFrame.lineTo(w - graphicsXPadding, graphicsYPadding);
	beamTableFrame.closeSubPath();

	drawedLine.clear();

	float velocityBeamWidth = (w - 2 * graphicsXPadding) / 128;
	float velocityGraphicsHeight = h - 2 * graphicsYPadding;

	for (int x = 0; x < 128; x++)
	{
		velocityBeamTable[x]->setBounds(
			graphicsXPadding + x*velocityBeamWidth,
			graphicsYPadding,
			velocityBeamWidth,
			velocityGraphicsHeight);
	}

	int buttonYPos = graphicsYPadding * 5 / 4 + velocityGraphicsHeight;

	buttonSendAll->setBounds(buttonSendAll->getX(), buttonYPos,
		buttonSendAll->getWidth(), buttonSendAll->getHeight());
	buttonDiscard->setBounds(buttonDiscard->getX(), buttonYPos,
		buttonDiscard->getWidth(), buttonDiscard->getHeight());
	buttonSaveEdits->setBounds(buttonSaveEdits->getX(), buttonYPos,
		buttonSaveEdits->getWidth(), buttonSaveEdits->getHeight());

    //[/UserResized]
}

void VelocityCurveDlg::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonSendAll)
    {
        //[UserButtonCode_buttonSendAll] -- add your button handler code here..
		// Send all
		sendVelocityTableToController();

		// Save
		TerpstraSysExApplication::getApp().getMidiDriver().saveVelocityConfig(keyType);
        //[/UserButtonCode_buttonSendAll]
    }
    else if (buttonThatWasClicked == buttonDiscard)
    {
        //[UserButtonCode_buttonDiscard] -- add your button handler code here..
		TerpstraSysExApplication::getApp().getMidiDriver().resetVelocityConfig(keyType);
        //[/UserButtonCode_buttonDiscard]
    }
    else if (buttonThatWasClicked == buttonSaveEdits)
    {
        //[UserButtonCode_buttonSaveEdits] -- add your button handler code here..
		TerpstraSysExApplication::getApp().getMidiDriver().saveVelocityConfig(keyType);
        //[/UserButtonCode_buttonSaveEdits]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

VelocityCurveDlg::VelocityCurveDlg(TerpstraKey::KEYTYPE keyTypeValue)
	: VelocityCurveDlg()
{
	keyType = keyTypeValue;

	// Set values according to the properties files
	restoreStateFromPropertiesFile(TerpstraSysExApplication::getApp().getPropertiesFile());
}

void VelocityCurveDlg::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	String keyName = keyType == TerpstraKey::continuousController ? "FaderVelocityCurveTable" : "NoteOnOffVelocityCurveTable";

	StringArray velocityCurveValueArray = StringArray::fromTokens(propertiesFile->getValue(keyName), false);
	if (velocityCurveValueArray.size() > 0)
	{
		jassert(velocityCurveValueArray.size() >= 128);

		for (int x = 0; x < 128; x++)
			setBeamValue(x, velocityCurveValueArray[x].getIntValue()/*, false*/);
	}
	else
	{
		// Initialize velocity lookup table
		for (int x = 0; x < 128; x++)
			setBeamValue(x, x/*, false*/ );
	}

	setSize(
		propertiesFile->getIntValue("VelocityCurveWindowWidth", 640),
		propertiesFile->getIntValue("VelocityCurveWindowHeight", 320));
}

void VelocityCurveDlg::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	String velocityCurveString;

	for (int x = 0; x < 128; x++)
		velocityCurveString += String(velocityBeamTable[x]->getValue()) + " ";

	String keyName = keyType == TerpstraKey::continuousController ? "FaderVelocityCurveTable" : "NoteOnOffVelocityCurveTable";

	propertiesFile->setValue(keyName, velocityCurveString);

	propertiesFile->setValue("VelocityCurveWindowWidth", getWidth());
	propertiesFile->setValue("VelocityCurveWindowHeight", getHeight());
}

void VelocityCurveDlg::setBeamValue(int pos, int newValue/*, bool sendToController*/)
{
	if (pos >= 0 && pos < 128)
	{
		if (newValue < 0)
		{
			jassertfalse;
			newValue = 0;
		}

		if (newValue > 127)
		{
			jassertfalse;
			newValue = 127;
		}

		if (newValue != velocityBeamTable[pos]->getValue())
		{
			velocityBeamTable[pos]->setValue(newValue);

			//if (sendToController)
			//	TerpstraSysExApplication::getApp().getMidiDriver().sendVelocityConfig(keyType, pos, newValue);
		}
	}
	else
		jassertfalse;
}

void VelocityCurveDlg::setBeamValueAtLeast(int pos, int newValue/*, bool sendToController*/)
{
	if (pos >= 0 && pos < 128)
	{
		if (velocityBeamTable[pos]->getValue() < newValue)
			setBeamValue(pos, newValue/*, sendToController*/);
	}
	else
		jassertfalse;
}

void VelocityCurveDlg::setBeamValueAtMost(int pos, int newValue/*, bool sendToController*/)
{
	if (pos >= 0 && pos < 128)
	{
		if (velocityBeamTable[pos]->getValue() > newValue)
			setBeamValue(pos, newValue/*, sendToController*/);
	}
	else
		jassertfalse;
}

void VelocityCurveDlg::sendVelocityTableToController()
{
	unsigned char velocityValues[128];

	for (int x = 0; x < 128; x++)
	{
		velocityValues[x] = velocityBeamTable[x]->getValue();
	}

	TerpstraSysExApplication::getApp().getMidiDriver().sendVelocityConfig(keyType, velocityValues);
}

void VelocityCurveDlg::mouseDown(const MouseEvent &event)
{
	int h = this->getHeight();
	float velocityGraphicsHeight = h - 2 * graphicsYPadding;

	Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	drawedLine.clear();
	if (beamTableFrame.contains(localPoint))
	{
		drawedLine.startNewSubPath(localPoint.x, localPoint.y);
	}
}

void VelocityCurveDlg::mouseDrag(const MouseEvent &event)
{
	Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	for (int x = 0; x < 128; x++)
	{
		Rectangle<int> beamRect = velocityBeamTable[x]->getBounds();
		if (beamRect.contains((int)localPoint.x, (int)localPoint.y))
		{
			drawedLine.lineTo(localPoint);
			repaint();

			int newBeamValue = (beamRect.getBottom() - localPoint.y) * 128 / beamRect.getHeight();
			setBeamValue(x, newBeamValue/*, true*/);

			// Change other beams' values so curve stays monotonous
			for (int x2 = 0; x2 < x; x2++)
				setBeamValueAtMost(x2, newBeamValue/*, true*/);

			for (int x2 = x + 1; x2 < 128; x2++)
				setBeamValueAtLeast(x2, newBeamValue/*, true*/);
		
			break;
		}
	}
}

void VelocityCurveDlg::mouseUp(const MouseEvent &event)
{
	// Send velocity table to controller
	sendVelocityTableToController();

	drawedLine.clear();
	repaint();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VelocityCurveDlg" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="640" initialHeight="320">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <LABEL name="lblDescription" id="e1affcc7a142cab2" memberName="lblDescription"
         virtualName="" explicitFocusOrder="0" pos="8 8 416 32" edTextCol="ff000000"
         edBkgCol="0" labelText="Click with the mouse in the graphics to draw the velocity curve."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="buttonSendAll" id="71e432722656a5b7" memberName="buttonSendAll"
              virtualName="" explicitFocusOrder="0" pos="168 288 150 24" tooltip="Send whole velocity curve map to controller and save it there."
              buttonText="Send &amp; Save All" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="buttonDiscard" id="8943d46ddc434616" memberName="buttonDiscard"
              virtualName="" explicitFocusOrder="0" pos="328 288 150 24" tooltip="Discard velocity curve edits on controller."
              buttonText="Discard Edits" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="buttonSaveEdits" id="b3ed9064acdde93" memberName="buttonSaveEdits"
              virtualName="" explicitFocusOrder="0" pos="8 288 150 24" tooltip="Save velocity curve edits that have been sent on controller"
              buttonText="Save Edits" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
