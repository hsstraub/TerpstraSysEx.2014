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

#include "PlayVirtualKeyboard.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PlayVirtualKeyboard::PlayVirtualKeyboard ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (lblInstructionText = new Label ("lblInstructionText",
                                                       TRANS("Clicking on the keys will send a note on MIDI command to the output channel.")));
    lblInstructionText->setFont (Font (15.00f, Font::plain));
    lblInstructionText->setJustificationType (Justification::centredLeft);
    lblInstructionText->setEditable (false, false, false);
    lblInstructionText->setColour (TextEditor::textColourId, Colours::black);
    lblInstructionText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (428, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

PlayVirtualKeyboard::~PlayVirtualKeyboard()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    lblInstructionText = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PlayVirtualKeyboard::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PlayVirtualKeyboard::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    lblInstructionText->setBounds (16, 16, 440, 72);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

// Called from MainComponent when one of the keys is clicked
void PlayVirtualKeyboard::performMouseDown(int setSelection, int keySelection)
{
	// ToDo
}

// Called from MainComponent when a previously clikeced key is released
void PlayVirtualKeyboard::performMouseUp(int setSelection, int keySelection)
{
	// ToDo
}


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PlayVirtualKeyboard" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="428" initialHeight="400">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <LABEL name="lblInstructionText" id="199c7576a5017f09" memberName="lblInstructionText"
         virtualName="" explicitFocusOrder="0" pos="16 16 440 72" edTextCol="ff000000"
         edBkgCol="0" labelText="Clicking on the keys will send a note on MIDI command to the output channel."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
