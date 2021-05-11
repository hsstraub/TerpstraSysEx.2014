/*
  ==============================================================================

    EditActions.cpp
    Created: 9 May 2021 9:49:31pm
    Author:  hsstraub

  ==============================================================================
*/

#include "EditActions.h"
#include "Main.h"

namespace Lumatone {

	SingleNoteAssignAction::SingleNoteAssignAction(
		int setSelection,
		int keySelection,
		bool setKeyType,
		bool setChannel, 
		bool setNote,
		bool setColour,
		TerpstraKey::KEYTYPE newKeyType,
		int newChannelNumber,
		int newNoteNumber, 
		TerpstraKey::COLOURTYPE newColour)
		: setSelection(setSelection), keySelection(keySelection)
		, setKeyType(setKeyType), setChannel(setChannel), setNote(setNote), setColour(setColour)
		, newData(newKeyType, newChannelNumber, newNoteNumber, newColour)
	{
		auto mainComponent = TerpstraSysExApplication::getApp().getMainContentComponent();
		jassert(mainComponent != nullptr);

		previousData = mainComponent->getMappingInEdit().sets[setSelection].theKeys[keySelection];
	}

	bool SingleNoteAssignAction::perform()
	{
		if (setSelection >= 0 && setSelection < NUMBEROFBOARDS && keySelection >= 0 && keySelection < TerpstraSysExApplication::getApp().getOctaveBoardSize())
		{
			if (setKeyType || setChannel || setNote || setColour)
			{
				auto mainComponent = TerpstraSysExApplication::getApp().getMainContentComponent();
				jassert(mainComponent != nullptr);

				if (setKeyType)
				{
					mainComponent->getMappingInEdit().sets[setSelection].theKeys[keySelection].keyType = newData.keyType;
				}
				if (setChannel)
				{
					mainComponent->getMappingInEdit().sets[setSelection].theKeys[keySelection].channelNumber = newData.channelNumber;
				}
				if (setNote)
				{
					mainComponent->getMappingInEdit().sets[setSelection].theKeys[keySelection].noteNumber = newData.noteNumber;
				}
				if (setColour)
				{
					mainComponent->getMappingInEdit().sets[setSelection].theKeys[keySelection].colour = newData.colour;
				}
			}
			else
			{
				// Nothing to do
				jassertfalse;
			}

			return true;
		}
		else
		{
			jassertfalse;
			return false;
		}

	}

	bool SingleNoteAssignAction::undo()
	{
		if (setSelection >= 0 && setSelection < NUMBEROFBOARDS && keySelection >= 0 && keySelection < TerpstraSysExApplication::getApp().getOctaveBoardSize())
		{
			if (setKeyType || setChannel || setNote || setColour)
			{
				auto mainComponent = TerpstraSysExApplication::getApp().getMainContentComponent();
				jassert(mainComponent != nullptr);

				if (setKeyType)
				{
					mainComponent->getMappingInEdit().sets[setSelection].theKeys[keySelection].keyType = previousData.keyType;
				}
				if (setChannel)
				{
					mainComponent->getMappingInEdit().sets[setSelection].theKeys[keySelection].channelNumber = previousData.channelNumber;
				}
				if (setNote)
				{
					mainComponent->getMappingInEdit().sets[setSelection].theKeys[keySelection].noteNumber = previousData.noteNumber;
				}
				if (setColour)
				{
					mainComponent->getMappingInEdit().sets[setSelection].theKeys[keySelection].colour = previousData.colour;
				}
			}
			else
			{
				// Nothing to do
				jassertfalse;
			}

			return true;
		}
		else
		{
			jassertfalse;
			return false;
		}
	}

}