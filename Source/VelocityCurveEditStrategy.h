/*
  ==============================================================================

    VelocityCurveEditStrategy.h
    Created: 10 Apr 2019 9:54:14pm
    Author:  hsstraub

  ==============================================================================
*/

#ifndef VELOCITYCURVEEDITSTRATEGY_H_INCLUDED
#define VELOCITYCURVEEDITSTRATEGY_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "VelocityCurveComponents.h"


/*
==============================================================================
Base class for velocity curve editing
==============================================================================
*/
class VelocityCurveEditStrategyBase
{
public:
	VelocityCurveEditStrategyBase(Path& beamTableFrameRef, VelocityCurveBeam** velocityBeamTablePtr);

	virtual void paint(Graphics& g) = 0;
	// Resize functionality. Assumes that beamTableFrameRef has already been resized.
	virtual void resized() { }
	virtual void mouseMove(const MouseEvent &event) { };
	// return: true if some editing was done
	virtual bool mouseDown(Point<float> localPoint) { return false; }
	virtual bool mouseDrag(Point<float> localPoint) { return false; };
	virtual void mouseUp(const MouseEvent &event) { };

protected:
	Path& beamTableFrame;
	VelocityCurveBeam** velocityBeamTable;
};

/*
==============================================================================
Velocity curve editing via free drawing
==============================================================================
*/
class VelocityCurveFreeDrawingStrategy : public VelocityCurveEditStrategyBase
{
public:
	VelocityCurveFreeDrawingStrategy(Path& beamTableFrameRef, VelocityCurveBeam** velocityBeamTablePtr);

	void paint(Graphics& g) override;
	void resized() override;
	bool mouseDown(Point<float> localPoint) override;
	bool mouseDrag(Point<float> localPoint) override;
	void mouseUp(const MouseEvent &event) override;

protected:
	Path drawedLine;
};

/*
==============================================================================
Velocity curve editing via line segments
==============================================================================
*/

class VelocityCurveLinearDrawingStrategy : public VelocityCurveEditStrategyBase
{
public:
	VelocityCurveLinearDrawingStrategy(Path& beamTableFrameRef, VelocityCurveBeam** velocityBeamTablePtr);

	void paint(Graphics& g) override;
	//void resized() override;
	bool mouseDown(Point<float> localPoint) override;
	bool mouseDrag(Point<float> localPoint) override;
	void mouseUp(const MouseEvent &event) override;

protected:
	bool isDragging() { return draggedOriginalXPosition >= 0; }

	// y-components of vector line point, -1 if no line points
	int fixPointBeamHeights[128];

	int draggedOriginalXPosition;
	int minDragXPosition;
	int maxDragXPosition;
};


#endif  // VELOCITYCURVEEDITSTRATEGY_H_INCLUDED
