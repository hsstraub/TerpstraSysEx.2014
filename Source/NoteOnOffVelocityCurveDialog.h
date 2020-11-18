/*
  ==============================================================================

    NoteOnOffVelocityCurveDialog.h
    Created: 13 Nov 2020 10:20:07pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

#include "VelocityCurveDlgBase.h"

#define VELOCITYINTERVALTABLESIZE 127

// Note on/on velocity curve dialog. Horizontal axis stands for ticks
class NoteOnOffVelocityCurveDialog : public VelocityCurveDlgBase
{
public:
	//==============================================================================
	NoteOnOffVelocityCurveDialog();
	~NoteOnOffVelocityCurveDialog() override;

	void restoreIntervalTableFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveIntervalTableToPropertiesFile(PropertiesFile* propertiesFile);

	virtual void sendVelocityTableToController() override;
	virtual void sendVelocityConfigurationRequest() override;
	virtual void midiMessageReceived(const MidiMessage& midiMessage) override;

protected:
	int ticksCountFromXPos(int xPos) const { return xPos * 16; }
	float milliSecondsFromTicksCount(int ticksCount) const { return 1.1f * ticksCount; }

	virtual String beamXPosText(int xPos) const override;
	virtual float beamWidth(int xPos) const override;

private:
	int velocityIntervalTableValues[VELOCITYINTERVALTABLESIZE];
};