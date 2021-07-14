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

    labelMappingType.reset(new juce::Label("labelMappingType", translate("Type")));
    addAndMakeVisible(labelMappingType.get());
    labelMappingType->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::GothamNarrowMedium));

    labelMappingType->setBounds(20, 64, 88, 24);

    cbMappingType.reset(new juce::ComboBox("cbMappingType"));
    addAndMakeVisible(cbMappingType.get());
    cbMappingType->setEditableText(false);
    cbMappingType->setJustificationType(juce::Justification::centredLeft);
    cbMappingType->setTextWhenNothingSelected(juce::String());
    cbMappingType->setTextWhenNoChoicesAvailable(translate("NoChoices"));
    cbMappingType->addItem(translate("MIDINotesIncreasingOrder"), 1);
    cbMappingType->addItem(translate("Scala KBM mappings"), 2);
    cbMappingType->addListener(this);

    cbMappingType->setBounds(64, 64, 240, 24);

    mappingLogic = nullptr;

    // Create the mapping sub components. Do not make them visible (when one becomes visible it will automatically update the mapping logic)
    incrMidiNotesMapping.reset(new IncrMidiNotesMapping(periodSize, scaleStructure, colourTable));
    incrMidiNotesMapping->setVisible(false);
    addChildComponent(incrMidiNotesMapping.get());


    kbmMappingDlg.reset(new KBMMappingDlg(periodSize, scaleStructure, colourTable));
    kbmMappingDlg->setVisible(false);
    addChildComponent(kbmMappingDlg.get());


    //groupMapping.reset(new juce::GroupComponent("groupMapping", translate("Mapping")));
    //addAndMakeVisible(groupMapping.get());
    //groupMapping->setBounds(8, 40, 304, 152);

    //editInstructionText.reset(new juce::Label("editInstructionText", translate("IsomorphicAssignDirections")));
    //addAndMakeVisible(editInstructionText.get());
    //editInstructionText->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    //editInstructionText->setJustificationType(juce::Justification::topLeft);
    //editInstructionText->setEditable(false, false, false);
    //editInstructionText->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    //editInstructionText->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    //editInstructionText->setBounds(8, 200, 296, 48);


    btnScaleStructureEditor.reset(new juce::TextButton("btnScaleStructureEditor"));
    addAndMakeVisible(btnScaleStructureEditor.get());
    btnScaleStructureEditor->setTooltip(translate("ShowHide") + " " + translate("ScaleStructureEditor"));
    btnScaleStructureEditor->setButtonText(translate("ScaleStructureEditor"));
    btnScaleStructureEditor->addListener(this);

    btnScaleStructureEditor->setBounds(160, 264, 152, 24);

    labelStartingPoint.reset(new juce::Label("labelStartingPoint", translate("StartingValue")));
    addAndMakeVisible(labelStartingPoint.get());
    labelStartingPoint->setTooltip(translate("StartingPointTooltip"));
    labelStartingPoint->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::GothamNarrowMedium));

    labelStartingPoint->setBounds(8, 344, 150, 24);

    startingPointBox.reset (new juce::ComboBox ("startingPointBox"));
    addAndMakeVisible (startingPointBox.get());
    startingPointBox->setEditableText (false);
    startingPointBox->setJustificationType (juce::Justification::centredLeft);
    startingPointBox->setTextWhenNothingSelected (juce::String());
    startingPointBox->setTextWhenNoChoicesAvailable (translate("NoChoices"));
    startingPointBox->addListener (this);

    startingPointBox->setBounds(8, 368, 150, 24);

    labelPeriodSize.reset(new juce::Label("labelPeriodSize", TRANS("Period")));
    addAndMakeVisible(labelPeriodSize.get());
    labelPeriodSize->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::GothamNarrowMedium));
  
    labelPeriodSize->setBounds(16, 8, 168, 24);

    periodSizeBox.reset (new juce::ComboBox ("periodSizeBox"));
    addAndMakeVisible (periodSizeBox.get());
    periodSizeBox->setTooltip (translate("PeriodToolTip"));
    periodSizeBox->setEditableText (false);
    periodSizeBox->setJustificationType (juce::Justification::centredLeft);
    periodSizeBox->setTextWhenNothingSelected (juce::String());
    periodSizeBox->setTextWhenNoChoicesAvailable (translate("NoChoices"));
    periodSizeBox->addListener (this);

    periodSizeBox->setBounds (200, 8, 56, 24);

    setColourToggleButton.reset (new juce::ToggleButton ("setColourToggleButton"));
    addAndMakeVisible (setColourToggleButton.get());
    setColourToggleButton->setButtonText (TRANS("ColourAssignment"));
    setColourToggleButton->addListener (this);

    setColourToggleButton->setBounds (8, 264, 152, 24);


    labelHorizontalSteps.reset(new juce::Label("labelHorizontalSteps", translate("HorizontalSteps")));
    addAndMakeVisible(labelHorizontalSteps.get());
    labelHorizontalSteps->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::GothamNarrowMedium));
 
    labelHorizontalSteps->setBounds(208, 344, 112, 24);

    editHorizontalSteps.reset(new juce::TextEditor("editHorizontalSteps"));
    addAndMakeVisible(editHorizontalSteps.get());
    editHorizontalSteps->setMultiLine(false);
    editHorizontalSteps->setReturnKeyStartsNewLine(false);
    editHorizontalSteps->setReadOnly(false);
    editHorizontalSteps->setScrollbarsShown(true);
    editHorizontalSteps->setCaretVisible(true);
    editHorizontalSteps->setPopupMenuEnabled(true);
    editHorizontalSteps->setText(juce::String());

    editHorizontalSteps->setBounds(216, 368, 40, 24);

    labelRightUpwardSteps.reset(new juce::Label("labelRightUpwardSteps", translate("RightUpwardSteps")));
    addAndMakeVisible(labelRightUpwardSteps.get());
    labelRightUpwardSteps->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::GothamNarrowMedium));
    labelRightUpwardSteps->setBounds(152, 296, 136, 24);

    editRightUpwardSteps.reset(new juce::TextEditor("editRightUpwardSteps"));
    addAndMakeVisible(editRightUpwardSteps.get());
    editRightUpwardSteps->setMultiLine(false);
    editRightUpwardSteps->setReturnKeyStartsNewLine(false);
    editRightUpwardSteps->setReadOnly(false);
    editRightUpwardSteps->setScrollbarsShown(true);
    editRightUpwardSteps->setCaretVisible(true);
    editRightUpwardSteps->setPopupMenuEnabled(true);
    editRightUpwardSteps->setText(juce::String());

    editRightUpwardSteps->setBounds(160, 320, 39, 24);

    //[/Constructor_pre]

    //[UserPreSize]

    flexBoxComponents.add(setColourToggleButton.get());
    flexBoxComponents.add(labelStartingPoint.get());
    flexBoxComponents.add(labelPeriodSize.get());
    flexBoxComponents.add(labelHorizontalSteps.get());
    flexBoxComponents.add(labelRightUpwardSteps.get());

    flexBox.flexWrap = FlexBox::Wrap::noWrap;
    flexBox.flexDirection = FlexBox::Direction::column;
    //[/UserPreSize]

    setSize (320, 400);


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
void IsomorphicMassAssign::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //g.fillAll (juce::Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
    g.fillAll(Colour(0xff2d3135));

	g.setColour(getLookAndFeel().findColour(LumatoneEditorColourIDs::LightBackground));
    g.fillRect(instructionsBounds);

    g.setColour(getLookAndFeel().findColour(LumatoneEditorColourIDs::InactiveText));
    g.setFont(instructionsFont);
    g.drawFittedText(translate("IsomorphicAssignDirections"), instructionsBounds, Justification::centred, 3, 1.0f);

    //[/UserPaint]
}

void IsomorphicMassAssign::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	int width = getWidth();
    int height = getHeight();

    int controlWidth = round(width * 0.4f);
    int controlHeight = round(height * controlHeightScalar);

    int xMargin = round(width * xMarginScalar);
    int yMargin = round(height * yMarginScalar);
    int xEnd = width - xMargin;

    flexBox.items.clear();
    for (auto child : flexBoxComponents)
    {
        flexBox.items.add(FlexItem(*child).withHeight(controlHeight).withMargin(FlexItem::Margin(yMargin, 0, 0, 0)));
    }

    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    instructionsBounds = getLocalBounds().withBottom(round(height * instructionsBottom));
    instructionsFont.setHeight(instructionsBounds.proportionOfHeight(fontHeightInBounds));

    resizeLabelWithHeight(labelMappingType.get(), controlHeight);
    labelMappingType->setTopLeftPosition(xMargin, labelMappingType->getY());
    cbMappingType->setBounds(labelMappingType->getBounds().withLeft(labelMappingType->getRight()).withRight(xEnd));

    Rectangle<int> mappingTypeBounds(xMargin, labelMappingType->getBottom() + yMargin, width - xMargin * 2, instructionsBounds.getHeight() + controlHeight);

    incrMidiNotesMapping->setBounds(mappingTypeBounds);
    kbmMappingDlg->setBounds(mappingTypeBounds);

    flexBox.performLayout(getLocalBounds().withTop(mappingTypeBounds.getBottom() + yMargin).withBottom(height - yMargin).reduced(xMargin, 0));

    resizeLabelWithHeight(labelStartingPoint.get(),     controlHeight);
    resizeLabelWithHeight(labelPeriodSize.get(),        controlHeight);
    resizeLabelWithHeight(labelHorizontalSteps.get(),   controlHeight);
    resizeLabelWithHeight(labelRightUpwardSteps.get(),  controlHeight);

    setColourToggleButton->setSize(controlWidth, controlHeight * 0.75);

    btnScaleStructureEditor->   setBounds(setColourToggleButton->   getBounds().withLeft(setColourToggleButton->    getRight() + xMargin).withRight(xEnd));
    startingPointBox->          setBounds(labelStartingPoint->      getBounds().withLeft(labelStartingPoint->       getRight()).withRight(xEnd));
    periodSizeBox->             setBounds(labelPeriodSize->         getBounds().withLeft(labelPeriodSize->          getRight()).withRight(xEnd));
    editHorizontalSteps->       setBounds(labelHorizontalSteps->    getBounds().withLeft(labelHorizontalSteps->     getRight()).withRight(xEnd));
    editRightUpwardSteps->      setBounds(labelRightUpwardSteps->   getBounds().withLeft(labelRightUpwardSteps->    getRight()).withRight(xEnd));


    //[/UserResized]
}

void IsomorphicMassAssign::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
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

void IsomorphicMassAssign::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnScaleStructureEditor.get())
    {
        //[UserButtonCode_btnScaleStructureEditor] -- add your button handler code here..
		// Initialize this here to allow the ScaleStructure settings to be loaded first
		if (scaleDesignWindow.get() == nullptr)
		{
			scaleDesignWindow.reset(
                new ScaleDesignWindow(scaleStructure, colourTable, findColour(ResizableWindow::backgroundColourId)));
			scaleDesignWindow->setLookAndFeel(&TerpstraSysExApplication::getApp().getLookAndFeel());
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
    //scaleStructure.resetToPeriod(periodSizeBox->getSelectedId());
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

	auto mainComponent = dynamic_cast<MainContentComponent*>(getParentComponent()->getParentComponent()->getParentComponent());

	// Save data
	this->mappingLogic->indexToTerpstraKey(
        noteIndex,
		mainComponent->getMappingInEdit().sets[setSelection].theKeys[keySelection]);

	// Send to device
	TerpstraSysExApplication::getApp().getLumatoneController().sendKeyParam(setSelection + 1, keySelection,
		mainComponent->getMappingInEdit().sets[setSelection].theKeys[keySelection]);
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
		auto mainComponent = (MainContentComponent*)(getParentComponent()->getParentComponent()->getParentComponent());

		for (auto horizLineField : horizLine)
		{
			// Find the vertical line at this position
			TerpstraBoardGeometry::StraightLine rUpLine = boardGeometry.rightUpwardLineOfField(horizLineField);
			int rUpStartPos = rUpLine.indexOf(horizLineField);

			// Start note index: the value that has been set to the horizontal line element (if it has)
			int rUpStartNoteIndex = this->mappingLogic->terpstraKeyToIndex(
				mainComponent->getMappingInEdit()
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
		auto mainComponent = dynamic_cast<MainContentComponent*>(getParentComponent()->getParentComponent()->getParentComponent());

		for (auto rUpwLineField : rUpwLine)
		{
			// Find the vertical line at this position
			TerpstraBoardGeometry::StraightLine horizLine = boardGeometry.horizontalLineOfField(rUpwLineField);
			int horizStartPos = horizLine.indexOf(rUpwLineField);

			// Start note index: the value that has been set to the horizontal line element (if it has)
			int horizStartNoteIndex = this->mappingLogic->terpstraKeyToIndex(
				mainComponent->getMappingInEdit().sets[setSelection].theKeys[rUpwLineField]);

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

    // Record name of last starting point
    String previousStartingLabel = startingPointBox->getItemText(startingPointBox->getSelectedItemIndex());
    int newIndex = -1;

	// Fill note combo with values according to mapping logic
	startingPointBox->clear(juce::NotificationType::dontSendNotification);

	for (int i = 0; i < mappingLogicThatChanged->globalMappingSize(); i++)
	{
		TerpstraKey keyData = mappingLogicThatChanged->indexToTerpstraKey(i);
		jassert(!keyData.isEmpty());

        String keyLabel = String(i) + ": Key_" + String(keyData.noteNumber) + ", Chan_" + String(keyData.channelNumber);
        startingPointBox->addItem(keyLabel, i + 1);

        if (keyLabel == previousStartingLabel)
            newIndex = i;
	}

    if (newIndex > 0)
        startingPointBox->setSelectedId(newIndex + 1);
}

void IsomorphicMassAssign::scaleStructurePeriodChanged(int newPeriod)
{
	periodSizeBox->setSelectedId(newPeriod);
}

void IsomorphicMassAssign::scaleStructureStepSizesChanged(int rightUpwardSize, int horizontalSize)
{
	// Orientation fix
	rightUpwardSize = horizontalSize - rightUpwardSize;

	editRightUpwardSteps->setText(String(rightUpwardSize));
	editHorizontalSteps->setText(String(horizontalSize));
}

/// <summary>Called from MainComponent when one of the keys is clicked</summary>
/// <returns>Mapping was changed yes/no</returns>
bool IsomorphicMassAssign::performMouseDown(int setSelection, int keySelection)
{
	bool mappingChanged = false;
	jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS && keySelection >= 0 && keySelection < TerpstraSysExApplication::getApp().getOctaveBoardSize());

	int startNoteIndex = this->startingPointBox->getSelectedItemIndex();
	if (this->mappingLogic != nullptr && startNoteIndex >= 0)
	{
	    auto mainComponent = dynamic_cast<MainContentComponent*>(getParentComponent()->getParentComponent()->getParentComponent());

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
                for ( auto succeedinghorizLine : linesWithRightContinuation)
                {
                    int noteIndex = this->mappingLogic->terpstraKeyToIndex(
                        mainComponent->getMappingInEdit().sets[octaveBoardIndex-1].theKeys[succeedinghorizLine.getLast()]);

                    if ( noteIndex >= 0 )
                    {
                        // A field in preceding octave board with an assigned value has been found-
                        // Value can be assigned to continuation on current board.
                        auto newHorizLine = boardGeometry.continuationOfHorizontalLine(succeedinghorizLine, 1);

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
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="320"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <COMBOBOX name="startingPointBox" id="d526f69bdc196fea" memberName="startingPointBox"
            virtualName="" explicitFocusOrder="0" pos="8 368 150 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="labelStartingPoint" id="5401a3246c13771e" memberName="labelStartingPoint"
         virtualName="" explicitFocusOrder="0" pos="8 344 150 24" tooltip="Value that will be assigned to the key at mouse pposition when clicking"
         edTextCol="ff000000" edBkgCol="0" labelText="Starting value"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="labelHorizontalSteps" id="3e6663aecf1474c8" memberName="labelHorizontalSteps"
         virtualName="" explicitFocusOrder="0" pos="208 344 112 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Horizontal steps" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="editHorizontalSteps" id="8d2f5f07f337b9ef" memberName="editHorizontalSteps"
              virtualName="" explicitFocusOrder="0" pos="216 368 40 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="labelRightUpwardSteps" id="43530804741d9cb7" memberName="labelRightUpwardSteps"
         virtualName="" explicitFocusOrder="0" pos="152 296 136 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Right upward steps" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="editRightUpwardSteps" id="3a1cf8588366e0ca" memberName="editRightUpwardSteps"
              virtualName="" explicitFocusOrder="0" pos="160 320 39 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="editInstructionText" id="c03ef432c2b4599" memberName="editInstructionText"
         virtualName="" explicitFocusOrder="0" pos="8 200 296 48" edTextCol="ff000000"
         edBkgCol="0" labelText="Fill a line or the whole field with constant step distances. &#10;Click on desired key field to start."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="9"/>
  <GROUPCOMPONENT name="groupMapping" id="fbb69100a7b12118" memberName="groupMapping"
                  virtualName="" explicitFocusOrder="0" pos="8 40 304 152" title="Mapping"/>
  <COMBOBOX name="cbMappingType" id="a7825b65cfb78392" memberName="cbMappingType"
            virtualName="" explicitFocusOrder="0" pos="64 64 240 24" editable="0"
            layout="33" items="MIDI notes, increasing order&#10;Scala KBM mappings"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="labelMappingType" id="d77d8a4b80130afc" memberName="labelMappingType"
         virtualName="" explicitFocusOrder="0" pos="20 64 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Type:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="btnScaleStructureEditor" id="23cc77cbad6653d7" memberName="btnScaleStructureEditor"
              virtualName="" explicitFocusOrder="0" pos="160 264 152 24" tooltip="Show/hide scale structure editor"
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
                virtualName="" explicitFocusOrder="0" pos="8 264 152 24" buttonText="Colour assignment"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

