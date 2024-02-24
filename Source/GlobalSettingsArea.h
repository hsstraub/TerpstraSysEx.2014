/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.9

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "ColourEditComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class GlobalSettingsArea  : public juce::Component,
                            public ChangeListener,
                            public juce::Button::Listener
{
public:
    //==============================================================================
    GlobalSettingsArea ();
    ~GlobalSettingsArea() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void changeListenerCallback(ChangeBroadcaster *source) override;

	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	std::unique_ptr<ColourEditComponent> inactiveMacroButtonColourEdit;
	std::unique_ptr<ColourEditComponent> activeMacroButtonColourEdit;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> lblPresetButtonColours;
    std::unique_ptr<juce::Label> lblColourInactiveMacroButton;
    std::unique_ptr<juce::Label> lblColourActiveMacroButton;
    std::unique_ptr<juce::TextButton> buttonCalibrate;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlobalSettingsArea)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

