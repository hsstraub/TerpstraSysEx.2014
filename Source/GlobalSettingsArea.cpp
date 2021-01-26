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
#include "CalibrationDlg.h"
//[/Headers]

#include "GlobalSettingsArea.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GlobalSettingsArea::GlobalSettingsArea ()
    : Component("GlobalSettingsArea")
{
    lblPresetButtonColours.reset(new juce::Label("lblPresetButtonColours", translate("PresetButtonColours")));
    addAndMakeVisible(lblPresetButtonColours.get());
    lblPresetButtonColours->setFont(LumatoneEditorFonts::UniviaProBold());

    activeMacroButtonColourEdit.reset(new ColourEditComponent());
    addAndMakeVisible(activeMacroButtonColourEdit.get());
    activeMacroButtonColourEdit->addChangeListener(this);

    lblColourActiveMacroButton.reset(new juce::Label("lblColourActiveMacroButton", translate("Active")));
    addAndMakeVisible(lblColourActiveMacroButton.get());
    lblColourActiveMacroButton->setFont(LumatoneEditorFonts::GothamNarrowMedium());

	inactiveMacroButtonColourEdit.reset(new ColourEditComponent());
	addAndMakeVisible(inactiveMacroButtonColourEdit.get());
	inactiveMacroButtonColourEdit->addChangeListener(this);

    lblColourInactiveMacroButton.reset(new juce::Label("lblColourInactiveMacroButton", translate("Inactive")));
    addAndMakeVisible(lblColourInactiveMacroButton.get());
    lblPresetButtonColours->setFont(LumatoneEditorFonts::UniviaProBold());


    buttonCalibrate.reset (new juce::TextButton ("buttonCalibrate"));
    addAndMakeVisible (buttonCalibrate.get());
    buttonCalibrate->setTooltip (translate("CalibrateKeys") + " " + translate("Aftertouch"));
    buttonCalibrate->setButtonText (translate("CalibrateKeys"));
    buttonCalibrate->addListener (this);

    //[UserPreSize]
    //[/UserPreSize]

    //[Constructor] You can add your own custom stuff here..
	// Set values according to the properties files
	restoreStateFromPropertiesFile(TerpstraSysExApplication::getApp().getPropertiesFile());
    //[/Constructor]
}

GlobalSettingsArea::~GlobalSettingsArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	inactiveMacroButtonColourEdit = nullptr;
	activeMacroButtonColourEdit = nullptr;
    //[/Destructor_pre]

    lblPresetButtonColours = nullptr;
    lblColourInactiveMacroButton = nullptr;
    lblColourActiveMacroButton = nullptr;
    buttonCalibrate = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GlobalSettingsArea::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GlobalSettingsArea::resized()
{
    //[UserPreResize] Add your own custom resize code here..

    // Build right-to-left

    int calbrateBtnHeight = roundToInt(getHeight() * calibrateHeight);
    int calibrateWidth = getLookAndFeel().getTextButtonWidthToFitText(*buttonCalibrate, calbrateBtnHeight);
    
    buttonCalibrate->setSize(calibrateWidth, calbrateBtnHeight);
    buttonCalibrate->setTopRightPosition(getWidth(), roundToInt((getHeight() - buttonCalibrate->getHeight()) / 2.0f));
    
    float margin = roundToInt(getHeight() * 0.1f);
    float colourEditHeight = proportionOfHeight(controlsHeight);
    float controlY = proportionOfHeight((1 - controlsHeight) / 2.0f);
    float colourButtonWidth = colourEditHeight * colourButtonAspect;
    Font colourLabelsFont = LumatoneEditorFonts::FranklinGothic(colourEditHeight * 1.1f);

    lblColourInactiveMacroButton->setFont(colourLabelsFont);
    resizeLabelWithHeight(lblColourInactiveMacroButton.get(), colourEditHeight);
    lblColourInactiveMacroButton->setTopRightPosition(buttonCalibrate->getX() - margin, controlY);

    inactiveMacroButtonColourEdit->setSize(colourButtonWidth, colourEditHeight);
    inactiveMacroButtonColourEdit->setTopRightPosition(lblColourInactiveMacroButton->getX() - margin, controlY);

    lblColourActiveMacroButton->setFont(colourLabelsFont);
    resizeLabelWithHeight(lblColourActiveMacroButton.get(), colourEditHeight);
    lblColourActiveMacroButton->setTopRightPosition(inactiveMacroButtonColourEdit->getX() - margin, controlY);

    activeMacroButtonColourEdit->setSize(colourButtonWidth, colourEditHeight);
    activeMacroButtonColourEdit->setTopRightPosition(lblColourActiveMacroButton->getX() - margin, controlY);

    resizeLabelWithHeight(lblPresetButtonColours.get(), colourEditHeight);
    lblPresetButtonColours->setTopRightPosition(activeMacroButtonColourEdit->getX() - margin, controlY);
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GlobalSettingsArea::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == buttonCalibrate.get())
    {
        //[UserButtonCode_buttonCalibrate] -- add your button handler code here..

		CalibrationDlg* optionsWindow = new CalibrationDlg();

		DialogWindow::LaunchOptions launchOptions;
		launchOptions.content.setOwned(optionsWindow);
		launchOptions.content->setSize(480, 240);

		launchOptions.dialogTitle = "Calibration";
		launchOptions.escapeKeyTriggersCloseButton = true;
		launchOptions.useNativeTitleBar = false;
		launchOptions.resizable = true;

		DialogWindow* dw = launchOptions.launchAsync();
		dw->centreWithSize(464, 360);

        //[/UserButtonCode_buttonCalibrate]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void GlobalSettingsArea::lookAndFeelChanged()
{
    lblPresetButtonColours->setColour(Label::ColourIds::textColourId, getLookAndFeel().findColour(LumatoneEditorColourIDs::LabelPink));

    lblColourActiveMacroButton->setColour(Label::ColourIds::textColourId, getLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    lblColourInactiveMacroButton->setColour(Label::ColourIds::textColourId, getLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));

    buttonCalibrate->setColour(TextButton::ColourIds::buttonColourId, Colour(0xff383b3d));
    buttonCalibrate->setColour(TextButton::ColourIds::textColourOffId, Colour(0xffffffff));
}

void GlobalSettingsArea::changeListenerCallback(ChangeBroadcaster *source)
{
	if (source == inactiveMacroButtonColourEdit.get())
	{
		String inactiveMacroButtonColour = inactiveMacroButtonColourEdit->getColourAsString();
		TerpstraSysExApplication::getApp().getMidiDriver().sendMacroButtonInactiveColour(inactiveMacroButtonColour);
	}
	else if (source == activeMacroButtonColourEdit.get())
	{
		String activeMacroButtonColour = activeMacroButtonColourEdit->getColourAsString();
		TerpstraSysExApplication::getApp().getMidiDriver().sendMacroButtonActiveColour(activeMacroButtonColour);
	}
}

void GlobalSettingsArea::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	inactiveMacroButtonColourEdit->setColour(
		propertiesFile->getValue("InactiveMacroButtonColour", "000000"));

	activeMacroButtonColourEdit->setColour(
		propertiesFile->getValue("ActiveMacroButtonColour", "FFFFFF"));
}

void GlobalSettingsArea::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	String inactiveMacroButtonColour = inactiveMacroButtonColourEdit->getColourAsString();
	propertiesFile->setValue("InactiveMacroButtonColour", inactiveMacroButtonColour);

	String activeMacroButtonColour = activeMacroButtonColourEdit->getColourAsString();
	propertiesFile->setValue("ActiveMacroButtonColour", activeMacroButtonColour);
}

void GlobalSettingsArea::listenToColourEditButtons(Button::Listener* listenerIn)
{
    inactiveMacroButtonColourEdit->addListener(listenerIn);
    activeMacroButtonColourEdit->addListener(listenerIn);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GlobalSettingsArea" componentName=""
                 parentClasses="public juce::Component, public ChangeListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="456"
                 initialHeight="64">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="lblPresetButtonColours" id="988da53c99fc73f2" memberName="lblPresetButtonColours"
         virtualName="" explicitFocusOrder="0" pos="0 8 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Preset Button Colours:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblColourInactiveMacroButton" id="9c5cddfcc2966280" memberName="lblColourInactiveMacroButton"
         virtualName="" explicitFocusOrder="0" pos="152 32 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="inactive" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblColourActiveMacroButton" id="79f93468a20f174" memberName="lblColourActiveMacroButton"
         virtualName="" explicitFocusOrder="0" pos="152 8 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="active" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="buttonCalibrate" id="47242594c34a8de9" memberName="buttonCalibrate"
              virtualName="" explicitFocusOrder="0" pos="336 8 112 24" tooltip="Calibrate keys aftertouch"
              buttonText="Calibrate Keys" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

