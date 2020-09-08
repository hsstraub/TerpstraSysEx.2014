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
#include <JuceHeader.h>

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
class VelocityCurveSubDlg  : public Component,
                             public TerpstraMidiDriver::Listener,
                             public ComboBox::Listener
{
public:
    //==============================================================================
    VelocityCurveSubDlg (TerpstraMidiDriver::VelocityCurveType typeValue, int newMaxBeamValue);
    ~VelocityCurveSubDlg() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void restoreStateFromPropertiesString(const String& propertiesString);
	String saveStateToPropertiesString();
	void sendVelocityTableToController();

	virtual void showBeamValueOfMousePosition(juce::Point<float> localPoint);

	void mouseMove(const MouseEvent &event);
	void mouseDown(const MouseEvent &event);
	void mouseDrag(const MouseEvent &event);
	void mouseUp(const MouseEvent &event);

	// Implementation of TerpstraNidiDriver::Listener
	virtual void midiMessageReceived(const MidiMessage& midiMessage) override;
	void midiMessageSent(const MidiMessage& midiMessage) override {}
	void midiSendQueueSize(int queueSize) override {}
    void generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel) override {}
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
protected:
	typedef enum
	{
		none = -1,
		freeDrawing = 0,
		linearSegments = 1,
		quadraticCurves = 2
	} EDITSTRATEGYINDEX;

    const TerpstraMidiDriver::VelocityCurveType velocityCurveType;
    const int maxBeamValue;

	Path beamTableFrame;
	std::unique_ptr<VelocityCurveBeam> velocityBeamTable[128];

	VelocityCurveFreeDrawingStrategy freeDrawingStrategy;
	VelocityCurveLinearDrawingStrategy linearDrawingStrategy;
	VelocityCurveQuadraticDrawingStrategy quadraticDrawingStrategy;
	VelocityCurveEditStrategyBase*	currentCurveEditStrategy;

	const float graphicsYPadding = 16.0f;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Label> lblDescription;
    std::unique_ptr<ComboBox> cbEditMode;
    std::unique_ptr<Label> labelEditMode;
    std::unique_ptr<ComboBox> cbPreset;
    std::unique_ptr<Label> labelPresets;
    std::unique_ptr<Label> labelCurrentBeamValue;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VelocityCurveSubDlg)
};

//[EndFile] You can add extra defines here...

class VelocityIntervalTableSubDlg: public VelocityCurveSubDlg
{
public:
    VelocityIntervalTableSubDlg();

	void sendVelocityTableToController();
	virtual void showBeamValueOfMousePosition(juce::Point<float> localPoint)override;
	virtual void midiMessageReceived(const MidiMessage& midiMessage) override;
};

//[/EndFile]

