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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"

#include "VelocityCurveComponents.h"
#include "VelocityCurveEditStrategy.h"
#include "TerpstraMidiDriver.h"

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
An auto-generated component, created by the Projucer.

Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VelocityCurveDlgBase  : public Component,
                              public juce::ComboBox::Listener
{
public:
    //==============================================================================
    VelocityCurveDlgBase (TerpstraMidiDriver::VelocityCurveType typeValue);
    ~VelocityCurveDlgBase() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

	// New mapping is loaded. Display data.
	void loadFromMapping();

	virtual void sendVelocityTableToController();

	bool showBeamValueOfMousePosition(juce::Point<float> localPoint);

	void mouseMove(const MouseEvent &event);
	void mouseDown(const MouseEvent &event);
	void mouseDrag(const MouseEvent &event);
	void mouseUp(const MouseEvent &event);

protected:
	virtual String beamValueText(int beamValue) const { return String(beamValue); }
	virtual String beamXPosText(int xPos) const { return String(xPos); }
	virtual float beamWidth(int xPos) { return (getWidth() - 2.0f * cbEditMode->getX()) / 128.0f; }

	TerpstraKeyMapping*	getMappingInEdit();
	TerpstraVelocityCurveConfig* getConfigInEdit();
	VelocityCurveEditStrategyBase* getCurrentDrawingStrategy();

public:
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	TerpstraMidiDriver::VelocityCurveType velocityCurveType;
	Path beamTableFrame;
	std::unique_ptr<VelocityCurveBeam> velocityBeamTable[128];

	VelocityCurveFreeDrawingStrategy freeDrawingStrategy;
	VelocityCurveLinearDrawingStrategy linearDrawingStrategy;
	VelocityCurveQuadraticDrawingStrategy quadraticDrawingStrategy;
	
	std::map<TerpstraVelocityCurveConfig::EDITSTRATEGYINDEX, VelocityCurveEditStrategyBase*> drawingStrategies;

protected:
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::ComboBox> cbEditMode;
    std::unique_ptr<juce::Label> labelCurrentBeamValue;
    std::unique_ptr<juce::Label> labelCurrentXPos;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VelocityCurveDlgBase)
};

//[EndFile] You can add extra defines here...

//[/EndFile]

