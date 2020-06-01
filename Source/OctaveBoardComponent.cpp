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
	bool isSelected = ((OctaveBoardComponent*)getParentComponent())->getIsSelected();

    Colour hexagonColour = findColour(TerpstraKeyEdit::backgroundColourId).overlaidWith(getKeyColour()
        .withAlpha(isSelected ? TERPSTRASINGLEKEYCOLOURALPHA : TERPSTRASINGLEKEYCOLOURUNSELECTEDMINIALPHA));
	g.setColour(hexagonColour);
	g.fillPath(hexPath);

	Colour lineColour = findColour(TerpstraKeyEdit::outlineColourId);
	if ( !isSelected)
        lineColour = lineColour.withAlpha(TERPSTRASINGLEKEYCOLOURUNSELECTEDMINIALPHA);
	g.setColour(lineColour);
	g.strokePath(hexPath, PathStrokeType(1));
}

void KeyMiniDisplayInsideOctaveBoardComponent::resized()
{
	float w = this->getWidth();
	float h = this->getHeight();

	float marginOffset = 1.5;

	// recalculate position and size of hexagon
	hexPath.clear();
	hexPath.startNewSubPath(w / 2.0f, 0);
	hexPath.lineTo(w, h / 4.0f);
	hexPath.lineTo(w, 3.0f * h / 4.0f);
	hexPath.lineTo(w / 2.0f, h);
	hexPath.lineTo(0, 3.0f * h / 4.0f);
	hexPath.lineTo(0, h / 4.0f);
	hexPath.closeSubPath();

	// Rotate slightly counterclockwise around the center
	AffineTransform transform = AffineTransform::translation(-w / 2.0f, -h / 2.0f);
	transform = transform.rotated(TERPSTRASINGLEKEYROTATIONANGLE);
	transform = transform.translated(w / 2.0f, h / 2.0f);

	hexPath.applyTransform(transform);
	hexPath.scaleToFit(marginOffset, marginOffset, w - 2*marginOffset, h - 2*marginOffset, true);
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
}

void OctaveBoardComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

	int newHeight = getHeight();
	int newWidth = getWidth();

	// Single key field size
	// ToDo better logic
	int newSingleKeySize = jmin(newWidth*2/17, newHeight/8);

	// Transformation Rotate slightly counterclockwise
	float x = 0.0;
	float y = 0.0;
	AffineTransform transform = AffineTransform::rotation(TERPSTRASINGLEKEYROTATIONANGLE);

	int keyIndex = 0;
	int mostBottomKeyPos = 0;

	auto boardGeometry = ((MainContentComponent*)getParentComponent())->getBoardGeometry();
	// Rows
	int rowCount = boardGeometry.horizontalLineCount();
	for (int rowIndex = 0; rowIndex < rowCount; rowIndex++)
	{
		float xbasepos;
		if (rowIndex % 2 == 0)
			xbasepos = 0.0;
		else
			xbasepos = 0.0 + newSingleKeySize / 2.0;

		float ybasepos = 0.0 + 3.0 * rowIndex * newSingleKeySize / 4.0;

		int subBoardRowSize = boardGeometry.horizontalLineSize(rowIndex);
		for (int posInRow = 0; posInRow < subBoardRowSize; posInRow++)
		{
			x = xbasepos + (boardGeometry.firstColumnOffset(rowIndex) + posInRow)*newSingleKeySize;
			y = ybasepos;
			transform.transformPoint(x, y);
			keyMiniDisplay[keyIndex]->setBounds(roundToInt(x), roundToInt(y), newSingleKeySize, newSingleKeySize);

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
