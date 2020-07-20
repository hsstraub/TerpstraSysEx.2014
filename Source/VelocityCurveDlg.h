/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

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
                          public Button::Listener,
                          public ComboBox::Listener
{
public:
    //==============================================================================
    VelocityCurveDlg (TerpstraMidiDriver::VelocityCurveType typeValue);
    ~VelocityCurveDlg() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);
	void sendVelocityTableToController();

	void showBeamValueOfMousePosition(juce::Point<float> localPoint);

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
	typedef enum
	{
		none = -1,
		freeDrawing = 0,
		linearSegments = 1,
		quadraticCurves = 2
	} EDITSTRATEGYINDEX;


	TerpstraMidiDriver::VelocityCurveType velocityCurveType;
	Path beamTableFrame;
	VelocityCurveBeam* velocityBeamTable[128];

	VelocityCurveFreeDrawingStrategy freeDrawingStrategy;
	VelocityCurveLinearDrawingStrategy linearDrawingStrategy;
	VelocityCurveQuadraticDrawingStrategy quadraticDrawingStrategy;
	VelocityCurveEditStrategyBase*	currentCurveEditStrategy;

	const float graphicsYPadding = 116.0f;
	const float pushButtonAreaHeight = 36.0f;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Label> lblDescription;
    std::unique_ptr<TextButton> buttonSendAll;
    std::unique_ptr<TextButton> buttonDiscard;
    std::unique_ptr<TextButton> buttonSaveEdits;
    std::unique_ptr<ComboBox> cbEditMode;
    std::unique_ptr<Label> labelEditMode;
    std::unique_ptr<ComboBox> cbPreset;
    std::unique_ptr<Label> labelPresets;
    std::unique_ptr<Label> labelCurrentBeamValue;
    std::unique_ptr<TextButton> buttonReceive;
    std::unique_ptr<TextButton> buttonCalibrate;
    std::unique_ptr<ToggleButton> buttonAfterTouchActive;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VelocityCurveDlg)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

