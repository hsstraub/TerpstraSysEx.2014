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
#include "MappingLogic.h"
#include "IncrMidiNotesMapping.h"
#include "KBMMappingDlg.h"
#include "BoardGeometry.h"
#include "ScaleStructureController/ScaleStructureComponent.h"
#include "ScaleStructureController/ScaleDesignWindow.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class IsomorphicMassAssign  : public Component,
                              public MappingLogicBase::Listener,
                              public ScaleStructureComponent::Listener,
                              public juce::ComboBox::Listener,
                              public juce::Button::Listener
{
public:
    //==============================================================================
    IsomorphicMassAssign ();
    ~IsomorphicMassAssign() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	// Implementation of MappingLogicListener
	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);

	// Set, save and maybe send data of one key
	void setSaveSend(int setSelection, int keySelection, int noteIndex);

	// Fill a line, Starting point is assumed to have been set
	void fillLine(int setSelection, TerpstraBoardGeometry::StraightLine& line, int startPos, int startNoteIndex, int stepSize);

	// Fill a line over all octave boards. Starting point is assumed to have been set.
	void fillGlobalLine(int setSelection, TerpstraBoardGeometry::StraightLineSet& globalLine, int startPos, int startNoteIndex, int stepSize);

	// Fill a horizontal line and its cutting upwards lines, recursively
	void fill2DHorizLineRecursive(int setSelection, TerpstraBoardGeometry::StraightLine& horizLine, int startPos, int startNoteIndex,
		int horizStepSize, int rUpwStepSize,
		TerpstraBoardGeometry::StraightLineSet& finishedLines);

	// Fill a right upward line and its cutting horizontal lines, recursively
	void fill2DRUpwLineRecursive(int setSelection, TerpstraBoardGeometry::StraightLine& rUpwLine, int startPos, int startNoteIndex,
		int horizStepSize, int rUpwStepSize,
		TerpstraBoardGeometry::StraightLineSet& finishedLines);

	// Implementation of MappingLogicListener
	void mappingLogicChanged(MappingLogicBase* mappingLogicThatChanged) override;

	// Implementation of ScaleStructureComponent::Listener
	void scaleStructurePeriodChanged(int newPeriod) override;
	void scaleStructureStepSizesChanged(int rightUpwardSize, int horizontalSize) override;

	bool performMouseDown(int setSelection, int keySelection);

	void lookAndFeelChanged() override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	std::unique_ptr<IncrMidiNotesMapping>	incrMidiNotesMapping;
	std::unique_ptr<KBMMappingDlg>          kbmMappingDlg;
	std::unique_ptr<ScaleDesignWindow>		scaleDesignWindow;

	MappingLogicBase*			mappingLogic;
	int							periodSize;
	TerpstraBoardGeometry		boardGeometry;
	ScaleStructure				scaleStructure;

	// Style helpers
	Array<Component*> flexBoxComponents;
	FlexBox          flexBox;

	Rectangle<int> instructionsBounds;
	Font           instructionsFont;

	// Can be replaced, but ScaleStructureComponent currently needs a reference to a list of Colours
	// to use and update them automatically in realtime
	Array<Colour>	colourTable =
	{
		Colours::white,
		Colours::red,
		Colours::blue,
		Colours::yellow,
		Colours::green,
		Colours::rebeccapurple,
		Colours::orange
	};

	const float xMarginScalar = 0.0917f;
	const float yMarginScalar = 0.025f;
	const float instructionsBottom = 0.183333f;
	const float fontHeightInBounds = 0.2f;
	const float controlHeightScalar = 0.058f;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::ComboBox> startingPointBox;
    std::unique_ptr<juce::Label> labelStartingPoint;
    std::unique_ptr<juce::Label> labelHorizontalSteps;
    std::unique_ptr<juce::TextEditor> editHorizontalSteps;
    std::unique_ptr<juce::Label> labelRightUpwardSteps;
    std::unique_ptr<juce::TextEditor> editRightUpwardSteps;
    std::unique_ptr<juce::Label> editInstructionText;
    std::unique_ptr<juce::GroupComponent> groupMapping;
    std::unique_ptr<juce::ComboBox> cbMappingType;
    std::unique_ptr<juce::Label> labelMappingType;
    std::unique_ptr<juce::TextButton> btnScaleStructureEditor;
    std::unique_ptr<juce::ComboBox> periodSizeBox;
    std::unique_ptr<juce::Label> labelPeriodSize;
    std::unique_ptr<juce::ToggleButton> setColourToggleButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IsomorphicMassAssign)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

