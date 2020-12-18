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
#include <JuceHeader.h>

#include "KeyboardDataStructure.h"
#include "TerpstraMidiDriver.h"


// Representation of a key inside the overview
class KeyMiniDisplayInsideAllKeysOverview : public Component, public TerpstraMidiDriver::Listener
{
public:
	KeyMiniDisplayInsideAllKeysOverview(int newBoardIndex, int newKeyIndex);
	~KeyMiniDisplayInsideAllKeysOverview();

	void paint(Graphics&) override;
	void resized() override;
	void mouseDown(const MouseEvent& e) override;
	void mouseUp(const juce::MouseEvent& e) override;

	// Implementation of TerpstraNidiDriver::Listener
	void midiMessageReceived(const MidiMessage& midiMessage) override;
	void midiMessageSent(const MidiMessage& midiMessage) override {}
	void midiSendQueueSize(int queueSize) override {}
	void generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel) override {}

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyMiniDisplayInsideAllKeysOverview)

	const TerpstraKey* getKeyData() const;
	Colour getKeyColour() const;

	int boardIndex = -1;
	int keyIndex = -1;
	Path hexPath;
	bool isHighlighted = false;
};

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AllKeysOverview  : public juce::Component,
                         public juce::Button::Listener
{
public:
    //==============================================================================
    AllKeysOverview ();
    ~AllKeysOverview() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

	int getCurrentSetSelection() const { return currentSetSelection ;}
	void setCurrentSetSelection(int newSetSelection) { currentSetSelection = newSetSelection; repaint(); }

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	struct OctaveBoard
	{
		std::unique_ptr<KeyMiniDisplayInsideAllKeysOverview>	keyMiniDisplay[TERPSTRABOARDSIZE];
		int leftPos;
		int rightPos;
	};

	OctaveBoard octaveBoards[NUMBEROFBOARDS];

	int			currentSetSelection;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextButton> btnLoadFile;
    std::unique_ptr<juce::TextButton> btnSaveFile;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AllKeysOverview)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

