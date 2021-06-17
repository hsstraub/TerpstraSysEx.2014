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
		LumatoneKeyType newKeyType,
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
				TerpstraSysExApplication::getApp().getLumatoneController().sendKeyParam(
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
				TerpstraSysExApplication::getApp().getLumatoneController().sendKeyParam(
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
			TerpstraSysExApplication::getApp().getLumatoneController().sendAllParamsOfBoard(setSelection + 1, mappingInEdit.sets[setSelection]);

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
			TerpstraSysExApplication::getApp().getLumatoneController().sendAllParamsOfBoard(setSelection + 1, mappingInEdit.sets[setSelection]);

			// Notify that there are changes: in calling function
			return true;
		}
		else
		{
			jassertfalse;
			return false;
		}

	}

	// ==============================================================================
	// Implementation of InvertFootControllerEditAction

	InvertFootControllerEditAction::InvertFootControllerEditAction(bool newValue)
		: newData(newValue)
	{
		auto mainComponent = TerpstraSysExApplication::getApp().getMainContentComponent();
		jassert(mainComponent != nullptr);

		previousData = mainComponent->getMappingInEdit().invertExpression;
	}

	bool InvertFootControllerEditAction::perform()
	{
		auto mainComponent = TerpstraSysExApplication::getApp().getMainContentComponent();
		jassert(mainComponent != nullptr);
		TerpstraKeyMapping& mappingInEdit = mainComponent->getMappingInEdit();

		mappingInEdit.invertExpression = newData;

		// Send to device
		TerpstraSysExApplication::getApp().getLumatoneController().sendInvertFootController(newData);

		// Notify that there are changes: in calling function
		return true;
	}

	bool InvertFootControllerEditAction::undo()
	{
		auto mainComponent = TerpstraSysExApplication::getApp().getMainContentComponent();
		jassert(mainComponent != nullptr);
		TerpstraKeyMapping& mappingInEdit = mainComponent->getMappingInEdit();

		mappingInEdit.invertExpression = previousData;

		// Send to device
		TerpstraSysExApplication::getApp().getLumatoneController().sendInvertFootController(previousData);

		// Notify that there are changes: in calling function
		return true;
	}

	// ==============================================================================
	// Implementation of ExprPedalSensivityEditAction

	ExprPedalSensivityEditAction::ExprPedalSensivityEditAction(int newValue)
		: newData(newValue)
	{
		auto mainComponent = TerpstraSysExApplication::getApp().getMainContentComponent();
		jassert(mainComponent != nullptr);

		previousData = mainComponent->getMappingInEdit().expressionControllerSensivity;
	}

	bool ExprPedalSensivityEditAction::perform()
	{
		auto mainComponent = TerpstraSysExApplication::getApp().getMainContentComponent();
		jassert(mainComponent != nullptr);
		TerpstraKeyMapping& mappingInEdit = mainComponent->getMappingInEdit();

		mappingInEdit.expressionControllerSensivity = newData;

		// Send to device
		TerpstraSysExApplication::getApp().getLumatoneController().sendExpressionPedalSensivity(newData);

		// Notify that there are changes: in calling function
		return true;
	}

	bool ExprPedalSensivityEditAction::undo()
	{
		auto mainComponent = TerpstraSysExApplication::getApp().getMainContentComponent();
		jassert(mainComponent != nullptr);
		TerpstraKeyMapping& mappingInEdit = mainComponent->getMappingInEdit();

		mappingInEdit.expressionControllerSensivity = previousData;

		// Send to device
		TerpstraSysExApplication::getApp().getLumatoneController().sendExpressionPedalSensivity(previousData);

		// Notify that there are changes: in calling function
		return true;
	}

}
