/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "ViewConstants.h"
//[/Headers]

#include "KBMMappingDlg.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
KBMMappingDlg::KBMMappingDlg ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    lblChannel.reset (new Label ("lblChannel",
                                 TRANS("MIDI chnl")));
    addAndMakeVisible (lblChannel.get());
    lblChannel->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    lblChannel->setJustificationType (Justification::centredLeft);
    lblChannel->setEditable (false, false, false);
    lblChannel->setColour (TextEditor::textColourId, Colours::black);
    lblChannel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lblChannel->setBounds (8, 8, 72, 24);

    lblMappingFile.reset (new Label ("lblMappingFile",
                                     TRANS("Mapping file")));
    addAndMakeVisible (lblMappingFile.get());
    lblMappingFile->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    lblMappingFile->setJustificationType (Justification::centredLeft);
    lblMappingFile->setEditable (false, false, false);
    lblMappingFile->setColour (TextEditor::textColourId, Colours::black);
    lblMappingFile->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lblMappingFile->setBounds (88, 8, 96, 24);


    //[UserPreSize]
	for (int i = 0; i < 4; i++)
	{
		channelMappingComponents[i].reset(new KBMForOneChannel());
		addAndMakeVisible(channelMappingComponents[i].get());
	}
    //[/UserPreSize]

    setSize (416, 220);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

KBMMappingDlg::~KBMMappingDlg()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    lblChannel = nullptr;
    lblMappingFile = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void KBMMappingDlg::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
	for (int i = 0; i < 4; i++)
		channelMappingComponents[i] = nullptr;
    //[/UserPaint]
}

void KBMMappingDlg::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
 	for (int i = 0; i < 2; i ++)
	{
		channelMappingComponents[i]->setBounds(0, 112 + 40 * i, EDITSUBWINWIDTH / 2, 40);
		channelMappingComponents[i + 2]->setBounds(EDITSUBWINWIDTH / 2, 112 + 40 * i, EDITSUBWINWIDTH / 2, 40);
	}
   //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="KBMMappingDlg" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="416" initialHeight="220">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <LABEL name="lblChannel" id="dda6cf104269f7ea" memberName="lblChannel"
         virtualName="" explicitFocusOrder="0" pos="8 8 72 24" edTextCol="ff000000"
         edBkgCol="0" labelText="MIDI chnl" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblMappingFile" id="3fd88d602009452e" memberName="lblMappingFile"
         virtualName="" explicitFocusOrder="0" pos="88 8 96 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Mapping file" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

