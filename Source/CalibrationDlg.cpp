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

#include "CalibrationDlg.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
CalibrationDlg::CalibrationDlg ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    labelInfoText.reset (new juce::Label ("labelINfoText",
                                          TRANS("Click \'Start calibration\' to calibrate. Click \'End calibration\' to stop.")));
    addAndMakeVisible (labelInfoText.get());
    labelInfoText->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelInfoText->setJustificationType (juce::Justification::centredLeft);
    labelInfoText->setEditable (false, false, false);
    labelInfoText->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelInfoText->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelInfoText->setBounds (16, 48, 432, 96);

    btnStart.reset (new juce::TextButton ("btnStart"));
    addAndMakeVisible (btnStart.get());
    btnStart->setButtonText (TRANS("Start calibration"));
    btnStart->addListener (this);

    btnStart->setBounds (16, 176, 144, 24);

    btnStop.reset (new juce::TextButton ("btnStop"));
    addAndMakeVisible (btnStop.get());
    btnStop->setButtonText (TRANS("End calibration"));
    btnStop->addListener (this);

    btnStop->setBounds (304, 176, 144, 24);


    //[UserPreSize]

	// Calibration type selector
	calibrationSelectorTab.reset(new TabbedButtonBar(TabbedButtonBar::Orientation::TabsAtTop));
	addAndMakeVisible(calibrationSelectorTab.get());

	calibrationSelectorTab->addTab(translate("Calibrate Keys"), juce::Colours::lightgrey, 1);
	calibrationSelectorTab->addTab(translate("Calibrate Aftertouch"), juce::Colours::lightgrey, 2);
	calibrationSelectorTab->addTab(translate("Calibrate Modulation Wheel"), juce::Colours::lightgrey, 3);

	calibrationSelectorTab->addChangeListener(this);
    //[/UserPreSize]

    setSize (500, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

CalibrationDlg::~CalibrationDlg()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    labelInfoText = nullptr;
    btnStart = nullptr;
    btnStop = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	calibrationSelectorTab = nullptr;
    //[/Destructor]
}

//==============================================================================
void CalibrationDlg::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void CalibrationDlg::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
	calibrationSelectorTab->setBounds(labelInfoText->getX(), 0, getWidth() - labelInfoText->getX(), labelInfoText->getY() - CURVETABRIMABOVE);
    //[/UserResized]
}

void CalibrationDlg::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnStart.get())
    {
        //[UserButtonCode_btnStart] -- add your button handler code here..

		// Command depending on tab selection
		auto tabSelection = calibrationSelectorTab->getCurrentTabIndex();
		switch (tabSelection)
		{
		case calibrateKeys:
			// ToDO
			break;

		case calibrateAftertouch:
			TerpstraSysExApplication::getApp().getMidiDriver().sendCalibrateAfterTouch();
			break;

		case calibrateModulationWheel:
			// ToDo
			break;

		default:
			jassertfalse;
			break;
		}

        //[/UserButtonCode_btnStart]
    }
    else if (buttonThatWasClicked == btnStop.get())
    {
        //[UserButtonCode_btnStop] -- add your button handler code here..

		// Command depending on tab selection
		auto tabSelection = calibrationSelectorTab->getCurrentTabIndex();
		switch (tabSelection)
		{
		case calibrateKeys:
		case calibrateAftertouch:
		case calibrateModulationWheel:
			// ToDo
			break;
		default:
			jassertfalse;
			break;
		}

        //[/UserButtonCode_btnStop]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void CalibrationDlg::changeListenerCallback(ChangeBroadcaster *source)
{
	if (source == calibrationSelectorTab.get())
	{
		// Instructions depending on tab selection
		String msg;
		auto tabSelection = calibrationSelectorTab->getCurrentTabIndex();
		switch (tabSelection)
		{
		case calibrateKeys:
			msg << translate("Click \'Start calibration\' to calibrate. Hit any key on the Lumatone to stop.");
			labelInfoText->setText(msg, NotificationType::dontSendNotification);
			btnStop->setVisible(false);
			break;

		case calibrateAftertouch:
			msg << translate("Click \'Start calibration\' to calibrate. Hit any key on the Lumatone to stop.");
			labelInfoText->setText(msg, NotificationType::dontSendNotification);
			btnStop->setVisible(false);
			break;

		case calibrateModulationWheel:
			msg << translate("Click \'Start calibration\' to start calibratin, then turn the wheels to their extremities to update the wheel range values.")
				<< newLine
				<< translate("Click \'End calibration\' to stop.");
			labelInfoText->setText(msg, NotificationType::dontSendNotification);
			btnStop->setVisible(true);
			break;
		default:
			jassertfalse;
			break;
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

<JUCER_COMPONENT documentType="Component" className="CalibrationDlg" componentName=""
                 parentClasses="public juce::Component, public ChangeListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="500"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="labelINfoText" id="21d38888763cccbc" memberName="labelInfoText"
         virtualName="" explicitFocusOrder="0" pos="16 48 432 96" edTextCol="ff000000"
         edBkgCol="0" labelText="Click 'Start calibration' to calibrate. Click 'End calibration' to stop."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <TEXTBUTTON name="btnStart" id="b61e736f368865ec" memberName="btnStart" virtualName=""
              explicitFocusOrder="0" pos="16 176 144 24" buttonText="Start calibration"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnStop" id="4f14fb4a331cad95" memberName="btnStop" virtualName=""
              explicitFocusOrder="0" pos="304 176 144 24" buttonText="End calibration"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

