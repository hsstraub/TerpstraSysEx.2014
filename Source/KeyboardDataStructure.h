/*
==============================================================================

KeyboardDataStructure.h
Created: XXX
Author:  hsstraub

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


// Mapping of one key
class TerpstraKey
{
public:
	typedef enum
	{
		noteOnNoteOff = 1,
		continuousController = 2,
		lumaTouch = 3
	} KEYTYPE;

public:
	TerpstraKey() { noteNumber = 0; channelNumber = 0; colour = 0; keyType = noteOnNoteOff; };
	bool isEmpty() const { return channelNumber == 0; }

	bool operator!=(const TerpstraKey& second) const { return noteNumber != second.noteNumber || channelNumber != second.channelNumber || colour != second.colour || keyType != second.keyType; }

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

	bool isEmpty() const;
};

/*
==============================================================================
A complete key mapping
==============================================================================
*/
#define NUMBEROFBOARDS 5

// Number of entries in the velocity interval table
#define VELOCITYINTERVALTABLESIZE 127

class TerpstraKeyMapping
{
public:
	TerpstraKeyMapping();

	void clearVelocityIntervalTable();
	void clearAll();

	void fromStringArray(const StringArray& stringArray);
	StringArray toStringArray();

	// The colours that are used
	SortedSet<int> getUsedColours();

	// Ticks count of a position in the celocity interval table
	static int ticksCountFromXPos(int xPos) { return xPos * 16; }


public:
	// Key configuration
	TerpstraKeys	sets[NUMBEROFBOARDS];

	// General options
	bool afterTouchActive;
	bool lightOnKeyStrokes;
	bool invertFootController;
	int expressionControllerSensivity;

	// Velocity curves
	int velocityIntervalTableValues[VELOCITYINTERVALTABLESIZE];

};
