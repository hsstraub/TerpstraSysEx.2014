/*
  ==============================================================================

    KBMMappingDataStructure.cpp
    Created: 10 Apr 2020 11:53:40am
    Author:  hsstraub

  ==============================================================================
*/

#include "KBMMappingDataStructure.h"

KBMMappingDataStructure::KBMMappingDataStructure()
{
    // Default values, for a one-to-one mapping of a 12 note scale to a standard keyboard
    mapSize = 12;
    firstMIDINoteNr = 0;
    lastMIDINoteNr = 127;
    noteNrWhereMappingStarts = 60;
    referenceNoteNr = 69;
    referenceNoteFrequency = 440.0;
    scaleSize = 12;

    noteMappingTable = Array<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
}

// Returns a descriptive error message if format is not valid, or an empty string
String KBMMappingDataStructure::getErrorMessage() const
{
    if (firstMIDINoteNr > lastMIDINoteNr)
        return "First MIDI note is higher than lastMIDINote";

    if (noteMappingTable.size() > mapSize)
        return "Mapping table larger than mapping size";

    if (mapSize == 0)
        return "Map size 0 is not supported at the moment";

    if (noteMappingTable[mappingIndexOfReferenceNote()] < 0)
        return ("No mapping specified for reference note");

    return "";
}

// Read from a string array as it stands in a KBM file. Returns true if successful
bool KBMMappingDataStructure::fromStringArray(const StringArray& stringArray)
{
    StringArray valuesWithoutComments;

        // Omit comment lines
    for ( int i; i < stringArray.size(); i++)
    {
        if (stringArray[i].indexOf("!") < 0)
            valuesWithoutComments.add(stringArray[i]);
    }

    if (valuesWithoutComments.size() < 7)
        return false;

    mapSize = valuesWithoutComments[0].getIntValue();
    firstMIDINoteNr = valuesWithoutComments[1].getIntValue();
    lastMIDINoteNr = valuesWithoutComments[2].getIntValue();
    noteNrWhereMappingStarts = valuesWithoutComments[3].getIntValue();
    referenceNoteNr = valuesWithoutComments[4].getIntValue();
    referenceNoteFrequency = valuesWithoutComments[5].getFloatValue();
    scaleSize = valuesWithoutComments[6].getIntValue();

    // The mapping is supposed to contain at most mapSizer entries. We just read them all here.
    noteMappingTable.clear();
    for ( int i = 7; i < valuesWithoutComments.size(); i++)
        noteMappingTable.add(valuesWithoutComments[i].indexOf("x") >= 0 ? -1 : valuesWithoutComments[i].getIntValue());

    return true;
}

// Convert to string array, to be written to a KBM file
StringArray KBMMappingDataStructure::toStringArray(const String& description)
{
	StringArray result;

	result.add("! " + description);
	result.add("! ");
	result.add("! Map size:");
	result.add(String(mapSize));
	result.add("! First MIDI note number to retune:");
	result.add(String(firstMIDINoteNr));
	result.add("! Last MIDI note number to retune:");
	result.add(String(lastMIDINoteNr));
	result.add("! Middle note where the first entry of the mapping is mapped to:");
	result.add(String(noteNrWhereMappingStarts));
	result.add("! Reference note for which frequency is given:");
	result.add(String(referenceNoteNr));
	result.add("! Frequency to tune the above note to (floating point e.g. 440.0):");
	result.add(String(referenceNoteFrequency));
	result.add("! Scale degree to consider as formal octave:");
	result.add(String(scaleSize));
	result.add("! Mapping");
	result.add("! The numbers represent scale degrees mapped to keys. The first entry is for");
	result.add("! the given middle note, the next for subsequent higher keys.");
	result.add("For an unmapped key, put in an 'x'. At the end, unmapped keys may be left out.");

	for ( int i = 0; i<noteMappingTable.size(); i++ )
        result.add(noteMappingTable[i] >= 0 ? String(noteMappingTable[i]) : "x");

	return result;
}

int KBMMappingDataStructure::mappingIndexOFMIDINote(int midiNoteNr) const
{
    // ToDO Logic for mapSize == 0
    return (midiNoteNr - noteNrWhereMappingStarts) % mapSize;
}

// Create note - frequency table
KBMMappingDataStructure::NoteAndFrequencyTable KBMMappingDataStructure::createNoteFrequencyTable() const
{
    KBMMappingDataStructure::NoteAndFrequencyTable result;

    if (!isValid() || isEmpty())
        return result;

    for ( int midiNoteNr = firstMIDINoteNr; midiNoteNr <= lastMIDINoteNr; midiNoteNr++)
    {
        // ToDo special logic with mapSize == 0
        int indexFromStartOfMap = mappingIndexOFMIDINote(midiNoteNr);
        int mappingValue = noteMappingTable[indexFromStartOfMap];
        if (mappingValue < 0)
            continue;   // Not mapped

        int indexFromReferenceNote = midiNoteNr - referenceNoteNr;

        int currentStartOfMap =  midiNoteNr - indexFromStartOfMap;

        // ToDO
    }

    return result;
}
