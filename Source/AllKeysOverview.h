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
#include "HexagonTilingGeometry.h"


// Representation of a key inside the overview
class KeyMiniDisplayInsideAllKeysOverview : public Component
{
public:
	KeyMiniDisplayInsideAllKeysOverview(int newBoardIndex, int newKeyIndex);
	~KeyMiniDisplayInsideAllKeysOverview();

	void paint(Graphics&) override;
	void resized() override;
	void mouseDown(const MouseEvent& e) override;
	void mouseUp(const juce::MouseEvent& e) override;

	void setKeyGraphics(Image& colourGraphicIn, Image& shadowGraphicIn);

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
		OwnedArray<KeyMiniDisplayInsideAllKeysOverview>	keyMiniDisplay;
		int leftPos;
		int rightPos;
	};

	OwnedArray<OctaveBoard> octaveBoards;

	int			currentSetSelection;

	HexagonTilingGeometry tilingGeometry;

	//==============================================================================
	// Style helpers

	Rectangle<int> lumatoneBounds;
	Rectangle<float> keybedBounds;
	
	Image lumatoneGraphic;
	Image keyShapeGraphic;
	Image keyShadowGraphic;

	//==============================================================================
	// Position and sizing constants in reference to parent bounds

	const float imageAspect = 2121.0f / 849.0f;
	const float imageY = 1.0f / 7.0f;
	const float imageHeight = 5.0f / 7.0f;

	// In reference to lumatoneBounds
	const float keybedX = 0.06908748f;
	const float keybedY = 0.29522687f;
	
	const float keybedBottomX = 0.9450601f;
	const float keybedBottomY = 0.8744844f;

	const float keyW = 0.027352f;
	const float keyH = 0.07307f;

	// Key centers in reference to lumatoneBounds
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

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextButton> btnLoadFile;
    std::unique_ptr<juce::TextButton> btnSaveFile;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AllKeysOverview)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

