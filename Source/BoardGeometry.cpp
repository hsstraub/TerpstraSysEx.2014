/*
  ==============================================================================

    BoardGeometry.cpp
    Created: 9 May 2016 9:04:35pm
    Author:  hsstraub

  ==============================================================================
*/

#include "BoardGeometry.h"

TerpstraBoardGeometry::TerpstraBoardGeometry()
{
	// The horizontal lines of a 56 key subset. From left to right
	this->horizontalLines.add(StraightLine({ 1, 0 }));
	
	this->horizontalLines.add(StraightLine({ 7, 6, 5, 4, 3, 2 }));
	this->horizontalLines.add(StraightLine({ 13, 12, 11, 10, 9, 8 }));
	this->horizontalLines.add(StraightLine({ 19, 18, 17, 16, 15, 14 }));
	this->horizontalLines.add(StraightLine({ 25, 24, 23, 22, 21, 20 }));
	this->horizontalLines.add(StraightLine({ 31, 30, 29, 28, 27, 26 }));
	this->horizontalLines.add(StraightLine({ 37, 36, 35, 34, 33, 32 }));
	this->horizontalLines.add(StraightLine({ 43, 42, 41, 40, 39, 38 }));
	this->horizontalLines.add(StraightLine({ 49, 48, 47, 46, 45, 44 }));

	this->horizontalLines.add(StraightLine({ 54, 53, 52, 51, 50 }));
	this->horizontalLines.add(StraightLine({ 55 }));

	// The right upward lines of a 56 key subset. From bottom to top
	this->rightUpwardLines.add(StraightLine({ 1 }));
	this->rightUpwardLines.add(StraightLine({ 13, 7, 0 }));
	this->rightUpwardLines.add(StraightLine({ 25, 19, 12, 6 }));
	this->rightUpwardLines.add(StraightLine({ 37, 31, 24, 18, 11, 5 }));
	this->rightUpwardLines.add(StraightLine({ 49, 43, 36, 30, 23, 17, 10, 4 }));
	this->rightUpwardLines.add(StraightLine({ 48, 42, 35, 29, 22, 16, 9, 3 }));
	this->rightUpwardLines.add(StraightLine({ 54, 47, 41, 34, 28, 21, 15, 8, 2 }));
	this->rightUpwardLines.add(StraightLine({ 53, 46, 40, 33, 27, 20, 14 }));
	this->rightUpwardLines.add(StraightLine({ 52, 45, 39, 32, 26 }));
	this->rightUpwardLines.add(StraightLine({ 51, 44, 38 }));
	this->rightUpwardLines.add(StraightLine({ 55, 50 }));
	// XXX


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

	// Line must have found - we should never get here
	jassert(false);
	return StraightLine();	// Ddefensive code: return empty object
}
