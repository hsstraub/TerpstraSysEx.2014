
/*
  ==============================================================================

	ScaleStructure.cpp
	Created: 25 Jan 2020 5:12:32pm
	Author:  Vincenzo

  ==============================================================================
*/

#include "ScaleStructure.h"

ScaleStructure::ScaleStructure()
{
	period = 0;
	generatorIndex = -1;
	sizeIndexSelected = -1;
}

ScaleStructure::ScaleStructure(
	int periodIn,
	int genIndexIn,
	int sizeIndexIn,
	int genOffsetIn,
	int periodFactorIndexIn,
	Array<int> degreeGroupsIn,
	Array<Point<int>> chromaAlterationsIn)
{
	setAll(periodIn, genIndexIn, sizeIndexIn, genOffsetIn, periodFactorIndexIn, degreeGroupsIn, chromaAlterationsIn);
}

void ScaleStructure::resetToPeriod(int periodIn)
{
	setAll(periodIn, -1, -1);
}

void ScaleStructure::setAll(
	int periodIn,
	int generatorIndexIn,
	int sizeIndexIn,
	int generatorOffsetIn,
	int periodFactorIndexIn,
	Array<int> degreeGroupSizeIndiciesIn,
	Array<Point<int>> chromaAlterationsIn)
{
	bool fPeriodChanged = false;

	if (period != periodIn)
	{
		period = periodIn;
		periodFactors = getFactors(period);
		fPeriodChanged = true;
	}

	if (fPeriodChanged || periodFactorIndexSelected != periodFactorIndexIn)
	{
		periodFactorIndexSelected = jlimit(0, periodFactors.size() - 1, periodFactorIndexIn);
		periodFactorSelected = periodFactors[periodFactorIndexSelected];
		fPeriod = period / periodFactorSelected;

		validGenerators = getCoprimes(fPeriod);
	}

	// if generator index is invalid, default to suggested
	if (fPeriodChanged || generatorIndex != generatorIndexIn)
	{
		if (generatorIndexIn < 0 || generatorIndexIn >= validGenerators.size())
			generatorIndex = getSuggestedGeneratorIndex();
		else
			generatorIndex = generatorIndexIn;

		calculateProperties();
	}

	// if size index is invalid, default to suggested
	if (sizeIndexIn < 0 || sizeIndexIn >= scaleSizes.size())
		sizeIndexSelected = getSuggestedSizeIndex();
	else
		sizeIndexSelected = sizeIndexIn;

	generatorOffset = jlimit(0, getScaleSize() - 1, generatorOffsetIn);

	calculateGeneratorChain();

	if (chromaAlterationsIn.size() == period)
	{
		// TODO: check if valid
		chromaAlterations = chromaAlterationsIn;
	}

	if (degreeGroupSizeIndiciesIn.size() > 0)
	{
		// TODO: check if valid scale size
		degreeGroupScaleSizes = degreeGroupSizeIndiciesIn;

		// TODO: symmetric checks/fixes
		arrangeSymmetrically(degreeGroupScaleSizes);
		fillSymmetricGrouping();
	}
	else
	{
		useSuggestedSizeGrouping();
	}
}

bool ScaleStructure::isValid() const
{
	// Period is not selected yet
	jassert(period >= 1);

	// Invalid period factor index
	jassert(periodFactorIndexSelected >= 0 && periodFactorIndexSelected < periodFactors.size());

	// Invalid generator index
	jassert(generatorIndex >= 0 && generatorIndex < validGenerators.size());

	// Invalid scale size index
	jassert(sizeIndexSelected >= 0 && sizeIndexSelected < scaleSizes.size());

	// Invalid generator offset
	jassert(generatorOffset >= 0 && generatorOffset < scaleSizes.size());

	int sum = 0;
	for (int s = 0; s < degreeGroupIndexedSizes.size(); s++)
	{
		sum += scaleSizes[degreeGroupIndexedSizes[s]] * periodFactorSelected;
	}

	// Scale degree groupings do not have correct amount of degrees
	jassert(sum == period);

	return true;
}

int ScaleStructure::getPeriod(bool ofFactorSelected) const
{
	if (ofFactorSelected && periodFactorSelected > 0)
		return fPeriod;

	return period;
}

Array<int> ScaleStructure::getPeriodFactors() const
{
	return periodFactors;
}

int ScaleStructure::getPeriodFactorIndex() const
{
	return periodFactorIndexSelected;
}

int ScaleStructure::getPeriodFactor() const
{
	return periodFactorSelected;
}

Array<int> ScaleStructure::getValidGenerators() const
{
	return validGenerators;
}

const Array<int>& ScaleStructure::getValidGeneratorsReference()
{
	return validGenerators;
}

int ScaleStructure::getValidGenerator(int genIndex) const
{
	return validGenerators[genIndex];
}

int ScaleStructure::getGenerator() const
{
	return validGenerators[generatorIndex];
}

int ScaleStructure::getGeneratorIndex() const
{
	return generatorIndex;
}

Array<int> ScaleStructure::getScaleSizes() const
{
	return scaleSizes;
}

int ScaleStructure::getScaleSize(int ind) const
{
	return scaleSizes[ind];
}

int ScaleStructure::getScaleSize() const
{
	return scaleSizes[sizeIndexSelected];
}

int ScaleStructure::getScaleSizeIndex() const
{
	return sizeIndexSelected;
}

int ScaleStructure::getGeneratorOffset() const
{
	return generatorOffset;
}

Array<Point<int>> ScaleStructure::getKeyboardTypes() const
{
	return keyboardTypes;
}

Point<int> ScaleStructure::getKeyboardType(int ind) const
{
	return keyboardTypes[ind];
}

Array<PointPair<int>> ScaleStructure::getPGCoords() const
{
	return pgCoords;
}

PointPair<int> ScaleStructure::getPGCoord(int ind) const
{
	return pgCoords[ind];
}

Point<int> ScaleStructure::getStepSizes(int kbdTypeIn) const
{
	return stepSizes[kbdTypeIn];
}

Point<int> ScaleStructure::getStepSize() const
{
	return stepSizes[sizeIndexSelected];
}

Array<int> ScaleStructure::getGeneratorChain() const
{
	return generatorChain;
}

Array<int> ScaleStructure::getGroupChain() const
{
	return groupChain;
}

int ScaleStructure::getGroupChainIndexOfDegree(int degreeIn) const
{
	return groupChainDegreeIndicies[degreeIn];
}

Array<int> ScaleStructure::getGroupingIndexedSizes() const
{
	return degreeGroupIndexedSizes;
}

Array<int> ScaleStructure::getDegreeGroupSizes() const
{
	return degreeGroupScaleSizes;
}

Array<Array<int>> ScaleStructure::getDegreeGroupings() const
{
	return degreeGroupings;
}

int ScaleStructure::getGroupOfDegree(int scaleDegreeIn) const
{
	return degreeGroupChainMap[groupChainDegreeIndicies[scaleDegreeIn]];
}

int ScaleStructure::getGroupOfDegreeIndex(int groupChainIndex) const
{
	return degreeGroupChainMap[groupChainIndex];
}

bool ScaleStructure::isRetainingSymmetry() const
{
	return retainGroupingSymmetry;
}

bool ScaleStructure::isRetainingMOSSizes() const
{
	return retainMOSGroupSizes;
}

Array<Point<int>> ScaleStructure::findDegreeMods(int degreeIndex, int chromaLevels) const
{
	// Uses scale size as first step, and then differences of succeeding scale sizes

	Array<int> stepsToChromas = { scaleSizes[sizeIndexSelected] * periodFactorSelected};
	for (int i = sizeIndexSelected + 1; i < scaleSizes.size(); i++)
	{
		stepsToChromas.add((scaleSizes[i] - scaleSizes[i-1]) * periodFactorSelected);
	}
	DBG("SS: Degree mod steps: " + arrayToString(stepsToChromas));

	Array<Point<int>> degreeModCandidates;
	degreeModCandidates.resize(period);
	degreeModCandidates.fill(Point<int>(-1, 0));

	int levels;
	if (chromaLevels < 1) // Invalid values use all chromas
		levels = stepsToChromas.size();
	else
		levels = jmin(chromaLevels, stepsToChromas.size());

	String dbgstr = "";
	
	// Change so that non-group 0 degree can be fully altered
	int indexLimit = getScaleSize() * periodFactorSelected;

	int step = stepsToChromas[0];
	int index = modulo(degreeIndex + step, period);
	int degree, chromas = 1;

	// forward
	while (index >= indexLimit)
	{
		for (int c = 0; c < levels; c++)
		{
			if (index < indexLimit)
				break;

			degree = groupChain[index];

			// Only add if degree isn't already altered
			if (degreeModCandidates[degree].x == -1 && degreeAlterations[degree].x == -1)
			{
				degreeModCandidates.set(degree, Point<int>(c, chromas));
				dbgstr += String(degree) + ", ";
			}

			step = stepsToChromas[c + 1];
			index = modulo(index + step, period);
		}

		chromas++;
		step = stepsToChromas[0] * chromas;
		index = modulo(degreeIndex + step, period);
	}

	step = stepsToChromas[0];
	index = modulo(degreeIndex - step, period);
	chromas = -1;

	// backward
	while (index >= indexLimit)
	{
		for (int c = 0; c < levels; c++)
		{
			if (index < indexLimit)
				break;

			degree = groupChain[index];

			// Only add if degree isn't already altered
			if (degreeModCandidates[degree].x == -1 && degreeAlterations[degree].x == -1)
			{
				degreeModCandidates.set(degree, Point<int>(c, chromas));
				dbgstr += String(degree) + ", ";
			}

			step = stepsToChromas[c + 1];
			index = modulo(index - step, period);
		}

		chromas--;
		step = stepsToChromas[0] * -chromas;
		index = modulo(degreeIndex - step, period);
	}
	

	DBG("Degree candidates for modding " + String(groupChain[degreeIndex]) + ":\n\t" + dbgstr);

	return degreeModCandidates;
}

Array<int> ScaleStructure::getNominalScaleDegrees() const
{
	return nominalScaleDegrees;
}

Array<Point<int>> ScaleStructure::getChromaAlterations() const
{
	return chromaAlterations;
}

Array<Point<int>> ScaleStructure::getDegreeAlterations() const
{
	return degreeAlterations;
}

Array<Point<int>>  ScaleStructure::getAlterationOfDegree(int degreeIn) const
{
	return chromaAlterations[degreeIn];
}

int ScaleStructure::findIndexedAlterationOfDegree(int degreeIndexIn, Point<int> alteration) const
{
	if (alteration.x >= 0 && alteration.y != 0)
	{
		int step = scaleSizes[sizeIndexSelected + alteration.x] * periodFactorSelected;
		return modulo(degreeIndexIn + step * alteration.y, period);
	}

	return degreeIndexIn;
}

int ScaleStructure::findAlterationOfDegree(int degreeIn, Point<int> alteration) const
{
	int groupIndex = groupChain.indexOf(degreeIn);
	return groupChain[findIndexedAlterationOfDegree(groupIndex, alteration)];
}

void ScaleStructure::attachAlterationsToDegree(bool isAttachedToDegree)
{
	if (alterationsAttachedToDegree != isAttachedToDegree)
	{
		Array<Point<int>> originalAlterations = chromaAlterations;

		// remove alterations
		chromaAlterations.fill(Point<int>(-1, 0));
		fillSymmetricGrouping(false);

		Point<int> alteration;

		// convert from degrees to indicies
		if (!isAttachedToDegree)
		{
			for (int i = 0; i < groupChain.size(); i++)
			{
				alteration = originalAlterations[groupChain[i]];
				if (alteration.x > -1 && alteration.y != 0)
					chromaAlterations.set(i, alteration);
			}
		}

		// convert from indicies to degrees
		else
		{
			for (int i = 0; i < originalAlterations.size(); i++)
			{
				alteration = originalAlterations[i];
				if (alteration.x > -1 && alteration.y != 0)
					chromaAlterations.set(groupChain[i], alteration);
			}
		}

		// reapply after conversion
		alterationsAttachedToDegree = isAttachedToDegree;
		applyChromaAlterations();
	}
}


void ScaleStructure::setAlterationOfDegree(int degreeIndexIn, Point<int> alteration)
{
	if (degreeIndexIn < period)
	{
		// TODO: Restrict alterations if too big (degree gets swapped with another group0 degree)
		if (alterationsAttachedToDegree)
		{
			chromaAlterations.set(groupChain[degreeIndexIn], alteration);
		}
		else
		{
			chromaAlterations.set(degreeIndexIn, alteration);
		}

		fillSymmetricGrouping();
	}
}

void ScaleStructure::resetAlterationOfDegree(int degreeIndexIn)
{
	if (degreeIndexIn < period)
	{
		int index;
		Point<int> alteration;

		if (alterationsAttachedToDegree)
		{
			index = groupChain[degreeIndexIn];
			alteration = chromaAlterations[index];

			// check if the input is actually the inverse
			if (alteration.x == -1)
			{
				index = findAlterationOfDegree(index, alteration.withY(-alteration.y));
			}
		}
		else
		{
			index = degreeIndexIn;
			alteration = chromaAlterations[index];

			if (alteration.x == -1)
			{
				alteration = degreeAlterations[groupChain[index]];
				index = findIndexedAlterationOfDegree(index, alteration.withY(-alteration.y));
			}
		}


		chromaAlterations.set(index, Point<int>(-1, 0));

		fillSymmetricGrouping();
	}
}

void ScaleStructure::setPeriodFactorIndex(int index)
{
	periodFactorIndexSelected = index;
	periodFactorSelected = periodFactors[periodFactorIndexSelected];
	fPeriod = period / periodFactorSelected;
	validGenerators = getCoprimes(fPeriod);

	if (generatorIndex > -1)
	{
		calculateProperties();
		calculateGeneratorChain();
	}
}

void ScaleStructure::setSizeIndex(int index)
{
	sizeIndexSelected = index;

	// Restrict GeneratorOffset to valid value
	if (generatorOffset >= getScaleSize())
		setGeneratorOffset(getScaleSize() - 1);
	else
		useSuggestedSizeGrouping();
}

void ScaleStructure::setGeneratorOffset(int offsetIn)
{
	generatorOffset = offsetIn;
	fillSymmetricGrouping();
}

void ScaleStructure::setRetainGroupingSymmetry(bool isSymmetric)
{
	retainGroupingSymmetry = isSymmetric;
}

void ScaleStructure::setRetainMOSSizes(bool retainSizes)
{
	retainMOSGroupSizes = retainSizes;
}

bool ScaleStructure::setChromaAlterations(Array<Point<int>> chromaAlterationsIn)
{
	// TODO: better validity check
	if (chromaAlterationsIn.size() == 0)
	{
		chromaAlterationsIn.resize(period);
		chromaAlterationsIn.fill(Point<int>(-1, 0));
	}

	if (chromaAlterationsIn.size() == period)
	{
		chromaAlterations = chromaAlterationsIn;
		fillSymmetricGrouping(); // TODO: handle non-symmetric cases
		return true;
	}

	return false;
}

void ScaleStructure::calculateProperties()
{
	//DBG("~~~ ScaleStructure is calculating properties ~~~")
	// Clear all data dependent on Generator and Size choices
	scaleSizes.clear();
	keyboardTypes.clear();
	pgCoords.clear();

	chromaAlterations.resize(period);
	chromaAlterations.fill(Point<int>(-1, 0));

	// Calculate properties of scale
	Array<int> cf = getContinuedFraction((double)getGenerator() / fPeriod);

	// seed the sequence
	Point<int> parent1 = Point<int>(-1 + cf[0], 1);
	Point<int> parent2 = Point<int>(1, 0);
	Point<int> gp = Point<int>(cf[0], 1);

	Array<Point<int>> packet = { gp, parent2, gp + parent2 }; // makes for cleaner code
	pgCoords.add(PointPair<int>(packet[0], packet[1]));
	keyboardTypes.add(packet[2]);
	scaleSizes.add(packet[2].y);

	// find keyboard types, and their period/generator coordinates
	for (int i = 1; i < cf.size(); i++)
	{
		for (int d = 0; d < cf[i]; d++)
		{
			pgCoords.add(PointPair<int>(packet[0], packet[1]));
			keyboardTypes.add(packet[2]);
			scaleSizes.add(packet[2].y);

			parent1 = packet[0];
			parent2 = packet[1];
			gp = packet[2];

			// if previous continued fraction index is even,
			// set parent2 to previous result
			if (i % 2 == 0)
				parent1 = gp;
			else // if odd, set parent1 to previous result
				parent2 = gp;

			packet = { parent1, parent2, parent1 + parent2 };
		}
	}

	calculateStepSizes();
}

void ScaleStructure::calculateStepSizes()
{
	stepSizes.clear();

	Point<int> stepSizesOut;
	Point<int> periodCoordinate;
	Point<int> generatorCoordinate;

	int gen = getGenerator();

	for (int i = 0; i < scaleSizes.size(); i++)
	{
		generatorCoordinate = Point<int>(pgCoords[i].x.x, pgCoords[i].y.x);
		periodCoordinate = Point<int>(pgCoords[i].x.y, pgCoords[i].y.y);

		// find horiztonal step size (X)
		if (periodCoordinate.y == generatorCoordinate.y)
			stepSizesOut.setX(fPeriod - gen);
		else if (periodCoordinate.y == 0)
			stepSizesOut.setX(fPeriod);
		else if (generatorCoordinate.y == 0)
			stepSizesOut.setX(gen);
		else
			stepSizesOut.setX(fPeriod * generatorCoordinate.y - gen * periodCoordinate.y);

		// find upward right step size (Y)
		if (periodCoordinate.x == generatorCoordinate.x)
			stepSizesOut.setY(fPeriod - gen);
		else if (periodCoordinate.x == 0)
			stepSizesOut.setX(fPeriod);
		else if (generatorCoordinate.y == 0)
			stepSizesOut.setX(gen);
		else
			stepSizesOut.setY(gen * periodCoordinate.x - fPeriod * generatorCoordinate.x);

		stepSizes.add(stepSizesOut);
	}
}

void ScaleStructure::calculateIntervalScales()
{
	scalesInIntervals.clear();

	for (auto size : scaleSizes)
	{
		Array<int> sizes, scale;

		for (int i = 0; i < size; i++)
		{
			for (int p = 0; p < periodFactorSelected; p++)
				sizes.add(generatorChain[i + fPeriod * p]);
		}

		sizes.add(period);
		sizes.sort();

		String steps = "";
		for (int i = 1; i <= getScaleSize() * periodFactorSelected; i++)
		{
			scale.add(i - 1, sizes[i] - sizes[i - 1]);
		}

		scalesInIntervals.add(scale);
	}
}

void ScaleStructure::calculateGeneratorChain()
{
	generatorChain.clear();
	int gen = getGenerator();

	String dbgstr = "";

	for (int i = 0; i < fPeriod; i++)
	{
		generatorChain.add(modulo(i * gen, fPeriod));
		dbgstr += String(generatorChain[i]) + ", ";
	}

	// fill rest of degrees if fractional period is in place
	for (int i = 1; i < periodFactorSelected; i++)
	{
		for (int deg = 0; deg < fPeriod; deg++)
		{
			generatorChain.add(generatorChain[deg] + fPeriod * i);
			dbgstr += String(generatorChain[deg + fPeriod * i]) + ", ";
		}
	}

//	DBG("SS Generator Chain: " + dbgstr);

	calculateIntervalScales();
}

void ScaleStructure::fillSymmetricGrouping(bool applyAlterations)
{
	degreeGroupings.clear();
	degreeGroupings.resize(degreeGroupIndexedSizes.size());
	degreeGroupScaleSizes.clear();
		
	groupChainDegreeIndicies.resize(period);
	degreeGroupChainMap.resize(period);
	groupChain.clear();

	// Fill degree groups symmetrically
	int groupSize, degree, ind = 0;
	for (int group = 0; group < degreeGroupIndexedSizes.size(); group++)
	{
		groupSize = scaleSizes[degreeGroupIndexedSizes[group]];
		degreeGroupScaleSizes.add(groupSize * periodFactorSelected);

		for (int f = 0; f < periodFactorSelected; f++)
		{
			for (int groupInd = 0; groupInd < groupSize; groupInd++)
			{
				degree = generatorChain[modulo(ind + groupInd + fPeriod * f - generatorOffset, period)];
				degreeGroupings.getReference(group).add(degree);
				
				degreeGroupChainMap.set(groupChain.size(), group);
				groupChainDegreeIndicies.set(degree, groupChain.size());
				groupChain.add(degree);
			}
		}

		ind += groupSize;
	}

	if (applyAlterations)
		applyChromaAlterations();

	nominalScaleDegrees.clear();
	nominalScaleDegrees = degreeGroupings[0];
	nominalScaleDegrees.sort();

	// DEBUG PRINTING

//	String dbgstr = "";
//	int size, sum = 0;
//	for (int i = 0; i < degreeGroupIndexedSizes.size(); i++) {
//		size = degreeGroupScaleSizes[i];
//		dbgstr += String(size) + ", ";
//		sum += size;
//	}
//	dbgstr += " = " + String(sum);
//	DBG("SS Updated grouping: " + dbgstr);

//	dbgstr = "\t";
//	for (int group = 0; group < degreeGroupIndexedSizes.size(); group++)
//	{
//		Array<int> degreeGroup = degreeGroupings[group];
//		dbgstr += "Tier " + String(group) + ": ";
//		for (int deg = 0; deg < degreeGroup.size(); deg++)
//		{
//			dbgstr += String(degreeGroup[deg]) + ", ";
//		}
//
//		if (group + 1 < degreeGroupIndexedSizes.size())
//			dbgstr += "\n\t";
//	}
//
//	DBG("SS Degree groupings: ");
//	DBG(dbgstr);
}

void ScaleStructure::applyChromaAlterations()
{
	int numAlterations = 0;

	// Copy alterations
	if (alterationsAttachedToDegree)
	{
		degreeAlterations = chromaAlterations;
	}
	else
	{
		degreeAlterations.resize(chromaAlterations.size());
		degreeAlterations.fill(Point<int>(-1, 0));
		for (int i = 0; i < chromaAlterations.size(); i++)
		{
			const Point<int>& alteration = chromaAlterations.getReference(i);
			if (alteration.x != -1)
				degreeAlterations.set(groupChain[i], alteration);
		}
	}


	for (int degIndex = 0; degIndex < period; degIndex++)
	{
		int degree = groupChain[degIndex];

		Point<int> alteration;
		if (alterationsAttachedToDegree)
			alteration = chromaAlterations[degree];
		else
			alteration = chromaAlterations[degIndex];

		// Find the altered scale degree
		if (alteration.x >= 0 && alteration.y != 0)
		{
			int shiftedIndex = findIndexedAlterationOfDegree(degIndex, alteration);
			int alteredDegree = groupChain[shiftedIndex];

			int orignalGroupIndex, originalGroupNum = -1;
			int shiftedGroupIndex, shiftedGroupNum = -1;

			// Find the groups indicies that each degree is in
			for (int g = 0; g < degreeGroupings.size(); g++)
			{
				Array<int>& group = degreeGroupings.getReference(g);
				
				for (int i = 0; i < group.size(); i++)
				{
					if (group[i] == degree)
					{
						originalGroupNum = g;
						orignalGroupIndex = i;
					}
					else if (group[i] == alteredDegree)
					{
						shiftedGroupNum = g;
						shiftedGroupIndex = i;
					}
				}
				
				// Swap the degrees
				if (originalGroupNum > -1 && shiftedGroupNum > -1)
				{
					degreeGroupings.getReference(originalGroupNum).set(orignalGroupIndex, alteredDegree);
					degreeGroupings.getReference(shiftedGroupNum).set(shiftedGroupIndex, degree);

					// record inverse alteration
					degreeAlterations.set(alteredDegree, alteration.withY(-alteration.y));

					// update helper maps
					degreeGroupChainMap.set(shiftedIndex, originalGroupNum);
					degreeGroupChainMap.set(degIndex, shiftedGroupNum);
					
					break;
				}
			}

			numAlterations++;
		}
	}

	if (numAlterations > 0)
	{
		// Update group chain with swapped degrees
		groupChain.clear();
		for (auto group : degreeGroupings)
			for (auto deg : group)
				groupChain.add(deg);
	}

//	String dbgstr = "";
//
//	for (auto alteration : chromaAlterations)
//	{
//		dbgstr += "(" + alteration.toString() + "), ";
//	}
//	dbgstr = alterationsAttachedToDegree
//		? "SS MODMOS Properties by degree:\t" + dbgstr
//		: "SS MODMOS Properties by gIndex:\t" + dbgstr;
//	DBG(dbgstr);
}

int ScaleStructure::getSymmetricGroup(int groupIndexIn) const
{
	// Check for symmetry first?
	if (retainGroupingSymmetry)
	{
		if (groupIndexIn == 0)
			return 0;

		return degreeGroupIndexedSizes.size() - groupIndexIn;
	}

	return -1;
}


/*
	Input a degree group index and get chain indicies that the edge of a group can move to.
	If retainMOSGroupSizes, then the degree indicies are only what can create supported MOS sizes out of
	the group and adjacent group sizes.
	If retainGroupSymmetry, the y value corresponds to the degree indicies of the symmetric group.

	The first value will always refer to the adjacent CCW edge that the group edge can move to, and
	the second value will always refer to the adjacent CW edge.
*/
Array<Point<int>> ScaleStructure::findIndiciesForGroupResizing(int groupIndexIn) const
{
	int numGroups = degreeGroupIndexedSizes.size();
	Array<Point<int>> degreeIndiciesOut;

	if (groupIndexIn > 0 && groupIndexIn < numGroups)
	{
		int groupSizeIndex = degreeGroupIndexedSizes[groupIndexIn];
		int groupSize = degreeGroupScaleSizes[groupIndexIn];
		int adjGroupIndex = groupIndexIn - 1;

		// For now, don't allow resizing of group 0 
		if (adjGroupIndex <= 0)
			return degreeIndiciesOut;

		int adjGroupSize = degreeGroupScaleSizes[adjGroupIndex];  
		
		int symGroup = getSymmetricGroup(groupIndexIn);
		int symAdjGroup = symGroup + 1;

		int degIndex = groupChain.indexOf(degreeGroupings[groupIndexIn][0]);
		int symDegIndex = groupChain.indexOf(degreeGroupings[symAdjGroup][0]); // manipulated in opposite direction

		int newSize, adjSize, dif;

		// If handle is in exact center then degrees are symmetrical
		if (retainGroupingSymmetry && groupIndexIn == symAdjGroup)
		{
			for (int i = 1; i <= groupSize; i++)
			{
				dif = i;

				// skip if sizes are not supported by MOS
				if (retainMOSGroupSizes)
				{
					dif *= periodFactorSelected;
					newSize = groupSize - dif;

					if ((newSize != 0 && scaleSizes.indexOf(newSize / periodFactorSelected) < 0) || scaleSizes.indexOf(dif * 2 / periodFactorSelected) < 0)
						continue;
				}

				degreeIndiciesOut.add({ degIndex + dif, symDegIndex - dif });
			}
		}

		else
		{
			// Find CW degree indicies
			for (int i = 1; i <= groupSize; i++)
			{
				dif = i;

				if (retainMOSGroupSizes)
					dif *= periodFactorSelected;

				newSize = groupSize - dif;
				adjSize = adjGroupSize + dif;

				if (retainGroupingSymmetry)
				{
					if (groupIndexIn == symAdjGroup)
					{
						newSize -= dif;
						adjSize += dif;

						if (retainMOSGroupSizes && scaleSizes.indexOf(dif * 2 / periodFactorSelected) < 0)
							continue;
					}

					if (groupIndexIn == symGroup)
						newSize -= dif;

					if (adjGroupIndex == symAdjGroup)
						adjSize += dif;

					if (newSize < 0)
						continue;
				}

				if (retainMOSGroupSizes)
				{
					if (
						(newSize >= 0 && scaleSizes.indexOf(adjSize / periodFactorSelected) < 0) ||
						(newSize != 0 && scaleSizes.indexOf(newSize / periodFactorSelected) < 0)
					   )
						continue;
				}
				
				if (retainGroupingSymmetry)
					degreeIndiciesOut.add({ degIndex + dif, symDegIndex - dif });
				else
					degreeIndiciesOut.add({ degIndex + dif, -1 });
			}

			// Find CCW degree indicies
			bool addedFirstEdge = false;
			for (int i = 1; i <= adjGroupSize; i++)
			{
				dif = i;
				// skip if sizes are not supported by MOS
				if (retainMOSGroupSizes)
					dif *= periodFactorSelected;
				
				newSize = groupSize + dif;
				adjSize = adjGroupSize - dif;

				if (retainGroupingSymmetry)
				{
					if (groupIndexIn == symGroup)
						newSize += dif;

					if (adjGroupIndex == symAdjGroup)
						adjSize -= dif;

					if (adjSize < 0)
						continue;
				}

				if (retainMOSGroupSizes)
				{
					if (
						(adjSize >= 0 && scaleSizes.indexOf(newSize / periodFactorSelected) < 0) || 
						(adjSize != 0 && scaleSizes.indexOf(adjSize / periodFactorSelected) < 0)
					   )
						continue;
				}

				Point<int> indiciesOut;

				if (retainGroupingSymmetry)
					indiciesOut = { degIndex - dif, symDegIndex + dif };
				else
					indiciesOut = { degIndex - dif, -1 };

				if (addedFirstEdge)
					degreeIndiciesOut.add(indiciesOut);
				else
				{
					degreeIndiciesOut.insert(0, indiciesOut);
					addedFirstEdge = true;
				}
			}

			if (!addedFirstEdge)
				degreeIndiciesOut.insert(0, { -1, -1 });
		}
	}

	return degreeIndiciesOut;
}

Array<Point<int>> ScaleStructure::findIndiciesForGroupSplitting(int groupIndexIn, bool newGroupClockwise) const
{
	int numGroups = degreeGroupIndexedSizes.size();
	Array<Point<int>> degreeIndiciesOut;

	if (groupIndexIn > 0 && groupIndexIn < numGroups)
	{
		int groupSizeIndex = degreeGroupIndexedSizes[groupIndexIn];
		int groupSize = scaleSizes[groupSizeIndex] * periodFactorSelected;
		int symGroup = getSymmetricGroup(groupIndexIn);
		
		// Degree indicies to reference whether group is being split via CCW or CW direction
		int ccwDegIndex = groupChain.indexOf(degreeGroupings[groupIndexIn][0]);
		int cwDegIndex = ccwDegIndex + groupSize;
		int ccwSymIndex = groupChain.indexOf(degreeGroupings[symGroup][0]);
		int cwSymIndex = ccwSymIndex + groupSize;

		// Build alias functions for simplifying getting degree indicies and symmetrical complements
		auto displacedFromCCWEdge = [](int degIndexIn, int numDegrees)
		{
			return degIndexIn + numDegrees;
		};

		auto displacedFromCWEdge = [](int degIndexIn, int numDegrees)
		{
			return degIndexIn - numDegrees;
		};

		std::function<int(int)> getDegIndex, getSymIndex;
		if (newGroupClockwise)
		{
			getDegIndex = std::bind(displacedFromCWEdge, cwDegIndex, std::placeholders::_1);
			getSymIndex = std::bind(displacedFromCCWEdge, ccwSymIndex, std::placeholders::_1);
		}
		else
		{
			getDegIndex = std::bind(displacedFromCCWEdge, ccwDegIndex, std::placeholders::_1);
			getSymIndex = std::bind(displacedFromCWEdge, cwSymIndex, std::placeholders::_1);
		}

		// For-loop limit whether MOS sizes are retained or not
		int indexLimit = retainMOSGroupSizes ? groupSizeIndex : groupSize;
		int degLimit = groupSize;
		if (retainGroupingSymmetry && groupIndexIn == symGroup)
			degLimit /= 2;
		
		for (int i = 1, dif = 0; i < indexLimit && dif < degLimit ; i++)
		{
			if (retainMOSGroupSizes)
				dif = scaleSizes[i] * periodFactorSelected;
			else
				dif = i;

			// skip if sizes are not supported by MOS
			if (retainMOSGroupSizes)
			{
				int remSize = groupSize - dif;

				if (retainGroupingSymmetry && groupIndexIn == symGroup)
					remSize -= dif;

				// accept 0, where group will split into equal parts
				if (remSize != 0 && scaleSizes.indexOf(remSize / periodFactorSelected) < 0)
					continue;
			}

			if (retainGroupingSymmetry)
				degreeIndiciesOut.add({ getDegIndex(dif), getSymIndex(dif) });
			else
				degreeIndiciesOut.add({ getDegIndex(dif), -1 });
		}
	}

	return degreeIndiciesOut;
}

/*
	Sets and updates degree grouping arrangment.
	If the passed in grouping contains invalid scale size indicies and if they do not
	add to the period, then it will not be applied.
	If retainSymmetry is true, this will check if the grouping is symmetric, and won't apply it if not.
*/
void ScaleStructure::setDegreeGrouping(Array<int> groupingSizeIndiciesIn)
{
	DBG("Scale Structure: Grouping passed in: " + arrayToString(groupingSizeIndiciesIn));

	bool sym = isSymmetric(groupingSizeIndiciesIn);
	if (retainGroupingSymmetry && !sym)
	{
		// TODO: status flags?
		DBG("Scale Structure: Grouping is not symmetric");
		return;
	}

	// Check if group sizes add up to period
	int sum = 0;
	for (auto i : groupingSizeIndiciesIn)
	{
		sum += scaleSizes[i];
	}

	if (sum == fPeriod)
	{
		degreeGroupIndexedSizes = groupingSizeIndiciesIn;

		if (sym)
			fillSymmetricGrouping();
		else
			return; // TODO: bring back other degree grouping function

		return;
	}

	DBG("Scale Structure: Groupings did not add up to period");
}

/*
	Splits a group index into two groups. The two sizes added must equal original size.
	If newGroupClockwise is true, the new group using the remainder size will be added
	clockwise to the original group.
	If retaining symmetry, this also effects the group on the other side.
*/
void ScaleStructure::splitDegreeGroup(int groupIndexIn, int sizeChangeAmount)
{
	int numGroups = degreeGroupScaleSizes.size();

	// Check if group index is valid
	if (groupIndexIn < 1 || groupIndexIn >= numGroups)
	{
		DBG("ScaleStructure: Split Error - group 0 index present");
		return;
	}

	int symGroupIndex = getSymmetricGroup(groupIndexIn);

	bool newGroupClockwise = sizeChangeAmount > 0;
	sizeChangeAmount = abs(sizeChangeAmount);

	int groupSize = degreeGroupScaleSizes[groupIndexIn] - sizeChangeAmount;
	// Subtract size again if group is symmetric with self
	if (retainGroupingSymmetry && symGroupIndex == groupIndexIn)
	{
		groupSize -= sizeChangeAmount;
	}

	int groupSizeIndex = scaleSizes.indexOf(groupSize / periodFactorSelected);
	int newSizeIndex = scaleSizes.indexOf(sizeChangeAmount / periodFactorSelected);

	// Check if group's new size is valid
	if (retainMOSGroupSizes && groupSize != 0 && (groupSizeIndex < 0 || newSizeIndex < 0))
	{
		DBG("Scale Structure: Split Error - size not supported by MOS");
		return;
	}

	Array<int> newGrouping;
	for (int i = 0; i < degreeGroupIndexedSizes.size(); i++)
	{
		if (i != groupIndexIn && i != symGroupIndex)
		{
			newGrouping.add(degreeGroupIndexedSizes[i]);
		}

		else if (i == groupIndexIn && groupIndexIn == symGroupIndex)
		{
			newGrouping.add(newSizeIndex);

			if (groupSize > 0)
				newGrouping.add(groupSizeIndex);

			newGrouping.add(newSizeIndex);
		}

		else if (i == groupIndexIn)
		{

			if (!newGroupClockwise)
			{
				newGrouping.add(groupSizeIndex);
				newGrouping.add(newSizeIndex);
			}
			else
			{
				newGrouping.add(newSizeIndex);
				newGrouping.add(groupSizeIndex);
			}
		}

		else if (i == symGroupIndex)
		{
			// Inverse for symmetric group
			if (newGroupClockwise)
			{
				newGrouping.add(groupSizeIndex);
				newGrouping.add(newSizeIndex);
			}
			else
			{
				newGrouping.add(newSizeIndex);
				newGrouping.add(groupSizeIndex);
			}
		}
	}

	setDegreeGrouping(newGrouping);
}

/*
	Resizes two adjacent groups, where the passed in group's size will have the passed in sizeChangeAmount added.
	If retaining symmetry, this also effects the groups on the other side.
*/
void ScaleStructure::resizeDegreeGroup(int groupIndex, int sizeChangeAmount, bool draggedClockwise)
{
	int adjGroupIndex = groupIndex - 1;
	int numGroups = degreeGroupScaleSizes.size();
	
	// makes sure group indicies are not group 0
	if (groupIndex < 2 || groupIndex >= numGroups || adjGroupIndex < 1 || adjGroupIndex >= numGroups)
	{
		DBG("ScaleStructure: Resize Error - group 0 index present");
		return;
	}

	int symGroupIndex = -1;
	int symAdjGroup = -1;

	if (retainGroupingSymmetry)
	{
		symGroupIndex = getSymmetricGroup(groupIndex);
		symAdjGroup = getSymmetricGroup(adjGroupIndex);
	}

	bool resizeAndAdd = false;
	bool resizeAndMerge = false;

	int groupSize = degreeGroupScaleSizes[groupIndex];
	int adjGroupSize = degreeGroupScaleSizes[adjGroupIndex];

	// results in a new group
	if (groupIndex == symAdjGroup)
	{
		resizeAndAdd = true;
	}

	// results in one less group
	else if ((groupIndex == symGroupIndex && abs(sizeChangeAmount) * 2  == groupSize && draggedClockwise) || 
			 (adjGroupIndex == symAdjGroup && abs(sizeChangeAmount) * 2 == adjGroupSize && !draggedClockwise))
	{
		resizeAndMerge = true;
	}

	if (resizeAndAdd)
	{
		sizeChangeAmount = abs(sizeChangeAmount);
		groupSize -= sizeChangeAmount;
		adjGroupSize = sizeChangeAmount * 2;
	}
	else if (resizeAndMerge)
	{
		sizeChangeAmount = abs(sizeChangeAmount);
		if (draggedClockwise)
		{
			groupSize = adjGroupSize + sizeChangeAmount;
		}
		else
		{
			groupSize += sizeChangeAmount;
		}

		adjGroupSize = 0;
	}
	else
	{
		groupSize -= sizeChangeAmount;
		adjGroupSize += sizeChangeAmount;

		if (retainGroupingSymmetry)
		{
			if (groupIndex == symGroupIndex)
				groupSize -= sizeChangeAmount;
			
			else if (adjGroupIndex == symAdjGroup)
				adjGroupSize += sizeChangeAmount;
		}
	}

	int groupSizeIndex = scaleSizes.indexOf(groupSize / periodFactorSelected);
	int adjSizeIndex = scaleSizes.indexOf(adjGroupSize / periodFactorSelected);
	
	if (retainMOSGroupSizes)
	{
		if (groupSizeIndex < 0 || (adjGroupSize != 0 && adjSizeIndex < 0))
		{
			DBG("ScaleStructure: Resize Error - sizes not supported by MOS");
			return;
		}
	}

	Array<int> newGrouping;

	for (int i = 0; i < numGroups; i++)
	{
		if (resizeAndAdd)
		{
			if (i == groupIndex)
			{
				newGrouping.add(groupSizeIndex);
				newGrouping.add(adjSizeIndex);
				newGrouping.add(groupSizeIndex);
			}
			else if (i != adjGroupIndex & i != symGroupIndex)
				newGrouping.add(degreeGroupIndexedSizes[i]);
		}

		else if (resizeAndMerge)
		{
			if (groupIndex == symAdjGroup && (i == symGroupIndex || i == symGroupIndex - 1))
			{
				continue;
			}
			else if (groupIndex == symGroupIndex - 1 && (i == adjGroupIndex || i == symGroupIndex))
			{
				continue;
			}
			else if (i == groupIndex)
			{
				newGrouping.add(groupSizeIndex);
				newGrouping.add(groupSizeIndex);
			}
			else if (i == adjGroupIndex || i == symAdjGroup || i == symGroupIndex)
			{
				continue;
			}
			else
				newGrouping.add(degreeGroupIndexedSizes[i]);
		}

		else if ((i == groupIndex && adjGroupSize > 0) || i == symGroupIndex)
			newGrouping.add(groupSizeIndex);

		else if (i == adjGroupIndex || i == symAdjGroup)
			newGrouping.add(adjSizeIndex);

		else
			newGrouping.add(degreeGroupIndexedSizes[i]);
	}

	setDegreeGrouping(newGrouping);
}

/*
	Merges two adjacent groups. The new group size must be a valid size that the scale structure supports.
	If mergedClockwise is true, the group clockwise to the passed in groupIndex will be consumed.
	If retaining symmetry, this also effects the group on the other side.
*/
void ScaleStructure::mergeDegreeGroups(int groupIndex)
{
	int numGroups = degreeGroupScaleSizes.size();
	Array<int> newGrouping;

	int adjGroupIndex = groupIndex - 1;

	// Checks if adjacent group is not group 0 and if the new size index is valid
	if (groupIndex < 2 || groupIndex >= numGroups || adjGroupIndex < 1 || adjGroupIndex >= numGroups)
	{
		DBG("Scale Structure: Merge Error - group 0 index present");
		return;
	}

	int newGroupSize = degreeGroupScaleSizes[groupIndex] + degreeGroupScaleSizes[adjGroupIndex];		
	int symGroupIndex = getSymmetricGroup(groupIndex);
	int symAdjGroup = symGroupIndex + 1;
	
	bool ignoreSymIndicies = !retainGroupingSymmetry;
	if (retainGroupingSymmetry)
	{
		if (groupIndex == symGroupIndex)
		{
			newGroupSize += degreeGroupScaleSizes[adjGroupIndex];
			ignoreSymIndicies = true;
		}
		else if (adjGroupIndex == symAdjGroup)
		{
			newGroupSize += degreeGroupScaleSizes[groupIndex];
			ignoreSymIndicies = true;
		}
	}
	
	int newSizeIndex = scaleSizes.indexOf(newGroupSize / periodFactorSelected);

	if (retainMOSGroupSizes && newSizeIndex < 0)
	{
		DBG("Scale Structure: Merge Error - size not supported by MOS");
		return;
	}

	for (int i = 0; i < degreeGroupIndexedSizes.size(); i++)
	{

		if (i == groupIndex)
			newGrouping.add(newSizeIndex);

		else if (i == adjGroupIndex || i == symAdjGroup)
			continue;

		else if (i == symGroupIndex)
		{
			if (ignoreSymIndicies)
			{
				continue;
			}

			newGrouping.add(newSizeIndex);
		}

		else
			newGrouping.add(degreeGroupIndexedSizes[i]);
	}

	setDegreeGrouping(newGrouping);
}

int ScaleStructure::getSuggestedGeneratorIndex()
{
	int index = -1;
	float dif1, dif2 = 10e6;
	float interval = 1200.0f / fPeriod;
	int suggestedCents = 700;

	for (int i = 1; i < validGenerators.size(); i++)
	{
		dif1 = abs(suggestedCents - interval * validGenerators[i]);

		if (dif1 < dif2)
		{
			dif2 = dif1;
			index = i;
		}
	}

	return index;
}

int ScaleStructure::getSuggestedSizeIndex()
{
	int index = -1;
	int dif1, dif2 = 0x7fffffff;
	int scaleSize = 7 / periodFactorSelected;

	for (int i = 0; i < scaleSizes.size(); i++)
	{
		dif1 = abs(scaleSize - scaleSizes[i]);

		if (dif1 < dif2)
		{
			dif2 = dif1;
			index = i;
		}
	}

	return index;
}

int ScaleStructure::getPrefferedSizeIndex(int prefferedSize, bool preferLarger)
{
	int index = -1;
	int dif1, dif2 = 0x7fffffff;
	int scaleSize = prefferedSize / periodFactorSelected;

	for (int i = 0; i < scaleSizes.size(); i++)
	{
		dif1 = abs(scaleSize - scaleSizes[i]);

		if (dif1 < dif2 || (preferLarger && dif1 == dif2))
		{
			dif2 = dif1;
			index = i;
		}
	}

	return index;
}

Array<int> ScaleStructure::getNestedSizeGrouping()
{
	int scaleSize = getScaleSize();
	Array<int> grouping = { sizeIndexSelected };

	int notesLeft = fPeriod - scaleSize;
	int subSizeInd = sizeIndexSelected;
	int subSize = jmax(1, scaleSize);

	while (notesLeft > 0)
	{
		int q = notesLeft / subSize;

		if (notesLeft <= subSize && scaleSizes.contains(notesLeft))
		{
			grouping.add(scaleSizes.indexOf(notesLeft));
			notesLeft = 0;
		}
		else if (q >= 2)
		{
			int numToAdd = notesLeft % subSize == 0 ? q : q - (q % 2);
			for (int n = 0; n < numToAdd; n++)
			{
				grouping.add(subSizeInd);
				notesLeft -= subSize;

				// check if notesLeft can be divided equally by next size
				if (subSizeInd > 0)
				{
					int stagedSubInd = subSizeInd - 1;
					int stagedSubSize = scaleSizes[stagedSubInd];
					int q2 = notesLeft / stagedSubSize;

					if (notesLeft % stagedSubSize == 0)
					{
						// extra check for symmetry
						if (q2 % 2 == 0)
						{
							for (int qn = 0; qn < q2; qn++)
							{
								grouping.add(stagedSubInd);
								notesLeft -= stagedSubSize;
							}

							break;
						}
					}
				}
			}
		}

		subSize = scaleSizes[--subSizeInd];
	}

	//DBG("Nested group:");
	//String dbgstr = "";
	//for (int i = 0; i < grouping.size(); i++)
	//	dbgstr += String(scaleSizes[grouping[i]]) + ", ";
	//DBG(dbgstr);

	return grouping;
}

// TODO: possibility for improving large scales groupings.
// When checking the "staged" sizes, there could be an additional check to see if
// a different configuration of those two sizes produce a smaller grouping
// ex. with this algorithm, "Schismatic" 69/118 scale gives (7 * 4) + (5 * 18) or 22 groups
// but this could be reduced to (7 * 14) + (5 * 4) for 18 groups
Array<int> ScaleStructure::getComplimentarySizeGrouping()
{
	int scaleSize = getScaleSize();
	Array<int> grouping = { sizeIndexSelected };

	int notesLeft = fPeriod - scaleSize;
	int subSizeInd = sizeIndexSelected;
	int subSize = jmax(1, scaleSize);

	int q = notesLeft / subSize;
	int numToAdd = notesLeft % subSize == 0 ? q : q - (q % 2);
	int num = 0;

	while (notesLeft > 0)
	{
		// check if notesLeft can be divided equally by next size
		if (subSizeInd > 0)
		{
			int stagedSubInd = subSizeInd - 1;
			int stagedSubSize = scaleSizes[stagedSubInd];
			int q2 = notesLeft / stagedSubSize;

			if (notesLeft % stagedSubSize == 0)
			{
				// extra check for symmetry
				int groupSize = grouping.size() - 1;
				if (groupSize == 0 || q2 % 2 == 0)
				{
					for (int qn = 0; qn < q2; qn++)
					{
						grouping.add(stagedSubInd);
						notesLeft -= stagedSubSize;
					}

					break;
				}
			}
		}

		if (notesLeft <= subSize && scaleSizes.contains(notesLeft))
		{
			grouping.add(scaleSizes.indexOf(notesLeft));
			notesLeft = 0;
		}
		else if (q >= 0 && num < numToAdd)
		{
			grouping.add(subSizeInd);
			notesLeft -= subSize;
			num++;
		}
		else if (notesLeft > 0)
		{
			subSize = scaleSizes[--subSizeInd];
			q = notesLeft / subSize;
			numToAdd = notesLeft % subSize == 0 ? q : q - (q % 2);
			num = 0;
		}
	}
	
	//DBG("Complimentary group:");
	//String dbgstr = "";
	//for (int i = 0; i < grouping.size(); i++)
	//	dbgstr += String(scaleSizes[grouping[i]]) + ", ";
	//DBG(dbgstr);

	return grouping;
}

void ScaleStructure::useSuggestedSizeGrouping()
{
	Array<Array<int>> groupings;
	groupings.add(getNestedSizeGrouping());
	groupings.add(getComplimentarySizeGrouping());

	Array<float> groupingScores;

	// create score by averaging range and length of group
	for (auto g : groupings)
	{
		int range = g[0] - g[g.size() - 1];
		groupingScores.add((range + g.size()) / (float)groupings.size());
	}

	// find lowest score and return respective group
	int index = 0;
	for (int s = 0; s < groupings.size(); s++)
	{
		if (groupingScores[s] < groupingScores[index])
			index = s;
	}

	// make symmetric and fill degree groupings
	degreeGroupIndexedSizes = arrangeSymmetrically(groupings[index]);

	//DBG("Symmetric group:");
	//String dbgstr = "";
	//for (int i = 0; i < degreeGroupIndexedSizes.size(); i++)
	//	dbgstr += String(scaleSizes[degreeGroupIndexedSizes[i]]) + ", ";
	//DBG(dbgstr);

	fillSymmetricGrouping();
}

String ScaleStructure::getIntervalSteps(Point<int>& stepSizesOut, bool withModifications)
{
	Array<int> sizes;

	if (!withModifications)
	{
		for (int i = 0; i < getScaleSize(); i++)
		{
			for (int p = 0; p < periodFactorSelected; p++)
				sizes.add(generatorChain[i + fPeriod * p]);
		}
	}
	else
	{
		sizes = degreeGroupings[0];
	}

	sizes.add(period);
	sizes.sort();

	String steps = "";
	for (int i = 1; i <= getScaleSize() * periodFactorSelected; i++)
	{
		sizes.set(i - 1, sizes[i] - sizes[i - 1]);
#if JUCE_DEBUG
		steps += String(sizes[i - 1]) + " ";
#endif
	}

	// Extract step sizes
	stepSizesOut.x = sizes[0];
	for (auto step : sizes)
	{
		if (step != stepSizesOut.x)
		{
			stepSizesOut.y = step;
			break;
		}
	}

	DBG("ScaleStructure: MOS scale step sizes are: " + stepSizesOut.toString());

	return steps.substring(0, steps.length() - 1);
}

String ScaleStructure::getIntervalSteps(bool withModifications)
{
	if (withModifications)
	{
		Point<int> dummySteps;
		return getIntervalSteps(dummySteps, withModifications);
	}
	else
	{
		Array<int> intervals = scalesInIntervals[sizeIndexSelected];
		return arrayToString(intervals);
	}
}

String ScaleStructure::getLsSteps()
{
	Point<int> sizes;
	String steps = getIntervalSteps(sizes);

	String L = String(jmax(sizes.x, sizes.y));
	String s = String(jmin(sizes.x, sizes.y));

	// TODO: make more efficent
	while (steps.containsWholeWord(L))
		steps = steps.replaceFirstOccurrenceOf(L, "L");

	while (steps.containsWholeWord(s))
		steps = steps.replaceFirstOccurrenceOf(s, "s");

	return steps;
}
