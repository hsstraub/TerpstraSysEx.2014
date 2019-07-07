/*
  ==============================================================================

    BoardGeometry.h
    Created: 9 May 2016 9:04:35pm
    Author:  hsstraub

  ==============================================================================
*/

#ifndef BOARDGEOMETRY_H_INCLUDED
#define BOARDGEOMETRY_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
// Basic board geometry constants

class TerpstraBoardBasicConstants
{
public:
	static const int SubBoardRowCount = 11;

	// Number of keys in each row of a subboard
	static const int SubBoardRowSizes[SubBoardRowCount];

	TerpstraBoardBasicConstants();
};

//==============================================================================
// Manages the geometry of the board - used to create isomorphic key mappings 
class TerpstraBoardGeometry
{
	// Types
public:
	typedef Array<int> StraightLine;
	typedef Array<StraightLine> StraightLineSet;

	// Construction
public:
	TerpstraBoardGeometry();

	// Methods
public:
	StraightLine horizontalLineOfField(int fieldIndex) { return getLineOfField(fieldIndex, this->horizontalLines); };
	StraightLine rightUpwardLineOfField(int fieldIndex) { return getLineOfField(fieldIndex, this->rightUpwardLines); };

private: 
	StraightLine getLineOfField(int fieldIndex, StraightLineSet lineSet);

	// Attributes
private:
	StraightLineSet	horizontalLines;
	StraightLineSet	rightUpwardLines;
};


#endif  // BOARDGEOMETRY_H_INCLUDED
