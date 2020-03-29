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
//[/Headers]

#include "KBMForOneChannel.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
KBMForOneChannel::KBMForOneChannel ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    channelBox.reset (new ComboBox ("channelBox"));
    addAndMakeVisible (channelBox.get());
    channelBox->setEditableText (false);
    channelBox->setJustificationType (Justification::centredLeft);
    channelBox->setTextWhenNothingSelected (String());
    channelBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    channelBox->addListener (this);

    channelBox->setBounds (0, 8, 48, 24);

    textMappingFile.reset (new TextEditor ("textMappingFile"));
    addAndMakeVisible (textMappingFile.get());
    textMappingFile->setMultiLine (false);
    textMappingFile->setReturnKeyStartsNewLine (false);
    textMappingFile->setReadOnly (false);
    textMappingFile->setScrollbarsShown (true);
    textMappingFile->setCaretVisible (true);
    textMappingFile->setPopupMenuEnabled (true);
    textMappingFile->setText (String());

    textMappingFile->setBounds (56, 8, 104, 24);

    btnFileSelectMacro.reset (new TextButton ("btnFileSelectMacro"));
    addAndMakeVisible (btnFileSelectMacro.get());
    btnFileSelectMacro->setButtonText (TRANS("..."));
    btnFileSelectMacro->addListener (this);

    btnFileSelectMacro->setBounds (168, 8, 32, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (200, 32);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

KBMForOneChannel::~KBMForOneChannel()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    channelBox = nullptr;
    textMappingFile = nullptr;
    btnFileSelectMacro = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void KBMForOneChannel::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(findColour(ResizableWindow::backgroundColourId));
    //[/UserPaint]
}

void KBMForOneChannel::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    for (int i = 1; i <= 16; i++)
        channelBox->addItem(String(i), i);
    //[/UserResized]
}

void KBMForOneChannel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == channelBox.get())
    {
        //[UserComboBoxCode_channelBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_channelBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void KBMForOneChannel::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnFileSelectMacro.get())
    {
        //[UserButtonCode_btnFileSelectMacro] -- add your button handler code here..
		FileChooser chooser("Open a Scala KBM mapping", File(), "*.kbm");
		if (chooser.browseForFileToOpen())
		{
			currentFile = chooser.getResult();
			updateTextEditorFromFileObject();
		}
        //[/UserButtonCode_btnFileSelectMacro]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void KBMForOneChannel::textEditorFocusLost(TextEditor& textEdit)
{
	if (&textEdit == textMappingFile.get())
	{
		currentFile = File(textMappingFile->getText());
		updateTooltipFromFileObject();
	}
}

// Set tooltip to full path of file, or "File not found"
void KBMForOneChannel::updateTooltipFromFileObject()
{
	if (currentFile.existsAsFile())
	{
		textMappingFile->setTooltip(currentFile.getFullPathName());
	}
	else
	{
		textMappingFile->setTooltip("File not found");
	}
}

// Set text editor text to file name and tooltip to full path (or "File not found")
void KBMForOneChannel::updateTextEditorFromFileObject()
{
	textMappingFile->setText(currentFile.getFileName());
	updateTooltipFromFileObject();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="KBMForOneChannel" componentName=""
                 parentClasses="public Component, public TextEditor::Listener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="200"
                 initialHeight="32">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <COMBOBOX name="channelBox" id="250a1dde474111c4" memberName="channelBox"
            virtualName="" explicitFocusOrder="0" pos="0 8 48 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTEDITOR name="textMappingFile" id="b9bc5baf677ed902" memberName="textMappingFile"
              virtualName="" explicitFocusOrder="0" pos="56 8 104 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="btnFileSelectMacro" id="23cc77cbad6653d7" memberName="btnFileSelectMacro"
              virtualName="" explicitFocusOrder="0" pos="168 8 32 24" buttonText="..."
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

