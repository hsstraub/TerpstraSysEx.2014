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
#include "JuceHeader.h"
#include "TerpstraMidiDriver.h"
#include "HajuLib/HajuErrorVisualizer.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MidiEditArea  : public Component,
                      public TerpstraMidiDriver::Listener,
                      public ChangeListener,
                      public juce::ComboBox::Listener
{
public:
    //==============================================================================
    MidiEditArea ();
    ~MidiEditArea() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void onOpenConnectionToDevice();
	void requestConfigurationFromDevice();

	// Implementation of ChangeListener
	void changeListenerCallback(ChangeBroadcaster *source) override;

	// Implementation of TerpstraNidiDriver::Listener
	void midiMessageReceived(const MidiMessage& midiMessage) override;
	void midiMessageSent(const MidiMessage& midiMessage) override {}
	void midiSendQueueSize(int queueSize) override {}
	void generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	// Indices of the editModeSelector tab
	enum midiEditMode
	{
		liveEditor = 0,
		offlineEditor = 1
	};

	std::unique_ptr<TabbedButtonBar> editModeSelector;
	HajuErrorVisualizer     errorVisualizer;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::ComboBox> cbMidiInput;
    std::unique_ptr<juce::ComboBox> cbMidiOutput;
    std::unique_ptr<juce::Label> lblConnectionState;
    std::unique_ptr<juce::Label> lblEditMode;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiEditArea)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

