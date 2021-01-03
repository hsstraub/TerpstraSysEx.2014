/*
  ==============================================================================

    VelocityCurveComponents.cpp
    Created: 14 Apr 2019 8:07:49pm
    Author:  hsstraub

  ==============================================================================
*/

#include "VelocityCurveComponents.h"
#include "ViewConstants.h"

/*
==============================================================================
VelocityCurveBeam class
==============================================================================
*/

VelocityCurveBeam::VelocityCurveBeam()
{
	beamValue = 0;
}

VelocityCurveBeam::~VelocityCurveBeam()
{

}

void VelocityCurveBeam::setValue(int newValue)
{
	if (newValue < 0)
	{
		jassertfalse;
		newValue = 0;
	}

	if (newValue > 127)
	{
		jassertfalse;
		newValue = 127;
	}

	if (newValue != beamValue)
	{
		beamValue = newValue;
		repaint();
	}
}

void VelocityCurveBeam::setValueAtLeast(int newValue)
{
	if (getValue() < newValue)
		setValue(newValue);
}

void VelocityCurveBeam::setValueAtMost(int newValue)
{
	if (getValue() > newValue)
		setValue(newValue);
}

void VelocityCurveBeam::paint(Graphics& g)
{
	float w = this->getWidth();
	float h = this->getHeight();
	float beamHeight = getBeamHeightFromValue();

	Path rectPath;
	rectPath.startNewSubPath(0, h);
	rectPath.lineTo(w, h);
	rectPath.lineTo(w, h - beamHeight);
	rectPath.lineTo(0, h - beamHeight);
	rectPath.closeSubPath();

	g.setColour(findColour(beamColourId));
	g.fillPath(rectPath);
}

void VelocityCurveBeam::resized()
{

}

juce::Point<float> VelocityCurveBeam::getBottomMid()
{
	juce::Point<float> pt = this->getBoundsInParent().getBottomLeft().toFloat();
	pt.addXY(this->getWidth() / 2.0, 0);

	return pt;
}

float VelocityCurveBeam::getBeamHeightFromValue(int value)
{
	return value * this->getHeight() / 128;
}

int VelocityCurveBeam::getBeamValueFromLocalPoint(juce::Point<float> localPoint)
{
    auto val = (getBottom() - localPoint.y) * 128 / getHeight();
    if (val < 0)
        return 0;
    else if (val >= 128)
        return 127;
    else
        return val;
}
