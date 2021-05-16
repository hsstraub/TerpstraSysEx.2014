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
