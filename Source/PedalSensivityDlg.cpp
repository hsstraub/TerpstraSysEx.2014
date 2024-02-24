/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.9

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "PedalSensivityDlg.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PedalSensivityDlg::PedalSensivityDlg ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    labelExprContrSensitivity.reset (new juce::Label ("new label",
                                                      TRANS ("Sensitivity")));
    addAndMakeVisible (labelExprContrSensitivity.get());
    labelExprContrSensitivity->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelExprContrSensitivity->setJustificationType (juce::Justification::centredLeft);
    labelExprContrSensitivity->setEditable (false, false, false);
    labelExprContrSensitivity->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelExprContrSensitivity->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelExprContrSensitivity->setBounds (16, 152, 74, 24);

    btnInvertExpression.reset (new juce::ToggleButton ("btnInvertExpression"));
    addAndMakeVisible (btnInvertExpression.get());
    btnInvertExpression->setButtonText (TRANS ("Invert"));
    btnInvertExpression->addListener (this);

    btnInvertExpression->setBounds (8, 32, 99, 24);

    lblExpression.reset (new juce::Label ("lblExpression",
                                          TRANS ("Expression")));
    addAndMakeVisible (lblExpression.get());
    lblExpression->setFont (juce::Font (18.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblExpression->setJustificationType (juce::Justification::centredLeft);
    lblExpression->setEditable (false, false, false);
    lblExpression->setColour (juce::Label::textColourId, juce::Colour (0xff61acc8));
    lblExpression->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblExpression->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblExpression->setBounds (8, 0, 104, 24);

    sldExprCtrlSensitivity.reset (new juce::Slider ("sldExprCtrlSensitivity"));
    addAndMakeVisible (sldExprCtrlSensitivity.get());
    sldExprCtrlSensitivity->setRange (0, 127, 1);
    sldExprCtrlSensitivity->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    sldExprCtrlSensitivity->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    sldExprCtrlSensitivity->addListener (this);

    sldExprCtrlSensitivity->setBounds (-32, 48, 160, 97);

    btnInvertSustain.reset (new juce::ToggleButton ("btnInvertSustain"));
    addAndMakeVisible (btnInvertSustain.get());
    btnInvertSustain->setButtonText (TRANS ("Invert"));
    btnInvertSustain->addListener (this);

    btnInvertSustain->setBounds (96, 32, 86, 24);

    lblSustain.reset (new juce::Label ("lblSustain",
                                       TRANS ("Sustain")));
    addAndMakeVisible (lblSustain.get());
    lblSustain->setFont (juce::Font (18.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblSustain->setJustificationType (juce::Justification::centredLeft);
    lblSustain->setEditable (false, false, false);
    lblSustain->setColour (juce::Label::textColourId, juce::Colour (0xff61acc8));
    lblSustain->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblSustain->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblSustain->setBounds (96, 0, 99, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (180, 112);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

PedalSensivityDlg::~PedalSensivityDlg()
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
void PedalSensivityDlg::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PedalSensivityDlg::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PedalSensivityDlg::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnInvertExpression.get())
    {
        //[UserButtonCode_btnInvertExpression] -- add your button handler code here..
        //[/UserButtonCode_btnInvertExpression]
    }
    else if (buttonThatWasClicked == btnInvertSustain.get())
    {
        //[UserButtonCode_btnInvertSustain] -- add your button handler code here..
        //[/UserButtonCode_btnInvertSustain]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void PedalSensivityDlg::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sldExprCtrlSensitivity.get())
    {
        //[UserSliderCode_sldExprCtrlSensitivity] -- add your slider handling code here..
        //[/UserSliderCode_sldExprCtrlSensitivity]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PedalSensivityDlg" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="180" initialHeight="112">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="new label" id="22d529ada4ac7738" memberName="labelExprContrSensitivity"
         virtualName="" explicitFocusOrder="0" pos="16 152 74 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Sensitivity" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="btnInvertExpression" id="ef6e332d2b99beda" memberName="btnInvertExpression"
                virtualName="" explicitFocusOrder="0" pos="8 32 99 24" buttonText="Invert"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="lblExpression" id="afc0b85c8e03b3d6" memberName="lblExpression"
         virtualName="" explicitFocusOrder="0" pos="8 0 104 24" textCol="ff61acc8"
         edTextCol="ff000000" edBkgCol="0" labelText="Expression" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="18.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="sldExprCtrlSensitivity" id="7f6911c9ec82fd65" memberName="sldExprCtrlSensitivity"
          virtualName="" explicitFocusOrder="0" pos="-32 48 160 97" min="0.0"
          max="127.0" int="1.0" style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="60" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="btnInvertSustain" id="59dfe985e5dfbdca" memberName="btnInvertSustain"
                virtualName="" explicitFocusOrder="0" pos="96 32 86 24" buttonText="Invert"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="lblSustain" id="7582808b917095d4" memberName="lblSustain"
         virtualName="" explicitFocusOrder="0" pos="96 0 99 24" textCol="ff61acc8"
         edTextCol="ff000000" edBkgCol="0" labelText="Sustain" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="18.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

