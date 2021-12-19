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
        bool setCCPolarity,
		LumatoneKeyType newKeyType,
		int newChannelNumber,
		int newNoteNumber, 
		TerpstraKey::COLOURTYPE newColour,
        bool newCCFaderIsDefault)
		: setSelection(setSelection), keySelection(keySelection)
		, setKeyType(setKeyType), setChannel(setChannel), setNote(setNote), setColour(setColour), setCCFaderPolarity(setCCPolarity)
		, newData(newKeyType, newChannelNumber, newNoteNumber, newColour, newCCFaderIsDefault)
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
			if (setKeyType || setChannel || setNote || setColour || setCCFaderPolarity)
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
                if (setCCFaderPolarity)
                {
                    mappingInEdit.sets[setSelection].theKeys[keySelection].ccFaderDefault = newData.ccFaderDefault;
                }

				// Send to device
				TerpstraSysExApplication::getApp().getLumatoneController()->sendKeyParam(
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
			if (setKeyType || setChannel || setNote || setColour || setCCFaderPolarity)
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
                if (setCCFaderPolarity)
                {
                    mappingInEdit.sets[setSelection].theKeys[keySelection].ccFaderDefault = previousData.ccFaderDefault;
                }


				// Send to device
				TerpstraSysExApplication::getApp().getLumatoneController()->sendKeyParam(
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
			TerpstraSysExApplication::getApp().getLumatoneController()->sendAllParamsOfBoard(setSelection + 1, mappingInEdit.sets[setSelection]);

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
			TerpstraSysExApplication::getApp().getLumatoneController()->sendAllParamsOfBoard(setSelection + 1, mappingInEdit.sets[setSelection]);

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
		TerpstraSysExApplication::getApp().getLumatoneController()->sendInvertFootController(newData);

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
		TerpstraSysExApplication::getApp().getLumatoneController()->sendInvertFootController(previousData);

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
		TerpstraSysExApplication::getApp().getLumatoneController()->sendExpressionPedalSensivity(newData);

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
		TerpstraSysExApplication::getApp().getLumatoneController()->sendExpressionPedalSensivity(previousData);

		// Notify that there are changes: in calling function
		return true;
	}

    // ==============================================================================
    // Implementation of InvertSustainEditAction

    InvertSustainEditAction::InvertSustainEditAction(bool newValue)
        : newData(newValue)
    {
        auto mainComponent = TerpstraSysExApplication::getApp().getMainContentComponent();
        jassert(mainComponent != nullptr);

        previousData = mainComponent->getMappingInEdit().invertSustain;
    }

    bool InvertSustainEditAction::perform()
    {
        auto mainComponent = TerpstraSysExApplication::getApp().getMainContentComponent();
        jassert(mainComponent != nullptr);
        TerpstraKeyMapping& mappingInEdit = mainComponent->getMappingInEdit();

        mappingInEdit.invertSustain = newData;

        // Send to device
        TerpstraSysExApplication::getApp().getLumatoneController()->invertSustainPedal(newData);

        // Notify that there are changes: in calling function
        return true;
    }

    bool InvertSustainEditAction::undo()
    {
        auto mainComponent = TerpstraSysExApplication::getApp().getMainContentComponent();
        jassert(mainComponent != nullptr);
        TerpstraKeyMapping& mappingInEdit = mainComponent->getMappingInEdit();

        mappingInEdit.invertSustain = previousData;

        // Send to device
        TerpstraSysExApplication::getApp().getLumatoneController()->invertSustainPedal(previousData);

        // Notify that there are changes: in calling function
        return true;
    }
}
