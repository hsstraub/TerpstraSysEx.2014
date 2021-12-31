/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.1.3

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

    startingPointBox.reset (new juce::ComboBox ("startingPointBox"));
    addAndMakeVisible (startingPointBox.get());
    startingPointBox->setEditableText (false);
    startingPointBox->setJustificationType (juce::Justification::centredLeft);
    startingPointBox->setTextWhenNothingSelected (juce::String());
    startingPointBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    startingPointBox->addListener (this);

    startingPointBox->setBounds (8, 368, 144, 24);

    labelStartingPoint.reset (new juce::Label ("labelStartingPoint",
                                               TRANS("Starting value")));
    addAndMakeVisible (labelStartingPoint.get());
    labelStartingPoint->setTooltip (TRANS("Value that will be assigned to the key at mouse pposition when clicking"));
    labelStartingPoint->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelStartingPoint->setJustificationType (juce::Justification::centredLeft);
    labelStartingPoint->setEditable (false, false, false);
    labelStartingPoint->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelStartingPoint->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelStartingPoint->setBounds (8, 344, 150, 24);

    labelHorizontalSteps.reset (new juce::Label ("labelHorizontalSteps",
                                                 TRANS("Horizontal steps")));
    addAndMakeVisible (labelHorizontalSteps.get());
    labelHorizontalSteps->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelHorizontalSteps->setJustificationType (juce::Justification::centredLeft);
    labelHorizontalSteps->setEditable (false, false, false);
    labelHorizontalSteps->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelHorizontalSteps->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelHorizontalSteps->setBounds (208, 344, 112, 24);

    editHorizontalSteps.reset (new juce::TextEditor ("editHorizontalSteps"));
    addAndMakeVisible (editHorizontalSteps.get());
    editHorizontalSteps->setMultiLine (false);
    editHorizontalSteps->setReturnKeyStartsNewLine (false);
    editHorizontalSteps->setReadOnly (false);
    editHorizontalSteps->setScrollbarsShown (true);
    editHorizontalSteps->setCaretVisible (true);
    editHorizontalSteps->setPopupMenuEnabled (true);
    editHorizontalSteps->setText (juce::String());

    editHorizontalSteps->setBounds (216, 368, 40, 24);

    labelRightUpwardSteps.reset (new juce::Label ("labelRightUpwardSteps",
                                                  TRANS("Right upward steps")));
    addAndMakeVisible (labelRightUpwardSteps.get());
    labelRightUpwardSteps->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelRightUpwardSteps->setJustificationType (juce::Justification::centredLeft);
    labelRightUpwardSteps->setEditable (false, false, false);
    labelRightUpwardSteps->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelRightUpwardSteps->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelRightUpwardSteps->setBounds (160, 312, 136, 24);

    editRightUpwardSteps.reset (new juce::TextEditor ("editRightUpwardSteps"));
    addAndMakeVisible (editRightUpwardSteps.get());
    editRightUpwardSteps->setMultiLine (false);
    editRightUpwardSteps->setReturnKeyStartsNewLine (false);
    editRightUpwardSteps->setReadOnly (false);
    editRightUpwardSteps->setScrollbarsShown (true);
    editRightUpwardSteps->setCaretVisible (true);
    editRightUpwardSteps->setPopupMenuEnabled (true);
    editRightUpwardSteps->setText (juce::String());

    editRightUpwardSteps->setBounds (168, 336, 39, 24);

    editInstructionText.reset (new juce::Label ("editInstructionText",
                                                TRANS("Fill a line or the whole field with constant step distances. \n"
                                                "Click on desired key field to start.")));
    addAndMakeVisible (editInstructionText.get());
    editInstructionText->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    editInstructionText->setJustificationType (juce::Justification::topLeft);
    editInstructionText->setEditable (false, false, false);
    editInstructionText->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    editInstructionText->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    editInstructionText->setBounds (8, 200, 296, 48);

    groupMapping.reset (new juce::GroupComponent ("groupMapping",
                                                  TRANS("Mapping")));
    addAndMakeVisible (groupMapping.get());

    groupMapping->setBounds (8, 40, 304, 152);

    cbMappingType.reset (new juce::ComboBox ("cbMappingType"));
    addAndMakeVisible (cbMappingType.get());
    cbMappingType->setEditableText (false);
    cbMappingType->setJustificationType (juce::Justification::centredLeft);
    cbMappingType->setTextWhenNothingSelected (juce::String());
    cbMappingType->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbMappingType->addItem (TRANS("MIDI notes, increasing order"), 1);
    cbMappingType->addItem (TRANS("Scala KBM mappings"), 2);
    cbMappingType->addListener (this);

    cbMappingType->setBounds (64, 64, 240, 24);

    labelMappingType.reset (new juce::Label ("labelMappingType",
                                             TRANS("Type:")));
    addAndMakeVisible (labelMappingType.get());
    labelMappingType->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelMappingType->setJustificationType (juce::Justification::centredLeft);
    labelMappingType->setEditable (false, false, false);
    labelMappingType->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelMappingType->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelMappingType->setBounds (20, 64, 88, 24);

    btnScaleStructureEditor.reset (new juce::TextButton ("btnScaleStructureEditor"));
    addAndMakeVisible (btnScaleStructureEditor.get());
    btnScaleStructureEditor->setTooltip (TRANS("Show/hide scale structure editor"));
    btnScaleStructureEditor->setButtonText (TRANS("Scale Wheel"));
    btnScaleStructureEditor->addListener (this);

    btnScaleStructureEditor->setBounds (160, 278, 152, 32);

    periodSizeBox.reset (new juce::ComboBox ("periodSizeBox"));
    addAndMakeVisible (periodSizeBox.get());
    periodSizeBox->setTooltip (TRANS("Number of tones per period interval (octave)"));
    periodSizeBox->setEditableText (false);
    periodSizeBox->setJustificationType (juce::Justification::centredLeft);
    periodSizeBox->setTextWhenNothingSelected (juce::String());
    periodSizeBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    periodSizeBox->addListener (this);

    periodSizeBox->setBounds (200, 8, 56, 24);

    labelPeriodSize.reset (new juce::Label ("labelPeriodSize",
                                            TRANS("Period:")));
    addAndMakeVisible (labelPeriodSize.get());
    labelPeriodSize->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelPeriodSize->setJustificationType (juce::Justification::centredLeft);
    labelPeriodSize->setEditable (false, false, false);
    labelPeriodSize->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelPeriodSize->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelPeriodSize->setBounds (16, 8, 168, 24);

    setColourToggleButton.reset (new juce::ToggleButton ("setColourToggleButton"));
    addAndMakeVisible (setColourToggleButton.get());
    setColourToggleButton->setButtonText (TRANS("Assign Colour"));
    setColourToggleButton->addListener (this);

    setColourToggleButton->setBounds (8, 264, 152, 24);

    setStepsToggleButton.reset (new juce::ToggleButton ("setStepsToggleButton"));
    addAndMakeVisible (setStepsToggleButton.get());
    setStepsToggleButton->setTooltip (TRANS("Allow editing steps through the Scale Wheel"));
    setStepsToggleButton->setButtonText (TRANS("Assign Steps"));
    setStepsToggleButton->addListener (this);

    setStepsToggleButton->setBounds (8, 296, 119, 24);


    //[UserPreSize]
    editInstructionText->setVisible(false);
    groupMapping->setVisible(false);

    labelPeriodSize->attachToComponent(periodSizeBox.get(), true);
    labelMappingType->attachToComponent(cbMappingType.get(), true);
    labelStartingPoint->attachToComponent(startingPointBox.get(), true);
    //labelHorizontalSteps->attachToComponent(editHorizontalSteps.get(), false);
    //labelRightUpwardSteps->attachToComponent(editRightUpwardSteps.get(), false);

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
    setStepsToggleButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void IsomorphicMassAssign::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xffbad0de));

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

    int xMargin = roundToInt(width * xMarginScalar);
    int yMargin = roundToInt(height * yMarginScalar);
    
    auto areaWidth = width - (xMargin * 2);
    int controlHeight = roundToInt((height - yMargin * 2) * 0.04);
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    instructionsBounds = getLocalBounds().withBottom(round(height * instructionsBottom));
    instructionsFont.setHeight(instructionsBounds.proportionOfHeight(fontHeightInBounds));

    auto labelFont = labelPeriodSize->getFont();

    resizeLabelWithHeight(labelPeriodSize.get(), controlHeight);
    auto periodLabelWidth = labelFont.getStringWidth(labelPeriodSize->getText());
    periodSizeBox->setBounds(periodLabelWidth + xMargin, instructionsBounds.getBottom() + yMargin, areaWidth - periodLabelWidth, controlHeight);

    resizeLabelWithHeight(labelMappingType.get(), controlHeight);
    auto mappingTypeWidth = labelFont.getStringWidth(labelMappingType->getText());
    cbMappingType->setBounds(xMargin + mappingTypeWidth, periodSizeBox->getBottom() + yMargin, areaWidth - mappingTypeWidth, controlHeight);

    auto mappingTypeHeight = instructionsBounds.getHeight() + yMargin;
    Rectangle<int> mappingTypeBounds(xMargin, cbMappingType->getBottom() + yMargin, areaWidth, mappingTypeHeight);
    incrMidiNotesMapping->setBounds(mappingTypeBounds);
    kbmMappingDlg->setBounds(mappingTypeBounds);

    FlexBox scaleWheelBox;
    scaleWheelBox.flexWrap = FlexBox::Wrap::wrap;
    scaleWheelBox.flexDirection = FlexBox::Direction::column;
    scaleWheelBox.justifyContent = FlexBox::JustifyContent::center;

    auto scaleWheelRowHeight = controlHeight * 1.5;
    auto scaleWheelToggleWidth = labelFont.getStringWidth(setColourToggleButton->getButtonText()) + controlHeight;
    auto scaleWheelToggleHeight = scaleWheelRowHeight * 0.5;
    auto scaleWheelButtonMargin = controlHeight * 0.5;
    scaleWheelBox.items.add(FlexItem(areaWidth - scaleWheelToggleWidth, scaleWheelRowHeight, *btnScaleStructureEditor).withMargin(FlexItem::Margin(scaleWheelButtonMargin, 0, scaleWheelButtonMargin, 0)));
    scaleWheelBox.items.add(FlexItem(scaleWheelToggleWidth, scaleWheelToggleHeight, *setColourToggleButton));
    scaleWheelBox.items.add(FlexItem(scaleWheelToggleWidth, scaleWheelToggleHeight, *setStepsToggleButton));

    FlexBox flexbox;
    flexbox.flexWrap = FlexBox::Wrap::noWrap;
    flexbox.flexDirection = FlexBox::Direction::column;

    FlexItem::Margin itemYMargin;
    itemYMargin.top = yMargin;

    flexbox.items.add(FlexItem(areaWidth, scaleWheelRowHeight, scaleWheelBox));

    resizeLabelWithHeight(labelStartingPoint.get(), controlHeight);
    auto startingPointLabelWidth = labelFont.getStringWidth(labelStartingPoint->getText()) + (labelPeriodSize->getX());
    flexbox.items.add(FlexItem(areaWidth - startingPointLabelWidth, controlHeight, *startingPointBox).withMargin(FlexItem::Margin(yMargin, 0, 0, startingPointLabelWidth)));

    FlexBox stepsBox;
    stepsBox.flexDirection = FlexBox::Direction::column;

    auto stepsWidth = labelFont.getStringWidth("999");
    stepsBox.items.add(FlexItem(areaWidth, controlHeight, *labelHorizontalSteps).withMargin(itemYMargin));
    stepsBox.items.add(FlexItem(stepsWidth, controlHeight, *editHorizontalSteps).withMargin(itemYMargin));
    stepsBox.items.add(FlexItem(areaWidth, controlHeight, *labelRightUpwardSteps).withMargin(itemYMargin));
    stepsBox.items.add(FlexItem(stepsWidth, controlHeight, *editRightUpwardSteps).withMargin(itemYMargin));
    flexbox.items.add(FlexItem(stepsWidth, controlHeight * 5, stepsBox).withMargin(itemYMargin));
    
    flexbox.performLayout(getLocalBounds().withLeft(labelPeriodSize->getX())
                                          .withTrimmedRight(xMargin)
                                          .withTop(mappingTypeBounds.getBottom() + yMargin)
                                          .withBottom(height - yMargin));

    //btnScaleStructureEditor->   setBounds(setColourToggleButton->   getBounds().withLeft(setColourToggleButton->    getRight() + xMargin).withRight(xEnd));
    //startingPointBox->          setBounds(labelStartingPoint->      getBounds().withLeft(labelStartingPoint->       getRight()).withRight(xEnd));
    //periodSizeBox->             setBounds(labelPeriodSize->         getBounds().withLeft(labelPeriodSize->          getRight()).withRight(xEnd));
    //editHorizontalSteps->       setBounds(labelHorizontalSteps->    getBounds().withLeft(labelHorizontalSteps->     getRight()).withRight(xEnd));
    //editRightUpwardSteps->      setBounds(labelRightUpwardSteps->   getBounds().withLeft(labelRightUpwardSteps->    getRight()).withRight(xEnd));


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
    else if (buttonThatWasClicked == setStepsToggleButton.get())
    {
        //[UserButtonCode_setStepsToggleButton] -- add your button handler code here..
        //[/UserButtonCode_setStepsToggleButton]
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

    TerpstraKey& keyData = mainComponent->getMappingInEdit().sets[setSelection].theKeys[keySelection];
    keyData.keyType = LumatoneKeyType::noteOnNoteOff;

	// Save data
	this->mappingLogic->indexToTerpstraKey(
        noteIndex,
        keyData
		);

	// Send to device
	TerpstraSysExApplication::getApp().getLumatoneController()->sendKeyParam(setSelection + 1, keySelection,
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
	TerpstraSysExApplication::getApp().getLumatoneController().sendKeyParam(setSelection + 1, keySelection, keyData);
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

void IsomorphicMassAssign::colourChangedCallback(ColourSelectionBroadcaster* source, Colour newColour)
{
    auto palette = dynamic_cast<ColourPaletteComponent*>(source);
    if (palette)
    {
        auto newColours = palette->getColourPalette();

        int startIndex = newColours.indexOf(source->getSelectedColour());

        if (startIndex >= 0)
        {
            colourTable.clear();
            for (int i = 0; i < newColours.size(); i++)
            {
                colourTable.add(newColours[(i + startIndex) % newColours.size()]);
            }
        }
        else
        {
            colourTable = newColours;
        }

        mappingLogic->setColourTable(colourTable);
    }
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

        Point<int>startPos = boardGeometry.coordinatesForKey(setSelection, keySelection); // Get the coordinates of the clicked key.

        for (int boardIx = 0; boardIx < NUMBEROFBOARDS; boardIx++) { // For each board
            for (int keyIx = 0; keyIx < TerpstraSysExApplication::getApp().getOctaveBoardSize(); keyIx++) { // For each key on that board
                Point<int> keyPos = boardGeometry.coordinatesForKey(boardIx, keyIx); // Get the coordinates of the key to assign.
                Point<int> keyOffset = keyPos - startPos; // Get the offset of the key to assign, relative to where the user clicked.
                setSaveSend(
                    boardIx,
                    keyIx,
                    mod(startNoteIndex
                         + keyOffset.getX() * horizStepSize
                         + keyOffset.getY() * rUpwStepSize,
                        this->mappingLogic->globalMappingSize()));
            }
        }
        mappingChanged = true;
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
                 parentClasses="public Component, public MappingLogicBase::Listener, public ScaleStructureComponent::Listener, public ColourSelectionListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="320"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <COMBOBOX name="startingPointBox" id="d526f69bdc196fea" memberName="startingPointBox"
            virtualName="" explicitFocusOrder="0" pos="8 368 144 24" editable="0"
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
         virtualName="" explicitFocusOrder="0" pos="160 312 136 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Right upward steps" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="editRightUpwardSteps" id="3a1cf8588366e0ca" memberName="editRightUpwardSteps"
              virtualName="" explicitFocusOrder="0" pos="168 336 39 24" initialText=""
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
              virtualName="" explicitFocusOrder="0" pos="160 278 152 32" tooltip="Show/hide scale structure editor"
              buttonText="Scale Wheel" connectedEdges="0" needsCallback="1"
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
                virtualName="" explicitFocusOrder="0" pos="8 264 152 24" buttonText="Assign Colour"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="setStepsToggleButton" id="ea7fdc2125770501" memberName="setStepsToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 296 119 24" tooltip="Allow editing steps through the Scale Wheel"
                buttonText="Assign Steps" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

