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
//[/Headers]

#include "IncrMidiNotesMapping.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
IncrMidiNotesMapping::IncrMidiNotesMapping (int& periodSizeReference, ScaleStructure& scaleStructureIn, Array<Colour>& colourTableIn)
    : periodSize(periodSizeReference),  mappingLogic(scaleStructureIn, colourTableIn)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    channelAutoIncrButton.reset (new juce::ToggleButton ("channelAutoIncrButton"));
    addAndMakeVisible (channelAutoIncrButton.get());
    channelAutoIncrButton->setTooltip (TRANS("Auto-increment channel after maximal note is reached"));
    channelAutoIncrButton->setButtonText (TRANS("Multichannel"));
    channelAutoIncrButton->setConnectedEdges (juce::Button::ConnectedOnTop);
    channelAutoIncrButton->setRadioGroupId (1);
    channelAutoIncrButton->addListener (this);

    channelAutoIncrButton->setBounds (8, 64, 280, 24);

    labelMidiNotesUntil.reset (new juce::Label ("labelMidiNotesUntil",
                                                TRANS("MIDI notes from 0 to the scale size")));
    addAndMakeVisible (labelMidiNotesUntil.get());
    labelMidiNotesUntil->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelMidiNotesUntil->setJustificationType (juce::Justification::centredLeft);
    labelMidiNotesUntil->setEditable (false, false, false);
    labelMidiNotesUntil->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelMidiNotesUntil->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelMidiNotesUntil->setBounds (8, 8, 432, 24);

    singleChannelButton.reset (new juce::ToggleButton ("singleChannelButton"));
    addAndMakeVisible (singleChannelButton.get());
    singleChannelButton->setTooltip (TRANS("Only the MIDI notes from a single channel"));
    singleChannelButton->setButtonText (TRANS("Single channel"));
    singleChannelButton->setConnectedEdges (juce::Button::ConnectedOnTop);
    singleChannelButton->setRadioGroupId (1);
    singleChannelButton->addListener (this);

    singleChannelButton->setBounds (8, 40, 160, 24);

    channelBox.reset (new juce::ComboBox ("channelBox"));
    addAndMakeVisible (channelBox.get());
    channelBox->setTooltip (TRANS("The MIDI channel (in case of single channel)"));
    channelBox->setEditableText (false);
    channelBox->setJustificationType (juce::Justification::centredLeft);
    channelBox->setTextWhenNothingSelected (juce::String());
    channelBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    channelBox->addListener (this);

    channelBox->setBounds (184, 40, 56, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (416, 220);


    //[Constructor] You can add your own custom stuff here..
	for (int i = 1; i <= 16; i++)
		channelBox->addItem(String(i), i);

	// Default: multi channel
	// Todo latest choice by user, from settings
	channelAutoIncrButton->setToggleState(true, juce::NotificationType::sendNotification);

	//noteLabel->attachToComponent(noteBox, true);
	//channelLabel->attachToComponent(channelBox, true);
    //[/Constructor]
}

IncrMidiNotesMapping::~IncrMidiNotesMapping()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    channelAutoIncrButton = nullptr;
    labelMidiNotesUntil = nullptr;
    singleChannelButton = nullptr;
    channelBox = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void IncrMidiNotesMapping::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xffb8d0de));

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(findColour(ResizableWindow::backgroundColourId));
    //[/UserPaint]
}

void IncrMidiNotesMapping::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void IncrMidiNotesMapping::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == channelAutoIncrButton.get())
    {
        //[UserButtonCode_channelAutoIncrButton] -- add your button handler code here..
		if (channelAutoIncrButton->getToggleState())
		{
			channelBox->setVisible(false);
			this->mappingLogic.setChannelInCaseOfSingleChannel(0);	// 0 for no selection or 1-16
		}
        //[/UserButtonCode_channelAutoIncrButton]
    }
    else if (buttonThatWasClicked == singleChannelButton.get())
    {
        //[UserButtonCode_singleChannelButton] -- add your button handler code here..
		if (singleChannelButton->getToggleState())
		{
			channelBox->setVisible(true);
			// Make sure something is selected
			if (channelBox->getSelectedId() <= 0)
				channelBox->setSelectedId(1);   // This will generate an update of the mapping logic via comboBoxChanged
		}
        //[/UserButtonCode_singleChannelButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void IncrMidiNotesMapping::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == channelBox.get())
    {
        //[UserComboBoxCode_channelBox] -- add your combo box handling code here..
		this->mappingLogic.setChannelInCaseOfSingleChannel(channelBox->getSelectedId());	// 0 for no selection or 1-16
        //[/UserComboBoxCode_channelBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void IncrMidiNotesMapping::visibilityChanged()
{
    //[UserCode_visibilityChanged] -- Add your code here...
    if ( isVisible())
    {
        // Re-fill note combo box according to mapping logic
        this->mappingLogic.setValues(
            periodSize,
            channelAutoIncrButton->getToggleState() ? channelBox->getSelectedId() : 0,
            true);
    }
    //[/UserCode_visibilityChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void IncrMidiNotesMapping::onUpdatePeriodSize()
{
    if ( isVisible())
    {
        this->mappingLogic.setPeriodSize(periodSize);
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="IncrMidiNotesMapping" componentName=""
                 parentClasses="public Component" constructorParams="int&amp; periodSizeReference, ScaleStructure&amp; scaleStructureIn, Array&lt;Colour&gt;&amp; colourTableIn"
                 variableInitialisers="periodSize(periodSizeReference),  mappingLogic(scaleStructureIn, colourTableIn)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="416" initialHeight="220">
  <METHODS>
    <METHOD name="visibilityChanged()"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffb8d0de"/>
  <TOGGLEBUTTON name="channelAutoIncrButton" id="1749290d10236ec3" memberName="channelAutoIncrButton"
                virtualName="" explicitFocusOrder="0" pos="8 64 280 24" tooltip="Auto-increment channel after maximal note is reached"
                buttonText="Multichannel" connectedEdges="4" needsCallback="1"
                radioGroupId="1" state="0"/>
  <LABEL name="labelMidiNotesUntil" id="c566693d4c0fdb01" memberName="labelMidiNotesUntil"
         virtualName="" explicitFocusOrder="0" pos="8 8 432 24" edTextCol="ff000000"
         edBkgCol="0" labelText="MIDI notes from 0 to the scale size"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <TOGGLEBUTTON name="singleChannelButton" id="c96447a8b1eacf35" memberName="singleChannelButton"
                virtualName="" explicitFocusOrder="0" pos="8 40 160 24" tooltip="Only the MIDI notes from a single channel"
                buttonText="Single channel" connectedEdges="4" needsCallback="1"
                radioGroupId="1" state="0"/>
  <COMBOBOX name="channelBox" id="3ac4d54146311c84" memberName="channelBox"
            virtualName="" explicitFocusOrder="0" pos="184 40 56 24" tooltip="The MIDI channel (in case of single channel)"
            editable="0" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

