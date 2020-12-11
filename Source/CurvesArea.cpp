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
#include "NoteOnOffVelocityCurveDialog.h"
#include "ViewConstants.h"
//[/Headers]

#include "CurvesArea.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
CurvesArea::CurvesArea ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    labelWindowTitle.reset (new juce::Label ("labelWindowTitle",
                                             TRANS("Curves")));
    addAndMakeVisible (labelWindowTitle.get());
    labelWindowTitle->setFont (juce::Font (18.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelWindowTitle->setJustificationType (juce::Justification::centredLeft);
    labelWindowTitle->setEditable (false, false, false);
    labelWindowTitle->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelWindowTitle->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelWindowTitle->setBounds (8, 8, 150, 24);

    curvesTab.reset (new juce::TabbedComponent (juce::TabbedButtonBar::TabsAtTop));
    addAndMakeVisible (curvesTab.get());
    curvesTab->setTabBarDepth (30);
    curvesTab->addTab (TRANS("Note Velocity"), juce::Colours::lightgrey, new NoteOnOffVelocityCurveDialog(), true);
    curvesTab->addTab (TRANS("CC Fader"), juce::Colours::lightgrey, new FaderVelocityCurveDialog(), true);
    curvesTab->addTab (TRANS("Aftertouch"), juce::Colours::lightgrey, new AftertouchVelocityCurveDialog(), true);
    curvesTab->setCurrentTabIndex (0);

    curvesTab->setBounds (8, 40, 464, 200);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (472, 240);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

CurvesArea::~CurvesArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    labelWindowTitle = nullptr;
    curvesTab = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void CurvesArea::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void CurvesArea::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	int newWidth = getWidth();
	int newHeight = getHeight();
	int curvesTabRimLeft = labelWindowTitle->getX();
	int curvesTabYPos = labelWindowTitle->getBottom() + CURVETABRIMABOVE;

	int newTabCompWidth = jmax(newWidth - 2 * curvesTabRimLeft, MINIMALCURVESAREAWIDTH);
	int newTabCompHeight = jmax(newHeight - curvesTabYPos, MINIMALCURVESAREAHEIGHT);
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
	curvesTab->setBounds(curvesTabRimLeft, curvesTabYPos, newTabCompWidth, newTabCompHeight);
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void CurvesArea::loadFromMapping()
{
	dynamic_cast<NoteOnOffVelocityCurveDialog*>(curvesTab->getTabContentComponent(curvesTabIndex::noteOnOffVelocityCurve))->loadFromMapping();
	dynamic_cast<FaderVelocityCurveDialog*>(curvesTab->getTabContentComponent(curvesTabIndex::faderConfig))->loadFromMapping();
	dynamic_cast<AftertouchVelocityCurveDialog*>(curvesTab->getTabContentComponent(curvesTabIndex::aftertouchConfig))->loadFromMapping();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="CurvesArea" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="472" initialHeight="240">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="labelWindowTitle" id="2a34b5a9d981827a" memberName="labelWindowTitle"
         virtualName="" explicitFocusOrder="0" pos="8 8 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Curves" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="18.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TABBEDCOMPONENT name="curvesTab" id="795631386b24b3ba" memberName="curvesTab"
                   virtualName="" explicitFocusOrder="0" pos="8 40 464 200" orientation="top"
                   tabBarDepth="30" initialTab="0">
    <TAB name="Note Velocity" colour="ffd3d3d3" useJucerComp="0" contentClassName="NoteOnOffVelocityCurveDialog"
         constructorParams="" jucerComponentFile=""/>
    <TAB name="CC Fader" colour="ffd3d3d3" useJucerComp="0" contentClassName="FaderVelocityCurveDialog"
         constructorParams="" jucerComponentFile=""/>
    <TAB name="Aftertouch" colour="ffd3d3d3" useJucerComp="0" contentClassName="AftertouchVelocityCurveDialog"
         constructorParams="" jucerComponentFile=""/>
  </TABBEDCOMPONENT>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

