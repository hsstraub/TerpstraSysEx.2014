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
    periodSize = 12;

    noteMappingTable = Array<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
}

// Returns a descriptive error message if format is not valid, or an empty string
String KBMMappingDataStructure::getErrorMessage() const
{
    if (firstMIDINoteNr > lastMIDINoteNr)
        return "First MIDI note is higher than last MIDINote";

    if (noteMappingTable.size() > mapSize)
        return "Mapping table larger than mapping size";

    if (mapSize == 0)
        return "Map size is specified as 0, which is not supported by the tool at the moment";

    if (noteMappingTable[getMappingIndexOfReferenceNote()] < 0)
        return ("No mapping specified for reference note");

    return "";
}

// Read from a string array as it stands in a KBM file. Returns true if successful
bool KBMMappingDataStructure::fromStringArray(const StringArray& stringArray)
{
    StringArray valuesWithoutComments;

        // Omit empty and comment lines
    for ( int i = 0; i < stringArray.size(); i++)
    {
        if (stringArray[i].indexOf("!") < 0 && !stringArray[i].trim().isEmpty())
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
    periodSize = valuesWithoutComments[6].getIntValue();

    // The mapping is supposed to contain at most mapSize entries. We just read them all here.
    noteMappingTable.clear();
    int i;
    for ( i = 7; i < valuesWithoutComments.size(); i++)
        noteMappingTable.add(valuesWithoutComments[i].indexOf("x") >= 0 ? -1 : valuesWithoutComments[i].getIntValue());

    // If there are less entries than specified in mapSize, the rest is supposed to be "x"
    for ( ; i < mapSize+7; i++)
        noteMappingTable.add(-1);

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
	result.add(String(periodSize));
	result.add("! Mapping");
	result.add("! The numbers represent scale degrees mapped to keys. The first entry is for");
	result.add("! the given middle note, the next for subsequent higher keys.");
	result.add("For an unmapped key, put in an 'x'. At the end, unmapped keys may be left out.");

	for ( int i = 0; i<noteMappingTable.size(); i++ )
        result.add(noteMappingTable[i] >= 0 ? String(noteMappingTable[i]) : "x");

	return result;
}

int KBMMappingDataStructure::getMappingIndexOFMIDINote(int midiNoteNr) const
{
    jassert(mapSize > 0);

    int inx = (midiNoteNr - noteNrWhereMappingStarts) % mapSize;
    while (inx < 0)
        inx += mapSize;
    return inx;
}

// Create note - frequency table
KBMMappingDataStructure::NoteAndFrequencyTable KBMMappingDataStructure::createNoteFrequencyTable() const
{
    KBMMappingDataStructure::NoteAndFrequencyTable result;

    if (!isValid() || isEmpty())
        return result;

    // ToDo special logic with mapSize == 0
    jassert(mapSize > 0);

    int mapStartIndexOfReferenceNote = getMapStartIndexOfReferenceNote();
    int mapValueOfReferenceNote = noteMappingTable[getMappingIndexOfReferenceNote()];   // Supposed to have a value (was checked in isValid())
    float scaleStepInterval = pow(2, 1.0/periodSize);

    for ( int midiNoteNr = firstMIDINoteNr; midiNoteNr <= lastMIDINoteNr; midiNoteNr++)
    {
        int indexFromStartOfMap = getMappingIndexOFMIDINote(midiNoteNr);
        int currentMappingValue = noteMappingTable[indexFromStartOfMap];
        if (currentMappingValue < 0)
            continue;   // Not mapped

        int currentStartOfMap =  getMapStartIndexOfMIDINote(midiNoteNr);
        int octaveDifference = (currentStartOfMap - mapStartIndexOfReferenceNote) / mapSize;

        int relativeMapValueDifference = currentMappingValue - mapValueOfReferenceNote;

        noteAndFrequency resultValue;
        resultValue.noteNumber = midiNoteNr;
        resultValue.frequency = referenceNoteFrequency;

        if (octaveDifference >= 0)
        {
            while ( octaveDifference > 0)
            {
                resultValue.frequency *= 2;
                octaveDifference--;
            }
        }
        else
        {
            while ( octaveDifference < 0)
            {
                resultValue.frequency /= 2;
                octaveDifference++;
            }
        }


        if ( relativeMapValueDifference > 0)
        {
            while ( relativeMapValueDifference > 0)
            {
                resultValue.frequency *= scaleStepInterval;
                relativeMapValueDifference--;
            }
        }
        else
        {
            while ( relativeMapValueDifference < 0)
            {
                resultValue.frequency /= scaleStepInterval;
                relativeMapValueDifference++;
            }
        }

        result.add(resultValue);
    }

    return result;
}
