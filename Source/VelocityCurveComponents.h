/*
  ==============================================================================

    VelocityCurveComponents.h
    Created: 14 Apr 2019 8:07:49pm
    Author:  hsstraub

  ==============================================================================
*/

#ifndef VELOCITYCURVECOMPONENTS_H_INCLUDED
#define VELOCITYCURVECOMPONENTS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


/*
==============================================================================
Display of one beam in a velocity curve table
==============================================================================
*/
class VelocityCurveBeam : public Component
{
public:
	VelocityCurveBeam();
	~VelocityCurveBeam();

	int getValue() const { return beamValue; }
	void setValue(int newValue);
	void setValueAtLeast(int newValue);
	void setValueAtMost(int newValue);

	void paint(Graphics& g);
	void resized();

private:
	int beamValue;
};





#endif  // VELOCITYCURVECOMPONENTS_H_INCLUDED
