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

TerpstraKeyMapping::TerpstraKeyMapping()
{
	clearAll();
}

void TerpstraKeyMapping::clearAll()
{
	for (int i = 0; i < NUMBEROFBOARDS; i++)
		sets[i] = TerpstraKeys();
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
	for (int boardIndex = 0; boardIndex < NUMBEROFBOARDS; boardIndex++)
		fiftysixthKeys[boardIndex] = TerpstraKey();

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
	}

	// If it was a 56-key layout, convert to 55-key layout
	bool fromFiftySixKeys = false;
	for (int boardIndex = 0; boardIndex < NUMBEROFBOARDS && !fromFiftySixKeys; boardIndex++)
		fromFiftySixKeys |= (!fiftysixthKeys[boardIndex].isEmpty());

	if (fromFiftySixKeys)
	{
		for (int boardIndex = 0; boardIndex < NUMBEROFBOARDS; boardIndex++)
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