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

    theMapping = Array<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
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
    theMapping.clear();
    for ( int i = 7; i < valuesWithoutComments.size(); i++)
        theMapping.add(valuesWithoutComments[i].indexOf("x") >= 0 ? -1 : valuesWithoutComments[i].getIntValue());

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

	for ( int i = 0; i<theMapping.size(); i++ )
        result.add(theMapping[i] >= 0 ? String(theMapping[i]) : "x");

	return result;
}
