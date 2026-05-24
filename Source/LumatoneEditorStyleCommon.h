/*
  ==============================================================================

    LumatoneEditorStyleCommon.h
    Created: 24 May 2026 10:27:31am
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

//==================================================================
//
// STATIC HELPERS
//
//==================================================================

#define PHI 1.618033989f // (1 + sqrt(5)) / 2
#define HEXRADIUSTOLATERAL 0.8660254f // sqrt(3) / 2

#if JUCE_MAC
    #define GLOBALFONTSCALAR 0.9f
    #define CONTROLBOXFONTHEIGHTSCALAR 0.7f
#elif JUCE_WINDOWS
    #define GLOBALFONTSCALAR 1.0f
    #define CONTROLBOXFONTHEIGHTSCALAR 0.8f
#elif JUCE_LINUX
    #define GLOBALFONTSCALAR 1.0f
    #define CONTROLBOXFONTHEIGHTSCALAR 0.73f
#endif

static void resizeLabelWithHeight(Label* label, int height, float fontHeightScalar = 1.0f, String textSuffix = "_")
{
    label->setFont(label->getFont().withHeight(height * fontHeightScalar * GLOBALFONTSCALAR));
    label->setSize(round(label->getFont().getStringWidthFloat(label->getText() + textSuffix)), height);
}

static void resizeLabelWithWidth(Label* label, int width, float fontHeightScalar = 1.0f)
{
    float heightOverWidth = label->getFont().getHeight() / label->getFont().getStringWidthFloat(label->getText());
    label->setSize(width, round(heightOverWidth * width));
    label->setFont(label->getFont().withHeight(label->getHeight() * fontHeightScalar * GLOBALFONTSCALAR));
}

static void drawPathToFillBounds(Graphics& g, const Path& path, Rectangle<float> boundsToFill)
{
    AffineTransform transform = RectanglePlacement().getTransformToFit(
        path.getBounds(),
        boundsToFill
    );

    g.strokePath(path, PathStrokeType(0.8f), transform);
}

// Creates the Lumatone logomark in 1x1 floating bit path
static Path createLogomark()
{
    Path logo;
    Point<float> center(0.5f, 0.5f);

    float phi2 = PHI * 2;
    float innerRad = 1.0f / phi2;
    float outerRad = phi2 * 0.125f;
    float ang = MathConstants<float>::pi * 0.083333f;
    float angOff = ang * 0.5f;

    logo.addPolygon(center, 6, innerRad, ang - angOff);
    logo.addPolygon(center, 6, HEXRADIUSTOLATERAL * innerRad, -ang - angOff);
    logo.addPolygon(center, 6, outerRad, ang - angOff);
    logo.addPolygon(center, 6, outerRad, -ang - angOff);

    return logo;
}
