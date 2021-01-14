/*
  ==============================================================================

    GroupHandle.h
    Created: 29 May 2020 4:33:46pm
    Author:  Vincenzo

	Data structure regarding UI with resizing degree groups

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class GroupHandle
{

public:

	GroupHandle(int groupIndexIn, int degreeIndexIn, bool addsGroupIn, bool clockwiseDragIn);
	~GroupHandle() {};

	int getGroupIndex() const;

	int getDegreeIndex() const;

	bool addsGroupWhenDragged() const;

	bool isDraggingClockwise() const;

	/*
		Returns polar coordinates
	*/
	Point<float> getPosition() const;

	Colour getColour() const;

	float getSize() const;

	/*
		Returns the path that represents this handle as a dot
	*/
	Path getDot(float dotRadius) const;

	/*
		Returns the path that represents this handle as a line (edge)
	*/
	Path getLine(float lineThickness) const;

	/*
		Returns the path that represents this handle depending on the addsGroup member
	*/
	Path getPath() const;

	/*
		Checks if mouse is over path. Adjusts sizes accordingly
	*/
	bool isMouseOver(const MouseEvent& event);

	/*
		Returns thickness value.
		If represents an edge (addsGroup = false), this is thickness. If it's a dot, it's a radius.
	*/

	void setDraggingClockwise(bool clockwiseDragIn);

	/*
		Set center position in polar coordinates
	*/

	void setPosition(float radius, float angle, Point<float> centerIn=Point<float>());
	void setPosition(Point<float> polarCoordinates, Point<float> centerIn = Point<float>());

	/*
		If represents an edge (addsGroup = false), this is the line length. If it's a dot, it's a radius.
	*/
	void setSize(float sizeIn);
	void setColour(Colour colourIn);

	void setMouseOver(bool isMouseOver);

public:

	static Line<float> getGroupEdgeLine(Point<float> centerCircle, Point<float> polarCoords, float ringWidthRatio);

private:

	int groupIndex;
	int degreeIndex;
	bool addsGroup;
	bool clockwiseDrag;

	Point<float> position;
	Point<float> center;

	float size;
	Colour colour;
	bool mouseIsOver = false;

	const float lineThickness = 7.0f;
	const float mouseOverMultiplier = 1.5f;

	Path handlePath;
};