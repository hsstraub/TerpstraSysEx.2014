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
	Path hexPath;
	bool isHighlighted = false;

	Image* colourGraphic = nullptr;
	Image* shadowGraphic = nullptr;
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

	void setKeysMargin(float marginBoundsHeightRatio);
	void setKeysAngle(float rotationAngle);


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
	const float keybedX = 0.069286f;
	const float keybedY = 0.2964286f;
	const float keybedWidth = 0.875714286f;
	const float keybedHeight = 0.57857143f;
	const float keybedBottomX = 0.945f;
	const float keybedBottomY = 0.875f;

	const float firstKeyX = 0.0842857f;
	const float firstKeyY = 0.3357143f;

	const float lastKeyX = 0.93f;
	const float lastKeyY = 0.83036f;

	//===============================================================================

	float keyMarginToRadius= 0.091f;
	float keyRotationAngle = -0.27850f;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextButton> btnLoadFile;
    std::unique_ptr<juce::TextButton> btnSaveFile;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AllKeysOverview)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

