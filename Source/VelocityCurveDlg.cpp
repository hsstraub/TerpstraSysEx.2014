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
                                                   TRANS("Click with the mouse in the graphics to draw the velocity curve.")));
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

    setSize (640, 320);


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

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void VelocityCurveDlg::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	int w = this->getWidth();
	int h = this->getHeight();
    //[/UserPreResize]

    lblDescription->setBounds (8, 8, 416, 32);
    //[UserResized] Add your own custom resize handling here..

	Path internalPath1;
	internalPath1.startNewSubPath(graphicsXPadding, graphicsYPadding);
	internalPath1.lineTo(graphicsXPadding, h - graphicsYPadding);
	internalPath1.lineTo(w - graphicsXPadding, h - graphicsYPadding);
	internalPath1.lineTo(w - graphicsXPadding, graphicsYPadding);
	internalPath1.closeSubPath();

	float velocityBeamWidth = (w - 2 * graphicsXPadding) / 128;
	float velocityGraphicsHeight = h - 2 * graphicsYPadding;

	for (int x = 0; x < 128; x++)
	{
		velocityBeamTable[x]->setBounds(
			graphicsXPadding + x*velocityBeamWidth,
			graphicsYPadding,
			velocityBeamWidth,
			velocityGraphicsHeight);
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
	int h = this->getHeight();
	float velocityGraphicsHeight = h - 2 * graphicsYPadding;

	for (int x = 0; x < 128; x++)
	{
		if (event.eventComponent == velocityBeamTable[x] || event.eventComponent->getParentComponent() == velocityBeamTable[x])
		{
			int newBeamValue = (velocityGraphicsHeight - event.getMouseDownY()) * 128 / velocityGraphicsHeight;
			velocityBeamTable[x]->setValue(newBeamValue);

			// Change other beams' values so curve stays monotonous
			for(int x2 = 0; x2 < x; x2++)
				velocityBeamTable[x2]->setValueAtMost(newBeamValue);

			for (int x2 = x+1; x2 < 128; x2++)
				velocityBeamTable[x2]->setValueAtLeast(newBeamValue);

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
                 fixedSize="1" initialWidth="640" initialHeight="320">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <LABEL name="lblDescription" id="e1affcc7a142cab2" memberName="lblDescription"
         virtualName="" explicitFocusOrder="0" pos="8 8 416 32" edTextCol="ff000000"
         edBkgCol="0" labelText="Click with the mouse in the graphics to draw the velocity curve."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
