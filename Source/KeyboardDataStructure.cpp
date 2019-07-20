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
}

/*
==============================================================================
Read data 
==============================================================================
*/
void TerpstraKeyMapping::fromStringArray(const StringArray& stringArray)
{
	clearAll();

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
				if (boardIndex >= 0 && boardIndex < NUMBEROFBOARDS && keyIndex >= 0 && keyIndex < TERPSTRABOARDSIZE)
				{
					sets[boardIndex].theKeys[keyIndex].noteNumber = keyValue;
					// XXX if keyValue is illegal: error status?
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
				if (boardIndex >= 0 && boardIndex < NUMBEROFBOARDS && keyIndex >= 0 && keyIndex < TERPSTRABOARDSIZE)
				{
					sets[boardIndex].theKeys[keyIndex].channelNumber = keyValue;
					// XXX if keyValue is illegal: error status?
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
				if (boardIndex >= 0 && boardIndex < NUMBEROFBOARDS && keyIndex >= 0 && keyIndex < TERPSTRABOARDSIZE)
				{
					sets[boardIndex].theKeys[keyIndex].colour = colValue;
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
				if (boardIndex >= 0 && boardIndex < NUMBEROFBOARDS && keyIndex >= 0 && keyIndex < TERPSTRABOARDSIZE)
				{
					sets[boardIndex].theKeys[keyIndex].keyType = (TerpstraKey::KEYTYPE)keyValue;
				}
				else
					jassert(false);
			}
			else
				jassert(false);
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