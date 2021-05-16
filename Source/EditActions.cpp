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

	// ==============================================================================
	// Implementation of SingleNoteAssignAction
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

	bool SingleNoteAssignAction::isValid() const
	{
		return setSelection >= 0 && setSelection < NUMBEROFBOARDS && keySelection >= 0 && keySelection < TerpstraSysExApplication::getApp().getOctaveBoardSize();
	}

	bool SingleNoteAssignAction::perform()
	{
		if (setSelection >= 0 && setSelection < NUMBEROFBOARDS && keySelection >= 0 && keySelection < TerpstraSysExApplication::getApp().getOctaveBoardSize())
		{
			if (setKeyType || setChannel || setNote || setColour)
			{
				auto mainComponent = TerpstraSysExApplication::getApp().getMainContentComponent();
				jassert(mainComponent != nullptr);
				TerpstraKeyMapping& mappingInEdit = mainComponent->getMappingInEdit();

				if (setKeyType)
				{
					mappingInEdit.sets[setSelection].theKeys[keySelection].keyType = newData.keyType;
				}
				if (setChannel)
				{
					mappingInEdit.sets[setSelection].theKeys[keySelection].channelNumber = newData.channelNumber;
				}
				if (setNote)
				{
					mappingInEdit.sets[setSelection].theKeys[keySelection].noteNumber = newData.noteNumber;
				}
				if (setColour)
				{
					mappingInEdit.sets[setSelection].theKeys[keySelection].colour = newData.colour;
				}

				// Send to device
				TerpstraSysExApplication::getApp().getMidiDriver().sendKeyParam(
					setSelection + 1, 
					keySelection, 
					mappingInEdit.sets[setSelection].theKeys[keySelection]);
				
				// Notfy that there are changes: in calling function
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
				TerpstraKeyMapping& mappingInEdit = mainComponent->getMappingInEdit();

				if (setKeyType)
				{
					mappingInEdit.sets[setSelection].theKeys[keySelection].keyType = previousData.keyType;
				}
				if (setChannel)
				{
					mappingInEdit.sets[setSelection].theKeys[keySelection].channelNumber = previousData.channelNumber;
				}
				if (setNote)
				{
					mappingInEdit.sets[setSelection].theKeys[keySelection].noteNumber = previousData.noteNumber;
				}
				if (setColour)
				{
					mappingInEdit.sets[setSelection].theKeys[keySelection].colour = previousData.colour;
				}

				// Send to device
				TerpstraSysExApplication::getApp().getMidiDriver().sendKeyParam(
					setSelection + 1,
					keySelection,
					mappingInEdit.sets[setSelection].theKeys[keySelection]);

				// Notify that there are changes: in calling function
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


	// ==============================================================================
	// Implementation of SectionEditAction

	SectionEditAction::SectionEditAction(int setSelection, TerpstraKeys& newSectionValue)
		: setSelection(setSelection)
		, newData(newSectionValue)
	{
		auto mainComponent = TerpstraSysExApplication::getApp().getMainContentComponent();
		jassert(mainComponent != nullptr);

		previousData = mainComponent->getMappingInEdit().sets[setSelection];
	}

	bool SectionEditAction::isValid() const
	{
		return setSelection >= 0 && setSelection < NUMBEROFBOARDS;
	}

	bool SectionEditAction::perform()
	{
		if (setSelection >= 0 && setSelection < NUMBEROFBOARDS)
		{
			auto mainComponent = TerpstraSysExApplication::getApp().getMainContentComponent();
			jassert(mainComponent != nullptr);
			TerpstraKeyMapping& mappingInEdit = mainComponent->getMappingInEdit();

			mappingInEdit.sets[setSelection] = newData;

			// Send to device
			TerpstraSysExApplication::getApp().getMidiDriver().sendAllParamsOfBoard(setSelection + 1, mappingInEdit.sets[setSelection]);

			// Notify that there are changes: in calling function
			return true;
		}
		else
		{
			jassertfalse;
			return false;
		}
	}

	bool SectionEditAction::undo()
	{
		if (setSelection >= 0 && setSelection < NUMBEROFBOARDS)
		{
			auto mainComponent = TerpstraSysExApplication::getApp().getMainContentComponent();
			jassert(mainComponent != nullptr);
			TerpstraKeyMapping& mappingInEdit = mainComponent->getMappingInEdit();

			mappingInEdit.sets[setSelection] = previousData;

			// Send to device
			TerpstraSysExApplication::getApp().getMidiDriver().sendAllParamsOfBoard(setSelection + 1, mappingInEdit.sets[setSelection]);

			// Notify that there are changes: in calling function
			return true;
		}
		else
		{
			jassertfalse;
			return false;
		}

	}

}