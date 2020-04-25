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
//[/Headers]

#include "KBMForOneChannel.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
KBMForOneChannel::KBMForOneChannel (int		subDlgIndex, KBMFilesMappingLogic&	mappingLogic, int& scaleSizeReference)
    : errorVisualizer(TerpstraSysExApplication::getApp().getLookAndFeel()), scaleSize(scaleSizeReference)
{
    //[Constructor_pre] You can add your own custom stuff here..
    this->subDlgIndex = subDlgIndex;
    this->pMappingLogic = &mappingLogic;
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

    btnFileSelectMacro->setBounds (168, 8, 24, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (200, 32);


    //[Constructor] You can add your own custom stuff here..
    channelBox->addItem("none", -1);      // Empty value

    for (int i = 1; i <= 16; i++)
        channelBox->addItem(String(i), i);

    textMappingFile->addListener(this);
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
    //[/UserResized]
}

void KBMForOneChannel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == channelBox.get())
    {
        //[UserComboBoxCode_channelBox] -- add your combo box handling code here..

        // If no channel is selected: clear file object
        if ( channelBox->getSelectedId() <= 0 )
            currentFile = File();

        updateFieldsAndMappingLogic();

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
			updateFieldsAndMappingLogic();
		}
        //[/UserButtonCode_btnFileSelectMacro]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void KBMForOneChannel::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	String keyName = "KBMChannel" + String(subDlgIndex);
	int midiChannel = propertiesFile->getIntValue(keyName);
	channelBox->setSelectedId(midiChannel, juce::dontSendNotification);

	keyName = "KBMFile" + String(subDlgIndex);
	String keyValue = propertiesFile->getValue(keyName);

	currentFile = File(keyValue);

	if ( this->isVisible() && this->getParentComponent()->isVisible())
        updateFieldsAndMappingLogic();
}

void KBMForOneChannel::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
    int midiChannel = channelBox->getSelectedId();
	String keyName = "KBMChannel" + String(subDlgIndex);
	propertiesFile->setValue(keyName, midiChannel);

	keyName = "KBMFile" + String(subDlgIndex);
	String keyValue = currentFile.getFullPathName();
	propertiesFile->setValue(keyName, keyValue);
}

void KBMForOneChannel::textEditorFocusLost(TextEditor& textEdit)
{
	if (&textEdit == textMappingFile.get())
	{
	    auto newFileName = textMappingFile->getText();
	    auto newFile = File(newFileName);

        // Attention to the case where textEdit contains just the file name (without the path) of an existing file,
        // which is specified in currentFile
	    if (newFile.existsAsFile() || currentFile.getFileName() != newFileName)
        {
            currentFile = newFile;
            updateFieldsAndMappingLogic();
        }
	}
}

void KBMForOneChannel::updateFieldsAndMappingLogic()
{
    int midiChannel = channelBox->getSelectedId();

    // Edit field: file name (without full path), to save space
	textMappingFile->setText(currentFile.getFileName());

    if (currentFile.existsAsFile())
    {
        StringArray stringArray;
        currentFile.readLines(stringArray);
        kbmMappingStructure.fromStringArray(stringArray);

        // Tooltip: either the file name with full path or an error message
        String errorMsg = kbmMappingStructure.getErrorMessage();
        if (!errorMsg.isEmpty())
        {
            errorVisualizer.setErrorLevel(
                *textMappingFile.get(),
                HajuErrorVisualizer::ErrorLevel::error,
                errorMsg);
        }
        else
        {
            // Warning if the kbm file's scale size doesn't match the global scaleSize
            if (kbmMappingStructure.scaleSize != this->scaleSize)
                errorVisualizer.setErrorLevel(
                    *textMappingFile.get(),
                    HajuErrorVisualizer::ErrorLevel::warning,
                    "Scale size of KBM file does not match");
            else
                errorVisualizer.setErrorLevel(
                    *textMappingFile.get(),
                    HajuErrorVisualizer::ErrorLevel::noError,
                    currentFile.getFullPathName());
        }
    }
	else
	{
		kbmMappingStructure = KBMMappingDataStructure();

        errorVisualizer.setErrorLevel(
            *textMappingFile.get(),
            midiChannel > 0 ? HajuErrorVisualizer::ErrorLevel::warning : HajuErrorVisualizer::ErrorLevel::noError,
            "File not found");
	}

    // Enable or disable file fields
    if ( midiChannel > 0 )
    {
        textMappingFile->setEnabled(true);
        btnFileSelectMacro->setEnabled(true);
    }
    else
    {
        textMappingFile->setEnabled(false);
        btnFileSelectMacro->setEnabled(false);
    }

    // Update mapping logic. Both midiChannel and kbmMappingStructure may be empty.
    if (pMappingLogic != nullptr)
        pMappingLogic->setMapping(subDlgIndex, midiChannel, kbmMappingStructure);
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
                 constructorParams="int&#9;&#9;subDlgIndex, KBMFilesMappingLogic&amp;&#9;mappingLogic, int&amp; scaleSizeReference"
                 variableInitialisers="errorVisualizer(TerpstraSysExApplication::getApp().getLookAndFeel()), scaleSize(scaleSizeReference)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="200" initialHeight="32">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <COMBOBOX name="channelBox" id="250a1dde474111c4" memberName="channelBox"
            virtualName="" explicitFocusOrder="0" pos="0 8 48 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTEDITOR name="textMappingFile" id="b9bc5baf677ed902" memberName="textMappingFile"
              virtualName="" explicitFocusOrder="0" pos="56 8 104 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTBUTTON name="btnFileSelectMacro" id="23cc77cbad6653d7" memberName="btnFileSelectMacro"
              virtualName="" explicitFocusOrder="0" pos="168 8 24 24" buttonText="..."
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

