/*
  ==============================================================================

    NoteOnOffVelocityCurveDialog.cpp
    Created: 13 Nov 2020 10:20:07pm
    Author:  hsstraub

  ==============================================================================
*/

#include "NoteOnOffVelocityCurveDialog.h"
#include "TerpstraMidiDriver.h"
#include "Main.h"

//==============================================================================
NoteOnOffVelocityCurveDialog::NoteOnOffVelocityCurveDialog()
	: VelocityCurveDlgBase(TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff)
{
	labelCurrentXPos->setSize(128, labelCurrentXPos->getHeight());
}

void NoteOnOffVelocityCurveDialog::midiMessageReceived(const MidiMessage& message)
{
	if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsVelocityIntervalConfigReceptionMessage(message))
	{
		// ToDO
	}
	else
		__super::midiMessageReceived(message);
}

String NoteOnOffVelocityCurveDialog::beamXPosText(int xPos) const
{
	auto ticksCount = ticksCountFromXPos(xPos);

	{ return String(ticksCount) + " ticks (" + String(milliSecondsFromTicksCount(ticksCount)) + " ms)"; }
}