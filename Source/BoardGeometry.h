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

	int horizontalLineCount() { return this->horizontalLines.size(); }
	int rightUpwardLineCount() { return this->rightUpwardLines.size(); }

	int horizontalLineSize(int rowIndex) { return (rowIndex >= 0 && rowIndex < horizontalLineCount()) ? this->horizontalLines[rowIndex].size() : 0; }
	int firstColumnOffset(int rowIndex) { return (rowIndex >= 0 && rowIndex < firstColumnOffsets.size()) ? this->firstColumnOffsets[rowIndex] : 0; }

	StraightLineSet globalHorizontalLineOfField(int setSelection, int fieldIndex);
	StraightLineSet globalRightUpwardLineOfField(int setSelection, int fieldIndex);

	StraightLineSet getHorizontalLinesWithContinuation(int octaveBoardOffset);

	StraightLine getLineOfField(int fieldIndex, StraightLineSet lineSet);
	StraightLine continuationOfHorizontalLine(StraightLine line, int octaveBoardOffset);
	StraightLine continuationOfRightUpwardLine(StraightLine line, int octaveBoardOffset);

	// Attributes
private:
	StraightLineSet	horizontalLines;
	StraightLineSet	rightUpwardLines;
	Array<int>		firstColumnOffsets;
};

//==============================================================================
// Used to calculate the center point coordinates for a hexagon tiling

class HexagonTilingGeometry
{
public:

	HexagonTilingGeometry(TerpstraBoardGeometry& boardGeometry);

	//void setOctaveBounds(Rectangle<float> octaveBoundsIn);

	//void setMarginSize(float marginSize);

	//void setRotationAngle(float rotationAngle);

	void setParameters(Rectangle<float> octaveBoundsIn, float marginSize, float rotateAngle);

	/// <summary>
	/// Returns the circumradius of a hexagon
	/// </summary>
	/// <returns></returns>
	float getKeySize(bool scaled = true);

	/// <summary>
	/// Get the centres of each hexagon before the the transformation.
	/// </summary
	/// <param name="useBoardBounds">Octaves will have full keyboard coordinates</param>
	/// <param name="octaveIndex"></param>
	/// <returns></returns>
	Array<Point<float>> getHexagonCentres(bool useBoardBounds = false, int octaveBoardOffset = 0);

	/// <summary>
	/// Returns the full transform of the octave
	/// </summary>
	/// <returns></returns>
	AffineTransform getTransform(bool withoutCentreTranslation = false);

	/// <summary>
	/// Returns the smallest rectangle containing the octave before transformation
	/// </summary>
	/// <returns></returns>
	Rectangle<float> getTileBounds() { return tileBounds; };

private:

	// Finds the best radius given the bounds, margin, and the octave board shape
	double calculateBestRadius();

	void recalculateProperties();

	void recalculateTransform();

	void rebuildCentres();

	Rectangle<float> calculateTileBounds();

private:

	TerpstraBoardGeometry& terpstraBoardGeometry;

	const int numRows;
	int widestRowCount;

	//=======================================================================
	// Parameters
	Rectangle<float> octaveBounds;
	float margin = 0;
	float angle = 0;

	//=======================================================================
	// Properties based on parameters

	double circumRadius;        // derived from widestRowCount, bounds, and margin
	double incircleRadius;      // derived from circumRadius
	double incircleDiameter;    // derived from circumRadius
	double verticalMarginUnit;  // derived from circumRadius and margin

	float currentScaleFactor = 1.0f;
	AffineTransform transform;
	Array<Point<float>> hexagonCentres;
	
	Rectangle<float> tileBounds;

	//=======================================================================
	// Hexagon geometry constants (proportional to cirumradius)
	const double incircleDiameterRatio = sqrtf(3);
	const double incircleRadiusRatio = incircleDiameterRatio / 2.0f;
};


#endif  // BOARDGEOMETRY_H_INCLUDED
