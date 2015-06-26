/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "EditFunctionArea.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
EditFunctionArea::EditFunctionArea ()
{
    addAndMakeVisible (tabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtTop));
    tabbedComponent->setTabBarDepth (30);
    tabbedComponent->addTab (TRANS("Assign Notes to Keys"), Colour (0xffbad0de), 0, false);
    tabbedComponent->addTab (TRANS("Macro Buttons"), Colour (0xffbad0de), 0, false);
    tabbedComponent->setCurrentTabIndex (0);

    addAndMakeVisible (noteAndChannelAssGroup = new GroupComponent ("noteAndChannelAssGroup",
                                                                    TRANS("Key Note and Channel Assignment")));

    addAndMakeVisible (editInstructionText = new Label ("editInstructionText",
                                                        TRANS("Assign these values to a key by clicking on the desired key-face.")));
    editInstructionText->setFont (Font (15.00f, Font::plain));
    editInstructionText->setJustificationType (Justification::centredLeft);
    editInstructionText->setEditable (false, false, false);
    editInstructionText->setColour (TextEditor::textColourId, Colours::black);
    editInstructionText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (380, 320);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

EditFunctionArea::~EditFunctionArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    tabbedComponent = nullptr;
    noteAndChannelAssGroup = nullptr;
    editInstructionText = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void EditFunctionArea::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void EditFunctionArea::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    tabbedComponent->setBounds (8, 8, 380, 320);
    noteAndChannelAssGroup->setBounds (8, 48, 370, 200);
    editInstructionText->setBounds (24, 72, 320, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="EditFunctionArea" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="380" initialHeight="320">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <TABBEDCOMPONENT name="new tabbed component" id="7bab997e0e513464" memberName="tabbedComponent"
                   virtualName="" explicitFocusOrder="0" pos="8 8 380 320" orientation="top"
                   tabBarDepth="30" initialTab="0">
    <TAB name="Assign Notes to Keys" colour="ffbad0de" useJucerComp="0"
         contentClassName="" constructorParams="" jucerComponentFile=""/>
    <TAB name="Macro Buttons" colour="ffbad0de" useJucerComp="0" contentClassName=""
         constructorParams="" jucerComponentFile=""/>
  </TABBEDCOMPONENT>
  <GROUPCOMPONENT name="noteAndChannelAssGroup" id="9101ebd18baa947c" memberName="noteAndChannelAssGroup"
                  virtualName="" explicitFocusOrder="0" pos="8 48 370 200" title="Key Note and Channel Assignment"/>
  <LABEL name="editInstructionText" id="60086ae2641b425e" memberName="editInstructionText"
         virtualName="" explicitFocusOrder="0" pos="24 72 320 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Assign these values to a key by clicking on the desired key-face."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
