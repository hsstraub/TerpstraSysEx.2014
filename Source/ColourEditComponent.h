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
                             public Button::Listener,
                             public ComboBox::Listener
{
public:
    //==============================================================================
    ColourEditComponent ();
    ~ColourEditComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void changeListenerCallback(ChangeBroadcaster *source) override;

	void setColour(String colourAsString);
	String getColourAsString();
	int getColourAsNumber();
	void addColourToBox(int newColourAsNumber);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextButton> btnColourPicker;
    std::unique_ptr<ColourComboBox> colourCombo;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourEditComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

