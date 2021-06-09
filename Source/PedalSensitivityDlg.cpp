/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.8

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

    labelExprContrSensitivity.reset (new juce::Label ("new label",
                                                    TRANS("Sensitivity:")));
    addAndMakeVisible (labelExprContrSensitivity.get());
    labelExprContrSensitivity->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelExprContrSensitivity->setJustificationType (juce::Justification::centredLeft);
    labelExprContrSensitivity->setEditable (false, false, false);
    labelExprContrSensitivity->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelExprContrSensitivity->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelExprContrSensitivity->setBounds (6, 35, 64, 24);

    btnInvertExpression.reset (new juce::ToggleButton ("btnInvertExpression"));
    addAndMakeVisible (btnInvertExpression.get());
    btnInvertExpression->setButtonText (TRANS("Invert Expression"));
    btnInvertExpression->addListener (this);

    btnInvertExpression->setBounds (8, 96, 162, 24);

    labelPedalTitle.reset (new juce::Label ("labelPedalTitle",
                                            TRANS("Pedal Settings")));
    addAndMakeVisible (labelPedalTitle.get());
    labelPedalTitle->setFont (juce::Font (18.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelPedalTitle->setJustificationType (juce::Justification::centredLeft);
    labelPedalTitle->setEditable (false, false, false);
    labelPedalTitle->setColour (juce::Label::textColourId, juce::Colour (0xff61acc8));
    labelPedalTitle->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelPedalTitle->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelPedalTitle->setBounds (6, 3, 104, 24);

    sldExprCtrlSensitivity.reset (new juce::Slider ("sldExprCtrlSensitivity"));
    addAndMakeVisible (sldExprCtrlSensitivity.get());
    sldExprCtrlSensitivity->setRange (0, 127, 1);
    sldExprCtrlSensitivity->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    sldExprCtrlSensitivity->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 60, 20);
    sldExprCtrlSensitivity->addListener (this);

    sldExprCtrlSensitivity->setBounds (77, 12, 160, 72);

    btnInvertSustain.reset (new juce::ToggleButton ("btnInvertSustain"));
    addAndMakeVisible (btnInvertSustain.get());
    btnInvertSustain->setButtonText (TRANS("Invert Sustain"));
    btnInvertSustain->addListener (this);

    btnInvertSustain->setBounds (8, 72, 150, 24);


    //[UserPreSize]
    labelPedalTitle->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::UniviaProBold));
    labelExprContrSensitivity->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::GothamNarrowMedium));
    //[/UserPreSize]

    setSize (134, 96);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

PedalSensitivityDlg::~PedalSensitivityDlg()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    labelExprContrSensitivity = nullptr;
    btnInvertExpression = nullptr;
    labelPedalTitle = nullptr;
    sldExprCtrlSensitivity = nullptr;
    btnInvertSustain = nullptr;


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

    resizeLabelWithHeight(labelPedalTitle.get(), roundToInt(getHeight() * SETTINGSLABELHEIGHT));
    labelPedalTitle->setTopLeftPosition(roundToInt(getWidth() * SETTINGSLABELMARGINWIDTH), 0);

    int marginX = roundToInt(getParentWidth() * SETTINGSCONTROLMARGINTOAPPWIDTH);
    int buttonHeight = roundToInt(h * SETTINGSTOGGLEHEIGHTSCALAR);
    btnInvertExpression->setBounds(marginX, proportionOfHeight(0.3f), w, buttonHeight);
    btnInvertSustain->setBounds(marginX, proportionOfHeight(0.5), w, buttonHeight);

    sldExprCtrlSensitivity->setBounds(getLocalBounds().toFloat().getProportion(sliderBoundsProps).toNearestInt());

    resizeLabelWithHeight(labelExprContrSensitivity.get(), buttonHeight * 1.2f, 1.0f, "");
    labelExprContrSensitivity->setCentrePosition(sldExprCtrlSensitivity->getBounds().getCentreX(), btnInvertExpression->getBounds().getCentreY());
    //[/UserResized]
}

void PedalSensitivityDlg::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnInvertExpression.get())
    {
        //[UserButtonCode_btnInvertExpression] -- add your button handler code here..
        bool invert = btnInvertExpression->getToggleState();
        ((MainContentComponent*)getParentComponent())->getMappingInEdit().invertExpression = invert;
        TerpstraSysExApplication::getApp().setHasChangesToSave(true);
        TerpstraSysExApplication::getApp().getLumatoneController().sendInvertFootController(invert);
        //[/UserButtonCode_btnInvertExpression]
    }
    else if (buttonThatWasClicked == btnInvertSustain.get())
    {
        //[UserButtonCode_btnInvertSustain] -- add your button handler code here..
        bool invert = btnInvertSustain->getToggleState();
        ((MainContentComponent*)getParentComponent())->getMappingInEdit().invertSustain = invert;
        TerpstraSysExApplication::getApp().setHasChangesToSave(true);
        TerpstraSysExApplication::getApp().getLumatoneController().invertSustainPedal(invert);
        //[/UserButtonCode_btnInvertSustain]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void PedalSensitivityDlg::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sldExprCtrlSensitivity.get())
    {
        //[UserSliderCode_sldExprCtrlSensivity] -- add your slider handling code here..
        int newSensitvity = sldExprCtrlSensitivity->getValue();
        // ToDo value checking: encapsulate in keyboard data structure?
        if (newSensitvity < 0)
        {
            newSensitvity = 0;
            sldExprCtrlSensitivity->setValue(newSensitvity);
        }

        if (newSensitvity > 0x7f)
        {
            newSensitvity = 0x7f;
            sldExprCtrlSensitivity->setValue(newSensitvity);
        }

        ((MainContentComponent*)getParentComponent())->getMappingInEdit().expressionControllerSensivity = newSensitvity;
        TerpstraSysExApplication::getApp().setHasChangesToSave(true);
        TerpstraSysExApplication::getApp().getLumatoneController().sendExpressionPedalSensivity(newSensitvity);
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
    auto newLookAndFeel = dynamic_cast<LumatoneEditorLookAndFeel*>(&getLookAndFeel());
    if (newLookAndFeel)
    {
        labelPedalTitle->setColour(Label::ColourIds::textColourId, newLookAndFeel->findColour(LumatoneEditorColourIDs::LabelBlue));
        labelExprContrSensitivity->setColour(Label::ColourIds::textColourId, newLookAndFeel->findColour(LumatoneEditorColourIDs::DescriptionText));
    }
}

void PedalSensitivityDlg::loadFromMapping()
{
	auto mappingInEdit = ((MainContentComponent*)getParentComponent())->getMappingInEdit();

	btnInvertExpression->setToggleState(mappingInEdit.invertExpression, juce::NotificationType::dontSendNotification);
    btnInvertSustain->setToggleState(mappingInEdit.invertSustain, juce::NotificationType::dontSendNotification);
	sldExprCtrlSensitivity->setValue(mappingInEdit.expressionControllerSensivity, juce::NotificationType::dontSendNotification);
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
  <LABEL name="new label" id="22d529ada4ac7738" memberName="labelExprContrSensitivity"
         virtualName="" explicitFocusOrder="0" pos="6 35 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Sensitivity:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="btnInvertExpression" id="ef6e332d2b99beda" memberName="btnInvertExpression"
                virtualName="" explicitFocusOrder="0" pos="8 96 162 24" buttonText="Invert Expression"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="labelPedalTitle" id="afc0b85c8e03b3d6" memberName="labelPedalTitle"
         virtualName="" explicitFocusOrder="0" pos="6 3 104 24" textCol="ff61acc8"
         edTextCol="ff000000" edBkgCol="0" labelText="Pedal Settings"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="18.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <SLIDER name="sldExprCtrlSensitivity" id="7f6911c9ec82fd65" memberName="sldExprCtrlSensitivity"
          virtualName="" explicitFocusOrder="0" pos="77 12 160 72" min="0.0"
          max="127.0" int="1.0" style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="btnInvertSustain" id="59dfe985e5dfbdca" memberName="btnInvertSustain"
                virtualName="" explicitFocusOrder="0" pos="8 72 150 24" buttonText="Invert Sustain"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

