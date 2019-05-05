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

	// Takes a given velocity table and tries to extract edit parameters. Returns whether it was successful.
	virtual bool setEditConfigFromVelocityTable() { return true; }
	// Sets velocity table values from edit parameters
	virtual void setVelocityTableValuesFromEditConfig() {}
	// Parse a saved configuration. return whether parsing was successful. 
	virtual bool setEditConfigFromSavedString(String propertiesString) = 0;
	// save current configuration in a string, for saving
	virtual String createPropertiesStringForSaving() = 0;

	virtual String getDescriptionText() { return "Click with the mouse in the graphics to draw the velocity curve."; }

	virtual void paint(Graphics& g) { };
	// Resize functionality. Assumes that beamTableFrame has already been resized.
	virtual void resized() { }
	// return: true if a repainting is to be done
	virtual bool mouseMove(const MouseEvent &event, Point<float> localPoint) { return false; };
	// return: true if some editing was done
	virtual bool mouseDown(const MouseEvent &event, Point<float> localPoint) { return false; }
	virtual bool mouseDrag(const MouseEvent &event, Point<float> localPoint) { return false; };
	virtual void mouseUp(const MouseEvent &event, Point<float> localPoint) { };

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

	bool setEditConfigFromSavedString(String propertiesString) override;
	String createPropertiesStringForSaving() override;
	
	void paint(Graphics& g) override;
	void resized() override;
	bool mouseDown(const MouseEvent &event, Point<float> localPoint) override;
	bool mouseDrag(const MouseEvent &event, Point<float> localPoint) override;
	void mouseUp(const MouseEvent &event, Point<float> localPoint) override;

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
	VelocityCurveSegmentEditStrategyBase(Path& beamTableFrameRef, VelocityCurveBeam** velocityBeamTablePtr);

	String getDescriptionText() override { return "Click with the mouse in the graphics to draw the velocity curve. Right-click to delete a segment point."; }

	bool mouseMove(const MouseEvent &event, Point<float> localPoint) override;
	bool mouseDown(const MouseEvent &event, Point<float> localPoint) override;
	bool mouseDrag(const MouseEvent &event, Point<float> localPoint) override;
	void mouseUp(const MouseEvent &event, Point<float> localPoint) override;

protected:
	bool isDragging() { return draggedOriginalXPosition >= 0; }

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
	VelocityCurveLinearDrawingStrategy(Path& beamTableFrameRef, VelocityCurveBeam** velocityBeamTablePtr);

	bool setEditConfigFromVelocityTable() override;
	void setVelocityTableValuesFromEditConfig() override;
	bool setEditConfigFromSavedString(String propertiesString) override;
	String createPropertiesStringForSaving() override;

	void paint(Graphics& g) override;

protected:
	// Points that are part of a straight line can be removed
	void clearSuperfluousPoints();
};


#endif  // VELOCITYCURVEEDITSTRATEGY_H_INCLUDED
