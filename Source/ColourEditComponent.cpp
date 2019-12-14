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
//[/Headers]

#include "ColourEditComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

/*
==============================================================================
ColourComboBox class
==============================================================================
*/

ColourComboBox::ColourComboBox(const String& componentName) : ComboBox(componentName)
{
}

void ColourComboBox::setTextFieldToColourAsObject(Colour newColourAsObject, NotificationType notification)
{
	setText(newColourAsObject.toDisplayString(false));

	// XXX Add to box
}

String ColourComboBox::getColourAsStringFromText(colourComboboxOptions boxOptions)
{
	String colourAsString = getText();

	// XXX validation of colour value

	if (boxOptions == colourComboboxOptions::AddColourToComboBox)
		addColourToBox(colourAsString);

	return colourAsString;
}

int ColourComboBox::getColourAsNumberFromText(colourComboboxOptions boxOptions)
{
	String colourString = getColourAsStringFromText(boxOptions);

	// XXX validation of colour value

	int colourAsNumber = colourString.getHexValue32();

	return colourAsNumber;
}

// Add colour to combo box
void ColourComboBox::addColourToBox(String newColourAsString)
{
	int pos;
	for (pos = 0; pos < getNumItems(); pos++)
	{
		if (getItemText(pos) == newColourAsString)
			break;
	}

	if (pos >= getNumItems())
	{
		// Colour is not in list yet - add it
		addItem(newColourAsString, pos + 1);
	}
}

Colour ColourComboBox::getColourAsObjectFromText(colourComboboxOptions boxOptions)
{
	int colourAsNumber = getColourAsNumberFromText(boxOptions);
	return Colour(colourAsNumber);
}

//[/MiscUserDefs]

//==============================================================================
ColourEditComponent::ColourEditComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    btnColourPicker.reset (new TextButton ("btnColourPicker"));
    addAndMakeVisible (btnColourPicker.get());
    btnColourPicker->setButtonText (TRANS("Colour picker"));
    btnColourPicker->addListener (this);

    btnColourPicker->setBounds (88, 0, 104, 24);

    colourCombo.reset (new ColourComboBox ("colourCombo"));
    addAndMakeVisible (colourCombo.get());
    colourCombo->setEditableText (true);
    colourCombo->setJustificationType (Justification::centredLeft);
    colourCombo->setTextWhenNothingSelected (String());
    colourCombo->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    colourCombo->addListener (this);

    colourCombo->setBounds (0, 0, 79, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (196, 32);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ColourEditComponent::~ColourEditComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    btnColourPicker = nullptr;
    colourCombo = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ColourEditComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffb8d0de));

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(findColour(ResizableWindow::backgroundColourId));
    //[/UserPaint]
}

void ColourEditComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ColourEditComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnColourPicker.get())
    {
        //[UserButtonCode_btnColourPicker] -- add your button handler code here..
		ColourSelector* colourSelector = new ColourSelector(ColourSelector::showSliders | ColourSelector::showColourspace);
		colourSelector->setName("Colour picker");
		colourSelector->addChangeListener(this);

		Colour currentColor = colourCombo->getColourAsObjectFromText(ColourComboBox::DoNotAddColourToCombobox);

		colourSelector->setCurrentColour(currentColor);

		colourSelector->setColour(ColourSelector::backgroundColourId, currentColor);
		colourSelector->setSize(300, 400);

		CallOutBox::launchAsynchronously(colourSelector, buttonThatWasClicked->getScreenBounds(), nullptr);
        //[/UserButtonCode_btnColourPicker]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void ColourEditComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == colourCombo.get())
    {
        //[UserComboBoxCode_colourCombo] -- add your combo box handling code here..

		// Notify parent that value has changed and can be sent to MIDI controller
		sendChangeMessage();

        //[/UserComboBoxCode_colourCombo]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void ColourEditComponent::changeListenerCallback(ChangeBroadcaster *source)
{
	ColourSelector* cs = dynamic_cast <ColourSelector*> (source);
	Colour currentColor = cs->getCurrentColour();
	colourCombo->setTextFieldToColourAsObject(currentColor);

}

void ColourEditComponent::setColour(String colourAsString)
{
	jassert(colourCombo != nullptr);

	// XXX validation of colour value

	colourCombo->setText(colourAsString);

	// XXX Add to box
}

String ColourEditComponent::getColourAsString()
{
	jassert(colourCombo != nullptr);

	return colourCombo->getColourAsStringFromText(ColourComboBox::AddColourToComboBox);
}

int ColourEditComponent::getColourAsNumber()
{
	jassert(colourCombo != nullptr);

	return colourCombo->getColourAsNumberFromText(ColourComboBox::AddColourToComboBox);
}

void ColourEditComponent::addColourToBox(int newColourAsNumber)
{
	jassert(colourCombo != nullptr);
	colourCombo->addColourToBox(newColourAsNumber);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ColourEditComponent" componentName=""
                 parentClasses="public Component, public ChangeListener, public ChangeBroadcaster"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="1" initialWidth="196"
                 initialHeight="32">
  <BACKGROUND backgroundColour="ffb8d0de"/>
  <TEXTBUTTON name="btnColourPicker" id="fb8f62a75c5cd9ec" memberName="btnColourPicker"
              virtualName="" explicitFocusOrder="0" pos="88 0 104 24" buttonText="Colour picker"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="colourCombo" id="86628debb1bafc04" memberName="colourCombo"
            virtualName="ColourComboBox" explicitFocusOrder="0" pos="0 0 79 24"
            editable="1" layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

