/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "ViewConstants.h"
#include "Main.h"
//[/Headers]

#include "NoteEditArea.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
NoteEditArea::NoteEditArea ()
{
    //[Constructor_pre] You can add your own custom stuff here..
	singleNoteAssign.reset(new SingleNoteAssign());
	addAndMakeVisible(singleNoteAssign.get());
	singleNoteAssign->setVisible(false);

	isomorphicMassAssign.reset(new IsomorphicMassAssign());
	addAndMakeVisible(isomorphicMassAssign.get());
	isomorphicMassAssign->setVisible(false);

	playVirtualKeyboardWindow.reset(new PlayVirtualKeyboard());
	addAndMakeVisible(playVirtualKeyboardWindow.get());
	playVirtualKeyboardWindow->setVisible(false);
    //[/Constructor_pre]

    cbEditMode.reset (new ComboBox ("cbEditMode"));
    addAndMakeVisible (cbEditMode.get());
    cbEditMode->setEditableText (false);
    cbEditMode->setJustificationType (Justification::centredLeft);
    cbEditMode->setTextWhenNothingSelected (String());
    cbEditMode->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbEditMode->addItem (TRANS("Assign notes to keys one by one"), 1);
    cbEditMode->addItem (TRANS("Isomorphic mass assign"), 2);
    cbEditMode->addItem (TRANS("Play virtual keyboard"), 3);
    cbEditMode->addListener (this);

    cbEditMode->setBounds (104, 16, 304, 24);

    labelEditMode.reset (new Label ("labelEditMode",
                                    TRANS("Edit Function:")));
    addAndMakeVisible (labelEditMode.get());
    labelEditMode->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelEditMode->setJustificationType (Justification::centredLeft);
    labelEditMode->setEditable (false, false, false);
    labelEditMode->setColour (TextEditor::textColourId, Colours::black);
    labelEditMode->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelEditMode->setBounds (6, 15, 88, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (428, 480);


    //[Constructor] You can add your own custom stuff here..

	// Default selection
	// Todo: read from user settings
	cbEditMode->setSelectedItemIndex(noteEditMode::SingleNoteAssignMode, juce::NotificationType::sendNotification);

    //[/Constructor]
}

NoteEditArea::~NoteEditArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	singleNoteAssign = nullptr;
	isomorphicMassAssign = nullptr;
	playVirtualKeyboardWindow = nullptr;
    //[/Destructor_pre]

    cbEditMode = nullptr;
    labelEditMode = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void NoteEditArea::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(findColour(ResizableWindow::backgroundColourId));
    //[/UserPaint]
}

void NoteEditArea::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	singleNoteAssign->setBounds(0, NOTEASSIGNSUBWINTOP, EDITAREAWIDTH, NOTEASSIGNSUBWINHEIGHT);
	isomorphicMassAssign->setBounds(0, NOTEASSIGNSUBWINTOP, EDITAREAWIDTH, NOTEASSIGNSUBWINHEIGHT);
	playVirtualKeyboardWindow->setBounds(0, NOTEASSIGNSUBWINTOP, EDITAREAWIDTH, NOTEASSIGNSUBWINHEIGHT);
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void NoteEditArea::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cbEditMode.get())
    {
        //[UserComboBoxCode_cbEditMode] -- add your combo box handling code here..
		int editMode = cbEditMode->getSelectedItemIndex();

		// Show sub window corresponding to selected edit mode
		switch (editMode)
		{
		case noteEditMode::SingleNoteAssignMode:
			singleNoteAssign->setVisible(true);
			isomorphicMassAssign->setVisible(false);
			playVirtualKeyboardWindow->setVisible(false);
			break;
		case noteEditMode::IsomorphicMassAssignMode:
			singleNoteAssign->setVisible(false);
			isomorphicMassAssign->setVisible(true);
			playVirtualKeyboardWindow->setVisible(false);
			break;
		case noteEditMode::PlayVirtualKeaboardMode:
			singleNoteAssign->setVisible(false);
			isomorphicMassAssign->setVisible(false);
			playVirtualKeyboardWindow->setVisible(true);
			break;
		default:
			singleNoteAssign->setVisible(false);
			isomorphicMassAssign->setVisible(false);
			playVirtualKeyboardWindow->setVisible(false);
			break;
		}
        //[/UserComboBoxCode_cbEditMode]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void NoteEditArea::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	singleNoteAssign->restoreStateFromPropertiesFile(propertiesFile);
	isomorphicMassAssign->restoreStateFromPropertiesFile(propertiesFile);
}

void NoteEditArea::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	singleNoteAssign->saveStateToPropertiesFile(propertiesFile);
	isomorphicMassAssign->saveStateToPropertiesFile(propertiesFile);
}

/// <summary>Called from MainComponent when one of the keys is clicked</summary>
/// <returns>Mapping was changed yes/no</returns>
bool NoteEditArea::performMouseDown(int setSelection, int keySelection)
{
	jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS && keySelection >= 0 && keySelection < TerpstraSysExApplication::getApp().getOctaveBoardSize());

	int editMode = cbEditMode->getSelectedItemIndex();
	switch (editMode)
	{
	case noteEditMode::SingleNoteAssignMode:
		return singleNoteAssign->performMouseDown(setSelection, keySelection);
	case noteEditMode::IsomorphicMassAssignMode:
		return isomorphicMassAssign->performMouseDown(setSelection, keySelection);
	case noteEditMode::PlayVirtualKeaboardMode:
		return playVirtualKeyboardWindow->performMouseDown(setSelection, keySelection);
	default:
		return false;
	}
}

/// <summary>Called from MainComponent when a previously clicked key is released</summary>
/// <returns>Mapping was changed yes/no</returns>
bool NoteEditArea::performMouseUp(int setSelection, int keySelection)
{
	jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS && keySelection >= 0 && keySelection < TerpstraSysExApplication::getApp().getOctaveBoardSize());

	int editMode = cbEditMode->getSelectedItemIndex();

	// Mouse up functionality: only for playing on virtual keyboard
	if (editMode == noteEditMode::PlayVirtualKeaboardMode)
		return playVirtualKeyboardWindow->performMouseUp(setSelection, keySelection);

	return false;
}

void NoteEditArea::onSetData(TerpstraKeyMapping& newData)
{
	// Add colours of the mapping to the colour combo box
	singleNoteAssign->onSetData(newData);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="NoteEditArea" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="428" initialHeight="480">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <COMBOBOX name="cbEditMode" id="1f22301dd42b968e" memberName="cbEditMode"
            virtualName="" explicitFocusOrder="0" pos="104 16 304 24" editable="0"
            layout="33" items="Assign notes to keys one by one&#10;Isomorphic mass assign&#10;Play virtual keyboard"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="labelEditMode" id="55d538af27203498" memberName="labelEditMode"
         virtualName="" explicitFocusOrder="0" pos="6 15 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Edit Function:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

