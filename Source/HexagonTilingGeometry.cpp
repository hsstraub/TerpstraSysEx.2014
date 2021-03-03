/*
  ==============================================================================

    HexagonTilingGeometry.cpp
    Created: 6 Jan 2021 11:42:30pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "HexagonTilingGeometry.h"

void HexagonTilingGeometry::setColumnAngle(double angleIn)
{ 
	columnBasisAngle = angleIn; 
	columnAngleCos = cos(columnBasisAngle);
	columnAngleSin = sin(columnBasisAngle);
}

void HexagonTilingGeometry::setRowAngle(double angleIn)
{ 
	rowBasisAngle = angleIn;
	rowAngleCos = cos(rowBasisAngle);
	rowAngleSin = sin(rowBasisAngle);
}

void HexagonTilingGeometry::fitTilingTo(Rectangle<float> boundsIn, 
	int widestRow, int longestColumn, float marginSize, float rotateAngle, 
	bool scaleToFitRotation, float radiusScalarIn, float lateralScalarIn)
{
	bounds = boundsIn;
	margin = marginSize;
	angle = rotateAngle;
	scaleToFit = scaleToFitRotation;
	
	verticalScalar = radiusScalarIn;
	horizontalScalar = lateralScalarIn;

	radius = findBestRadius(widestRow, longestColumn);

	tileBounds = calculateSmallestBounds(widestRow, longestColumn);

	double rad = radius * verticalScalar;
	double lat = radius * LATERALRADIUSRATIO * horizontalScalar;
	startingCentre = Point<double>(bounds.getX() + lat, bounds.getY() + rad);

	recalculateTransform(tileBounds.getCentre(), true);
}

void HexagonTilingGeometry::fitSkewedTiling(Point<float> firstKeyCentre,
	Point<float> secondKeyCentre, int rowStepsFirstToSecond,
	Point<float> thirdKeyCentre,  int colStepsSecondToThird,
	bool calculateAngles)
{
	useSkewedBasis = true;
	
	startingCentre = firstKeyCentre.toDouble();
	Point<double> secondKeyNormFirst = secondKeyCentre.toDouble() - startingCentre;
	Point<double> thirdKeyNormSecond = (thirdKeyCentre - secondKeyCentre).toDouble();

	if (calculateAngles)
	{
		setColumnAngle(atan((thirdKeyCentre.y - secondKeyCentre.y) / (thirdKeyCentre.x - secondKeyCentre.x)));
		   setRowAngle(atan(secondKeyNormFirst.y / secondKeyNormFirst.x));
	}

	float colUnit = thirdKeyNormSecond.x / (colStepsSecondToThird * columnAngleCos);
	float rowUnit = secondKeyNormFirst.y / (rowStepsFirstToSecond * rowAngleSin);

	columnXComponent = colUnit * columnAngleCos;
	columnYComponent = colUnit * columnAngleSin;
	rowXComponent    = rowUnit * rowAngleCos;
	rowYComponent    = rowUnit * rowAngleSin;
}

Array<Point<float>> HexagonTilingGeometry::getHexagonCentres(const TerpstraBoardGeometry& boardGeometry, int startingOctave, int numOctavesIn) const
{
	return calculateCentres(boardGeometry, startingOctave, numOctavesIn);
}

Array<Point<float>> HexagonTilingGeometry::getHexagonCentresSkewed(const TerpstraBoardGeometry& boardGeometry, int startingOctave, int numOctavesIn) const
{
	return calculateCentresSkewed(boardGeometry, startingOctave, numOctavesIn);
}

float HexagonTilingGeometry::getKeySize(bool scaled) const
{
	float keySize = (scaled) ? radius * rotationScalar * verticalScalar : (float)radius;
	return keySize * 2.0f;
}

Rectangle<float> HexagonTilingGeometry::getRecentTileBounds(bool withTransformation)
{
	if (withTransformation)
		return transformedBounds;

	return tileBounds;
}

double HexagonTilingGeometry::calculateTileWidth(int widestRow, double radiusInside, double margin)
{
	return widestRow * (margin + 2 * radiusInside) - margin + radiusInside;
}

double HexagonTilingGeometry::calculateTileHeight(int longestColumn, double radiusBounding, double margin)
{
	return (longestColumn - 1.0) * (1.5 * radiusBounding + margin * LATERALRADIUSRATIO) + 2 * radiusBounding;
}

double HexagonTilingGeometry::distanceStepsAwayX(double lateral, double margin, int stepsX, int stepsY)
{
	return stepsX * (2 * lateral + margin) + ((stepsY + 1) / 2) * (lateral + margin / 2.0);
}

double HexagonTilingGeometry::distanceStepsAwayY(double radius, double margin, int stepsY)
{
	return stepsY * (radius * 1.5 + margin * LATERALRADIUSRATIO);
}

Point<double> HexagonTilingGeometry::getSkewedPoint(
	double columnAngleX, double columnAngleY, double rowAngleX, double rowAngleY, 
	double columnUnit, double rowUnit, int columnOffset, int rowOffset) 
{
	//double colX = columnOffset * ( lateral * (columnAngleX/* + rowAngleX)*/)) + (rowOffset % 2) * (lateral * columnAngleX) + radius * rowOffset * rowAngleX;
	//double colY = rowOffset * (radius * 1.5 * rowAngleY) + lateral * columnOffset * columnAngleY;

	double colX = columnOffset * columnUnit * columnAngleX + rowOffset * rowUnit * rowAngleX;
	double colY = columnOffset * columnUnit * columnAngleY + rowOffset * rowUnit * rowAngleY;

	return { colX, colY };
}

double HexagonTilingGeometry::findBestRadius(int widestRow, int longestColumn)
{
	double widthBased = (bounds.getWidth() - margin * (widestRow - 1)) / (2 * widestRow + 1) / LATERALRADIUSRATIO;
	double heightBased = (2 * bounds.getHeight() - margin * 2 * LATERALRADIUSRATIO * (longestColumn + 1)) / (3 * longestColumn + 1);

	return juce::jmin(widthBased, heightBased);
}

void HexagonTilingGeometry::recalculateTransform(Point<float> rotateOrigin, bool centreAndScale)
{
	transform = AffineTransform::rotation(angle, rotateOrigin.x, rotateOrigin.y);
	Rectangle<float> rotatedBounds = tileBounds.transformedBy(transform);
	
	if (centreAndScale)
	{
		transform = transform.followedBy(AffineTransform::translation(
			bounds.getCentreX() - rotatedBounds.getCentreX(),
			bounds.getCentreY() - rotatedBounds.getCentreY()
		));

		transformedBounds = tileBounds.transformedBy(transform);

		// Find amount to scale by so it fits in the bounds
		rotationScalar = juce::jmax(
			(float)bounds.getWidth() / transformedBounds.getWidth(),
			(float)bounds.getHeight() / transformedBounds.getHeight()
		);

		if (scaleToFit)
		{
			transform = transform.followedBy(
				AffineTransform::scale(rotationScalar, rotationScalar, transformedBounds.getCentreX(), transformedBounds.getCentreY())
			);
		}
	}
}

Array<Point<float>> HexagonTilingGeometry::transformPointsFromOrigin(Array<Point<int>> hexagonalCoordinatesIn)
{
	Array<Point<float>> hexagonCentres;
	AffineTransform transform = AffineTransform::translation(startingCentre).followedBy(AffineTransform::rotation(angle, startingCentre.x, startingCentre.y));

	const double rad = radius * verticalScalar;
	const double lat = radius * LATERALRADIUSRATIO * horizontalScalar;

	const double xUnit = lat * 2 + margin;
	const double yUnitX = distanceStepsAwayX(lat, margin, 1, 1) - distanceStepsAwayX(lat, margin, 1, 0);
	const double yUnitY = distanceStepsAwayY(rad, margin, 1);

	Array<Point<float>> pointsOut;

	for (auto point : hexagonalCoordinatesIn)
	{
		Point<float> pointTrans = Point<float>(
			point.x * xUnit + point.y * yUnitX,
			point.y * yUnitY
			);

		pointsOut.add(pointTrans.transformedBy(transform));
	}

	return pointsOut;
}

Array<Point<float>> HexagonTilingGeometry::calculateCentres(const TerpstraBoardGeometry& boardGeometry, int startingOctave, int numOctaves) const
{
	Array<Point<float>> hexagonCentres;

	const int numColumnsInOctave = boardGeometry.getMaxHorizontalLineSize();
	const int numRowsInOctave	 = boardGeometry.horizontalLineCount();

	const int totalOctaves = abs(numOctaves - startingOctave);
	const int maxRowLength = totalOctaves * numColumnsInOctave;
	const int maxColumnLength = numRowsInOctave + BOARDROWOFFSET * (totalOctaves - 1);

	const double rad = radius * verticalScalar;
	const double lat = radius * LATERALRADIUSRATIO * horizontalScalar;

	int octaveColumnOffset = startingOctave * numColumnsInOctave;
	int octaveRowOffset = startingOctave * BOARDROWOFFSET;

	const double yUnit = distanceStepsAwayY(rad, margin, 1);
	
	double yCoordinate = startingCentre.y;

	for (int octaveIndex = startingOctave; octaveIndex < startingOctave + numOctaves; octaveIndex++)
	{
		for (int row = 0; row < maxColumnLength; row++)
		{
			int rowCount = boardGeometry.horizontalLineSize(row);
			int firstColumn = boardGeometry.firstColumnOffset(row) + octaveColumnOffset;

			int octaveRow = row + octaveRowOffset;

			for (int col = 0; col < rowCount; col++)
			{
				Point<float> centre = Point<float>(
					// TODO: review implementation. this curently treats rows as zigzag 
					startingCentre.x + distanceStepsAwayX(lat, margin, firstColumn + col, (octaveRow % 2)),
					yCoordinate
				);
				centre.applyTransform(transform);
				hexagonCentres.add(centre);
			}

			yCoordinate += yUnit;
		}

		octaveColumnOffset += numColumnsInOctave;
		octaveRowOffset += BOARDROWOFFSET;
		yCoordinate = startingCentre.y + distanceStepsAwayY(rad, margin, octaveRowOffset);
	}

	return hexagonCentres;
}

int HexagonTilingGeometry::verticalToSlantOffset(int rowNum, int offsetIn)
{
	return offsetIn - (rowNum / 2);
}

Array<Point<float>> HexagonTilingGeometry::calculateCentresSkewed(const TerpstraBoardGeometry& boardGeometry, int startingOctave, int numOctaves) const
{
	Array<Point<float>> hexagonCentres;

	const int numColumnsInOctave = boardGeometry.getMaxHorizontalLineSize() ;
	const int numRowsInOctave = boardGeometry.horizontalLineCount();

	const int totalOctaves = abs(numOctaves - startingOctave);
	const int maxRowLength = totalOctaves * numColumnsInOctave;
	const int maxColumnLength = numRowsInOctave + BOARDROWOFFSET * (totalOctaves - 1);

	const double colX = columnXComponent * horizontalScalar;
	const double colY = columnYComponent * horizontalScalar;
	const double rowX = rowXComponent    * verticalScalar;
	const double rowY = rowYComponent    * verticalScalar;

	int octaveColumnOffset = startingOctave * numColumnsInOctave;
	int octaveRowOffset = startingOctave * BOARDROWOFFSET;

	for (int octaveIndex = startingOctave; octaveIndex < startingOctave + numOctaves; octaveIndex++)
	{
		for (int row = 0; row < maxColumnLength; row++)
		{
			int colStart = verticalToSlantOffset(row, boardGeometry.firstColumnOffset(row)) + octaveColumnOffset;
			int colEnd = colStart + boardGeometry.horizontalLineSize(row);

			int octaveRow = row + octaveRowOffset;

			for (int col = colStart; col < colEnd; col++)
			{
				Point<float> centre = startingCentre.toFloat() + Point<float>(
					col * colX + octaveRow * rowX,
					col * colY + octaveRow * rowY
				);

				centre.applyTransform(transform);
				hexagonCentres.add(centre);
			}
		}

		octaveColumnOffset += (numColumnsInOctave-1);
		octaveRowOffset += BOARDROWOFFSET;
	}

	return hexagonCentres;
}


Rectangle<float> HexagonTilingGeometry::calculateSmallestBounds(int widestRowSize, int longestColumnSize) const
{
	float rad = radius * verticalScalar;
	float lat = radius * LATERALRADIUSRATIO * horizontalScalar;

	return Rectangle<float>(
		bounds.getX(), bounds.getY(),
		calculateTileWidth(widestRowSize, lat, margin), calculateTileHeight(longestColumnSize, rad, margin)
	);
}