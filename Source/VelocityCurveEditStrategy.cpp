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
	, leftPoint(0, 0), rightPoint(127, 127)
{
}

void VelocityCurveLinearDrawingStrategy::paint(Graphics& g)
{
	g.setColour(Colours::black);

	// First point
	Point<float> pt = velocityBeamTable[0]->getBottomMid();
	pt.setY(pt.y - velocityBeamTable[0]->getBeamHeightFromValue(leftPoint.y));

	Path drawedLine;
	drawedLine.startNewSubPath(pt);
	
	// points in-between
	// XXX

	// Last point
	pt = velocityBeamTable[127]->getBottomMid();
	pt.setY(pt.y - velocityBeamTable[0]->getBeamHeightFromValue(rightPoint.y));
	drawedLine.lineTo(pt);

	g.strokePath(drawedLine, PathStrokeType(1.000f));

	// Circles around points, one of them possibly selected
	// XXX
}

//void VelocityCurveLinearDrawingStrategy::resized()
//{
//}
