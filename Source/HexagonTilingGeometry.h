/*
  ==============================================================================

    HexagonTilingGeometry.h
    Created: 6 Jan 2021 11:42:30pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "BoardGeometry.h"
#include "KeyboardDataStructure.h"


// Hexagon geometry constants (proportional to cirumradius)
#define INCIRCLEDIAMETERRATIO 1.7320508075688f // sqrt(3)
#define INCIRCLERADIUSRATIO   0.8660254037844f // sqrt(3) / 2;

//==============================================================================
// Used to calculate the center point coordinates for a hexagon tiling


class HexagonTilingGeometry
{
public:

	HexagonTilingGeometry(int numRowsIn, int octaveColumns) 
		: numRows(numRowsIn), rowCount(octaveColumns) {};

	/// <summary>
	/// Returns the circumradius of a hexagon
	/// </summary>
	/// <returns></returns>
	float getKeySize(bool scaled = true);

	/// <summary>
	/// Get the centres of each hexagon, given bounds, margin size, and rotation
	/// </summary>
	/// <param name="boardGeometry"></param>
	/// <param name="octaveBoundsIn"></param>
	/// <param name="marginSize"></param>
	/// <param name="rotateAngle"></param>
	/// <param name="startingOctave"></param>
	/// <param name="numOctavesIn"></param>
	/// <param name="scaleToFitRotation"></param>
	/// <returns></returns>
	Array<Point<float>> getHexagonCentres(const TerpstraBoardGeometry& boardGeometry,
		Rectangle<float>	boundsIn, 
		float				marginSize, 
		float				rotateAngle, 
		int					startingOctave = 0, 
		int					numOctavesIn = 1, 
		bool				scaleToFitRotation = true);

	/// <summary>
	/// Get the centres of each hexagon given bounds and the centre point of the top left and bottom right hexagon
	/// </summary>
	/// <param name="boardGeometry"></param>
	/// <param name="boardBoundsIn">Rectangle created by bounds of first and last key.</param>
	/// <param name="topLeftCentre"></param>
	/// <param name="bottomRightCentre"></param>
	/// <param name="marginToRadiusRatio"></param>
	/// <param name="startingOctave"></param>
	/// <param name="numOctavesIn"></param>
	/// <returns></returns>
	Array<Point<float>> getHexagonCentres(const TerpstraBoardGeometry& boardGeometry,
		Rectangle<float>	boundsIn, 
		Point<float>		firstKeyCentre, 
		Point<float>		lastKeyCentre, 
		float				marginToRadiusRatio, 
		int					startingOctave = 0, 
		int					numOctavesIn = 1
	);

	/// <summary>
	/// Returns the smallest rectangle containing tiling
	/// </summary>
	/// <param name="withTransformation">If true, the bounds will be after the transformation</param>
	/// <returns></returns>
	Rectangle<float> getTileBounds(bool withTransformation = true);

private:

	// Finds the best radius given the bounds, margin, and the octave board shape
	double calculateBestRadius();

	void recalculateProperties();

	void recalculateTransform(Point<float> rotateOrigin, bool centreAndScale);

	// Finds the smallest rectangle based on tiling with board geometry since it may be smaller than bounds passed in
	Rectangle<float> calculateSmallestBounds();

	Array<Point<float>> calculateCentres(const TerpstraBoardGeometry& boardGeometry);

	Array<Point<float>> calculateCentresFromFirstKeyCorner(const TerpstraBoardGeometry& boardGeometry);

	Array<Point<float>> calculateCentresFromFirstKeyCentre(const TerpstraBoardGeometry& boardGeometry, Point<float> firstKeyCentre);

public:

	static double calculateTileWidth(int numColumns, double radiusInside, double margin);

	static double calculateTileHeight(int numRows, double radiusBounding, double margin);

	static double distanceStepsAwayX(double radiusInside, double margin, int stepsX, int stepsY);

	static double distanceStepsAwayY(double radiusBounding, double margin, int stepsY);


private:

	const int numRows;
	const int rowCount;

	//=======================================================================
	// Parameters
	Rectangle<float> bounds;
	float margin = 0;
	float angle = 0;
	bool scaleToFit = true;

	int startingOctaveIndex = 0;
	int numOctaves = 1;

	//=======================================================================
	// Properties based on parameters

	double circumRadius;        // derived from widestRowCount, bounds, and margin
	double incircleRadius;      // derived from circumRadius
	double incircleDiameter;    // derived from circumRadius
	double verticalMarginUnit;  // derived from circumRadius and margin

	float currentScaleFactor = 1.0f;
	AffineTransform transform = AffineTransform();

	Rectangle<float> tileBounds;        // Smallest rectangle containing tiles pre-transformation
	Rectangle<float> transformedBounds; // Smallest rectangle containing tiles post-transformation
};