/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

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
                              public Button::Listener,
                              public ComboBox::Listener
{
public:
    //==============================================================================
    IncrMidiNotesMapping ();
    ~IncrMidiNotesMapping();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	MappingLogicBase* getMappingLogic() { return &mappingLogic; }
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void visibilityChanged() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	IncrMidiNotesMappingLogic	mappingLogic;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ToggleButton> channelAutoIncrButton;
    std::unique_ptr<ComboBox> channelAutoIncrNoteBox;
    std::unique_ptr<Label> labelMidiNotesUntil;
    std::unique_ptr<ToggleButton> singleChannelButton;
    std::unique_ptr<ComboBox> channelBox;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IncrMidiNotesMapping)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

