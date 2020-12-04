/*
  ==============================================================================

    KeyboardDataStructure.cpp
    Created: 1 Jul 2014 9:26:39pm
    Author:  hsstraub

  ==============================================================================
*/

#include "KeyboardDataStructure.h"

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

TerpstraKeyMapping::TerpstraKeyMapping()
{
	clearAll();
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
}

/*
==============================================================================
Read data 
==============================================================================
*/
void TerpstraKeyMapping::fromStringArray(const StringArray& stringArray)
{
	clearAll();

	// Buffe data in case stringArray is from an older 56-keys subset 
	TerpstraKey fiftysixthKeys[NUMBEROFBOARDS];
	int boardIndex;
	for (boardIndex = 0; boardIndex < NUMBEROFBOARDS; boardIndex++)
		fiftysixthKeys[boardIndex] = TerpstraKey();

	boardIndex = -1;
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
					if (keyIndex >= 0 && keyIndex < TERPSTRABOARDSIZE)
						sets[boardIndex].theKeys[keyIndex].noteNumber = keyValue;
					else if (keyIndex == 55)
						fiftysixthKeys[boardIndex].noteNumber = keyValue;
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
					if (keyIndex >= 0 && keyIndex < TERPSTRABOARDSIZE)
						sets[boardIndex].theKeys[keyIndex].channelNumber = keyValue;
					else if (keyIndex == 55)
						fiftysixthKeys[boardIndex].channelNumber = keyValue;
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
					if (keyIndex >= 0 && keyIndex < TERPSTRABOARDSIZE)
						sets[boardIndex].theKeys[keyIndex].colour = colValue;
					else if (keyIndex == 55)
						fiftysixthKeys[boardIndex].colour = colValue;
					else
						jassert(false);
				}
				else
					jassert(false);
			}
			else
				jassert(false);
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
					if (keyIndex >= 0 && keyIndex < TERPSTRABOARDSIZE)
						sets[boardIndex].theKeys[keyIndex].keyType = (TerpstraKey::KEYTYPE)keyValue;
					else if (keyIndex == 55)
						fiftysixthKeys[boardIndex].keyType = (TerpstraKey::KEYTYPE)keyValue;
					else
						jassert(false);
				}
				else
					jassert(false);
			}
			else
				jassert(false);
		}
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
		// ToDO expressionControllerSensivity

		// Velocity curve config
		// ToDo Velocity interval table
		// ToDo Note on/off velocity configuration
		// ToDo Fader configuration
		// ToDo Aftertouch configuration
	}

	// If it was a 56-key layout, convert to 55-key layout
	bool fromFiftySixKeys = false;
	for (boardIndex = 0; boardIndex < NUMBEROFBOARDS && !fromFiftySixKeys; boardIndex++)
		fromFiftySixKeys |= (!fiftysixthKeys[boardIndex].isEmpty());

	if (fromFiftySixKeys)
	{
		for (boardIndex = 0; boardIndex < NUMBEROFBOARDS; boardIndex++)
		{
			for (int keyIndex = 7; keyIndex < 54; keyIndex++)
				sets[boardIndex].theKeys[keyIndex] = sets[boardIndex].theKeys[keyIndex + 1];

			sets[boardIndex].theKeys[54] = fiftysixthKeys[boardIndex];
		}
	}
}

/*
==============================================================================
Write data
==============================================================================
*/
StringArray TerpstraKeyMapping::toStringArray()
{
	StringArray result;
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

	result.add("AfterTouchActive=" + String(afterTouchActive ? 1 : 0));
	result.add("LightOnKeyStrokes=" + String(lightOnKeyStrokes ? 1 : 0));
	result.add("InvertFootController=" + String(invertFootController ? 1 : 0));
	// ToDO expressionControllerSensivity

	// Velocity curve config
	// ToDo Velocity interval table
	// ToDo Note on/off velocity configuration
	// ToDo Fader configuration
	// ToDo Aftertouch configuration

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