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

    labelGeneralSettingslTitle.reset (new juce::Label ("labelGeneralSettingslTitle",
                                                       TRANS ("General Settings")));
    addAndMakeVisible (labelGeneralSettingslTitle.get());
    labelGeneralSettingslTitle->setFont (juce::Font (18.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelGeneralSettingslTitle->setJustificationType (juce::Justification::centredLeft);
    labelGeneralSettingslTitle->setEditable (false, false, false);
    labelGeneralSettingslTitle->setColour (juce::Label::textColourId, juce::Colour (0xff61acc8));
    labelGeneralSettingslTitle->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelGeneralSettingslTitle->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelGeneralSettingslTitle->setBounds (8, 0, 104, 24);

    buttonAfterTouchActive.reset (new juce::ToggleButton ("buttonAfterTouchActive"));
    addAndMakeVisible (buttonAfterTouchActive.get());
    buttonAfterTouchActive->setButtonText (TRANS ("Polyphonic Aftertouch"));
    buttonAfterTouchActive->addListener (this);

    buttonAfterTouchActive->setBounds (8, 32, 176, 24);

    buttonLightOnKeyStrokes.reset (new juce::ToggleButton ("buttonLightOnKeyStrokes"));
    addAndMakeVisible (buttonLightOnKeyStrokes.get());
    buttonLightOnKeyStrokes->setButtonText (TRANS ("Light on Keystrokes"));
    buttonLightOnKeyStrokes->addListener (this);

    buttonLightOnKeyStrokes->setBounds (8, 64, 176, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (188, 148);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

GeneralOptionsDlg::~GeneralOptionsDlg()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    labelGeneralSettingslTitle = nullptr;
    buttonAfterTouchActive = nullptr;
    buttonLightOnKeyStrokes = nullptr;


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

    if (buttonThatWasClicked == buttonAfterTouchActive.get())
    {
        //[UserButtonCode_buttonAfterTouchActive] -- add your button handler code here..
		((MainContentComponent*)getParentComponent())->getMappingInEdit().afterTouchActive = buttonAfterTouchActive->getToggleState();
		TerpstraSysExApplication::getApp().setHasChangesToSave(true);
		TerpstraSysExApplication::getApp().getMidiDriver().sendAfterTouchActivation(buttonAfterTouchActive->getToggleState());
        //[/UserButtonCode_buttonAfterTouchActive]
    }
    else if (buttonThatWasClicked == buttonLightOnKeyStrokes.get())
    {
        //[UserButtonCode_buttonLightOnKeyStrokes] -- add your button handler code here..
		((MainContentComponent*)getParentComponent())->getMappingInEdit().lightOnKeyStrokes = buttonLightOnKeyStrokes->getToggleState();
		TerpstraSysExApplication::getApp().setHasChangesToSave(true);
		TerpstraSysExApplication::getApp().getMidiDriver().sendLightOnKeyStrokes(buttonLightOnKeyStrokes->getToggleState());
        //[/UserButtonCode_buttonLightOnKeyStrokes]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void GeneralOptionsDlg::loadFromMapping()
{
	auto mappingInEdit = ((MainContentComponent*)getParentComponent())->getMappingInEdit();

	buttonAfterTouchActive->setToggleState(mappingInEdit.afterTouchActive, juce::NotificationType::dontSendNotification);
	buttonLightOnKeyStrokes->setToggleState(mappingInEdit.lightOnKeyStrokes, juce::NotificationType::dontSendNotification);

}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GeneralOptionsDlg" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="188" initialHeight="148">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <LABEL name="labelGeneralSettingslTitle" id="9ac4ac959d4cd11a" memberName="labelGeneralSettingslTitle"
         virtualName="" explicitFocusOrder="0" pos="8 0 104 24" textCol="ff61acc8"
         edTextCol="ff000000" edBkgCol="0" labelText="General Settings"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="18.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <TOGGLEBUTTON name="buttonAfterTouchActive" id="3f2eba6027c4f2f5" memberName="buttonAfterTouchActive"
                virtualName="" explicitFocusOrder="0" pos="8 32 176 24" buttonText="Polyphonic Aftertouch"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="buttonLightOnKeyStrokes" id="80710852cfd98f27" memberName="buttonLightOnKeyStrokes"
                virtualName="" explicitFocusOrder="0" pos="8 64 176 24" buttonText="Light on Keystrokes"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

