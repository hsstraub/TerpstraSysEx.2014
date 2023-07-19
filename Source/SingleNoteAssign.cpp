/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.8

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "Main.h"
#include "EditActions.h"
//[/Headers]

#include "SingleNoteAssign.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SingleNoteAssign::SingleNoteAssign ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("SingleNoteAssign");
    noteAutoIncrButton.reset (new juce::ToggleButton ("noteAutoIncrButton"));
    addAndMakeVisible (noteAutoIncrButton.get());
    noteAutoIncrButton->setButtonText (TRANS("Notes-Per-Click"));
    noteAutoIncrButton->addListener (this);

    noteAutoIncrButton->setBounds (8, 232, 160, 24);

    channelAutoIncrButton.reset (new juce::ToggleButton ("channelAutoIncrButton"));
    addAndMakeVisible (channelAutoIncrButton.get());
    channelAutoIncrButton->setButtonText (TRANS("Channels, after Note #:"));
    channelAutoIncrButton->addListener (this);

    channelAutoIncrButton->setBounds (8, 264, 160, 24);

    setNoteToggleButton.reset (new juce::ToggleButton ("setNoteToggleButton"));
    addAndMakeVisible (setNoteToggleButton.get());
    setNoteToggleButton->setButtonText (TRANS("Note # (0-127):"));
    setNoteToggleButton->addListener (this);

    setNoteToggleButton->setBounds (8, 128, 112, 24);

    setChannelToggleButton.reset (new juce::ToggleButton ("setChannelToggleButton"));
    addAndMakeVisible (setChannelToggleButton.get());
    setChannelToggleButton->setButtonText (TRANS("Channel (1-16):"));
    setChannelToggleButton->addListener (this);

    setChannelToggleButton->setBounds (8, 160, 112, 24);

    setColourToggleButton.reset (new juce::ToggleButton ("setColourToggleButton"));
    addAndMakeVisible (setColourToggleButton.get());
    setColourToggleButton->setButtonText (TRANS("Key Colour: "));
    setColourToggleButton->addListener (this);

    setColourToggleButton->setBounds (8, 96, 112, 24);

    keyTypeToggleButton.reset (new juce::ToggleButton ("keyTypeToggleButton"));
    addAndMakeVisible (keyTypeToggleButton.get());
    keyTypeToggleButton->setButtonText (TRANS("Key type:"));
    keyTypeToggleButton->addListener (this);

    keyTypeToggleButton->setBounds (8, 64, 112, 24);

    keyTypeCombo.reset (new juce::ComboBox ("keyTypeCombo"));
    addAndMakeVisible (keyTypeCombo.get());
    keyTypeCombo->setEditableText (false);
    keyTypeCombo->setJustificationType (juce::Justification::centredLeft);
    keyTypeCombo->setTextWhenNothingSelected (juce::String());
    keyTypeCombo->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    keyTypeCombo->addItem (TRANS("Note on/Note off"), 1);
    keyTypeCombo->addItem (TRANS("Continuous controller"), 2);
    keyTypeCombo->addItem (TRANS("Lumatouch"), 3);
    keyTypeCombo->addItem (TRANS("Disabled"), 4);
    keyTypeCombo->addListener (this);

    keyTypeCombo->setBounds (120, 64, 192, 24);

    noteInput.reset (new juce::Slider ("noteInput"));
    addAndMakeVisible (noteInput.get());
    noteInput->setTooltip (TRANS("MIDI note or MIDI controller no. (for key type \'continuous controller\')"));
    noteInput->setRange (0, 127, 1);
    noteInput->setSliderStyle (juce::Slider::IncDecButtons);
    noteInput->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 56, 20);
    noteInput->addListener (this);

    noteInput->setBounds (120, 128, 112, 24);

    colourSubwindow.reset (new ColourEditComponent());
    addAndMakeVisible (colourSubwindow.get());
    colourSubwindow->setName ("colourSubwindow");

    colourSubwindow->setBounds (120, 96, 56, 24);

    autoIncrementLabel.reset (new juce::Label ("autoIncrementLabel",
                                               TRANS("Auto-Increment")));
    addAndMakeVisible (autoIncrementLabel.get());
    autoIncrementLabel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    autoIncrementLabel->setJustificationType (juce::Justification::centredLeft);
    autoIncrementLabel->setEditable (false, false, false);
    autoIncrementLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    autoIncrementLabel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    autoIncrementLabel->setBounds (8, 204, 111, 24);

    colourTextEditor.reset (new ColourTextEditor ("colourTextEditor", "60aac5"));
    addAndMakeVisible (colourTextEditor.get());
    colourTextEditor->setName ("colourTextEditor");

    colourTextEditor->setBounds (184, 96, 128, 24);

    channelInput.reset (new juce::Slider ("channelInput"));
    addAndMakeVisible (channelInput.get());
    channelInput->setRange (1, 16, 1);
    channelInput->setSliderStyle (juce::Slider::IncDecButtons);
    channelInput->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 56, 20);
    channelInput->addListener (this);

    channelInput->setBounds (120, 160, 112, 24);

    channelAutoIncrNoteInput.reset (new juce::Slider ("channelAutoIncrNoteInput"));
    addAndMakeVisible (channelAutoIncrNoteInput.get());
    channelAutoIncrNoteInput->setTooltip (TRANS("After reaching this note, the channel is incremented and the note is reset to 0."));
    channelAutoIncrNoteInput->setRange (0, 127, 1);
    channelAutoIncrNoteInput->setSliderStyle (juce::Slider::IncDecButtons);
    channelAutoIncrNoteInput->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 56, 20);
    channelAutoIncrNoteInput->addListener (this);

    channelAutoIncrNoteInput->setBounds (176, 264, 112, 24);


    //[UserPreSize]

    keyTypeToggleButton->setButtonText(translate("KeyType"));
    keyTypeToggleButton->setColour(ToggleButton::ColourIds::textColourId, toggleTextColour);

    ccFaderIsDefault.reset(new juce::ImageButton());
    ccFaderIsDefault->setClickingTogglesState(true);
    ccFaderIsDefault->addListener(this);
    ccFaderIsDefault->setTooltip(translate("Toggle CC polarity inversion. Default, arrow down: values increase with key press. Inverted, arrow up: values decrease with key press."));
    addAndMakeVisible(ccFaderIsDefault.get());
    ccFaderIsDefault->setToggleState(true, dontSendNotification);
    faderUpArrow = getArrowPath(Point<float>(0.5f, 1.0f), Point<float>(0.5f, 0.0f), 0.5f, 0.25f);
    faderDownArrow = getArrowPath(Point<float>(0.5f, 0.0f), Point<float>(0.5f, 1.0f), 0.5f, 0.75f);

    keyTypeCombo->getProperties().set(LumatoneEditorStyleIDs::popupMenuTargetWidth, 1);

    setColourToggleButton->setColour(ToggleButton::ColourIds::textColourId, toggleTextColour);
    colourSubwindow->setColour("ff60aac5");

    setNoteToggleButton->setColour(ToggleButton::ColourIds::textColourId, toggleTextColour);
    noteInput->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, controlBoxFontHeightScalar);

    setChannelToggleButton->setColour(ToggleButton::ColourIds::textColourId, toggleTextColour);
    channelInput->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, controlBoxFontHeightScalar);

    autoIncrementLabel->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::GothamNarrowMedium));
    autoIncrementLabel->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, controlBoxFontHeightScalar);

    noteAutoIncrButton->setButtonText(translate("NotesPerClick"));
    noteAutoIncrButton->setColour(ToggleButton::ColourIds::textColourId, toggleTextColour);

    channelAutoIncrButton->setColour(ToggleButton::ColourIds::textColourId, toggleTextColour);
    channelAutoIncrNoteInput->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, controlBoxFontHeightScalar);

    instructionsFont = TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::FranklinGothic);
    parametersFont = TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::GothamNarrowMedium);

    // TODO: load last active colour?
    colourTextEditor->addColourSelectionListener(this);

    // Set up FlexBox rows
    for (int i = 0; i <= SingleNoteFlexRows::channelIncrement; i++)
    {
        flexRows.add(FlexBox(
                             FlexBox::Direction::row,
                             FlexBox::Wrap::noWrap,
                             FlexBox::AlignContent::center,
                             FlexBox::AlignItems::center,
                             FlexBox::JustifyContent::flexStart));
    }

    //[/UserPreSize]

    setSize (320, 400);


    //[Constructor] You can add your own custom stuff here..
	setNoteToggleButton->setToggleState(true, juce::NotificationType::sendNotification);
	setChannelToggleButton->setToggleState(true, juce::NotificationType::sendNotification);
	setColourToggleButton->setToggleState(true, juce::NotificationType::sendNotification);
	keyTypeToggleButton->setToggleState(true, juce::NotificationType::sendNotification);
	keyTypeCombo->setSelectedId(LumatoneKeyType::noteOnNoteOff);
    //[/Constructor]
}

SingleNoteAssign::~SingleNoteAssign()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	colourSubwindow = nullptr;
    //[/Destructor_pre]

    noteAutoIncrButton = nullptr;
    channelAutoIncrButton = nullptr;
    setNoteToggleButton = nullptr;
    setChannelToggleButton = nullptr;
    setColourToggleButton = nullptr;
    keyTypeToggleButton = nullptr;
    keyTypeCombo = nullptr;
    noteInput = nullptr;
    colourSubwindow = nullptr;
    autoIncrementLabel = nullptr;
    colourTextEditor = nullptr;
    channelInput = nullptr;
    channelAutoIncrNoteInput = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SingleNoteAssign::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
    g.fillAll(Colour(0xff212626));

    Rectangle<float> bottomPart = getLocalBounds().toFloat().withTrimmedTop(instructionsAreaBounds.getBottom());

    Path topBack = getConnectedRoundedRectPath(instructionsAreaBounds.toFloat(), roundedCornerSize, Button::ConnectedEdgeFlags::ConnectedOnBottom);
    Path bottomBack = getConnectedRoundedRectPath(bottomPart, roundedCornerSize, Button::ConnectedEdgeFlags::ConnectedOnTop);

    g.setColour(getLookAndFeel().findColour(LumatoneEditorColourIDs::LightBackground));
    g.fillPath(topBack);

    g.setColour(Colour(0xff2d3135));
    g.fillPath(bottomBack);

    g.setColour(getLookAndFeel().findColour(LumatoneEditorColourIDs::InactiveText));
    g.setFont(instructionsFont);
    g.drawFittedText(translate("ManualAssignDirections"), instructionsBounds, Justification::centred, 2, 1.0f);

    g.setColour(Colours::darkgrey);
    float thickness = getHeight() * separatorThicknessScalar;
    g.drawLine(controlsX, separatorY, getWidth() - controlsX, separatorY, jmax(1.0f, thickness));

//    // Debug FlexBox positioning
//    Random r;
//    for (auto child : getChildren())
//    {
//        g.setColour(Colour(r.nextFloat(), r.nextFloat() * 0.5f + 0.5f, r.nextFloat() * 0.5f + 0.5f, 1.0f));
//        g.drawRect(child->getBounds());
//    }
//    for (int i = 0; i < flexBounds.size(); i++)
//    {
//        g.setColour(Colours::red);
//        g.drawRect(flexBounds[i]);
//    }
    //[/UserPaint]
}

void SingleNoteAssign::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    float w = getWidth();
    float h = getHeight();

    if (getParentComponent()) // This changes when the tab changes
        roundedCornerSize = roundToInt(getParentComponent()->getParentComponent()->getParentHeight() * ROUNDEDCORNERTOAPPHEIGHT);

    int controlAreaTop = roundToInt(h * controlAreaYScalar);

    int toggleHeight = roundToInt(h * toggleHeightScalar);
    int toggleHeightScaled = roundToInt(toggleHeight * GLOBALFONTSCALAR);

    int controlH = roundToInt(h * controlHeightScalar);
    int controlHScaled = roundToInt(controlH * GLOBALFONTSCALAR);

    int marginX = roundToInt(controlH * 0.6f);
    int marginY = roundToInt(h * yMarginScalar);
    int halfMarginX = roundToInt(marginX * 0.5f);
    int halfMarginY = roundToInt(marginY * 0.5f);

    int rowWidth = roundToInt(w - marginX * 2);
    int toggleWidthMargin = toggleHeight + marginX * 1.25f; // Toggle icon + combined margins

    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    instructionsAreaBounds.setBounds(0, 0, w, controlAreaTop);
    instructionsFont.setHeight(instructionsAreaBounds.getHeight() * fontHeightInBounds);
    controlsX = roundToInt(w * controlsXScalar);


    instructionsBounds.setBounds(marginX, 0, w - marginX * 2, controlAreaTop);

    parametersFont.setHeight(toggleHeightScaled);

    flexBounds.clear();
    auto tglTemplateItem = FlexItem().withHeight(toggleHeightScaled).withAlignSelf(FlexItem::AlignSelf::center);
    auto ctrlTemplateItem = FlexItem(FlexItem::autoValue, controlH).withFlex(1.0f);

    auto getTglWidth = [=](ToggleButton* btn) {
        return roundToInt(parametersFont.getStringWidthFloat(btn->getButtonText()) + toggleWidthMargin);
    };

    for (int r = 0; r <= SingleNoteFlexRows::channelIncrement; r++)
    {
        auto row = flexRows.getReference(r);
        row.items.clear();

        // Generalize row properties
        auto toggleItem = FlexItem(tglTemplateItem);
        auto setupToggleItem = [&] (ToggleButton* btn) {
            toggleItem.width = getTglWidth(btn);
            toggleItem.associatedComponent = btn;
        };

        auto controlItem = FlexItem(ctrlTemplateItem).withMargin(FlexItem::Margin(0, 0, 0, halfMarginX));

        auto getAndPerformBounds = [&] () {
            auto bounds = Rectangle<int>(controlsX, flexBounds[r - 1].getBottom() + halfMarginY, rowWidth, controlH);
            flexBounds.add(bounds);
            row.performLayout(bounds);
        };

        switch (r)
        {
            case SingleNoteFlexRows::keyType:
            {
                setupToggleItem(keyTypeToggleButton.get());
                row.items.add(toggleItem);

                controlItem.associatedComponent = keyTypeCombo.get();
                row.items.add(controlItem);

                flexBounds.add(Rectangle<int>(controlsX, controlAreaTop + halfMarginY, rowWidth, controlH));
                row.performLayout(flexBounds[r]);
                break;
            }
            case SingleNoteFlexRows::keyColour:
            {
                setupToggleItem(setColourToggleButton.get());
                row.items.add(toggleItem);

                auto colourItem = FlexItem(controlH * 1.5f, controlH, *colourSubwindow.get());
                colourItem.margin = FlexItem::Margin(0, halfMarginX, 0, halfMarginX);
                row.items.add(colourItem);

                controlItem.associatedComponent = colourTextEditor.get();
                row.items.add(controlItem);

                getAndPerformBounds();
                break;
            }
            case SingleNoteFlexRows::keyNum:
            {
                setupToggleItem(setNoteToggleButton.get());
                row.items.add(toggleItem);

                if (keyTypeCombo->getSelectedId() == LumatoneKeyType::continuousController)
                {
                    auto ccInvertItem = FlexItem(controlH, controlH, *ccFaderIsDefault.get());
                    ccInvertItem.margin = FlexItem::Margin(0, halfMarginX, 0, 0);
                    row.items.add(ccInvertItem);
                    controlItem.margin = FlexItem::Margin();
                }

                controlItem.associatedComponent = noteInput.get();
                row.items.add(controlItem);

                getAndPerformBounds();
                break;
            }
            case SingleNoteFlexRows::keyChannel:
            {
                setupToggleItem(setChannelToggleButton.get());
                row.items.add(toggleItem);

                controlItem.associatedComponent = channelInput.get();
                row.items.add(controlItem);

                getAndPerformBounds();

                // Position Auto-Increment section since channelIncrement is relative to it
                separatorY = channelInput->getBottom() + halfMarginY;
                autoIncrementLabel->setBounds(controlsX, separatorY + halfMarginY, w - controlsX, controlHScaled);
                noteAutoIncrButton->setBounds(controlsX, autoIncrementLabel->getBottom() + halfMarginY,
                                              rowWidth, toggleHeightScaled);
                break;
            }
            case SingleNoteFlexRows::channelIncrement:
            {
                setupToggleItem(channelAutoIncrButton.get());
                row.items.add(toggleItem);

                controlItem.associatedComponent = channelAutoIncrNoteInput.get();
                row.items.add(controlItem);

                flexBounds.add(Rectangle<int>(controlsX, noteAutoIncrButton->getBottom() + halfMarginY, rowWidth, controlH));
                row.performLayout(flexBounds[r]);
                break;
            }
            default:
                break;
        }
    }

    // Style adjustments
    colourTextEditor->applyFontToAllText(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::GothamNarrowMedium, controlHScaled * CONTROLBOXFONTHEIGHTSCALAR), true);

    noteInput->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, false, roundToInt(noteInput->getWidth() * incDecButtonTextBoxWidthScalar), noteInput->getHeight());

    if (keyTypeCombo->getSelectedId() == LumatoneKeyType::continuousController)
        redrawCCFlipBtn();

    channelInput->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, false, roundToInt(channelInput->getWidth() * incDecButtonTextBoxWidthScalar), channelInput->getHeight());

    channelAutoIncrNoteInput->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, false, roundToInt(channelAutoIncrNoteInput->getWidth() * incDecButtonTextBoxWidthScalar), channelAutoIncrNoteInput->getHeight());
    //[/UserResized]
}

void SingleNoteAssign::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == noteAutoIncrButton.get())
    {
        //[UserButtonCode_noteAutoIncrButton] -- add your button handler code here..
        //[/UserButtonCode_noteAutoIncrButton]
    }
    else if (buttonThatWasClicked == channelAutoIncrButton.get())
    {
        //[UserButtonCode_channelAutoIncrButton] -- add your button handler code here..
        bool fieldActive = channelAutoIncrButton->getToggleState();
        channelAutoIncrNoteInput->setEnabled(fieldActive);
        //[/UserButtonCode_channelAutoIncrButton]
    }
    else if (buttonThatWasClicked == setNoteToggleButton.get())
    {
        //[UserButtonCode_setNoteToggleButton] -- add your button handler code here..
		bool fieldActive = setNoteToggleButton->getToggleState();
		noteInput->setEnabled(fieldActive);
		noteAutoIncrButton->setEnabled(fieldActive);
        //[/UserButtonCode_setNoteToggleButton]
    }
    else if (buttonThatWasClicked == setChannelToggleButton.get())
    {
        //[UserButtonCode_setChannelToggleButton] -- add your button handler code here..
		bool fieldActive = setChannelToggleButton->getToggleState();
		channelInput->setEnabled(fieldActive);
		channelAutoIncrButton->setEnabled(fieldActive);
		channelAutoIncrNoteInput->setEnabled(fieldActive);
        //[/UserButtonCode_setChannelToggleButton]
    }
    else if (buttonThatWasClicked == setColourToggleButton.get())
    {
        //[UserButtonCode_setColourToggleButton] -- add your button handler code here..
		bool fieldActive = setColourToggleButton->getToggleState();
		colourSubwindow->setEnabled(fieldActive);
        colourTextEditor->setEnabled(fieldActive);
        //[/UserButtonCode_setColourToggleButton]
    }
    else if (buttonThatWasClicked == keyTypeToggleButton.get())
    {
        //[UserButtonCode_keyTypeToggleButton] -- add your button handler code here..
		bool fieldActive = keyTypeToggleButton->getToggleState();
		keyTypeCombo->setEnabled(fieldActive);
        //[/UserButtonCode_keyTypeToggleButton]
    }

    //[UserbuttonClicked_Post]
    else if (buttonThatWasClicked == ccFaderIsDefault.get())
    {
    }
    //[/UserbuttonClicked_Post]
}

void SingleNoteAssign::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == keyTypeCombo.get())
    {
        //[UserComboBoxCode_keyTypeCombo] -- add your combo box handling code here..

        // Label the "note box" accordingly (controller no. for key type "Fader")
        if (keyTypeCombo->getSelectedId() == LumatoneKeyType::continuousController)
        {
            setNoteToggleButton->setButtonText("CC #:");
            ccFaderIsDefault->setVisible(true);
        }
        else
        {
            setNoteToggleButton->setButtonText("Note # (0-127):");
            ccFaderIsDefault->setVisible(false);
        }

        resized();

        //[/UserComboBoxCode_keyTypeCombo]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void SingleNoteAssign::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == noteInput.get())
    {
        //[UserSliderCode_noteInput] -- add your slider handling code here..
        //[/UserSliderCode_noteInput]
    }
    else if (sliderThatWasMoved == channelInput.get())
    {
        //[UserSliderCode_channelInput] -- add your slider handling code here..
        //[/UserSliderCode_channelInput]
    }
    else if (sliderThatWasMoved == channelAutoIncrNoteInput.get())
    {
        //[UserSliderCode_channelAutoIncrNoteInput] -- add your slider handling code here..
        //[/UserSliderCode_channelAutoIncrNoteInput]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void SingleNoteAssign::lookAndFeelChanged()
{
    auto lookAndFeel = dynamic_cast<LumatoneEditorLookAndFeel*>(&getLookAndFeel());
    if (lookAndFeel)
    {
        lookAndFeel->setupTextEditor(*colourTextEditor);
    }
}

void SingleNoteAssign::colourChangedCallback(ColourSelectionBroadcaster* source, Colour newColour)
{
    if (source != colourTextEditor.get())
    {
        colourTextEditor->setText(newColour.toDisplayString(false), false);
    }

    colourSubwindow->setColour(colourTextEditor->getText());
}

/// <summary>Called from parent when one of the keys is clicked</summary>
/// <returns>Pointer to undoable action to be passed to the undo manager. The latter has to be done in calling function.</returns>
UndoableAction* SingleNoteAssign::createEditAction(int setSelection, int keySelection)
{
	int newNote = noteInput->getValue();
	int newChannel = channelInput->getValue();

	auto editAction = new Lumatone::SingleNoteAssignAction(
		setSelection, keySelection,
		keyTypeToggleButton->getToggleState(), setChannelToggleButton->getToggleState(),
		setNoteToggleButton->getToggleState(), setColourToggleButton->getToggleState(),
        (LumatoneKeyType)keyTypeCombo->getSelectedId() == LumatoneKeyType::continuousController,
		(LumatoneKeyType)keyTypeCombo->getSelectedId(), newChannel,
		newNote, colourSubwindow->getColourAsObject(), ccFaderIsDefault->getToggleState());

	jassert(editAction != nullptr && editAction->isValid());

	// Auto increment note
	if (noteAutoIncrButton->getToggleState())
	{
		newNote++;

		// Auto increment channel
		if (channelAutoIncrButton->getToggleState() && channelAutoIncrNoteInput->getValue() > 0 &&
			newNote > channelAutoIncrNoteInput->getValue())
		{
			newNote = 0;
			newChannel++;
			if (newChannel > 16)
				newChannel = 1;
			channelInput->setValue(newChannel);
		}

		if (newNote > 127)
			newNote = 0;

		noteInput->setValue(newNote);
	}

	return editAction;
}

void SingleNoteAssign::onSetData(TerpstraKeyMapping& newData)
{
    // TODO: switch to palettes & active colour
	//SortedSet<int> usedColours = newData.getUsedColours();
	//for (int pos = 0; pos < usedColours.size(); pos++)
	//	colourSubwindow->addColourToBox(usedColours[pos]);
}

void SingleNoteAssign::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	setNoteToggleButton->setToggleState(
		propertiesFile->getBoolValue("SingleNoteNoteSetActive", true),
		juce::NotificationType::sendNotification);
	setChannelToggleButton->setToggleState(
		propertiesFile->getBoolValue("SingleNoteChannelSetActive", true),
		juce::NotificationType::sendNotification);
	setColourToggleButton->setToggleState(
		propertiesFile->getBoolValue("SingleNoteColourSetActive", true),
		juce::NotificationType::sendNotification);
	keyTypeToggleButton->setToggleState(
		propertiesFile->getBoolValue("SingleNoteKeyTypeSetActive", true),
		juce::NotificationType::sendNotification);
}

void SingleNoteAssign::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	propertiesFile->setValue("SingleNoteNoteSetActive", setNoteToggleButton->getToggleState());
	propertiesFile->setValue("SingleNoteChannelSetActive", setChannelToggleButton->getToggleState());
	propertiesFile->setValue("SingleNoteColourSetActive", setColourToggleButton->getToggleState());
	propertiesFile->setValue("SingleNoteKeyTypeSetActive", keyTypeToggleButton->getToggleState());
}

void SingleNoteAssign::redrawCCFlipBtn()
{
    Path arrowPath, faderPath, arrowInvertedPath, faderInvertedPath;
    getCCPolarityIconPath(false, arrowPath, faderPath);
    getCCPolarityIconPath(true, arrowInvertedPath, faderInvertedPath);
    auto defaultImg = Image(Image::PixelFormat::ARGB, ccFaderIsDefault->getWidth(), ccFaderIsDefault->getHeight(), true);
    auto invertedImg = defaultImg.createCopy();

    Graphics g(defaultImg);
    Graphics gi(invertedImg);

    auto transform = AffineTransform::scale(defaultImg.getWidth(), defaultImg.getHeight());
    arrowPath.applyTransform(transform);
    faderPath.applyTransform(transform);
    arrowInvertedPath.applyTransform(transform);
    faderInvertedPath.applyTransform(transform);

    auto lookAndFeel = &TerpstraSysExApplication::getApp().getLookAndFeel();
    auto background = lookAndFeel->findColour(TextButton::ColourIds::buttonColourId);
    g.setColour(background);
    g.fillRoundedRectangle(ccFaderIsDefault->getLocalBounds().toFloat(), 5.0f);
    gi.setColour(background);
    gi.fillRoundedRectangle(ccFaderIsDefault->getLocalBounds().toFloat(), 5.0f);

    auto arrowStroke = PathStrokeType(PHI, PathStrokeType::JointStyle::curved);
    auto colour = lookAndFeel->findColour(LumatoneEditorColourIDs::ActiveText).brighter(0.1);
    g.setColour(colour);
    g.strokePath(arrowPath, arrowStroke);
    g.fillPath(faderPath);

    gi.setColour(colour);
    gi.strokePath(arrowInvertedPath, arrowStroke);
    gi.fillPath(faderInvertedPath);

//    auto mouseOverImg = ccFaderFlipBtn->getToggleState() ? &invertedImg : &defaultImg;
    auto highlight = Colours::white.withAlpha(0.0333f);

    ccFaderIsDefault->setImages(false, false, true,
      invertedImg, 1.0f, Colour(),
      invertedImg, 1.0f, highlight,
      defaultImg, 1.0f, Colour());
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SingleNoteAssign" componentName="SingleNoteAssign"
                 parentClasses="public Component, public ColourSelectionListener, public TextEditor::Listener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="320"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <TOGGLEBUTTON name="noteAutoIncrButton" id="49829699593b11f7" memberName="noteAutoIncrButton"
                virtualName="" explicitFocusOrder="0" pos="8 232 160 24" buttonText="Notes-Per-Click"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="channelAutoIncrButton" id="1749290d10236ec3" memberName="channelAutoIncrButton"
                virtualName="" explicitFocusOrder="0" pos="8 264 160 24" buttonText="Channels, after Note #:"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="setNoteToggleButton" id="79f2522d584925d1" memberName="setNoteToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 128 112 24" buttonText="Note # (0-127):"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="setChannelToggleButton" id="f79f82eef6095c3c" memberName="setChannelToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 160 112 24" buttonText="Channel (1-16):"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="setColourToggleButton" id="fb41f2b9539dfb3f" memberName="setColourToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 96 112 24" buttonText="Key Colour: "
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="keyTypeToggleButton" id="3f2eba6027c4f2f5" memberName="keyTypeToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 64 112 24" buttonText="Key type:"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="keyTypeCombo" id="6a64d9cabf0d810f" memberName="keyTypeCombo"
            virtualName="" explicitFocusOrder="0" pos="120 64 192 24" editable="0"
            layout="33" items="Note on/Note off&#10;Continuous controller&#10;Lumatouch&#10;Disabled"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="noteInput" id="5858bc8c893dc11" memberName="noteInput"
          virtualName="" explicitFocusOrder="0" pos="120 128 112 24" tooltip="MIDI note or MIDI controller no. (for key type 'continuous controller')"
          min="0.0" max="127.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="56" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <GENERICCOMPONENT name="colourSubwindow" id="e66042f7ac61358f" memberName="colourSubwindow"
                    virtualName="ColourEditComponent" explicitFocusOrder="0" pos="120 96 56 24"
                    class="ColourEditComponent" params=""/>
  <LABEL name="autoIncrementLabel" id="5657d893807891e2" memberName="autoIncrementLabel"
         virtualName="" explicitFocusOrder="0" pos="8 204 111 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Auto-Increment" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="colourTextEditor" id="b2537113bfe0b3f9" memberName="colourTextEditor"
                    virtualName="ColourTextEditor" explicitFocusOrder="0" pos="184 96 128 24"
                    class="ColourTextEditor" params="&quot;colourTextEditor&quot;, &quot;60aac5&quot;"/>
  <SLIDER name="channelInput" id="acf3b651919a83fd" memberName="channelInput"
          virtualName="" explicitFocusOrder="0" pos="120 160 112 24" min="1.0"
          max="16.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="56" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="channelAutoIncrNoteInput" id="6f0004856a09c2f5" memberName="channelAutoIncrNoteInput"
          virtualName="" explicitFocusOrder="0" pos="176 264 112 24" tooltip="After reaching this note, the channel is incremented and the note is reset to 0."
          min="0.0" max="127.0" int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="56" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

