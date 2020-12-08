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

NoteOnOffVelocityCurveDialog::~NoteOnOffVelocityCurveDialog()
{
}

// ToDo Read from and write to *.LMT file

String NoteOnOffVelocityCurveDialog::beamXPosText(int xPos) const
{
	auto ticksCount = TerpstraKeyMapping::ticksCountFromXPos(xPos);

	{ return String(ticksCount) + " ticks (" + String(milliSecondsFromTicksCount(ticksCount)) + " ms)"; }
}

float NoteOnOffVelocityCurveDialog::beamWidth(int xPos) const
{
	// Securtity in case parent- component has no value yet
	jassert(getParentComponent() != nullptr);
	jassert(getParentComponent()->getParentComponent() != nullptr);

	// Resized is already called when tabbed ocponent is constructed, at a time where its parent has not been assigned yet
	if (getParentComponent()->getParentComponent()->getParentComponent() == nullptr)
	{
		return 1;	// ad-hoc
	}

	auto mappingInEdit = (dynamic_cast<MainContentComponent*>(getParentComponent()->getParentComponent()->getParentComponent()))->getMappingInEdit();

	if (xPos == 0)
	{
		return (getWidth() - 2.0f * cbEditMode->getX()) * mappingInEdit.velocityIntervalTableValues[0] / 2048.0f;
	}
	else if (xPos < VELOCITYINTERVALTABLESIZE)
	{
		return (getWidth() - 2.0f * cbEditMode->getX()) * (mappingInEdit.velocityIntervalTableValues[xPos] - mappingInEdit.velocityIntervalTableValues[xPos-1]) / 2048.0f;
	}
	else if (xPos == VELOCITYINTERVALTABLESIZE)
	{
		return (getWidth() - 2.0f * cbEditMode->getX()) * (2048.0f - mappingInEdit.velocityIntervalTableValues[xPos-1]) / 2048.0f;
	}
	else
	{
		jassertfalse;
		return 1;
	}
}