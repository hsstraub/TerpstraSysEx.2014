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
	incrMidiNotesMapping = new IncrMidiNotesMapping();
	addAndMakeVisible(incrMidiNotesMapping);
	incrMidiNotesMapping->setVisible(false);

	singleNoteAssign = new SingleNoteAssign();
	addAndMakeVisible(singleNoteAssign);
	singleNoteAssign->setVisible(false);

	isomorphicMassAssign = new IsomorphicMassAssign();
	addAndMakeVisible(isomorphicMassAssign);
	isomorphicMassAssign->setVisible(false);

    //[/Constructor_pre]

    addAndMakeVisible (cbMappingStyle = new ComboBox ("cbMappingStyle"));
    cbMappingStyle->setEditableText (false);
    cbMappingStyle->setJustificationType (Justification::centredLeft);
    cbMappingStyle->setTextWhenNothingSelected (String());
    cbMappingStyle->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbMappingStyle->addItem (TRANS("MIDI notes, increasing order"), 1);
    cbMappingStyle->addItem (TRANS("MidiIntegrator mapping"), 2);
    cbMappingStyle->addListener (this);

    addAndMakeVisible (labelMappingStyle = new Label ("labelMappingStyle",
                                                      TRANS("Style:")));
    labelMappingStyle->setFont (Font (15.00f, Font::plain));
    labelMappingStyle->setJustificationType (Justification::centredLeft);
    labelMappingStyle->setEditable (false, false, false);
    labelMappingStyle->setColour (TextEditor::textColourId, Colours::black);
    labelMappingStyle->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (cbEditMode = new ComboBox ("cbEditMode"));
    cbEditMode->setEditableText (false);
    cbEditMode->setJustificationType (Justification::centredLeft);
    cbEditMode->setTextWhenNothingSelected (String());
    cbEditMode->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbEditMode->addItem (TRANS("Assign notes to keys one by one"), 1);
    cbEditMode->addItem (TRANS("Isomorphic mass assign"), 2);
    cbEditMode->addListener (this);

    addAndMakeVisible (labelEditMode = new Label ("labelEditMode",
                                                  TRANS("Edit Mode:")));
    labelEditMode->setFont (Font (15.00f, Font::plain));
    labelEditMode->setJustificationType (Justification::centredLeft);
    labelEditMode->setEditable (false, false, false);
    labelEditMode->setColour (TextEditor::textColourId, Colours::black);
    labelEditMode->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (groupMapping = new GroupComponent ("groupMapping",
                                                          TRANS("Mapping")));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (428, 480);


    //[Constructor] You can add your own custom stuff here..

	// MappingLogic listener
	incrMidiNotesMapping->getMappingLogic()->addListener(singleNoteAssign);
	incrMidiNotesMapping->getMappingLogic()->addListener(isomorphicMassAssign);

	// Default selection
	// Todo: read from user settings
	cbMappingStyle->setSelectedItemIndex(0, juce::NotificationType::sendNotification);

    //[/Constructor]
}

NoteEditArea::~NoteEditArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	incrMidiNotesMapping = nullptr;
    //[/Destructor_pre]

    cbMappingStyle = nullptr;
    labelMappingStyle = nullptr;
    cbEditMode = nullptr;
    labelEditMode = nullptr;
    groupMapping = nullptr;


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
	incrMidiNotesMapping->setBounds(16, MAPPINGSUBWINTOP, EDITSUBWINWIDTH, MAPPINGSUBWINHEIGHT);

	singleNoteAssign->setBounds(16, NOTEASSIGNSUBWINTOP, EDITSUBWINWIDTH, NOTEASSIGNSUBWINHEIGHT);
	isomorphicMassAssign->setBounds(16, NOTEASSIGNSUBWINTOP, EDITSUBWINWIDTH, NOTEASSIGNSUBWINHEIGHT);
    //[/UserPreResize]

    cbMappingStyle->setBounds (112, 24, 296, 24);
    labelMappingStyle->setBounds (16, 24, 88, 24);
    cbEditMode->setBounds (112, 176, 296, 24);
    labelEditMode->setBounds (16, 176, 88, 24);
    groupMapping->setBounds (8, 8, 416, 152);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void NoteEditArea::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cbMappingStyle)
    {
        //[UserComboBoxCode_cbMappingStyle] -- add your combo box handling code here..
		int mappingStyle = cbMappingStyle->getSelectedItemIndex();

		// Show sub window corresponding to selected mapping style. Currently there is only one
		if (mappingStyle == 0)
			incrMidiNotesMapping->setVisible(true);
		else
			incrMidiNotesMapping->setVisible(false);

        //[/UserComboBoxCode_cbMappingStyle]
    }
    else if (comboBoxThatHasChanged == cbEditMode)
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
		// XXX

        //[/UserComboBoxCode_cbEditMode]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void NoteEditArea::PerformMouseClickEdit(TerpstraKeyMapping& mappingData, int setSelection, int keySelection, TerpstraMidiDriver& midiDriver)
{
	jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS && keySelection >= 0 && keySelection < TERPSTRABOARDSIZE);

	int editMode = cbEditMode->getSelectedItemIndex();
	switch (editMode)
	{
	case 0:
		singleNoteAssign->PerformMouseClickEdit(mappingData, setSelection, keySelection, midiDriver);
		break;
	case 1:
		isomorphicMassAssign->PerformMouseClickEdit(mappingData, setSelection, keySelection, midiDriver);
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
  <COMBOBOX name="cbMappingStyle" id="a7825b65cfb78392" memberName="cbMappingStyle"
            virtualName="" explicitFocusOrder="0" pos="112 24 296 24" editable="0"
            layout="33" items="MIDI notes, increasing order&#10;MidiIntegrator mapping"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="labelMappingStyle" id="d77d8a4b80130afc" memberName="labelMappingStyle"
         virtualName="" explicitFocusOrder="0" pos="16 24 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Style:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <COMBOBOX name="cbEditMode" id="1f22301dd42b968e" memberName="cbEditMode"
            virtualName="" explicitFocusOrder="0" pos="112 176 296 24" editable="0"
            layout="33" items="Assign notes to keys one by one&#10;Isomorphic mass assign"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="labelEditMode" id="55d538af27203498" memberName="labelEditMode"
         virtualName="" explicitFocusOrder="0" pos="16 176 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Edit Mode:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <GROUPCOMPONENT name="groupMapping" id="fbb69100a7b12118" memberName="groupMapping"
                  virtualName="" explicitFocusOrder="0" pos="8 8 416 152" title="Mapping"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
