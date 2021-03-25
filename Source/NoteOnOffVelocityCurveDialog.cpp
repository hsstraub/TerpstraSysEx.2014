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
    : VelocityCurveDlgBase(TerpstraVelocityCurveConfig::VelocityCurveType::noteOnNoteOff)
{
}

//NoteOnOffVelocityCurveDialog::~NoteOnOffVelocityCurveDialog()
//{
//}

//float NoteOnOffVelocityCurveDialog::beamWidth(int xPos)
//{
//	auto mappingInEdit = getMappingInEdit();
//	if ( mappingInEdit == nullptr)	// Security at start of program
//		return 1;	// ad-hoc
//
//	if (xPos == 0)
//	{
//		return (getWidth() - 2.0f * cbEditMode->getX()) * mappingInEdit->velocityIntervalTableValues[0] / 2048.0f;
//	}
//	else if (xPos < VELOCITYINTERVALTABLESIZE)
//	{
//		return (getWidth() - 2.0f * cbEditMode->getX()) * (mappingInEdit->velocityIntervalTableValues[xPos] - mappingInEdit->velocityIntervalTableValues[xPos-1]) / 2048.0f;
//	}
//	else if (xPos == VELOCITYINTERVALTABLESIZE)
//	{
//		return (getWidth() - 2.0f * cbEditMode->getX()) * (2048.0f - mappingInEdit->velocityIntervalTableValues[xPos-1]) / 2048.0f;
//	}
//	else
//	{
//		jassertfalse;
//		return 1;
//	}
//}