/*
  ==============================================================================

    KBMMappingDataStructure.h
    Created: 10 Apr 2020 11:53:40am
    Author:  hsstraub

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class KBMMappingDataStructure
{
public:
    KBMMappingDataStructure();

    bool isValid() const { return firstMIDINoteNr <= lastMIDINoteNr && theMapping.size() <= mapSize; }
    bool isEmpty() const { return firstMIDINoteNr > lastMIDINoteNr; }

    // Read from a string array as it stands in a KBM file
	bool fromStringArray(const StringArray& stringArray);

	// Convert to string array, to be written to a KBM file
	StringArray toStringArray(const String& description);

// Attributes
public:
    // Size of map. The pattern repeats every so many keys. Maybe 0
   int mapSize;

   //First MIDI note number to retune
   int firstMIDINoteNr;

   // Last MIDI note number to retune
   int lastMIDINoteNr;

   // Middle note where the first entry of the mapping is mapped to
   int noteNrWhereMappingStarts;

   // Reference note for which frequency is given
   int referenceNoteNr;

   // Frequency to tune the above note to (floating point e.g. 440.0)
   float referenceNoteFrequency;

   // Scale degree to consider as formal octave (determines difference in pitch between adjacent mapping patterns)
   int scaleSize;

   // Mapping. <= 'mapSize' values
   // The numbers represent scale degrees mapped to keys. The first entry is for
   // the given middle note, the next for subsequent higher keys.
   // A value of -1 means an unmapped key. At the end, unmapped keys may be left out.
   Array<int> theMapping;

};
