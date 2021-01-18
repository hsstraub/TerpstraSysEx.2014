/*
  ==============================================================================

    PolygonPalette.h
    Created: 13 Nov 2020 7:24:40pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PolygonPalette  : public juce::Component, private ChangeListener
{
public:
    PolygonPalette(int numSidesIn = 6, float angleOffsetIn = float_Pi / 12.0f)
        : numSides(numSidesIn), angleOffset(angleOffsetIn)
    {
        createSwatches();
    }

    ~PolygonPalette() override
    {
    }

    void paint(Graphics& g) override
    {
        for (int i = 0; i < numSides; i++)
        {
            Path& swatch = swatches.getReference(i);

            g.setColour(palette[i]);
            g.fillPath(swatch);

//#if JUCE_DEBUG
//            g.setColour(palette[i].contrasting());
//            g.drawFittedText(String(i), swatch.getBounds().toNearestInt(), Justification::centred, 1);
//#endif

            if (i == selectedSwatch)
            {
                g.setColour(Colours::white);
                g.strokePath(swatch, PathStrokeType(2.0f));
            }
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
        AffineTransform transform = RectanglePlacement().getTransformToFit(originalBounds, scaled.withCentre({ (float)getLocalBounds().getCentreX(), size / 2.0f }));

        for (int i = 0; i < numSides; i++)
        {
            Path p = triangles[i];
            p.applyTransform(transform);
            swatches.set(i, p);
        }
    }

    void mouseDown(const MouseEvent& e) override
    {
        if (isEnabled())
        {
            for (auto s : swatches)
            {
                if (s.contains(e.position))
                {
                    setSelectedSwatchNumber(swatches.indexOf(s));

                    if (selector)
                    {
                        selector->setCurrentColour(palette[selectedSwatch], dontSendNotification);
                    }

                    return;
                }
            }

            selectedSwatch = -1;
            repaint();
        }
    }

    int getNumberOfSwatches() const
    {
        return numSides;
    }

    // Returns index of selected swatch, -1 if none
    int getSelectedSwatchNumber() const
    {
        return selectedSwatch;
    } 

    Colour getSelectedSwatchColour() const
    {
        if (selectedSwatch >= 0 && selectedSwatch < numSides)
            return palette[selectedSwatch];

        return Colour();
    }

    Array<Colour> getColourPalette() const
    {
        return palette;
    }

    virtual void setSelectedSwatchNumber(int swatchIndex)
    {
        selectedSwatch = swatchIndex;
        repaint();
    }

    virtual void setColourPalette(Array<Colour> colourPaletteIn)
    {
        palette = colourPaletteIn;
        repaint();
    }

    virtual void setSwatchColour(int swatchNumber, Colour newColour)
    {
        if (swatchNumber >= 0 && swatchNumber < numSides)
            palette.set(swatchNumber, newColour);
    }

    virtual void attachColourSelector(ColourSelector* selectorIn)
    {
        selector = selectorIn;
        
        if (selector)
        {
            if (selectedSwatch > -1)
            {
                selector->setCurrentColour(palette[selectedSwatch], dontSendNotification);
                //selector->setFocusserCallback([&](Component* c, var data) {
                //    palette.set(focussedSwatch, Colour::fromString(data.toString()));
                //    repaint();
                //});
            }

            selector->addChangeListener(this);
        }
    }

    void changeListenerCallback(ChangeBroadcaster* source) override
    {
        if (source == selector && selectedSwatch > -1)
        {
            setSwatchColour(selectedSwatch, selector->getCurrentColour());
            onlyRepaintSelectedSwatch = true;
            repaint();
        }
    }

private:

    void createSwatches()
    {
        float angInc = 2 * float_Pi / numSides;
        float angMargin = angInc * margin / 2;

        for (int i = 0; i < numSides; i++)
        {
            Path swatchPath;
            
            float angFrom  =  angInc *  i      + angleOffset + angMargin;
            float angTo    =  angInc * (i + 1) + angleOffset - angMargin;
            float bisector = (angFrom + angTo) / 2.0f;

            Point<float> pointFrom = {
                center.x + cosf(angFrom)  * outerRadius + cosf(angFrom) * innerRadius,
                center.y + sinf(angFrom)  * outerRadius + sinf(angFrom) * innerRadius
            };

            Point<float> pointTo = {
                center.x + cosf(angTo)    * outerRadius + cosf(angTo)   * innerRadius,
                center.y + sinf(angTo)    * outerRadius + sinf(angTo)   * innerRadius
            };
            
            Point<float> pointCenter = {
                center.x + cosf(bisector) * innerRadius,
                center.y + sinf(bisector) * innerRadius
            };

            swatchPath.addTriangle(pointFrom, pointTo, pointCenter);
            
            triangles.add(swatchPath);
            swatches.add(swatchPath);
        }

        triangles.minimiseStorageOverheads();
        swatches.minimiseStorageOverheads();
    }

private:

    const int numSides;
    const float angleOffset;

    const Point<float> center = { 0.5f, 0.5f };

    const float margin = 0.025f;
    const float radiusBounds = 0.5f;

    const float outerRadius = radiusBounds * (1 - margin);
    const float innerRadius = radiusBounds * margin;

    float width = 1, height = 1;

    Array<Path> triangles;
    Array<Path> swatches;

    int selectedSwatch = -1;
    bool onlyRepaintSelectedSwatch = false;

    ColourSelector* selector = nullptr;

    Array<Colour> palette =
    {
        Colours::orange,
        Colours::red,
        Colours::springgreen,
        Colours::lightsteelblue,
        Colours::yellowgreen,
        Colours::rebeccapurple
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PolygonPalette)
};
