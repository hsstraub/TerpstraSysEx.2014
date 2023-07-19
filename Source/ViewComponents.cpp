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
	: isSelected(false), keyColour(juce::Colour()), keyType(LumatoneKeyType::noteOnNoteOff)
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
    newValue.ccFaderDefault = ccFaderDefault;

	return newValue;
}

void TerpstraKeyEdit::setValue(TerpstraKey newValue)
{
    if (newValue.keyType == LumatoneKeyType::disabled || newValue.keyType == LumatoneKeyType::disabledDefault)
    {
        newValue.keyType = LumatoneKeyType::disabled;
        midiNoteLabel->setText("x", juce::NotificationType::sendNotification);
        midiChannelLabel->setText("x", juce::NotificationType::sendNotification);
    }
    else
    {
        midiNoteLabel->setText(String(newValue.noteNumber), juce::NotificationType::sendNotification);
        midiChannelLabel->setText(String(newValue.channelNumber), juce::NotificationType::sendNotification);
    }
	keyColour = newValue.colour;
	keyType = newValue.keyType;
    ccFaderDefault = newValue.ccFaderDefault;

	String newTooltip = translate("KeyType") + " ";
	switch (keyType)
	{
	case LumatoneKeyType::noteOnNoteOff:
		newTooltip += translate("NoteOnOff");
		break;
	case LumatoneKeyType::continuousController:
		newTooltip += translate("ContinuousController");
        newTooltip += newLine;
        if (ccFaderDefault)
            newTooltip += "CC Default (0->127)";
        else
            newTooltip += "CC Inverted (127->0)";
		break;
	case LumatoneKeyType::lumaTouch:
		newTooltip += translate("Lumatouch");
		break;
	case LumatoneKeyType::disabled:
		newTooltip += translate("Disabled");
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
    if (currentValue.keyType == LumatoneKeyType::continuousController)
	{
		// Key type is continuous controller. Set colour gradient.
        float w = this->getWidth();
        float h = this->getHeight();
        Colour inside, outside;
        if (currentValue.ccFaderDefault)
        {
            outside = bgColour.darker();
            inside = bgColour.brighter();
        }
        else
        {
            outside = bgColour.brighter();
            inside = bgColour.darker();
        }
		g.setGradientFill(
			ColourGradient(inside, w * 0.5f, h * 0.5f, outside, w * 0.5f, 0.0f, true));
	}
	else
	{
		// Key type is note on/note off. Just plain colour.
		g.setColour(bgColour);
	}

	g.fillPath(hexPath);

	if (isSelected)
	{
		// Shrink bounds slightly
		Path hexOutline = Path(hexPath);
		Rectangle<int> hexBounds = getLocalBounds().reduced(1);
		hexOutline.scaleToFit(hexBounds.getX(), hexBounds.getY(), hexBounds.getWidth(), hexBounds.getHeight(), true);

		// Draw line
		g.setColour(lineColor);
		g.strokePath(hexOutline, PathStrokeType(lineWidth));
	}

//	if (currentValue.keyType == LumatoneKeyType::disabled)
//	{
//		float w = this->getWidth();
//		float h = this->getHeight();
//		float xProportion = 0.25f;
//		// Draw X on key
//		g.setColour(bgColour.contrasting(0.5f));
//		g.drawLine(w * xProportion, h * xProportion, w * (1-xProportion), h * (1-xProportion), 2);
//		g.drawLine(w * (1 - xProportion), h * xProportion, w * xProportion, h * (1 - xProportion), 2);
//	}

//    if (currentValue.keyType == LumatoneKeyType::disabled)
//    {
//        TerpstraSysExApplication::getApp().getLookAndFeel().getLabelFont(*midiNoteLabel);
//        g.setColour(textColour);
//        g.setFont(midiChannelLabel->getFont());
//        g.drawText("x", midiChannelLabel->getBounds(), midiChannelLabel->getJustificationType());
//        g.drawText("x", midiNoteLabel->getBounds(), midiChannelLabel->getJustificationType());
//    }
    
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
	//Rectangle<float> hexBounds = hexPath.getBounds().reduced(1, 1);

	float lblSize = radius * TERPSTRASINGLEKEYLABELSIZE;
	float lblOffset = radius * 0.375f;

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

