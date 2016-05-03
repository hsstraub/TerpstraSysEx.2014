/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_D02B34978EAB2CCC__
#define __JUCE_HEADER_D02B34978EAB2CCC__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "TerpstraMidiDriver.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MidiEditArea  : public Component,
                      public ComboBoxListener,
                      public ButtonListener
{
public:
    //==============================================================================
    MidiEditArea ();
    ~MidiEditArea();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void setMidiDriver(TerpstraMidiDriver* driver) { midiDriver = driver;  }
	bool isSendAllButton(Component* subcomponent) { return subcomponent == buttonSendAll; }
	void addSendAllButtonListener(ButtonListener* listener) { buttonSendAll->addListener(listener); }
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	TerpstraMidiDriver*		midiDriver;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> lblMidiInput;
    ScopedPointer<ComboBox> cbMidiInput;
    ScopedPointer<Label> lblMidiOutput;
    ScopedPointer<ComboBox> cbMidiOutput;
    ScopedPointer<ToggleButton> toggleAutoSave;
    ScopedPointer<TextButton> buttonSendSaveEdits;
    ScopedPointer<TextButton> buttonSendAll;
    ScopedPointer<TextButton> buttonDiscard;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiEditArea)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_D02B34978EAB2CCC__
