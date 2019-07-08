/*
  ==============================================================================

    BoardGeometry.cpp
    Created: 9 May 2016 9:04:35pm
    Author:  hsstraub

  ==============================================================================
*/

#include "BoardGeometry.h"
#include "KeyboardDataStructure.h"


//==============================================================================
// TerpstraBoardGeometry class

TerpstraBoardGeometry::TerpstraBoardGeometry()
{
	// The horizontal lines of a 56 key subset. From left to right
	this->horizontalLines.add(StraightLine({ 0, 1 }));
	
	this->horizontalLines.add(StraightLine({ 2, 3, 4, 5, 6, 7 }));
	this->horizontalLines.add(StraightLine({ 8, 9, 10, 11, 12, 13 }));
	this->horizontalLines.add(StraightLine({ 14, 15, 16, 17, 18, 19 }));
	this->horizontalLines.add(StraightLine({ 20, 21, 22, 23, 24, 25 }));
	this->horizontalLines.add(StraightLine({ 26, 27, 28, 29, 30, 31 }));
	this->horizontalLines.add(StraightLine({ 32, 33, 34, 35, 36, 37 }));
	this->horizontalLines.add(StraightLine({ 38, 39, 40, 41, 42, 43 }));
	this->horizontalLines.add(StraightLine({ 44, 45, 46, 47, 48, 49 }));

	this->horizontalLines.add(StraightLine({ 50, 51, 52, 53, 54 }));
	this->horizontalLines.add(StraightLine({ 55 }));

	// The right upward lines of a 56 key subset. From bottom to top
	this->rightUpwardLines.add(StraightLine({ 0 }));
	this->rightUpwardLines.add(StraightLine({ 8, 2, 1 }));
	this->rightUpwardLines.add(StraightLine({ 20, 14, 9, 3 }));
	this->rightUpwardLines.add(StraightLine({ 32, 26, 21, 15, 10, 4 }));
	this->rightUpwardLines.add(StraightLine({ 44, 38, 33, 27, 22, 16, 11, 5 }));
	this->rightUpwardLines.add(StraightLine({ 45, 39, 34, 28, 23, 17, 12, 6 }));
	this->rightUpwardLines.add(StraightLine({ 50, 46, 40, 35, 29, 24, 18, 13, 7 }));
	this->rightUpwardLines.add(StraightLine({ 51, 47, 41, 36, 30, 25, 19 }));
	this->rightUpwardLines.add(StraightLine({ 52, 48, 42, 37, 31 }));
	this->rightUpwardLines.add(StraightLine({ 53, 49, 43 }));
	this->rightUpwardLines.add(StraightLine({ 55, 54 }));

	this->firstColumnOffsets = Array<int>({0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5});
}

// returns the unique straight line that contains the given field  
TerpstraBoardGeometry::StraightLine TerpstraBoardGeometry::getLineOfField(int fieldIndex, StraightLineSet lineSet)
{
	int i;
	for (i = 0; i < lineSet.size(); i++)
	{
		if (lineSet[i].contains(fieldIndex))
			return lineSet[i];
	}

	// Line must have been found - we should never get here
	jassert(false);
	return StraightLine();	// Defensive code: return empty object
}
