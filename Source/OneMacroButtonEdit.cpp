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
#include "KeyboardDataStructure.h"
#include "Main.h"
//[/Headers]

#include "OneMacroButtonEdit.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
OneMacroButtonEdit::OneMacroButtonEdit ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (lblButton = new Label ("lblButton",
                                              TRANS("1")));
    lblButton->setFont (Font (15.00f, Font::plain));
    lblButton->setJustificationType (Justification::centredLeft);
    lblButton->setEditable (false, false, false);
    lblButton->setColour (TextEditor::textColourId, Colours::black);
    lblButton->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (textMacroFile = new TextEditor ("textMacroFile"));
    textMacroFile->setMultiLine (false);
    textMacroFile->setReturnKeyStartsNewLine (false);
    textMacroFile->setReadOnly (false);
    textMacroFile->setScrollbarsShown (true);
    textMacroFile->setCaretVisible (true);
    textMacroFile->setPopupMenuEnabled (true);
    textMacroFile->setText (String());

    addAndMakeVisible (btnFileSelectMacro = new TextButton ("btnFileSelectMacro"));
    btnFileSelectMacro->setButtonText (TRANS("..."));
    btnFileSelectMacro->addListener (this);


    //[UserPreSize]
	textMacroFile->addListener(this);
    //[/UserPreSize]

    setSize (200, 32);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

OneMacroButtonEdit::~OneMacroButtonEdit()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    lblButton = nullptr;
    textMacroFile = nullptr;
    btnFileSelectMacro = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OneMacroButtonEdit::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OneMacroButtonEdit::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    lblButton->setBounds (8, 8, 24, 24);
    textMacroFile->setBounds (32, 8, 136, 24);
    btnFileSelectMacro->setBounds (176, 8, 32, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OneMacroButtonEdit::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnFileSelectMacro)
    {
        //[UserButtonCode_btnFileSelectMacro] -- add your button handler code here..
		FileChooser chooser("Open a Terpstra SysEx mapping", File::nonexistent, "*.tsx");
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

void OneMacroButtonEdit::setMacroButtonNumber(int value)
{
	macroButtonNumber = value;
	lblButton->setText(String(macroButtonNumber+1), NotificationType::dontSendNotification);
}

void OneMacroButtonEdit::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	String keyName = "MacroButton" + String(macroButtonNumber);
	String keyValue = propertiesFile->getValue(keyName);
	currentFile = File(keyValue);
	updateTextEditorFromFileObject();
}

void OneMacroButtonEdit::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	String keyName = "MacroButton" + String(macroButtonNumber);
	String keyValue = currentFile.getFullPathName();

	propertiesFile->setValue(keyName, keyValue);
}

void OneMacroButtonEdit::textEditorFocusLost(TextEditor& textEdit)
{
	if (&textEdit == textMacroFile)
	{
		currentFile = File(textMacroFile->getText());
		updateTooltipFromFileObject();
	}
}

void OneMacroButtonEdit::sendParametrizationFileToDevice()
{
	if (currentFile.existsAsFile())
	{
		StringArray stringArray;
		currentFile.readLines(stringArray);
		TerpstraKeyMapping keyMapping;
		keyMapping.fromStringArray(stringArray);

		TerpstraSysExApplication::getApp().getMidiDriver().sendCompleteMapping(keyMapping, saveAfterSending);
	}
}

// Set tooltip to full path of file, or "File not found"
void OneMacroButtonEdit::updateTooltipFromFileObject()
{
	if (currentFile.existsAsFile())
	{
		textMacroFile->setTooltip(currentFile.getFullPathName());
	}
	else
	{
		textMacroFile->setTooltip("File not found");
	}
}

// Set text editor text to file name and tooltip to full path (or "File not found")
void OneMacroButtonEdit::updateTextEditorFromFileObject()
{
	textMacroFile->setText(currentFile.getFileName());
	updateTooltipFromFileObject();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OneMacroButtonEdit" componentName=""
                 parentClasses="public Component, public TextEditor::Listener" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="200" initialHeight="32">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <LABEL name="lblButton" id="20c692b045b913d5" memberName="lblButton"
         virtualName="" explicitFocusOrder="0" pos="8 8 24 24" edTextCol="ff000000"
         edBkgCol="0" labelText="1" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="textMacroFile" id="b9bc5baf677ed902" memberName="textMacroFile"
              virtualName="" explicitFocusOrder="0" pos="32 8 136 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="btnFileSelectMacro" id="23cc77cbad6653d7" memberName="btnFileSelectMacro"
              virtualName="" explicitFocusOrder="0" pos="176 8 32 24" buttonText="..."
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
