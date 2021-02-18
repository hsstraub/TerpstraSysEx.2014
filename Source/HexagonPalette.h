/*
  ==============================================================================

    PolygonPalette.h
    Created: 13 Nov 2020 7:24:40pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "Palette.h"
#include "HexagonTilingGeometry.h"


//==============================================================================
/*
*/
class HexagonPalette: public Palette
{
public:
    HexagonPalette(Array<Point<int>> hexCoordsIn, double rotationAngle = 0.0)
        : Palette(hexCoordsIn.size()), hexCoordinates(hexCoordsIn), angleOffset(rotationAngle)
    {
        for (auto point : hexCoordinates)
        {
            if (point.x > numColumns)
                numColumns = point.x;

            if (point.y > numRows)
                numRows = point.y;
        }

        numColumns += 1;
        numRows += 1;

        createSwatches();
    }

    ~HexagonPalette() override
    {
    }

    void paint(Graphics& g) override
    {
        for (int i = 0; i < getNumberOfSwatches(); i++)
        {
            Path& swatch = swatchPaths.getReference(i);

            g.setColour(getSwatchColour(i));
            g.fillPath(swatch);

#if JUCE_DEBUG
            g.setColour(getSwatchColour(i).contrasting());
            g.drawFittedText(String(i), swatch.getBounds().toNearestInt(), Justification::centred, 1);
#endif
        }

        if (selectedSwatch >= 0)
        {
            g.setColour(Colours::white);
            g.strokePath(swatchPaths[selectedSwatch], PathStrokeType(2.0f));
        }
    }

    void resized() override
    {
        // Original bounds
        Rectangle<float> originalBounds = { 0.0f, 0.0f, 1.0f, 1.0f };

        float size = jmin(getWidth(), getHeight());

        // Crop to square
        Rectangle<float> scaled = { 0, 0, size, size };

        // Get scaling and centering transform
        AffineTransform transform = RectanglePlacement().getTransformToFit(originalBounds, scaled.withCentre({ (float)getLocalBounds().getCentreX(), size * 0.5f }));

        for (int i = 0; i < getNumberOfSwatches(); i++)
        {
            Path p = swatchPaths[i];
            p.applyTransform(transform);
            swatchPaths.set(i, p);
        }
    }

private:

    void createSwatches()
    {
        HexagonTilingGeometry tiling;
        tiling.fitTilingTo(Rectangle<float>(0.0, 0.0, width, height),
            numColumns, numRows, margin, 0, true
        );

        Array<Point<float>> centres = tiling.transformPointsFromOrigin(hexCoordinates);

        AffineTransform transform;

        // TODO get rotation pivot
        if (angleOffset != 0.0)
            transform = AffineTransform::rotation(angleOffset);

        swatchPaths.clear();
        for (auto c : centres)
        {
            c.applyTransform(transform);

            Path hex;
            hex.addPolygon(c, 6, tiling.getRadius());
            swatchPaths.add(hex);
        }
    }

private:

    const Array<Point<int>> hexCoordinates;
    const float angleOffset;

    const Point<float> center = { 0.5f, 0.5f };

    const float margin = 0.001f;
    float width = 1, height = 1;
    
    int numColumns = 0;
    int numRows = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HexagonPalette)
};

class TenHexagonPalette : public HexagonPalette
{
    Array<Point<int>> createCoordinates()
    {
        Array<Point<int>> coords = { Point<int>
                {1, 0}, {2, 0}, {3, 0},
            {0, 1}, {1, 1}, {2, 1}, {3, 1},
                {0, 2}, {1, 2}, {2, 2}
        };

        return coords;
    }

public:

    TenHexagonPalette(double rotationAngle = 0.0)
        : HexagonPalette(createCoordinates(), rotationAngle)
    {}
};