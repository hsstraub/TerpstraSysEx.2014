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
#include "Main.h"
#include "MainComponent.h"
//[/Headers]

#include "SingleNoteAssign.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SingleNoteAssign::SingleNoteAssign ()
{
    keyTypeToggleButton.reset(new juce::ToggleButton("keyTypeToggleButton"));
    addAndMakeVisible(keyTypeToggleButton.get());
    keyTypeToggleButton->setButtonText(translate("KeyType"));
    keyTypeToggleButton->addListener(this);
    keyTypeToggleButton->setColour(ToggleButton::ColourIds::textColourId, toggleTextColour);

    keyTypeCombo.reset(new juce::ComboBox("keyTypeCombo"));
    addAndMakeVisible(keyTypeCombo.get());
    keyTypeCombo->setTextWhenNothingSelected(juce::String());
    keyTypeCombo->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    keyTypeCombo->addItem(translate("NoteOnOff"), 1);
    keyTypeCombo->addItem(translate("ContinuousController"), 2);
    keyTypeCombo->addItem("Lumatouch", 3);
    keyTypeCombo->getProperties().set(LumatoneEditorStyleIDs::fontOverride, "Gotham");
    keyTypeCombo->getProperties().set(LumatoneEditorStyleIDs::fontOverrideTypefaceStyle, "Narrow Medium");
    keyTypeCombo->addListener(this);

    setColourToggleButton.reset(new juce::ToggleButton("setColourToggleButton"));
    addAndMakeVisible(setColourToggleButton.get());
    setColourToggleButton->setButtonText(translate("KeyColour"));
    setColourToggleButton->addListener(this);
    setColourToggleButton->setColour(ToggleButton::ColourIds::textColourId, toggleTextColour);

    colourSubwindow.reset(new ColourEditComponent());
    addAndMakeVisible(colourSubwindow.get());
    //colourSubwindow->setColourAsString("ff60aac5");

    setNoteToggleButton.reset(new juce::ToggleButton("setNoteToggleButton"));
    addAndMakeVisible(setNoteToggleButton.get());
    setNoteToggleButton->setButtonText(TRANS("Note (0-127):"));
    setNoteToggleButton->addListener(this);
    setNoteToggleButton->setColour(ToggleButton::ColourIds::textColourId, toggleTextColour);

    noteBox.reset(new juce::ComboBox("noteBox"));
    addAndMakeVisible(noteBox.get());
    noteBox->setTooltip(TRANS("MIDI note or MIDI controller no. (for key type \'continuous controller\')"));
    noteBox->setTextWhenNothingSelected(juce::String());
    noteBox->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    noteBox->getProperties().set(LumatoneEditorStyleIDs::popupMenuMaxColumns, 8);
    noteBox->addListener(this);

    setChannelToggleButton.reset(new juce::ToggleButton("setChannelToggleButton"));
    addAndMakeVisible(setChannelToggleButton.get());
    setChannelToggleButton->setButtonText(translate("Channel") + " (1-16):");
    setChannelToggleButton->addListener(this);
    setChannelToggleButton->setColour(ToggleButton::ColourIds::textColourId, toggleTextColour);

    channelBox.reset(new juce::ComboBox("channelBox"));
    addAndMakeVisible(channelBox.get());
    channelBox->setTextWhenNothingSelected(juce::String());
    channelBox->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    channelBox->addListener(this);

    autoIncrementLabel.reset(new Label("AutoIncrementLabel", translate("AutoIncrement")));
    autoIncrementLabel->setFont(LumatoneEditorFonts::GothamNarrowMedium());
    autoIncrementLabel->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, 0.75f);
    addAndMakeVisible(*autoIncrementLabel);

    noteAutoIncrButton.reset(new juce::ToggleButton("noteAutoIncrButton"));
    addAndMakeVisible(noteAutoIncrButton.get());
    noteAutoIncrButton->setButtonText(translate("NotesPerClick"));
    noteAutoIncrButton->addListener(this);
    noteAutoIncrButton->setColour(ToggleButton::ColourIds::textColourId, toggleTextColour);

    channelAutoIncrButton.reset(new juce::ToggleButton("channelAutoIncrButton"));
    addAndMakeVisible(channelAutoIncrButton.get());
    channelAutoIncrButton->setButtonText(translate("ChannelsAfterNote"));
    channelAutoIncrButton->addListener(this);
    channelAutoIncrButton->setColour(ToggleButton::ColourIds::textColourId, toggleTextColour);

    channelAutoIncrNoteBox.reset(new juce::ComboBox("channelAutoIncrNoteBox"));
    addAndMakeVisible(channelAutoIncrNoteBox.get());
    channelAutoIncrNoteBox->setTooltip(TRANS("After reaching this note, the channel is incremented and the note is reset to 0."));
    channelAutoIncrNoteBox->setTextWhenNothingSelected(juce::String());
    channelAutoIncrNoteBox->setTextWhenNoChoicesAvailable(TRANS("(no choices)"));
    channelAutoIncrNoteBox->addListener(this);


    //[UserPreSize]
    //[/UserPreSize]


    //[Constructor] You can add your own custom stuff here..
	for (int i = 0; i <= 127; i++)
	{
		noteBox->addItem(String(i), i + 1);
		channelAutoIncrNoteBox->addItem(String(i), i + 1);
	}

	for (int i = 1; i <= 16; i++)
		channelBox->addItem(String(i), i);

	setNoteToggleButton->setToggleState(true, juce::NotificationType::sendNotification);
	setChannelToggleButton->setToggleState(true, juce::NotificationType::sendNotification);
	setColourToggleButton->setToggleState(true, juce::NotificationType::sendNotification);
	keyTypeToggleButton->setToggleState(true, juce::NotificationType::sendNotification);
	keyTypeCombo->setSelectedId(TerpstraKey::noteOnNoteOff);
    //[/Constructor]
}

SingleNoteAssign::~SingleNoteAssign()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	colourSubwindow = nullptr;
    autoIncrementLabel = nullptr;
    //[/Destructor_pre]
    
    noteBox = nullptr;
    noteAutoIncrButton = nullptr;
    channelBox = nullptr;
    channelAutoIncrButton = nullptr;
    channelAutoIncrNoteBox = nullptr;
    setNoteToggleButton = nullptr;
    setChannelToggleButton = nullptr;
    setColourToggleButton = nullptr;
    keyTypeToggleButton = nullptr;
    keyTypeCombo = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SingleNoteAssign::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]
    g.fillAll(Colour(0xff212626));

    Rectangle<float> bottomPart = getLocalBounds().toFloat().withTrimmedTop(instructionsBounds.getBottom());

    Path topBack = getConnectedRoundedRectPath(instructionsBounds.toFloat(), roundedCornerSize, Button::ConnectedEdgeFlags::ConnectedOnBottom);
    Path bottomBack = getConnectedRoundedRectPath(bottomPart, roundedCornerSize, Button::ConnectedEdgeFlags::ConnectedOnTop);

    g.setColour(getLookAndFeel().findColour(LumatoneEditorColourIDs::LightBackground));
    g.fillPath(topBack);

    g.setColour(Colour(0xff2d3135));
    g.fillPath(bottomBack);

    g.setColour(getLookAndFeel().findColour(LumatoneEditorColourIDs::InactiveText));
    g.setFont(instructionsFont);
    g.drawFittedText(translate("ManualAssignDirections"), instructionsBounds, Justification::centred, 2, 1.0f);

    g.setColour(Colours::darkgrey);
    g.drawLine(controlsX, separatorY, getWidth() - controlsX, separatorY);

    //[/UserPaint]
}

void SingleNoteAssign::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    if (getParentComponent()) // This changes when the tab changes
        roundedCornerSize = round(getParentComponent()->getParentComponent()->getParentHeight() * roundedCornerLayoutAppHeightScalar);

    int controlAreaTop = proportionOfHeight(controlAreaYScalar);

    instructionsBounds.setBounds(0, 0, getWidth(), controlAreaTop);
    instructionsFont.setHeight(instructionsBounds.proportionOfHeight(fontHeightInBounds));

    controlsX = proportionOfWidth(controlsXScalar);
    int marginY = proportionOfHeight(yMarginScalar);
    int controlH = proportionOfHeight(controlHeightScalar);
    int marginX = round(controlH / 2.0f);
    int halfWidth = proportionOfWidth(0.5f);
    int toggleHeight = proportionOfHeight(toggleHeightScalar);
    int halfMarginY = round(marginY / 2.0f);

    parametersFont.setHeight(toggleHeight * 1.25f);
    int comboBoxWidth = round(parametersFont.getStringWidth("127_") * 2);

    keyTypeToggleButton->setTopLeftPosition(controlsX, instructionsBounds.getBottom() + halfMarginY);
    resizeToggleButtonWithHeight(keyTypeToggleButton.get(), parametersFont, toggleHeight);
    keyTypeCombo->setSize(getWidth() - marginX * 2 - keyTypeToggleButton->getRight(), controlH);
    keyTypeCombo->setCentrePosition(
        round(keyTypeCombo->getWidth() / 2.0f) + keyTypeToggleButton->getRight(),
        keyTypeToggleButton->getBounds().getCentreY()
    );

    setColourToggleButton->setTopLeftPosition(controlsX, keyTypeToggleButton->getBottom() + marginY);
    resizeToggleButtonWithHeight(setColourToggleButton.get(), parametersFont, toggleHeight);
    colourSubwindow->setSize(controlH * 1.5f, controlH);
    colourSubwindow->setCentrePosition(
        round(colourSubwindow->getWidth() / 2.0f) + setColourToggleButton->getRight(),
        setColourToggleButton->getBounds().getCentreY()
    );

    setNoteToggleButton->setTopLeftPosition(controlsX, setColourToggleButton->getBottom() + marginY);
    resizeToggleButtonWithHeight(setNoteToggleButton.get(), parametersFont, toggleHeight);
    noteBox->setSize(comboBoxWidth, controlH);
    noteBox->setCentrePosition(
        round(noteBox->getWidth() / 2.0f) + setNoteToggleButton->getRight(),
        setNoteToggleButton->getBounds().getCentreY()
    );

    setChannelToggleButton->setTopLeftPosition(controlsX, setNoteToggleButton->getBottom() + marginY);
    resizeToggleButtonWithHeight(setChannelToggleButton.get(), parametersFont, toggleHeight);
    channelBox->setSize(comboBoxWidth, controlH);
    channelBox->setCentrePosition(
        round(channelBox->getWidth() / 2.0f) + setChannelToggleButton->getRight(),
        setChannelToggleButton->getBounds().getCentreY()
    );

    separatorY = setChannelToggleButton->getBottom() + halfMarginY;

    autoIncrementLabel->setBounds(controlsX, separatorY + halfMarginY, getWidth() - controlsX, controlH);

    noteAutoIncrButton->setTopLeftPosition(controlsX, autoIncrementLabel->getBottom() + halfMarginY);
    resizeToggleButtonWithHeight(noteAutoIncrButton.get(), parametersFont, toggleHeight);

    channelAutoIncrButton->setTopLeftPosition(controlsX, noteAutoIncrButton->getBottom() + halfMarginY);
    resizeToggleButtonWithHeight(channelAutoIncrButton.get(), parametersFont, toggleHeight);
    channelAutoIncrNoteBox->setSize(comboBoxWidth, controlH);
    channelAutoIncrNoteBox->setCentrePosition(
        round(channelAutoIncrNoteBox->getWidth() / 2.0f) + channelAutoIncrButton->getRight(),
        channelAutoIncrButton->getBounds().getCentreY()
    );
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SingleNoteAssign::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == noteBox.get())
    {
        //[UserComboBoxCode_noteBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_noteBox]
    }
    else if (comboBoxThatHasChanged == channelBox.get())
    {
        //[UserComboBoxCode_channelBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_channelBox]
    }
    else if (comboBoxThatHasChanged == channelAutoIncrNoteBox.get())
    {
        //[UserComboBoxCode_channelAutoIncrNoteBox] -- add your combo box handling code here..
        //[/UserComboBoxCode_channelAutoIncrNoteBox]
    }
    else if (comboBoxThatHasChanged == keyTypeCombo.get())
    {
        //[UserComboBoxCode_keyTypeCombo] -- add your combo box handling code here..

        // Label the "note box" accordingly (controller no. for key type "Fader")
        if (keyTypeCombo->getSelectedId() == TerpstraKey::KEYTYPE::continuousController)
        {
            setNoteToggleButton->setButtonText("CC Type:");

            // ToDo Auto increment does not make sense in this case?
        }
        else
        {
            setNoteToggleButton->setButtonText("Note (0-127):");
        }

        //[/UserComboBoxCode_keyTypeCombo]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
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
        //[/UserButtonCode_channelAutoIncrButton]
    }
    else if (buttonThatWasClicked == setNoteToggleButton.get())
    {
        //[UserButtonCode_setNoteToggleButton] -- add your button handler code here..
		bool fieldActive = setNoteToggleButton->getToggleState();
		noteBox->setEnabled(fieldActive);
		noteAutoIncrButton->setEnabled(fieldActive);
        //[/UserButtonCode_setNoteToggleButton]
    }
    else if (buttonThatWasClicked == setChannelToggleButton.get())
    {
        //[UserButtonCode_setChannelToggleButton] -- add your button handler code here..
		bool fieldActive = setChannelToggleButton->getToggleState();
		channelBox->setEnabled(fieldActive);
		channelAutoIncrButton->setEnabled(fieldActive);
		channelAutoIncrNoteBox->setEnabled(fieldActive);
        //[/UserButtonCode_setChannelToggleButton]
    }
    else if (buttonThatWasClicked == setColourToggleButton.get())
    {
        //[UserButtonCode_setColourToggleButton] -- add your button handler code here..
		bool fieldActive = setColourToggleButton->getToggleState();
		colourSubwindow->setEnabled(fieldActive);
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
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

/// <summary>Called from MainComponent when one of the keys is clicked</summary>
/// <returns>Mapping was changed yes/no</returns>
bool SingleNoteAssign::performMouseDown(int setSelection, int keySelection)
{
	bool mappingChanged = false;
	jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS && keySelection >= 0 && keySelection < TERPSTRABOARDSIZE);

	TerpstraKey& keyData = (dynamic_cast<MainContentComponent*>(getParentComponent()->getParentComponent()->getParentComponent()))->getMappingInEdit().sets[setSelection].theKeys[keySelection];

	// Set note if specified
	if (setNoteToggleButton->getToggleState())
	{
		keyData.noteNumber = noteBox->getSelectedItemIndex(); //-1 for no selection or 0-127
		if (keyData.noteNumber < 0) keyData.noteNumber = 0;
		mappingChanged = true;
	}

	// Set channel if specified
	if (setChannelToggleButton->getToggleState())
	{
		keyData.channelNumber = channelBox->getSelectedId();	// 0 for no selection or 1-16
		mappingChanged = true;
	}

	// Set colour if specified
	if (setColourToggleButton->getToggleState())
	{
		keyData.colour = colourSubwindow->getColourAsNumber();
		mappingChanged = true;
	}

	// Set key type if specified
	if (keyTypeToggleButton->getToggleState())
	{
		keyData.keyType = (TerpstraKey::KEYTYPE)keyTypeCombo->getSelectedId();	// XXX if no selection?
		mappingChanged = true;
	}

	// Send to device
	TerpstraSysExApplication::getApp().getMidiDriver().sendKeyParam(setSelection + 1, keySelection, keyData);

	// Auto increment note
	if (noteAutoIncrButton->getToggleState())
	{
		int newNote = keyData.noteNumber + 1;

		// Auto increment channel
		if (channelAutoIncrButton->getToggleState() && channelAutoIncrNoteBox->getSelectedItemIndex() > 0 &&
			newNote > channelAutoIncrNoteBox->getSelectedItemIndex())
		{
			newNote = 0;
			int newChannel = keyData.channelNumber + 1;
			if (newChannel > 16)
				newChannel = 1;
			channelBox->setSelectedId(newChannel);
		}

		if (newNote > 127)
			newNote = 0;

		noteBox->setSelectedItemIndex(newNote);
	}

	return mappingChanged;
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

void SingleNoteAssign::listenForPaletteWindowRequest(TextButton::Listener* listenerIn)
{
    colourSubwindow->addListener(listenerIn);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SingleNoteAssign" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="320" initialHeight="400">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <LABEL name="editInstructionText" id="c03ef432c2b4599" memberName="editInstructionText"
         virtualName="" explicitFocusOrder="0" pos="8 8 304 32" edTextCol="ff000000"
         edBkgCol="0" labelText="Define which values you'd like to apply to a key, and then click on the desired key-face"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <COMBOBOX name="noteBox" id="123cacc6155f964" memberName="noteBox" virtualName=""
            explicitFocusOrder="0" pos="120 128 56 24" tooltip="MIDI note or MIDI controller no. (for key type 'continuous controller')"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TOGGLEBUTTON name="noteAutoIncrButton" id="49829699593b11f7" memberName="noteAutoIncrButton"
                virtualName="" explicitFocusOrder="0" pos="8 232 160 24" buttonText="Notes-Per-Click"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="channelBox" id="208bbc8901c22319" memberName="channelBox"
            virtualName="" explicitFocusOrder="0" pos="120 160 56 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TOGGLEBUTTON name="channelAutoIncrButton" id="1749290d10236ec3" memberName="channelAutoIncrButton"
                virtualName="" explicitFocusOrder="0" pos="8 264 160 24" buttonText="Channels, after Note #"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="channelAutoIncrNoteBox" id="4560285c5e467e2f" memberName="channelAutoIncrNoteBox"
            virtualName="" explicitFocusOrder="0" pos="176 264 56 24" tooltip="After reaching this note, the channel is incremented and the note is reset to 0."
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TOGGLEBUTTON name="setNoteToggleButton" id="79f2522d584925d1" memberName="setNoteToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 128 112 24" buttonText="Note (0-127):"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="setChannelToggleButton" id="f79f82eef6095c3c" memberName="setChannelToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 160 112 24" buttonText="Channel (1-16):"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="setColourToggleButton" id="fb41f2b9539dfb3f" memberName="setColourToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 96 112 24" buttonText="Key Colour:"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="keyTypeToggleButton" id="3f2eba6027c4f2f5" memberName="keyTypeToggleButton"
                virtualName="" explicitFocusOrder="0" pos="8 64 112 24" buttonText="Key type:"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <COMBOBOX name="keyTypeCombo" id="6a64d9cabf0d810f" memberName="keyTypeCombo"
            virtualName="" explicitFocusOrder="0" pos="120 64 192 24" editable="0"
            layout="33" items="Note on/Note off&#10;Continuous controller&#10;Note on/Note off with Lumatouch"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <GROUPCOMPONENT name="autoIncrementgroup" id="249745a33736e282" memberName="juce__groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="0 209 240 95" title="Auto-Increment:"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

