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
//[/Headers]

#include "IsomorphicMassAssign.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
IsomorphicMassAssign::IsomorphicMassAssign ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (startingPointeBox = new ComboBox ("startingPointBox"));
    startingPointeBox->setEditableText (false);
    startingPointeBox->setJustificationType (Justification::centredLeft);
    startingPointeBox->setTextWhenNothingSelected (String());
    startingPointeBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    startingPointeBox->addListener (this);

    addAndMakeVisible (labelStartingPoint = new Label ("labelStartingPoint",
                                                       TRANS("Starting value")));
    labelStartingPoint->setTooltip (TRANS("Value that will be assigned to the key at mouse pposition when clicking"));
    labelStartingPoint->setFont (Font (15.00f, Font::plain));
    labelStartingPoint->setJustificationType (Justification::centredLeft);
    labelStartingPoint->setEditable (false, false, false);
    labelStartingPoint->setColour (TextEditor::textColourId, Colours::black);
    labelStartingPoint->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (labelHorizontalSteps = new Label ("labelHorizontalSteps",
                                                         TRANS("Horizontal steps")));
    labelHorizontalSteps->setFont (Font (15.00f, Font::plain));
    labelHorizontalSteps->setJustificationType (Justification::centredLeft);
    labelHorizontalSteps->setEditable (false, false, false);
    labelHorizontalSteps->setColour (TextEditor::textColourId, Colours::black);
    labelHorizontalSteps->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (editHorizontalSteps = new TextEditor ("editHorizontalSteps"));
    editHorizontalSteps->setMultiLine (false);
    editHorizontalSteps->setReturnKeyStartsNewLine (false);
    editHorizontalSteps->setReadOnly (false);
    editHorizontalSteps->setScrollbarsShown (true);
    editHorizontalSteps->setCaretVisible (true);
    editHorizontalSteps->setPopupMenuEnabled (true);
    editHorizontalSteps->setText (String());

    addAndMakeVisible (labelLeftUpwardsSteps = new Label ("labelLeftUpwardsSteps",
                                                          TRANS("Left upwards steps")));
    labelLeftUpwardsSteps->setFont (Font (15.00f, Font::plain));
    labelLeftUpwardsSteps->setJustificationType (Justification::centredLeft);
    labelLeftUpwardsSteps->setEditable (false, false, false);
    labelLeftUpwardsSteps->setColour (TextEditor::textColourId, Colours::black);
    labelLeftUpwardsSteps->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (editLeftUpwardsSteps = new TextEditor ("editLeftUpwardsSteps"));
    editLeftUpwardsSteps->setMultiLine (false);
    editLeftUpwardsSteps->setReturnKeyStartsNewLine (false);
    editLeftUpwardsSteps->setReadOnly (false);
    editLeftUpwardsSteps->setScrollbarsShown (true);
    editLeftUpwardsSteps->setCaretVisible (true);
    editLeftUpwardsSteps->setPopupMenuEnabled (true);
    editLeftUpwardsSteps->setText (String());

    addAndMakeVisible (editInstructionText = new Label ("editInstructionText",
                                                        TRANS("Fill a line or the whole field with constant step distances.")));
    editInstructionText->setFont (Font (15.00f, Font::plain));
    editInstructionText->setJustificationType (Justification::topLeft);
    editInstructionText->setEditable (false, false, false);
    editInstructionText->setColour (TextEditor::textColourId, Colours::black);
    editInstructionText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (428, 220);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

IsomorphicMassAssign::~IsomorphicMassAssign()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    startingPointeBox = nullptr;
    labelStartingPoint = nullptr;
    labelHorizontalSteps = nullptr;
    editHorizontalSteps = nullptr;
    labelLeftUpwardsSteps = nullptr;
    editLeftUpwardsSteps = nullptr;
    editInstructionText = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void IsomorphicMassAssign::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    g.setColour (Colour (0xff010e0c));
    g.fillPath (internalPath1);

    g.setColour (Colour (0xff02020e));
    g.fillPath (internalPath2);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void IsomorphicMassAssign::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    startingPointeBox->setBounds (24, 128, 150, 24);
    labelStartingPoint->setBounds (16, 104, 150, 24);
    labelHorizontalSteps->setBounds (224, 104, 150, 24);
    editHorizontalSteps->setBounds (232, 128, 40, 24);
    labelLeftUpwardsSteps->setBounds (168, 40, 150, 24);
    editLeftUpwardsSteps->setBounds (176, 64, 39, 24);
    editInstructionText->setBounds (8, 8, 416, 24);
    internalPath1.clear();
    internalPath1.startNewSubPath (188.0f, 132.0f);
    internalPath1.lineTo (220.0f, 140.0f);
    internalPath1.lineTo (188.0f, 148.0f);
    internalPath1.closeSubPath();

    internalPath2.clear();
    internalPath2.startNewSubPath (164.0f, 84.0f);
    internalPath2.lineTo (140.0f, 116.0f);
    internalPath2.lineTo (132.0f, 100.0f);
    internalPath2.closeSubPath();

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void IsomorphicMassAssign::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == startingPointeBox)
    {
        //[UserComboBoxCode_startingPointeBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_startingPointeBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
// Implementation of MappingLogicListener
void IsomorphicMassAssign::mappingLogicChanged(MappingLogicBase* mappingLogicThatChanged)
{
	// XXX
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="IsomorphicMassAssign" componentName=""
                 parentClasses="public Component, public MappingLogicListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="428"
                 initialHeight="220">
  <BACKGROUND backgroundColour="ffbad0de">
    <PATH pos="0 0 100 100" fill="solid: ff010e0c" hasStroke="0" nonZeroWinding="1">s 188 132 l 220 140 l 188 148 x</PATH>
    <PATH pos="0 0 100 100" fill="solid: ff02020e" hasStroke="0" nonZeroWinding="1">s 164 84 l 140 116 l 132 100 x</PATH>
  </BACKGROUND>
  <COMBOBOX name="startingPointBox" id="d526f69bdc196fea" memberName="startingPointeBox"
            virtualName="" explicitFocusOrder="0" pos="24 128 150 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="labelStartingPoint" id="5401a3246c13771e" memberName="labelStartingPoint"
         virtualName="" explicitFocusOrder="0" pos="16 104 150 24" tooltip="Value that will be assigned to the key at mouse pposition when clicking"
         edTextCol="ff000000" edBkgCol="0" labelText="Starting value"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="labelHorizontalSteps" id="3e6663aecf1474c8" memberName="labelHorizontalSteps"
         virtualName="" explicitFocusOrder="0" pos="224 104 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Horizontal steps" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="editHorizontalSteps" id="8d2f5f07f337b9ef" memberName="editHorizontalSteps"
              virtualName="" explicitFocusOrder="0" pos="232 128 40 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="labelLeftUpwardsSteps" id="43530804741d9cb7" memberName="labelLeftUpwardsSteps"
         virtualName="" explicitFocusOrder="0" pos="168 40 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Left upwards steps" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="editLeftUpwardsSteps" id="3a1cf8588366e0ca" memberName="editLeftUpwardsSteps"
              virtualName="" explicitFocusOrder="0" pos="176 64 39 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="editInstructionText" id="c03ef432c2b4599" memberName="editInstructionText"
         virtualName="" explicitFocusOrder="0" pos="8 8 416 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Fill a line or the whole field with constant step distances."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="9"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
