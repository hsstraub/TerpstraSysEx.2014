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

    labelExprContrSensivity.reset(new juce::Label("new label", translate("Sensitivity")));
    addAndMakeVisible (labelExprContrSensivity.get());
    labelExprContrSensivity->setJustificationType(Justification::centred);
    labelExprContrSensivity->setBounds (6, 35, 64, 24);

    btnInvertFootCtrl.reset (new juce::ToggleButton ("btnInvertFootCtrl"));
    addAndMakeVisible (btnInvertFootCtrl.get());
    btnInvertFootCtrl->setButtonText (translate("Invert Pedal"));
    btnInvertFootCtrl->addListener (this);

    btnInvertFootCtrl->setBounds (6, 67, 162, 24);

    labelEXpressionPedalTitle.reset (new juce::Label ("labelEXpressionPedalTitle", translate("Expression Pedal")));
    addAndMakeVisible (labelEXpressionPedalTitle.get());
    labelEXpressionPedalTitle->setBounds (6, 3, 104, 24);

    sldExprCtrlSensivity.reset (new juce::Slider ("sldExprCtrlSensivity"));
    addAndMakeVisible (sldExprCtrlSensivity.get());
    sldExprCtrlSensivity->setRange (0, 127, 1);
    sldExprCtrlSensivity->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    sldExprCtrlSensivity->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 60, 20);
    sldExprCtrlSensivity->addListener (this);

    sldExprCtrlSensivity->setBounds (77, 12, 160, 72);


    //[UserPreSize]
    labelEXpressionPedalTitle->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::UniviaProBold));
    labelExprContrSensivity->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::GothamNarrowMediumBold));
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
    btnInvertFootCtrl = nullptr;
    labelEXpressionPedalTitle = nullptr;
    sldExprCtrlSensivity = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PedalSensitivityDlg::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
	g.setColour(Colour(0xff212626));
	g.fillRoundedRectangle(getLocalBounds().toFloat().withTop(proportionOfHeight(SETTINGSAREAMARGINHEIGHT)), roundedCornerSize);
    //[/UserPaint]
}

void PedalSensitivityDlg::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    float w = getWidth();
    float h = getHeight();
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..

    roundedCornerSize = round(getParentHeight() * ROUNDEDCORNERTOAPPHEIGHT);

    resizeLabelWithHeight(labelEXpressionPedalTitle.get(), roundToInt(getHeight() * SETTINGSLABELHEIGHT));
    labelEXpressionPedalTitle->setTopLeftPosition(roundToInt(getWidth() * SETTINGSLABELMARGINWIDTH), 0);

    int marginX = roundToInt(getParentWidth() * SETTINGSCONTROLMARGINTOAPPWIDTH);
    int buttonHeight = roundToInt(h * SETTINGSTOGGLEHEIGHTSCALAR);
    btnInvertFootCtrl->setBounds(marginX, proportionOfHeight(0.3f), w, buttonHeight);

    sldExprCtrlSensivity->setBounds(getLocalBounds().toFloat().getProportion(sliderBoundsProps).toNearestInt());

    resizeLabelWithHeight(labelExprContrSensivity.get(), buttonHeight * 1.25f);
    labelExprContrSensivity->setCentrePosition(sldExprCtrlSensivity->getBounds().getCentreX(), btnInvertFootCtrl->getBounds().getCentreY());
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

void PedalSensitivityDlg::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sldExprCtrlSensivity.get())
    {
        //[UserSliderCode_sldExprCtrlSensivity] -- add your slider handling code here..
        int newSensitvity = sldExprCtrlSensivity->getValue();
        // ToDo value checking: encapsulate in keyboard data structure?
        if (newSensitvity < 0)
        {
            newSensitvity = 0;
            sldExprCtrlSensivity->setValue(newSensitvity);
        }

        if (newSensitvity > 0x7f)
        {
            newSensitvity = 0x7f;
            sldExprCtrlSensivity->setValue(newSensitvity);
        }

        ((MainContentComponent*)getParentComponent())->getMappingInEdit().expressionControllerSensivity = newSensitvity;
        TerpstraSysExApplication::getApp().setHasChangesToSave(true);
        TerpstraSysExApplication::getApp().getMidiDriver().sendExpressionPedalSensivity(newSensitvity);
        //[/UserSliderCode_sldExprCtrlSensivity]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...


//void PedalSensitivityDlg::textEditorTextChanged(TextEditor& textEdit)
//{
//}
//
//void PedalSensitivityDlg::textEditorFocusLost(TextEditor& textEdit)
//{
//	if (&textEdit == txtExprCtrlSensivity.get())
//	{
//		int newSensitvity = textEdit.getText().getIntValue();
//		if (newSensitvity < 0)
//		{
//			newSensitvity = 0;
//			textEdit.setText(String(newSensitvity));
//		}
//
//		if (newSensitvity > 0x7f)
//		{
//			newSensitvity = 0x7f;
//			textEdit.setText(String(newSensitvity));
//		}
//
//		((MainContentComponent*)getParentComponent())->getMappingInEdit().expressionControllerSensivity = newSensitvity;
//		TerpstraSysExApplication::getApp().setHasChangesToSave(true);
//		TerpstraSysExApplication::getApp().getMidiDriver().sendExpressionPedalSensivity(newSensitvity);
//	}
//}

void PedalSensitivityDlg::lookAndFeelChanged()
{
    auto lookAndFeel = dynamic_cast<LumatoneEditorLookAndFeel*>(&getLookAndFeel());
    if (lookAndFeel)
    {
        lookAndFeel->setupToggleButton(*btnInvertFootCtrl);
        labelEXpressionPedalTitle->setColour(Label::ColourIds::textColourId, lookAndFeel->findColour(LumatoneEditorColourIDs::LabelBlue));
          labelExprContrSensivity->setColour(Label::ColourIds::textColourId, lookAndFeel->findColour(LumatoneEditorColourIDs::DescriptionText));
    }
}

void PedalSensitivityDlg::loadFromMapping()
{
	auto mappingInEdit = ((MainContentComponent*)getParentComponent())->getMappingInEdit();

	btnInvertFootCtrl->setToggleState(mappingInEdit.invertFootController, juce::NotificationType::dontSendNotification);
	sldExprCtrlSensivity->setValue(mappingInEdit.expressionControllerSensivity, juce::NotificationType::dontSendNotification);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PedalSensitivityDlg" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="134" initialHeight="96">
  <BACKGROUND backgroundColour="0"/>
  <LABEL name="new label" id="22d529ada4ac7738" memberName="labelExprContrSensivity"
         virtualName="" explicitFocusOrder="0" pos="6 35 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Sensivity:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="btnInvertFootCtrl" id="ef6e332d2b99beda" memberName="btnInvertFootCtrl"
                virtualName="" explicitFocusOrder="0" pos="6 67 162 24" buttonText="Invert Sustain Pedal"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="labelEXpressionPedalTitle" id="afc0b85c8e03b3d6" memberName="labelEXpressionPedalTitle"
         virtualName="" explicitFocusOrder="0" pos="6 3 104 24" textCol="ff61acc8"
         edTextCol="ff000000" edBkgCol="0" labelText="Expression Pedal"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="18.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <SLIDER name="sldExprCtrlSensivity" id="7f6911c9ec82fd65" memberName="sldExprCtrlSensivity"
          virtualName="" explicitFocusOrder="0" pos="77 12 160 72" min="0.0"
          max="127.0" int="1.0" style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

