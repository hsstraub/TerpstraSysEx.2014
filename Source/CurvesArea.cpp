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
#include "NoteOnOffVelocityCurveDialog.h"
#include "ViewConstants.h"
#include "Main.h"
//[/Headers]

#include "CurvesArea.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

CurvesArea::CurvesTabComponent::CurvesTabComponent(TabbedButtonBar::Orientation orientation)
	: TabbedComponent::TabbedComponent(orientation)
{
    getTabbedButtonBar().getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, 1.25f);
}

void CurvesArea::CurvesTabComponent::currentTabChanged(int newCurrentTabIndex, const String &newCurrentTabName)
{
	dynamic_cast<VelocityCurveDlgBase*>(getTabContentComponent(newCurrentTabIndex))->loadFromMapping();
}

void CurvesArea::CurvesTabComponent::setTabsIndent(int indentWidthIn)
{
    currentTabIndent = indentWidthIn;
}

void CurvesArea::CurvesTabComponent::resized()
{
    TabbedComponent::resized();
    getTabbedButtonBar().setBounds(currentTabIndent, 0, getWidth() - currentTabIndent, getTabBarDepth());
}

//[/MiscUserDefs]

//==============================================================================
CurvesArea::CurvesArea ()
    : Component("CurvesArea")
{
    //[Constructor_pre] You can add your own custom stuff here..
	showDeveloperMode = TerpstraSysExApplication::getApp().getPropertiesFile()->getBoolValue("DeveloperMode", false);
	//[/Constructor_pre]

    labelWindowTitle.reset (new juce::Label ("labelWindowTitle", translate("Curves")));
    addAndMakeVisible (labelWindowTitle.get());
    labelWindowTitle->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::UniviaProBold));
    labelWindowTitle->setColour (Label::backgroundColourId, Colour());

    curvesTab.reset (new CurvesTabComponent (juce::TabbedButtonBar::TabsAtTop));
    addAndMakeVisible (curvesTab.get());
    curvesTab->setTabBarDepth (30);
    curvesTab->setColour(TabbedComponent::ColourIds::outlineColourId, Colour());
    curvesTab->setColour(TabbedComponent::ColourIds::backgroundColourId, Colour());
    curvesTab->addTab (TRANS("Note Velocity"), Colour(), new NoteOnOffVelocityCurveDialog(), true);
    curvesTab->setCurrentTabIndex (0);

    curvesTab->setBounds (8, 40, 464, 200);

    btnDeveloperMode.reset (new juce::ToggleButton ("btnDeveloperMode"));
    addAndMakeVisible (btnDeveloperMode.get());
    btnDeveloperMode->setButtonText (TRANS("Developer Mode"));
    btnDeveloperMode->addListener (this);

    btnDeveloperMode->setBounds (200, 8, 158, 24);


    //[UserPreSize]
    setDeveloperMode(showDeveloperMode);
    //[/UserPreSize]

    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

CurvesArea::~CurvesArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    labelWindowTitle = nullptr;
    curvesTab = nullptr;
    btnDeveloperMode = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void CurvesArea::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..

    //[/UserPaint]
}

void CurvesArea::resized()
{
    int tabBarDepth = roundToInt(getHeight() * tabDepth);
    int tabY = proportionOfHeight(tabYScalar);

	btnDeveloperMode->setBounds(
		getWidth() - btnDeveloperMode->getWidth(), 
		proportionOfHeight(0.0f), 
		btnDeveloperMode->getWidth(), 
		tabY);

    curvesTab->setTabBarDepth(tabBarDepth);
    curvesTab->setTabsIndent(roundToInt(getWidth() * tabXScalar));
  
    curvesTab->setBounds(0, tabY, getWidth(), getHeight() - tabY);

    resizeLabelWithHeight(labelWindowTitle.get(), tabBarDepth * 0.9f);
    labelWindowTitle->setTopLeftPosition(roundToInt(getWidth() * 0.01f), tabY);
}

void CurvesArea::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnDeveloperMode.get())
    {
        //[UserButtonCode_btnDeveloperMode] -- add your button handler code here..
		if (btnDeveloperMode->getToggleState())
		{
			curvesTab->addTab(TRANS("CC Fader"), juce::Colours::lightgrey, new FaderVelocityCurveDialog(), true);
			curvesTab->addTab(TRANS("Aftertouch"), juce::Colours::lightgrey, new AftertouchVelocityCurveDialog(), true);
			curvesTab->addTab(TRANS("Lumatouch"), juce::Colours::lightgrey, new LumatouchVelocityCurveDialog(), true);
		}
		else
		{
			curvesTab->setCurrentTabIndex(0);
			curvesTab->removeTab(3);
			curvesTab->removeTab(2);
			curvesTab->removeTab(1);
		}
        //[/UserButtonCode_btnDeveloperMode]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void CurvesArea::loadFromMapping()
{
	dynamic_cast<VelocityCurveDlgBase*>(curvesTab->getCurrentContentComponent())->loadFromMapping();
}

void CurvesArea::lookAndFeelChanged()
{
	auto newLookAndFeel = dynamic_cast<LumatoneEditorLookAndFeel*>(&getLookAndFeel());
	if (newLookAndFeel)
	{
		newLookAndFeel->setupToggleButton(*btnDeveloperMode);
	}
}

void CurvesArea::sendConfigToController()
{
	// Send all curves configurations to controller
	// The "developer mode" curve configs (fader, aftertouch, Lumatouch, modulation wheel) are sent only if developer mode is active.
	for (int i = 0; i < curvesTab->getNumTabs(); i++)
	{
		dynamic_cast<VelocityCurveDlgBase*>(curvesTab->getTabContentComponent(i))->sendVelocityTableToController();
	}
}

void CurvesArea::setDeveloperMode(bool devModeOn)
{
    showDeveloperMode = devModeOn;
    btnDeveloperMode->setVisible(showDeveloperMode);
    repaint();
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
                   virtualName="CurvesTabComponent" explicitFocusOrder="0" pos="8 40 464 200"
                   orientation="top" tabBarDepth="30" initialTab="0">
    <TAB name="Note Velocity" colour="ffd3d3d3" useJucerComp="0" contentClassName="NoteOnOffVelocityCurveDialog"
         constructorParams="" jucerComponentFile=""/>
  </TABBEDCOMPONENT>
  <TOGGLEBUTTON name="btnDeveloperMode" id="65b6469bf46b2a64" memberName="btnDeveloperMode"
                virtualName="" explicitFocusOrder="0" pos="200 8 158 24" buttonText="Developer Mode"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

