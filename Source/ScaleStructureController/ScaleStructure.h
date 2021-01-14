/*
  ==============================================================================

	ScaleStructure.h
	Created: 25 Jan 2020 5:12:32pm
	Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Common.h"
#include "Symmetry.h"

class ScaleStructure
{
	int period;
	int generatorIndex = -1;
	int periodFactorIndexSelected = 0;
	int sizeIndexSelected = -1;
	int generatorOffset = 0;

	int periodFactorSelected; // The factor by which the period is reduced
	int fPeriod; // The fractional period, if a period factor greater than one is chosen

	Array<int> periodFactors; // Used for fractional period scales
	Array<int> validGenerators; // Generators coprime with chosen period
	Array<int> scaleSizes; // Sizes supported by Period & Generator combo

	Array<Point<int>> keyboardTypes; // Erv Wilson's notation
	Array<PointPair<int>> pgCoords; // Hex Coordinates of period and generator
	Array<Point<int>> stepSizes; // Hex step sizes

	// Array of scale steps each scale size level creates
	Array<Array<int>> scalesInIntervals;

	// A list of scale degrees from the generator stacked period number of times. Takes offset into account.
	// For fractional periods, 
	Array<int> generatorChain;

	// Array of indicies referring to the support scale sizes
	Array<int> degreeGroupIndexedSizes;

	// Array of the real sizes of the degree groups
	Array<int> degreeGroupScaleSizes;

	// Array of groups of degrees, which refer to the colour index they should use
	Array<Array<int>> degreeGroupings;

	// List of scale degrees of how they're grouped - different than generatorChain for fractional periods
	Array<int> groupChain;

	// Map of scale degree -> groupChain index
	Array<int> groupChainDegreeIndicies;

	// Map of group chain indicies -> degree group index
	Array<int> degreeGroupChainMap;

	// Sorted array of scale degrees that make up the first degree group
	Array<int> nominalScaleDegrees;

	// List of user-chosen alterations by group chain index
	// X-value is chroma level, Y-value is alteration amount
	// A chroma level of -1 is used if there is no alteration
	Array<Point<int>> chromaAlterations;
	
	// List of chroma alterations of each scale degree
	Array<Point<int>> degreeAlterations;

	// An option for handling degree alterations when offset is changed
	// May removed one method if one is determined to be objectively wrong
	bool alterationsAttachedToDegree = false;

	// Used in degree grouping related functions to make sure that the groups 
	// can be arranged on a circle with symmetrically so that each group has
	// a group of the same size on the other side of the circle (horizontally across),
	// except for the first group and the middle group (for odd-amounts of groups).
	bool retainGroupingSymmetry = true;

	// Used in degree grouping related functions to make sure that the groups'
	// sizes are restricted to sizes supported by the MOS properties
	// This allows for consistency with the scale's natural properties which 
	// lends well particularly for notation and theory.
	bool retainMOSGroupSizes = true;

private:
	/*
		Private methods
	*/

	// Calculates the properties related to the Period & Generator combo
	void calculateProperties();

	// Uses scale properties to determine hex step sizes
	void calculateStepSizes();

	void calculateIntervalScales();

	// Enumerates the scale as a chain of generators
	void calculateGeneratorChain();

	// Takes an symmetric group size array and fills groups of degrees based on this.
	void fillSymmetricGrouping(bool applyAlterations=true);

	// Swaps degrees in generator chain based off of modifications
	void applyChromaAlterations();

public:

	ScaleStructure();
	ScaleStructure(
		int periodIn, 
		int genIndexIn, 
		int sizeIndexIn, 
		int genOffsetIn,
		int periodFactorIndexIn = 0, 
		Array<int> degreeGroupsIn = Array<int>(),
		Array<Point<int>> chromaAlterationsIn = Array<Point<int>>()
	);
	ScaleStructure(const ScaleStructure& scaleToCopy);
	ScaleStructure(ValueTree scaleStructureProperties);

	/*
		Set the period and use suggested and default values for everything else
	*/
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
		Array<Point<int>> chromaAlterationsIn = Array<Point<int>>()
	);

	/*
		Checks if the ScaleStructure has necessary data to be used
	*/
	bool isValid() const;

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
	Array<int> getGroupChain() const;

	bool isRetainingSymmetry() const;

	bool isRetainingMOSSizes() const;

	/*
		Returns the group chain index of the input scale degree
	*/
	int getGroupChainIndexOfDegree(int degreeIn) const;

	// Returns an array of indicies reffering to scale sizes of degree groups
	Array<int> getGroupingIndexedSizes() const;

	/*
		Returns a list of actual group sizes
	*/
	Array<int> getDegreeGroupSizes() const;

	/*
		Returns an array of scale sizes referring to the degree group sizes
	*/
	Array<Array<int>> getDegreeGroupings() const;
	
	/* 
		Returns the index of the group the scale degree is a part of.
		Used for determining the colour of the scale degree 
	*/
	int getGroupOfDegree(int scaleDegreeIn) const;

	/*
		Returns the index of the group the group chain index falls in
	*/
	int getGroupOfDegreeIndex(int groupChainIndex) const;

	/*
		Finds the generator chain indices a given degree can be altered to
	*/
	Array<Point<int>> findDegreeMods(int degreeIndex, int chromaLevels) const;
	
	/*
		Returns the list of user-chosen alterations of scale degrees
	*/
	Array<Point<int>> getChromaAlterations() const;

	/*
		Returns the list of alterations per scale degree
	*/
	Array<Point<int>> getDegreeAlterations() const;

	/*
		Returns the chroma alteration of a given scale degree
	*/
	Array<Point<int>> getAlterationOfDegree(int degreeIn) const;
	
	/*
		Given a group chain index and chroma alteration, this returns the altered degree's group chain index
	*/
	int findIndexedAlterationOfDegree(int degreeIndexIn, Point<int> alteration) const;

	/*
		Given a scale degree and chroma alteration, this returns the altered scale degree
	*/
	int findAlterationOfDegree(int degreeIn, Point<int> alteration) const;

	/*
		Toggles whether or not alterations move with altered degree or stay in place (of chain index) with offset changes.
		If true, patterns are not consistent when offset changes.
		This can mutate the original alterations if called after offset has been changed.
	*/
	void attachAlterationsToDegree(bool isAttachedToDegree);

	void setAlterationOfDegree(int degreeIndexIn, Point<int> alteration);
	void resetAlterationOfDegree(int degreeIndexIn);

	void setPeriodFactorIndex(int index);
	void setSizeIndex(int index);
	void setGeneratorOffset(int offsetIn);

	void setRetainGroupingSymmetry(bool isSymmetric);
	void setRetainMOSSizes(bool retainSizes);

	/*
		Input a mapping of scale degrees and chroma alteration values.
		Must be equal to size of period
	*/
	bool setChromaAlterations(Array<Point<int>> chromaAlterationsIn);

	Array<int> getNominalScaleDegrees() const;

	/*
		Returns the degree group on the other side of the grouping cirle horizontally if groupings are symmetric.
		If there is no opposite it will return itself.
		If the grouping is not symmetric, it will return -1;
	*/
	int getSymmetricGroup(int groupIndexIn) const;

	/*
		Input a degree group index and get chain indicies that the edge of a group can move to.
		If retainMOSGroupSizes, then the degree indicies are only what can create supported MOS sizes out of
		the group and adjacent group sizes.
		If retainGroupSymmetry, the y value corresponds to the degree indicies of the symmetric group.

		The first value will always refer to the adjacent CCW edge that the group edge can move to, and 
		the second value will always refer to the adjacent CW edge.
	*/
	Array<Point<int>> findIndiciesForGroupResizing(int groupIndexIn) const;


	/*
		Input a degree group index and get group chain indicies that the edge of a group can move to.
		If retainMOSGroupSizes, then the degree indicies are only what can create supported MOS sizes out of
		the new group size and the remaining new group to be created.
		If retainGroupSymmetry, the y value corresponds to the degree indicies of the symmetric group.
	*/
	Array<Point<int>> findIndiciesForGroupSplitting(int groupIndexIn, bool newGroupClockwise) const;

	/*
		Sets and updates degree grouping arrangment.
		If the passed in grouping contains invalid scale size indicies and if they do not
		add to the period, then it will not be applied.
		If retainSymmetry is true, this will check if the grouping is symmetric, and won't apply it if not.
	*/
	void setDegreeGrouping(Array<int> groupingSizeIndiciesIn);

	/*
		Splits a group index into two groups. The two sizes added must equal original size.
		If newGroupClockwise is true, the new group using the remainder size will be added
		clockwise to the original group.
		If retaining symmetry, this also effects the group on the other side.
	*/
	void splitDegreeGroup(int groupIndexIn, int sizeChangeAmount);

	/*
		Resizes two adjacent groups, where the passed in group's size will have the passed in sizeChangeAmount added.
		If retaining symmetry, this also effects the groups on the other side.
	*/
	void resizeDegreeGroup(int groupIndex, int sizeChangeAmount, bool draggedClockwise);

	/*
		Merges two adjacent groups. The new group size must be a valid size that the scale structure supports.
		If retaining symmetry, this also effects the group on the other side.
	*/
	void mergeDegreeGroups(int groupIndex);

	// Returns the index whose generator is closest to a perfect fifth
	int getSuggestedGeneratorIndex();

	// Returns the index whose size is closest to 7
	int getSuggestedSizeIndex();
	int getPrefferedSizeIndex(int prefferedSize, bool preferLarger = true);

	Array<int> getNestedSizeGrouping();
	Array<int> getComplimentarySizeGrouping();
	void useSuggestedSizeGrouping();

	/*
		Returns the scale as a string of step sizes and puts integer values into passed in point reference
	*/
	String getIntervalSteps(Point<int>& stepSizesOut, bool withModifications = true);

	// Returns the scale as a string of step sizes
	String getIntervalSteps(bool withModifications = true);

	// Returns the scale as a string of L and s step sizes
	String getLsSteps();
};