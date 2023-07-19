/*
  ==============================================================================

    VelocityCurveComponents.h
    Created: 14 Apr 2019 8:07:49pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/*
==============================================================================
Display of one beam in a velocity curve table
==============================================================================
*/
class VelocityCurveBeam : public Component, public SettableTooltipClient
{
public:
	VelocityCurveBeam();
	~VelocityCurveBeam();

	int getValue() const { return beamValue; }
	void setValue(int newValue);
	void setValueAtLeast(int newValue);
	void setValueAtMost(int newValue);

	void paint(Graphics& g) override;
	void resized() override;

    juce::Point<float> getBottomMid();

	float getBeamHeightFromValue(int value);
	float getBeamHeightFromValue() { return getBeamHeightFromValue(beamValue); }

	int getBeamValueFromLocalPoint(juce::Point<float> localPoint);
	void setBeamValueFromLocalPoint(juce::Point<float> localPoint) { setValue(getBeamValueFromLocalPoint(localPoint)); }

	enum ColourIds
	{
		beamColourId = 0x2000111,
		outlineColourId = 0x2000121
	};

private:
	int beamValue;
};
