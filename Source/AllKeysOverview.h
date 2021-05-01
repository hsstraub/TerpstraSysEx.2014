/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.8

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>

#include "LumatoneController.h"
#include "HexagonTilingGeometry.h"

#include "ImageResampling/ImageResampler.h"
#include "BoardGeometry.h"
#include "LumatoneController.h"


// Representation of a key inside the overview
class KeyMiniDisplayInsideAllKeysOverview : public Component, public LumatoneController::MidiListener
{
public:
	KeyMiniDisplayInsideAllKeysOverview(int newBoardIndex, int newKeyIndex);
	~KeyMiniDisplayInsideAllKeysOverview();

	void paint(Graphics&) override;
	void resized() override;
	void mouseDown(const MouseEvent& e) override;
	void mouseUp(const juce::MouseEvent& e) override;


	void setKeyGraphics(Image& colourGraphicIn, Image& shadowGraphicIn);

	// Implementation of TerpstraNidiDriver::Listener
	//void midiMessageReceived(const MidiMessage& midiMessage) override;
	//void midiMessageSent(const MidiMessage& midiMessage) override {}
	//void midiSendQueueSize(int queueSize) override {}
	//void generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel) override {}
	void handleMidiMessage(const MidiMessage& msg) override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyMiniDisplayInsideAllKeysOverview)

	const TerpstraKey* getKeyData() const;
	Colour getKeyColour() const;

	int boardIndex = -1;
	int keyIndex = -1;
	bool isHighlighted = false;

	Image* colourGraphic = nullptr;
	Image* shadowGraphic = nullptr;

	//DEBUG
	Colour keyColour;
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
                         public LumatoneController::FirmwareListener,
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

    void showDeveloperMode(bool developerModeOn);

    void setFirmwareVersion(FirmwareVersion versionIn);

	void lookAndFeelChanged() override;

	void resetOctaveSize();
	void firmwareRevisionReceived(int major, int minor, int revision) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.


private:

	struct OctaveBoard
	{
		OwnedArray<KeyMiniDisplayInsideAllKeysOverview>	keyMiniDisplay;
		int leftPos;
		int rightPos;
	};

	OwnedArray<OctaveBoard> octaveBoards;

	int			currentOctaveSize = 0;
	int			currentSetSelection;

	HexagonTilingGeometry tilingGeometry;

	Image keyColourLayer;
	Image keyShadowLayer;

    std::unique_ptr<Label> lblFirmwareVersion;

	//==============================================================================
	// Style helpers

	std::unique_ptr<ImageProcessor> imageProcessor;

	Rectangle<int> lumatoneBounds;
	int octaveLineY = 0;

	Image lumatoneGraphic;
	Image keyShapeGraphic;
	Image keyShadowGraphic;

	//==============================================================================
	// Position and sizing constants in reference to parent bounds

	const float imageAspect = 2.498233f;
	const float imageY      = 1.0f / 7.0f;
	const float imageHeight = 5.0f / 7.0f;

	const float importYFromImageTop = 0.0752688f;
	const float importH             = 0.0526882f;
	const float importW             = 0.132f;

	const float btnYFromImageTop    = 0.04172043f;
	const float saveLoadW           = 0.07416f;
	const float saveLoadH           = 0.0537634f;
	const float saveloadMarginW     = 0.0034f;

	const float filenameBaselineY   = 0.1221505f;

	const float octaveLineYRatio    = 0.0236559f;

	// In reference to lumatoneBounds
	const float keybedX = 0.06908748f;

	const float keyW = 0.027352f;
	const float keyH = 0.07307f;

	const float oct1Key1X = 0.0839425f;
	const float oct1Key1Y = 0.335887f;

	const float oct1Key56X = 0.27304881f;
	const float oct1Key56Y = 0.8314673f;

	const float oct5Key7X = 0.878802f;
	const float oct5Key7Y = 0.356511491f;

	//===============================================================================

	Point<float>  oct1Key1;
	Point<float> oct1Key56;
	Point<float>  oct5Key7;

	// Geometry settings
	TerpstraBoardGeometry	boardGeometry;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextButton> btnLoadFile;
    std::unique_ptr<juce::TextButton> btnSaveFile;
    std::unique_ptr<juce::TextButton> buttonReceive;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AllKeysOverview)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

