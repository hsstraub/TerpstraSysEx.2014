/*
  ==============================================================================

    VelocityCurveEditStrategy.h
    Created: 10 Apr 2019 9:54:14pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

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
	VelocityCurveEditStrategyBase(/*Path& beamTableFrameRef,*/ std::unique_ptr<VelocityCurveBeam>* velocityBeamTablePtr);

	// Set value table (e. g. from LMT file)
	virtual bool setEditConfig(int velocityTableValues[]) = 0;
	// Export value table (for saving in LMT file)
	virtual bool exportEditConfig(int velocityTableValues[]) = 0;
	// Takes a given velocity table and tries to extract edit parameters. Returns whether it was successful.
	virtual bool setEditConfigFromVelocityTable() { return true; }
	// Sets velocity table values from edit parameters
	virtual void setVelocityTableValuesFromEditConfig() {}

	virtual String getDescriptionText() { return "Click with the mouse in the graphics to draw the velocity curve."; }

	virtual void paint(Graphics& g, LookAndFeel& lookAndFeel) { };
	// Resize functionality. Assumes that beamTableFrame has already been resized.
	virtual void resized() { }
	// return: true if a repainting is to be done
	virtual bool mouseMove(const MouseEvent &event, juce::Point<float> localPoint) { return false; };
	// return: true if some editing was done
	virtual bool mouseDown(const MouseEvent &event, juce::Point<float> localPoint) { return false; }
	virtual bool mouseDrag(const MouseEvent &event, juce::Point<float> localPoint) { return false; };
	virtual void mouseUp(const MouseEvent &event, juce::Point<float> localPoint) { };

protected:
	//Path& beamTableFrame;
	std::unique_ptr<VelocityCurveBeam>* velocityBeamTable;
};

/*
==============================================================================
Velocity curve editing via free drawing
==============================================================================
*/
class VelocityCurveFreeDrawingStrategy : public VelocityCurveEditStrategyBase
{
public:
	VelocityCurveFreeDrawingStrategy(/*Path& beamTableFrameRef,*/ std::unique_ptr<VelocityCurveBeam>* velocityBeamTablePtr);

	bool setEditConfig(int velocityTableValues[]) override;
	bool exportEditConfig(int velocityTableValues[]) override;

	void paint(Graphics& g, LookAndFeel& lookAndFeel) override;
	void resized() override;
	bool mouseDown(const MouseEvent &event, juce::Point<float> localPoint) override;
	bool mouseDrag(const MouseEvent &event, juce::Point<float> localPoint) override;
	void mouseUp(const MouseEvent &event, juce::Point<float> localPoint) override;

protected:
	Path drawedLine;
};

/*
==============================================================================
Base class for velocity curve editing with segments
==============================================================================
*/

class VelocityCurveSegmentEditStrategyBase : public VelocityCurveEditStrategyBase
{
public:
	VelocityCurveSegmentEditStrategyBase(/*Path& beamTableFrameRef, */std::unique_ptr<VelocityCurveBeam>* velocityBeamTablePtr);

	bool setEditConfig(int velocityTableValues[]) override;
	bool exportEditConfig(int velocityTableValues[]) override;

	String getDescriptionText() override { return "Click with the mouse in the graphics to draw the velocity curve. Right-click to delete a segment point."; }

	void paint(Graphics& g, LookAndFeel& lookAndFeel) override;
	bool mouseMove(const MouseEvent &event, juce::Point<float> localPoint) override;
	bool mouseDown(const MouseEvent &event, juce::Point<float> localPoint) override;
	bool mouseDrag(const MouseEvent &event, juce::Point<float> localPoint) override;
	void mouseUp(const MouseEvent &event, juce::Point<float> localPoint) override;

protected:
	bool isDragging() { return draggedOriginalXPosition >= 0; }

	Array<juce::Point<float>> getSegmentPoints();
	virtual Path createCurveToDraw() = 0;

	void drawSegmentPoints(Graphics& g, LookAndFeel& lookAndFeel);
	void drawCurve(Graphics& g, LookAndFeel& lookAndFeel);

	// y-components of vector line point, -1 if no line points
	int fixPointBeamHeights[128];

	int mouseXPosition;
	int draggedOriginalXPosition;
	int minDragXPosition;
	int maxDragXPosition;
};

/*
==============================================================================
Velocity curve editing via line segments
==============================================================================
*/

class VelocityCurveLinearDrawingStrategy : public VelocityCurveSegmentEditStrategyBase
{
public:
	VelocityCurveLinearDrawingStrategy(/*Path& beamTableFrameRef, */std::unique_ptr<VelocityCurveBeam>* velocityBeamTablePtr);

	bool setEditConfigFromVelocityTable() override;
	void setVelocityTableValuesFromEditConfig() override;

protected:
	Path createCurveToDraw() override;

	// Points that are part of a straight line can be removed
	void clearSuperfluousPoints();
};


/*
==============================================================================
Velocity curve editing via quadratic curves
==============================================================================
*/

class VelocityCurveQuadraticDrawingStrategy : public VelocityCurveSegmentEditStrategyBase
{
public:
	VelocityCurveQuadraticDrawingStrategy(/*Path& beamTableFrameRef,*/ std::unique_ptr<VelocityCurveBeam>* velocityBeamTablePtr);

	bool setEditConfigFromVelocityTable() override;
	void setVelocityTableValuesFromEditConfig() override;

protected:
	Path createCurveToDraw() override;
};

