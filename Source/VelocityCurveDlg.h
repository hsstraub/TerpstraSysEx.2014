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

#ifndef __JUCE_HEADER_8FE00A31FE0B8946__
#define __JUCE_HEADER_8FE00A31FE0B8946__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"

#include "VelocityCurveComponents.h"
#include "VelocityCurveEditStrategy.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VelocityCurveDlg  : public Component,
                          public ButtonListener,
                          public ComboBoxListener
{
public:
    //==============================================================================
    VelocityCurveDlg (TerpstraKey::KEYTYPE keyTypeValue);
    ~VelocityCurveDlg();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);
	void sendVelocityTableToController();

	void showBeamValueOfMousePosition(Point<float> localPoint);

	void mouseMove(const MouseEvent &event);
	void mouseDown(const MouseEvent &event);
	void mouseDrag(const MouseEvent &event);
	void mouseUp(const MouseEvent &event);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	TerpstraKey::KEYTYPE keyType;
	Path beamTableFrame;
	VelocityCurveBeam* velocityBeamTable[128];

	VelocityCurveFreeDrawingStrategy freeDrawingStrategy;
	VelocityCurveLinearDrawingStrategy linearDrawingStrategy;
	VelocityCurveEditStrategyBase*	currentCurveEditStrategy;

	const float graphicsYPadding = 116.0f;
	const float pushButtonAreaHeight = 36.0f;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> lblDescription;
    ScopedPointer<TextButton> buttonSendAll;
    ScopedPointer<TextButton> buttonDiscard;
    ScopedPointer<TextButton> buttonSaveEdits;
    ScopedPointer<ComboBox> cbEditMode;
    ScopedPointer<Label> labelEditMode;
    ScopedPointer<ComboBox> cbPreset;
    ScopedPointer<Label> labelPresets;
    ScopedPointer<Label> labelCurrentBeamValue;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VelocityCurveDlg)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_8FE00A31FE0B8946__
