/*
  ==============================================================================

    WheelsCalibrationComponent.h
    Created: 13 Jul 2021 7:45:55pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../LumatoneFirmwareDefinitions.h"

//==============================================================================
/*
*/
class WheelsCalibrationComponent  : public juce::Component
{
public:

    WheelsCalibrationComponent()
    {
        float defaultMinPoint = (0.5f - 0.15) / ADCSCALAR;
        float defaultMaxPoint = (0.5f + 0.15) / ADCSCALAR;
        auto initCalibration = WheelsCalibrationData();
        initCalibration.minPitch = defaultMinPoint;
        initCalibration.maxPitch = defaultMaxPoint;
        initCalibration.minMod = defaultMinPoint;
        initCalibration.maxMod= defaultMaxPoint;
    }

    void paint (juce::Graphics& g) override
    {
        // Fill gradient
        g.setGradientFill(pitchGradient);
        g.fillPath(pitchPath);

        g.setGradientFill(modGradient);
        g.fillPath(modPath);

        // Draw outline
        g.setColour(Colours::black);
        g.strokePath(pitchPath, PathStrokeType(wheelBorderThickness));
        g.strokePath(modPath, PathStrokeType(wheelBorderThickness));
    }

    void resized() override
    {
        wheelBounds.setSize(proportionOfHeight(widthToHeightRatio), getHeight());
        int maxWidth = wheelBounds.getWidth() * 3;

        if (maxWidth > getWidth())
        {
            wheelBounds.setSize(getHeight() * widthToHeightRatio, getHeight());
            yMargin = roundToInt((getHeight() - wheelBounds.getHeight()) * 0.5 + 1.5f * wheelBorderThickness);
        }

        roundedCorner = proportionOfWidth(roundedCornerToWidth);
        pitchPath = Path();
        pitchPath.addRoundedRectangle(wheelBounds.withX(xMargin).reduced(0, yMargin), roundedCorner);

        modPath = Path();
        modPath.addRoundedRectangle(wheelBounds.withRightX(getWidth() - xMargin).reduced(0, yMargin), roundedCorner);

        updateCalibrationData(calibrationData, false);
    }

    // TODO: improve this
    void updateCalibrationData(WheelsCalibrationData calibrationDataIn, bool redraw = true)
    {
        calibrationData = calibrationDataIn;

        float centerPitchNorm = calibrationData.getCentrePitchNorm();
        float minPitchNorm = calibrationData.getMinPitchNorm();
        float maxPitchNorm = calibrationData.getMaxPitchNorm();
        float minModNorm = calibrationData.getMinModNorm();
        float maxModNorm = calibrationData.getMaxModNorm();

        // "Max" values are "up", so needs y-axis inversion
        float minPitch = (1.0f - minPitchNorm);
        float maxPitch = (1.0f - maxPitchNorm);

        float minPitchMargin = jmin(1.0f, minPitch + gradientRange * 0.8f);
        float maxPitchMargin = jmax(0.0f, maxPitch - gradientRange);

        float minMod = (1.0f - minModNorm);
        float maxMod = (1.0f - maxModNorm);

        float minModMargin = jmin(1.0f, minMod + gradientRange * minMod * 2);
        float maxModMargin = jmax(0.0f, maxMod - gradientRange * maxModNorm * 2);

        Colour pitchMinColour = gradBackgroundBase.brighter((centerPitchNorm - minPitchNorm) * minPitch);
        Colour pitchMaxColour = gradBackgroundBase.brighter((maxPitchNorm - centerPitchNorm) * maxPitchNorm);

        Colour modMinColour = gradBackgroundBase.brighter(minMod * 0.3f);
        Colour modMaxColour = gradBackgroundBase.brighter(maxModNorm * 0.3f);

        pitchGradient = ColourGradient::vertical(gradBackgroundBase, gradBackgroundBase, wheelBounds);
        pitchGradient.addColour(minPitchMargin, pitchMinColour);
        pitchGradient.addColour(minPitch, gradCalibrate);
        pitchGradient.addColour(maxPitch, gradCalibrate);
        pitchGradient.addColour(maxPitchMargin, pitchMaxColour);

        modGradient = ColourGradient::vertical(gradBackgroundBase, gradBackgroundBase, wheelBounds);
        modGradient.addColour(minModMargin, modMinColour);
        modGradient.addColour(minMod, gradCalibrate);
        modGradient.addColour(maxMod, gradCalibrate);
        modGradient.addColour(maxModMargin, modMaxColour);

        if (redraw)
            repaint();
    }

private:

    WheelsCalibrationData calibrationData;

    Rectangle<float> wheelBounds;
    Path pitchPath;
    Path modPath;

    float wheelBorderThickness = 1.0f;
    int xMargin = roundToInt(wheelBorderThickness * 1.5f);
    int yMargin = 0;
    float roundedCorner = 12;

    Colour gradBackgroundBase = Colour(0xff1a1b1c);
    Colour gradCalibrate = Colour(0xffb1b1b1);
    ColourGradient pitchGradient;
    ColourGradient modGradient;

    const float gradientRange = 0.1f;

    Rectangle<float> pitchCal;
    Rectangle<float> modCal;

    const float widthToHeightRatio = 0.2381f;
    const float roundedCornerToWidth = 0.1f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WheelsCalibrationComponent)
};
