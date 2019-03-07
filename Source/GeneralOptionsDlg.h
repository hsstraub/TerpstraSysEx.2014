/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
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
                           public Button::Listener
{
public:
    //==============================================================================
    GeneralOptionsDlg ();
    ~GeneralOptionsDlg();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void textEditorFocusLost(TextEditor& textEdit) override;

	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Label> labelExprContrSensivity;
    std::unique_ptr<TextEditor> txtExprCtrlSensivity;
    std::unique_ptr<ToggleButton> btnInvertFootCtrl;
    std::unique_ptr<Label> lblInvFootCtrl;
    std::unique_ptr<Label> lblLightOnKeyStroke;
    std::unique_ptr<ToggleButton> btnLightOnKeyStroke;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GeneralOptionsDlg)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

