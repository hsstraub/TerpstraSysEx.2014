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

//[Headers] You can add your own extra header files here...
#include "ViewConstants.h"
#include "Main.h"
#include "MainComponent.h"
//[/Headers]

#include "IsomorphicMassAssign.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
IsomorphicMassAssign::IsomorphicMassAssign ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    periodSize = 0;
    // Create the mapping sub components. Do not make them visible (when one becomes visible it will automatically update the mapping logic)
	incrMidiNotesMapping.reset(new IncrMidiNotesMapping(periodSize, scaleStructure, colourTable));
	incrMidiNotesMapping->setVisible(false);
	addChildComponent(incrMidiNotesMapping.get());

	kbmMappingDlg.reset(new KBMMappingDlg(periodSize, scaleStructure, colourTable));
	kbmMappingDlg->setVisible(false);
	addChildComponent(kbmMappingDlg.get());

	mappingLogic = nullptr;
    //[/Constructor_pre]

    startingPointBox.reset (new ComboBox ("startingPointBox"));
    addAndMakeVisible (startingPointBox.get());
    startingPointBox->setEditableText (false);
    startingPointBox->setJustificationType (Justification::centredLeft);
    startingPointBox->setTextWhenNothingSelected (String());
    startingPointBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    startingPointBox->addListener (this);

    startingPointBox->setBounds (8, 320, 150, 24);

    labelStartingPoint.reset (new Label ("labelStartingPoint",
                                         TRANS("Starting value")));
    addAndMakeVisible (labelStartingPoint.get());
    labelStartingPoint->setTooltip (TRANS("Value that will be assigned to the key at mouse pposition when clicking"));
    labelStartingPoint->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelStartingPoint->setJustificationType (Justification::centredLeft);
    labelStartingPoint->setEditable (false, false, false);
    labelStartingPoint->setColour (TextEditor::textColourId, Colours::black);
    labelStartingPoint->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelStartingPoint->setBounds (8, 296, 150, 24);

    labelHorizontalSteps.reset (new Label ("labelHorizontalSteps",
                                           TRANS("Horizontal steps")));
    addAndMakeVisible (labelHorizontalSteps.get());
    labelHorizontalSteps->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelHorizontalSteps->setJustificationType (Justification::centredLeft);
    labelHorizontalSteps->setEditable (false, false, false);
    labelHorizontalSteps->setColour (TextEditor::textColourId, Colours::black);
    labelHorizontalSteps->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelHorizontalSteps->setBounds (304, 296, 136, 24);

    editHorizontalSteps.reset (new TextEditor ("editHorizontalSteps"));
    addAndMakeVisible (editHorizontalSteps.get());
    editHorizontalSteps->setMultiLine (false);
    editHorizontalSteps->setReturnKeyStartsNewLine (false);
    editHorizontalSteps->setReadOnly (false);
    editHorizontalSteps->setScrollbarsShown (true);
    editHorizontalSteps->setCaretVisible (true);
    editHorizontalSteps->setPopupMenuEnabled (true);
    editHorizontalSteps->setText (String());

    editHorizontalSteps->setBounds (312, 320, 40, 24);

    labelRightUpwardSteps.reset (new Label ("labelRightUpwardSteps",
                                            TRANS("Right upward steps")));
    addAndMakeVisible (labelRightUpwardSteps.get());
    labelRightUpwardSteps->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelRightUpwardSteps->setJustificationType (Justification::centredLeft);
    labelRightUpwardSteps->setEditable (false, false, false);
    labelRightUpwardSteps->setColour (TextEditor::textColourId, Colours::black);
    labelRightUpwardSteps->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelRightUpwardSteps->setBounds (176, 280, 136, 24);

    editRightUpwardSteps.reset (new TextEditor ("editRightUpwardSteps"));
    addAndMakeVisible (editRightUpwardSteps.get());
    editRightUpwardSteps->setMultiLine (false);
    editRightUpwardSteps->setReturnKeyStartsNewLine (false);
    editRightUpwardSteps->setReadOnly (false);
    editRightUpwardSteps->setScrollbarsShown (true);
    editRightUpwardSteps->setCaretVisible (true);
    editRightUpwardSteps->setPopupMenuEnabled (true);
    editRightUpwardSteps->setText (String());

    editRightUpwardSteps->setBounds (184, 304, 39, 24);

    editInstructionText.reset (new Label ("editInstructionText",
                                          TRANS("Fill a line or the whole field with constant step distances. \n"
                                          "Click on desired key field to start.")));
    addAndMakeVisible (editInstructionText.get());
    editInstructionText->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    editInstructionText->setJustificationType (Justification::topLeft);
    editInstructionText->setEditable (false, false, false);
    editInstructionText->setColour (TextEditor::textColourId, Colours::black);
    editInstructionText->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    editInstructionText->setBounds (8, 200, 416, 40);

    groupMapping.reset (new GroupComponent ("groupMapping",
                                            TRANS("Mapping")));
    addAndMakeVisible (groupMapping.get());

    groupMapping->setBounds (8, 40, 416, 152);

    cbMappingType.reset (new ComboBox ("cbMappingType"));
    addAndMakeVisible (cbMappingType.get());
    cbMappingType->setEditableText (false);
    cbMappingType->setJustificationType (Justification::centredLeft);
    cbMappingType->setTextWhenNothingSelected (String());
    cbMappingType->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbMappingType->addItem (TRANS("MIDI notes, increasing order"), 1);
    cbMappingType->addItem (TRANS("Scala KBM mappings"), 2);
    cbMappingType->addListener (this);

    cbMappingType->setBounds (104, 64, 304, 24);

    labelMappingType.reset (new Label ("labelMappingType",
                                       TRANS("Type:")));
    addAndMakeVisible (labelMappingType.get());
    labelMappingType->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelMappingType->setJustificationType (Justification::centredLeft);
    labelMappingType->setEditable (false, false, false);
    labelMappingType->setColour (TextEditor::textColourId, Colours::black);
    labelMappingType->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelMappingType->setBounds (20, 64, 88, 24);

    btnScaleStructureEditor.reset (new TextButton ("btnScaleStructureEditor"));
    addAndMakeVisible (btnScaleStructureEditor.get());
    btnScaleStructureEditor->setTooltip (TRANS("Show/hide scale structure editor"));
    btnScaleStructureEditor->setButtonText (TRANS("Scale structure editor"));
    btnScaleStructureEditor->addListener (this);

    btnScaleStructureEditor->setBounds (264, 248, 152, 24);

    periodSizeBox.reset (new ComboBox ("periodSizeBox"));
    addAndMakeVisible (periodSizeBox.get());
    periodSizeBox->setTooltip (TRANS("Number of tones per period interval (octave)"));
    periodSizeBox->setEditableText (false);
    periodSizeBox->setJustificationType (Justification::centredLeft);
    periodSizeBox->setTextWhenNothingSelected (String());
    periodSizeBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    periodSizeBox->addListener (this);

    periodSizeBox->setBounds (200, 8, 56, 24);

    labelPeriodSize.reset (new Label ("labelPeriodSize",
                                      TRANS("Period:")));
    addAndMakeVisible (labelPeriodSize.get());
    labelPeriodSize->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelPeriodSize->setJustificationType (Justification::centredLeft);
    labelPeriodSize->setEditable (false, false, false);
    labelPeriodSize->setColour (TextEditor::textColourId, Colours::black);
    labelPeriodSize->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelPeriodSize->setBounds (16, 8, 168, 24);

    setColourToggleButton.reset (new ToggleButton ("setColourToggleButton"));
    addAndMakeVisible (setColourToggleButton.get());
    setColourToggleButton->setButtonText (TRANS("Colour assignment"));
    setColourToggleButton->addListener (this);

    setColourToggleButton->setBounds (8, 248, 168, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (428, 352);


    //[Constructor] You can add your own custom stuff here..

	for (int i = 1; i <= 128; i++)
	{
		periodSizeBox->addItem(String(i), i);
	}

	incrMidiNotesMapping->getMappingLogic()->addListener(this);
	kbmMappingDlg->getMappingLogic()->addListener(this);

	// cbMappingStyle default selection: will be read from user settings
    //[/Constructor]
}

IsomorphicMassAssign::~IsomorphicMassAssign()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	incrMidiNotesMapping = nullptr;
	kbmMappingDlg = nullptr;
	scaleDesignWindow = nullptr;
    //[/Destructor_pre]

    startingPointBox = nullptr;
    labelStartingPoint = nullptr;
    labelHorizontalSteps = nullptr;
    editHorizontalSteps = nullptr;
    labelRightUpwardSteps = nullptr;
    editRightUpwardSteps = nullptr;
    editInstructionText = nullptr;
    groupMapping = nullptr;
    cbMappingType = nullptr;
    labelMappingType = nullptr;
    btnScaleStructureEditor = nullptr;
    periodSizeBox = nullptr;
    labelPeriodSize = nullptr;
    setColourToggleButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void IsomorphicMassAssign::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(findColour(ResizableWindow::backgroundColourId));
    //[/UserPaint]
}

void IsomorphicMassAssign::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	incrMidiNotesMapping->setBounds(16, MAPPINGSUBWINTOP, EDITAREAWIDTH, MAPPINGSUBWINHEIGHT);
	kbmMappingDlg->setBounds(16, MAPPINGSUBWINTOP, EDITAREAWIDTH, MAPPINGSUBWINHEIGHT);
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void IsomorphicMassAssign::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == startingPointBox.get())
    {
        //[UserComboBoxCode_startingPointBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_startingPointBox]
    }
    else if (comboBoxThatHasChanged == cbMappingType.get())
    {
        //[UserComboBoxCode_cbMappingType] -- add your combo box handling code here..
		int mappingType = cbMappingType->getSelectedItemIndex();

		// Show sub window corresponding to selected mapping style
		switch(mappingType)
		{
        case 0:
            incrMidiNotesMapping->setVisible(true);
            kbmMappingDlg->setVisible(false);
            break;
        case 1:
            incrMidiNotesMapping->setVisible(false);
            kbmMappingDlg->setVisible(true);
            break;
        default:
            incrMidiNotesMapping->setVisible(false);
            kbmMappingDlg->setVisible(false);
		}
        //[/UserComboBoxCode_cbMappingType]
    }
    else if (comboBoxThatHasChanged == periodSizeBox.get())
    {
        //[UserComboBoxCode_periodSizeBox] -- add your combo box handling code here..
        periodSize = comboBoxThatHasChanged->getSelectedId();

        // Update scale structure (must be done before update of the mapping sub dialogs)
		scaleStructure.resetToPeriod(periodSize);
		if (scaleDesignWindow.get())
			scaleDesignWindow->loadScaleStructureSettings();

        incrMidiNotesMapping->onUpdatePeriodSize();
        kbmMappingDlg->onUpdatePeriodSize();
        //[/UserComboBoxCode_periodSizeBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void IsomorphicMassAssign::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnScaleStructureEditor.get())
    {
        //[UserButtonCode_btnScaleStructureEditor] -- add your button handler code here..
		// Initialize this here to allow the ScaleStructure settings to be loaded first
		if (scaleDesignWindow.get() == nullptr)
		{
			scaleDesignWindow.reset(new ScaleDesignWindow(scaleStructure, colourTable, findColour(ResizableWindow::backgroundColourId)));
			scaleDesignWindow->addScaleDesignerListener(this);
		}

		scaleDesignWindow->setVisible(!scaleDesignWindow->isVisible());
        //[/UserButtonCode_btnScaleStructureEditor]
    }
    else if (buttonThatWasClicked == setColourToggleButton.get())
    {
        //[UserButtonCode_setColourToggleButton] -- add your button handler code here..
        auto assignColours = setColourToggleButton->getToggleState();
        incrMidiNotesMapping->getMappingLogic()->setAssignColours(assignColours);
        kbmMappingDlg->getMappingLogic()->setAssignColours(assignColours);
        //[/UserButtonCode_setColourToggleButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void IsomorphicMassAssign::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
    periodSizeBox->setSelectedId(
        propertiesFile->getIntValue("PeriodSize", 0),
        juce::NotificationType::sendNotification);

	cbMappingType->setSelectedItemIndex(
        propertiesFile->getIntValue("MappingType", 0),
        juce::NotificationType::sendNotification);

	setColourToggleButton->setToggleState(
		propertiesFile->getBoolValue("MassAssignColourSetActive", true),
		juce::NotificationType::sendNotification);

    kbmMappingDlg->restoreStateFromPropertiesFile(propertiesFile);

	// TODO: Full scaleStructure state recall
}

void IsomorphicMassAssign::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
    propertiesFile->setValue("PeriodSize", periodSizeBox->getSelectedId());
    propertiesFile->setValue("MappingType", cbMappingType->getSelectedItemIndex());
	propertiesFile->setValue("MassAssignColourSetActive", setColourToggleButton->getToggleState());

    kbmMappingDlg->saveStateToPropertiesFile(propertiesFile);

	// TODO: Save full ScaleStructure state
}

void IsomorphicMassAssign::setSaveSend(int setSelection, int keySelection, int noteIndex)
{
	// XXX This could be in a common base class for all assign edit components
	// Save data
	this->mappingLogic->indexToTerpstraKey(
        noteIndex,
        ((MainContentComponent*)(getParentComponent()->getParentComponent()))->getMappingInEdit().sets[setSelection].theKeys[keySelection]);

	// Send to device
	TerpstraSysExApplication::getApp().getMidiDriver().sendKeyParam(setSelection + 1, keySelection,
        ((MainContentComponent*)(getParentComponent()->getParentComponent()))->getMappingInEdit().sets[setSelection].theKeys[keySelection]);
}

// Fill a line in current octave board. Starting point is assumed to have been set
void IsomorphicMassAssign::fillLine(int setSelection, TerpstraBoardGeometry::StraightLine& line, int startPos, int startNoteIndex, int stepSize)
{
	jassert(stepSize != 0);

	int pos, noteIndex;
	// Forward
	for (pos = startPos + 1, noteIndex = startNoteIndex + stepSize;
		pos < line.size() && noteIndex < this->mappingLogic->globalMappingSize();
		pos++, noteIndex += stepSize)
	{
		setSaveSend(setSelection, line[pos], noteIndex);
	}

	// Backward
	for (pos = startPos - 1, noteIndex = startNoteIndex - stepSize;
		pos >= 0 && noteIndex >= 0;
		pos--, noteIndex -= stepSize)
	{
		setSaveSend(setSelection, line[pos], noteIndex);
	}
}

// Fill a horizontal line over all octave boards. Starting point is assumed to have been set.
void IsomorphicMassAssign::fillGlobalLine(int setSelection, TerpstraBoardGeometry::StraightLineSet& globalLine, int startPos, int startNoteIndex, int stepSize)
{
	jassert(stepSize != 0);

	int pos, octaveBoardIndex;

	// Forward
	// Line belonging to current octave board
	int noteIndex = startNoteIndex + stepSize;

	for (pos = startPos + 1;
		pos < globalLine[setSelection].size() && noteIndex < this->mappingLogic->globalMappingSize();
		pos++, noteIndex += stepSize)
	{
		setSaveSend(setSelection, globalLine[setSelection][pos], noteIndex);
	}

	// Following octave boards
	for (octaveBoardIndex = setSelection+1; octaveBoardIndex < NUMBEROFBOARDS; octaveBoardIndex++)
    {
        for (pos = 0;
            pos < globalLine[octaveBoardIndex].size() && noteIndex < this->mappingLogic->globalMappingSize();
            pos++, noteIndex += stepSize)
        {
            setSaveSend(octaveBoardIndex, globalLine[octaveBoardIndex][pos], noteIndex);
        }
    }

	// Backward
	// Current octave board
	noteIndex = startNoteIndex - stepSize;

	for (pos = startPos - 1; pos >= 0 && noteIndex >= 0; pos--, noteIndex -= stepSize)
	{
		setSaveSend(setSelection, globalLine[setSelection][pos], noteIndex);
	}

    // Preceding octave boards
	for (octaveBoardIndex = setSelection-1; octaveBoardIndex >= 0; octaveBoardIndex--)
    {
        for (pos = globalLine[octaveBoardIndex].size()-1;
            pos >=0  && noteIndex >= 0;
            pos--, noteIndex -= stepSize)
        {
            setSaveSend(octaveBoardIndex, globalLine[octaveBoardIndex][pos], noteIndex);
        }
    }
}

// Fill a horizontal line and its cutting upwards lines, recursively. Fill only those that have not been filled yet. Starting point is assumed to have been set.
void IsomorphicMassAssign::fill2DHorizLineRecursive(int setSelection, TerpstraBoardGeometry::StraightLine& horizLine, int startPos, int startNoteIndex,
	int horizStepSize, int rUpwStepSize,
	TerpstraBoardGeometry::StraightLineSet& finishedLines)
{
	// Only if not done yet
	if (!finishedLines.contains(horizLine))
	{
		// Fill the line itself
		fillLine(setSelection, horizLine, startPos, startNoteIndex, horizStepSize);

		// Add to list of finished lines
		finishedLines.add(horizLine);

		// Find cutting lines and fill them
		for (auto horizLineField : horizLine)
		{
			// Find the vertical line at this position
			TerpstraBoardGeometry::StraightLine rUpLine = boardGeometry.rightUpwardLineOfField(horizLineField);
			int rUpStartPos = rUpLine.indexOf(horizLineField);

			// Start note index: the value that has been set to the horizontal line element (if it has)
			int rUpStartNoteIndex = this->mappingLogic->terpstraKeyToIndex(
				((MainContentComponent*)(getParentComponent()->getParentComponent()))->getMappingInEdit()
                .sets[setSelection].theKeys[horizLineField]);

            if (rUpStartNoteIndex >= 0)
                // Fill it and its cutting lines, if it has not been done before. Check of the latter is done inside.
                fill2DRUpwLineRecursive(setSelection, rUpLine, rUpStartPos, rUpStartNoteIndex, horizStepSize, rUpwStepSize, finishedLines);
		}
	}
}

// Fill a right upward line and its cutting horizontal lines, recursively. Fill only those that have not been filled yet. Starting point is assumed to have been set.
void IsomorphicMassAssign::fill2DRUpwLineRecursive(int setSelection, TerpstraBoardGeometry::StraightLine& rUpwLine, int startPos, int startNoteIndex,
	int horizStepSize, int rUpwStepSize,
	TerpstraBoardGeometry::StraightLineSet& finishedLines)
{
	// Only if not done yet
	if (!finishedLines.contains(rUpwLine))
	{
		// Fill the line itself
		fillLine(setSelection, rUpwLine, startPos, startNoteIndex, rUpwStepSize);

		// Add to list of finished lines
		finishedLines.add(rUpwLine);

		// Find cutting lines and fill them
		for (auto rUpwLineField : rUpwLine)
		{
			// Find the vertical line at this position
			TerpstraBoardGeometry::StraightLine horizLine = boardGeometry.horizontalLineOfField(rUpwLineField);
			int horizStartPos = horizLine.indexOf(rUpwLineField);

			// Start note index: the value that has been set to the horizontal line element (if it has)
			int horizStartNoteIndex = this->mappingLogic->terpstraKeyToIndex(
				((MainContentComponent*)(getParentComponent()->getParentComponent()))->getMappingInEdit().sets[setSelection].theKeys[rUpwLineField]);

            if (horizStartNoteIndex >= 0)
                // Fill it and its cutting lines, if it has not been done before. Check of the latter is done inside.
                fill2DHorizLineRecursive(setSelection, horizLine, horizStartPos, horizStartNoteIndex, horizStepSize, rUpwStepSize, finishedLines);
		}
	}
}

// Implementation of MappingLogicListener
void IsomorphicMassAssign::mappingLogicChanged(MappingLogicBase* mappingLogicThatChanged)
{
	this->mappingLogic = mappingLogicThatChanged;

	// Fill note combo with values according to mapping logic
	startingPointBox->clear(juce::NotificationType::dontSendNotification);

	for (int i = 0; i < mappingLogicThatChanged->globalMappingSize(); i++)
	{
		TerpstraKey keyData = mappingLogicThatChanged->indexToTerpstraKey(i);
		jassert(!keyData.isEmpty());

		startingPointBox->addItem(String(i) + ": Key_" + String(keyData.noteNumber) + ", Chan_" + String(keyData.channelNumber), i + 1);
	}
}

void IsomorphicMassAssign::scaleStructurePeriodChanged(int newPeriod)
{
	periodSizeBox->setSelectedId(newPeriod);
}

void IsomorphicMassAssign::scaleStructureStepSizesChanged(int rightUpwardSize, int horizontalSize)
{
	editRightUpwardSteps->setText(String(rightUpwardSize));
	editHorizontalSteps->setText(String(horizontalSize));
}

/// <summary>Called from MainComponent when one of the keys is clicked</summary>
/// <returns>Mapping was changed yes/no</returns>
bool IsomorphicMassAssign::performMouseDown(int setSelection, int keySelection)
{
	bool mappingChanged = false;
	jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS && keySelection >= 0 && keySelection < TERPSTRABOARDSIZE);

	int startNoteIndex = this->startingPointBox->getSelectedItemIndex();
	if (this->mappingLogic != nullptr && startNoteIndex >= 0)
	{
	    auto mainComponent = ((MainContentComponent*)(getParentComponent()->getParentComponent()));

		int horizStepSize = editHorizontalSteps->getText().getIntValue();
		int rUpwStepSize = editRightUpwardSteps->getText().getIntValue();

		// Horizontal line
		if (horizStepSize != 0 && rUpwStepSize == 0)
		{
            // Set value of starting point
            setSaveSend(setSelection, keySelection, startNoteIndex);

			TerpstraBoardGeometry::StraightLineSet globalHorizLine =
                boardGeometry.globalHorizontalLineOfField(setSelection, keySelection);
			int startPos = globalHorizLine[setSelection].indexOf(keySelection);
			fillGlobalLine(setSelection, globalHorizLine, startPos, startNoteIndex, horizStepSize);

			mappingChanged = true;
		}

		// Right vertical line
		else if (horizStepSize == 0 && rUpwStepSize != 0)
		{
            // Set value of starting point
            setSaveSend(setSelection, keySelection, startNoteIndex);

			TerpstraBoardGeometry::StraightLineSet globalRUpLine =
                boardGeometry.globalRightUpwardLineOfField(setSelection, keySelection);
			int startPos = globalRUpLine[setSelection].indexOf(keySelection);
			fillGlobalLine(setSelection, globalRUpLine, startPos, startNoteIndex, rUpwStepSize);
			mappingChanged = true;
		}

		// Two dimensional: fill whole subset
		else if (horizStepSize != 0 && rUpwStepSize != 0)
		{
		    // Lines with right / left continuation
		    auto linesWithRightContinuation = boardGeometry.getHorizontalLinesWithContinuation(1);
		    auto linesWithLeftContinuation = boardGeometry.getHorizontalLinesWithContinuation(-1);

		    // List of lines that have been finished, so the recursion ends - per octave board
			TerpstraBoardGeometry::StraightLineSet finishedLineSets[NUMBEROFBOARDS];

		    // Delete all data first
		    mainComponent->deleteAll(false);

            // Set value of starting point
            setSaveSend(setSelection, keySelection, startNoteIndex);

			// Find the horizontal line
			auto horizLine = boardGeometry.horizontalLineOfField(keySelection);
			int startPos = horizLine.indexOf(keySelection);

			// Fill the board of current selection, starting from this line
			fill2DHorizLineRecursive(setSelection, horizLine, startPos, startNoteIndex, horizStepSize, rUpwStepSize,
                finishedLineSets[setSelection]);

            // Following octave boards
            for ( auto octaveBoardIndex = setSelection+1; octaveBoardIndex < NUMBEROFBOARDS; octaveBoardIndex++)
            {
                // Find a field that can be filled, from the values of the preceding octave board
                for ( auto horizLine : linesWithRightContinuation)
                {
                    int noteIndex = this->mappingLogic->terpstraKeyToIndex(
                        mainComponent->getMappingInEdit().sets[octaveBoardIndex-1].theKeys[horizLine.getLast()]);

                    if ( noteIndex >= 0 )
                    {
                        // A field in preceding octave board with an assigned value has been found-
                        // Value can be assigned to continuation on current board.
                        auto newHorizLine = boardGeometry.continuationOfHorizontalLine(horizLine, 1);

                   		setSaveSend(octaveBoardIndex, newHorizLine.getFirst(), noteIndex + horizStepSize);

                   		// Fill the whole sub board based on this field
                        fill2DHorizLineRecursive(octaveBoardIndex, newHorizLine, 0,
                            noteIndex + horizStepSize, horizStepSize, rUpwStepSize,
                            finishedLineSets[octaveBoardIndex]);

                        break;  // One field is enough to fill a whole octave board
                    }
                }
            }

            // Preceding octave boards
            // ToDo

			mappingChanged = true;
		}
	}

	return mappingChanged;
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="IsomorphicMassAssign" componentName=""
                 parentClasses="public Component, public MappingLogicBase::Listener, public ScaleStructureComponent::Listener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="428"
                 initialHeight="352">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <COMBOBOX name="startingPointBox" id="d526f69bdc196fea" memberName="startingPointBox"
            virtualName="" explicitFocusOrder="0" pos="8 320 150 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="labelStartingPoint" id="5401a3246c13771e" memberName="labelStartingPoint"
         virtualName="" explicitFocusOrder="0" pos="8 296 150 24" tooltip="Value that will be assigned to the key at mouse pposition when clicking"
         edTextCol="ff000000" edBkgCol="0" labelText="Starting value"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="labelHorizontalSteps" id="3e6663aecf1474c8" memberName="labelHorizontalSteps"
         virtualName="" explicitFocusOrder="0" pos="304 296 136 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Horizontal steps" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="editHorizontalSteps" id="8d2f5f07f337b9ef" memberName="editHorizontalSteps"
              virtualName="" explicitFocusOrder="0" pos="312 320 40 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="labelRightUpwardSteps" id="43530804741d9cb7" memberName="labelRightUpwardSteps"
         virtualName="" explicitFocusOrder="0" pos="176 280 136 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Right upward steps" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="editRightUpwardSteps" id="3a1cf8588366e0ca" memberName="editRightUpwardSteps"
              virtualName="" explicitFocusOrder="0" pos="184 304 39 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="editInstructionText" id="c03ef432c2b4599" memberName="editInstructionText"
         virtualName="" explicitFocusOrder="0" pos="8 200 416 40" edTextCol="ff000000"
         edBkgCol="0" labelText="Fill a line or the whole field with constant step distances. &#10;Click on desired key field to start."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="9"/>
  <GROUPCOMPONENT name="groupMapping" id="fbb69100a7b12118" memberName="groupMapping"
                  virtualName="" explicitFocusOrder="0" pos="8 40 416 152" title="Mapping"/>
  <COMBOBOX name="cbMappingType" id="a7825b65cfb78392" memberName="cbMappingType"
            virtualName="" explicitFocusOrder="0" pos="104 64 304 24" editable="0"
            layout="33" items="MIDI notes, increasing order&#10;Scala KBM mappings"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="labelMappingType" id="d77d8a4b80130afc" memberName="labelMappingType"
         virtualName="" explicitFocusOrder="0" pos="20 64 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Type:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="btnScaleStructureEditor" id="23cc77cbad6653d7" memberName="btnScaleStructureEditor"
              virtualName="" explicitFocusOrder="0" pos="264 248 152 24" tooltip="Show/hide scale structure editor"
              buttonText="Scale structure editor" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <COMBOBOX name="periodSizeBox" id="4560285c5e467e2f" memberName="periodSizeBox"
            virtualName="" explicitFocusOrder="0" pos="200 8 56 24" tooltip="Number of tones per period interval (octave)"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="labelPeriodSize" id="c2aeb9a3f194ed5b" memberName="labelPeriodSize"
         virtualName="" explicitFocusOrder="0" pos="16 8 168 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Period:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="setColourToggleButton" id="fb41f2b9539dfb3f" memberName="setColourToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 248 168 24" buttonText="Colour assignment"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

