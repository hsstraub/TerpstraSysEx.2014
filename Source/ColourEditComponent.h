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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"

/*
==============================================================================
Combo box with colours
==============================================================================
*/

#define ADDCOLOURTOCOMBOBOX true
#define DONTADDCOLOURTOCOMBOBOX false

class ColourComboBox : public ComboBox
{
public:
	enum colourComboboxOptions
	{
		DoNotAddColourToCombobox = 0,
		AddColourToComboBox
	};

	explicit ColourComboBox(const String& componentName = String());

	void setTextFieldToColourAsObject(Colour newColourAsObject,
		NotificationType notification = sendNotificationAsync);
	String getColourAsStringFromText(colourComboboxOptions boxOptions);
	int getColourAsNumberFromText(colourComboboxOptions boxOptions);
	Colour getColourAsObjectFromText(colourComboboxOptions boxOptions);

	void addColourToBox(String newColourAsString);
	void addColourToBox(Colour newColourAsObject) { addColourToBox(newColourAsObject.toDisplayString(false)); }
	void addColourToBox(int newColourAsNumber) { addColourToBox(Colour(newColourAsNumber)); }
};

/*
==============================================================================
Draw each item in ColourComboBox's drop-down list in "its" colour
==============================================================================
*/
class ColourComboLookAndFeel : public LookAndFeel_V4
{
public:
    void drawPopupMenuItem (Graphics&, const Rectangle<int>& area,
                            bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
                            const String& text, const String& shortcutKeyText,
                            const Drawable* icon, const Colour* textColour) override;

};


//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ColourEditComponent  : public Component,
                             public ChangeListener,
                             public ChangeBroadcaster,
                             public juce::Button::Listener,
                             public juce::ComboBox::Listener
{
public:
    //==============================================================================
    ColourEditComponent ();
    ~ColourEditComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void changeListenerCallback(ChangeBroadcaster *source) override;

	void setColour(String colourAsString);
	String getColourAsString();
	int getColourAsNumber();
	void addColourToBox(int newColourAsNumber);
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    std::unique_ptr<ColourComboLookAndFeel> colourComboLookAndFeel;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextButton> btnColourPicker;
    std::unique_ptr<ColourComboBox> colourCombo;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourEditComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

