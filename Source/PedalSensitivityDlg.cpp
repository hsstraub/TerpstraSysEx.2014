/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "Main.h"
//[/Headers]

#include "PedalSensitivityDlg.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PedalSensitivityDlg::PedalSensitivityDlg ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    labelExprContrSensivity.reset (new juce::Label ("new label", translate("Sensitivity")));
    addAndMakeVisible (labelExprContrSensivity.get());
	labelExprContrSensivity->setFont(LumatoneEditorFonts::UniviaProBold());

    labelExprContrSensivity->setBounds (6, 35, 64, 24);

    txtExprCtrlSensivity.reset (new juce::TextEditor ("txtExprCtrlSensivity"));
    addAndMakeVisible (txtExprCtrlSensivity.get());
    txtExprCtrlSensivity->setMultiLine (false);
    txtExprCtrlSensivity->setReturnKeyStartsNewLine (false);
    txtExprCtrlSensivity->setReadOnly (false);
    txtExprCtrlSensivity->setScrollbarsShown (true);
    txtExprCtrlSensivity->setCaretVisible (true);
    txtExprCtrlSensivity->setPopupMenuEnabled (true);
    txtExprCtrlSensivity->setText (juce::String());

    txtExprCtrlSensivity->setBounds (70, 35, 56, 24);

    btnInvertFootCtrl.reset (new juce::ToggleButton ("btnInvertFootCtrl"));
    addAndMakeVisible (btnInvertFootCtrl.get());
    btnInvertFootCtrl->setButtonText (translate("Invert Sustain Pedal"));
    btnInvertFootCtrl->addListener (this);

    btnInvertFootCtrl->setBounds (6, 67, 162, 24);

    labelEXpressionPedalTitle.reset (new juce::Label ("labelEXpressionPedalTitle",
                                                      translate("Expression Pedal")));
    addAndMakeVisible (labelEXpressionPedalTitle.get());
	labelEXpressionPedalTitle->setFont(LumatoneEditorFonts::UniviaProBold());

    //[UserPreSize]
	txtExprCtrlSensivity->addListener(this);
    //[/UserPreSize]

    setSize (134, 96);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

PedalSensitivityDlg::~PedalSensitivityDlg()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    labelExprContrSensivity = nullptr;
    txtExprCtrlSensivity = nullptr;
    btnInvertFootCtrl = nullptr;
    labelEXpressionPedalTitle = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PedalSensitivityDlg::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
	g.setColour(Colour(0xff212626));
	g.fillRoundedRectangle(getLocalBounds().toFloat().withTop(proportionOfHeight(settingsAreaMarginHeight)), roundedCornerSize);
    //[/UserPaint]
}

void PedalSensitivityDlg::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..

	roundedCornerSize = round(getParentHeight() * roundedCornerLayoutAppHeightScalar);

	resizeLabelWithHeight(labelEXpressionPedalTitle.get(), roundToInt(getHeight() * settingsLabelHeight));
	labelEXpressionPedalTitle->setTopLeftPosition(roundToInt(getWidth() * settingsLabelMarginWidth), 0);

	int marginX = roundToInt(getParentWidth() * settingsControlMarginParentWidthScalar);

	labelExprContrSensivity->setBounds(
		marginX, proportionOfHeight(0.3f), proportionOfWidth(1.0f), proportionOfHeight(settingsToggleButtonHeight)
	);

	btnInvertFootCtrl->setBounds(
		marginX, proportionOfHeight(0.6f), proportionOfWidth(1.0f), proportionOfHeight(settingsToggleButtonHeight)
	);
	
	//[/UserResized]
}

void PedalSensitivityDlg::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnInvertFootCtrl.get())
    {
        //[UserButtonCode_btnInvertFootCtrl] -- add your button handler code here..
		((MainContentComponent*)getParentComponent())->getMappingInEdit().invertFootController = btnInvertFootCtrl->getToggleState();
		TerpstraSysExApplication::getApp().setHasChangesToSave(true);
		TerpstraSysExApplication::getApp().getMidiDriver().sendInvertFootController(btnInvertFootCtrl->getToggleState());
        //[/UserButtonCode_btnInvertFootCtrl]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...


void PedalSensitivityDlg::textEditorTextChanged(TextEditor& textEdit)
{
	if (&textEdit == txtExprCtrlSensivity.get())
	{
		int newSensitvity = textEdit.getText().getIntValue();
		// ToDo value checking: encapsulate in keyboard data structure?
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

		((MainContentComponent*)getParentComponent())->getMappingInEdit().expressionControllerSensivity = newSensitvity;
		TerpstraSysExApplication::getApp().setHasChangesToSave(true);
		TerpstraSysExApplication::getApp().getMidiDriver().sendExpressionPedalSensivity(newSensitvity);
	}
}

void PedalSensitivityDlg::textEditorFocusLost(TextEditor& textEdit)
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

		((MainContentComponent*)getParentComponent())->getMappingInEdit().expressionControllerSensivity = newSensitvity;
		TerpstraSysExApplication::getApp().setHasChangesToSave(true);
		TerpstraSysExApplication::getApp().getMidiDriver().sendExpressionPedalSensivity(newSensitvity);
	}
}

void PedalSensitivityDlg::lookAndFeelChanged()
{
	labelEXpressionPedalTitle->setColour(Label::ColourIds::textColourId, getLookAndFeel().findColour(LumatoneEditorColourIDs::LabelBlue));
}

void PedalSensitivityDlg::loadFromMapping()
{
	auto mappingInEdit = ((MainContentComponent*)getParentComponent())->getMappingInEdit();

	btnInvertFootCtrl->setToggleState(mappingInEdit.invertFootController, juce::NotificationType::dontSendNotification);
	txtExprCtrlSensivity->setText(String(mappingInEdit.expressionControllerSensivity), false);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PedalSensitivityDlg" componentName=""
                 parentClasses="public juce::Component, public TextEditor::Listener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="134"
                 initialHeight="96">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="new label" id="22d529ada4ac7738" memberName="labelExprContrSensivity"
         virtualName="" explicitFocusOrder="0" pos="6 35 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Sensivity:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="txtExprCtrlSensivity" id="859fad57998470cd" memberName="txtExprCtrlSensivity"
              virtualName="" explicitFocusOrder="0" pos="70 35 56 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TOGGLEBUTTON name="btnInvertFootCtrl" id="ef6e332d2b99beda" memberName="btnInvertFootCtrl"
                virtualName="" explicitFocusOrder="0" pos="6 67 162 24" buttonText="Invert Sustain Pedal"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="labelEXpressionPedalTitle" id="afc0b85c8e03b3d6" memberName="labelEXpressionPedalTitle"
         virtualName="" explicitFocusOrder="0" pos="6 3 104 24" textCol="ff61acc8"
         edTextCol="ff000000" edBkgCol="0" labelText="Expression Pedal"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="18.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

