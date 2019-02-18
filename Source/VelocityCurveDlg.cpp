/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "Main.h"
//[/Headers]

#include "VelocityCurveDlg.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VelocityCurveDlg::VelocityCurveDlg ()
{
    //[Constructor_pre] You can add your own custom stuff here..
	//[/Constructor_pre]

    addAndMakeVisible (lblDescription = new Label ("lblDescription",
                                                   TRANS("Draw velocity curve with mouse.")));
    lblDescription->setFont (Font (15.00f, Font::plain));
    lblDescription->setJustificationType (Justification::centredLeft);
    lblDescription->setEditable (false, false, false);
    lblDescription->setColour (TextEditor::textColourId, Colours::black);
    lblDescription->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]

	for (int x = 0; x < 128; x++)
	{
		velocityBeamTable[x] = new  VelocityCurveBeam();
		addAndMakeVisible(velocityBeamTable[x]);
		velocityBeamTable[x]->addMouseListener(this, true);
	}

	//[/UserPreSize]

    setSize (428, 220);


    //[Constructor] You can add your own custom stuff here..
  
	// Set values according to the properties files
	restoreStateFromPropertiesFile(TerpstraSysExApplication::getApp().getPropertiesFile());

	//[/Constructor]
}

VelocityCurveDlg::~VelocityCurveDlg()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	// Save values to properties file
	saveStateToPropertiesFile(TerpstraSysExApplication::getApp().getPropertiesFile());
	//[/Destructor_pre]

    lblDescription = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	deleteAllChildren();
	//[/Destructor]
}

//==============================================================================
void VelocityCurveDlg::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    g.setColour (Colour (0xffbad0de));
    g.fillPath (internalPath1);
    g.setColour (Colours::black);
    g.strokePath (internalPath1, PathStrokeType (1.000f));

    //[UserPaint] Add your own custom painting code here..
	//[/UserPaint]
}

void VelocityCurveDlg::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	float w = this->getWidth();
	float h = this->getHeight();
	//[/UserPreResize]

    lblDescription->setBounds (8, 8, 416, 32);
    internalPath1.clear();
    internalPath1.startNewSubPath (14.0f, 181.0f);
    internalPath1.lineTo (14.0f, 46.0f);
    internalPath1.lineTo (252.0f, 46.0f);
    internalPath1.lineTo (254.0f, 182.0f);
    internalPath1.closeSubPath();

    //[UserResized] Add your own custom resize handling here..

	Rectangle<float> velocityTableRect = internalPath1.getBounds();
	Point<float> velocityTableRectTopLeft = velocityTableRect.getTopLeft();

	float velocityBeamWidth = velocityTableRect.getWidth() / 128;

	for (int x = 0; x < 128; x++)
	{
		velocityBeamTable[x]->setBounds(
			velocityTableRectTopLeft.x + x*velocityBeamWidth,
			velocityTableRectTopLeft.y,
			velocityBeamWidth,
			velocityTableRect.getHeight());
	}
	//[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void VelocityCurveDlg::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	// XXX Read from propertiesFile

	// XXX ad hoc Initialize velocity lookup table
	for (int x = 0; x < 128; x++)
		velocityBeamTable[x]->setValue(x);
}

void VelocityCurveDlg::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
}

void VelocityCurveDlg::mouseDown(const MouseEvent &event)
{
	for (int x = 0; x < 128; x++)
	{
		if (event.eventComponent == velocityBeamTable[x] || event.eventComponent->getParentComponent() == velocityBeamTable[x])
		{
			// XXX
			break;
		}
	}
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VelocityCurveDlg" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="428" initialHeight="220">
  <BACKGROUND backgroundColour="ffbad0de">
    <PATH pos="0 0 100 100" fill="solid: ffbad0de" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff000000" nonZeroWinding="1">s 14 180.71 l 14 46 l 252.37 46 l 254 182 x</PATH>
  </BACKGROUND>
  <LABEL name="lblDescription" id="e1affcc7a142cab2" memberName="lblDescription"
         virtualName="" explicitFocusOrder="0" pos="8 8 416 32" edTextCol="ff000000"
         edBkgCol="0" labelText="Draw velocity curve with mouse." editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
