/*
  ==============================================================================

    KeyboardDataStructure.cpp
    Created: 1 Jul 2014 9:26:39pm
    Author:  hsstraub

  ==============================================================================
*/

#include "KeyboardDataStructure.h"
#include "Main.h"

/*
==============================================================================
TerpstraKeys class
==============================================================================
*/

TerpstraKeys::TerpstraKeys()
{
	for (int i = 0; i < 56; i++)
		theKeys[i] = TerpstraKey();
	board_idx = 0;
	key_idx = 0;

}

bool TerpstraKeys::isEmpty() const
{
	TerpstraKey emptyKeyData = TerpstraKey();

	bool setIsEmpty = true;
	for (int i = 0; i < TerpstraSysExApplication::getApp().getOctaveBoardSize() && setIsEmpty; i++)
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

int DefaultFaderVelocityTable[128] = {
	1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 12, 12, 12, 13, 13, 14, 14, 14, 15, 15, 16, 16,
	17, 17, 17, 18, 18, 19, 19, 20, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 31, 31, 32, 32, 33, 33,
	34, 35, 35, 36, 37, 37, 38, 39, 39, 40, 41, 41, 42, 43, 44, 45, 45, 46, 47, 48, 49, 50, 51, 52, 53, 55, 56, 57, 59, 62, 65, 68, 71, 74, 77,
	79, 82, 85, 88, 91, 94, 97, 99, 102, 105, 108, 111, 114, 117, 119, 122, 125, 127 };

TerpstraVelocityCurveConfig::TerpstraVelocityCurveConfig()
{
	// Default config: one to one
	for (int x = 0; x < 128; x++)
		velocityValues[x] = x;

	editStrategy = EDITSTRATEGYINDEX::none;
}

TerpstraVelocityCurveConfig::TerpstraVelocityCurveConfig(TerpstraVelocityCurveConfig::VelocityCurveType velocityCurveType)
{
	switch (velocityCurveType)
	{
	case TerpstraVelocityCurveConfig::VelocityCurveType::fader:
		jassert(sizeof(DefaultFaderVelocityTable) == sizeof(velocityValues));
		memmove(velocityValues, DefaultFaderVelocityTable, sizeof(DefaultFaderVelocityTable));
		editStrategy = EDITSTRATEGYINDEX::freeDrawing;
		break;

	case TerpstraVelocityCurveConfig::VelocityCurveType::afterTouch:
		// ToDo

	case TerpstraVelocityCurveConfig::VelocityCurveType::noteOnNoteOff:
	default:
	{
		// Default config: one to one
		for (int x = 0; x < 128; x++)
			velocityValues[x] = x;

		editStrategy = EDITSTRATEGYINDEX::none;
	}
	break;
	}
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
	invertFootController = false;
	expressionControllerSensivity = 0;

	clearVelocityIntervalTable();
	noteOnOffVelocityCurveConfig = TerpstraVelocityCurveConfig(TerpstraVelocityCurveConfig::VelocityCurveType::noteOnNoteOff);
	faderConfig = TerpstraVelocityCurveConfig(TerpstraVelocityCurveConfig::VelocityCurveType::fader);
	afterTouchConfig = TerpstraVelocityCurveConfig(TerpstraVelocityCurveConfig::VelocityCurveType::afterTouch);
}

void TerpstraKeyMapping::fromStringArray(const StringArray& stringArray)
{
	clearAll();

	// Buffer data in case stringArray is from an older 56-keys subset 
	int boardIndex = -1;
	for (int i = 0; i < stringArray.size(); i++)
	{
		String currentLine = stringArray[i];
		int pos1, pos2;
		if ((pos1 = currentLine.indexOf("[Board")) >= 0)
		{
			pos2 = currentLine.indexOf("]");
			if (pos2 >= 0 && pos2>pos1)
			{
				boardIndex = currentLine.substring(pos1 + 6, pos2).getIntValue();
			}
			else
				jassert(false);
		}
		else if ((pos1 = currentLine.indexOf("Key_")) >= 0)
		{
			pos2 = currentLine.indexOf("=");
			if (pos2 >= 0 && pos2 > pos1)
			{
				int keyIndex = currentLine.substring(pos1 + 4, pos2).getIntValue();
				int keyValue = currentLine.substring(pos2 + 1).getIntValue();
				if (boardIndex >= 0 && boardIndex < NUMBEROFBOARDS)
				{
					if (keyIndex >= 0 && keyIndex < 56)
						sets[boardIndex].theKeys[keyIndex].noteNumber = keyValue;
					else
						jassert(false);
				}
				else
					jassert(false);
			}
		}
		else if ((pos1 = currentLine.indexOf("Chan_")) >= 0)
		{
			pos2 = currentLine.indexOf("=");
			if (pos2 >= 0 && pos2 > pos1)
			{
				int keyIndex = currentLine.substring(pos1 + 5, pos2).getIntValue();
				int keyValue = currentLine.substring(pos2 + 1).getIntValue();
				if (boardIndex >= 0 && boardIndex < NUMBEROFBOARDS)
				{
					if (keyIndex >= 0 && keyIndex < 56)
						sets[boardIndex].theKeys[keyIndex].channelNumber = keyValue;
					else
						jassert(false);
				}
				else
					jassert(false);
			}
		}
		else if ((pos1 = currentLine.indexOf("Col_")) >= 0)
		{
			pos2 = currentLine.indexOf("=");
			if (pos2 >= 0 && pos2 > pos1)
			{
				int keyIndex = currentLine.substring(pos1 + 4, pos2).getIntValue();
				int colValue = currentLine.substring(pos2 + 1).getHexValue32();
				if (boardIndex >= 0 && boardIndex < NUMBEROFBOARDS)
				{
					if (keyIndex >= 0 && keyIndex < 56)
						sets[boardIndex].theKeys[keyIndex].colour = colValue;
					else
						jassert(false);
				}
				else
					jassert(false);
			}
		}
		else if ((pos1 = currentLine.indexOf("KTyp_")) >= 0)
		{
			pos2 = currentLine.indexOf("=");
			if (pos2 >= 0 && pos2 > pos1)
			{
				int keyIndex = currentLine.substring(pos1 + 5, pos2).getIntValue();
				int keyValue = currentLine.substring(pos2 + 1).getIntValue();
				if (boardIndex >= 0 && boardIndex < NUMBEROFBOARDS)
				{
					if (keyIndex >= 0 && keyIndex < 56)
						sets[boardIndex].theKeys[keyIndex].keyType = (TerpstraKey::KEYTYPE)keyValue;
					else
						jassert(false);
				}
			}
			else
				jassert(false);
		}
		// General options
		else if ((pos1 = currentLine.indexOf("AfterTouchActive=")) >= 0)
		{
			afterTouchActive = currentLine.substring(pos1 + 17).getIntValue() > 0;
		}
		else if ((pos1 = currentLine.indexOf("LightOnKeyStrokes=")) >= 0)
		{
			lightOnKeyStrokes = currentLine.substring(pos1 + 18).getIntValue() > 0;
		}
		else if ((pos1 = currentLine.indexOf("InvertFootController=")) >= 0)
		{
			invertFootController = currentLine.substring(pos1 + 21).getIntValue() > 0;
		}
		else if ((pos1 = currentLine.indexOf("ExprCtrlSensivity=")) >= 0)
		{
			expressionControllerSensivity = currentLine.substring(pos1 + 18).getIntValue();
		}
		// Velocity curve config
		else if ((pos1 = currentLine.indexOf("VelocityIntrvlTbl=")) >= 0)
		{
			String intervalTableString = currentLine.substring(pos1 + 18);
			StringArray intervalTableValueArray = StringArray::fromTokens(intervalTableString, false);
			if (intervalTableValueArray.size() > 0)
			{
				jassert(intervalTableValueArray.size() >= VELOCITYINTERVALTABLESIZE);

				for (int x = 0; x < VELOCITYINTERVALTABLESIZE; x++)
				{
					velocityIntervalTableValues[x] = intervalTableValueArray[x].getIntValue();
				}
			}
			else
			{
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

	// Conversion between 55-key and 56-key layout
	// ToDo ?
}

StringArray TerpstraKeyMapping::toStringArray()
{
	StringArray result;

	// ToDO Write version (for detection of old 56-key layout - those will be layouts with 56 keys but without version)
	for (int boardIndex = 0; boardIndex < NUMBEROFBOARDS; boardIndex++)
	{
		result.add("[Board" + String(boardIndex) + "]");

		for (int keyIndex = 0; keyIndex < TerpstraSysExApplication::getApp().getOctaveBoardSize(); keyIndex++)
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
	result.add("InvertFootController=" + String(invertFootController ? 1 : 0));
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
		for (int keyIndex = 0; keyIndex < 56; keyIndex++)
		{
			result.add(sets[boardIndex].theKeys[keyIndex].colour);
		}
	}

	return result;
}
