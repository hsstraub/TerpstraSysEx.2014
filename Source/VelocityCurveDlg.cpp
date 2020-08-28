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
{
    //[Constructor_pre] You can add your own custom stuff here..
    velocityCurveType = typeValue;

    lookupTableSubDlg.reset(new VelocityCurveSubDlg(velocityCurveType));
    addAndMakeVisible (lookupTableSubDlg.get());
    //[/Constructor_pre]

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

    buttonDiscard->setBounds (464, 392, 144, 24);

    buttonSaveEdits.reset (new TextButton ("buttonSaveEdits"));
    addAndMakeVisible (buttonSaveEdits.get());
    buttonSaveEdits->setTooltip (TRANS("Save velocity curve edits that have been sent on controller"));
    buttonSaveEdits->setButtonText (TRANS("Save Edits"));
    buttonSaveEdits->addListener (this);

    buttonSaveEdits->setBounds (160, 392, 144, 24);

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

    buttonCalibrate->setBounds (616, 392, 144, 24);

    buttonAfterTouchActive.reset (new ToggleButton ("buttonAfterTouchActive"));
    addAndMakeVisible (buttonAfterTouchActive.get());
    buttonAfterTouchActive->setButtonText (TRANS("Aftertouch active"));
    buttonAfterTouchActive->addListener (this);

    buttonAfterTouchActive->setBounds (144, 0, 144, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (768, 424);


    //[Constructor] You can add your own custom stuff here..

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

	lookupTableSubDlg = nullptr;
    //[/Destructor_pre]

    buttonSendAll = nullptr;
    buttonDiscard = nullptr;
    buttonSaveEdits = nullptr;
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
    //[/UserPaint]
}

void VelocityCurveDlg::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    int w = this->getWidth();
    int h = this->getHeight();
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    int lookupTableYPos = buttonAfterTouchActive->getBottom();
	int buttonYPos = h - pushButtonAreaHeight + (pushButtonAreaHeight - buttonSendAll->getHeight())/2;

	lookupTableSubDlg->setBounds(0, lookupTableYPos, w, buttonYPos-lookupTableYPos);

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
		lookupTableSubDlg->sendVelocityTableToController();

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
        // ToDo in case of OnOffVelocityCurve: interval table config request

        TerpstraSysExApplication::getApp().getMidiDriver().sendVelocityConfigurationRequest(velocityCurveType);
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

	lookupTableSubDlg->restoreStateFromPropertiesString(propertiesString);

    switch(velocityCurveType)
    {
        case TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff:
            setSize(
                propertiesFile->getIntValue("VelocityCurveWindowWidth", 648),
                propertiesFile->getIntValue("VelocityCurveWindowHeight", 320));
            break;
        case TerpstraMidiDriver::VelocityCurveType::fader:
            setSize(
                propertiesFile->getIntValue("FaderVelocityCurveWindowWidth", 648),
                propertiesFile->getIntValue("FaderVelocityCurveWindowHeight", 320));
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
	String velocityCurveString = lookupTableSubDlg->saveStateToPropertiesString();

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
            propertiesFile->setValue("FaderVelocityCurveWindowWidth", getWidth());
            propertiesFile->setValue("FaderVelocityCurveWindowHeight", getHeight());
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

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VelocityCurveDlg" componentName=""
                 parentClasses="public Component" constructorParams="TerpstraMidiDriver::VelocityCurveType typeValue"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="1" initialWidth="768" initialHeight="424">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <TEXTBUTTON name="buttonSendAll" id="71e432722656a5b7" memberName="buttonSendAll"
              virtualName="" explicitFocusOrder="0" pos="312 392 144 24" tooltip="Send whole velocity curve map to controller and save it there."
              buttonText="Send &amp; Save All" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="buttonDiscard" id="8943d46ddc434616" memberName="buttonDiscard"
              virtualName="" explicitFocusOrder="0" pos="464 392 144 24" tooltip="Discard velocity curve edits on controller."
              buttonText="Discard Edits" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="buttonSaveEdits" id="b3ed9064acdde93" memberName="buttonSaveEdits"
              virtualName="" explicitFocusOrder="0" pos="160 392 144 24" tooltip="Save velocity curve edits that have been sent on controller"
              buttonText="Save Edits" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="buttonReceive" id="5545cd9fc9bd20cb" memberName="buttonReceive"
              virtualName="" explicitFocusOrder="0" pos="8 392 144 24" tooltip="Receive the current configurartion from controller"
              buttonText="Receive" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonCalibrate" id="47242594c34a8de9" memberName="buttonCalibrate"
              virtualName="" explicitFocusOrder="0" pos="616 392 144 24" tooltip="Calibrate aftertouch"
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

