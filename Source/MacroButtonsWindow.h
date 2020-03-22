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

#include "OneMacroButtonEdit.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MacroButtonsWindow  : public Component,
                            public MidiInputCallback,
                            public Button::Listener
{
public:
    //==============================================================================
    MacroButtonsWindow ();
    ~MacroButtonsWindow();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

	// MIDI input callback
	void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override;

	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.

	// Sub-components for the buttons
	std::unique_ptr<OneMacroButtonEdit>	buttonComponents[10];

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ToggleButton> btnEnableMacroButtons;
    std::unique_ptr<Label> lblMacroButtonsInfo;
    std::unique_ptr<Drawable> drawable1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MacroButtonsWindow)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

