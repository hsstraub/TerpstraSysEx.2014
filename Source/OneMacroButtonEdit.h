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
class OneMacroButtonEdit  : public Component,
                            public TextEditor::Listener,
                            public Button::Listener
{
public:
    //==============================================================================
    OneMacroButtonEdit ();
    ~OneMacroButtonEdit();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void setMacroButtonNumber(int value);
	void setSaveAfterSending(bool value) { saveAfterSending = value; }
	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);
	void textEditorFocusLost(TextEditor& textEdit) override;
	void sendParametrizationFileToDevice();

protected:
	void updateTooltipFromFileObject();
	void updateTextEditorFromFileObject();
public:
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	int		macroButtonNumber;
	File	currentFile;
	bool    saveAfterSending;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Label> lblButton;
    std::unique_ptr<TextEditor> textMacroFile;
    std::unique_ptr<TextButton> btnFileSelectMacro;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneMacroButtonEdit)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

