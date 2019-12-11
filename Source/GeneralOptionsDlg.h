/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_D96E78A3018B928E__
#define __JUCE_HEADER_D96E78A3018B928E__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "ColourEditComponent.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class GeneralOptionsDlg  : public Component,
                           public TextEditor::Listener,
                           public ChangeListener,
                           public Button::Listener
{
public:
    //==============================================================================
    GeneralOptionsDlg ();
    ~GeneralOptionsDlg();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void textEditorFocusLost(TextEditor& textEdit) override;
	void changeListenerCallback(ChangeBroadcaster *source) override;

	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	ScopedPointer<ColourEditComponent> inactiveMacroButtonColourEdit;
	ScopedPointer<ColourEditComponent> activeMacroButtonColourEdit;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> labelExprContrSensivity;
    ScopedPointer<TextEditor> txtExprCtrlSensivity;
    ScopedPointer<ToggleButton> btnInvertFootCtrl;
    ScopedPointer<Label> lblInvFootCtrl;
    ScopedPointer<Label> lblLightOnKeyStroke;
    ScopedPointer<ToggleButton> btnLightOnKeyStroke;
    ScopedPointer<Label> lblColourInactiveMacroButton;
    ScopedPointer<Label> lblColourActiveMacroButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GeneralOptionsDlg)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_D96E78A3018B928E__
