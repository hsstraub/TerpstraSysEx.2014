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
	// The horizontal lines of a 55 key subset. From left to right
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
	for (auto line: lineSet)
	{
		if (line.contains(fieldIndex))
			return line;
	}

	// Line must have been found - we should never get here
	jassert(false);
	return StraightLine();	// Defensive code: return empty object
}

// Returns the line that is a continuation of the given horizontal line in another octave board
TerpstraBoardGeometry::StraightLine TerpstraBoardGeometry::continuationOfHorizontalLine(StraightLine line, int octaveBoardOffset)
{
    int currentLineIndex = this->horizontalLines.indexOf(line);
    if ( currentLineIndex < 0)
    {
        jassert(false);         // Should not happen
        return StraightLine();	// return empty object
    }

    int otherSubBoardLineIndex = currentLineIndex - 2 * octaveBoardOffset;
    if (otherSubBoardLineIndex < 0 || otherSubBoardLineIndex >= horizontalLineCount())
    {
        // Line does not continue on other octave board
        return StraightLine();
    }

    return this->horizontalLines[otherSubBoardLineIndex];
}

// Returns the line that is a continuation of the given horizontal line in another octave board
TerpstraBoardGeometry::StraightLine TerpstraBoardGeometry::continuationOfRightUpwardLine(StraightLine line, int octaveBoardOffset)
{
	int currentLineIndex = this->rightUpwardLines.indexOf(line);
	if (currentLineIndex < 0)
	{
		jassert(false);         // Should not happen
		return StraightLine();	// return empty object
	}

	int otherSubBoardLineIndex = currentLineIndex - 7 * octaveBoardOffset;
	if (otherSubBoardLineIndex < 0 || otherSubBoardLineIndex >= rightUpwardLineCount())
	{
		// Line does not continue on other octave board
		return StraightLine();
	}

	return this->rightUpwardLines[otherSubBoardLineIndex];
}

// Returns the unique horizontal line that contains the given field, over all octave boards
TerpstraBoardGeometry::StraightLineSet TerpstraBoardGeometry::globalHorizontalLineOfField(int setSelection, int fieldIndex)
{
    jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS);

    StraightLineSet result;

    int octaveBoardIndex = 0;

    // Initialize line segments before current octave board
    for (; octaveBoardIndex < setSelection; octaveBoardIndex++)
    {
        result.add(StraightLine());
    }

    // The line for the given field in the given octave board
    StraightLine line = horizontalLineOfField(fieldIndex);
    result.add(line);
    octaveBoardIndex++;

    // Line segments in succeeding octave boards (further right)
    for (; octaveBoardIndex < NUMBEROFBOARDS && !line.isEmpty(); octaveBoardIndex++)
    {
        line = continuationOfHorizontalLine(line, 1);
        result.add(line);
    }

    // Line segments in preceding octave boards
    line = result[setSelection];
    octaveBoardIndex = setSelection-1;

    for (; octaveBoardIndex >=0 && !line.isEmpty(); octaveBoardIndex--)
    {
        line = continuationOfHorizontalLine(line, -1);
        result.set(octaveBoardIndex, line);
    }

    return result;
}

// Returns the unique right upward line that contains the given field, over all octave boards
TerpstraBoardGeometry::StraightLineSet TerpstraBoardGeometry::globalRightUpwardLineOfField(int setSelection, int fieldIndex)
{
	jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS);

	StraightLineSet result;

	int octaveBoardIndex = 0;

	// Initialize line segments before current octave board
	for (; octaveBoardIndex < setSelection; octaveBoardIndex++)
	{
		result.add(StraightLine());
	}

	// The line for the given field in the given octave board
	StraightLine line = rightUpwardLineOfField(fieldIndex);
	result.add(line);
	octaveBoardIndex++;

	// Line segments in succeeding octave boards (further right)
	for (; octaveBoardIndex < NUMBEROFBOARDS && !line.isEmpty(); octaveBoardIndex++)
	{
		line = continuationOfRightUpwardLine(line, 1);
		result.add(line);
	}

	// Line segments in preceding octave boards
	line = result[setSelection];
	octaveBoardIndex = setSelection - 1;

	for (; octaveBoardIndex >= 0 && !line.isEmpty(); octaveBoardIndex--)
	{
		line = continuationOfRightUpwardLine(line, -1);
		result.set(octaveBoardIndex, line);
	}

	return result;
}

// Returns the horizontal lines that have a continuation (right or left)
TerpstraBoardGeometry::StraightLineSet TerpstraBoardGeometry::getHorizontalLinesWithContinuation(int octaveBoardOffset)
{
	StraightLineSet result;

	for (auto line : horizontalLines)
    {
        if (continuationOfHorizontalLine(line, octaveBoardOffset).size() > 0)
            result.add(line);
    }

	return result;
}
