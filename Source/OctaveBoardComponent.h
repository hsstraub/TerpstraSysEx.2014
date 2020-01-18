/*
  ==============================================================================

    OctaveBoardComponent.h
    Created: 11 Jan 2020 6:54:03pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

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