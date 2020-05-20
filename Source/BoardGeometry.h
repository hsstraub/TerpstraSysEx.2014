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

	int horizontaLineCount() { return this->horizontalLines.size(); }
	int horizontalLineSize(int rowIndex) { return (rowIndex >= 0 && rowIndex < horizontaLineCount()) ? this->horizontalLines[rowIndex].size() : 0; }
	int firstColumnOffset(int rowIndex) { return (rowIndex >= 0 && rowIndex < firstColumnOffsets.size()) ? this->firstColumnOffsets[rowIndex] : 0; }

	StraightLine continuationOfHorizontalLine(StraightLine line, int octaveBoardOffset);
	StraightLineSet globalHorizontalLineOfField(int setSelection, int fieldIndex);

private:
	StraightLine getLineOfField(int fieldIndex, StraightLineSet lineSet);

	// Attributes
private:
	StraightLineSet	horizontalLines;
	StraightLineSet	rightUpwardLines;
	Array<int>		firstColumnOffsets;
};


#endif  // BOARDGEOMETRY_H_INCLUDED
