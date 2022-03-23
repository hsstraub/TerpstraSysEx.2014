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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../LumatoneController.h"
#include "WheelsCalibrationComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class CalibrationDlg  : public juce::Component,
                        public ChangeListener,
                        public LumatoneEditor::FirmwareListener,
                        public juce::Button::Listener
{
public:
    //==============================================================================
    CalibrationDlg ();
    ~CalibrationDlg() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

	// Implementation of ChangeListener
	void changeListenerCallback(ChangeBroadcaster *source) override;

    void setupWheelCalibrationLayout();
    void updateWheelCalibrationStatus();

    //==============================================================================
    // LumatoneEditor::FirmwareListener Implementation

    void calibratePitchModWheelAnswer(TerpstraMIDIAnswerReturnCode code) override;
    
    void wheelsCalibrationDataReceived(WheelsCalibrationData calibrationData) override;


    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void lookAndFeelChanged() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	enum calibrationMode
	{
		calibrateKeys = 0,
		calibrateAftertouch = 1,
		calibrateModulationWheel
	};

	std::unique_ptr<TabbedButtonBar> calibrationSelectorTab;
    std::unique_ptr<WheelsCalibrationComponent> wheelsCalibrationComponent;
    
    bool startCalibration = false;

	String instructionText;

	// Style Helpers
	Rectangle<int> instructionsBounds;
    Font instructionsFont;
	const float fontHeightInBounds = 0.125f;
	const int generalRim = 12;

    const float wheelsGraphicWidthScalar = 0.15f;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextButton> btnStart;
    std::unique_ptr<juce::TextButton> btnStop;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CalibrationDlg)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

