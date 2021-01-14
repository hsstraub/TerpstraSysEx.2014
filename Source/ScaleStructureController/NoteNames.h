/*
  ==============================================================================

    NoteNames.h
    Created: 14 Jun 2020 2:14:20pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ScaleStructure.h"

class NoteNames
{
	const ScaleStructure& scaleStructure;

	bool brightGenerator = false;

	Font noteNameFont;
	Font accidentalFont;

	// Used in building table
	StringArray nominals;
	StringArray groupAccidentals;

	int nominalSizeIndex = 0; // ScaleStructure scaleSizes index
	int nominalSize = 1;

	// TODO: improve
	const String defaultRaised = "#";
	const String defaultLowered = "b";
	const String defaultNeutral = "o";

public:

	NoteNames(const ScaleStructure& structureIn);
	~NoteNames();

	void setNoteNameFont(Font noteNameFontIn);
	void setAccidentalFont(Font accidentalFontIn);

	// TODO: some copy constructor / recall constructor

	void setNominalSize(int sizeIndexIn);

	void setNominals(int sizeIndexIn, StringArray nominalsLabelsIn);
	void setNominal(int nominalIndex, String noteNameIn);

	void setGroupAccidentals(StringArray accidentalsIn);
	void setGroupAccidental(int groupIndex, String accidentalIn);

	Font getNoteNameFont();
	Font getAccidentalFont();

	String getScaleDegreeName(int scaleDegreeIn);
	String getNominalName(int nominalDegreeIn);
	String getChainIndexName(int chainIndexIn);

	void useAlphabeticalDefault();
	void useNumericalDefault();
	void useGenericAccidentals();
};