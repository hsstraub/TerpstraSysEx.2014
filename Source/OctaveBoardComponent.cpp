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
KeyMiniDisplayInsideOctaveBoardComponent::KeyMiniDisplayInsideOctaveBoardComponent(int newKeyIndex)
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
	keyIndex = newKeyIndex;

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
	float w = this->getWidth();
	float h = this->getHeight();

	float lineWidth = TERPSTRASINGLEKEYFLDLINEWIDTH;

	// Draw hexagon
	Path hexPath;
	hexPath.startNewSubPath(w / 2.0f, lineWidth);
	hexPath.lineTo(w - lineWidth, h / 4.0f);
	hexPath.lineTo(w - lineWidth, 3.0f * h / 4.0f);
	hexPath.lineTo(w / 2.0f, h - lineWidth);
	hexPath.lineTo(lineWidth, 3.0f * h / 4.0f);
	hexPath.lineTo(lineWidth, h / 4.0f);
	hexPath.closeSubPath();

	// Rotate slightly counterclockwise around the center
	AffineTransform transform = AffineTransform::translation(-w / 2.0f, -h / 2.0f);
	transform = transform.rotated(TERPSTRASINGLEKEYROTATIONANGLE);
	transform = transform.translated(w / 2.0f, h / 2.0f);

	hexPath.applyTransform(transform);
	hexPath.scaleToFit(lineWidth, lineWidth, w - lineWidth, h - lineWidth, true);

    Colour bgColour = findColour(TerpstraKeyEdit::backgroundColourId).overlaidWith(getKeyColour().withAlpha((uint8)0x40));

	//g.fillAll(getLookAndFeel().findColour(TextEditor::backgroundColourId));   // clear the background

	g.setColour(bgColour);
	g.fillPath(hexPath);

	g.setColour(findColour(TerpstraKeyEdit::outlineColourId));
	g.strokePath(hexPath, PathStrokeType(lineWidth));
}

void KeyMiniDisplayInsideOctaveBoardComponent::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..

}

Colour KeyMiniDisplayInsideOctaveBoardComponent::getKeyColour()
{
    if ( keyIndex >= 0 && keyIndex < TERPSTRABOARDSIZE)
    {
        TerpstraKeys* pCurrentOctaveBoardData = ((OctaveBoardComponent*)getParentComponent())->getKeyData();
        if ( pCurrentOctaveBoardData != nullptr)
            return Colour(pCurrentOctaveBoardData->theKeys[keyIndex].colour);
    }

    return findColour(TerpstraKeyEdit::backgroundColourId);
}

//==============================================================================
OctaveBoardComponent::OctaveBoardComponent(int newOctaveBoardIndex)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    octaveBoardIndex = newOctaveBoardIndex;

	for (int i = 0; i < TERPSTRABOARDSIZE; i++)
	{
		keyMiniDisplay[i].reset(new KeyMiniDisplayInsideOctaveBoardComponent(i));
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

    Colour bgColour = getLookAndFeel().findColour (ResizableWindow::backgroundColourId)/*.withAlpha((uint8)0xff)*/;
    g.fillAll(bgColour);   // clear the background

    g.setColour ( isSelected ?  Colours::orange : Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void OctaveBoardComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

	int newHeight = getHeight();
	int newWidth = getWidth();

	// Single key field size
	// ToDo better logic
	int newSingleKeySize = jmin(newWidth / 8, newHeight/8);

	// Transformation Rotate slightly counterclockwise
	int x = 0;
	int y = 0;
	AffineTransform transform = AffineTransform::translation(-x, -y);
	transform = transform.rotated(TERPSTRASINGLEKEYROTATIONANGLE);
	transform = transform.translated(x, y);

	int keyIndex = 0;
	int mostBottomKeyPos = 0;

	auto boardGeometry = ((MainContentComponent*)getParentComponent())->getBoardGeometry();
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

			mostBottomKeyPos = jmax(mostBottomKeyPos, keyMiniDisplay[keyIndex]->getBottom());

			keyIndex++;
		}
	}

	// Move key fields to bottom
	if ( mostBottomKeyPos < newHeight)
	{
		int ydispacement = newHeight - mostBottomKeyPos;
		int maxKeyIndex = keyIndex;
		for (keyIndex = 0; keyIndex < maxKeyIndex; keyIndex++)
			keyMiniDisplay[keyIndex]->setTopLeftPosition(
				juce::Point<int>(keyMiniDisplay[keyIndex]->getX(), keyMiniDisplay[keyIndex]->getY() + ydispacement));
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

TerpstraKeys* OctaveBoardComponent::getKeyData()
{
     if ( octaveBoardIndex >= 0 && octaveBoardIndex < TERPSTRABOARDSIZE)
    {
        return &((MainContentComponent*)(getParentComponent()))->getMappingInEdit().sets[octaveBoardIndex];
    }
    else
        return nullptr;
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
