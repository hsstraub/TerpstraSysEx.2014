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
#include <JuceHeader.h>

#include "LumatoneEditorStyleCommon.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PedalSensitivityDlg  : public juce::Component,
                             public LumatoneController::FirmwareListener,
                             public juce::Button::Listener,
                             public juce::Slider::Listener
{
public:
    //==============================================================================
    PedalSensitivityDlg ();
    ~PedalSensitivityDlg() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	// New mapping is loaded. Display data.
	void loadFromMapping();

	void lookAndFeelChanged() override;

    // LumatoneController::FirmwareListener implementation
    void firmwareRevisionReceived(FirmwareVersion version) override;

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    int roundedCornerSize = 0;
    Rectangle<float> expressionBounds;
    Rectangle<float> sustainBounds;
    const float sectionMarginWidth = 0.05f;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> labelExprContrSensitivity;
    std::unique_ptr<juce::ToggleButton> btnInvertExpression;
    std::unique_ptr<juce::Label> lblExpression;
    std::unique_ptr<juce::Slider> sldExprCtrlSensitivity;
    std::unique_ptr<juce::ToggleButton> btnInvertSustain;
    std::unique_ptr<juce::Label> lblSustain;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PedalSensitivityDlg)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

