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
	// XXX


}

// returns the unique straight line that contains the given field  
TerpstraBoardGeometry::StraightLine TerpstraBoardGeometry::horizontalLineOfField(int fieldIndex)
{
	int i;
	for (i = 0; i < this->horizontalLines.size(); i++)
	{
		if (this->horizontalLines[i].contains(fieldIndex))
			return this->horizontalLines[i];
	}

	// Line must have found - we should never get here
	jassert(false);
	return StraightLine();	// Ddefensive code: return empty object
}
