/*
  ==============================================================================

    KeyboardDataStructure.cpp
    Created: 1 Jul 2014 9:26:39pm
    Author:  hsstraub

  ==============================================================================
*/

#include "KeyboardDataStructure.h"

/*
==============================================================================
TerpstraKeys class
==============================================================================
*/

TerpstraKeys::TerpstraKeys()
{
	for (int i = 0; i < TERPSTRABOARDSIZE; i++)
		theKeys[i] = TerpstraKey();
	board_idx = 0;
	key_idx = 0;

}

bool TerpstraKeys::isEmpty() const
{
	TerpstraKey emptyKeyData = TerpstraKey();

	bool setIsEmpty = true;
	for (int i = 0; i < TERPSTRABOARDSIZE && setIsEmpty; i++)
	{
		if (theKeys[i] != emptyKeyData)
		{
			setIsEmpty = false;
			break;
		}
	}

	return setIsEmpty;
}

/*
==============================================================================
TerpstraVelocityCurveConfig class
==============================================================================
*/

TerpstraVelocityCurveConfig::TerpstraVelocityCurveConfig()
{
	editStrategy = EDITSTRATEGYINDEX::none;

	// Default config: one to one
	for (int x = 0; x < 128; x++)
		velocityValues[x] = x;
}

TerpstraVelocityCurveConfig::TerpstraVelocityCurveConfig(const String& velocityCurveConfigString)
{
	if (velocityCurveConfigString.startsWith("LINEAR"))
	{
		editStrategy = TerpstraVelocityCurveConfig::EDITSTRATEGYINDEX::linearSegments;

		StringArray velocityCurveValueArray = StringArray::fromTokens(velocityCurveConfigString.substring(6), false);
		if (velocityCurveValueArray.size() > 0)
		{
			jassert(velocityCurveValueArray.size() >= 128);

			for (int x = 0; x < 128; x++)
				velocityValues[x] = velocityCurveValueArray[x].getIntValue();
		}
		else
		{
			// Initialize segment table
			for (int x = 0; x < 128; x++)
				velocityValues[x] = -1;
		}
	}
	else if (velocityCurveConfigString.startsWith("Quadratic"))
	{
		editStrategy = TerpstraVelocityCurveConfig::EDITSTRATEGYINDEX::quadraticCurves;

		StringArray velocityCurveValueArray = StringArray::fromTokens(velocityCurveConfigString.substring(6), false);
		if (velocityCurveValueArray.size() > 0)
		{
			jassert(velocityCurveValueArray.size() >= 128);

			for (int x = 0; x < 128; x++)
				velocityValues[x] = velocityCurveValueArray[x].getIntValue();
		}
		else
		{
			// Initialize segment table
			for (int x = 0; x < 128; x++)
				velocityValues[x] = -1;
		}
	}
	else
	{
		editStrategy = TerpstraVelocityCurveConfig::EDITSTRATEGYINDEX::freeDrawing;

		StringArray velocityCurveValueArray = StringArray::fromTokens(velocityCurveConfigString, false);
		if (velocityCurveValueArray.size() > 0)
		{
			jassert(velocityCurveValueArray.size() >= 128);

			for (int x = 0; x < 128; x++)
				velocityValues[x] = velocityCurveValueArray[x].getIntValue();
		}
		else
		{
			// Initialize velocity lookup table
			for (int x = 0; x < 128; x++)
				velocityValues[x] = x;
		}
	}
}

String TerpstraVelocityCurveConfig::createConfigStringForSaving()
{
	switch (editStrategy)
	{
	case EDITSTRATEGYINDEX::freeDrawing:
	{
		String velocityCurveString;

		for (int x = 0; x < 128; x++)
			velocityCurveString += String(velocityValues[x]) + " ";

		return velocityCurveString;
	}

	case EDITSTRATEGYINDEX::linearSegments:
	{
		String velocityCurveString = "LINEAR";

		for (int x = 0; x < 128; x++)
			velocityCurveString += String(velocityValues[x]) + " ";

		return velocityCurveString;

	}

	case EDITSTRATEGYINDEX::quadraticCurves:
	{
		String velocityCurveString = "Quadratic";

		for (int x = 0; x < 128; x++)
			velocityCurveString += String(velocityValues[x]) + " ";

		return velocityCurveString;
	}

	default:
		return "";
	}
}

/*
==============================================================================
TerpstraKeyMapping class
==============================================================================
*/

TerpstraKeyMapping::TerpstraKeyMapping()
{
	clearAll();
}

void TerpstraKeyMapping::clearVelocityIntervalTable()
{
	// Default interval table: equal division
	for (int i = 0; i < VELOCITYINTERVALTABLESIZE; i++)
	{
		velocityIntervalTableValues[i] = ticksCountFromXPos(i + 1);
	}
}

void TerpstraKeyMapping::clearAll()
{
	for (int i = 0; i < NUMBEROFBOARDS; i++)
		sets[i] = TerpstraKeys();

	// Default values for options
	afterTouchActive = false;
	lightOnKeyStrokes = false;
    invertExpression = false;
    invertSustain = false;
	expressionControllerSensivity = 0;

	clearVelocityIntervalTable();
	noteOnOffVelocityCurveConfig = TerpstraVelocityCurveConfig();
	faderConfig = TerpstraVelocityCurveConfig();
	afterTouchConfig = TerpstraVelocityCurveConfig();
}

void TerpstraKeyMapping::fromStringArray(const StringArray& stringArray)
{
	clearAll();

	bool hasFiftySixKeys = false;
	int boardIndex = -1;
	for (int i = 0; i < stringArray.size(); i++) {
		String currentLine = stringArray[i];
		int pos1, pos2;
		if ((pos1 = currentLine.indexOf("[Board")) >= 0) {
			pos2 = currentLine.indexOf("]");
			if (pos2 >= 0 && pos2>pos1) {
				boardIndex = currentLine.substring(pos1 + 6, pos2).getIntValue();
			} else
				jassert(false);
		} else if ((pos1 = currentLine.indexOf("Key_")) >= 0) {
			pos2 = currentLine.indexOf("=");
			if (pos2 >= 0 && pos2 > pos1) {
				int keyIndex = currentLine.substring(pos1 + 4, pos2).getIntValue();
				int keyValue = currentLine.substring(pos2 + 1).getIntValue();
				if (boardIndex >= 0 && boardIndex < NUMBEROFBOARDS) {
					if (keyIndex >= 0 && keyIndex < TERPSTRABOARDSIZE)
						sets[boardIndex].theKeys[keyIndex].noteNumber = keyValue;
					else
						jassert(false);
				} else
					jassert(false);
			}
		} else if ((pos1 = currentLine.indexOf("Chan_")) >= 0) {
			pos2 = currentLine.indexOf("=");
			if (pos2 >= 0 && pos2 > pos1) {
				int keyIndex = currentLine.substring(pos1 + 5, pos2).getIntValue();
				int keyValue = currentLine.substring(pos2 + 1).getIntValue();
				if (boardIndex >= 0 && boardIndex < NUMBEROFBOARDS) {
					if (keyIndex >= 0 && keyIndex < TERPSTRABOARDSIZE) {
						sets[boardIndex].theKeys[keyIndex].channelNumber = keyValue;

						if ( keyIndex == 55) 
                            hasFiftySixKeys = true;
                    } else
						jassert(false);
				} else
					jassert(false);
			}
		} else if ((pos1 = currentLine.indexOf("Col_")) >= 0) {
			pos2 = currentLine.indexOf("=");
			if (pos2 >= 0 && pos2 > pos1) {
				int keyIndex = currentLine.substring(pos1 + 4, pos2).getIntValue();
				int colValue = currentLine.substring(pos2 + 1).getHexValue32();
				if (boardIndex >= 0 && boardIndex < NUMBEROFBOARDS) {
					if (keyIndex >= 0 && keyIndex < TERPSTRABOARDSIZE)
						sets[boardIndex].theKeys[keyIndex].colour = colValue;
					else
						jassert(false);
				} else
					jassert(false);
			}
		} else if ((pos1 = currentLine.indexOf("KTyp_")) >= 0) {
			pos2 = currentLine.indexOf("=");
			if (pos2 >= 0 && pos2 > pos1) {
				int keyIndex = currentLine.substring(pos1 + 5, pos2).getIntValue();
				int keyValue = currentLine.substring(pos2 + 1).getIntValue();
				if (boardIndex >= 0 && boardIndex < NUMBEROFBOARDS) {
					if (keyIndex >= 0 && keyIndex < TERPSTRABOARDSIZE)
						sets[boardIndex].theKeys[keyIndex].keyType = (TerpstraKey::KEYTYPE)keyValue;
					else
					jassert(false);
				}
			} else
				jassert(false);
		}
		// General options
		else if ((pos1 = currentLine.indexOf("AfterTouchActive=")) >= 0) {
			afterTouchActive = currentLine.substring(pos1 + 17).getIntValue() > 0;
		} else if ((pos1 = currentLine.indexOf("LightOnKeyStrokes=")) >= 0) {
			lightOnKeyStrokes = currentLine.substring(pos1 + 18).getIntValue() > 0;
        } else if ((pos1 = currentLine.indexOf("InvertFootController=")) >= 0) {
            invertExpression = currentLine.substring(pos1 + 21).getIntValue() > 0;
		} else if ((pos1 = currentLine.indexOf("InvertSustain=")) >= 0) {
			invertSustain = currentLine.substring(pos1 + 14).getIntValue() > 0;
		} else if ((pos1 = currentLine.indexOf("ExprCtrlSensivity=")) >= 0) {
			expressionControllerSensivity = currentLine.substring(pos1 + 18).getIntValue();
		}
		// Velocity curve config
		else if ((pos1 = currentLine.indexOf("VelocityIntrvlTbl=")) >= 0) {
			String intervalTableString = currentLine.substring(pos1 + 18);
			StringArray intervalTableValueArray = StringArray::fromTokens(intervalTableString, false);
			if (intervalTableValueArray.size() > 0) {
				jassert(intervalTableValueArray.size() >= VELOCITYINTERVALTABLESIZE);

				for (int x = 0; x < VELOCITYINTERVALTABLESIZE; x++) {
					velocityIntervalTableValues[x] = intervalTableValueArray[x].getIntValue();
				}
			} else {
				clearVelocityIntervalTable();
			}
		}
		// Note on/off velocity configuration
		else if ((pos1 = currentLine.indexOf("NoteOnOffVelocityCrvTbl=")) >= 0)
		{
			noteOnOffVelocityCurveConfig = TerpstraVelocityCurveConfig(currentLine.substring(pos1 + 24));
		}
		// Fader configuration
		else if ((pos1 = currentLine.indexOf("FaderConfig=")) >= 0)
		{
			faderConfig = TerpstraVelocityCurveConfig(currentLine.substring(pos1 + 12));
		}
		// Aftertouch configuration
		else if ((pos1 = currentLine.indexOf("afterTouchConfig=")) >= 0)
		{
			afterTouchConfig = TerpstraVelocityCurveConfig(currentLine.substring(pos1 + 17));
		}
	}

	// if it was a 55-key layout, convert to new 56-key layout
	// ToDo Also convert from old 56-key layout? For this we will have to know the version
	if (!hasFiftySixKeys)
	{
		for (boardIndex = 0; boardIndex < NUMBEROFBOARDS; boardIndex++)
		{
			sets[boardIndex].theKeys[55] = sets[boardIndex].theKeys[54];
			sets[boardIndex].theKeys[54] = TerpstraKey();
		}
	}
}

StringArray TerpstraKeyMapping::toStringArray()
{
	StringArray result;

	// ToDO Write version (for detection of old 56-key layout - those will be layouts with 56 keys but without version)
	for (int boardIndex = 0; boardIndex < NUMBEROFBOARDS; boardIndex++)
	{
		result.add("[Board" + String(boardIndex) + "]");

		for (int keyIndex = 0; keyIndex < TERPSTRABOARDSIZE; keyIndex++)
		{
			result.add("Key_" + String(keyIndex) + "=" + String(sets[boardIndex].theKeys[keyIndex].noteNumber));
			result.add("Chan_" + String(keyIndex) + "=" + String(sets[boardIndex].theKeys[keyIndex].channelNumber));
			if (sets[boardIndex].theKeys[keyIndex].colour != 0)
				result.add("Col_" + String(keyIndex) + "=" + String::toHexString((sets[boardIndex].theKeys[keyIndex].colour)));
			if (sets[boardIndex].theKeys[keyIndex].keyType != TerpstraKey::noteOnNoteOff)
				result.add("KTyp_" + String(keyIndex) + "=" + String(sets[boardIndex].theKeys[keyIndex].keyType));
		}
	}

	// General options
	result.add("AfterTouchActive=" + String(afterTouchActive ? 1 : 0));
	result.add("LightOnKeyStrokes=" + String(lightOnKeyStrokes ? 1 : 0));
    result.add("InvertFootController=" + String(invertExpression ? 1 : 0));
    result.add("InvertSustain=" + String(invertSustain ? 1 : 0));
	result.add("ExprCtrlSensivity=" + String(expressionControllerSensivity));

	// Velocity curve interval table
	String intervalTableString;
	for (auto intervalTableValue : velocityIntervalTableValues)
		intervalTableString += String(intervalTableValue) + " ";
	result.add("VelocityIntrvlTbl=" + intervalTableString);

	// Note on/off velocity configuration
	result.add("NoteOnOffVelocityCrvTbl=" + noteOnOffVelocityCurveConfig.createConfigStringForSaving());
	// Fader configuration
	result.add("FaderConfig=" + faderConfig.createConfigStringForSaving());
	// Aftertouch configuration
	result.add("afterTouchConfig=" + afterTouchConfig.createConfigStringForSaving());

	return result;
}

SortedSet<int> TerpstraKeyMapping::getUsedColours()
{
	SortedSet<int> result;

	for (int boardIndex = 0; boardIndex < NUMBEROFBOARDS; boardIndex++)
	{
		for (int keyIndex = 0; keyIndex < TERPSTRABOARDSIZE; keyIndex++)
		{
			result.add(sets[boardIndex].theKeys[keyIndex].colour);
		}
	}

	return result;
}
