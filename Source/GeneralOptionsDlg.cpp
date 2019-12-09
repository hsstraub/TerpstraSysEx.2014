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
#include "Main.h"
//[/Headers]

#include "GeneralOptionsDlg.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GeneralOptionsDlg::GeneralOptionsDlg ()
{
    //[Constructor_pre] You can add your own custom stuff here..
	inactiveMacroButtonColourEdit = new ColourEditComponent();
	addAndMakeVisible(inactiveMacroButtonColourEdit);
	inactiveMacroButtonColourEdit->setVisible(true);
	inactiveMacroButtonColourEdit->addChangeListener(this);

	activeMacroButtonColourEdit = new ColourEditComponent();
	addAndMakeVisible(activeMacroButtonColourEdit);
	activeMacroButtonColourEdit->setVisible(true);
	activeMacroButtonColourEdit->addChangeListener(this);
	//[/Constructor_pre]

    addAndMakeVisible (labelExprContrSensivity = new Label ("new label",
                                                            TRANS("Expression pedal sensivity:")));
    labelExprContrSensivity->setFont (Font (15.00f, Font::plain));
    labelExprContrSensivity->setJustificationType (Justification::centredLeft);
    labelExprContrSensivity->setEditable (false, false, false);
    labelExprContrSensivity->setColour (TextEditor::textColourId, Colours::black);
    labelExprContrSensivity->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (txtExprCtrlSensivity = new TextEditor ("new text editor"));
    txtExprCtrlSensivity->setTooltip (TRANS("Not working yet"));
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

    addAndMakeVisible (lblColourInactiveMacroButton = new Label ("lblColourInactiveMacroButton",
                                                                 TRANS("Colour of inactive macro buttons:")));
    lblColourInactiveMacroButton->setFont (Font (15.00f, Font::plain));
    lblColourInactiveMacroButton->setJustificationType (Justification::centredLeft);
    lblColourInactiveMacroButton->setEditable (false, false, false);
    lblColourInactiveMacroButton->setColour (TextEditor::textColourId, Colours::black);
    lblColourInactiveMacroButton->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (lblColourActiveMacroButton = new Label ("lblColourActiveMacroButton",
                                                               TRANS("Colour of active macro buttons:")));
    lblColourActiveMacroButton->setFont (Font (15.00f, Font::plain));
    lblColourActiveMacroButton->setJustificationType (Justification::centredLeft);
    lblColourActiveMacroButton->setEditable (false, false, false);
    lblColourActiveMacroButton->setColour (TextEditor::textColourId, Colours::black);
    lblColourActiveMacroButton->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
	txtExprCtrlSensivity->addListener(this);
    //[/UserPreSize]

    setSize (480, 220);


    //[Constructor] You can add your own custom stuff here..

	// Set values according to the properties files
	restoreStateFromPropertiesFile(TerpstraSysExApplication::getApp().getPropertiesFile());

    //[/Constructor]
}

GeneralOptionsDlg::~GeneralOptionsDlg()
{
    //[Destructor_pre]. You can add your own custom destruction code here..

	// Save values to properties file
	saveStateToPropertiesFile(TerpstraSysExApplication::getApp().getPropertiesFile());

	inactiveMacroButtonColourEdit = nullptr;
	activeMacroButtonColourEdit = nullptr;
    //[/Destructor_pre]

    labelExprContrSensivity = nullptr;
    txtExprCtrlSensivity = nullptr;
    btnInvertFootCtrl = nullptr;
    lblInvFootCtrl = nullptr;
    lblLightOnKeyStroke = nullptr;
    btnLightOnKeyStroke = nullptr;
    lblColourInactiveMacroButton = nullptr;
    lblColourActiveMacroButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GeneralOptionsDlg::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(findColour(ResizableWindow::backgroundColourId));
    //[/UserPaint]
}

void GeneralOptionsDlg::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	inactiveMacroButtonColourEdit->setBounds(240, 112, 196, 32);
	activeMacroButtonColourEdit->setBounds(240, 144, 196, 32);
    //[/UserPreResize]

    labelExprContrSensivity->setBounds (8, 16, 176, 24);
    txtExprCtrlSensivity->setBounds (240, 8, 56, 24);
    btnInvertFootCtrl->setBounds (240, 40, 56, 24);
    lblInvFootCtrl->setBounds (8, 48, 176, 24);
    lblLightOnKeyStroke->setBounds (8, 80, 176, 24);
    btnLightOnKeyStroke->setBounds (240, 72, 56, 24);
    lblColourInactiveMacroButton->setBounds (8, 112, 224, 24);
    lblColourActiveMacroButton->setBounds (8, 144, 224, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GeneralOptionsDlg::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnInvertFootCtrl)
    {
        //[UserButtonCode_btnInvertFootCtrl] -- add your button handler code here..

		// Send foot controller parametrization to controller
		TerpstraSysExApplication::getApp().getMidiDriver().sendInvertFootController(buttonThatWasClicked->getToggleState());
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

void GeneralOptionsDlg::textEditorFocusLost(TextEditor& textEdit)
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

void GeneralOptionsDlg::changeListenerCallback(ChangeBroadcaster *source)
{
	if (source == inactiveMacroButtonColourEdit)
	{
		String inactiveMacroButtonColour = inactiveMacroButtonColourEdit->getColourAsString();
		TerpstraSysExApplication::getApp().getMidiDriver().sendMacroButtonInactiveColour(inactiveMacroButtonColour);
	}
	else if (source == activeMacroButtonColourEdit)
	{
		String activeMacroButtonColour = activeMacroButtonColourEdit->getColourAsString();
		TerpstraSysExApplication::getApp().getMidiDriver().sendMacroButtonActiveColour(activeMacroButtonColour);
	}
}

void GeneralOptionsDlg::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	btnInvertFootCtrl->setToggleState(
		propertiesFile->getBoolValue("InvertFootController", false),
		juce::NotificationType::dontSendNotification);

	txtExprCtrlSensivity->setText(String(propertiesFile->getIntValue("ExpressionControllerSensivity", 0x7f)));

	btnLightOnKeyStroke->setToggleState(
		propertiesFile->getBoolValue("LightOnKeyStroke", false),
		juce::NotificationType::dontSendNotification);

	inactiveMacroButtonColourEdit->setColour(
		propertiesFile->getValue("InactiveMacroButtonColour", "000000"));

	activeMacroButtonColourEdit->setColour(
		propertiesFile->getValue("ActiveMacroButtonColour", "FFFFFF"));

}

void GeneralOptionsDlg::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	propertiesFile->setValue("InvertFootController", btnInvertFootCtrl->getToggleState());

	int newSensitvity = txtExprCtrlSensivity->getText().getIntValue();
	if (newSensitvity < 0)
		newSensitvity = 0;
	if (newSensitvity > 0x7f)
		newSensitvity = 0x7f;
	propertiesFile->setValue("ExpressionControllerSensivity", newSensitvity);

	propertiesFile->setValue("LightOnKeyStroke", btnLightOnKeyStroke->getToggleState());

	String inactiveMacroButtonColour = inactiveMacroButtonColourEdit->getColourAsString();
	propertiesFile->setValue("InactiveMacroButtonColour", inactiveMacroButtonColour);

	String activeMacroButtonColour = activeMacroButtonColourEdit->getColourAsString();
	propertiesFile->setValue("ActiveMacroButtonColour", activeMacroButtonColour);
}


//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GeneralOptionsDlg" componentName=""
                 parentClasses="public Component, public TextEditor::Listener, public ChangeListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="1" initialWidth="480"
                 initialHeight="220">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <LABEL name="new label" id="22d529ada4ac7738" memberName="labelExprContrSensivity"
         virtualName="" explicitFocusOrder="0" pos="8 16 176 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Expression pedal sensivity:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="859fad57998470cd" memberName="txtExprCtrlSensivity"
              virtualName="" explicitFocusOrder="0" pos="240 8 56 24" tooltip="Not working yet"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TOGGLEBUTTON name="btnInvertFootCtrl" id="ef6e332d2b99beda" memberName="btnInvertFootCtrl"
                virtualName="" explicitFocusOrder="0" pos="240 40 56 24" buttonText=""
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
                virtualName="" explicitFocusOrder="0" pos="240 72 56 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="lblColourInactiveMacroButton" id="9c5cddfcc2966280" memberName="lblColourInactiveMacroButton"
         virtualName="" explicitFocusOrder="0" pos="8 112 224 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Colour of inactive macro buttons:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
  <LABEL name="lblColourActiveMacroButton" id="79f93468a20f174" memberName="lblColourActiveMacroButton"
         virtualName="" explicitFocusOrder="0" pos="8 144 224 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Colour of active macro buttons:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
