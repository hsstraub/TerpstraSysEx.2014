/*
==============================================================================

KeyboardDataStructure.h
Created: XXX
Author:  hsstraub

==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LumatoneFirmwareDefinitions.h"

// Mapping of one key
class TerpstraKey
{
public:
	typedef juce::Colour COLOURTYPE;

public:
    TerpstraKey(LumatoneKeyType newKeyType = LumatoneKeyType::disabled) { noteNumber = 0; channelNumber = 1; colour = juce::Colour(); keyType = newKeyType; ccFaderDefault = true; };
	TerpstraKey(LumatoneKeyType newKeyType, int newChannelNumber, int newNoteNumber, COLOURTYPE newColour, bool invertCCFader = false)
	{
        keyType = newKeyType; channelNumber = newChannelNumber; noteNumber = newNoteNumber; colour = newColour; ccFaderDefault = invertCCFader;
	}
	bool isEmpty() const { return channelNumber == 0; }

    bool operator!=(const TerpstraKey& second) const { return noteNumber != second.noteNumber || channelNumber != second.channelNumber || colour != second.colour || keyType != second.keyType || ccFaderDefault != second.ccFaderDefault; }
    
    // Chainable setters
    TerpstraKey withKeyType(LumatoneKeyType type) const { return TerpstraKey(type, channelNumber, noteNumber, colour, ccFaderDefault); }
    TerpstraKey withColour(Colour newColour) const { return TerpstraKey(keyType, channelNumber, noteNumber, newColour, ccFaderDefault); }
    TerpstraKey withNoteOrCC(int noteOrCC) const { return TerpstraKey(keyType, channelNumber, noteOrCC, colour, ccFaderDefault); }
    TerpstraKey withChannelNumber(int channel) const { return TerpstraKey(keyType, channel, noteNumber, colour, ccFaderDefault); }
    TerpstraKey withInvertCCFader(bool invertCCFader) const { return TerpstraKey(keyType, channelNumber, noteNumber, colour, invertCCFader); }

public:
	int			noteNumber;
	int			channelNumber;
	COLOURTYPE	colour;
    bool        ccFaderDefault;
	LumatoneKeyType		keyType;
};

// Subset of 56 (or 55) keys
struct TerpstraKeys {
	TerpstraKey		theKeys[56];
	int				board_idx;
	int				key_idx;

	TerpstraKeys(LumatoneKeyType newKeyType = LumatoneKeyType::disabled);

	bool isEmpty() const;
};

// Velocity curve config
class TerpstraVelocityCurveConfig
{
public:

	typedef enum
	{
		none = -1,
		freeDrawing = 0,
		linearSegments = 1,
		quadraticCurves = 2
	} EDITSTRATEGYINDEX;

	typedef enum
	{
		noteOnNoteOff = 1,
		fader = 2,
		afterTouch = 3,
		lumaTouch = 4
		// ToDo modulation wheel
	} VelocityCurveType;

public:
	TerpstraVelocityCurveConfig();
	// Default value for the given curve type
	TerpstraVelocityCurveConfig(VelocityCurveType velocityCurveType);
	TerpstraVelocityCurveConfig(const String& velocityCurveConfigString);

	String createConfigStringForSaving();

public:
	EDITSTRATEGYINDEX editStrategy;

	// Velocity curve values. Meaning varies depending on editStrategy
	int velocityValues[128];
};

/*
==============================================================================
A complete key mapping
==============================================================================
*/
#define NUMBEROFBOARDS 5

// Number of rows by which each octave board is offset
#define BOARDROWOFFSET 2

// Number of entries in the velocity interval table
#define VELOCITYINTERVALTABLESIZE 127

class TerpstraKeyMapping
{
public:
	TerpstraKeyMapping();

	void clearVelocityIntervalTable();
	void clearAll(bool initializeWithNoteKeyType=false);
    bool isEmpty() const;

	void fromStringArray(const StringArray& stringArray);
	StringArray toStringArray();

	// The colours that are used
	//SortedSet<TerpstraKey::COLOURTYPE> getUsedColours();

	TerpstraVelocityCurveConfig* getVelocityCurveConfig(TerpstraVelocityCurveConfig::VelocityCurveType velocityCurveType);

public:
	// Key configuration
	TerpstraKeys	sets[NUMBEROFBOARDS];

	// General options
	bool afterTouchActive;
	bool lightOnKeyStrokes;
	bool invertExpression;
	bool invertSustain;
	int expressionControllerSensivity;

	// Velocity curves
	int velocityIntervalTableValues[VELOCITYINTERVALTABLESIZE];

	TerpstraVelocityCurveConfig	noteOnOffVelocityCurveConfig;
	TerpstraVelocityCurveConfig faderConfig;
	TerpstraVelocityCurveConfig afterTouchConfig;
	TerpstraVelocityCurveConfig lumaTouchConfig;
};
