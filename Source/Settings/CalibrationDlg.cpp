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
#include "../ViewConstants.h"
#include "../Main.h"
//[/Headers]

#include "CalibrationDlg.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
CalibrationDlg::CalibrationDlg ()
{
    //[Constructor_pre] You can add your own custom stuff here..
	instructionsFont = TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::FranklinGothic);
    //[/Constructor_pre]

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

	updateWheelCalibrationStatus();

	// Calibration type selector
	calibrationSelectorTab.reset(new TabbedButtonBar(TabbedButtonBar::Orientation::TabsAtTop));
	addAndMakeVisible(calibrationSelectorTab.get());

	calibrationSelectorTab->addTab(translate("Keys"), juce::Colours::lightgrey, 1);
	calibrationSelectorTab->addTab(translate("Aftertouch"), juce::Colours::lightgrey, 2);
	calibrationSelectorTab->addTab(translate("Modulation Wheel"), juce::Colours::lightgrey, 3);

	calibrationSelectorTab->addChangeListener(this);

	TerpstraSysExApplication::getApp().getLumatoneController().addFirmwareListener(this);

    //[/UserPreSize]

    setSize (524, 212);


    //[Constructor] You can add your own custom stuff here..
	calibrationSelectorTab->setCurrentTabIndex(0);
	// Force calling the callback routine
	changeListenerCallback(calibrationSelectorTab.get());
    //[/Constructor]
}

CalibrationDlg::~CalibrationDlg()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	TerpstraSysExApplication::getApp().getLumatoneController().removeFirmwareListener(this);
    //[/Destructor_pre]

    btnStart = nullptr;
    btnStop = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	calibrationSelectorTab = nullptr;
	wheelsCalibrationComponent = nullptr;
    //[/Destructor]
}

//==============================================================================
void CalibrationDlg::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(findColour(ResizableWindow::ColourIds::backgroundColourId));

	g.setColour(findColour(DocumentWindow::ColourIds::textColourId));
	g.setFont(instructionsFont);
	g.drawFittedText(instructionText, instructionsBounds, Justification::centred, 2, 1.0f);
    //[/UserPaint]
}

void CalibrationDlg::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
	auto currentTab = calibrationSelectorTab->getCurrentTabIndex();

	calibrationSelectorTab->setBounds(0, 0, getWidth() - generalRim, OCTAVEBOARDTABHEIGHT);

	int buttonWidth = proportionOfWidth(0.3f);
	int buttonHeight = proportionOfHeight(0.125f);

	btnStart->setSize(buttonWidth, buttonHeight);
	btnStart->setTopLeftPosition(generalRim, getHeight() - generalRim - buttonHeight);
	btnStop->setBounds(btnStart->getBounds().withX(getWidth() - generalRim - buttonWidth));

	if (currentTab == calibrateModulationWheel && wheelsCalibrationComponent != nullptr)
	{
		int panelHeight = btnStart->getY() - calibrationSelectorTab->getBottom() - 2 * generalRim;

		wheelsCalibrationComponent->setBounds(
			generalRim,
			calibrationSelectorTab->getBottom() + generalRim,
			proportionOfWidth(wheelsGraphicWidthScalar),
			panelHeight);

		instructionsBounds.setBounds(
			wheelsCalibrationComponent->getRight() + generalRim,
			calibrationSelectorTab->getBottom() + generalRim,
			getWidth() - wheelsCalibrationComponent->getRight() - 2 * generalRim,
			panelHeight);
	}

	else
	{
		instructionsBounds.setBounds(
			generalRim,
			calibrationSelectorTab->getBottom() + generalRim,
			getWidth() - 2 * generalRim,
			btnStart->getY() - calibrationSelectorTab->getBottom() - 2 * generalRim);
		instructionsFont.setHeight(instructionsBounds.getHeight() * fontHeightInBounds);
	}


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
			TerpstraSysExApplication::getApp().getLumatoneController().startCalibrateKeys();
			break;

		case calibrateAftertouch:
			TerpstraSysExApplication::getApp().getLumatoneController().startCalibrateAftertouch();
			break;

		case calibrateModulationWheel:
			TerpstraSysExApplication::getApp().getLumatoneController().setCalibratePitchModWheel(true);

			// Todo - wait for ack
			TerpstraSysExApplication::getApp().setCalibrationMode(true);
			updateWheelCalibrationStatus();
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
			// No function for the stop button in this mode. Whe should never get here.
			jassertfalse;
			break;
		case calibrateModulationWheel:
			TerpstraSysExApplication::getApp().getLumatoneController().setCalibratePitchModWheel(false);
			// Todo - wait for ack
			TerpstraSysExApplication::getApp().setCalibrationMode(false);
			updateWheelCalibrationStatus();
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

void CalibrationDlg::lookAndFeelChanged()
{
    //[UserCode_lookAndFeelChanged] -- Add your code here...
	auto lookAndFeel = dynamic_cast<LumatoneEditorLookAndFeel*>(&getLookAndFeel());
	if (lookAndFeel)
	{
		setColour(ResizableWindow::ColourIds::backgroundColourId, lookAndFeel->findColour(LumatoneEditorColourIDs::LightBackground));
		setColour(DocumentWindow::ColourIds::textColourId, lookAndFeel->findColour(LumatoneEditorColourIDs::InactiveText));
	}
    //[/UserCode_lookAndFeelChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void CalibrationDlg::changeListenerCallback(ChangeBroadcaster *source)
{
	if (source == calibrationSelectorTab.get())
	{
		btnStart->setEnabled(true);
		// Instructions depending on tab selection
		auto tabSelection = calibrationSelectorTab->getCurrentTabIndex();
		switch (tabSelection)
		{
		case calibrateKeys:
			instructionText.clear();
			instructionText << translate("Click \'Start calibration\' to let the keyboard operate in key calibration mode.")
				<< newLine
				<< translate("To return to normal operating state, the five submodule boards must exit out calibration mode by pressing their corresponding macro buttons to save or cancel calibration.");
			btnStop->setVisible(false);
			wheelsCalibrationComponent = nullptr;
			repaint();
			break;

		case calibrateAftertouch:
			instructionText.clear();
			instructionText << translate("Click \'Start calibration\'to let the keyboard operate in aftertouch calibration mode.")
				<< newLine
				<< translate("To return to normal operating state, the five submodule boards must exit out calibration mode by pressing their corresponding macro buttons to save or cancel calibration.");
			btnStop->setVisible(false);
			wheelsCalibrationComponent = nullptr;
			repaint();
			break;

		case calibrateModulationWheel:
			instructionText.clear();
			instructionText << translate("Click \'Start calibration\' to start calibrating, then turn the wheels to their extremities to update the wheel range values.")
				<< newLine
				<< translate("Click \'End calibration\' to stop.");
			btnStop->setVisible(true);
			setupWheelCalibrationLayout();
			updateWheelCalibrationStatus();
			repaint();
			break;
		default:
			jassertfalse;
			break;
		}
	}
}

void CalibrationDlg::setupWheelCalibrationLayout()
{
	FirmwareSupport support;
	if (support.versionAcknowledgesCommand(TerpstraSysExApplication::getApp().getFirmwareVersion(), PERIPHERAL_CALBRATION_DATA))
	{
		wheelsCalibrationComponent.reset(new WheelsCalibrationComponent());
		addAndMakeVisible(wheelsCalibrationComponent.get());
		resized();
	}
}


void CalibrationDlg::updateWheelCalibrationStatus()
{
	bool inCalibration = TerpstraSysExApplication::getApp().getInCalibrationMode();
	btnStart->setEnabled(!inCalibration);
	btnStop->setEnabled(inCalibration);
}

void CalibrationDlg::wheelsCalibrationDataReceived(WheelsCalibrationData calibrationData)
{
	if (wheelsCalibrationComponent != nullptr)
	{
		wheelsCalibrationComponent->updateCalibrationData(calibrationData);
	}
	else
		jassertfalse;
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="CalibrationDlg" componentName=""
                 parentClasses="public juce::Component, public ChangeListener, public LumatoneController::FirmwareListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="1" initialWidth="524"
                 initialHeight="212">
  <METHODS>
    <METHOD name="lookAndFeelChanged()"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44"/>
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

