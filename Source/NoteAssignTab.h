/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_DA54242FB8EF0684__
#define __JUCE_HEADER_DA54242FB8EF0684__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "KeyboardDataStructure.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class NoteAssignTab  : public Component,
                       public ComboBoxListener,
                       public ButtonListener
{
public:
    //==============================================================================
    NoteAssignTab ();
    ~NoteAssignTab();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	TerpstraKey createKeyMapping();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void buttonClicked (Button* buttonThatWasClicked);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<GroupComponent> noteAndChannelAssGroup;
    ScopedPointer<Label> editInstructionText;
    ScopedPointer<Label> noteLabel;
    ScopedPointer<ComboBox> noteBox;
    ScopedPointer<ToggleButton> noteAutoIncrButton;
    ScopedPointer<Label> channelLabel;
    ScopedPointer<ComboBox> channelBox;
    ScopedPointer<ToggleButton> channelAutoIncrButton;
    ScopedPointer<ComboBox> channelAutoIncrNoteBox;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteAssignTab)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_DA54242FB8EF0684__
