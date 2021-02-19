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
	: isSelected(false), keyColour(juce::Colour()), keyType(TerpstraKey::noteOnNoteOff)
{
	midiNoteLabel = new Label("midiNoteLabel", "0");
	addAndMakeVisible(midiNoteLabel);
	midiNoteLabel->setJustificationType(Justification::centredRight);
	midiNoteLabel->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::GothamNarrowBold));
	
	midiChannelLabel = new Label("midiChannelLabel", "0");
	addAndMakeVisible(midiChannelLabel);
	midiChannelLabel->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::GothamNarrowMedium));
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

	String newTooltip = translate("KeyType") + " ";
	switch (keyType)
	{
	case TerpstraKey::noteOnNoteOff:
		newTooltip += translate("NoteOnOff");
		break;
	case TerpstraKey::continuousController:
		newTooltip += translate("ContinuousController");
		break;
	case TerpstraKey::lumaTouch:
		newTooltip += translate("Lumatouch");
		break;
	default:
		jassertfalse;
		newTooltip += translate("Unknown");
		break;
	}

	newTooltip += newLine;
	newTooltip += translate("KeyColour") + " " + keyColour.toDisplayString(false);
	
	setTooltip(newTooltip);
	midiNoteLabel->setTooltip(newTooltip);
	midiChannelLabel->setTooltip(newTooltip);

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
	Colour bgColour = findColour(backgroundColourId).overlaidWith(currentValue.colour.withAlpha(TERPSTRASINGLEKEYCOLOURALPHA));
    Colour textColour = bgColour.contrasting(0.7f);

	if (bgColour.getPerceivedBrightness() < 0.5)
	{
		textColour = textColour.brighter();
	}

    midiChannelLabel->setColour(juce::Label::textColourId, textColour);
    midiNoteLabel->setColour(juce::Label::textColourId, textColour);

	// Look depending on Key type
	if (currentValue.keyType == TerpstraKey::continuousController)
	{
		// Key type is continuous controller. Set colour gradient.
        float w = this->getWidth();
        float h = this->getHeight();
		g.setGradientFill(
			ColourGradient(bgColour.darker(), w * 0.5f, h * 0.5f, bgColour.brighter(), w * 0.5f, 0.0f, true));
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
		midiChannelLabel->setAlpha(0.3f);
		midiNoteLabel->setAlpha(0.3f);
	}
	else
	{
		midiChannelLabel->setAlpha(1.0f);
		midiNoteLabel->setAlpha(1.0f);
	}
}

void TerpstraKeyEdit::resized()
{
	Point<int> centre = getLocalBounds().getCentre();
	float radius = keySize * 0.5f;

	// Draw hexagon
	hexPath.clear();
	hexPath.addPolygon(centre.toFloat(), 6, radius, TERPSTRASINGLEKEYROTATIONANGLE);

	float lblSize = radius * TERPSTRASINGLEKEYLABELSIZE;
	float lblOffset = radius * 0.333333f;

	midiChannelLabel->setSize(radius, lblSize);
	midiChannelLabel->setCentrePosition(centre.translated(-lblOffset, -(radius * channelLabelRadiusScalar)));

	midiNoteLabel->setSize(radius, lblSize);
	midiNoteLabel->setCentrePosition(centre.translated(0, lblOffset));
}

void TerpstraKeyEdit::setKeySize(float keySizeIn)
{
	keySize = keySizeIn;
	setSize(keySize, keySize);
}

