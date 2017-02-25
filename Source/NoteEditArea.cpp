/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "ViewConstants.h"
//[/Headers]

#include "NoteEditArea.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
NoteEditArea::NoteEditArea ()
{
    //[Constructor_pre] You can add your own custom stuff here..
	singleNoteAssign = new SingleNoteAssign();
	addAndMakeVisible(singleNoteAssign);
	singleNoteAssign->setVisible(false);

	isomorphicMassAssign = new IsomorphicMassAssign();
	addAndMakeVisible(isomorphicMassAssign);
	isomorphicMassAssign->setVisible(false);
    //[/Constructor_pre]

    addAndMakeVisible (cbEditMode = new ComboBox ("cbEditMode"));
    cbEditMode->setEditableText (false);
    cbEditMode->setJustificationType (Justification::centredLeft);
    cbEditMode->setTextWhenNothingSelected (String());
    cbEditMode->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbEditMode->addItem (TRANS("Assign notes to keys one by one"), 1);
    cbEditMode->addItem (TRANS("Isomorphic mass assign"), 2);
    cbEditMode->addItem (TRANS("Macro Buttons"), 3);
    cbEditMode->addListener (this);

    addAndMakeVisible (labelEditMode = new Label ("labelEditMode",
                                                  TRANS("Edit Function:")));
    labelEditMode->setFont (Font (15.00f, Font::plain));
    labelEditMode->setJustificationType (Justification::centredLeft);
    labelEditMode->setEditable (false, false, false);
    labelEditMode->setColour (TextEditor::textColourId, Colours::black);
    labelEditMode->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (428, 480);


    //[Constructor] You can add your own custom stuff here..

	// Default selection
	// Todo: read from user settings
	cbEditMode->setSelectedItemIndex(0, juce::NotificationType::sendNotification);

    //[/Constructor]
}

NoteEditArea::~NoteEditArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	singleNoteAssign = nullptr;
	isomorphicMassAssign = nullptr;
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
    //[/UserPaint]
}

void NoteEditArea::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	singleNoteAssign->setBounds(0, NOTEASSIGNSUBWINTOP, EDITSUBWINWIDTH, NOTEASSIGNSUBWINHEIGHT);
	isomorphicMassAssign->setBounds(0, NOTEASSIGNSUBWINTOP, EDITSUBWINWIDTH, NOTEASSIGNSUBWINHEIGHT);
    //[/UserPreResize]

    cbEditMode->setBounds (102, 15, 296, 24);
    labelEditMode->setBounds (6, 15, 88, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void NoteEditArea::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cbEditMode)
    {
        //[UserComboBoxCode_cbEditMode] -- add your combo box handling code here..
		int editMode = cbEditMode->getSelectedItemIndex();

		// Show sub window corresponding to selected edit mode
		switch (editMode)
		{
		case 0:
			singleNoteAssign->setVisible(true);
			isomorphicMassAssign->setVisible(false);
			break;
		case 1:
			singleNoteAssign->setVisible(false);
			isomorphicMassAssign->setVisible(true);
			break;
		default:
			singleNoteAssign->setVisible(false);
			isomorphicMassAssign->setVisible(false);
			break;
		}
        //[/UserComboBoxCode_cbEditMode]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

// Called from MainComponent when one of the keys is clicked
void NoteEditArea::PerformMouseClickEdit(int setSelection, int keySelection)
{
	jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS && keySelection >= 0 && keySelection < TERPSTRABOARDSIZE);

	int editMode = cbEditMode->getSelectedItemIndex();
	switch (editMode)
	{
	case 0:
		singleNoteAssign->PerformMouseClickEdit(setSelection, keySelection);
		break;
	case 1:
		isomorphicMassAssign->PerformMouseClickEdit(setSelection, keySelection);
		break;
	}
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
            virtualName="" explicitFocusOrder="0" pos="102 15 296 24" editable="0"
            layout="33" items="Assign notes to keys one by one&#10;Isomorphic mass assign&#10;Macro Buttons"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="labelEditMode" id="55d538af27203498" memberName="labelEditMode"
         virtualName="" explicitFocusOrder="0" pos="6 15 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Edit Function:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
