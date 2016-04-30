/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_B85945BB76A8470__
#define __JUCE_HEADER_B85945BB76A8470__

//[Headers]     -- You can add your own extra header files here --
#include "../../JuceLibraryCode/JuceHeader.h"
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
                              public ButtonListener,
                              public ComboBoxListener
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



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	IncrMidiNotesMappingLogic	mappingLogic;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ToggleButton> channelAutoIncrButton;
    ScopedPointer<ComboBox> channelAutoIncrNoteBox;
    ScopedPointer<Label> labelMidiNotesUntil;
    ScopedPointer<ToggleButton> singleChannelButton;
    ScopedPointer<ComboBox> channelBox;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IncrMidiNotesMapping)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_B85945BB76A8470__
