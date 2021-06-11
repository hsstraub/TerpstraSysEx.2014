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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "KeyboardDataStructure.h"
#include "ColourEditComponent.h"
#include "LumatoneEditorLookAndFeel.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SingleNoteAssign  : public Component,
                          public ColourSelectionListener,
                          public TextEditor::Listener,
                          public juce::Button::Listener,
                          public juce::ComboBox::Listener,
                          public juce::Slider::Listener
{
public:
    //==============================================================================
    SingleNoteAssign ();
    ~SingleNoteAssign() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	UndoableAction* createEditAction(int setSelection, int keySelection);
	void onSetData(TerpstraKeyMapping& newData);

	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);

    void lookAndFeelChanged() override;

    ColourEditComponent* getColourEditComponent() { return colourSubwindow.get(); }
    ColourTextEditor* getColourTextEditor() { return colourTextEditor.get(); }

    void colourChangedCallback(ColourSelectionBroadcaster* source, Colour newColour) override;
    
    void redrawCCFlipBtn();
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //==============================================================================
    // Style Helpers
    int roundedCornerSize;
    Rectangle<int> instructionsAreaBounds;
    Rectangle<int> instructionsBounds;
    int controlsX;
    int separatorY;

    Font instructionsFont;
    Font parametersFont;

    //==============================================================================
    // Size and position constants
    const float fontHeightInBounds  = 0.21f;

    const float xMarginScalar       = 0.0917f;
    const float yMarginScalar       = 0.0813f;
    const float controlAreaYScalar  = 0.183333f;
    const float controlsXScalar     = 0.06;
    const float separatorYScalar    = 0.666667f;
    const float toggleHeightScalar  = 0.034f;
    const float controlHeightScalar = 0.0647f;

    const float controlBoxFontHeightScalar     = 0.75f;
    const float incDecButtonTextBoxWidthScalar = 0.4f;

    const Colour toggleTextColour = Colour(0xffcbcbcb);
    
    std::unique_ptr<juce::TextButton> ccFaderFlipBtn;
    Path faderDownArrow;
    Path faderUpArrow;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::ToggleButton> noteAutoIncrButton;
    std::unique_ptr<juce::ToggleButton> channelAutoIncrButton;
    std::unique_ptr<juce::ToggleButton> setNoteToggleButton;
    std::unique_ptr<juce::ToggleButton> setChannelToggleButton;
    std::unique_ptr<juce::ToggleButton> setColourToggleButton;
    std::unique_ptr<juce::ToggleButton> keyTypeToggleButton;
    std::unique_ptr<juce::ComboBox> keyTypeCombo;
    std::unique_ptr<juce::Slider> noteInput;
    std::unique_ptr<ColourEditComponent> colourSubwindow;
    std::unique_ptr<juce::Label> autoIncrementLabel;
    std::unique_ptr<ColourTextEditor> colourTextEditor;
    std::unique_ptr<juce::Slider> channelInput;
    std::unique_ptr<juce::Slider> channelAutoIncrNoteInput;
    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SingleNoteAssign)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

