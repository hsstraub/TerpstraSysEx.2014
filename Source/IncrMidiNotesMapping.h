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
#include "../JuceLibraryCode/JuceHeader.h"
#include "MappingLogic.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class IncrMidiNotesMapping  : public Component,
                              public juce::Button::Listener,
                              public juce::ComboBox::Listener
{
public:
    //==============================================================================
    IncrMidiNotesMapping (int& periodSizeReference, ScaleStructure& scaleStructureIn, Array<Colour>& colourTableIn);
    ~IncrMidiNotesMapping() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	MappingLogicBase* getMappingLogic() { return &mappingLogic; }
	void onUpdatePeriodSize();
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void visibilityChanged() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    int& periodSize;
	IncrMidiNotesMappingLogic	mappingLogic;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::ToggleButton> channelAutoIncrButton;
    std::unique_ptr<juce::Label> labelMidiNotesUntil;
    std::unique_ptr<juce::ToggleButton> singleChannelButton;
    std::unique_ptr<juce::ComboBox> channelBox;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IncrMidiNotesMapping)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

