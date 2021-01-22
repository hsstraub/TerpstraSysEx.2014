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
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	int roundedCornerSize;
    Rectangle<float> controlBounds;

    // Style Constants
    const Rectangle<float> sliderBoundsProps = { 0.54f, 0.38f, 0.4f, 0.52f };

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> labelExprContrSensivity;
    std::unique_ptr<juce::ToggleButton> btnInvertFootCtrl;
    std::unique_ptr<juce::Label> labelEXpressionPedalTitle;
    std::unique_ptr<juce::Slider> sldExprCtrlSensivity;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PedalSensitivityDlg)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

