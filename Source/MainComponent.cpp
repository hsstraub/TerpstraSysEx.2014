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
	// Key set fields
	for (int i = 0; i < NUMBEROFBOARDS; i++)
	{
		// Macro button
		macroButtons[i] = new TerpstraMacroButton();
		addAndMakeVisible(macroButtons[i]);

		// Paint set fields from right to left
		// (This will not matter any more when the images' backgrounds are transparent)
		// Width and heigth: were taken from image
		terpstraSetSelectors[4-i] = new TerpstraKeySetEdit();
		addAndMakeVisible(terpstraSetSelectors[4-i]);
		terpstraSetSelectors[4-i]->addListener(this);
	}

	// Single Key fields

	// Rows from right to left
	// first row
	terpstraKeyFields[0] = new TerpstraKeyEdit();
	addAndMakeVisible(terpstraKeyFields[0]);

	terpstraKeyFields[1] = new TerpstraKeyEdit();
	addAndMakeVisible(terpstraKeyFields[1]);

	// 8 identical rows
	for (int row = 0; row < 8; row++)
	{
		for (int i = 0; i < 6; i++)
		{
			terpstraKeyFields[2 + 6 * row + i] = new TerpstraKeyEdit();
			addAndMakeVisible(terpstraKeyFields[2 + 6 * row + i]);
		}
	}

	// 9th row
	for (int i = 0; i < 5; i++)
	{
		terpstraKeyFields[50 + i] = new TerpstraKeyEdit();
		addAndMakeVisible(terpstraKeyFields[50 + i]);
	}

	// 10th row
	terpstraKeyFields[55] = new TerpstraKeyEdit();
	addAndMakeVisible(terpstraKeyFields[55]);

	for (int i = 0; i < TERPSTRABOARDSIZE; i++)
		terpstraKeyFields[i]->addMouseListener(this, true);

	// Midi input + output
	midiEditArea = new MidiEditArea();
	addAndMakeVisible(midiEditArea);
	midiEditArea->addSendAllButtonListener(this);

	// Edit function area
	noteEditArea = new NoteEditArea();
	addAndMakeVisible(noteEditArea);

	// Initial size 
	setSize(DEFAULTMAINWINDOWWIDTH, DEFAULTMAINWINDOWHEIGHT);

	// Select first board and first key
	changeSetSelection(0);
	changeSingleKeySelection(0);
}

MainContentComponent::~MainContentComponent()
{
	deleteAllChildren();
}

void MainContentComponent::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	noteEditArea->restoreStateFromPropertiesFile(propertiesFile);
}

void MainContentComponent::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	noteEditArea->saveStateToPropertiesFile(propertiesFile);
}

// Set the current mapping to be edited to the value passed in parameter
void MainContentComponent::setData(TerpstraKeyMapping& newData)
{
	mappingData = newData;

	noteEditArea->onSetData(newData);

	changeSetSelection(-1);
	changeSetSelection(0);
}

// Copy the edited mapping to the variable passed as parameter
void MainContentComponent::getData(TerpstraKeyMapping& newData)
{
	newData = mappingData;
}

void MainContentComponent::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message)
{
	if (message.isController())
	{
		// Established that a controller change has occurred, which is due
		// to a keyboard macro button being pressed. Now Channel will hold
		// a value from 0 to 15, and InData1 (controller number) will hold either 16 or 17.
		// The formula to establish which button is pressed is
		// ButtonNum = channel*2 + (InData1 - 16)
		// One button subcomponent holds 2 buttons
		int buttonSubwinIndex = message.getChannel() - 1;
		if (buttonSubwinIndex >= 0 && buttonSubwinIndex < 5)
		{
			if (message.getControllerNumber() == 16 || message.getControllerNumber() == 17)
			{
				// Highlight controller button on/off. Left side, if controller == 17, right side, if it is 16
				macroButtons[buttonSubwinIndex]->setIsSelected(
					message.getControllerNumber() == 17 ? TerpstraMacroButton::leftbutton : TerpstraMacroButton::rightbutton,
					message.getControllerValue() >= 0x3f);

				// Send parametrization file to controller, if one is specified
				noteEditArea->handleIncomingMidiMessage(source, message);
			}
		}
	}
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

	int newWidth = getWidth();
	int newHeight = getHeight();

	double newResizeFactor = jmin((double)newWidth / DEFAULTMAINWINDOWWIDTH, (double)newHeight / DEFAULTMAINWINDOWHEIGHT);
	jassert(newResizeFactor > 0.0);
	double newDecreaseFactor = jmin(newResizeFactor, 1.0);
	jassert(newDecreaseFactor > 0.0);

	// New width and height of subset fields
	int newSubsetWidth = (newWidth - 2 * MAINWINDOWFIRSTCOLPOS) / 5;
	if (newSubsetWidth <= 0)
		newSubsetWidth = DEFAULTTERPSTRAKEYSETWIDTH;

	int newSubsetHeight = newHeight - MIDIEDITAREAHEIGHT - EDITFUNCTIONAREAHEIGHT;
	if (newSubsetHeight < TERPSTRAKEYSETFLDFIRSTYPOS + MINIMALTERPSTRAKEYSETHEIGHT)
		newSubsetHeight = MINIMALTERPSTRAKEYSETHEIGHT;

	int newSubsetFirstYPos = TERPSTRAKEYSETFLDFIRSTYPOS * newDecreaseFactor;
	int newSingleKeyFieldFirstYPos = newSubsetHeight + TERPSTRASINGLEKEYFIELDRIMABOVE;

	// Key set fields
	for (int i = 0; i < NUMBEROFBOARDS; i++)
	{
		// Macro button
		macroButtons[i]->setBounds(FIRSTMACROBUTTOONCOLPOS + i*newSubsetWidth, MACROBUTTONYPOS, DEFAULTMACROBUTTONWIDTH*newDecreaseFactor, DEFAULTMACROBUTTONHEIGHT*newDecreaseFactor);

		// Paint set fields from right to left
		// (This will not matter any more when the images' backgrounds are transparent)
		// Width and heigth: were taken from image
		terpstraSetSelectors[4 - i]->setBounds(MAINWINDOWFIRSTCOLPOS + (4 - i)*newSubsetWidth, newSubsetFirstYPos, newSubsetWidth, newSubsetHeight);
	}

	// Single Key fields

	// Transformation Rotate slightly counterclockwise
	int x = MAINWINDOWFIRSTCOLPOS;
	int y = newSingleKeyFieldFirstYPos;
	AffineTransform transform = AffineTransform::translation(-x, -y);
	transform = transform.rotated(TERPSTRASINGLEKEYROTATIONANGLE);
	transform = transform.translated(x, y);

	// Rows from right to left
	// first row
	x = MAINWINDOWFIRSTCOLPOS;
	y = newSingleKeyFieldFirstYPos;
	transform.transformPoint(x, y);
	terpstraKeyFields[0]->setBounds(x, y, TERPSTRASINGLEKEYFLDSIZE, TERPSTRASINGLEKEYFLDSIZE);

	x = MAINWINDOWFIRSTCOLPOS + TERPSTRASINGLEKEYFLDSIZE;
	y = newSingleKeyFieldFirstYPos;
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

		int ybasepos = newSingleKeyFieldFirstYPos + 3 * (row + 1) * TERPSTRASINGLEKEYFLDSIZE / 4;

		for (int i = 0; i < 6; i++)
		{
			x = xbasepos + i*TERPSTRASINGLEKEYFLDSIZE;
			y = ybasepos;
			transform.transformPoint(x, y);
			terpstraKeyFields[2 + 6 * row + i]->setBounds(x, y, TERPSTRASINGLEKEYFLDSIZE, TERPSTRASINGLEKEYFLDSIZE);
		}
	}

	// 9th row
	for (int i = 0; i < 5; i++)
	{
		x = MAINWINDOWFIRSTCOLPOS + (2 * i + 3)*TERPSTRASINGLEKEYFLDSIZE / 2;
		y = newSingleKeyFieldFirstYPos + 3 * 9 * TERPSTRASINGLEKEYFLDSIZE / 4;
		transform.transformPoint(x, y);
		terpstraKeyFields[50 + i]->setBounds(x, y, TERPSTRASINGLEKEYFLDSIZE, TERPSTRASINGLEKEYFLDSIZE);
	}

	// 10th row
	x = MAINWINDOWFIRSTCOLPOS + 5 * TERPSTRASINGLEKEYFLDSIZE;
	y = newSingleKeyFieldFirstYPos + 3 * 10 * TERPSTRASINGLEKEYFLDSIZE / 4;
	transform.transformPoint(x, y);
	terpstraKeyFields[55]->setBounds(x, y, TERPSTRASINGLEKEYFLDSIZE, TERPSTRASINGLEKEYFLDSIZE);

	// Midi input + output
	midiEditArea->setBounds(EDITAREAFIRSTCOLPOS, newSubsetHeight, EDITAREAWIDTH, MIDIEDITAREAHEIGHT);

	// Edit function area
	noteEditArea->setBounds(EDITAREAFIRSTCOLPOS, newSubsetHeight + MIDIEDITAREAHEIGHT, EDITAREAWIDTH, EDITFUNCTIONAREAHEIGHT);
}

void MainContentComponent::buttonClicked(Button *button)
{
	if (midiEditArea->isSendAllButton(button))
	{
		TerpstraSysExApplication::getApp().getMidiDriver().sendAndSaveCompleteMapping(mappingData);
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
			this->noteEditArea->performMouseClickEdit(currentSetSelection, i);

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