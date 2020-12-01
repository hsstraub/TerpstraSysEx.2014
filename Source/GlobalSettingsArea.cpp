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

#include "GlobalSettingsArea.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GlobalSettingsArea::GlobalSettingsArea ()
{
    //[Constructor_pre] You can add your own custom stuff here..
	inactiveMacroButtonColourEdit.reset(new ColourEditComponent());
	addAndMakeVisible(inactiveMacroButtonColourEdit.get());
	inactiveMacroButtonColourEdit->setVisible(true);
	inactiveMacroButtonColourEdit->addChangeListener(this);

	activeMacroButtonColourEdit.reset(new ColourEditComponent());
	addAndMakeVisible(activeMacroButtonColourEdit.get());
	activeMacroButtonColourEdit->setVisible(true);
	activeMacroButtonColourEdit->addChangeListener(this);
    //[/Constructor_pre]

    lblPresetButtonColours.reset (new juce::Label ("lblPresetButtonColours",
                                                   TRANS("Preset Button Colours:")));
    addAndMakeVisible (lblPresetButtonColours.get());
    lblPresetButtonColours->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblPresetButtonColours->setJustificationType (juce::Justification::centredLeft);
    lblPresetButtonColours->setEditable (false, false, false);
    lblPresetButtonColours->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblPresetButtonColours->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblPresetButtonColours->setBounds (0, 0, 150, 24);

    lblColourInactiveMacroButton.reset (new juce::Label ("lblColourInactiveMacroButton",
                                                         TRANS("inactive")));
    addAndMakeVisible (lblColourInactiveMacroButton.get());
    lblColourInactiveMacroButton->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblColourInactiveMacroButton->setJustificationType (juce::Justification::centredLeft);
    lblColourInactiveMacroButton->setEditable (false, false, false);
    lblColourInactiveMacroButton->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblColourInactiveMacroButton->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblColourInactiveMacroButton->setBounds (152, 24, 64, 24);

    lblColourActiveMacroButton.reset (new juce::Label ("lblColourActiveMacroButton",
                                                       TRANS("active")));
    addAndMakeVisible (lblColourActiveMacroButton.get());
    lblColourActiveMacroButton->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblColourActiveMacroButton->setJustificationType (juce::Justification::centredLeft);
    lblColourActiveMacroButton->setEditable (false, false, false);
    lblColourActiveMacroButton->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblColourActiveMacroButton->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblColourActiveMacroButton->setBounds (152, 0, 56, 24);

    buttonCalibrate.reset (new juce::TextButton ("buttonCalibrate"));
    addAndMakeVisible (buttonCalibrate.get());
    buttonCalibrate->setTooltip (TRANS("Calibrate keys aftertouch"));
    buttonCalibrate->setButtonText (TRANS("Calibrate Keys"));
    buttonCalibrate->addListener (this);

    buttonCalibrate->setBounds (344, 8, 112, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (472, 48);


    //[Constructor] You can add your own custom stuff here..
	// Set values according to the properties files
	restoreStateFromPropertiesFile(TerpstraSysExApplication::getApp().getPropertiesFile());
    //[/Constructor]
}

GlobalSettingsArea::~GlobalSettingsArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..

	// Save values to properties file
	saveStateToPropertiesFile(TerpstraSysExApplication::getApp().getPropertiesFile());

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

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GlobalSettingsArea::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	inactiveMacroButtonColourEdit->setBounds(210, 0, 196, 24);
	activeMacroButtonColourEdit->setBounds(210, 24, 196, 24);
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

		// ToDo popup dialog
		TerpstraSysExApplication::getApp().getMidiDriver().sendCalibrateAfterTouch();

        //[/UserButtonCode_buttonCalibrate]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

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
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="472"
                 initialHeight="48">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="lblPresetButtonColours" id="988da53c99fc73f2" memberName="lblPresetButtonColours"
         virtualName="" explicitFocusOrder="0" pos="0 0 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Preset Button Colours:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblColourInactiveMacroButton" id="9c5cddfcc2966280" memberName="lblColourInactiveMacroButton"
         virtualName="" explicitFocusOrder="0" pos="152 24 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="inactive" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblColourActiveMacroButton" id="79f93468a20f174" memberName="lblColourActiveMacroButton"
         virtualName="" explicitFocusOrder="0" pos="152 0 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="active" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="buttonCalibrate" id="47242594c34a8de9" memberName="buttonCalibrate"
              virtualName="" explicitFocusOrder="0" pos="344 8 112 24" tooltip="Calibrate keys aftertouch"
              buttonText="Calibrate Keys" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

