/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "ColourEditComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ColourEditComponent::ColourEditComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (btnColourPicker = new TextButton ("btnColourPicker"));
    btnColourPicker->setButtonText (TRANS("Colour picker"));
    btnColourPicker->addListener (this);

    addAndMakeVisible (colourCombo = new ColourComboBox ("colourCombo"));
    colourCombo->setEditableText (true);
    colourCombo->setJustificationType (Justification::centredLeft);
    colourCombo->setTextWhenNothingSelected (String());
    colourCombo->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    colourCombo->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (196, 32);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ColourEditComponent::~ColourEditComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    btnColourPicker = nullptr;
    colourCombo = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ColourEditComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffb8d0de));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ColourEditComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    btnColourPicker->setBounds (88, 0, 104, 24);
    colourCombo->setBounds (0, 0, 79, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ColourEditComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnColourPicker)
    {
        //[UserButtonCode_btnColourPicker] -- add your button handler code here..
        //[/UserButtonCode_btnColourPicker]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void ColourEditComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == colourCombo)
    {
        //[UserComboBoxCode_colourCombo] -- add your combo box handling code here..
        //[/UserComboBoxCode_colourCombo]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ColourEditComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="196" initialHeight="32">
  <BACKGROUND backgroundColour="ffb8d0de"/>
  <TEXTBUTTON name="btnColourPicker" id="fb8f62a75c5cd9ec" memberName="btnColourPicker"
              virtualName="" explicitFocusOrder="0" pos="88 0 104 24" buttonText="Colour picker"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="colourCombo" id="86628debb1bafc04" memberName="colourCombo"
            virtualName="ColourComboBox" explicitFocusOrder="0" pos="0 0 79 24"
            editable="1" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
