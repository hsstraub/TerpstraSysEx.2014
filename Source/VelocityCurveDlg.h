/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"

#include "ViewComponents.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VelocityCurveDlg  : public Component,
                          public Button::Listener
{
public:
    //==============================================================================
    VelocityCurveDlg ();
    ~VelocityCurveDlg();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	VelocityCurveDlg(TerpstraKey::KEYTYPE keyTypeValue);
	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);
	void setBeamValue(int pos, int newValue, bool sendToController);
	void setBeamValueAtLeast(int pos, int newValue, bool sendToController);
	void setBeamValueAtMost(int pos, int newValue, bool sendToController);

	void mouseDown(const MouseEvent &event);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	TerpstraKey::KEYTYPE keyType;
	Path internalPath1;
	VelocityCurveBeam* velocityBeamTable[128];

	const float graphicsXPadding = 14.0f;
	const float graphicsYPadding = 46.0f;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Label> lblDescription;
    std::unique_ptr<TextButton> buttonSendAll;
    std::unique_ptr<TextButton> buttonDiscard;
    std::unique_ptr<TextButton> buttonSaveEdits;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VelocityCurveDlg)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

