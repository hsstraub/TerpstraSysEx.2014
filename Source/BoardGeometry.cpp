/*
  ==============================================================================

    BoardGeometry.cpp
    Created: 9 May 2016 9:04:35pm
    Author:  hsstraub

  ==============================================================================
*/

#include "BoardGeometry.h"
#include "KeyboardDataStructure.h"
#include "Main.h"


//==============================================================================
// TerpstraBoardGeometry class

TerpstraBoardGeometry::TerpstraBoardGeometry()
{
	if (TerpstraSysExApplication::getApp().getOctaveBoardSize() == 55)
	{
		// The horizontal lines of a 55 key subset. From left to right
		this->horizontalLines.add(StraightLine({ 0, 1 }));
		this->horizontalLines.add(StraightLine({ 2, 3, 4, 5, 6 }));
		this->horizontalLines.add(StraightLine({ 7, 8, 9, 10, 11, 12 }));
		this->horizontalLines.add(StraightLine({ 13, 14, 15, 16, 17, 18 }));
		this->horizontalLines.add(StraightLine({ 19, 20, 21, 22, 23, 24 }));
		this->horizontalLines.add(StraightLine({ 25, 26, 27, 28, 29, 30 }));
		this->horizontalLines.add(StraightLine({ 31, 32, 33, 34, 35, 36 }));
		this->horizontalLines.add(StraightLine({ 37, 38, 39, 40, 41, 42 }));
		this->horizontalLines.add(StraightLine({ 43, 44, 45, 46, 47, 48 }));
		this->horizontalLines.add(StraightLine({ 49, 50, 51, 52, 53 }));
		this->horizontalLines.add(StraightLine({ 54 }));

		// The right upward lines of a 55 key subset. From bottom to top
		this->rightUpwardLines.add(StraightLine({ 0 }));
		this->rightUpwardLines.add(StraightLine({ 7, 2, 1 }));
		this->rightUpwardLines.add(StraightLine({ 19, 13, 8, 3 }));
		this->rightUpwardLines.add(StraightLine({ 31, 25, 20, 14, 9, 4 }));
		this->rightUpwardLines.add(StraightLine({ 43, 37, 32, 26, 21, 15, 10, 5 }));
		this->rightUpwardLines.add(StraightLine({ 44, 38, 33, 27, 22, 16, 11, 6 }));
		this->rightUpwardLines.add(StraightLine({ 49, 45, 39, 34, 28, 23, 17, 12 }));
		this->rightUpwardLines.add(StraightLine({ 50, 46, 40, 35, 29, 24, 18 }));
		this->rightUpwardLines.add(StraightLine({ 51, 47, 41, 36, 30 }));
		this->rightUpwardLines.add(StraightLine({ 52, 48, 42 }));
		this->rightUpwardLines.add(StraightLine({ 54, 53 }));

		this->firstColumnOffsets = Array<int>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5 });
		this->rowOffsets = Array<int>({-4, -3, -3, -2, -2, -1, -1, 0, 0, 2, 6});
		this->boardXOffset = 7;
		this->boardYOffset = -2;
	}
	else
	{
		// The horizontal lines of a 56 key subset. From left to right
		this->horizontalLines.add(StraightLine({ 0, 1 }));
		this->horizontalLines.add(StraightLine({ 2, 3, 4, 5, 6 }));
		this->horizontalLines.add(StraightLine({ 7, 8, 9, 10, 11, 12 }));
		this->horizontalLines.add(StraightLine({ 13, 14, 15, 16, 17, 18 }));
		this->horizontalLines.add(StraightLine({ 19, 20, 21, 22, 23, 24 }));
		this->horizontalLines.add(StraightLine({ 25, 26, 27, 28, 29, 30 }));
		this->horizontalLines.add(StraightLine({ 31, 32, 33, 34, 35, 36 }));
		this->horizontalLines.add(StraightLine({ 37, 38, 39, 40, 41, 42 }));
		this->horizontalLines.add(StraightLine({ 43, 44, 45, 46, 47, 48 }));
		this->horizontalLines.add(StraightLine({ 49, 50, 51, 52, 53 }));
		this->horizontalLines.add(StraightLine({ 54, 55 }));

		// The right upward lines of a 56 key subset. From bottom to top
		this->rightUpwardLines.add(StraightLine({ 0 }));
		this->rightUpwardLines.add(StraightLine({ 7, 2, 1 }));
		this->rightUpwardLines.add(StraightLine({ 19, 13, 8, 3 }));
		this->rightUpwardLines.add(StraightLine({ 31, 25, 20, 14, 9, 4 }));
		this->rightUpwardLines.add(StraightLine({ 43, 37, 32, 26, 21, 15, 10, 5 }));
		this->rightUpwardLines.add(StraightLine({ 44, 38, 33, 27, 22, 16, 11, 6 }));
		this->rightUpwardLines.add(StraightLine({ 49, 45, 39, 34, 28, 23, 17, 12 }));
		this->rightUpwardLines.add(StraightLine({ 50, 46, 40, 35, 29, 24, 18 }));
		this->rightUpwardLines.add(StraightLine({ 51, 47, 41, 36, 30 }));
		this->rightUpwardLines.add(StraightLine({ 54, 52, 48, 42 }));
		this->rightUpwardLines.add(StraightLine({ 55, 53 }));

		this->firstColumnOffsets = Array<int>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4 });
		this->rowOffsets = Array<int>({-4, -3, -3, -2, -2, -1, -1, 0, 0, 2, 5});
		this->boardXOffset = 7;
		this->boardYOffset = -2;
	}

	maxHorizontalLineSize = 0;

	for (auto line : horizontalLines)
		if (line.size() > maxHorizontalLineSize)
			maxHorizontalLineSize = line.size();

}

// Given a board number and key index, compute Cartesian coordinates for the given key,
// the x axis is the shallow diagonal, and the y axis is the right-upward diagonal.
// (0,0) is at the top left, so most coordinates are negative, but that's okay for our purposes.
Point<int> TerpstraBoardGeometry::coordinatesForKey (int boardIndex, int keyIndex) const {
	for (int lineIx = 0; lineIx < this->horizontalLines.size(); lineIx++) {
		int rowIx = this->horizontalLines[lineIx].indexOf(keyIndex);
		if (rowIx != -1) {
			return (Point<int>(this->boardXOffset * boardIndex + rowIx + this->rowOffsets[lineIx], this->boardYOffset * boardIndex - lineIx));
		}
	}
	// If we get here, the requested key was out of range.
	jassert(false);
	return Point<int>(0,0);
}

// returns the unique straight line that contains the given field
TerpstraBoardGeometry::StraightLine TerpstraBoardGeometry::getLineOfField(int fieldIndex, StraightLineSet lineSet) const
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
TerpstraBoardGeometry::StraightLine TerpstraBoardGeometry::continuationOfHorizontalLine(StraightLine line, int octaveBoardOffset) const
{
    int currentLineIndex = this->horizontalLines.indexOf(line);
    if ( currentLineIndex < 0)
    {
        jassert(false);         // Should not happen
        return StraightLine();	// return empty object
    }

    int otherSubBoardLineIndex = currentLineIndex - BOARDROWOFFSET * octaveBoardOffset;
    if (otherSubBoardLineIndex < 0 || otherSubBoardLineIndex >= horizontalLineCount())
    {
        // Line does not continue on other octave board
        return StraightLine();
    }

    return this->horizontalLines[otherSubBoardLineIndex];
}

// Returns the line that is a continuation of the given horizontal line in another octave board
TerpstraBoardGeometry::StraightLine TerpstraBoardGeometry::continuationOfRightUpwardLine(StraightLine line, int octaveBoardOffset) const
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
TerpstraBoardGeometry::StraightLineSet TerpstraBoardGeometry::globalHorizontalLineOfField(int setSelection, int fieldIndex) const
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
TerpstraBoardGeometry::StraightLineSet TerpstraBoardGeometry::globalRightUpwardLineOfField(int setSelection, int fieldIndex) const
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
TerpstraBoardGeometry::StraightLineSet TerpstraBoardGeometry::getHorizontalLinesWithContinuation(int octaveBoardOffset) const
{
	StraightLineSet result;

	for (auto line : horizontalLines)
    {
        if (continuationOfHorizontalLine(line, octaveBoardOffset).size() > 0)
            result.add(line);
    }

	return result;
}

int TerpstraBoardGeometry::getMaxHorizontalLineSize() const
{
	return maxHorizontalLineSize;
}

Array<Point<int>> TerpstraBoardGeometry::getOctaveCoordinates(int boardIndex) const
{
	Array<Point<int>> pointsOut;

	int boardColOffset = boardIndex * getMaxHorizontalLineSize();
	int boardRowOffset = boardIndex * BOARDROWOFFSET;

	for (int row = 0; row < horizontalLines.size(); row++)
	{
		for (int col = 0; col < horizontalLineSize(row); col++)
		{
			int boardCol = firstColumnOffsets[row] + col + boardColOffset;
			int boardRow = row + boardRowOffset;
			pointsOut.add(Point<int>(boardCol - (boardRow / 2), boardRow));
			// Converts column offsets from vertical rows coordinates to slanted coordinates
		}
	}

	return pointsOut;
}
