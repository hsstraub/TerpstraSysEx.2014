/*
  ==============================================================================

    ViewComponents.cpp
    Created: 16 Jun 2014 7:20:34pm
    Author:  hsstraub

  ==============================================================================
*/

#include "ViewComponents.h"
#include "ViewConstants.h"

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
	midiNoteLabel->setBounds((TERPSTRASINGLEKEYFLDSIZE - 30) / 2, TERPSTRASINGLEKEYFLDSIZE / 2 - 15, 30, STANDARDLABELHEIGTH);
	midiNoteLabel->setFont( midiNoteLabel->getFont().boldened());

	midiChannelLabel = new Label("midiChannelLabel", "0");
	addAndMakeVisible(midiChannelLabel);
	midiChannelLabel->setBounds((TERPSTRASINGLEKEYFLDSIZE - 25) / 2, TERPSTRASINGLEKEYFLDSIZE / 2, 25, STANDARDLABELHEIGTH);
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
	// Both values are set in calling function when constructing this, are supposed to be TERPSTRASINGLEKEYFLDSIZE
	float w = this->getWidth();
	float h = this->getHeight();

	// Selected or not: color and thickness of the line
	float lineWidth = isSelected ? TERPSTRASELECTEDKEYFLDLINEWIDTH : TERPSTRASINGLEKEYFLDLINEWIDTH;
	juce::Colour lineColor = isSelected ? Colour(TERPSTRASELECTEDFLDLINECOLOUR) : Colours::black;

	// Draw hexagon
	Path hexPath;
	hexPath.startNewSubPath(w / 2.0f, lineWidth);
	hexPath.lineTo(w - lineWidth, h / 4.0f);
	hexPath.lineTo(w - lineWidth, 3.0f * h / 4.0f);
	hexPath.lineTo(w / 2.0f, h - lineWidth);
	hexPath.lineTo(lineWidth, 3.0f * h / 4.0f);
	hexPath.lineTo(lineWidth, h / 4.0f);
	hexPath.closeSubPath();

	// Rotate slightly counterclockwise around the center
	AffineTransform transform = AffineTransform::translation(-w / 2.0f, -h / 2.0f);
	transform = transform.rotated(TERPSTRASINGLEKEYROTATIONANGLE);
	transform = transform.translated(w / 2.0f, h / 2.0f);
	
	hexPath.applyTransform(transform);
	hexPath.scaleToFit(lineWidth, lineWidth, w - lineWidth, h - lineWidth, true);

	// Color: empty or the parametrized color
	TerpstraKey currentValue = getValue();

	Colour bgColour = Colour(MAINWINDOWBGCOLOUR).overlaidWith(Colour(currentValue.colour).withAlpha((uint8)0x40));

	// Look depending on Key type
	if (currentValue.keyType == TerpstraKey::continuousController)
	{
		// Key type is continuous controller. Set colour gradient.
		g.setGradientFill(
			ColourGradient(bgColour.darker(), w / 2.0f, h / 2.0f, bgColour.brighter(), w / 2.0f, 0.0f, true));
	}
	else
	{
		// Key type is note on/note off. Just plain colour.
		g.setColour(bgColour);
	}

	g.fillPath(hexPath);

	// Draw line
	g.setColour(lineColor);
	g.strokePath(hexPath, PathStrokeType(lineWidth));

	// Something parametrized or not?  
	if (currentValue.channelNumber == 0)
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
}

/*
==============================================================================
TerpstraKeySetEdit class
==============================================================================
*/

TerpstraKeySetEdit::TerpstraKeySetEdit()
{
	Image imgUnselected = ImageCache::getFromMemory(BinaryData::OctaveGraphic_png, BinaryData::OctaveGraphic_pngSize);
	Image imgSelected = ImageCache::getFromMemory(BinaryData::OctaveWithSelection_png, BinaryData::OctaveWithSelection_pngSize);

	setImages(true, true, true,
		imgUnselected, 0.5f, Colours::transparentBlack,
		imgUnselected, 0.6f, Colours::transparentBlack,
		imgSelected, 0.9f, Colours::transparentBlack,
		0.5f);

	setClickingTogglesState(true);
}

TerpstraKeySetEdit::~TerpstraKeySetEdit()
{

}

/*
==============================================================================
TerpstraMacroButton class
==============================================================================
*/
TerpstraMacroButton::TerpstraMacroButton()
{
	setIsSelected(rightbutton, false);

	setClickingTogglesState(false);	// ad hoc, to test: set to true. Will be obsolete when MIDI messages are processed
}

TerpstraMacroButton::~TerpstraMacroButton()
{

}

void TerpstraMacroButton::setIsSelected(MACROBUTTONSUBINDEX subIndex, bool newValue) 
{
	Image imgUnselected = ImageCache::getFromMemory(BinaryData::TopEdgeButton_png, BinaryData::TopEdgeButton_pngSize);

	Image imgSelected = subIndex == leftbutton ?
		ImageCache::getFromMemory(BinaryData::TopEdgeLightLeft_png, BinaryData::TopEdgeLightLeft_pngSize) :
		ImageCache::getFromMemory(BinaryData::TopEdgeLightRight_png, BinaryData::TopEdgeLightRight_pngSize);

	setImages(true, true, true,
		imgUnselected, 0.6f, Colours::transparentBlack,
		imgUnselected, 0.6f, Colours::transparentBlack,
		imgSelected, 0.7f, Colours::transparentBlack,
		0.5f);

	setToggleState(newValue, dontSendNotification); 
};


/*
==============================================================================
ColourComboBox class
==============================================================================
*/

ColourComboBox::ColourComboBox(const String& componentName) : ComboBox(componentName)
{
}

void ColourComboBox::setTextFieldToColourAsObject(Colour newColourAsObject,	NotificationType notification)
{
	setText(newColourAsObject.toDisplayString(false));
	
	// XXX Add to box
}

int ColourComboBox::getColourAsNumberFromText(colourComboboxOptions boxOptions)
{
	String colourString = getText();
	
	// XXX validation of colour value
	int colourAsNumber = colourString.getHexValue32();

	if (boxOptions == colourComboboxOptions::AddColourToComboBox)
		addColourToBox(colourString);

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
 class
==============================================================================
*/

VelocityCurveBeam::VelocityCurveBeam()
{
	beamValue = 0;
}

VelocityCurveBeam::~VelocityCurveBeam()
{

}

void VelocityCurveBeam::setValue(int newValue)
{
	beamValue = newValue;
	// XXX Neighbouring beams: change, so that the curve is monotonous. Here or in parent...

	repaint();
}

void VelocityCurveBeam::paint(Graphics& g)
{
	float w = this->getWidth();
	float h = this->getHeight();

	float velocityBeamUnitHeight = h / 128;

	Path rectPath;
	rectPath.startNewSubPath(0, h);
	rectPath.lineTo(w, h);
	rectPath.lineTo(w, h - beamValue * velocityBeamUnitHeight);
	rectPath.lineTo(0, h - beamValue * velocityBeamUnitHeight);
	rectPath.closeSubPath();

	g.setColour(Colour(MAINWINDOWSELECTEDCOLOUR));
	g.fillPath(rectPath);
}

void VelocityCurveBeam::resized()
{

}
