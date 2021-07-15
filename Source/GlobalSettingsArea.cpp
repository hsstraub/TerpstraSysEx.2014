/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.8

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "Main.h"
#include "Settings/CalibrationDlg.h"
//[/Headers]

#include "GlobalSettingsArea.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GlobalSettingsArea::GlobalSettingsArea ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    lblPresetButtonColours.reset (new juce::Label ("lblPresetButtonColours",
                                                   TRANS("Preset Button Colours:")));
    addAndMakeVisible (lblPresetButtonColours.get());
    lblPresetButtonColours->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblPresetButtonColours->setJustificationType (juce::Justification::centredLeft);
    lblPresetButtonColours->setEditable (false, false, false);
    lblPresetButtonColours->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblPresetButtonColours->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblPresetButtonColours->setBounds (0, 8, 150, 24);

    lblColourInactiveMacroButton.reset (new juce::Label ("lblColourInactiveMacroButton",
                                                         TRANS("inactive")));
    addAndMakeVisible (lblColourInactiveMacroButton.get());
    lblColourInactiveMacroButton->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblColourInactiveMacroButton->setJustificationType (juce::Justification::centredLeft);
    lblColourInactiveMacroButton->setEditable (false, false, false);
    lblColourInactiveMacroButton->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblColourInactiveMacroButton->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblColourInactiveMacroButton->setBounds (152, 32, 64, 24);

    lblColourActiveMacroButton.reset (new juce::Label ("lblColourActiveMacroButton",
                                                       TRANS("active")));
    addAndMakeVisible (lblColourActiveMacroButton.get());
    lblColourActiveMacroButton->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblColourActiveMacroButton->setJustificationType (juce::Justification::centredLeft);
    lblColourActiveMacroButton->setEditable (false, false, false);
    lblColourActiveMacroButton->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblColourActiveMacroButton->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblColourActiveMacroButton->setBounds (152, 8, 56, 24);

    buttonCalibrate.reset (new juce::TextButton ("buttonCalibrate"));
    addAndMakeVisible (buttonCalibrate.get());
    buttonCalibrate->setTooltip (TRANS("Show controls for calibration, setting controller MIDI channels, and updating firmware"));
    buttonCalibrate->setButtonText (TRANS("Settings"));
    buttonCalibrate->addListener (this);

    buttonCalibrate->setBounds (336, 8, 112, 24);


    //[UserPreSize]
    activeMacroButtonColourEdit.reset(new ColourEditComponent());
    addAndMakeVisible(activeMacroButtonColourEdit.get());
    activeMacroButtonColourEdit->addChangeListener(this);

    inactiveMacroButtonColourEdit.reset(new ColourEditComponent());
    addAndMakeVisible(inactiveMacroButtonColourEdit.get());
    inactiveMacroButtonColourEdit->addChangeListener(this);

    lblDeveloperMode.reset(new Label("DeveloperModeLabel", "Developer Mode"));
    addChildComponent(lblDeveloperMode.get());
    setDeveloperMode(TerpstraSysExApplication::getApp().getPropertiesFile()->getBoolValue("DeveloperMode", false));


    lblPresetButtonColours->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::UniviaProBold));
    lblColourActiveMacroButton->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::GothamNarrowMedium));
    lblPresetButtonColours->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::UniviaProBold));


    TerpstraSysExApplication::getApp().getLumatoneController().addStatusListener(this);

    buttonCalibrate->setEnabled(false);

    /* We don't want a resize here
    /*
    //[/UserPreSize]

    setSize (456, 64);


    //[Constructor] You can add your own custom stuff here..
    */


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
    if (settingsAreOpen)
    {
        settingsDialog->exitModalState(0);
        settingsDialog = nullptr;
    }
    //[/Destructor]
}

//==============================================================================
void GlobalSettingsArea::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

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
    buttonCalibrate->setTopRightPosition(getWidth(), roundToInt((getHeight() - buttonCalibrate->getHeight()) * 0.5f));

    float margin = roundToInt(getHeight() * 0.1f);
    float colourEditHeight = proportionOfHeight(controlsHeight);
    float controlY = proportionOfHeight((1 - controlsHeight) / 2.0f);
    float colourButtonWidth = colourEditHeight * colourButtonAspect;
    Font colourLabelsFont = TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::FranklinGothic, colourEditHeight * 1.1f);

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

    resizeLabelWithHeight(lblDeveloperMode.get(), getHeight());
    lblDeveloperMode->setCentrePosition(getLocalBounds().getCentre());

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

		auto settingsComponent = new SettingsContainer();
        settingsComponent->setLookAndFeel(&getLookAndFeel());
        settingsComponent->addChangeListener(this);

		DialogWindow::LaunchOptions launchOptions;
		launchOptions.content.setOwned(settingsComponent);
		launchOptions.content->setSize(480, 240);

		launchOptions.dialogTitle = "Settings";
		launchOptions.escapeKeyTriggersCloseButton = true;
		launchOptions.useNativeTitleBar = false;
		launchOptions.resizable = false;

        launchOptions.dialogBackgroundColour = Colour();

		settingsDialog = launchOptions.launchAsync();
        settingsDialog->centreWithSize(548, 240);
        settingsDialog->setLookAndFeel(&TerpstraSysExApplication::getApp().getLookAndFeel().compactWindowStyle);

        settingsAreOpen = true;
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
		TerpstraSysExApplication::getApp().getLumatoneController().sendMacroButtonInactiveColour(inactiveMacroButtonColour);
	}
	else if (source == activeMacroButtonColourEdit.get())
	{
		String activeMacroButtonColour = activeMacroButtonColourEdit->getColourAsString();
		TerpstraSysExApplication::getApp().getLumatoneController().sendMacroButtonActiveColour(activeMacroButtonColour);
	}
    else
    {
        auto settings = dynamic_cast<SettingsContainer*>(source);
        if (settings != nullptr)
        {
            // Settings dialog was exited
            settingsAreOpen = false;
        }
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

void GlobalSettingsArea::setDeveloperMode(bool devModeOn)
{
    showDeveloperMode = devModeOn;
    lblDeveloperMode->setVisible(showDeveloperMode);
    if (devModeOn)
        buttonCalibrate->setEnabled(true);
    repaint();
}

void GlobalSettingsArea::connectionEstablished(int inputDevice, int outputDevice)
{
    buttonCalibrate->setEnabled(true);
}

void GlobalSettingsArea::connectionLost()
{
    buttonCalibrate->setEnabled(false);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GlobalSettingsArea" componentName=""
                 parentClasses="public juce::Component, public ChangeListener, public LumatoneController::StatusListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="456"
                 initialHeight="64">
  <BACKGROUND backgroundColour="0"/>
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
              virtualName="" explicitFocusOrder="0" pos="336 8 112 24" tooltip="Show controls for calibration, setting controller MIDI channels, and updating firmware"
              buttonText="Settings" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

