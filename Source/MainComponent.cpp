/*
  ==============================================================================

  MainComponent.cpp
  Created: XXX.2014
  Author:  hsstraub

  ==============================================================================
*/

#include "MainComponent.h"
#include "ViewConstants.h"
#include "Main.h"


//==============================================================================
MainContentComponent::MainContentComponent()
: currentSetSelection(-1), currentSingleKeySelection(-1)
{
	setSize(MAINWINDOWWIDTH, MAINWINDOWHEIGHT);

	// SysEx command buttons
	// XXX

	// Key set fields
	for (int i = 0; i < NUMBEROFBOARDS; i++)
	{
		// Paint them from right to left
		// (This will not matter any more when the images' backgrounds are transparent)
		// Width and heigth: were taken from image
		terpstraSetSelectors[4-i] = new TerpstraKeySetEdit();
		addAndMakeVisible(terpstraSetSelectors[4-i]);
		terpstraSetSelectors[4 - i]->setBounds(MAINWINDOWFIRSTCOLPOS + (4 - i)*TERPSTRAKEYSETFLDSPAN, TERPSTRAKEYSETFLDFIRSTYPOS, terpstraSetSelectors[4 - i]->getWidth(), terpstraSetSelectors[4 - i]->getHeight());
		terpstraSetSelectors[4-i]->addListener(this);
	}

	// Single Key fields

	// Transformation Rotate slightly counterclockwise
	int x = MAINWINDOWFIRSTCOLPOS;
	int y = TERPSTRASINGLEKEYFIELDFIRSTYPOS;
	AffineTransform transform = AffineTransform::translation(-x, -y);
	transform = transform.rotated(TERPSTRASINGLEKEYROTATIONANGLE);
	transform = transform.translated(x, y);

	// Rows from right to left
	// first row
	x = MAINWINDOWFIRSTCOLPOS + TERPSTRASINGLEKEYFLDSIZE;
	y = TERPSTRASINGLEKEYFIELDFIRSTYPOS;
	transform.transformPoint(x, y);
	terpstraKeyFields[0] = new TerpstraKeyEdit();
	addAndMakeVisible(terpstraKeyFields[0]);
	terpstraKeyFields[0]->setBounds(x, y, TERPSTRASINGLEKEYFLDSIZE, TERPSTRASINGLEKEYFLDSIZE);

	terpstraKeyFields[1] = new TerpstraKeyEdit();
	addAndMakeVisible(terpstraKeyFields[1]);
	x = MAINWINDOWFIRSTCOLPOS;
	y = TERPSTRASINGLEKEYFIELDFIRSTYPOS;
	transform.transformPoint(x, y);
	terpstraKeyFields[1]->setBounds(x, y, TERPSTRASINGLEKEYFLDSIZE, TERPSTRASINGLEKEYFLDSIZE);

	// 8 identical rows
	for (int row = 0; row < 8; row++)
	{
		int xbasepos;
		if (row % 2 == 0)
			xbasepos = MAINWINDOWFIRSTCOLPOS + TERPSTRASINGLEKEYFLDSIZE / 2;
		else
			xbasepos = MAINWINDOWFIRSTCOLPOS;

		int ybasepos = TERPSTRASINGLEKEYFIELDFIRSTYPOS + 3 * (row + 1) * TERPSTRASINGLEKEYFLDSIZE / 4;

		for (int i = 0; i < 6; i++)
		{
			terpstraKeyFields[2 + 6 * row + i] = new TerpstraKeyEdit();
			addAndMakeVisible(terpstraKeyFields[2 + 6 * row + i]);
			x = xbasepos + (5-i)*TERPSTRASINGLEKEYFLDSIZE;
			y = ybasepos;
			transform.transformPoint(x, y);
			terpstraKeyFields[2 + 6 * row + i]->setBounds(x, y, TERPSTRASINGLEKEYFLDSIZE, TERPSTRASINGLEKEYFLDSIZE);
		}
	}

	// 9th row
	for (int i = 0; i < 5; i++)
	{
		terpstraKeyFields[50 + i] = new TerpstraKeyEdit();
		addAndMakeVisible(terpstraKeyFields[50 + i]);
		x = MAINWINDOWFIRSTCOLPOS + (2 * (4-i) + 3)*TERPSTRASINGLEKEYFLDSIZE / 2;
		y = TERPSTRASINGLEKEYFIELDFIRSTYPOS + 3 * 9 * TERPSTRASINGLEKEYFLDSIZE / 4;
		transform.transformPoint(x, y);
		terpstraKeyFields[50 + i]->setBounds(x, y, TERPSTRASINGLEKEYFLDSIZE, TERPSTRASINGLEKEYFLDSIZE);
	}

	// 10th row
	terpstraKeyFields[55] = new TerpstraKeyEdit();
	addAndMakeVisible(terpstraKeyFields[55]);
	x = MAINWINDOWFIRSTCOLPOS + 5 * TERPSTRASINGLEKEYFLDSIZE;
	y = TERPSTRASINGLEKEYFIELDFIRSTYPOS + 3 * 10 * TERPSTRASINGLEKEYFLDSIZE / 4;
	transform.transformPoint(x, y);
	terpstraKeyFields[55]->setBounds(x, y, TERPSTRASINGLEKEYFLDSIZE, TERPSTRASINGLEKEYFLDSIZE);

	for (int i = 0; i < TERPSTRABOARDSIZE; i++)
		terpstraKeyFields[i]->addMouseListener(this, true);

	// Midi input + output
	midiEditArea = new MidiEditArea();
	midiEditArea->setMidiDriver(&midiDriver);
	addAndMakeVisible(midiEditArea);
	midiEditArea->setBounds(EDITAREAFIRSTCOLPOS, 250, EDITAREAWIDTH, MIDIEDITAREAHEIGHT);
	midiEditArea->addSendAllButtonListener(this);

	noteEditArea = new NoteEditArea();
	addAndMakeVisible(noteEditArea);
	noteEditArea->setBounds(EDITAREAFIRSTCOLPOS, 250 + MIDIEDITAREAHEIGHT, EDITAREAWIDTH, EDITFUNCTIONAREAHEIGHT);

	// Select first board and first key
	changeSetSelection(0);
	changeSingleKeySelection(0);
}

MainContentComponent::~MainContentComponent()
{
	deleteAllChildren();
}

// Transfer of data
void MainContentComponent::setData(TerpstraKeyMapping& newData)
{
	mappingData = newData;

	changeSetSelection(-1);
	changeSetSelection(0);
}

void MainContentComponent::getData(TerpstraKeyMapping& newData)
{
	newData = mappingData;
}


void MainContentComponent::paint (Graphics& g)
{
	g.fillAll(Colour(MAINWINDOWBGCOLOUR));

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainContentComponent::buttonClicked(Button *button)
{
	if (midiEditArea->isSendAllButton(button))
	{
		midiDriver.sendAndSaveCompleteMapping(mappingData);
	}
	else
	{
		for (int i = 0; i < NUMBEROFBOARDS; i++)
		{
			if (button == terpstraSetSelectors[i])
			{
				changeSetSelection(i);
				break;
			}
		}
	}
}

void MainContentComponent::mouseDown(const MouseEvent &event)
{
	// Selection of single key fields
	for (int i = 0; i < TERPSTRABOARDSIZE; i++)
	{
		if (event.eventComponent == terpstraKeyFields[i] || event.eventComponent->getParentComponent() == terpstraKeyFields[i])
		{
			// Select field
			changeSingleKeySelection(i);

			// Perform the edit, according to edit mode. Including sending to device
			this->noteEditArea->PerformMouseClickEdit(mappingData, currentSetSelection, i, midiDriver);

			// Refresh display
			changeSetSelection(currentSetSelection, true);

			// Mark that there are changes
			TerpstraSysExApplication::getApp().setHasChangesToSave(true);

			break;
		}
	}
}

void MainContentComponent::changeSetSelection(int newSelection, bool forceRefresh)
{
	if (newSelection != currentSetSelection || forceRefresh)
	{
		// Unselect previous set
		// saving the data was done in click event on the single key fields
		if (currentSetSelection >= 0 && currentSetSelection < NUMBEROFBOARDS)
			terpstraSetSelectors[currentSetSelection]->setIsSelected(false);

		// Set data of new selection
		if (newSelection >= 0 && newSelection < NUMBEROFBOARDS )
		{
			for (int i = 0; i < TERPSTRABOARDSIZE; i++)
				terpstraKeyFields[i]->setValue(mappingData.sets[newSelection].theKeys[i]);
		}

		currentSetSelection = newSelection;
	}

	// Set toggle state in any case (override default imagebutton functionality)
	if ( currentSetSelection >= 0 && currentSetSelection < NUMBEROFBOARDS )
		terpstraSetSelectors[currentSetSelection]->setIsSelected(true);
}

void MainContentComponent::changeSingleKeySelection(int newSelection)
{
	// Unselect previous key
	if (currentSingleKeySelection >= 0 && currentSingleKeySelection < TERPSTRABOARDSIZE )
		terpstraKeyFields[currentSingleKeySelection]->setIsSelected(false);

	// Select new key
	currentSingleKeySelection = newSelection;
	if (currentSingleKeySelection >= 0 && currentSingleKeySelection < TERPSTRABOARDSIZE)
		terpstraKeyFields[currentSingleKeySelection]->setIsSelected(true);
}