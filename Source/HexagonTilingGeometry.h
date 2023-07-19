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

//==============================================================================
// Used to calculate the center point coordinates for a hexagon tiling.
// This class treats a standard hexagon with a vertex aligned with the y-axis.
// Added a bunch of "skew" methods to solve the render overlay system, but I will separate that out at some point.

#define LATERALRADIUSRATIO   0.8660254037844 // sqrt(3) / 2, proportional to radius, the length between the center and a vertex)

class HexagonTilingGeometry
{
public:

	/// <summary>
	/// Creates a basic hexagon tiling with a 1x1 bounding box set at (0, 0)
	/// </summary>
	HexagonTilingGeometry() {};

	void setRadius(double radiusIn) { radius = radiusIn; }

	void setMargin(double marginIn) { margin = marginIn; }

	void setRotationAngle(double angleIn) { angle = angleIn; recalculateTransform(startingCentre.toFloat(), scaleToFit); }

	void setBounds(Rectangle<float> boundsIn) { bounds = boundsIn; }

	void scaleToFitBounds(bool doScaling) { scaleToFit = doScaling; }

	void setOriginPoint(Point<float> originPoint) { startingCentre = originPoint.toDouble(); }

	void setSkewed(bool tilingIsSkewed) { useSkewedBasis = tilingIsSkewed; }

	void setColumnAngle(double angleIn);

	void setRowAngle(double angleIn);

	/// <summary>
	//  Stretches tiling in the horizontal direction, along the axis where a line extended from the hexagon centres are perpendicular to an edge at 3 and 9 o'clock
	/// </summary>
	/// <param name="lateralScalarIn"></param>
	void setHorizontalScalar(float scalarIn) { horizontalScalar = scalarIn; }


	/// <summary>
	/// Stretches tiling in the vertical direction, along the axis where a line extended from the hexagon centres meet a vertex at 12 o'clock
	/// </summary>
	/// <param name="radiusScalarIn"></param>
	void setVerticalScalar(float scalarIn) { verticalScalar = scalarIn; }

	//==============================================================================
	// Helper functions to fit a tile to certain specifications

	/// <summary>
	/// Calculate properties of the hexagon tiling given bounds, margin size, and rotation
	/// </summary>
	/// <param name="boundsIn"></param>
	/// <param name="numColumns"></param>
	/// <param name="numRows"></param>
	/// <param name="marginSize"></param>
	/// <param name="rotateAngle"></param>
	/// <param name="scaleToFitRotation"></param>
	/// <param name="radiusScalarIn"></param>
	/// <param name="lateralScalarIn"></param>
	void fitTilingTo(
		Rectangle<float>	boundsIn, 
		int					widestRow,
		int					longestColumn,
		float				marginSize, 
		float				rotateAngle, 
		bool				scaleToFitRotation = true,
		float				radiusScalarIn = 1.0f,
		float				lateralScalarIn = 1.0f
	);

	/// <summary>
	/// Calculate properties of the hexagon tiling given certain parameters
	/// </summary>
	/// <param name="firstKeyCentre"></param>
	/// <param name="secondKeyCentre"></param>
	/// <param name="rowStepsFirstToSecond"></param>
	/// <param name="thirdKeyCentre"></param>
	/// <param name="colStepsSecondToThird"></param>
	/// <param name="correctionAngle"></param>
	void fitSkewedTiling(
		Point<float>		firstKeyCentre,
		Point<float>		secondKeyCentre,
		int					rowStepsFirstToSecond,
		Point<float>		thirdKeyCentre,
		int					colStepsSecondToThird,
		bool				calculateAngles = true
	);

	// TODO: Generalized hex plane for any shapes
	//Array<Point<float>> getHexagonCentres(const Point<float> originCentre, const Array<Array<int>> stepsFromCentre) const;

	Array<Point<float>> getHexagonCentres(const TerpstraBoardGeometry& boardGeometry, int startingOctave = 0, int numOctavesIn = 1) const;

	Array<Point<float>> getHexagonCentresSkewed(const TerpstraBoardGeometry& boardGeometry, int startingOctave, int numOctavesIn) const;

	// I have a new model of this class in the TilingGeometry branch that is based on this function, but it happened to perform worse 
	// in terms of rounding errors, so this is a quick-fix for implementing HexPalettes before I can officially clean this class up
	Array<Point<float>> transformPointsFromOrigin(Array<Point<int>> hexagonalCoordinatesIn);
	
	//==============================================================================
	// Property getters

	/// <summary>
	/// Returns the circumradius of a hexagon
	/// </summary>
	/// <returns></returns>
	float getKeySize(bool scaled = true) const;

	/// <summary>
	/// Returns the smallest rectangle containing tiling based on the last time centres were requested
	/// </summary>
	/// <param name="withTransformation">If true, the bounds will be after the transformation</param>
	/// <returns></returns>
	Rectangle<float> getRecentTileBounds(bool withTransformation = true);

	double getRadius() const { return radius; }

	double getLateral() const { return radius * LATERALRADIUSRATIO; }

	double getRadiusScaled() const { return radius * verticalScalar; }

	double getLateralScaled() const { return getLateral() * horizontalScalar; }

	double getMargin() const { return margin; }

	Point<float> getCurrentOriginPoint() const { return startingCentre.toFloat(); }

	AffineTransform getCurrentTransformation() const { return transform; }

	float getCurrentAngle() const { return angle; }

	float getHorizontalScalar() const { return horizontalScalar; };

	float getVerticalScalar() const { return verticalScalar; }

	double getColumnAngleBasis() const { return columnBasisAngle; }

	double getRowAngleBasis() const { return rowBasisAngle; }

	//==============================================================================
	
	double findBestRadius(int widestRow, int longestColumn);

private:

	void recalculateTransform(Point<float> rotateOrigin, bool centreAndScale);

	// Finds the smallest rectangle based on tiling with board geometry since it may be smaller than bounds passed in
	Rectangle<float> calculateSmallestBounds(int widestRowSize, int longestColumnSize) const;

	// Generalized ones
	//Array<Point<float>> calculateCentres(const TerpstraBoardGeometry& boardGeometry, Point<float> firstKeyCentre);

	Array<Point<float>> calculateCentres(const TerpstraBoardGeometry& boardGeometry, int startingOctave = 0, int numOctaves = 1) const;

	Array<Point<float>> calculateCentresSkewed(const TerpstraBoardGeometry& boardGeometry, int startingOctave = 0, int numOctaves = 1) const;

	static int verticalToSlantOffset(int rowNum, int offsetIn);

public:

	static double calculateTileWidth(int numColumns, double radiusInside, double margin);

	static double calculateTileHeight(int numRows, double radiusBounding, double margin);

	static double distanceStepsAwayX(double radiusInside, double margin, int stepsX, int stepsY);

	static double distanceStepsAwayY(double radiusBounding, double margin, int stepsY);

	static Point<double> getSkewedPoint(
		double columnAngleX, double columnAngleY, 
		double rowAngleX, double rowAngleY, 
		double colUnit, double rowUnit, int columnOffset, int rowOffset
	);

private:

	//=======================================================================
	// Parameters
	
	Rectangle<float> bounds = Rectangle<float>(0, 0, 1, 1);
	double radius = 0.5;

	double margin = 0.0;
	double angle = 0.0;

	bool scaleToFit = false;
	bool useSkewedBasis = false;

	float horizontalScalar = 1.0f;
	float verticalScalar = 1.0f;

	double columnBasisAngle = 0.0;
	double rowBasisAngle = -MathConstants<double>::pi / 3.0;

	Point<double> startingCentre = Point<double>(0.4330127, 0.5);

	//=======================================================================
	// Properties based on parameters

	float rotationScalar = 1.0f;
	AffineTransform transform = AffineTransform();

	double columnAngleCos = 0;
	double columnAngleSin = 0;
	double    rowAngleCos = 0;
	double    rowAngleSin = 0;

	double columnXComponent = 0;
	double columnYComponent = 0;
	double    rowXComponent = 0;
	double    rowYComponent = 0;

	Rectangle<float> tileBounds;        // Smallest rectangle containing tiles pre-transformation
	Rectangle<float> transformedBounds; // Smallest rectangle containing tiles post-transformation
};