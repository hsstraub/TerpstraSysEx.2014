/*
  ==============================================================================

    NoteNames.cpp
    Created: 14 Jun 2020 2:14:20pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "NoteNames.h"

NoteNames::NoteNames(const ScaleStructure& structureIn)
	: scaleStructure(structureIn)
{
	useNumericalDefault();
}

NoteNames::~NoteNames()
{

}

// TODO: some copy constructor / recall constructor


void NoteNames::setNoteNameFont(Font noteNameFontIn)
{
	noteNameFont = noteNameFontIn;
}

void NoteNames::setAccidentalFont(Font accidentalFontIn)
{
	accidentalFont = accidentalFontIn;
}

void NoteNames::setNominalSize(int sizeIndexIn)
{
	nominalSizeIndex = sizeIndexIn;
	nominalSize = scaleStructure.getScaleSize(sizeIndexIn);
}

void NoteNames::setNominals(int sizeIndexIn, StringArray nominalsLabelsIn)
{
	setNominalSize(sizeIndexIn);
	nominals = nominalsLabelsIn;
}

void NoteNames::setNominal(int nominalIndex, String noteNameIn)
{
	nominals.set(nominalIndex, noteNameIn);
}

void NoteNames::setGroupAccidentals(StringArray accidentalsIn)
{
	groupAccidentals = accidentalsIn;
}

void NoteNames::setGroupAccidental(int groupIndex, String accidentalIn)
{
	groupAccidentals.set(groupIndex, accidentalIn);
}

Font NoteNames::getNoteNameFont()
{
	return noteNameFont;
}

Font NoteNames::getAccidentalFont()
{
	return accidentalFont;
}

String NoteNames::getScaleDegreeName(int scaleDegreeIn)
{
	int groupNum = scaleStructure.getGroupOfDegree(scaleDegreeIn);
	int groupInd = scaleStructure.getDegreeGroupings().getReference(groupNum).indexOf(scaleDegreeIn);

	return nominals[groupInd] + groupAccidentals[groupNum];
}

String NoteNames::getNominalName(int nominalDegreeIn)
{
	return getScaleDegreeName(scaleStructure.getNominalScaleDegrees()[nominalDegreeIn]);
}

String NoteNames::getChainIndexName(int chainIndexIn)
{
	return getScaleDegreeName(scaleStructure.getGroupChain()[chainIndexIn]);
}

void NoteNames::useAlphabeticalDefault()
{
	brightGenerator = scaleStructure.getGenerator() > (scaleStructure.getPeriod() / 2.0f);

	nominals.clear();
	Array<int> nominalGroup = scaleStructure.getDegreeGroupings()[0];
	char c = 'A';
	for (int i = 0; i < nominalSize; i++)
	{
		nominals.add(String::charToString(c + i));
	}

	useGenericAccidentals();
}

void NoteNames::useNumericalDefault()
{
	brightGenerator = scaleStructure.getGenerator() > (scaleStructure.getPeriod() / 2.0f);

	Array<int> nominalGroup = scaleStructure.getDegreeGroupings()[0];
	for (int i = 0; i < nominalSize; ++i)
	{
		nominals.add(String(i));
	}

	useGenericAccidentals();
}

void NoteNames::useGenericAccidentals()
{
	int numGroups = scaleStructure.getDegreeGroupings().size();
	int half = numGroups / 2;
	if (numGroups % 2 == 0)
		half -= 1;

	StringArray raised, lowered;

	for (int i = 0; i < half; i++)
	{
		String r, l;

		for (int j = i; j < (i + 1); j++)
		{
			r += defaultRaised;
			l += defaultLowered;
		}
		
		raised.add(r);
		lowered.add(l);
	}

	StringArray& clockwise = brightGenerator ? raised : lowered;
	StringArray& counterclockwise = brightGenerator ? lowered : raised;

	groupAccidentals.clear();
	
	// Natural
	groupAccidentals.add("");

	for (int i = 0; i < half; i++)
	{
		groupAccidentals.add(clockwise[i]);
	}

	if (numGroups % 2 == 0)
		groupAccidentals.add(defaultNeutral);

	for (int i = 0; i < half; i++)
	{
		groupAccidentals.add(counterclockwise[half - i - 1]);
	}
}