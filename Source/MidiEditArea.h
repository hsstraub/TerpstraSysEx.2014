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
#include "LumatoneEditorLookAndFeel.h"
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
                      public juce::Button::Listener,
                      public juce::ComboBox::Listener
{
public:
    //==============================================================================
    MidiEditArea ();
    ~MidiEditArea() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void lookAndFeelChanged() override;

    // Implementation of Button::Listener
    void buttonClicked(Button* btn) override;

	void onOpenConnectionToDevice();

	// Implementation of TerpstraNidiDriver::Listener
	void midiMessageReceived(const MidiMessage& midiMessage) override;
	void midiMessageSent(const MidiMessage& midiMessage) override {}
	void midiSendQueueSize(int queueSize) override {}
	void generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel) override;

private:

    void setConnectivity(bool isConnected);

public:
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

	HajuErrorVisualizer     errorVisualizer;

    LumatoneEditorLookAndFeel& lookAndFeel;

    std::unique_ptr<Label> lumatoneLabel;

    std::unique_ptr<TextButton> liveEditorBtn;
    std::unique_ptr<TextButton> offlineEditorBtn;

    std::unique_ptr<Label> pleaseConnectLabel;
    std::unique_ptr<Label> offlineMsgLabel;

    std::unique_ptr<Component> logomark;
    Path logomarkPath;

    //==============================================================================
    // Helpers

    bool connectedToLumatone = false;

    Rectangle<int>   lumatoneLabelBounds;
    Rectangle<float> connectivityArea;
    Rectangle<float> logomarkBounds;
    Rectangle<float> ioBounds;

    const StringArray connectedText = { translate("Disconnected"), translate("Connected") };
    Array<Colour> connectedColours = { Colour(0xffd7002a), Colour(0xff84aea3) };

    //==============================================================================
    // Position & Size constants
    const float lumatoneLabelAreaWidth          = 0.1579f;
    const float lumatoneLabelWidthInArea        = 2.0f / 3.0f;

    const float pleaseConnectX                  = 0.1816f;
    const float pleaseConnectY                  = 0.325f;
    const float pleaseConnectHeight             = 0.17f;

    const float connectionDirectionsX           = 0.1812;
    const float connectionDirectionsY           = 0.51f;
    const float connectionDirectionsHeight      = 0.17f;

    const float editModeX                       = 2.0f / 75.0f;
    const float editModeHeight                  = 0.2394f;

    const float editModeButtonX                 = 0.10606f;
    const float editModeButtonHeight            = 0.4194f;
    const float editModeFontScalar              = 3.0f / 7.0f;

    const float liveEditButtonWidth             = 0.0804f;
    const float offlineEditButtonWidth          = 0.0862f;

    const float controlBoundsX                  = 0.56;
    const float controlBoundsY                  = 2.0f / 7.0f;
    const float controlBoundsWidth              = 0.3684f;
    const float controlBoundsHeight             = 0.4375f;
    const float controlBoundsCornerRadius       = 0.1273f;

    const float  midiInputControlBoundsX        = 0.0625f;
    const float midiOutputControlBoundsX        = 0.4138f;
    const float midiDeviceControlBoundsWidth    = 0.2955f;
    const float midiDeviceControlBoundsHeight   = 0.66f;

    const float disconnectedAreaX               = 0.5f;
    const float connectedAreaX                  = 0.8387f;

    const float disconnectedControlBoundsX      = 0.7632f;
    const float connectedX                      = 0.871f;
    const float connectivityHeight              = 0.1957f;

    const float logomarkX                       = 0.9559f;
    const float logomarkY                       = 2.0f / 9.0f;
    const float logomarkHeight                  = 0.5f;
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

