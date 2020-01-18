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
OctaveBoardComponent::OctaveBoardComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

OctaveBoardComponent::~OctaveBoardComponent()
{
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

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("OctaveBoardComponent", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void OctaveBoardComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

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