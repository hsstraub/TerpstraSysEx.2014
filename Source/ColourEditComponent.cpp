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

//[Headers] You can add your own extra header files here...
#include "ViewConstants.h"
#include "ViewComponents.h"
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

	// getLookAndFeel().setColour(juce::ComboBox::arrowColourId, newColourAsObject.contrasting(1.0)); Doesn't work XXX

	// ToDo Add to box
}

String ColourComboBox::getColourAsStringFromText(colourComboboxOptions boxOptions)
{
	String colourAsString = getText();

	// ToDo validation of colour value

	if (boxOptions == colourComboboxOptions::AddColourToComboBox && !colourAsString.isEmpty())
		addColourToBox(colourAsString);

	return colourAsString;
}

int ColourComboBox::getColourAsNumberFromText(colourComboboxOptions boxOptions)
{
	String colourString = getColourAsStringFromText(boxOptions);

	// ToDo validation of colour value

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


/*
==============================================================================
ColourComboLookAndFeel class
==============================================================================
*/

void ColourComboLookAndFeel::drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                                        const bool isSeparator, const bool isActive,
                                        const bool isHighlighted, const bool isTicked,
                                        const bool hasSubMenu, const String& text,
                                        const String& shortcutKeyText,
                                        const Drawable* icon, const Colour* const textColourToUse)
{
    if (isSeparator)
    {
        LookAndFeel_V4::drawPopupMenuItem(
            g, area, isSeparator, isActive, isHighlighted, isTicked, hasSubMenu, text, shortcutKeyText, icon, textColourToUse);
    }
    else
    {
         auto bgColour = findColour(TerpstraKeyEdit::backgroundColourId).overlaidWith(
            Colour(text.getHexValue32())
                .withAlpha(isHighlighted && isActive ? TERPSTRASINGLEKEYCOLOURALPHA : TERPSTRASINGLEKEYCOLOURUNSELECTEDMINIALPHA));
        auto textColour = bgColour.contrasting(1.0);

        auto r  = area.reduced (1);

        g.setColour (bgColour);
        g.fillRect (r);

        g.setColour (textColour);

        r.reduce (jmin (5, area.getWidth() / 20), 0);

        auto font = getPopupMenuFont();

        auto maxFontHeight = r.getHeight() / 1.3f;

        if (font.getHeight() > maxFontHeight)
            font.setHeight (maxFontHeight);

        g.setFont (font);

        auto iconArea = r.removeFromLeft (roundToInt (maxFontHeight)).toFloat();

        if (icon != nullptr)
        {
            icon->drawWithin (g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
            r.removeFromLeft (roundToInt (maxFontHeight * 0.5f));
        }
        else if (isTicked)
        {
            auto tick = getTickShape (1.0f);
            g.fillPath (tick, tick.getTransformToScaleToFit (iconArea.reduced (iconArea.getWidth() / 5, 0).toFloat(), true));
        }

        if (hasSubMenu)
        {
            auto arrowH = 0.6f * getPopupMenuFont().getAscent();

            auto x = static_cast<float> (r.removeFromRight ((int) arrowH).getX());
            auto halfH = static_cast<float> (r.getCentreY());

            Path path;
            path.startNewSubPath (x, halfH - arrowH * 0.5f);
            path.lineTo (x + arrowH * 0.6f, halfH);
            path.lineTo (x, halfH + arrowH * 0.5f);

            g.strokePath (path, PathStrokeType (2.0f));
        }

        r.removeFromRight (3);
        g.drawFittedText (text, r, Justification::centredLeft, 1);

        if (shortcutKeyText.isNotEmpty())
        {
            auto f2 = font;
            f2.setHeight (f2.getHeight() * 0.75f);
            f2.setHorizontalScale (0.95f);
            g.setFont (f2);

            g.drawText (shortcutKeyText, r, Justification::centredRight, true);
        }
    }
}

//[/MiscUserDefs]

//==============================================================================
ColourEditComponent::ColourEditComponent (Colour initialColour)
    : Button("ColourEditComponent")
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    //btnColourPicker.reset (new juce::TextButton ("btnColourPicker"));
    //addAndMakeVisible (btnColourPicker.get());
    //btnColourPicker->setButtonText (TRANS(".."));
    //btnColourPicker->addListener (this);

    //btnColourPicker->setBounds (88, 0, 24, 24);

    //colourCombo.reset (new ColourComboBox ("colourCombo"));
    //addAndMakeVisible (colourCombo.get());
    //colourCombo->setEditableText (true);
    //colourCombo->setJustificationType (juce::Justification::centredLeft);
    //colourCombo->setTextWhenNothingSelected (juce::String());
    //colourCombo->setTextWhenNoChoicesAvailable (translate("NoChoices"));
    //colourCombo->addListener (this);

    //colourCombo->setBounds (0, 0, 79, 24);


    //[UserPreSize]
    //[/UserPreSize]

    //setSize (120, 32);


    //[Constructor] You can add your own custom stuff here..
    //colourComboLookAndFeel.reset(new ColourComboLookAndFeel());
	//colourComboLookAndFeel->setColour(juce::ComboBox::backgroundColourId, findColour(juce::ComboBox::backgroundColourId));
	//colourComboLookAndFeel->setColour(juce::ComboBox::textColourId, findColour(juce::ComboBox::textColourId));

	//colourCombo->setLookAndFeel(colourComboLookAndFeel.get());

    currentColour = initialColour;
    currentColourAsString = currentColour.toString();

    setButtonText("v");
    //[/Constructor]
}

ColourEditComponent::~ColourEditComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    //btnColourPicker = nullptr;
    //colourCombo = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
//void ColourEditComponent::paint (juce::Graphics& g)
//{
//    //[UserPrePaint] Add your own custom painting code here..
//    //[/UserPrePaint]
//
//    g.fillAll (juce::Colour (0xffb8d0de));
//
//    //[UserPaint] Add your own custom painting code here..
//	g.fillAll(findColour(ResizableWindow::backgroundColourId));
//
//	colourComboLookAndFeel->setColour(juce::ComboBox::arrowColourId, findColour(juce::ComboBox::arrowColourId));
//	colourComboLookAndFeel->setColour(TerpstraKeyEdit::backgroundColourId, findColour(TerpstraKeyEdit::backgroundColourId));
//    //[/UserPaint]
//}
//
//void ColourEditComponent::resized()
//{
//    //[UserPreResize] Add your own custom resize code here..
//    //[/UserPreResize]
//
//    //[UserResized] Add your own custom resize handling here..
//    //[/UserResized]
//}
//
//void ColourEditComponent::buttonClicked (juce::Button* buttonThatWasClicked)
//{
//    //[UserbuttonClicked_Pre]
//    //[/UserbuttonClicked_Pre]
//
//    if (buttonThatWasClicked == btnColourPicker.get())
//    {
//        //[UserButtonCode_btnColourPicker] -- add your button handler code here..
//		auto colourSelector = std::make_unique<ColourSelector>(ColourSelector::showSliders | ColourSelector::showColourspace);
//		colourSelector->setName("Colour picker");
//		colourSelector->addChangeListener(this);
//
//		Colour currentColor = colourCombo->getColourAsObjectFromText(ColourComboBox::AddColourToComboBox);
//
//		colourSelector->setCurrentColour(currentColor);
//
//		colourSelector->setColour(ColourSelector::backgroundColourId, currentColor);
//		colourSelector->setSize(300, 400);
//
//		CallOutBox::launchAsynchronously(std::move(colourSelector), buttonThatWasClicked->getScreenBounds(), nullptr);
//        //[/UserButtonCode_btnColourPicker]
//    }
//
//    //[UserbuttonClicked_Post]
//    //[/UserbuttonClicked_Post]
//}
//
//void ColourEditComponent::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
//{
//    //[UsercomboBoxChanged_Pre]
//    //[/UsercomboBoxChanged_Pre]
//
//    if (comboBoxThatHasChanged == colourCombo.get())
//    {
//        //[UserComboBoxCode_colourCombo] -- add your combo box handling code here..
//
//        // Set background colour of combo box to current selection
//         auto bgColour = findColour(TerpstraKeyEdit::backgroundColourId).overlaidWith(
//            colourCombo->getColourAsObjectFromText(ColourComboBox::DoNotAddColourToCombobox).withAlpha(TERPSTRASINGLEKEYCOLOURALPHA));
//
//        colourCombo->setColour(juce::ComboBox::backgroundColourId, bgColour);
//        colourCombo->setColour(juce::ComboBox::textColourId, bgColour.contrasting(1.0));
//
//		// Notify parent that value has changed and can be sent to MIDI controller
//		sendChangeMessage();
//
//        //[/UserComboBoxCode_colourCombo]
//    }
//
//    //[UsercomboBoxChanged_Post]
//    //[/UsercomboBoxChanged_Post]
//}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void ColourEditComponent::paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    Colour backgroundColour = findColour(TextButton::ColourIds::buttonColourId);
    Colour textColour       = backgroundColour.contrasting();

    if (shouldDrawButtonAsHighlighted)
    {
        backgroundColour = (isMouseButtonDown())
            ? backgroundColour.darker(0.25f)
            : backgroundColour.brighter(0.25f);
    }

    g.setColour(backgroundColour);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), getHeight() / 3.0f);

    g.setFont(Font().withHeight(proportionOfHeight(0.75f)).withHorizontalScale(2.0f));
    g.setColour(textColour);
    g.drawFittedText(getButtonText(), getLocalBounds(), Justification::centred, 1);
}

//void ColourEditComponent::changeListenerCallback(ChangeBroadcaster *source)
//{
//	ColourSelector* cs = dynamic_cast <ColourSelector*> (source);
//	Colour currentColor = cs->getCurrentColour();
//	colourCombo->setTextFieldToColourAsObject(currentColor);
//}

void ColourEditComponent::setColour(String colourAsString)
{
	//jassert(colourCombo != nullptr);

    currentColourAsString = colourAsString;
    currentColour = Colour::fromString(colourAsString);
    Component::setColour(TextButton::ColourIds::buttonColourId, currentColour);
    repaint();

    // Notify parent that value has changed and can be sent to MIDI controller
    sendChangeMessage();
}

String ColourEditComponent::getColourAsString()
{
	//jassert(colourCombo != nullptr);

    return currentColourAsString;
}

int ColourEditComponent::getColourAsNumber()
{
	//jassert(colourCombo != nullptr);

	return currentColourAsString.getHexValue32();
}

Colour ColourEditComponent::getColourAsObject()
{
    return currentColour;
}

//void ColourEditComponent::addColourToBox(int newColourAsNumber)
//{
//	//jassert(colourCombo != nullptr);
//	colourCombo->addColourToBox(newColourAsNumber);
//}

void ColourEditComponent::colourChangedCallback(ColourSelectionBroadcaster* source, Colour newColour)
{
    setColour(newColour.toString());
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
                 snapShown="1" overlayOpacity="0.330" fixedSize="1" initialWidth="120"
                 initialHeight="32">
  <BACKGROUND backgroundColour="ffb8d0de"/>
  <TEXTBUTTON name="btnColourPicker" id="fb8f62a75c5cd9ec" memberName="btnColourPicker"
              virtualName="" explicitFocusOrder="0" pos="88 0 24 24" buttonText=".."
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

