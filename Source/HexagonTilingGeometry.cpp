/*
  ==============================================================================

    HexagonTilingGeometry.cpp
    Created: 6 Jan 2021 11:42:30pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "HexagonTilingGeometry.h"

float HexagonTilingGeometry::getKeySize(bool scaled)
{
	float keySize = (scaled) ? circumRadius * currentScaleFactor : (float)circumRadius;
	return keySize * 2.0f;
}

Array<Point<float>> HexagonTilingGeometry::getHexagonCentres(const TerpstraBoardGeometry& boardGeometry,
	Rectangle<float> boundsIn, float marginSize, float rotateAngle, int startingOctave, int numOctaves, bool scaleToFitRotation)
{
	bounds = boundsIn;
	margin = marginSize;
	angle = rotateAngle;
	scaleToFit = scaleToFitRotation;

	circumRadius = calculateBestRadius();
	recalculateProperties();
	recalculateTransform(tileBounds.getCentre(), true);

	return calculateCentres(boardGeometry);
}

Array<Point<float>> HexagonTilingGeometry::getHexagonCentres(const TerpstraBoardGeometry& boardGeometry,
	Rectangle<float> boundsIn, Point<float> firstKeyCentre, Point<float> lastKeyCentre, float marginToRadiusRatio, int startingOctave, int numOctavesIn)
{
	bounds = boundsIn;
	tileBounds = boundsIn;
	
	// Get radius from distance between the firstKeyCentre and the point on the same line with the y-coordinate of bounds top left position
	float distanceToCenter = firstKeyCentre.getDistanceFrom(Point<float>(firstKeyCentre.getX(), boundsIn.getY()));
	margin = marginToRadiusRatio * distanceToCenter;
	circumRadius = (1.0 - marginToRadiusRatio) * distanceToCenter;
	recalculateProperties();

	DBG("bounds        : " + bounds.toString());
	DBG("firstKeyCentre: " + firstKeyCentre.toString());
	DBG("circumradius:   " + String(circumRadius));
	DBG("tileBounds:     " + tileBounds.toString());

	scaleToFit = false;

	startingOctaveIndex = startingOctave;
	numOctaves = numOctavesIn;

	// Steps from first key to last key // TODO starting/num octaves
	Point<int> stepsFirstTolast;
	stepsFirstTolast.x = (numOctaves - 1) * rowCount + boardGeometry.firstColumnOffset(numRows - 1) + boardGeometry.horizontalLineSize(numRows - 1) - 1;
	stepsFirstTolast.y = numRows + BOARDROWOFFSET * (numOctaves - 1) - 1;

	//DEBUG
	DBG("Steps first key to last key: " + stepsFirstTolast.toString());

	// Calculate rotation
	
	// Find expected distance to last key centre point
	Point<float> expectedDistanceToLastKeyCentre;
	expectedDistanceToLastKeyCentre.x = distanceStepsAwayX(incircleRadius, margin, stepsFirstTolast.x, stepsFirstTolast.y);
	expectedDistanceToLastKeyCentre.y = distanceStepsAwayY(circumRadius, margin, stepsFirstTolast.y);

	DBG("Expected Centre: " + (firstKeyCentre + expectedDistanceToLastKeyCentre).toString());
	DBG("Last key Centre: " + lastKeyCentre.toString());

	// Find angle from first key centre as origin to last key centre
	float expectedAngle = atanf(-expectedDistanceToLastKeyCentre.y / expectedDistanceToLastKeyCentre.x);
	DBG("Expected Angle: " + String(expectedAngle));

	// Find actual angle
	Point<float> actuallastKeyNorm = lastKeyCentre - firstKeyCentre;
	angle = atanf(-actuallastKeyNorm.y / actuallastKeyNorm.x);
	DBG("Actual Angle:   " + String(angle));

	// Get angle to rotate for correction
	angle = expectedAngle - angle;
	DBG("Correct Angle:  " + String(angle));

	recalculateTransform(firstKeyCentre, false);


	// Add correction scaling
	Point<float> generatedLastKeyCentre = (firstKeyCentre + expectedDistanceToLastKeyCentre).transformedBy(transform);

	float scaleX = actuallastKeyNorm.x / (generatedLastKeyCentre.x - firstKeyCentre.x);
	float scaleY = actuallastKeyNorm.y / (generatedLastKeyCentre.y - firstKeyCentre.y);

	transform = transform.followedBy(AffineTransform::scale(scaleX, scaleY, firstKeyCentre.x, firstKeyCentre.y));

	return calculateCentresFromFirstKeyCentre(boardGeometry, firstKeyCentre);
}

Rectangle<float> HexagonTilingGeometry::getTileBounds(bool withTransformation)
{
	if (withTransformation)
		return transformedBounds;

	return tileBounds;
}

double HexagonTilingGeometry::calculateTileWidth(int numColumns, double radiusInside, double margin)
{
	return numColumns * (margin + 2 * radiusInside) - margin + radiusInside;
}

double HexagonTilingGeometry::calculateTileHeight(int numRows, double radiusBounding, double margin)
{
	return (numRows - 1.0) * (1.5 * radiusBounding + margin * INCIRCLERADIUSRATIO) + 2 * radiusBounding;
}

double HexagonTilingGeometry::distanceStepsAwayX(double radiusInside, double margin, int stepsX, int stepsY)
{
	double halfMargin = margin / 2.0;
	return stepsX * (2 * radiusInside + halfMargin) + (stepsY % 2) * (radiusInside + halfMargin);
}

double HexagonTilingGeometry::distanceStepsAwayY(double radiusBounding, double margin, int stepsY)
{
	return stepsY * (radiusBounding * 1.5 + margin * INCIRCLERADIUSRATIO);
}

double HexagonTilingGeometry::calculateBestRadius()
{
	int totalColumns = rowCount * numOctaves;
	int totalRows = numRows + BOARDROWOFFSET * (numOctaves - 1);
	double widthBased = (bounds.getWidth() - margin * (totalColumns - 1)) / (2 * totalColumns + 1) / INCIRCLERADIUSRATIO;
	double heightBased = (2 * bounds.getHeight() - margin * INCIRCLEDIAMETERRATIO * (totalRows + 1)) / (3 * totalRows + 1);

	return juce::jmin(widthBased, heightBased);
}

void HexagonTilingGeometry::recalculateProperties()
{
	incircleRadius = circumRadius * INCIRCLERADIUSRATIO;
	incircleDiameter = incircleRadius * 2.0;

	verticalMarginUnit = margin * INCIRCLEDIAMETERRATIO / 2.0;

	tileBounds = calculateSmallestBounds();
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
		currentScaleFactor = juce::jmax(
			(float)bounds.getWidth() / transformedBounds.getWidth(),
			(float)bounds.getHeight() / transformedBounds.getHeight()
		);

		if (scaleToFit)
		{
			transform = transform.followedBy(
				AffineTransform::scale(currentScaleFactor, currentScaleFactor, transformedBounds.getCentreX(), transformedBounds.getCentreY())
			);
		}
	}
}

Array<Point<float>> HexagonTilingGeometry::calculateCentres(const TerpstraBoardGeometry& boardGeometry)
{
	auto getCentreX = [&](int indexInRow, double oddRowOffset) {
		return (incircleDiameter + margin) * indexInRow + oddRowOffset + tileBounds.getX();
	};

	Array<Point<float>> hexagonCentres;

	float octaveColumnOffset = 0;
	float octaveRowOffset = 0;

	for (int octaveIndex = startingOctaveIndex; octaveIndex < startingOctaveIndex + numOctaves; octaveIndex++)
	{
		octaveColumnOffset = rowCount * octaveIndex;
		octaveRowOffset = BOARDROWOFFSET * octaveIndex;

		for (int row = 0; row < numRows; row++)
		{
			int rowCount = boardGeometry.horizontalLineSize(row);
			int firstColumn = boardGeometry.firstColumnOffset(row) + octaveColumnOffset;

			int octaveRow = row + octaveRowOffset;

			float oddRow = (octaveRow % 2 == 0) ? 0 : incircleRadius + margin / 2.0;

			float yCoordinate = octaveRow * (1.5 * circumRadius + verticalMarginUnit) + circumRadius + tileBounds.getY();

			juce::Line<float> line(
				Point<float>(getCentreX(firstColumn, oddRow) + incircleRadius, yCoordinate),
				Point<float>(getCentreX(firstColumn + rowCount, oddRow), yCoordinate)
			);

			for (int i = 0; i < rowCount; i++)
			{
				Point<float> centre = line.getPointAlongLine(i * (2 * incircleRadius + margin));
				centre.applyTransform(transform);
				hexagonCentres.add(centre);
				//DBG("TILING OCTAVE " + String(octaveIndex) + ": " + hexagonCentres[hexagonCentres.size() - 1].toString());
			}
		}
	}

	return hexagonCentres;
}

Array<Point<float>> HexagonTilingGeometry::calculateCentresFromFirstKeyCorner(const TerpstraBoardGeometry& boardGeometry)
{
	return {};
}

Array<Point<float>> HexagonTilingGeometry::calculateCentresFromFirstKeyCentre(const TerpstraBoardGeometry& boardGeometry, Point<float> firstKeyCentre)
{
	Array<Point<float>> hexagonCentres;

	float octaveColumnOffset = 0;
	float octaveRowOffset = 0;

	for (int octaveIndex = startingOctaveIndex; octaveIndex < startingOctaveIndex + numOctaves; octaveIndex++)
	{
		octaveColumnOffset = rowCount * octaveIndex;
		octaveRowOffset = BOARDROWOFFSET * octaveIndex;

		for (int row = 0; row < numRows; row++)
		{
			int rowCount = boardGeometry.horizontalLineSize(row);
			int firstColumn = boardGeometry.firstColumnOffset(row) + octaveColumnOffset;

			int octaveRow = row + octaveRowOffset;

			float yCoordinate = distanceStepsAwayY(circumRadius, margin, octaveRow) + firstKeyCentre.getY();

			for (int i = 0; i < rowCount; i++)
			{
				Point<float> centre = Point<float>(distanceStepsAwayX(incircleRadius, margin, firstColumn + i, octaveRow) + firstKeyCentre.x, yCoordinate);
				centre.applyTransform(transform);
				hexagonCentres.add(centre);
			}
		}
	}

	return hexagonCentres;
}


Rectangle<float> HexagonTilingGeometry::calculateSmallestBounds()
{
	int totalColumns = rowCount * numOctaves;
	int totalRows = numRows + BOARDROWOFFSET * (numOctaves - 1);
	return Rectangle<float>(
		bounds.getX(), bounds.getY(),
		calculateTileWidth(totalColumns, incircleRadius, margin), calculateTileHeight(totalRows, circumRadius, margin)
	);
}