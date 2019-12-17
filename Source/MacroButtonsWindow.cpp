/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "ViewConstants.h"
//[/Headers]

#include "MacroButtonsWindow.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MacroButtonsWindow::MacroButtonsWindow ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    btnEnableMacroButtons.reset (new ToggleButton ("btnEnableMacroButtons"));
    addAndMakeVisible (btnEnableMacroButtons.get());
    btnEnableMacroButtons->setButtonText (TRANS("Enable Macro Buttons"));
    btnEnableMacroButtons->addListener (this);

    btnEnableMacroButtons->setBounds (136, 16, 150, 24);

    lblMacroButtonsInfo.reset (new Label ("lblMacroButtonsInfo",
                                          TRANS("When the corresponding macro button is pressed, the selected key mapping file will be sent to the keyboard.")));
    addAndMakeVisible (lblMacroButtonsInfo.get());
    lblMacroButtonsInfo->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    lblMacroButtonsInfo->setJustificationType (Justification::centredLeft);
    lblMacroButtonsInfo->setEditable (false, false, false);
    lblMacroButtonsInfo->setColour (TextEditor::textColourId, Colours::black);
    lblMacroButtonsInfo->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lblMacroButtonsInfo->setBounds (8, 64, 440, 48);

    btnSaveAfterSending.reset (new ToggleButton ("btnSaveAfterSending"));
    addAndMakeVisible (btnSaveAfterSending.get());
    btnSaveAfterSending->setButtonText (TRANS("Save mapping after sending"));
    btnSaveAfterSending->addListener (this);

    btnSaveAfterSending->setBounds (136, 40, 216, 24);

    drawable1 = Drawable::createFromImageData (BinaryData::TopEdgeButton_png, BinaryData::TopEdgeButton_pngSize);

    //[UserPreSize]

	for (int i = 0; i < 10; i++)
	{
		buttonComponents[i] = new OneMacroButtonEdit();
		addAndMakeVisible(buttonComponents[i]);
		buttonComponents[i]->setMacroButtonNumber(i);
	}

    //[/UserPreSize]

    setSize (428, 400);


    //[Constructor] You can add your own custom stuff here..
	btnEnableMacroButtons->setToggleState(true, juce::NotificationType::sendNotification);
    //[/Constructor]
}

MacroButtonsWindow::~MacroButtonsWindow()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    btnEnableMacroButtons = nullptr;
    lblMacroButtonsInfo = nullptr;
    btnSaveAfterSending = nullptr;
    drawable1 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	for (int i = 0; i < 10; i++)
		buttonComponents[i] = nullptr;
    //[/Destructor]
}

//==============================================================================
void MacroButtonsWindow::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffbad0de));

    {
        int x = 12, y = 12, width = 100, height = 46;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable1 != 0);
        if (drawable1 != 0)
            drawable1->drawWithin (g, Rectangle<int> (x, y, width, height).toFloat(),
                                   RectanglePlacement::stretchToFit, 1.000f);
    }

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(findColour(ResizableWindow::backgroundColourId));

	g.setColour(Colours::black);
	jassert(drawable1 != 0);
	if (drawable1 != 0)
		drawable1->drawWithin(g, Rectangle<float>(12, 12, 100, 46),
		RectanglePlacement::stretchToFit, 1.000f);
    //[/UserPaint]
}

void MacroButtonsWindow::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
	for (int i = 0; i < 5; i ++)
	{
		buttonComponents[i]->setBounds(0, 112 + 40 * i, EDITSUBWINWIDTH / 2, 40);
		buttonComponents[i + 5]->setBounds(EDITSUBWINWIDTH / 2, 112 + 40 * i, EDITSUBWINWIDTH / 2, 40);
	}
    //[/UserResized]
}

void MacroButtonsWindow::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnEnableMacroButtons.get())
    {
        //[UserButtonCode_btnEnableMacroButtons] -- add your button handler code here..
		bool enable = btnEnableMacroButtons->getToggleState();
		for (int i = 0; i < 10; i++)
		{
			buttonComponents[i]->setVisible(enable);
		}

        //[/UserButtonCode_btnEnableMacroButtons]
    }
    else if (buttonThatWasClicked == btnSaveAfterSending.get())
    {
        //[UserButtonCode_btnSaveAfterSending] -- add your button handler code here..
		bool saveAfterSending = btnSaveAfterSending->getToggleState();
		for (int i = 0; i < 10; i++)
		{
			buttonComponents[i]->setSaveAfterSending(saveAfterSending);
		}
        //[/UserButtonCode_btnSaveAfterSending]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MacroButtonsWindow::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message)
{
	if (message.isController() && btnEnableMacroButtons->getToggleState())
	{
		// Established that a controller change has occurred, which is due
		// to a keyboard macro button being pressed. Now Channel will hold
		// a value from 0 to 15, and InData1 will hold either 16 or 17.
		// the formula to establish which button is pressed is
		// ButtonNum = channel*2 + (InData1 - 16)
		int buttonNo = (message.getChannel() - 1) * 2 + message.getControllerNumber() - 16;

		if (buttonNo >= 0 && buttonNo < 10 && message.getControllerValue() == 0x3f)
		{
			// Send the SysEx parameter file of the corresponding controller
			buttonComponents[buttonNo]->sendParametrizationFileToDevice();
		}
	}
}


void MacroButtonsWindow::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	for (int i = 0; i < 10; i++)
		buttonComponents[i]->restoreStateFromPropertiesFile(propertiesFile);
}

void MacroButtonsWindow::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	for (int i = 0; i < 10; i++)
		buttonComponents[i]->saveStateToPropertiesFile(propertiesFile);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MacroButtonsWindow" componentName=""
                 parentClasses="public Component, public MidiInputCallback" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="428" initialHeight="400">
  <BACKGROUND backgroundColour="ffbad0de">
    <IMAGE pos="12 12 100 46" resource="BinaryData::TopEdgeButton_png" opacity="1.0"
           mode="0"/>
  </BACKGROUND>
  <TOGGLEBUTTON name="btnEnableMacroButtons" id="9e31c84073a54686" memberName="btnEnableMacroButtons"
                virtualName="" explicitFocusOrder="0" pos="136 16 150 24" buttonText="Enable Macro Buttons"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <LABEL name="lblMacroButtonsInfo" id="199c7576a5017f09" memberName="lblMacroButtonsInfo"
         virtualName="" explicitFocusOrder="0" pos="8 64 440 48" edTextCol="ff000000"
         edBkgCol="0" labelText="When the corresponding macro button is pressed, the selected key mapping file will be sent to the keyboard."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <TOGGLEBUTTON name="btnSaveAfterSending" id="a23d41b7fb227aed" memberName="btnSaveAfterSending"
                virtualName="" explicitFocusOrder="0" pos="136 40 216 24" buttonText="Save mapping after sending"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

