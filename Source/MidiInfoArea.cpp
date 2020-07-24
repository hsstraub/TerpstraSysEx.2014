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
#include "Main.h"
//[/Headers]

#include "MidiInfoArea.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MidiInfoArea::MidiInfoArea ()
    : errorVisualizer(TerpstraSysExApplication::getApp().getLookAndFeel())
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    labelInfoTitle.reset (new Label ("labelInfoTitle",
                                     TRANS("Latest MIDI message:")));
    addAndMakeVisible (labelInfoTitle.get());
    labelInfoTitle->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelInfoTitle->setJustificationType (Justification::centredLeft);
    labelInfoTitle->setEditable (false, false, false);
    labelInfoTitle->setColour (TextEditor::textColourId, Colours::black);
    labelInfoTitle->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    labelInfoTitle->setBounds (8, 0, 160, 24);

    textInfo.reset (new Label ("textInfo",
                               String()));
    addAndMakeVisible (textInfo.get());
    textInfo->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    textInfo->setJustificationType (Justification::centredLeft);
    textInfo->setEditable (false, false, false);
    textInfo->setColour (TextEditor::textColourId, Colours::black);
    textInfo->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    textInfo->setBounds (8, 24, 432, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 64);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MidiInfoArea::~MidiInfoArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    labelInfoTitle = nullptr;
    textInfo = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MidiInfoArea::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
    g.fillAll(findColour(ResizableWindow::backgroundColourId));
    //[/UserPaint]
}

void MidiInfoArea::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MidiInfoArea::writeLog(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel)
{
    textInfo->setText(textMessage, NotificationType::dontSendNotification);
    errorVisualizer.setErrorLevel(*textInfo.get(), errorLevel, textMessage);
}

void MidiInfoArea::writeLog(const MidiMessage& midiMessage, TerpstraMidiDriver::MIDISendDirection sendDirection)
{
    textInfo->setText(
        sendDirection == TerpstraMidiDriver::MIDISendDirection::received ? "<< " : ">> " + midiMessage.getDescription(),
        NotificationType::dontSendNotification);

    if (sendDirection == TerpstraMidiDriver::MIDISendDirection::received &&
        TerpstraSysExApplication::getApp().getMidiDriver().messageIsTerpstraConfigurationDataReceptionMessage(midiMessage))
    {
        if (midiMessage.getSysExDataSize() < 6)
        {
            errorVisualizer.setErrorLevel(
                *textInfo.get(),
                HajuErrorVisualizer::ErrorLevel::error,
                "Message too short");
        }
        else
        {
            auto sysExData = midiMessage.getSysExData();
            auto answerState = sysExData[5];

            switch(answerState)
            {
            case TerpstraMidiDriver::TerpstraMIDIAnswerReturnCode::NACK:  // Not recognized
                errorVisualizer.setErrorLevel(
                    *textInfo.get(),
                    HajuErrorVisualizer::ErrorLevel::error,
                    "Not Recognized");
                break;

            case TerpstraMidiDriver::TerpstraMIDIAnswerReturnCode::ACK:  // Acknowledged, OK
                errorVisualizer.setErrorLevel(
                    *textInfo.get(),
                    HajuErrorVisualizer::ErrorLevel::noError,
                    "Ack");
                break;

            case TerpstraMidiDriver::TerpstraMIDIAnswerReturnCode::BUSY: // Controller busy
                errorVisualizer.setErrorLevel(
                    *textInfo.get(),
                    HajuErrorVisualizer::ErrorLevel::warning,
                    "Busy");
                break;

            case TerpstraMidiDriver::TerpstraMIDIAnswerReturnCode::ERROR:    // Error
                errorVisualizer.setErrorLevel(
                    *textInfo.get(),
                    HajuErrorVisualizer::ErrorLevel::error,
                    "Error from device");
                break;

            default:
                errorVisualizer.setErrorLevel(
                    *textInfo.get(),
                    HajuErrorVisualizer::ErrorLevel::noError,
                    "");
                break;
            }
        }
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiInfoArea" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers="errorVisualizer(TerpstraSysExApplication::getApp().getLookAndFeel()), "
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="64">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <LABEL name="labelInfoTitle" id="b9d64b95c48abbe3" memberName="labelInfoTitle"
         virtualName="" explicitFocusOrder="0" pos="8 0 160 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Latest MIDI message:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="textInfo" id="c1ed1065b4532578" memberName="textInfo" virtualName=""
         explicitFocusOrder="0" pos="8 24 432 24" edTextCol="ff000000"
         edBkgCol="0" labelText="" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

