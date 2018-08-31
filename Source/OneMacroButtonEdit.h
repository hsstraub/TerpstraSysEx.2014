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

#ifndef __JUCE_HEADER_6E043DC845DB684C__
#define __JUCE_HEADER_6E043DC845DB684C__

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
                            public TextEditorListener,
                            public ButtonListener
{
public:
    //==============================================================================
    OneMacroButtonEdit ();
    ~OneMacroButtonEdit();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void setMacroButtonNumber(int value);
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
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> lblButton;
    ScopedPointer<TextEditor> textMacroFile;
    ScopedPointer<TextButton> btnFileSelectMacro;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneMacroButtonEdit)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_6E043DC845DB684C__
