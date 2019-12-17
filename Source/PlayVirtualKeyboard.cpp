/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "Main.h"
#include "MainComponent.h"
//[/Headers]

#include "PlayVirtualKeyboard.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PlayVirtualKeyboard::PlayVirtualKeyboard ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    textInstructionText.reset (new TextEditor ("textInstructionText"));
    addAndMakeVisible (textInstructionText.get());
    textInstructionText->setMultiLine (true);
    textInstructionText->setReturnKeyStartsNewLine (false);
    textInstructionText->setReadOnly (true);
    textInstructionText->setScrollbarsShown (false);
    textInstructionText->setCaretVisible (false);
    textInstructionText->setPopupMenuEnabled (false);
    textInstructionText->setText (TRANS("Click on the keys to send note on/off MIDI commands to the output channel."));

    textInstructionText->setBounds (8, 16, 360, 88);


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

    textInstructionText = nullptr;


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
	g.fillAll(findColour(ResizableWindow::backgroundColourId));

	// Instruction text is coded as TextEditor (for multiline display) but should look like a label
	textInstructionText->setColour(TextEditor::backgroundColourId, findColour(Label::backgroundColourId));
    //[/UserPaint]
}

void PlayVirtualKeyboard::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

/// <summary>Called from MainComponent when one of the keys is clicked</summary>
/// <returns>Mapping was changed yes/no</returns>
bool PlayVirtualKeyboard::performMouseDown(int setSelection, int keySelection)
{
	jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS && keySelection >= 0 && keySelection < TERPSTRABOARDSIZE);

	TerpstraKey& keyData = ((MainContentComponent*)(getParentComponent()->getParentComponent()))->getMappingInEdit().sets[setSelection].theKeys[keySelection];

	if (keyData.channelNumber > 0)
	{
		if (keyData.keyType == TerpstraKey::noteOnNoteOff)
		{
			// Send "note on" event
			TerpstraSysExApplication::getApp().getMidiDriver().sendNoteOnMessage(keyData.noteNumber, keyData.channelNumber, 60);
		}
		// ToDo if keyType is "continuous controller": send controller event?
	}

	return false;
}

/// <summary>Called from MainComponent when a previously clicked key is released</summary>
/// <returns>Mapping was changed yes/no</returns>
bool PlayVirtualKeyboard::performMouseUp(int setSelection, int keySelection)
{
	jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS && keySelection >= 0 && keySelection < TERPSTRABOARDSIZE);

	TerpstraKey& keyData = ((MainContentComponent*)(getParentComponent()->getParentComponent()))->getMappingInEdit().sets[setSelection].theKeys[keySelection];

	if (keyData.channelNumber > 0)
	{
		if (keyData.keyType == TerpstraKey::noteOnNoteOff)
		{
			// Send "note off" event
			TerpstraSysExApplication::getApp().getMidiDriver().sendNoteOffMessage(keyData.noteNumber, keyData.channelNumber, 60);
		}
	}
	return false;
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
  <TEXTEDITOR name="textInstructionText" id="ab84427977a3ee81" memberName="textInstructionText"
              virtualName="" explicitFocusOrder="0" pos="8 16 360 88" initialText="Click on the keys to send note on/off MIDI commands to the output channel."
              multiline="1" retKeyStartsLine="0" readonly="1" scrollbars="0"
              caret="0" popupmenu="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

