/*
  ==============================================================================

    VelocityCurveEditStrategy.cpp
    Created: 10 Apr 2019 9:54:14pm
    Author:  hsstraub

  ==============================================================================
*/

#include "VelocityCurveEditStrategy.h"


/*
==============================================================================
VelocityCurveEditStrategyBase class
==============================================================================
*/

VelocityCurveEditStrategyBase::VelocityCurveEditStrategyBase(Path& beamTableFrameRef, VelocityCurveBeam** velocityBeamTablePtr)
	: beamTableFrame(beamTableFrameRef), velocityBeamTable(velocityBeamTablePtr)
{
}


/*
==============================================================================
VelocityCurveFreeDrawingStrategy class
==============================================================================
*/

VelocityCurveFreeDrawingStrategy::VelocityCurveFreeDrawingStrategy(Path& beamTableFrameRef, VelocityCurveBeam** velocityBeamTablePtr)
	: VelocityCurveEditStrategyBase(beamTableFrameRef, velocityBeamTablePtr)
{
}

void VelocityCurveFreeDrawingStrategy::paint(Graphics& g)
{
	if (!drawedLine.isEmpty())
		g.strokePath(drawedLine, PathStrokeType(1.000f));
}

void VelocityCurveFreeDrawingStrategy::resized()
{
	drawedLine.clear();
}

bool VelocityCurveFreeDrawingStrategy::mouseDown(Point<float> localPoint)
{
	drawedLine.clear();
	if (beamTableFrame.contains(localPoint))
	{
		drawedLine.startNewSubPath(localPoint.x, localPoint.y);

		for (int x = 0; x < 128; x++)
		{
			Rectangle<int> beamRect = velocityBeamTable[x]->getBounds();
			if (beamRect.contains((int)localPoint.x, (int)localPoint.y))
			{
				int newBeamValue = velocityBeamTable[x]->getBeamValueFromLocalPoint(localPoint);
				velocityBeamTable[x]->setValue(newBeamValue);

				// Change other beams' values so curve stays monotonous
				for (int x2 = 0; x2 < x; x2++)
					velocityBeamTable[x2]->setValueAtMost(newBeamValue);

				for (int x2 = x + 1; x2 < 128; x2++)
					velocityBeamTable[x2]->setValueAtLeast(newBeamValue);

				return true;
			}
		}
	}

	return false;

}

bool VelocityCurveFreeDrawingStrategy::mouseDrag(Point<float> localPoint)
{
	for (int x = 0; x < 128; x++)
	{
		Rectangle<int> beamRect = velocityBeamTable[x]->getBounds();
		if (beamRect.contains((int)localPoint.x, (int)localPoint.y))
		{
			drawedLine.lineTo(localPoint);

			int newBeamValue = velocityBeamTable[x]->getBeamValueFromLocalPoint(localPoint);
			velocityBeamTable[x]->setValue(newBeamValue);

			// Change other beams' values so curve stays monotonous
			for (int x2 = 0; x2 < x; x2++)
				velocityBeamTable[x2]->setValueAtMost(newBeamValue);

			for (int x2 = x + 1; x2 < 128; x2++)
				velocityBeamTable[x2]->setValueAtLeast(newBeamValue);

			return true;
		}
	}
	
	return false;
}

void VelocityCurveFreeDrawingStrategy::mouseUp(const MouseEvent &event)
{
	drawedLine.clear();
}

/*
==============================================================================
VelocityCurveLinearDrawingStrategy class
==============================================================================
*/

VelocityCurveLinearDrawingStrategy::VelocityCurveLinearDrawingStrategy(Path& beamTableFrameRef, VelocityCurveBeam** velocityBeamTablePtr)
	: VelocityCurveEditStrategyBase(beamTableFrameRef, velocityBeamTablePtr)
	, draggedOriginalXPosition(-1)
{
	fixPointBeamHeights[0] = 0;
	
	for (int x = 1; x < 127; x++)
		fixPointBeamHeights[x] = -1;

	fixPointBeamHeights[127] = 127;
}

void VelocityCurveLinearDrawingStrategy::Initialize()
{
	// Take segments from current beam values, with superfluous points removed
	for (int x = 0; x < 128; x++)
	{
		fixPointBeamHeights[x] = velocityBeamTable[x]->getValue();
	}

	ClearSuperfluousPoints();
}

void VelocityCurveLinearDrawingStrategy::paint(Graphics& g)
{
	g.setColour(Colours::black);

	// First point
	Point<float> pt = velocityBeamTable[0]->getBottomMid();
	pt.setY(pt.y - velocityBeamTable[0]->getBeamHeightFromValue(fixPointBeamHeights[0]));

	Path drawedLine;
	drawedLine.startNewSubPath(pt);
	
	// points in-between
	for (int x = 1; x < 128; x++)
	{
		if (fixPointBeamHeights[x] != -1)
		{
			pt = velocityBeamTable[x]->getBottomMid();
			pt.setY(pt.y - velocityBeamTable[x]->getBeamHeightFromValue(fixPointBeamHeights[x]));
			drawedLine.lineTo(pt);
		}
	}

	// Circles around the point
	for (int x = 0; x < 128; x++)
	{
		if (fixPointBeamHeights[x] != -1)
		{
			pt = velocityBeamTable[x]->getBottomMid();
			pt.setY(pt.y - velocityBeamTable[x]->getBeamHeightFromValue(fixPointBeamHeights[x]));

			drawedLine.addEllipse(pt.x-4, pt.y-4, 8, 8);
		}
	}

	g.strokePath(drawedLine, PathStrokeType(1.000f));
}

bool VelocityCurveLinearDrawingStrategy::mouseDown(Point<float> localPoint)
{
	// Find position to drag
	draggedOriginalXPosition = -1;
	for (int x = 0; x < 128; x++)
	{
		Rectangle<int> beamRect = velocityBeamTable[x]->getBounds();
		if (beamRect.contains(localPoint.toInt()))
		{
			draggedOriginalXPosition = x;
			break;
		}
	}

	if (draggedOriginalXPosition >= 0)
	{
		// First and last position cannot be dragged horizontally
		if (draggedOriginalXPosition == 0 || draggedOriginalXPosition == 127)
		{
			minDragXPosition = draggedOriginalXPosition;
			maxDragXPosition = draggedOriginalXPosition;
		}
		else
		{
			// Dragging would be psosible until next line point
			// for now: no horizontal dragging
			// ToDO
			minDragXPosition = draggedOriginalXPosition;
			maxDragXPosition = draggedOriginalXPosition;
		}
	}

	if (draggedOriginalXPosition >= 0)
	{
		fixPointBeamHeights[draggedOriginalXPosition] = velocityBeamTable[draggedOriginalXPosition]->getBeamValueFromLocalPoint(localPoint);
		return true;
	}

	return false;
}

bool VelocityCurveLinearDrawingStrategy::mouseDrag(Point<float> localPoint)
{
	if (isDragging())
	{
		int currentDraggedXPosition = -1;
		for (int x = 0; x < 128; x++)
		{
			Rectangle<int> beamRect = velocityBeamTable[x]->getBounds();
			if (beamRect.contains(localPoint.toInt()))
			{
				currentDraggedXPosition = x;
				break;
			}
		}

		if (currentDraggedXPosition != -1)
		{
			if (currentDraggedXPosition < minDragXPosition)
				currentDraggedXPosition = minDragXPosition;
			else if (currentDraggedXPosition > maxDragXPosition)
				currentDraggedXPosition = maxDragXPosition;

			// If x-position changed: remove point of original position
			if (currentDraggedXPosition != draggedOriginalXPosition)
				fixPointBeamHeights[draggedOriginalXPosition] = -1;

			fixPointBeamHeights[currentDraggedXPosition] = velocityBeamTable[currentDraggedXPosition]->getBeamValueFromLocalPoint(localPoint);

			return true;
		}
	}

	return false;
}

void VelocityCurveLinearDrawingStrategy::mouseUp(const MouseEvent &event)
{
	draggedOriginalXPosition = -1;

	// Change values of velocityBeamTable
	
	// First position
	int lineStartXPosition = 0;
	velocityBeamTable[0]->setValue(fixPointBeamHeights[0]);	// Must have a valid value
	
	int lineStopXPosition = -1;
	for (int x = 1; x < 128; x++)
	{
		if (fixPointBeamHeights[x] != -1)
		{
			// New fixed point found
			lineStopXPosition = x;

			// Calculate positions in-between and end
			for (int x2 = lineStartXPosition + 1; x2 <= lineStopXPosition; x2++)
			{
				velocityBeamTable[x2]->setValue(
					fixPointBeamHeights[lineStartXPosition] +
					(fixPointBeamHeights[lineStopXPosition] - fixPointBeamHeights[lineStartXPosition])
						* ((x2 - lineStartXPosition)) / (lineStopXPosition - lineStartXPosition));
			}

			// Current end position becomes new start position
			lineStartXPosition = lineStopXPosition;
		}
	}

	ClearSuperfluousPoints();
}

void VelocityCurveLinearDrawingStrategy::ClearSuperfluousPoints()
{
	int lineStartXPosition = 0;
	int lineStopXPosition = -1;
	float previousLineSegmentRiseRatio;

	// Identify first segment
	for (int x = 1; x < 128; x++)
	{
		if (fixPointBeamHeights[x] != -1)
		{
			// New fixed point found
			lineStopXPosition = x;

			previousLineSegmentRiseRatio = (float)(fixPointBeamHeights[lineStopXPosition] - fixPointBeamHeights[lineStartXPosition])
				/ (float)(lineStopXPosition - lineStartXPosition);

			break;
		}
	}

	jassert(lineStopXPosition != -1);

	// Current end position becomes new start position
	int previousLineStartXPosition = lineStartXPosition;
	lineStartXPosition = lineStopXPosition;

	// Following segments
	for (int x = lineStartXPosition + 1; x < 128; x++)
	{
		if (fixPointBeamHeights[x] != -1)
		{
			// New fixed point found
			lineStopXPosition = x;

			float newLineSegmentRiseRatio = (float)(fixPointBeamHeights[lineStopXPosition] - fixPointBeamHeights[lineStartXPosition])
				/ (float)(lineStopXPosition - lineStartXPosition);

			if (newLineSegmentRiseRatio == previousLineSegmentRiseRatio)
			{
				// New segment is direct continuation of previous segment. Remove segment start point, if it is not the first.
				if (lineStartXPosition > 0)		// may be == 0 if previously a segment was cleared, making the first segment the current one 
				{
					fixPointBeamHeights[lineStartXPosition] = -1;
					lineStartXPosition = previousLineStartXPosition;
				}
			}
			else
			{
				// New segment has started. Current end position becomes new start position.
				previousLineStartXPosition = lineStartXPosition;
				lineStartXPosition = lineStopXPosition;
				previousLineSegmentRiseRatio = newLineSegmentRiseRatio;
			}
		}
	}
}