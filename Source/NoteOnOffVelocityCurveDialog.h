/*
  ==============================================================================

    NoteOnOffVelocityCurveDialog.h
    Created: 13 Nov 2020 10:20:07pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

#include "VelocityCurveDlgBase.h"

// Note on/on velocity curve dialog. Horizontal axis stands for ticks
class NoteOnOffVelocityCurveDialog : public VelocityCurveDlgBase
{
public:
	//==============================================================================
	NoteOnOffVelocityCurveDialog();
	~NoteOnOffVelocityCurveDialog() override;

	// ToDo Read from and write to *.LMT file

protected:
	float milliSecondsFromTicksCount(int ticksCount) const { return 1.1f * ticksCount; }

	virtual String beamXPosText(int xPos) const override;
	virtual float beamWidth(int xPos) override;

};

class FaderVelocityCurveDialog : public VelocityCurveDlgBase
{
public:
	FaderVelocityCurveDialog() : VelocityCurveDlgBase(TerpstraMidiDriver::VelocityCurveType::fader) {}
};

class AftertouchVelocityCurveDialog : public VelocityCurveDlgBase
{
public:
	AftertouchVelocityCurveDialog() : VelocityCurveDlgBase(TerpstraMidiDriver::VelocityCurveType::afterTouch) {}
};