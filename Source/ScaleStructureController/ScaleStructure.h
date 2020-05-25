/*
  ==============================================================================

	ScaleStructure.h
	Created: 25 Jan 2020 5:12:32pm
	Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ScaleMath.h"

class ScaleStructure
{
	int period;
	int generatorIndex = -1;
	int periodFactorIndexSelected = 0;
	int sizeIndexSelected = -1;
	int generatorOffset = 0;

	int periodFactorSelected;
	int fPeriod; // The fractional period, if a period factor greater than one is chosen

	Array<int> periodFactors; // Used for fractional period scales
	Array<int> validGenerators; // Generators coprime with chosen period
	Array<int> scaleSizes; // Sizes supported by Period & Generator combo

	Array<Point<int>> keyboardTypes; // Erv Wilson's notation
	Array<PointPair<int>> pgCoords; // Hex Coordinates of period and generator
	Array<Point<int>> stepSizes; // Hex step sizes

	Array<int> generatorChain;
	Array<int> degreeGroupIndexedSizes;
	Array<int> degreeGroupScaleSizes;
	Array<Array<int>> degreeGroupings;

	Array<int> chromaAlterations;

	// Calculates the properties related to the Period & Generator combo
	void calculateProperties();

	// Uses scale properties to determine hex step sizes
	void calculateStepSizes();

	// Enumerates the scale as a chain of generators
	void calculateGeneratorChain();

	// Fills arrays of degrees based on degreeGroupIndexedSizes. Use this if group sizes haven't been arranged symmetrically
	// Starts at beginning of generatorChain and puts the succeeding degrees in the second group, but wraps to degrees
	// preceding the generatorChain beginning into the third group, and alternates to fill all degrees
	void fillGroupingSymmetrically();

	// Takes an already 'symmetricized' group size array and fills groups of degrees based on this.
	void fillSymmetricGrouping();

	// Swaps degrees in generator chain based off of modifications
	void applyChromaAlterations();

public:

	ScaleStructure() {};
	ScaleStructure(int periodIn);
	ScaleStructure(int periodIn, int genIndexIn, int sizeIndexIn, Array<int> degreeGroupsIn = Array<int>());
	ScaleStructure(const ScaleStructure& scaleToCopy);
	ScaleStructure(ValueTree scaleStructureProperties);

	void resetToPeriod(int periodIn);

	/*
		Set all properties efficiently. If generator, size, or degreeGroups are invalid, the suggested index is used.
	*/
	void setAll(
		int periodIn,
		int generatorIndexIn,
		int sizeIndexIn,
		int generatorOffsetIn = 0,
		int periodFactorIndexIn = 0,
		Array<int> degreeGroupSizeIndiciesIn = Array<int>(),
		Array<int> chromaAlterationsIn = Array<int>());

	/*
		Returns the chosen period. If 'true' is passed in, the period is divided by
		the factor, if a fractional period is selected.
	*/
	int getPeriod(bool ofSelectedFactor = false) const;
	
	Array<int> getPeriodFactors() const;
	int getPeriodFactorIndex() const;
	int getPeriodFactor() const;

	Array<int> getValidGenerators() const;
	const Array<int>& getValidGeneratorsReference();
	int getValidGenerator(int genIndex) const;
	int getGenerator() const;
	int getGeneratorIndex() const;

	Array<int> getScaleSizes() const;
	int getScaleSize(int ind) const;

	/*
		Returns the currently selected scale size
	*/
	int getScaleSize() const;
	int getScaleSizeIndex() const;

	int getGeneratorOffset() const;

	Array<Point<int>> getKeyboardTypes() const;
	Point<int> getKeyboardType(int ind) const;

	Array<PointPair<int>> getPGCoords() const;
	PointPair<int> getPGCoord(int ind) const;

	Point<int> getStepSizes(int kbdTypeIn) const;
	Point<int> getStepSize() const;

	Array<int> getGeneratorChain() const;
	const Array<int>& getGeneratorChainReference();

	// Returns an array of indicies reffering to scale sizes of degree groups
	Array<int> getGroupingIndexedSizes() const;

	// Returns an array of actual group sizes, for visualization use
	const Array<int>& getGroupingSizesReference();

	Array<Array<int>> getDegreeGroupings() const;
	const Array<Array<int>>& getDegreeGroupingsReference() const;
	int getGroupOfDegree(int scaleDegreeIn) const;

	Array<int> findDegreeMods(int degreeIndex, int chromaLevels) const;
	
	Array<int> getChromaAlterations() const;
	int getAlterationOfDegree(int degreeIn) const;
	int getDegreeIndexOfAlteration(int degreeIn, int alteration) const;

	void setAlterationOfDegree(int degreeIn, int alteration);
	void addAlterationToDegree(int degreeIn, int alterSize);
	void resetAlterationOfDegree(int degreeIn);

	bool isValid() const;

	void setPeriodFactorIndex(int index);
	void setGeneratorIndex(int index);
	void setSizeIndex(int index);
	void setGeneratorOffset(int offsetIn);

	/*
		Input a mapping of scale degrees and chroma alteration values.
		Index refers to generator chain degree, and value is how many chromas it's moved by
		Must be equal to size of period
	*/
	bool setChromaAlterations(Array<int> modmosPropertiesIn);

	// Returns the index whose generator is closest to a perfect fifth
	int getSuggestedGeneratorIndex();

	// Returns the index whose size is closest to 7
	int getSuggestedSizeIndex();
	int getPrefferedSizeIndex(int prefferedSize, bool preferLarger = true);

	Array<int> getNestedSizeGrouping();
	Array<int> getComplimentarySizeGrouping();
	void useSuggestedSizeGrouping();

	// Returns the scale as a string of step sizes and puts integer values into passed in point reference
	String getIntervalSteps(Point<int>& stepSizesOut);

	// Returns the scale as a string of step sizes
	String getIntervalSteps();

	// Returns the scale as a string of L and s step sizes
	String getLsSteps();
};