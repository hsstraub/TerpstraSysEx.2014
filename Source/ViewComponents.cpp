/*
  ==============================================================================

    ViewComponents.cpp
    Created: 16 Jun 2014 7:20:34pm
    Author:  hsstraub

  ==============================================================================
*/

#include "ViewComponents.h"
#include "Main.h"

/*
==============================================================================
TerpstraKeyEdit class
==============================================================================
*/

TerpstraKeyEdit::TerpstraKeyEdit()
	: isSelected(false), keyColour(0), keyType(TerpstraKey::noteOnNoteOff)
{
	midiNoteLabel = new Label("midiNoteLabel", "0");
	addAndMakeVisible(midiNoteLabel);
	midiNoteLabel->setJustificationType(Justification::centredRight);
	midiNoteLabel->setFont(LumatoneEditorFonts::UniviaProBold());
	
	midiChannelLabel = new Label("midiChannelLabel", "0");
	addAndMakeVisible(midiChannelLabel);
	midiChannelLabel->setFont(LumatoneEditorFonts::UniviaPro());
	midiChannelLabel->setJustificationType(Justification::centredRight);
}

TerpstraKeyEdit::~TerpstraKeyEdit()
{
	deleteAllChildren();
}

TerpstraKey TerpstraKeyEdit::getValue() const
{
	TerpstraKey newValue;
	newValue.noteNumber = midiNoteLabel->getText().getIntValue();
	newValue.channelNumber = midiChannelLabel->getText().getIntValue();
	newValue.colour = keyColour;
	newValue.keyType = keyType;

	return newValue;
}

void TerpstraKeyEdit::setValue(TerpstraKey newValue)
{
	midiNoteLabel->setText(String(newValue.noteNumber), juce::NotificationType::sendNotification);
	midiChannelLabel->setText(String(newValue.channelNumber), juce::NotificationType::sendNotification);
	keyColour = newValue.colour;
	keyType = newValue.keyType;

	repaint();
}

void TerpstraKeyEdit::setIsSelected(bool newValue)
{
	if (this->isSelected != newValue)
	{
		this->isSelected = newValue;
		repaint();
	}
}

void TerpstraKeyEdit::paint(Graphics& g)
{
	TerpstraKey currentValue = getValue();

	// Selected or not: color and thickness of the line
	float lineWidth = TERPSTRASINGLEKEYFLDLINEWIDTH;
	Colour lineColor = findColour(selectedKeyOutlineId);

	// Color: empty or the parametrized color
	Colour bgColour = findColour(backgroundColourId).overlaidWith(Colour(currentValue.colour)
        .withAlpha(TERPSTRASINGLEKEYCOLOURALPHA));
    Colour textColour = bgColour.contrasting(1.0).darker(1.0f);

    midiChannelLabel->setColour(juce::Label::textColourId, textColour);
    midiNoteLabel->setColour(juce::Label::textColourId, textColour);

	// Look depending on Key type
	if (currentValue.keyType == TerpstraKey::continuousController)
	{
		// Key type is continuous controller. Set colour gradient.
        float w = this->getWidth();
        float h = this->getHeight();
		g.setGradientFill(
			ColourGradient(bgColour.darker(), w / 2.0f, h / 2.0f, bgColour.brighter(), w / 2.0f, 0.0f, true));
	}
	else
	{
		// Key type is note on/note off. Just plain colour.
		g.setColour(bgColour);
	}

	g.fillPath(hexPath);

	if (isSelected)
	{
		// Draw line
		g.setColour(lineColor);
		g.strokePath(hexPath, PathStrokeType(lineWidth));
	}

	// Something parametrized or not?
	if (currentValue.isEmpty())
	{
		midiChannelLabel->setAlpha(0.3);
		midiNoteLabel->setAlpha(0.3);
	}
	else
	{
		midiChannelLabel->setAlpha(1.0);
		midiNoteLabel->setAlpha(1.0);
	}
}

void TerpstraKeyEdit::resized()
{
	Point<int> centre = getLocalBounds().getCentre();
	float radius = keySize / 2.0f;

	// Draw hexagon
	hexPath.clear();
	hexPath.addPolygon(centre.toFloat(), 6, radius, TERPSTRASINGLEKEYROTATIONANGLE);

	float lblSize = radius * TERPSTRASINGLEKEYLABELSIZE;
	float lblOffset = radius / 3.0f;

	midiChannelLabel->setSize(radius, lblSize);
	midiChannelLabel->setCentrePosition(centre.translated(-lblOffset, -(radius / 2.3f)));

	midiNoteLabel->setSize(radius, lblSize);
	midiNoteLabel->setCentrePosition(centre.translated(0, lblOffset));
}

void TerpstraKeyEdit::setKeySize(float keySizeIn)
{
	keySize = keySizeIn;
	setSize(keySize, keySize);
}

