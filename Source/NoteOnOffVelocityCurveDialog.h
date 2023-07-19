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
class NoteOnOffVelocityCurveDialog : public VelocityCurveDlgBase {
public:
    NoteOnOffVelocityCurveDialog();
};

class FaderVelocityCurveDialog : public VelocityCurveDlgBase {
public:
    FaderVelocityCurveDialog() : VelocityCurveDlgBase(TerpstraVelocityCurveConfig::VelocityCurveType::fader)
    {
    }
};

class AftertouchVelocityCurveDialog : public VelocityCurveDlgBase {
public:
    AftertouchVelocityCurveDialog() : VelocityCurveDlgBase(TerpstraVelocityCurveConfig::VelocityCurveType::afterTouch)
    {
    }
};

class LumatouchVelocityCurveDialog : public VelocityCurveDlgBase {
public:
    LumatouchVelocityCurveDialog() : VelocityCurveDlgBase(TerpstraVelocityCurveConfig::VelocityCurveType::lumaTouch)
    {
    }
};