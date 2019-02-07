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

#include "Main.h"
#include "OptionsWindow.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
OptionsWindow::OptionsWindow ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (labelExprContrSensivity = new Label ("new label",
                                                            TRANS("Expression pedal sensivity:")));
    labelExprContrSensivity->setFont (Font (15.00f, Font::plain));
    labelExprContrSensivity->setJustificationType (Justification::centredLeft);
    labelExprContrSensivity->setEditable (false, false, false);
    labelExprContrSensivity->setColour (TextEditor::textColourId, Colours::black);
    labelExprContrSensivity->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (txtExprCtrlSensivity = new TextEditor ("new text editor"));
    txtExprCtrlSensivity->setMultiLine (false);
    txtExprCtrlSensivity->setReturnKeyStartsNewLine (false);
    txtExprCtrlSensivity->setReadOnly (false);
    txtExprCtrlSensivity->setScrollbarsShown (true);
    txtExprCtrlSensivity->setCaretVisible (true);
    txtExprCtrlSensivity->setPopupMenuEnabled (true);
    txtExprCtrlSensivity->setText (String());

    addAndMakeVisible (btnInvertFootCtrl = new ToggleButton ("btnInvertFootCtrl"));
    btnInvertFootCtrl->setButtonText (String());
    btnInvertFootCtrl->addListener (this);

    addAndMakeVisible (lblInvFootCtrl = new Label ("lblInvFootCtrl",
                                                   TRANS("Invert foot controller:")));
    lblInvFootCtrl->setFont (Font (15.00f, Font::plain));
    lblInvFootCtrl->setJustificationType (Justification::centredLeft);
    lblInvFootCtrl->setEditable (false, false, false);
    lblInvFootCtrl->setColour (TextEditor::textColourId, Colours::black);
    lblInvFootCtrl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (lblLightOnKeyStroke = new Label ("lblLightOnKeyStroke",
                                                        TRANS("Light on keystrokes:")));
    lblLightOnKeyStroke->setFont (Font (15.00f, Font::plain));
    lblLightOnKeyStroke->setJustificationType (Justification::centredLeft);
    lblLightOnKeyStroke->setEditable (false, false, false);
    lblLightOnKeyStroke->setColour (TextEditor::textColourId, Colours::black);
    lblLightOnKeyStroke->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (btnLightOnKeyStroke = new ToggleButton ("btnLightOnKeyStroke"));
    btnLightOnKeyStroke->setButtonText (String());
    btnLightOnKeyStroke->addListener (this);


    //[UserPreSize]
	txtExprCtrlSensivity->addListener(this);
    //[/UserPreSize]

    setSize (428, 220);


    //[Constructor] You can add your own custom stuff here..

	// Set values according to the properties files
	restoreStateFromPropertiesFile(TerpstraSysExApplication::getApp().getPropertiesFile());

	//[/Constructor]
}

OptionsWindow::~OptionsWindow()
{
    //[Destructor_pre]. You can add your own custom destruction code here..

	// Save values to properties file
	saveStateToPropertiesFile(TerpstraSysExApplication::getApp().getPropertiesFile());
    
	//[/Destructor_pre]

    labelExprContrSensivity = nullptr;
    txtExprCtrlSensivity = nullptr;
    btnInvertFootCtrl = nullptr;
    lblInvFootCtrl = nullptr;
    lblLightOnKeyStroke = nullptr;
    btnLightOnKeyStroke = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OptionsWindow::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OptionsWindow::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    labelExprContrSensivity->setBounds (8, 16, 176, 24);
	txtExprCtrlSensivity->setBounds(192, 16, 56, 24);
    btnInvertFootCtrl->setBounds (192, 48, 56, 24);
    lblInvFootCtrl->setBounds (8, 48, 176, 24);
    lblLightOnKeyStroke->setBounds (8, 80, 176, 24);
    btnLightOnKeyStroke->setBounds (192, 80, 56, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OptionsWindow::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnInvertFootCtrl)
    {
        //[UserButtonCode_btnInvertFootCtrl] -- add your button handler code here..
		
		// Send foot controller parametrization to controller
		//TerpstraSysExApplication::getApp().getMidiDriver().sendInvertFootController(buttonThatWasClicked->getToggleState());
		//[/UserButtonCode_btnInvertFootCtrl]
    }
    else if (buttonThatWasClicked == btnLightOnKeyStroke)
    {
        //[UserButtonCode_btnLightOnKeyStroke] -- add your button handler code here..
		TerpstraSysExApplication::getApp().getMidiDriver().sendLightOnKeyStroke(buttonThatWasClicked->getToggleState());
		//[/UserButtonCode_btnLightOnKeyStroke]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void OptionsWindow::textEditorFocusLost(TextEditor& textEdit)
{
	if (&textEdit == txtExprCtrlSensivity)
	{
		int newSensitvity = textEdit.getText().getIntValue();
		if (newSensitvity < 0)
		{
			newSensitvity = 0;
			textEdit.setText(String(newSensitvity));
		}

		if (newSensitvity > 0x7f)
		{
			newSensitvity = 0x7f;
			textEdit.setText(String(newSensitvity));
		}

		TerpstraSysExApplication::getApp().getMidiDriver().sendExpressionPedalSensivity(newSensitvity);
	}
}

void OptionsWindow::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	btnInvertFootCtrl->setToggleState(
		propertiesFile->getBoolValue("InvertFootController", false),
		juce::NotificationType::dontSendNotification);

	txtExprCtrlSensivity->setText(String(propertiesFile->getIntValue("ExpressionControllerSensivity", 0x7f)));

	btnLightOnKeyStroke->setToggleState(
		propertiesFile->getBoolValue("LightOnKeyStroke", false),
		juce::NotificationType::dontSendNotification);
}

void OptionsWindow::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	propertiesFile->setValue("InvertFootController", btnInvertFootCtrl->getToggleState());
	
	int newSensitvity = txtExprCtrlSensivity->getText().getIntValue();
	if (newSensitvity < 0)
		newSensitvity = 0;
	if (newSensitvity > 0x7f)
		newSensitvity = 0x7f;	
	propertiesFile->setValue("ExpressionControllerSensivity", newSensitvity);

	propertiesFile->setValue("LightOnKeyStroke", btnLightOnKeyStroke->getToggleState());
}


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OptionsWindow" componentName=""
                 parentClasses="public Component, public TextEditorListener" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="428" initialHeight="220">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <LABEL name="new label" id="22d529ada4ac7738" memberName="labelExprContrSensivity"
         virtualName="" explicitFocusOrder="0" pos="8 16 176 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Expression pedal sensivity:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="859fad57998470cd" memberName="txtExprCtrlSensivity"
              virtualName="" explicitFocusOrder="0" pos="192 16 56 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TOGGLEBUTTON name="btnInvertFootCtrl" id="ef6e332d2b99beda" memberName="btnInvertFootCtrl"
                virtualName="" explicitFocusOrder="0" pos="192 48 56 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="lblInvFootCtrl" id="6d42082683bab36c" memberName="lblInvFootCtrl"
         virtualName="" explicitFocusOrder="0" pos="8 48 176 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Invert foot controller:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="lblLightOnKeyStroke" id="d8737def929a5aa" memberName="lblLightOnKeyStroke"
         virtualName="" explicitFocusOrder="0" pos="8 80 176 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Light on keystrokes:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="btnLightOnKeyStroke" id="85279f9e93401da3" memberName="btnLightOnKeyStroke"
                virtualName="" explicitFocusOrder="0" pos="192 80 56 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
