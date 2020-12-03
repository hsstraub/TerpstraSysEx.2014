/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.4

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

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
    //[/Constructor_pre]

    labelExprContrSensivity.reset (new juce::Label ("new label",
                                                    TRANS("Sensivity:")));
    addAndMakeVisible (labelExprContrSensivity.get());
    labelExprContrSensivity->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelExprContrSensivity->setJustificationType (juce::Justification::centredLeft);
    labelExprContrSensivity->setEditable (false, false, false);
    labelExprContrSensivity->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelExprContrSensivity->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelExprContrSensivity->setBounds (192, 32, 64, 24);

    txtExprCtrlSensivity.reset (new juce::TextEditor ("new text editor"));
    addAndMakeVisible (txtExprCtrlSensivity.get());
    txtExprCtrlSensivity->setTooltip (TRANS("Not working yet"));
    txtExprCtrlSensivity->setMultiLine (false);
    txtExprCtrlSensivity->setReturnKeyStartsNewLine (false);
    txtExprCtrlSensivity->setReadOnly (false);
    txtExprCtrlSensivity->setScrollbarsShown (true);
    txtExprCtrlSensivity->setCaretVisible (true);
    txtExprCtrlSensivity->setPopupMenuEnabled (true);
    txtExprCtrlSensivity->setText (juce::String());

    txtExprCtrlSensivity->setBounds (256, 32, 56, 24);

    btnInvertFootCtrl.reset (new juce::ToggleButton ("btnInvertFootCtrl"));
    addAndMakeVisible (btnInvertFootCtrl.get());
    btnInvertFootCtrl->setButtonText (TRANS("Invert Pedal"));
    btnInvertFootCtrl->addListener (this);

    btnInvertFootCtrl->setBounds (192, 64, 128, 24);

    labelEXpressionPedalTitle.reset (new juce::Label ("labelEXpressionPedalTitle",
                                                      TRANS("Expression Pedal")));
    addAndMakeVisible (labelEXpressionPedalTitle.get());
    labelEXpressionPedalTitle->setFont (juce::Font (18.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelEXpressionPedalTitle->setJustificationType (juce::Justification::centredLeft);
    labelEXpressionPedalTitle->setEditable (false, false, false);
    labelEXpressionPedalTitle->setColour (juce::Label::textColourId, juce::Colour (0xff61acc8));
    labelEXpressionPedalTitle->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelEXpressionPedalTitle->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelEXpressionPedalTitle->setBounds (192, 8, 104, 24);

    labelGeneralSettingslTitle.reset (new juce::Label ("labelGeneralSettingslTitle",
                                                       TRANS("General Settings")));
    addAndMakeVisible (labelGeneralSettingslTitle.get());
    labelGeneralSettingslTitle->setFont (juce::Font (18.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelGeneralSettingslTitle->setJustificationType (juce::Justification::centredLeft);
    labelGeneralSettingslTitle->setEditable (false, false, false);
    labelGeneralSettingslTitle->setColour (juce::Label::textColourId, juce::Colour (0xff61acc8));
    labelGeneralSettingslTitle->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelGeneralSettingslTitle->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelGeneralSettingslTitle->setBounds (8, 8, 104, 24);

    buttonAfterTouchActive.reset (new juce::ToggleButton ("buttonAfterTouchActive"));
    addAndMakeVisible (buttonAfterTouchActive.get());
    buttonAfterTouchActive->setButtonText (TRANS("Polyphonic Aftertouch"));
    buttonAfterTouchActive->addListener (this);

    buttonAfterTouchActive->setBounds (8, 32, 176, 24);


    //[UserPreSize]
	txtExprCtrlSensivity->addListener(this);
    //[/UserPreSize]

    setSize (472, 96);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

GeneralOptionsDlg::~GeneralOptionsDlg()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    labelExprContrSensivity = nullptr;
    txtExprCtrlSensivity = nullptr;
    btnInvertFootCtrl = nullptr;
    labelEXpressionPedalTitle = nullptr;
    labelGeneralSettingslTitle = nullptr;
    buttonAfterTouchActive = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GeneralOptionsDlg::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(findColour(ResizableWindow::backgroundColourId));
    //[/UserPaint]
}

void GeneralOptionsDlg::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GeneralOptionsDlg::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnInvertFootCtrl.get())
    {
        //[UserButtonCode_btnInvertFootCtrl] -- add your button handler code here..

		// ToDo Set to parameter structure (to be saved in *.LMT file)

		// Send foot controller parametrization to controller
		TerpstraSysExApplication::getApp().getMidiDriver().sendInvertFootController(buttonThatWasClicked->getToggleState());
        //[/UserButtonCode_btnInvertFootCtrl]
    }
    else if (buttonThatWasClicked == buttonAfterTouchActive.get())
    {
        //[UserButtonCode_buttonAfterTouchActive] -- add your button handler code here..
        //[/UserButtonCode_buttonAfterTouchActive]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
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

		// ToDo Set to parameter structure (to be saved in *.LMT file)

		TerpstraSysExApplication::getApp().getMidiDriver().sendExpressionPedalSensivity(newSensitvity);
	}
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GeneralOptionsDlg" componentName=""
                 parentClasses="public Component, public TextEditor::Listener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="1" initialWidth="472"
                 initialHeight="96">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <LABEL name="new label" id="22d529ada4ac7738" memberName="labelExprContrSensivity"
         virtualName="" explicitFocusOrder="0" pos="192 32 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Sensivity:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="859fad57998470cd" memberName="txtExprCtrlSensivity"
              virtualName="" explicitFocusOrder="0" pos="256 32 56 24" tooltip="Not working yet"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <TOGGLEBUTTON name="btnInvertFootCtrl" id="ef6e332d2b99beda" memberName="btnInvertFootCtrl"
                virtualName="" explicitFocusOrder="0" pos="192 64 128 24" buttonText="Invert Pedal"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="labelEXpressionPedalTitle" id="afc0b85c8e03b3d6" memberName="labelEXpressionPedalTitle"
         virtualName="" explicitFocusOrder="0" pos="192 8 104 24" textCol="ff61acc8"
         edTextCol="ff000000" edBkgCol="0" labelText="Expression Pedal"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="18.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="labelGeneralSettingslTitle" id="9ac4ac959d4cd11a" memberName="labelGeneralSettingslTitle"
         virtualName="" explicitFocusOrder="0" pos="8 8 104 24" textCol="ff61acc8"
         edTextCol="ff000000" edBkgCol="0" labelText="General Settings"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="18.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <TOGGLEBUTTON name="buttonAfterTouchActive" id="3f2eba6027c4f2f5" memberName="buttonAfterTouchActive"
                virtualName="" explicitFocusOrder="0" pos="8 32 176 24" buttonText="Polyphonic Aftertouch"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

