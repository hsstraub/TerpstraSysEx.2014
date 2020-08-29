/*
  ==============================================================================

    GroupHandle.cpp
    Created: 29 May 2020 4:33:46pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "GroupHandle.h"

GroupHandle::GroupHandle(int groupIndexIn, int degreeIndexIn, bool addsGroupIn, bool clockwiseDragIn)
	:	groupIndex(groupIndexIn),
		degreeIndex(degreeIndexIn),
		addsGroup(addsGroupIn),
		clockwiseDrag(clockwiseDragIn)
{
	colour = Colours::black;
}

int GroupHandle::getGroupIndex() const
{
	return groupIndex;
}

int GroupHandle::getDegreeIndex() const
{
	return degreeIndex;
}

bool GroupHandle::addsGroupWhenDragged() const
{
	return addsGroup;
}

bool GroupHandle::isDraggingClockwise() const
{
	return clockwiseDrag;
}

/*
	Returns polar coordinates
*/
Point<float> GroupHandle::getPosition() const
{
	return position;
}

float GroupHandle::getSize() const
{
	return size;
}

/*
	Returns the path that represents this handle as a dot
*/
Path GroupHandle::getDot(float dotRadius) const
{
	Path dot;
	Point<float> p1, p2;
	float sqrtSizeSq = sqrtf(dotRadius * dotRadius * 2);
	float xv = position.y * cosf(position.x) + center.x;
	float yv = position.y * sinf(position.x) + center.y;

	p1.setXY(xv - sqrtSizeSq, yv + sqrtSizeSq);
	p2.setXY(xv + sqrtSizeSq, yv - sqrtSizeSq);

	dot.addEllipse(Rectangle<float>(p1, p2));

	return dot;
}

/*
	Returns the path that represents this handle as a line (edge)
*/
Path GroupHandle::getLine(float lineThickness) const
{
	Line<float> line = getGroupEdgeLine(center, position, size);
	Path diamond;

	if (size > 0)
		diamond.addPolygon(line.getPointAlongLineProportionally(1/size), 4, lineThickness, position.x + float_Pi / 2);

	return diamond;
}

/*
	Returns the path that represents this handle depending on the addsGroup member
*/
Path GroupHandle::getPath() const
{
	return handlePath;
}

Colour GroupHandle::getColour() const
{
	return colour;
}

bool GroupHandle::isMouseOver(const MouseEvent& event)
{
	bool isOver = handlePath.contains(event.position, 100.0f);
	setMouseOver(isOver);
	return isOver;
}

/*
	Returns thickness value.
	If represents an edge (addsGroup = false), this is thickness. If it's a dot, it's a radius.
*/

void GroupHandle::setDraggingClockwise(bool clockwiseDragIn)
{
	clockwiseDrag = clockwiseDragIn;
}

/*
	Set center position in polar coordinates
*/

void GroupHandle::setPosition(float radius, float angle, Point<float> centerIn)
{
	position = { radius, angle };
	center = centerIn;
}

void GroupHandle::setPosition(Point<float> polarCoordinates, Point<float> centerIn)
{
	position = polarCoordinates;
	center = centerIn;
}

/*
	If represents an edge(addsGroup = false), this is thickness. If it's a dot, it's a radius.
*/
void GroupHandle::setSize(float sizeIn)
{
	size = sizeIn;

	if (addsGroup)
		handlePath = getDot(size);
	else
		handlePath = getLine(lineThickness);
}

void GroupHandle::setColour(Colour colourIn)
{
	colour = colourIn;
}

void GroupHandle::setMouseOver(bool isMouseOver)
{
	if (mouseIsOver != isMouseOver)
	{
		if (mouseIsOver)
		{
			if (addsGroup)
				handlePath = getDot(size);
			else
				handlePath = getLine(lineThickness);
		}

		else
		{
			if (addsGroup)
				handlePath = getDot(size * mouseOverMultiplier);
			else
				handlePath = getLine(lineThickness * mouseOverMultiplier);
		}

		mouseIsOver = isMouseOver;
	}
}

Line<float> GroupHandle::getGroupEdgeLine(Point<float> centerCircle, Point<float> polarCoords, float ringWidthRatio)
{
	Point<float> p1, p2;

	float xm = polarCoords.y * cosf(polarCoords.x);
	float ym = polarCoords.y * sinf(polarCoords.x);

	p1 = { xm + centerCircle.x, ym + centerCircle.y };
	p2 = { xm * ringWidthRatio + centerCircle.x, ym * ringWidthRatio + centerCircle.y };
	
	return Line<float>(p1, p2);
}