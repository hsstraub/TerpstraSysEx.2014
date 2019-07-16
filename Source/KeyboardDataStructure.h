/*
==============================================================================

KeyboardDataStructure.h
Created: XXX
Author:  hsstraub

==============================================================================
*/

#ifndef KEYBOARDDATASTRUCTURE_INCLUDED
#define KEYBOARDDATASTRUCTURE_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


// Mapping of one key
class TerpstraKey 
{
public:
	typedef enum
	{
		noteOnNoteOff = 1,
		continuousController = 2
	} KEYTYPE;

public:
	TerpstraKey() { noteNumber = 0; channelNumber = 0; colour = 0; keyType = noteOnNoteOff; };
	bool isEmpty() const { return channelNumber == 0; }

public:
	int		noteNumber;
	int		channelNumber;
	int		colour;
	KEYTYPE	keyType;
};

// Subset of 55 keys
#define TERPSTRABOARDSIZE 55

struct TerpstraKeys {
	TerpstraKey		theKeys[TERPSTRABOARDSIZE];
	int				board_idx;
	int				key_idx;

	TerpstraKeys();
};

/*
==============================================================================
A complete key mapping
==============================================================================
*/
#define NUMBEROFBOARDS 5
class TerpstraKeyMapping
{
public:
	TerpstraKeyMapping();
	
	void clearAll();

	void fromStringArray(const StringArray& stringArray);
	StringArray toStringArray();
	// The colours that are used 
	SortedSet<int> getUsedColours();

public:
	TerpstraKeys	sets[NUMBEROFBOARDS];
};

#endif