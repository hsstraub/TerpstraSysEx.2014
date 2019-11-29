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

bool VelocityCurveFreeDrawingStrategy::setEditConfigFromSavedString(String propertiesString)
{
	StringArray velocityCurveValueArray = StringArray::fromTokens(propertiesString, false);
	if (velocityCurveValueArray.size() > 0)
	{
		jassert(velocityCurveValueArray.size() >= 128);

		for (int x = 0; x < 128; x++)
			velocityBeamTable[x]->setValue(velocityCurveValueArray[x].getIntValue());
	}
	else
	{
		// Initialize velocity lookup table
		for (int x = 0; x < 128; x++)
			velocityBeamTable[x]->setValue(x);
	}

	return true;
}

String VelocityCurveFreeDrawingStrategy::createPropertiesStringForSaving()
{
	String velocityCurveString;

	for (int x = 0; x < 128; x++)
		velocityCurveString += String(velocityBeamTable[x]->getValue()) + " ";

	return velocityCurveString;
}

void VelocityCurveFreeDrawingStrategy::paint(Graphics& g, LookAndFeel& lookAndFeel)
{
	if (!drawedLine.isEmpty())
		g.strokePath(drawedLine, PathStrokeType(1.000f));
}

void VelocityCurveFreeDrawingStrategy::resized()
{
	drawedLine.clear();
}

bool VelocityCurveFreeDrawingStrategy::mouseDown(const MouseEvent &event, Point<float> localPoint)
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

bool VelocityCurveFreeDrawingStrategy::mouseDrag(const MouseEvent &event, Point<float> localPoint)
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

void VelocityCurveFreeDrawingStrategy::mouseUp(const MouseEvent &event, Point<float> localPoint)
{
	drawedLine.clear();
}

/*
==============================================================================
VelocityCurveSegmentEditStrategyBase class
==============================================================================
*/

VelocityCurveSegmentEditStrategyBase::VelocityCurveSegmentEditStrategyBase(Path& beamTableFrameRef, VelocityCurveBeam** velocityBeamTablePtr)
	: VelocityCurveEditStrategyBase(beamTableFrameRef, velocityBeamTablePtr)
	, mouseXPosition(-1), draggedOriginalXPosition(-1)
{
	fixPointBeamHeights[0] = 0;
	
	for (int x = 1; x < 127; x++)
		fixPointBeamHeights[x] = -1;

	fixPointBeamHeights[127] = 127;
}

void VelocityCurveSegmentEditStrategyBase::paint(Graphics& g, LookAndFeel& lookAndFeel)
{
	drawCurve(g, lookAndFeel);

	// Segment points
	drawSegmentPoints(g, lookAndFeel);
}

bool VelocityCurveSegmentEditStrategyBase::mouseMove(const MouseEvent &event, Point<float> localPoint)
{
	int newMouseXPosition = -1;
	for (int x = 0; x < 128; x++)
	{
		Rectangle<int> beamRect = velocityBeamTable[x]->getBounds();
		if (beamRect.contains(localPoint.toInt()))
		{
			newMouseXPosition = x;
			break;
		}
	}

	if (newMouseXPosition != mouseXPosition)
	{
		mouseXPosition = newMouseXPosition;
		return true;
	}
	else
		return false;
}

bool VelocityCurveSegmentEditStrategyBase::mouseDown(const MouseEvent &event, Point<float> localPoint)
{
	if (event.mods.isLeftButtonDown())
	{
		// Start position to drag
		draggedOriginalXPosition = mouseXPosition;

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
				// Dragging possible until next line point
				for (int x = draggedOriginalXPosition - 1; x > 0 && fixPointBeamHeights[x] == -1; x--)
					minDragXPosition = x;

				for (int x2 = draggedOriginalXPosition + 1; x2 < 127 && fixPointBeamHeights[x2] == -1; x2++)
					maxDragXPosition = x2;
			}
		}

		if (draggedOriginalXPosition >= 0)
		{
			fixPointBeamHeights[draggedOriginalXPosition] = velocityBeamTable[draggedOriginalXPosition]->getBeamValueFromLocalPoint(localPoint);
			return true;
		}
	}
	else if (event.mods.isRightButtonDown())
	{
		// If mouse is on an interior segment point, remove it
		if (mouseXPosition > 0 && mouseXPosition < 127 && fixPointBeamHeights[mouseXPosition] != -1)
		{
			fixPointBeamHeights[mouseXPosition] = -1;
			return true;
		}
	}

	return false;
}

bool VelocityCurveSegmentEditStrategyBase::mouseDrag(const MouseEvent &event, Point<float> localPoint)
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

		if (currentDraggedXPosition >= 0)
		{
			if (currentDraggedXPosition < minDragXPosition)
				currentDraggedXPosition = minDragXPosition;
			else if (currentDraggedXPosition > maxDragXPosition)
				currentDraggedXPosition = maxDragXPosition;

			// If x-position changed: remove point of original position
			if (currentDraggedXPosition != draggedOriginalXPosition)
			{
				fixPointBeamHeights[draggedOriginalXPosition] = -1;
				draggedOriginalXPosition = currentDraggedXPosition;
			}

			fixPointBeamHeights[currentDraggedXPosition] = velocityBeamTable[currentDraggedXPosition]->getBeamValueFromLocalPoint(localPoint);

			return true;
		}
	}

	return false;
}

void VelocityCurveSegmentEditStrategyBase::mouseUp(const MouseEvent &event, Point<float> localPoint)
{
	draggedOriginalXPosition = -1;

	// Change values of velocityBeamTable
	setVelocityTableValuesFromEditConfig();
}

Array<Point<float>> VelocityCurveSegmentEditStrategyBase::getSegmentPoints()
{
	Array<Point<float>> result;

	// First point
	Point<float> pt = velocityBeamTable[0]->getBottomMid();
	pt.setY(pt.y - velocityBeamTable[0]->getBeamHeightFromValue(fixPointBeamHeights[0]));

	result.add(pt);

	// points in-between
	for (int x = 1; x < 128; x++)
	{
		if (fixPointBeamHeights[x] != -1)
		{
			pt = velocityBeamTable[x]->getBottomMid();
			pt.setY(pt.y - velocityBeamTable[x]->getBeamHeightFromValue(fixPointBeamHeights[x]));
			result.add(pt);
		}
	}

	return result;
}

void VelocityCurveSegmentEditStrategyBase::drawSegmentPoints(Graphics& g, LookAndFeel& lookAndFeel)
{
	g.setColour(lookAndFeel.findColour(GroupComponent::outlineColourId));

	// Circles around the point
	for (int x = 0; x < 128; x++)
	{
		if (fixPointBeamHeights[x] != -1)
		{
			Point<float> pt = velocityBeamTable[x]->getBottomMid();
			pt.setY(pt.y - velocityBeamTable[x]->getBeamHeightFromValue(fixPointBeamHeights[x]));
			
			Path currentNodePath;
			currentNodePath.startNewSubPath(pt);

			if (x == mouseXPosition)
			{
				// Emphasize current point
				currentNodePath.addEllipse(pt.x - 5.5, pt.y - 5.5, 11, 11);
				g.strokePath(currentNodePath, PathStrokeType(2.000f));
			}
			else
			{
				currentNodePath.addEllipse(pt.x - 4, pt.y - 4, 8, 8);
				g.strokePath(currentNodePath, PathStrokeType(1.000f));
			}
		}
	}
}

void VelocityCurveSegmentEditStrategyBase::drawCurve(Graphics& g, LookAndFeel& lookAndFeel)
{
	g.setColour(lookAndFeel.findColour(GroupComponent::outlineColourId));
	Path drawedLine = createCurveToDraw();
	g.strokePath(drawedLine, PathStrokeType(1.000f));
}

/*
==============================================================================
VelocityCurveLinearDrawingStrategy class
==============================================================================
*/

VelocityCurveLinearDrawingStrategy::VelocityCurveLinearDrawingStrategy(Path& beamTableFrameRef, VelocityCurveBeam** velocityBeamTablePtr)
	: VelocityCurveSegmentEditStrategyBase(beamTableFrameRef, velocityBeamTablePtr)
{
}

bool VelocityCurveLinearDrawingStrategy::setEditConfigFromVelocityTable()
{
	// Take segments from current beam values, with superfluous points removed
	for (int x = 0; x < 128; x++)
	{
		fixPointBeamHeights[x] = velocityBeamTable[x]->getValue();
	}

	clearSuperfluousPoints();

	return true;	// This works always
}

void VelocityCurveLinearDrawingStrategy::setVelocityTableValuesFromEditConfig()
{
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

	clearSuperfluousPoints();
}

bool VelocityCurveLinearDrawingStrategy::setEditConfigFromSavedString(String propertiesString)
{
	if (propertiesString.startsWith("LINEAR"))
	{
		StringArray velocityCurveValueArray = StringArray::fromTokens(propertiesString.substring(6), false);
		if (velocityCurveValueArray.size() > 0)
		{
			jassert(velocityCurveValueArray.size() >= 128);

			for (int x = 0; x < 128; x++)
				fixPointBeamHeights[x] = velocityCurveValueArray[x].getIntValue();
		}
		else
		{
			// Initialize segment table
			for (int x = 0; x < 128; x++)
				fixPointBeamHeights[x] = -1;
		}

		setVelocityTableValuesFromEditConfig();

		return true;
	}
	else
		return false;
}

String VelocityCurveLinearDrawingStrategy::createPropertiesStringForSaving()
{
	String velocityCurveString = "LINEAR";

	for (int x = 0; x < 128; x++)
		velocityCurveString += String(fixPointBeamHeights[x]) + " ";

	return velocityCurveString;
}

Path VelocityCurveLinearDrawingStrategy::createCurveToDraw()
{
	Array<Point<float>> segmentPoints = getSegmentPoints();

	Path drawedCurve;
	drawedCurve.startNewSubPath(segmentPoints[0]);

	// points in-between
	for (int x = 1; x < segmentPoints.size(); x++)
		drawedCurve.lineTo(segmentPoints[x]);

	return drawedCurve;
}

void VelocityCurveLinearDrawingStrategy::clearSuperfluousPoints()
{
	int lineStartXPosition = 0;
	int lineStopXPosition = -1;
	int previousLineSegmentRiseRatio;

	// Identify first segment
	for (int x = 1; x < 128; x++)
	{
		if (fixPointBeamHeights[x] != -1)
		{
			// New fixed point found
			lineStopXPosition = x;

			previousLineSegmentRiseRatio = (fixPointBeamHeights[lineStopXPosition] - fixPointBeamHeights[lineStartXPosition])*128
				/ (lineStopXPosition - lineStartXPosition);

			break;
		}
	}

	jassert(lineStopXPosition != -1);

	// Current end position becomes new start position
	lineStartXPosition = lineStopXPosition;

	// Following segments
	for (int x = lineStartXPosition + 1; x < 128; x++)
	{
		if (fixPointBeamHeights[x] != -1)
		{
			// New fixed point found
			lineStopXPosition = x;

			int newLineSegmentRiseRatio = (fixPointBeamHeights[lineStopXPosition] - fixPointBeamHeights[lineStartXPosition])*128
				/ (lineStopXPosition - lineStartXPosition);

			if (newLineSegmentRiseRatio == previousLineSegmentRiseRatio)
			{
				// New segment is direct continuation of previous segment. Remove segment start point.
				fixPointBeamHeights[lineStartXPosition] = -1;
			}
			else
			{
				// New segment has a different rise ratio than the preiouvs, must stay
				previousLineSegmentRiseRatio = newLineSegmentRiseRatio;
			}

			lineStartXPosition = lineStopXPosition;		// Current position becomes new start position 
		}
	}
}

/*
==============================================================================
VelocityCurveQuadraticDrawingStrategy class
==============================================================================
*/

VelocityCurveQuadraticDrawingStrategy::VelocityCurveQuadraticDrawingStrategy(Path& beamTableFrameRef, VelocityCurveBeam** velocityBeamTablePtr)
	: VelocityCurveSegmentEditStrategyBase(beamTableFrameRef, velocityBeamTablePtr)
{
}

bool VelocityCurveQuadraticDrawingStrategy::setEditConfigFromVelocityTable()
{
	// ToDo

	return true;	// This works always
}

void VelocityCurveQuadraticDrawingStrategy::setVelocityTableValuesFromEditConfig()
{
	Path drawedclosedLine = createCurveToDraw();

	Point<float> ptLeftBottom = velocityBeamTable[0]->getBottomMid();
	ptLeftBottom.addXY(-1, 1);	// To make sure the testLines start inside the drawedClosedLine
	Point<float> ptRightBottom = velocityBeamTable[127]->getBottomMid();
	ptRightBottom.addXY(1, 1);

	drawedclosedLine.lineTo(ptRightBottom);
	drawedclosedLine.lineTo(ptLeftBottom);
	drawedclosedLine.closeSubPath();

	int velTableHeight = velocityBeamTable[0]->getHeight()+1;	// +1 to make sure testLines end outside the drawedClosedLine

	for (int x = 0; x < 128; x++)
	{
		Point<float> ptBot = velocityBeamTable[x]->getBottomMid();
		Point<float> ptTop = ptBot;
		ptTop.addXY(0, -velTableHeight);
		
		juce::Line<float> testLine(ptBot, ptTop);

		juce::Line<float> clippedLine = drawedclosedLine.getClippedLine(testLine, false);

		int newBeamValue = velocityBeamTable[x]->getBeamValueFromLocalPoint(clippedLine.getEnd());
		velocityBeamTable[x]->setValue(newBeamValue);
	}
}

bool VelocityCurveQuadraticDrawingStrategy::setEditConfigFromSavedString(String propertiesString)
{
	if (propertiesString.startsWith("Quadratic"))
	{
		StringArray velocityCurveValueArray = StringArray::fromTokens(propertiesString.substring(6), false);
		if (velocityCurveValueArray.size() > 0)
		{
			jassert(velocityCurveValueArray.size() >= 128);

			for (int x = 0; x < 128; x++)
				fixPointBeamHeights[x] = velocityCurveValueArray[x].getIntValue();
		}
		else
		{
			// Initialize segment table
			for (int x = 0; x < 128; x++)
				fixPointBeamHeights[x] = -1;
		}

		setVelocityTableValuesFromEditConfig();

		return true;
	}
	else
		return false;
}

String VelocityCurveQuadraticDrawingStrategy::createPropertiesStringForSaving()
{
	String velocityCurveString = "Quadratic";

	for (int x = 0; x < 128; x++)
		velocityCurveString += String(fixPointBeamHeights[x]) + " ";

	return velocityCurveString;
}

Path VelocityCurveQuadraticDrawingStrategy::createCurveToDraw()
{
	Array<Point<float>> segmentPoints = getSegmentPoints();

	Path drawedCurve;
	drawedCurve.startNewSubPath(segmentPoints[0]);

	// points in-between
	int x;
	for (x = 1; x < segmentPoints.size()-1; x += 2)
	{
		drawedCurve.quadraticTo(segmentPoints[x], segmentPoints[x + 1]);
	}

	// If last segment is not complete, draw it as line
	if (segmentPoints.size() % 2 == 0)
	{
		drawedCurve.lineTo(segmentPoints.getLast());
	}

	return drawedCurve;
}
