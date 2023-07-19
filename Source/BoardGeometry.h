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
	StraightLine horizontalLineOfField(int fieldIndex) const { return getLineOfField(fieldIndex, this->horizontalLines); };
	StraightLine rightUpwardLineOfField(int fieldIndex) const { return getLineOfField(fieldIndex, this->rightUpwardLines); };

	int horizontalLineCount() const { return this->horizontalLines.size(); }
	int rightUpwardLineCount() const { return this->rightUpwardLines.size(); }

	int horizontalLineSize(int rowIndex) const { return (rowIndex >= 0 && rowIndex < horizontalLineCount()) ? this->horizontalLines[rowIndex].size() : 0; }
	int firstColumnOffset(int rowIndex) const { return (rowIndex >= 0 && rowIndex < firstColumnOffsets.size()) ? this->firstColumnOffsets[rowIndex] : 0; }

	StraightLineSet globalHorizontalLineOfField(int setSelection, int fieldIndex) const;
	StraightLineSet globalRightUpwardLineOfField(int setSelection, int fieldIndex) const;

	StraightLineSet getHorizontalLinesWithContinuation(int octaveBoardOffset) const;

	StraightLine getLineOfField(int fieldIndex, StraightLineSet lineSet) const;
	StraightLine continuationOfHorizontalLine(StraightLine line, int octaveBoardOffset) const;
	StraightLine continuationOfRightUpwardLine(StraightLine line, int octaveBoardOffset) const;

	int getMaxHorizontalLineSize() const;

	Array<Point<int>> getOctaveCoordinates(int boardIndex) const;

	Point<int> coordinatesForKey (int boardIndex, int keyIndex) const;

	// Attributes
private:
	StraightLineSet	horizontalLines;
	StraightLineSet	rightUpwardLines;
	Array<int> firstColumnOffsets;
	Array<int> rowOffsets; // Horizonal offset of each horizontal line on the board, relative to the lower left key.
	int boardXOffset; // Offset along shallow diagonals between analogous keys from one board to the next (typically 7)
	int boardYOffset; // Offset along up/right diagonals between analogous keys from one board to the next (typically -2)
	int maxHorizontalLineSize;
};

#endif  // BOARDGEOMETRY_H_INCLUDED
