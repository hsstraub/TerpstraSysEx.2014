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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "KeyboardDataStructure.h"
#include "ColourEditComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SingleNoteAssign  : public Component,
                          public juce::ComboBox::Listener,
                          public juce::Button::Listener
{
public:
    //==============================================================================
    SingleNoteAssign ();
    ~SingleNoteAssign() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	bool performMouseDown(int setSelection, int keySelection);
	void onSetData(TerpstraKeyMapping& newData);

	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	std::unique_ptr<ColourEditComponent> colourSubwindow;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> editInstructionText;
    std::unique_ptr<juce::ComboBox> noteBox;
    std::unique_ptr<juce::ToggleButton> noteAutoIncrButton;
    std::unique_ptr<juce::ComboBox> channelBox;
    std::unique_ptr<juce::ToggleButton> channelAutoIncrButton;
    std::unique_ptr<juce::ComboBox> channelAutoIncrNoteBox;
    std::unique_ptr<juce::ToggleButton> setNoteToggleButton;
    std::unique_ptr<juce::ToggleButton> setChannelToggleButton;
    std::unique_ptr<juce::ToggleButton> setColourToggleButton;
    std::unique_ptr<juce::ToggleButton> keyTypeToggleButton;
    std::unique_ptr<juce::ComboBox> keyTypeCombo;
    std::unique_ptr<juce::GroupComponent> juce__groupComponent;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SingleNoteAssign)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

