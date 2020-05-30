/*
  ==============================================================================

    GroupingCircle.h
    Created: 26 Apr 2020 4:59:12pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ScaleStructure.h"

//==============================================================================
/*
*/
class GroupingCircle    : public Component
{
public:
    GroupingCircle(const ScaleStructure& structureIn, Array<Colour>& colourTableIn);
    ~GroupingCircle();

	float getInnerRadius() const;
	float getMiddleRadius() const;
	float getOuterRadius() const;

	Rectangle<float> getInnerCircleBounds() const;
	Rectangle<float> getMiddleCircleBounds() const;
	Rectangle<float> getOuterCircleBounds() const;

	Point<float> getCenter() const;
	Point<float> getFloatPointFromCenter(float radius, float angle) const;
	Point<int> getIntPointFromCenter(float radius, float angle) const;

	void updatePeriod();
	void updateGenerator();

	void degreeToModSelectedCallback(int degreeIndex);
	void cancelDegreeMods();

    void paint (Graphics&) override;
    void resized() override;

	void mouseMove(const MouseEvent& event) override;
	void mouseDown(const MouseEvent& event) override;
	void mouseDrag(const MouseEvent& event) override;


	class Listener
	{
	public:
		~Listener() {};

		virtual void offsetChanged(int newOffset) = 0;
		virtual void degreeIndexAltered(int degreeIndex, Point<int> alteration) = 0;
		virtual void allModificationsReset() = 0;
	};

	void addListener(Listener* listenerToAdd);
	void removeListener(Listener* listenerToRemove);

	// Arc path helper
	static void addArcToPath(Path& pathIn, Rectangle<float>& ellipseBounds, float fromRadians, float toRadians, bool startAsNewSubPath = false);

protected:
	ListenerList<Listener> listeners;

private:
	OwnedArray<TextEditor> degreeLabels;
	OwnedArray<Label> groupSizeLabels;

	const ScaleStructure& scaleStructure;

	// Starting from the first degree of the primary degree grouping going clockwise
	const Array<Colour>& colourTable;

	// The groups of scale degree arrays
	Array<Array<int>> degreeGroupings;

	// The chain of scale degrees by stacking the chosen generator
	Array<int> groupChain;

	// Starting from the top (scale size selected) going clockwise
	Array<int> groupSizes;

	PopupMenu groupMenu;
	PopupMenu degreeMenu;
	
	// MODMOS functionality
	int degreeIndexToMod = -1;
	Array<Point<int>> degreeModCandidates;
	Array<Point<int>> chromaAlterations;
	Array<Point<int>> degreeAlterations;

	// Mouse functionality
	
	int mouseRadius, mouseDownRadius;
	int degreeSectorMouseOver = -1;
	int groupSectorMouseOver = -1;
	int lastDegreeSectorMouseIn = -1;
	int lastGroupSectorMouseIn = -1;

	// Drawing related members
	float borderRatio = 127.0f / 128.0f;
	const float groupWidthRatio = 1.0f / 10.0f;
	const float degreeWidthRatio = 1.0f / 15.0f;

	float groupOuterRadius;
	float groupMiddleRadius;

	float degreeOuterRadius;
	float degreeInnerRadius;
	float degreeMiddleRadius;

	Point<float> center;
	float circleOffset = float_Pi / 2.0f;

	float groupRingWidth;
	float degreeRingWidth;

	Rectangle<float> groupOuterCircleBounds;
	Rectangle<float> groupInnerCircleBounds;
	Rectangle<float> degreeInnerCircleBounds;

	double angleIncrement;
	double angleHalf;
	const float float_HalfPi = float_Pi / 2;
	const float float_Tau = float_Pi * 2;

	Array<Line<float>> radiLines;
	Array<Path> degreeArcPaths;
	Array<Path> groupArcPaths;
	PathStrokeType strokeType = PathStrokeType(2.0f);

	float sectorLabelSizeRatio = 0.875f;

	float highlightContrastRatio = 1.0f / 6.0f;
	float labelContrastRatio = 2.0f / 3.0f;

private:

	/*
		Returns the angle of the mouse based on the circle's center, with 0 starting at the counter-clockwise edge 
		of the first degree group's degree, and moving clockwise.
	*/
	float getNormalizedMouseAngle(const MouseEvent& event) const;

	/*
		Returns the degree index of the degree's corresponding sector that the angle is in.
	*/
	int degreeSectorOfAngle(float angle) const;

	/*
		Returns the degree index that the mouse is in if in the area of a degree ring sector.
		Returns -1 if outside of ring sector.
	*/
	int mouseInDegreeRingSector(const MouseEvent& event, float radiusFromCenter, float angle) const;

	/*
		Returns the group size index of the group's corresponding sector that the mouse is in
	*/
	int mouseInGroupSector(int degreeIndex) const;

	/*
		Returns the group size index that the mouse is in if in the area of a group size ring sector.
		Returns -1 if outside of ring sector.
	*/
	int mouseInGroupRingSector(const MouseEvent& event, float radiusFromCenter, float angle) const;
	int mouseInGroupRingSector(const MouseEvent& event, float radiusFromCenter, int degreeIndex) const;

	/*
		Returns true if degree sector is a modification candidate
	*/
	bool isDegreeSectorIndexModCandidate(int degreeSectorIndexIn) const;

	/*
		Returns true if degree sector is altered
	*/
	bool isDegreeSectorIndexAltered(int degreeSectorIndexIn) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GroupingCircle)
};
