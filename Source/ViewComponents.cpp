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
: isSelected(false)
{
	midiNoteLabel = new Label("midiNoteLabel", "0");
	addAndMakeVisible(midiNoteLabel);
	midiNoteLabel->setBounds((TERPSTRASINGLEKEYFLDSIZE - 30) / 2, TERPSTRASINGLEKEYFLDSIZE / 2 - 15, 30, STANDARDLABELHEIGTH);
	midiNoteLabel->setFont( midiNoteLabel->getFont().boldened());

	//channelTextLabel = new Label("channelTextLabel", "Ch.");
	//addAndMakeVisible(channelTextLabel);
	//channelTextLabel->setBounds(TERPSTRASINGLEKEYFLDSIZE / 2-10, 25, 10, 10);
	//channelTextLabel->setText("Ch.", sendNotification);

	midiChannelLabel = new Label("midiChannelLabel", "0");
	addAndMakeVisible(midiChannelLabel);
	midiChannelLabel->setBounds((TERPSTRASINGLEKEYFLDSIZE - 25) / 2, TERPSTRASINGLEKEYFLDSIZE / 2, 25, STANDARDLABELHEIGTH);

	//channelTextLabel->attachToComponent(midiChannelLabel, true);
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
	// XXX colour

	return newValue;
}

void TerpstraKeyEdit::setValue(TerpstraKey newValue)
{
	midiNoteLabel->setText(String(newValue.noteNumber), juce::NotificationType::sendNotification);
	midiChannelLabel->setText(String(newValue.channelNumber), juce::NotificationType::sendNotification);
	// XXX colour

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

	// Draw hexagon
	Path hexPath;
	hexPath.startNewSubPath(w / 2.0f, TERPSTRASINGLEKEYFLDLINEWIDTH );
	hexPath.lineTo(w - TERPSTRASINGLEKEYFLDLINEWIDTH, h / 4.0f);
	hexPath.lineTo(w - TERPSTRASINGLEKEYFLDLINEWIDTH, 3.0f * h / 4.0f);
	hexPath.lineTo(w / 2.0f, h - TERPSTRASINGLEKEYFLDLINEWIDTH);
	hexPath.lineTo(TERPSTRASINGLEKEYFLDLINEWIDTH, 3.0f * h / 4.0f);
	hexPath.lineTo(TERPSTRASINGLEKEYFLDLINEWIDTH, h / 4.0f);
	hexPath.closeSubPath();

	// Rotate slightly counterclockwise around the center
	AffineTransform transform = AffineTransform::translation(-w / 2.0f, -h / 2.0f);
	transform = transform.rotated(TERPSTRASINGLEKEYROTATIONANGLE);
	transform = transform.translated(w / 2.0f, h / 2.0f);
	
	hexPath.applyTransform(transform);
	hexPath.scaleToFit(TERPSTRASINGLEKEYFLDLINEWIDTH, TERPSTRASINGLEKEYFLDLINEWIDTH, w - TERPSTRASINGLEKEYFLDLINEWIDTH, h - TERPSTRASINGLEKEYFLDLINEWIDTH, true );
	// Selected or not: fill
	if (isSelected)
		g.setColour(Colour(MAINWINDOWSELECTEDCOLOUR));
	else if (getValue().isEmpty() )
		//g.setColour(Colour(MAINWINDOWBGCOLOUR).withMultipliedSaturation(0.5));
		//g.setColour(Colour(MAINWINDOWBGCOLOUR).withMultipliedSaturation(0.5).darker());
		g.setColour(Colour(MAINWINDOWBGCOLOUR).darker(0.08f));
	else
		g.setColour(Colour(MAINWINDOWBGCOLOUR));
	// XXX yet different colour if edited

	g.fillPath(hexPath);
	g.setColour(Colours::black);
	g.strokePath(hexPath, PathStrokeType(TERPSTRASINGLEKEYFLDLINEWIDTH));
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
	//Image imgSelected = ImageCache::getFromMemory(BinaryData::OctaveOutline_png, BinaryData::OctaveOutline_pngSize);

	setImages(true, true, true,
		imgUnselected, 0.3f, Colours::transparentBlack,
		imgUnselected, 0.6f, Colours::transparentBlack,
		imgUnselected, 0.9f, Colour(MAINWINDOWSELECTEDCOLOUR),
		0.5f);

	setClickingTogglesState(true);
}

TerpstraKeySetEdit::~TerpstraKeySetEdit()
{

}

