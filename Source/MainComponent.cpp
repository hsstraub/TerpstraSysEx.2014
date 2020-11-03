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
	: currentSetSelection(-1), currentSingleKeySelection(-1), copiedSubBoardData()
{
	// Key set fields
	for (int i = 0; i < NUMBEROFBOARDS; i++)
	{
		// Paint set fields from right to left
		// (This will not matter any more when the images' backgrounds are transparent)
		// Width and heigth: were taken from image
		terpstraSetSelectors[4-i].reset(new OctaveBoardComponent(4-i));
		addAndMakeVisible(terpstraSetSelectors[4-i].get());
		terpstraSetSelectors[4 - i]->addMouseListener(this, true);
	}

	// Single Key fields
	for (int i = 0; i < TERPSTRABOARDSIZE; i++)
	{
		terpstraKeyFields[i].reset(new TerpstraKeyEdit());
		addAndMakeVisible(terpstraKeyFields[i].get());
		terpstraKeyFields[i]->addMouseListener(this, true);
	}

	// Midi input + output
	midiEditArea.reset(new MidiEditArea());
	addAndMakeVisible(midiEditArea.get());

	// Edit function area
	noteEditArea.reset(new NoteEditArea());
	addAndMakeVisible(noteEditArea.get());

	TerpstraSysExApplication::getApp().getMidiDriver().addListener(this);

	// Initial size
	setSize(DEFAULTMAINWINDOWWIDTH, DEFAULTMAINWINDOWHEIGHT);

	// Select first board and first key
	changeSetSelection(0);
	changeSingleKeySelection(0);
}

MainContentComponent::~MainContentComponent()
{
    TerpstraSysExApplication::getApp().getMidiDriver().removeListener(this);

	for (int i = 0; i < NUMBEROFBOARDS; i++)
	{
		terpstraSetSelectors[i] = nullptr;
	}

	for (int i = 0; i < TERPSTRABOARDSIZE; i++)
	{
		terpstraKeyFields[i] = nullptr;
	}

	midiEditArea = nullptr;
	noteEditArea = nullptr;
}

void MainContentComponent::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	setSize(
		propertiesFile->getIntValue("MainWindowWidth", DEFAULTMAINWINDOWWIDTH),
		propertiesFile->getIntValue("MainWindowHeight", DEFAULTMAINWINDOWHEIGHT));

	noteEditArea->restoreStateFromPropertiesFile(propertiesFile);
}

void MainContentComponent::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	propertiesFile->setValue("MainWindowWidth", getWidth());
	propertiesFile->setValue("MainWindowHeight", getHeight());

	noteEditArea->saveStateToPropertiesFile(propertiesFile);
}

// Set the current mapping to be edited to the value passed in parameter
void MainContentComponent::setData(TerpstraKeyMapping& newData, bool withRefresh)
{
	mappingData = newData;

	noteEditArea->onSetData(newData);

    if ( withRefresh)
    {
        for (int i = 0; i < NUMBEROFBOARDS; i++)
            terpstraSetSelectors[i]->repaint();

        changeSetSelection(-1);
        changeSetSelection(0);
    }
}

void MainContentComponent::deleteAll(bool withRefresh)
{
	TerpstraKeyMapping keyMapping;
	setData(keyMapping, withRefresh);
}

// Copy the edited mapping to the variable passed as parameter
void MainContentComponent::getData(TerpstraKeyMapping& newData)
{
	newData = mappingData;
}

bool MainContentComponent::deleteCurrentSubBoardData()
{
	if (currentSetSelection >= 0 && currentSetSelection < NUMBEROFBOARDS)
	{
		// Delete subboard data
		mappingData.sets[currentSetSelection] = TerpstraKeys();

		// Refresh display
		changeSetSelection(currentSetSelection, true);

		// Mark that there are changes
		TerpstraSysExApplication::getApp().setHasChangesToSave(true);

		return true;
	}
	else
		return false;
}

bool MainContentComponent::copyCurrentSubBoardData()
{
	if (currentSetSelection >= 0 && currentSetSelection < NUMBEROFBOARDS)
	{
		copiedSubBoardData = mappingData.sets[currentSetSelection];
		return true;
	}
	else
		return false;
}

bool MainContentComponent::pasteCurrentSubBoardData()
{
	if (currentSetSelection >= 0 && currentSetSelection < NUMBEROFBOARDS)
	{
		if (!copiedSubBoardData.isEmpty())
		{
			mappingData.sets[currentSetSelection] = copiedSubBoardData;

			// Refresh display
			changeSetSelection(currentSetSelection, true);

			// Mark that there are changes
			TerpstraSysExApplication::getApp().setHasChangesToSave(true);
		}
		return true;
	}
	else
		return false;
}

void MainContentComponent::midiMessageReceived(const MidiMessage& message)
{
    if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsTerpstraConfigurationDataReceptionMessage(message))
    {
        auto sysExData = message.getSysExData();

        int boardNo = sysExData[3];
        jassert(boardNo >= 1 && boardNo <= NUMBEROFBOARDS);
        auto midiCmd = sysExData[4];
        auto answerState = sysExData[5];

        if (answerState == TerpstraMidiDriver::ACK)
        {
            // After the answer state byte there must be 55 bytes of data (one for each key)
            jassert(message.getSysExDataSize() >= TERPSTRABOARDSIZE + 6); // ToDo display error otherwise

            for (int keyIndex = 0; keyIndex < TERPSTRABOARDSIZE; keyIndex++)
            {
                auto newValue = sysExData[6 + keyIndex];

                TerpstraKey& keyData = this->mappingData.sets[boardNo-1].theKeys[keyIndex];

                switch(midiCmd)
                {
                case GET_RED_LED_CONFIG:
                {
                    auto theColour = Colour(keyData.colour);
                    theColour = Colour(newValue, theColour.getGreen(), theColour.getBlue());
                    keyData.colour = theColour.toDisplayString(false).getHexValue32();
                    break;
                }

                case GET_GREEN_LED_CONFIG:
                {
                    auto theColour = Colour(keyData.colour);
                    theColour = Colour(theColour.getRed(), newValue, theColour.getBlue());
                    keyData.colour = theColour.toDisplayString(false).getHexValue32();
                    break;
                }

                case GET_BLUE_LED_CONFIG:
                {
                    auto theColour = Colour(keyData.colour);
                    theColour = Colour(theColour.getRed(), theColour.getGreen(), newValue);
                    keyData.colour = theColour.toDisplayString(false).getHexValue32();
                    break;
                }

                case GET_CHANNEL_CONFIG:
                    keyData.channelNumber = newValue;
                    break;

                case GET_NOTE_CONFIG:
                    keyData.noteNumber = newValue;
                    break;

                case GET_KEYTYPE_CONFIG:
                    keyData.keyType = (TerpstraKey::KEYTYPE)newValue;
                    break;

                default:
                    jassertfalse;   // Should not happen
                    break;
                }
            }

            terpstraSetSelectors[boardNo-1]->repaint();
        }
    }
}

void MainContentComponent::paint (Graphics& g)
{
	g.fillAll(findColour(ResizableWindow::backgroundColourId));
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	int newWidth = getWidth();
	int newHeight = getHeight();

	// Logo, MIDI edit area and connection state
	int midiAreaWidth = midiEditArea->getWidth();
	int midiAreaHeight = midiEditArea->getHeight();
	int midiAreaXPos = jmax(newWidth - midiAreaWidth, 0);
	midiEditArea->setBounds(midiAreaXPos, 0, midiAreaWidth, midiAreaHeight);

	// All keys overview/virtual keyboard playing
	// New height of subset field area, with minimal value
	float newSubsetAreaHeight = jmax(newHeight - midiAreaHeight - EDITFUNCTIONAREAHEIGHT, MINIMALTERPSTRAKEYSETAREAHEIGHT);

	// Resize factor for the subset field area and the subset fields
	double newResizeFactor = (double)newSubsetAreaHeight * 1.1 / (DEFAULTMAINWINDOWHEIGHT - midiAreaHeight - EDITFUNCTIONAREAHEIGHT);
	jassert(newResizeFactor > 0.0);
	double newDecreaseFactor = jmin(newResizeFactor, 1.0);
	jassert(newDecreaseFactor > 0.0);

	// New position, width and height of subset fields
	float newSubsetFirstYPos = midiAreaHeight + TERPSTRAKEYSETFLDFIRSTYPOS * newDecreaseFactor;
	float newSubsetWidth = DEFAULTTERPSTRAKEYSETWIDTH * newDecreaseFactor;
	float newSubsetHeight = DEFAULTTERPSTRAKEYSETHEIGHT * newDecreaseFactor;
	float newSubsetXIncrement = DEFAULTTERPSTRAKEYSETXINCREMENT * newDecreaseFactor;

	float newSingleKeyFieldFirstYPos = midiAreaHeight + newSubsetAreaHeight + TERPSTRASINGLEKEYFIELDRIMABOVE * newDecreaseFactor;

	// Key set fields
	for (int i = 0; i < NUMBEROFBOARDS; i++)
	{
		// Paint set fields from right to left
		// (This will not matter any more when the images' backgrounds are transparent)
		terpstraSetSelectors[4 - i]->setBounds(
            roundToInt(MAINWINDOWFIRSTCOLPOS + (4 - i)*newSubsetXIncrement),
            roundToInt(newSubsetFirstYPos), newSubsetWidth, newSubsetHeight);
	}

	// Single Key fields

	// Transformation Rotate slightly counterclockwise
	float x = MAINWINDOWFIRSTCOLPOS;
	float y = newSingleKeyFieldFirstYPos;
	AffineTransform transform = AffineTransform::translation(-x, -y);
	transform = transform.rotated(TERPSTRASINGLEKEYROTATIONANGLE);
	transform = transform.translated(x, y);

	int keyIndex = 0;
    int mostBottomKeyPos = 0;

	// Rows
	int rowCount = boardGeometry.horizontalLineCount();
	for (int rowIndex = 0; rowIndex < rowCount; rowIndex++)
	{
		float xbasepos;
		if (rowIndex % 2 == 0)
			xbasepos = MAINWINDOWFIRSTCOLPOS;
		else
			xbasepos = MAINWINDOWFIRSTCOLPOS + TERPSTRASINGLEKEYFLDSIZE / 2;

		int ybasepos = newSingleKeyFieldFirstYPos + 3 * rowIndex * TERPSTRASINGLEKEYFLDSIZE / 4;

		int subBoardRowSize = boardGeometry.horizontalLineSize(rowIndex);
		for (int posInRow = 0; posInRow < subBoardRowSize; posInRow++)
		{
			x = xbasepos + (boardGeometry.firstColumnOffset(rowIndex) + posInRow)*TERPSTRASINGLEKEYFLDSIZE;
			y = ybasepos;
			transform.transformPoint(x, y);
			terpstraKeyFields[keyIndex]->setBounds(roundToInt(x), roundToInt(y), TERPSTRASINGLEKEYFLDSIZE, TERPSTRASINGLEKEYFLDSIZE);

			mostBottomKeyPos = jmax(mostBottomKeyPos, terpstraKeyFields[keyIndex]->getBottom());
			keyIndex++;
		}
	}

	jassert(TERPSTRABOARDSIZE == keyIndex);

	// Edit function area
	noteEditArea->setBounds(EDITAREAFIRSTCOLPOS, midiAreaHeight + newSubsetAreaHeight, EDITAREAWIDTH, EDITFUNCTIONAREAHEIGHT);
}

void MainContentComponent::mouseDown(const MouseEvent &event)
{
	bool mappingChanged = false;

	// Selection of subset components
	auto eventComponentParent = event.eventComponent->getParentComponent();
	for (int i = 0; i < NUMBEROFBOARDS; i++)
	{
		if (event.eventComponent == terpstraSetSelectors[i].get() || eventComponentParent == terpstraSetSelectors[i].get())
		{
			changeSetSelection(i);
			return;
		}
	}

	// Selection of single key fields
	for (int i = 0; i < TERPSTRABOARDSIZE; i++)
	{
		if (event.eventComponent == terpstraKeyFields[i].get() || event.eventComponent->getParentComponent() == terpstraKeyFields[i].get())
		{
			// Select field
			changeSingleKeySelection(i);

			// Perform the edit, according to edit mode. Including sending to device
			mappingChanged = this->noteEditArea->performMouseDown(currentSetSelection, i);

			// Mark that there are changes
			if (mappingChanged)
				TerpstraSysExApplication::getApp().setHasChangesToSave(true);

			break;
		}
	}

    // Refresh display (edit may affect all octave boards)
    if (mappingChanged)
    {
        for (int i = 0; i < TERPSTRABOARDSIZE; i++)
        {
            terpstraSetSelectors[i]->repaint();
        }

        changeSetSelection(currentSetSelection, true);
    }
}

void MainContentComponent::mouseUp(const MouseEvent &event)
{
	bool mappingChanged = false;

	// Selection of single key fields
	for (int i = 0; i < TERPSTRABOARDSIZE; i++)
	{
		if (event.eventComponent == terpstraKeyFields[i].get() || event.eventComponent->getParentComponent() == terpstraKeyFields[i].get())
		{
			// Perform the edit, according to edit mode. Including sending to device
			mappingChanged = this->noteEditArea->performMouseUp(currentSetSelection, i);

			// Mark that there are changes
			if (mappingChanged)
				TerpstraSysExApplication::getApp().setHasChangesToSave(true);

			break;
		}
	}

    // Refresh display (edit may affect all octave boards)
    if ( mappingChanged)
    {
        for (int i = 0; i < TERPSTRABOARDSIZE; i++)
        {
            terpstraSetSelectors[i]->repaint();
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
