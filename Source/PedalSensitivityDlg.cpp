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
#include "EditActions.h"
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
                                                      TRANS("Sensitivity")));
    addAndMakeVisible (labelExprContrSensitivity.get());
    labelExprContrSensitivity->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelExprContrSensitivity->setJustificationType (juce::Justification::centredLeft);
    labelExprContrSensitivity->setEditable (false, false, false);
    labelExprContrSensitivity->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelExprContrSensitivity->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelExprContrSensitivity->setBounds (13, 152, 74, 24);

    btnInvertExpression.reset (new juce::ToggleButton ("btnInvertExpression"));
    addAndMakeVisible (btnInvertExpression.get());
    btnInvertExpression->setButtonText (TRANS("Invert"));
    btnInvertExpression->addListener (this);

    btnInvertExpression->setBounds (10, 32, 99, 24);

    lblExpression.reset (new juce::Label ("lblExpression",
                                          TRANS("Expression")));
    addAndMakeVisible (lblExpression.get());
    lblExpression->setFont (juce::Font (18.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblExpression->setJustificationType (juce::Justification::centredLeft);
    lblExpression->setEditable (false, false, false);
    lblExpression->setColour (juce::Label::textColourId, juce::Colour (0xff61acc8));
    lblExpression->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblExpression->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblExpression->setBounds (6, 3, 104, 24);

    sldExprCtrlSensitivity.reset (new juce::Slider ("sldExprCtrlSensitivity"));
    addAndMakeVisible (sldExprCtrlSensitivity.get());
    sldExprCtrlSensitivity->setRange (0, 127, 1);
    sldExprCtrlSensitivity->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    sldExprCtrlSensitivity->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    sldExprCtrlSensitivity->addListener (this);

    sldExprCtrlSensitivity->setBounds (-32, 49, 160, 97);

    btnInvertSustain.reset (new juce::ToggleButton ("btnInvertSustain"));
    addAndMakeVisible (btnInvertSustain.get());
    btnInvertSustain->setButtonText (TRANS("Invert"));
    btnInvertSustain->addListener (this);

    btnInvertSustain->setBounds (125, 32, 86, 24);

    lblSustain.reset (new juce::Label ("lblSustain",
                                       TRANS("Sustain")));
    addAndMakeVisible (lblSustain.get());
    lblSustain->setFont (juce::Font (18.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblSustain->setJustificationType (juce::Justification::centredLeft);
    lblSustain->setEditable (false, false, false);
    lblSustain->setColour (juce::Label::textColourId, juce::Colour (0xff61acc8));
    lblSustain->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblSustain->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblSustain->setBounds (121, 3, 99, 24);


    //[UserPreSize]
    lblExpression->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::UniviaProBold));
    lblSustain->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::UniviaProBold));
    labelExprContrSensitivity->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::GothamNarrowMedium));
    labelExprContrSensitivity->setJustificationType(Justification::centred);
    TerpstraSysExApplication::getApp().getLumatoneController()->addFirmwareListener(this);
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
    lblExpression = nullptr;
    sldExprCtrlSensitivity = nullptr;
    btnInvertSustain = nullptr;
    lblSustain = nullptr;


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
	g.fillRoundedRectangle(expressionBounds, roundedCornerSize);
    g.fillRoundedRectangle(sustainBounds, roundedCornerSize);
    //[/UserPaint]
}

void PedalSensitivityDlg::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    float w = getWidth();
    float h = getHeight();
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..

    roundedCornerSize = roundToInt(getParentHeight() * ROUNDEDCORNERTOAPPHEIGHT);

    int areaMarginWidth = roundToInt(w * sectionMarginWidth) * 0.5f;
    int areaMarginHeight = roundToInt(h * SETTINGSAREAMARGINHEIGHT);

    expressionBounds = getLocalBounds().toFloat().withTop(areaMarginHeight).withRight(roundToInt(w * 0.5f - areaMarginWidth));
    sustainBounds = getLocalBounds().toFloat().withTop(areaMarginHeight).withLeft(roundToInt(w * 0.5f + areaMarginWidth));

    int lblMarginX = roundToInt(w * SETTINGSLABELMARGINWIDTH);
    int lblWidth = roundToInt(w * 0.5f);
    int lblHeight = roundToInt(h * SETTINGSLABELHEIGHT);
    lblExpression->setBounds(expressionBounds.getX() + lblMarginX, 0, lblWidth, lblHeight);
    lblSustain->setBounds(sustainBounds.getX() + lblMarginX, 0, lblWidth, lblHeight);

    int controlMargin = roundToInt(getParentWidth() * SETTINGSCONTROLMARGINTOAPPWIDTH);
    int buttonHeight = roundToInt(h * SETTINGSTOGGLEHEIGHTSCALAR);
    int buttonY = roundToInt(h * 0.3f);
    btnInvertExpression->setBounds(expressionBounds.withTrimmedLeft(controlMargin).withTop(buttonY).withHeight(buttonHeight).toNearestInt());
    btnInvertSustain->setBounds(sustainBounds.withTrimmedLeft(controlMargin).withTop(buttonY).withHeight(buttonHeight).toNearestInt());

    sldExprCtrlSensitivity->setBounds(
          expressionBounds.reduced(expressionBounds.getWidth() * 0.2f, 0)
                          .withTop(btnInvertExpression->getBottom() + buttonHeight)
                          .withTrimmedBottom(buttonHeight * 1.5f)
                          .toNearestInt()
    );

    labelExprContrSensitivity->setBounds(
         expressionBounds.withTop(sldExprCtrlSensitivity->getBottom() + buttonHeight * 0.1f)
                         .withTrimmedBottom(buttonHeight * 0.5f)
                         .toNearestInt()
    );
    //[/UserResized]
}

void PedalSensitivityDlg::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnInvertExpression.get())
    {
        //[UserButtonCode_btnInvertExpression] -- add your button handler code here..
		TerpstraSysExApplication::getApp().performUndoableAction(new Lumatone::InvertFootControllerEditAction(btnInvertExpression->getToggleState()));
        //[/UserButtonCode_btnInvertExpression]
    }
    else if (buttonThatWasClicked == btnInvertSustain.get())
    {
        //[UserButtonCode_btnInvertSustain] -- add your button handler code here..
        TerpstraSysExApplication::getApp().performUndoableAction(new Lumatone::InvertSustainEditAction(btnInvertSustain->getToggleState()));
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
        //[UserSliderCode_sldExprCtrlSensitivity] -- add your slider handling code here..
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
        TerpstraSysExApplication::getApp().getLumatoneController()->sendExpressionPedalSensivity(newSensitvity);

        //[/UserSliderCode_sldExprCtrlSensitivity]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void PedalSensitivityDlg::lookAndFeelChanged()
{
    auto newLookAndFeel = dynamic_cast<LumatoneEditorLookAndFeel*>(&getLookAndFeel());
    if (newLookAndFeel)
    {
        lblExpression->setColour(Label::ColourIds::textColourId, newLookAndFeel->findColour(LumatoneEditorColourIDs::LabelBlue));
        lblSustain->setColour(Label::ColourIds::textColourId, newLookAndFeel->findColour(LumatoneEditorColourIDs::LabelBlue));
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

void PedalSensitivityDlg::firmwareRevisionReceived(FirmwareVersion version)
{
    FirmwareSupport firmwareSupport;

    if (firmwareSupport.versionAcknowledgesCommand(version, INVERT_SUSTAIN_PEDAL))
    {
        btnInvertSustain->setEnabled(true);
        btnInvertSustain->setTooltip("");
    }
    else
    {
        btnInvertSustain->setEnabled(false);
        btnInvertSustain->setTooltip("This feature is not supported by the firmware version of your Lumatone.");
    }
}

void PedalSensitivityDlg::presetFlagsReceived(PresetFlags presetFlags)
{
    btnInvertExpression->setToggleState(presetFlags.expressionPedalInverted, dontSendNotification);
    btnInvertSustain->setToggleState(presetFlags.sustainPedalInverted, dontSendNotification);
}

void PedalSensitivityDlg::expressionPedalSensitivityReceived(int sensitivity)
{
    sldExprCtrlSensitivity->setValue(sensitivity, dontSendNotification);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PedalSensitivityDlg" componentName=""
                 parentClasses="public juce::Component, public LumatoneController::FirmwareListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="134"
                 initialHeight="96">
  <BACKGROUND backgroundColour="0"/>
  <LABEL name="new label" id="22d529ada4ac7738" memberName="labelExprContrSensitivity"
         virtualName="" explicitFocusOrder="0" pos="13 152 74 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Sensitivity" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="btnInvertExpression" id="ef6e332d2b99beda" memberName="btnInvertExpression"
                virtualName="" explicitFocusOrder="0" pos="10 32 99 24" buttonText="Invert"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="lblExpression" id="afc0b85c8e03b3d6" memberName="lblExpression"
         virtualName="" explicitFocusOrder="0" pos="6 3 104 24" textCol="ff61acc8"
         edTextCol="ff000000" edBkgCol="0" labelText="Expression" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="18.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sldExprCtrlSensitivity" id="7f6911c9ec82fd65" memberName="sldExprCtrlSensitivity"
          virtualName="" explicitFocusOrder="0" pos="-32 49 160 97" min="0.0"
          max="127.0" int="1.0" style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="btnInvertSustain" id="59dfe985e5dfbdca" memberName="btnInvertSustain"
                virtualName="" explicitFocusOrder="0" pos="125 32 86 24" buttonText="Invert"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="lblSustain" id="7582808b917095d4" memberName="lblSustain"
         virtualName="" explicitFocusOrder="0" pos="121 3 99 24" textCol="ff61acc8"
         edTextCol="ff000000" edBkgCol="0" labelText="Sustain" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="18.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

