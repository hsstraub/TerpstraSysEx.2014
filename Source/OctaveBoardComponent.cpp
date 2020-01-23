/*
  ==============================================================================

    OctaveBoardComponent.cpp
    Created: 11 Jan 2020 6:54:03pm
    Author:  hsstraub

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "OctaveBoardComponent.h"
#include "Main.h"


//==============================================================================
KeyMiniDisplayInsideOctaveBoardComponent::KeyMiniDisplayInsideOctaveBoardComponent()
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.

}

KeyMiniDisplayInsideOctaveBoardComponent::~KeyMiniDisplayInsideOctaveBoardComponent()
{
}

void KeyMiniDisplayInsideOctaveBoardComponent::paint(Graphics& g)
{
	/* This demo code just fills the component's background and
	draws some placeholder text to get you started.

	You should replace everything in this method with your own
	drawing code..
	*/

	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	g.setColour(Colours::white);
	g.setFont(14.0f);
	g.drawText("KeyMiniDisplayInsideOctaveBoardComponent", getLocalBounds(),
		Justification::centred, true);   // draw some placeholder text
}

void KeyMiniDisplayInsideOctaveBoardComponent::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..

}


//==============================================================================
OctaveBoardComponent::OctaveBoardComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

	for (int i = 0; i < TERPSTRABOARDSIZE; i++)
	{
		keyMiniDisplay[i].reset(new KeyMiniDisplayInsideOctaveBoardComponent());
		addAndMakeVisible(keyMiniDisplay[i].get());
	}
}

OctaveBoardComponent::~OctaveBoardComponent()
{
	for (int i = 0; i < TERPSTRABOARDSIZE; i++)
	{
		keyMiniDisplay[i] = nullptr;
	}
}

void OctaveBoardComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour ( isSelected ?  Colours::orange : Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void OctaveBoardComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

	int newHeight = getHeight();
	int newWidth = getWidth();

	// SIngle key field size
	// ToDo better logic
	int newSingleKeySize = jmin(newWidth / 8, newHeight/8);

	// Transformation Rotate slightly counterclockwise
	int x = 0;
	int y = 0;
	AffineTransform transform = AffineTransform::translation(-x, -y);
	transform = transform.rotated(TERPSTRASINGLEKEYROTATIONANGLE);
	transform = transform.translated(x, y);

	int keyIndex = 0;

	// Rows
	int rowCount = boardGeometry.horizontaLineCount();
	for (int rowIndex = 0; rowIndex < rowCount; rowIndex++)
	{
		int xbasepos;
		if (rowIndex % 2 == 0)
			xbasepos = 0;
		else
			xbasepos = 0 + newSingleKeySize / 2;

		int ybasepos = 0 + 3 * rowIndex * newSingleKeySize / 4;

		int subBoardRowSize = boardGeometry.horizontalLineSize(rowIndex);
		for (int posInRow = 0; posInRow < subBoardRowSize; posInRow++)
		{
			x = xbasepos + (boardGeometry.firstColumnOffset(rowIndex) + posInRow)*newSingleKeySize;
			y = ybasepos;
			transform.transformPoint(x, y);
			keyMiniDisplay[keyIndex]->setBounds(x, y, newSingleKeySize, newSingleKeySize);

			keyIndex++;
		}
	}
}

void OctaveBoardComponent::mouseDown(const MouseEvent& e)
{
	Component::mouseDown(e);

	// If right mouse click: popup menu
	if (e.mods.isRightButtonDown())
	{
		PopupMenu menu;
		TerpstraSysExApplication::getApp().getMainMenu()->createEditMenu(menu);
		menu.show();
	}
}

void OctaveBoardComponent::setIsSelected(bool newValue)
{
	if (newValue != isSelected)
	{
		isSelected = newValue;
		repaint();
	}
}

/*
==============================================================================
TerpstraKeySetEdit class
==============================================================================
*/
/*
OctaveBoardComponent::OctaveBoardComponent()
{
	Image imgUnselected = ImageCache::getFromMemory(BinaryData::OctaveGraphic_png, BinaryData::OctaveGraphic_pngSize);
	Image imgSelected = ImageCache::getFromMemory(BinaryData::OctaveWithSelection_png, BinaryData::OctaveWithSelection_pngSize);

	setImages(true, true, true,
		imgUnselected, 0.5f, Colours::transparentBlack,
		imgUnselected, 0.6f, Colours::transparentBlack,
		imgSelected, 0.9f, Colours::transparentBlack,
		0.5f);

	setClickingTogglesState(true);
}

OctaveBoardComponent::~OctaveBoardComponent()
{

}

void OctaveBoardComponent::mouseDown(const MouseEvent& e)
{
	ImageButton::mouseDown(e);

	// If right mouse click: popup menu
	if (e.mods.isRightButtonDown())
	{
		PopupMenu menu;
		TerpstraSysExApplication::getApp().getMainMenu()->createEditMenu(menu);
		menu.show();
	}
}
*/