/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_9AB3AB1026C8BC32__
#define __JUCE_HEADER_9AB3AB1026C8BC32__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "KeyboardDataStructure.h"
#include "ViewComponents.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SingleNoteAssign  : public Component,
                          public ComboBoxListener,
                          public ButtonListener,
                          public ChangeListener
{
public:
    //==============================================================================
    SingleNoteAssign ();
    ~SingleNoteAssign();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void changeListenerCallback(ChangeBroadcaster *source) override;
	void PerformMouseClickEdit(int setSelection, int keySelection);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<GroupComponent> noteAndChannelAssGroup;
    ScopedPointer<Label> editInstructionText;
    ScopedPointer<ComboBox> noteBox;
    ScopedPointer<ToggleButton> noteAutoIncrButton;
    ScopedPointer<ComboBox> channelBox;
    ScopedPointer<ToggleButton> channelAutoIncrButton;
    ScopedPointer<ComboBox> channelAutoIncrNoteBox;
    ScopedPointer<ToggleButton> setNoteToggleButton;
    ScopedPointer<ToggleButton> setChannelToggleButton;
    ScopedPointer<ToggleButton> setColourToggleButton;
    ScopedPointer<TextButton> btnColourPicker;
    ScopedPointer<ColourComboBox> colourCombo;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SingleNoteAssign)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_9AB3AB1026C8BC32__
