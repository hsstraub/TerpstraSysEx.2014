/*
  ==============================================================================

    OctaveBoardComponent.h
    Created: 11 Jan 2020 6:54:03pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "KeyboardDataStructure.h"
#include "BoardGeometry.h"

//==============================================================================

// Representation of a key inside the octaveboardcomponent
class KeyMiniDisplayInsideOctaveBoardComponent : public Component
{
public:
	KeyMiniDisplayInsideOctaveBoardComponent();
	~KeyMiniDisplayInsideOctaveBoardComponent();

	void paint(Graphics&) override;
	void resized() override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyMiniDisplayInsideOctaveBoardComponent)
};


//==============================================================================

class OctaveBoardComponent    : public Component
{
public:
    OctaveBoardComponent();
    ~OctaveBoardComponent();

    void paint (Graphics&) override;
    void resized() override;
	void mouseDown(const MouseEvent& e) override;

	void setIsSelected(bool newValue);

private:
	std::unique_ptr<KeyMiniDisplayInsideOctaveBoardComponent>	keyMiniDisplay[TERPSTRABOARDSIZE];

	// Geometry settings (same as main component XXX) 
	TerpstraBoardGeometry	boardGeometry;

	bool isSelected = false;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OctaveBoardComponent)
};


/*
==============================================================================
Display representant of a key set of 56 keys
==============================================================================
*/
/*
class OctaveBoardComponent : public ImageButton
{
public:
	OctaveBoardComponent();
	~OctaveBoardComponent();

	void setIsSelected(bool newValue) { setToggleState(newValue, dontSendNotification); };

	void mouseDown(const MouseEvent& e) override;
};
*/