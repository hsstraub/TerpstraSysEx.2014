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

    //[UserPreSize]
    //[/UserPreSize]

    //setSize (120, 32);


    //[Constructor] You can add your own custom stuff here..

    currentColour = initialColour;
    currentColourAsString = currentColour.toString();

    setButtonText("v");
    //[/Constructor]
}

ColourEditComponent::~ColourEditComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}


//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void ColourEditComponent::paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    Colour backgroundColour = findColour(TextButton::ColourIds::buttonColourId);
    Colour textColour       = backgroundColour.contrasting();

    if (!isEnabled())
    {
        backgroundColour = backgroundColour.darker().withMultipliedSaturation(0.3f);
        textColour = textColour.overlaidWith(Colours::darkgrey.withAlpha(0.6f));
    }

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

void ColourEditComponent::setColour(String colourAsString)
{
	//jassert(colourCombo != nullptr);

    currentColourAsString = colourAsString;
    if (currentColourAsString.length() == 6)
        currentColourAsString = "FF" + colourAsString;

    currentColour = Colour::fromString(currentColourAsString);
    Component::setColour(TextButton::ColourIds::buttonColourId, currentColour);
    repaint();

    // Notify parent that value has changed and can be sent to MIDI controller
    sendChangeMessage();
}

String ColourEditComponent::getColourAsString()
{
    return currentColourAsString;
}

int ColourEditComponent::getColourAsNumber()
{
	return currentColourAsString.getHexValue32();
}

Colour ColourEditComponent::getColourAsObject()
{
    return currentColour;
}

void ColourEditComponent::colourChangedCallback(ColourSelectionBroadcaster* source, Colour newColour)
{
    setColour(newColour.toString());
}


//====================================================================================================

ColourTextEditor::ColourTextEditor(String componentName, String initialString)
    : TextEditor(componentName)
{
    setMultiLine(false);
    setJustification(Justification::centredLeft);
    setTooltip(translate("ColourHexValueEditorTool"));
    setInputRestrictions(6, "0123456789ABCDEFabcdef");
    addListener(this);

    setText(initialString, NotificationType::dontSendNotification);
    checkInputAndUpdate();
}

String ColourTextEditor::checkInputAndUpdate(bool sendSelectorListenerUpdate)
{
    String text = parseTextToColourString(getText().toLowerCase());
    if (text.length() > 0)
    {
        // only accept RGB
        if (text.length() > 6)
            text = text.substring(text.length() - 6);

        // restrict to lowercase display
        setText(text, NotificationType::dontSendNotification);

        if (sendSelectorListenerUpdate)
        {
            String opaque = "ff" + text;
            lastBroadcastedColour = Colour(opaque.getHexValue32());
            selectorListeners.call(&ColourSelectionListener::colourChangedCallback, this, lastBroadcastedColour);
        }
    }

    return text;
}

void ColourTextEditor::resetToLastUpdated(bool sendSelectorListenerUpdate)
{
    NotificationType notification = (sendSelectorListenerUpdate) 
        ? NotificationType::sendNotification 
        : NotificationType::dontSendNotification;

    setText(lastBroadcastedColour.toDisplayString(true).toLowerCase(), notification);
}

/** Called when the user changes the text in some way. */
void ColourTextEditor::textEditorTextChanged(TextEditor&)
{
    // confirm changes
    checkInputAndUpdate();
}

/** Called when the user presses the return key. */
void ColourTextEditor::textEditorReturnKeyPressed(TextEditor&)
{
    // confirm changes
    checkInputAndUpdate();
}

/** Called when the user presses the escape key. */
void ColourTextEditor::textEditorEscapeKeyPressed(TextEditor&)
{
    // reset changes
    resetToLastUpdated();
}

/** Called when the text editor loses focus. */
void ColourTextEditor::textEditorFocusLost(TextEditor& editor)
{
    // confirm changes
    checkInputAndUpdate();
}

Colour ColourTextEditor::getSelectedColour()
{
    String opaque = "ff" + parseTextToColourString(getText());
    return Colour(opaque.getHexValue32());
}

void ColourTextEditor::colourChangedCallback(ColourSelectionBroadcaster* source, Colour newColour)
{
    // Safeguard
    if (this != source)
        setText(newColour.toString().substring(2), false);
}

Colour ColourTextEditor::getLastUpdatedColour() const
{
    return lastBroadcastedColour;
}

String ColourTextEditor::parseTextToColourString(String textIn)
{
    // Skip odd-numbered lengths and those less than 6 for RGB
    if (textIn.length() % 2 == 1 || textIn.length() != 6)
    {
        return String();
    }

    return textIn;
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

