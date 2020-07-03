/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

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
	inactiveMacroButtonColourEdit.reset(new ColourEditComponent());
	addAndMakeVisible(inactiveMacroButtonColourEdit.get());
	inactiveMacroButtonColourEdit->setVisible(true);
	inactiveMacroButtonColourEdit->addChangeListener(this);

	activeMacroButtonColourEdit.reset(new ColourEditComponent());
	addAndMakeVisible(activeMacroButtonColourEdit.get());
	activeMacroButtonColourEdit->setVisible(true);
	activeMacroButtonColourEdit->addChangeListener(this);
    //[/Constructor_pre]

    labelExprContrSensivity.reset (new Label ("new label",
                                              TRANS("Expression pedal sensivity:")));
    addAndMakeVisible (labelExprContrSensivity.get());
    labelExprContrSensivity->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelExprContrSensivity->setJustificationType (Justification::centredLeft);
    labelExprContrSensivity->setEditable (false, false, false);
    labelExprContrSensivity->setColour (TextEditor::textColourId, Colours::black);
    labelExprContrSensivity->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelExprContrSensivity->setBounds (8, 16, 176, 24);

    txtExprCtrlSensivity.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (txtExprCtrlSensivity.get());
    txtExprCtrlSensivity->setTooltip (TRANS("Not working yet"));
    txtExprCtrlSensivity->setMultiLine (false);
    txtExprCtrlSensivity->setReturnKeyStartsNewLine (false);
    txtExprCtrlSensivity->setReadOnly (false);
    txtExprCtrlSensivity->setScrollbarsShown (true);
    txtExprCtrlSensivity->setCaretVisible (true);
    txtExprCtrlSensivity->setPopupMenuEnabled (true);
    txtExprCtrlSensivity->setText (String());

    txtExprCtrlSensivity->setBounds (240, 8, 56, 24);

    btnInvertFootCtrl.reset (new ToggleButton ("btnInvertFootCtrl"));
    addAndMakeVisible (btnInvertFootCtrl.get());
    btnInvertFootCtrl->setButtonText (String());
    btnInvertFootCtrl->addListener (this);

    btnInvertFootCtrl->setBounds (240, 40, 56, 24);

    lblInvFootCtrl.reset (new Label ("lblInvFootCtrl",
                                     TRANS("Invert foot controller:")));
    addAndMakeVisible (lblInvFootCtrl.get());
    lblInvFootCtrl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    lblInvFootCtrl->setJustificationType (Justification::centredLeft);
    lblInvFootCtrl->setEditable (false, false, false);
    lblInvFootCtrl->setColour (TextEditor::textColourId, Colours::black);
    lblInvFootCtrl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lblInvFootCtrl->setBounds (8, 48, 176, 24);

    lblColourInactiveMacroButton.reset (new Label ("lblColourInactiveMacroButton",
                                                   TRANS("Colour of inactive macro buttons:")));
    addAndMakeVisible (lblColourInactiveMacroButton.get());
    lblColourInactiveMacroButton->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    lblColourInactiveMacroButton->setJustificationType (Justification::centredLeft);
    lblColourInactiveMacroButton->setEditable (false, false, false);
    lblColourInactiveMacroButton->setColour (TextEditor::textColourId, Colours::black);
    lblColourInactiveMacroButton->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lblColourInactiveMacroButton->setBounds (8, 112, 224, 24);

    lblColourActiveMacroButton.reset (new Label ("lblColourActiveMacroButton",
                                                 TRANS("Colour of active macro buttons:")));
    addAndMakeVisible (lblColourActiveMacroButton.get());
    lblColourActiveMacroButton->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    lblColourActiveMacroButton->setJustificationType (Justification::centredLeft);
    lblColourActiveMacroButton->setEditable (false, false, false);
    lblColourActiveMacroButton->setColour (TextEditor::textColourId, Colours::black);
    lblColourActiveMacroButton->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lblColourActiveMacroButton->setBounds (8, 144, 224, 24);

    lblManufacturerId.reset (new Label ("lblManufacturerId",
                                        TRANS("Manufacturer Id:")));
    addAndMakeVisible (lblManufacturerId.get());
    lblManufacturerId->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    lblManufacturerId->setJustificationType (Justification::centredLeft);
    lblManufacturerId->setEditable (false, false, false);
    lblManufacturerId->setColour (TextEditor::textColourId, Colours::black);
    lblManufacturerId->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lblManufacturerId->setBounds (8, 176, 224, 24);

    manufacturerIdBox.reset (new ComboBox ("manufacturerIdBox"));
    addAndMakeVisible (manufacturerIdBox.get());
    manufacturerIdBox->setEditableText (false);
    manufacturerIdBox->setJustificationType (Justification::centredLeft);
    manufacturerIdBox->setTextWhenNothingSelected (String());
    manufacturerIdBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    manufacturerIdBox->addItem (TRANS("0020FF"), 1);
    manufacturerIdBox->addItem (TRANS("002150"), 2);
    manufacturerIdBox->addListener (this);

    manufacturerIdBox->setBounds (240, 176, 96, 24);


    //[UserPreSize]
	txtExprCtrlSensivity->addListener(this);
    //[/UserPreSize]

    setSize (480, 240);


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
    lblColourInactiveMacroButton = nullptr;
    lblColourActiveMacroButton = nullptr;
    lblManufacturerId = nullptr;
    manufacturerIdBox = nullptr;


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

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GeneralOptionsDlg::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnInvertFootCtrl.get())
    {
        //[UserButtonCode_btnInvertFootCtrl] -- add your button handler code here..

		// Send foot controller parametrization to controller
		TerpstraSysExApplication::getApp().getMidiDriver().sendInvertFootController(buttonThatWasClicked->getToggleState());
        //[/UserButtonCode_btnInvertFootCtrl]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void GeneralOptionsDlg::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == manufacturerIdBox.get())
    {
        //[UserComboBoxCode_manufacturerIdBox] -- add your combo box handling code here..
        int manufacturerId =  manufacturerIdBox->getItemText(manufacturerIdBox->getSelectedItemIndex()).getHexValue32();
        TerpstraSysExApplication::getApp().getMidiDriver().setManufacturerId(manufacturerId);
        //[/UserComboBoxCode_manufacturerIdBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void GeneralOptionsDlg::textEditorFocusLost(TextEditor& textEdit)
{
	if (&textEdit == txtExprCtrlSensivity.get())
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
	if (source == inactiveMacroButtonColourEdit.get())
	{
		String inactiveMacroButtonColour = inactiveMacroButtonColourEdit->getColourAsString();
		TerpstraSysExApplication::getApp().getMidiDriver().sendMacroButtonInactiveColour(inactiveMacroButtonColour);
	}
	else if (source == activeMacroButtonColourEdit.get())
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

	inactiveMacroButtonColourEdit->setColour(
		propertiesFile->getValue("InactiveMacroButtonColour", "000000"));

	activeMacroButtonColourEdit->setColour(
		propertiesFile->getValue("ActiveMacroButtonColour", "FFFFFF"));

    String manufacturerIdText = String::toHexString(propertiesFile->getIntValue("ManufacturerId", 0x002150))
		.paddedLeft('0', 6).toUpperCase();
	// ToDO This is a little low-Level...?
    for ( int i = 0; i < manufacturerIdBox->getNumItems(); i++)
    {
        if (manufacturerIdBox->getItemText(i) == manufacturerIdText)
        {
            manufacturerIdBox->setSelectedItemIndex(i);
            break;
        }
    }
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

	propertiesFile->removeValue("LightOnKeyStroke");	// Old parameter that has been deprecated

	String inactiveMacroButtonColour = inactiveMacroButtonColourEdit->getColourAsString();
	propertiesFile->setValue("InactiveMacroButtonColour", inactiveMacroButtonColour);

	String activeMacroButtonColour = activeMacroButtonColourEdit->getColourAsString();
	propertiesFile->setValue("ActiveMacroButtonColour", activeMacroButtonColour);

    int manufacturerId = manufacturerIdBox->getItemText(manufacturerIdBox->getSelectedItemIndex()).getHexValue32();
    propertiesFile->setValue("ManufacturerId", manufacturerId);
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
                 initialHeight="240">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <LABEL name="new label" id="22d529ada4ac7738" memberName="labelExprContrSensivity"
         virtualName="" explicitFocusOrder="0" pos="8 16 176 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Expression pedal sensivity:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
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
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblColourInactiveMacroButton" id="9c5cddfcc2966280" memberName="lblColourInactiveMacroButton"
         virtualName="" explicitFocusOrder="0" pos="8 112 224 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Colour of inactive macro buttons:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblColourActiveMacroButton" id="79f93468a20f174" memberName="lblColourActiveMacroButton"
         virtualName="" explicitFocusOrder="0" pos="8 144 224 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Colour of active macro buttons:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblManufacturerId" id="10618e2f30d0f8f9" memberName="lblManufacturerId"
         virtualName="" explicitFocusOrder="0" pos="8 176 224 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Manufacturer Id:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="manufacturerIdBox" id="4cf52bc6db0440a6" memberName="manufacturerIdBox"
            virtualName="" explicitFocusOrder="0" pos="240 176 96 24" editable="0"
            layout="33" items="0020FF&#10;002150" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

