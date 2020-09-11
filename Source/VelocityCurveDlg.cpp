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
    : velocityCurveType(typeValue)
{
    //[Constructor_pre] You can add your own custom stuff here..
    intervalTableSubDlg.reset(new VelocityIntervalTableSubDlg());
    addAndMakeVisible (intervalTableSubDlg.get());

    lookupTableSubDlg.reset(new VelocityCurveSubDlg(velocityCurveType, 127));
    addAndMakeVisible (lookupTableSubDlg.get());
    //[/Constructor_pre]

    buttonSendAll.reset (new TextButton ("buttonSendAll"));
    addAndMakeVisible (buttonSendAll.get());
    buttonSendAll->setTooltip (TRANS("Send whole velocity curve map to controller and save it there."));
    buttonSendAll->setButtonText (TRANS("Send & Save All"));
    buttonSendAll->addListener (this);

    buttonSendAll->setBounds (320, 392, 144, 24);

    buttonDiscard.reset (new TextButton ("buttonDiscard"));
    addAndMakeVisible (buttonDiscard.get());
    buttonDiscard->setTooltip (TRANS("Discard velocity curve edits on controller."));
    buttonDiscard->setButtonText (TRANS("Discard Edits"));
    buttonDiscard->addListener (this);

    buttonDiscard->setBounds (472, 392, 144, 24);

    buttonSaveEdits.reset (new TextButton ("buttonSaveEdits"));
    addAndMakeVisible (buttonSaveEdits.get());
    buttonSaveEdits->setTooltip (TRANS("Save velocity curve edits that have been sent on controller"));
    buttonSaveEdits->setButtonText (TRANS("Save Edits"));
    buttonSaveEdits->addListener (this);

    buttonSaveEdits->setBounds (168, 392, 144, 24);

    buttonReceive.reset (new TextButton ("buttonReceive"));
    addAndMakeVisible (buttonReceive.get());
    buttonReceive->setTooltip (TRANS("Receive the current configurartion from controller"));
    buttonReceive->setButtonText (TRANS("Receive"));
    buttonReceive->addListener (this);

    buttonReceive->setBounds (16, 392, 144, 24);

    buttonCalibrate.reset (new TextButton ("buttonCalibrate"));
    addAndMakeVisible (buttonCalibrate.get());
    buttonCalibrate->setTooltip (TRANS("Calibrate aftertouch"));
    buttonCalibrate->setButtonText (TRANS("Calibrate"));
    buttonCalibrate->addListener (this);

    buttonCalibrate->setBounds (624, 392, 144, 24);

    buttonAfterTouchActive.reset (new ToggleButton ("buttonAfterTouchActive"));
    addAndMakeVisible (buttonAfterTouchActive.get());
    buttonAfterTouchActive->setButtonText (TRANS("Aftertouch active"));
    buttonAfterTouchActive->addListener (this);

    buttonAfterTouchActive->setBounds (144, 0, 144, 24);

    labelIntervalTable.reset (new Label ("labelIntervalTable",
                                         TRANS("Velocity interval table:")));
    addAndMakeVisible (labelIntervalTable.get());
    labelIntervalTable->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelIntervalTable->setJustificationType (Justification::centredLeft);
    labelIntervalTable->setEditable (false, false, false);
    labelIntervalTable->setColour (TextEditor::textColourId, Colours::black);
    labelIntervalTable->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelIntervalTable->setBounds (16, 24, 256, 24);

    labelLookupTable.reset (new Label ("labelLookupTable",
                                       TRANS("Velocity lookup table:")));
    addAndMakeVisible (labelLookupTable.get());
    labelLookupTable->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelLookupTable->setJustificationType (Justification::centredLeft);
    labelLookupTable->setEditable (false, false, false);
    labelLookupTable->setColour (TextEditor::textColourId, Colours::black);
    labelLookupTable->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelLookupTable->setBounds (16, 192, 256, 24);


    //[UserPreSize]

    // Show velocity interval table and table labels: only in case of note on/off velocity table
    if (velocityCurveType == TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff)
    {
        labelIntervalTable->setVisible(true);
        labelLookupTable->setVisible(true);

        intervalTableSubDlg->setVisible(true);
    }
    else
    {
        labelIntervalTable->setVisible(false);
        labelLookupTable->setVisible(false);

        intervalTableSubDlg->setVisible(false);
    }

	// Calibrate button: only for aftertouch
	buttonAfterTouchActive->setVisible(velocityCurveType == TerpstraMidiDriver::VelocityCurveType::afterTouch);
	buttonCalibrate->setVisible(velocityCurveType == TerpstraMidiDriver::VelocityCurveType::afterTouch);

    //[/UserPreSize]

    setSize (768, 424);


    //[Constructor] You can add your own custom stuff here..

	// Set values according to the properties files
	restoreStateFromPropertiesFile(TerpstraSysExApplication::getApp().getPropertiesFile());
    //[/Constructor]
}

VelocityCurveDlg::~VelocityCurveDlg()
{
    //[Destructor_pre]. You can add your own custom destruction code here..

	// Save values to properties file
	saveStateToPropertiesFile(TerpstraSysExApplication::getApp().getPropertiesFile());

	intervalTableSubDlg = nullptr;
	lookupTableSubDlg = nullptr;
    //[/Destructor_pre]

    buttonSendAll = nullptr;
    buttonDiscard = nullptr;
    buttonSaveEdits = nullptr;
    buttonReceive = nullptr;
    buttonCalibrate = nullptr;
    buttonAfterTouchActive = nullptr;
    labelIntervalTable = nullptr;
    labelLookupTable = nullptr;


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
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    int w = this->getWidth();
    int h = this->getHeight();

    int tableAreaBottom = h - pushButtonAreaHeight;
	int buttonYPos = tableAreaBottom + (pushButtonAreaHeight - buttonSendAll->getHeight())/2;

    int lookupTableYPos;
    if (velocityCurveType == TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff)
    {
        int intervalTableYPos = labelIntervalTable->getBottom();

        labelLookupTable->setBounds(labelLookupTable->getX(), intervalTableYPos + (tableAreaBottom - intervalTableYPos)/2,
            labelLookupTable->getWidth(), labelLookupTable->getHeight());

        lookupTableYPos = labelLookupTable->getBottom();

        intervalTableSubDlg->setBounds(0, intervalTableYPos, w, labelLookupTable->getY() - intervalTableYPos);
    }
    else if (velocityCurveType == TerpstraMidiDriver::VelocityCurveType::afterTouch)
    {
        lookupTableYPos = buttonAfterTouchActive->getBottom();
    }
    else
    {
        lookupTableYPos = 0;
    }

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

		if (velocityCurveType == TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff)
        {
            intervalTableSubDlg->sendVelocityTableToController();
        }
        else if (velocityCurveType == TerpstraMidiDriver::VelocityCurveType::afterTouch)
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
        if (velocityCurveType == TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff)
        {
            TerpstraSysExApplication::getApp().getMidiDriver().sendVelocityIntervalConfigRequest();
        }

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
    switch(velocityCurveType)
    {
        case TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff:
            intervalTableSubDlg->restoreStateFromPropertiesString(propertiesFile->getValue("NoteOnOffVelocityIntervalTable"));
            lookupTableSubDlg->restoreStateFromPropertiesString(propertiesFile->getValue("NoteOnOffVelocityCurveTable"));
 //           setSize(
 //               propertiesFile->getIntValue("VelocityCurveWindowWidth", 648),
 //               propertiesFile->getIntValue("VelocityCurveWindowHeight", 480));
            break;
        case TerpstraMidiDriver::VelocityCurveType::fader:
            lookupTableSubDlg->restoreStateFromPropertiesString(propertiesFile->getValue("FaderVelocityCurveTable"));
 //           setSize(
 //               propertiesFile->getIntValue("FaderVelocityCurveWindowWidth", 648),
 //               propertiesFile->getIntValue("FaderVelocityCurveWindowHeight", 320));
            break;
        case TerpstraMidiDriver::VelocityCurveType::afterTouch:
            lookupTableSubDlg->restoreStateFromPropertiesString(propertiesFile->getValue("AfterTouchCurveTable"));
            buttonAfterTouchActive->setToggleState(
                propertiesFile->getBoolValue("AfterTouchActive", false),
                juce::NotificationType::sendNotification);
  //          setSize(
  //              propertiesFile->getIntValue("AftertochVelocityCurveWindowWidth", 768),
  //              propertiesFile->getIntValue("AftertouchVelocityCurveWindowHeight", 320));
            break;
        default:
            jassert(false);
            break;
    }
}

void VelocityCurveDlg::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
    switch(velocityCurveType)
    {
        case TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff:
            propertiesFile->setValue("NoteOnOffVelocityIntervalTable", intervalTableSubDlg->saveStateToPropertiesString());
            propertiesFile->setValue("NoteOnOffVelocityCurveTable", lookupTableSubDlg->saveStateToPropertiesString());
            propertiesFile->setValue("VelocityCurveWindowWidth", getParentComponent()->getWidth());
            propertiesFile->setValue("VelocityCurveWindowHeight", getParentComponent()->getHeight());
            break;
        case TerpstraMidiDriver::VelocityCurveType::fader:
            propertiesFile->setValue("FaderVelocityCurveTable", lookupTableSubDlg->saveStateToPropertiesString());
            propertiesFile->setValue("FaderVelocityCurveWindowWidth", getParentComponent()->getWidth());
            propertiesFile->setValue("FaderVelocityCurveWindowHeight", getParentComponent()->getHeight());
            break;
        case TerpstraMidiDriver::VelocityCurveType::afterTouch:
            propertiesFile->setValue("AfterTouchCurveTable", lookupTableSubDlg->saveStateToPropertiesString());
            propertiesFile->setValue("AfterTouchActive", buttonAfterTouchActive->getToggleState());
            propertiesFile->setValue("AftertouchVelocityCurveWindowWidth", getParentComponent()->getWidth());
            propertiesFile->setValue("AftertouchVelocityCurveWindowHeight", getParentComponent()->getHeight());
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
                 variableInitialisers="velocityCurveType(typeValue)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="1"
                 initialWidth="768" initialHeight="424">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <TEXTBUTTON name="buttonSendAll" id="71e432722656a5b7" memberName="buttonSendAll"
              virtualName="" explicitFocusOrder="0" pos="320 392 144 24" tooltip="Send whole velocity curve map to controller and save it there."
              buttonText="Send &amp; Save All" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="buttonDiscard" id="8943d46ddc434616" memberName="buttonDiscard"
              virtualName="" explicitFocusOrder="0" pos="472 392 144 24" tooltip="Discard velocity curve edits on controller."
              buttonText="Discard Edits" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="buttonSaveEdits" id="b3ed9064acdde93" memberName="buttonSaveEdits"
              virtualName="" explicitFocusOrder="0" pos="168 392 144 24" tooltip="Save velocity curve edits that have been sent on controller"
              buttonText="Save Edits" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="buttonReceive" id="5545cd9fc9bd20cb" memberName="buttonReceive"
              virtualName="" explicitFocusOrder="0" pos="16 392 144 24" tooltip="Receive the current configurartion from controller"
              buttonText="Receive" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonCalibrate" id="47242594c34a8de9" memberName="buttonCalibrate"
              virtualName="" explicitFocusOrder="0" pos="624 392 144 24" tooltip="Calibrate aftertouch"
              buttonText="Calibrate" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="buttonAfterTouchActive" id="3f2eba6027c4f2f5" memberName="buttonAfterTouchActive"
                virtualName="" explicitFocusOrder="0" pos="144 0 144 24" buttonText="Aftertouch active"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="labelIntervalTable" id="8a7af695deb665b1" memberName="labelIntervalTable"
         virtualName="" explicitFocusOrder="0" pos="16 24 256 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Velocity interval table:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="labelLookupTable" id="b922d8fb3b3a5823" memberName="labelLookupTable"
         virtualName="" explicitFocusOrder="0" pos="16 192 256 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Velocity lookup table:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

