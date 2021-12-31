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

//#if JUCE_DEBUG
//            g.setColour(getSwatchColour(i).contrasting());
//            g.drawFittedText(String(i), swatch.getBounds().toNearestInt(), Justification::centred, 1);
//#endif
        }

        if (selectedSwatch >= 0)
        {
            g.setColour(Colours::white);
            g.strokePath(swatchPaths[selectedSwatch], PathStrokeType(2.0f));
        }
    }

    void resized() override
    {
        float scalar = (width > height)
            ? getWidth() / width
            : getHeight() / height;

        scalar *= (1 - 0.05f); // kludge margin to fix it getting cutoff sometimes

        float newWidth = width * scalar;
        float newHeight = height * scalar;

        float xOffset = (getWidth() - newWidth) * 0.5f - getWidth() * 0.125f; // Last bit is a correction needed in HexagonTiling
        float yOffset = (getHeight() - newHeight) * 0.5f;

        // Get scaling and centering transform
        AffineTransform transform = AffineTransform()
            .followedBy(AffineTransform::scale(scalar))
            .followedBy(AffineTransform::translation(xOffset, 0));

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

        // For finding actual width and height
        Path tilePath;

        swatchPaths.clear();
        for (auto c : centres)
        {
            c.applyTransform(transform);

            Path hex;
            hex.addPolygon(c, 6, tiling.getRadius());
            swatchPaths.add(hex);

            tilePath.addPath(hex);
        }

        Rectangle<float> tileBounds = tilePath.getBounds(); // does this clip (translate) for optimal size, or does it retain original origin?
        width = tileBounds.getWidth();
        height = tileBounds.getHeight();
    }

private:

    const Array<Point<int>> hexCoordinates;
    const float angleOffset;

    const float margin = 0.024f;
    float width = 1, height = 1;
    
    int numColumns = 0;
    int numRows = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HexagonPalette)
};

class TenHexagonPalette : public HexagonPalette
{
    Array<Point<int>> createCoordinates()
    {
        Array<Point<int>> coords = { 
			Point<int>(1, 0), 
			Point<int>(2, 0), 
			Point<int>(3, 0),
			Point<int>(0, 1), 
			Point<int>(1, 1), 
			Point<int>(2, 1), 
			Point<int>(3, 1),
			Point<int>(0, 2), 
			Point<int>(1, 2), 
			Point<int>(2, 2)
        };

        return coords;
    }

public:

    TenHexagonPalette(double rotationAngle = 0.0)
        : HexagonPalette(createCoordinates(), rotationAngle)
    {}
};