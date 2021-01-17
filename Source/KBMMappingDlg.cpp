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
//[/Headers]

#include "KBMMappingDlg.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
KBMMappingDlg::KBMMappingDlg (int& periodSizeReference, ScaleStructure& scaleStructureIn, Array<Colour>& colourTableIn)
    : periodSize(periodSizeReference),  mappingLogic(scaleStructureIn, colourTableIn)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    lblChannel.reset (new juce::Label ("lblChannel", translate("Chnl")));
    addAndMakeVisible (lblChannel.get());
    lblChannel->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblChannel->setJustificationType (juce::Justification::centredLeft);
    lblChannel->setEditable (false, false, false);
    lblChannel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblChannel->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblChannel->setBounds (0, 8, 40, 24);

    lblMappingFile.reset (new juce::Label ("lblMappingFile", translate("MappingFile")));
    addAndMakeVisible (lblMappingFile.get());
    lblMappingFile->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblMappingFile->setJustificationType (juce::Justification::centredLeft);
    lblMappingFile->setEditable (false, false, false);
    lblMappingFile->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblMappingFile->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblMappingFile->setBounds (48, 8, 104, 24);

    lblChannel2.reset (new juce::Label ("lblChannel2", translate("Chnl")));
    addAndMakeVisible (lblChannel2.get());
    lblChannel2->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblChannel2->setJustificationType (juce::Justification::centredLeft);
    lblChannel2->setEditable (false, false, false);
    lblChannel2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblChannel2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblChannel2->setBounds (152, 7, 40, 24);

    lblMappingFile2.reset (new juce::Label ("lblMappingFile2", translate("MappingFile")));
    addAndMakeVisible (lblMappingFile2.get());
    lblMappingFile2->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblMappingFile2->setJustificationType (juce::Justification::centredLeft);
    lblMappingFile2->setEditable (false, false, false);
    lblMappingFile2->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblMappingFile2->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblMappingFile2->setBounds (200, 8, 104, 24);


    //[UserPreSize]
	for (int i = 0; i < KBMFilesMappingLogic::noOfChannels; i++)
	{
		channelMappingComponents[i].reset(new KBMForOneChannel(i, mappingLogic, periodSize));
		addAndMakeVisible(channelMappingComponents[i].get());
	}

    flexBox.flexDirection = FlexBox::Direction::row;
    flexBox.flexWrap = FlexBox::Wrap::wrap;
    //[/UserPreSize]

    setSize (416, 220);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

KBMMappingDlg::~KBMMappingDlg()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    lblChannel = nullptr;
    lblMappingFile = nullptr;
    lblChannel2 = nullptr;
    lblMappingFile2 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	for (int i = 0; i < KBMFilesMappingLogic::noOfChannels; i++)
		channelMappingComponents[i] = nullptr;
    //[/Destructor]
}

//==============================================================================
void KBMMappingDlg::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //g.fillAll (juce::Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
	//g.fillAll(findColour(ResizableWindow::backgroundColourId));
    //[/UserPaint]
}

void KBMMappingDlg::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    int width = getWidth();
    int height = getHeight();


    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
 //   auto col1 = lblChannel->getX();
 //   auto col2 = lblChannel2->getX();

 //   int noOfRows = KBMFilesMappingLogic::noOfChannels/2;
 //	for (int i = 0; i < noOfRows; i ++)
	//{
	//	channelMappingComponents[i]->setBounds(col1, 32 + 32 * i, channelMappingComponents[i]->getWidth(), 32);
	//	channelMappingComponents[noOfRows + i]->setBounds(col2, 32 + 32 * i, channelMappingComponents[noOfRows + i]->getWidth(), 32);
	//}

    int labelHeight = round(height * 0.2f);
    int lblChannelWidth = round(width * 0.2f);
    int lblFileWidth = round(width * 0.3f);
    
    lblChannel->setBounds(0, 0, lblChannelWidth, labelHeight);
    lblMappingFile->setBounds(lblChannel->getRight(), 0, lblFileWidth, labelHeight);
    lblChannel2->setBounds(lblMappingFile->getRight(), 0, lblChannelWidth, labelHeight);
    lblMappingFile2->setBounds(lblChannel2->getRight(), 0, lblFileWidth, labelHeight);

    int rowHeight = height / (KBMFilesMappingLogic::noOfChannels * 0.5f);
    flexBox.items.clear();
    for (int i = 0; i < KBMFilesMappingLogic::noOfChannels; i++)
    {
        auto component = channelMappingComponents[i].get();
        flexBox.items.add(FlexItem(*component).withFlex(1.0f).withWidth(width * 0.4f));// .withHeight(rowHeight));
    }
    flexBox.performLayout(getLocalBounds().withTop(labelHeight));
    //[/UserResized]
}

void KBMMappingDlg::visibilityChanged()
{
    //[UserCode_visibilityChanged] -- Add your code here...
    if (isVisible())
    {
         // Re-fill note combo box according to mapping logic
       	for (int i = 0; i < KBMFilesMappingLogic::noOfChannels; i ++)
            channelMappingComponents[i]->updateFieldsAndMappingLogic();
    }
    //[/UserCode_visibilityChanged]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void KBMMappingDlg::onUpdatePeriodSize()
{
    if (isVisible())
    {
         // Re-fill note combo box according to mapping logic
       	for (int i = 0; i < KBMFilesMappingLogic::noOfChannels; i ++)
            channelMappingComponents[i]->updateFieldsAndMappingLogic();
    }
}

void KBMMappingDlg::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	for (int i = 0; i < KBMFilesMappingLogic::noOfChannels; i++)
		channelMappingComponents[i]->restoreStateFromPropertiesFile(propertiesFile);
}

void KBMMappingDlg::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	for (int i = 0; i < KBMFilesMappingLogic::noOfChannels; i++)
		channelMappingComponents[i]->saveStateToPropertiesFile(propertiesFile);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="KBMMappingDlg" componentName=""
                 parentClasses="public Component" constructorParams="int&amp; periodSizeReference, ScaleStructure&amp; scaleStructureIn, Array&lt;Colour&gt;&amp; colourTableIn"
                 variableInitialisers="periodSize(periodSizeReference),  mappingLogic(scaleStructureIn, colourTableIn)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="416" initialHeight="220">
  <METHODS>
    <METHOD name="visibilityChanged()"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffbad0de"/>
  <LABEL name="lblChannel" id="dda6cf104269f7ea" memberName="lblChannel"
         virtualName="" explicitFocusOrder="0" pos="0 8 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Chnl" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblMappingFile" id="3fd88d602009452e" memberName="lblMappingFile"
         virtualName="" explicitFocusOrder="0" pos="48 8 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Mapping file" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblChannel2" id="d3e62a70b82307b5" memberName="lblChannel2"
         virtualName="" explicitFocusOrder="0" pos="152 7 40 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Chnl" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblMappingFile2" id="e16bc6743135450d" memberName="lblMappingFile2"
         virtualName="" explicitFocusOrder="0" pos="200 8 104 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Mapping file" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

