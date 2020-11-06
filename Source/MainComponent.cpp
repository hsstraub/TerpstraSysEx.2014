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
	: currentSetSelection(-1), copiedSubBoardData()
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
}

MainContentComponent::~MainContentComponent()
{
    TerpstraSysExApplication::getApp().getMidiDriver().removeListener(this);

	for (int i = 0; i < NUMBEROFBOARDS; i++)
	{
		terpstraSetSelectors[i] = nullptr;
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
	int noteEditAreaWidth = noteEditArea->getWidth();
	int noteEditAreaHeight = noteEditArea->getHeight();

	float newSubsetAreaHeight = jmax(newHeight - midiAreaHeight - noteEditAreaHeight, MINIMALTERPSTRAKEYSETAREAHEIGHT);

	// Resize factor for the subset field area and the subset fields
	double newResizeFactor = (double)newSubsetAreaHeight * 1.1 / (DEFAULTMAINWINDOWHEIGHT - midiAreaHeight - noteEditAreaHeight);
	jassert(newResizeFactor > 0.0);
	double newDecreaseFactor = jmin(newResizeFactor, 1.0);
	jassert(newDecreaseFactor > 0.0);

	// New position, width and height of subset fields
	float newSubsetFirstYPos = midiAreaHeight + TERPSTRAKEYSETFLDFIRSTYPOS * newDecreaseFactor;
	float newSubsetWidth = DEFAULTTERPSTRAKEYSETWIDTH * newDecreaseFactor;
	float newSubsetHeight = DEFAULTTERPSTRAKEYSETHEIGHT * newDecreaseFactor;
	float newSubsetXIncrement = DEFAULTTERPSTRAKEYSETXINCREMENT * newDecreaseFactor;

	// Key set fields
	for (int i = 0; i < NUMBEROFBOARDS; i++)
	{
		// Paint set fields from right to left
		// (This will not matter any more when the images' backgrounds are transparent)
		terpstraSetSelectors[4 - i]->setBounds(
            roundToInt(MAINWINDOWFIRSTCOLPOS + (4 - i)*newSubsetXIncrement),
            roundToInt(newSubsetFirstYPos), newSubsetWidth, newSubsetHeight);
	}

	// Edit function/single key field area
	noteEditArea->setBounds(0, midiAreaHeight + newSubsetAreaHeight, noteEditAreaWidth, noteEditAreaHeight);
}

void MainContentComponent::mouseDown(const MouseEvent &event)
{
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
			noteEditArea->setKeyFieldValues(mappingData.sets[newSelection]);

		currentSetSelection = newSelection;
	}

	// Set toggle state in any case (override default imagebutton functionality)
	if ( currentSetSelection >= 0 && currentSetSelection < NUMBEROFBOARDS )
		terpstraSetSelectors[currentSetSelection]->setIsSelected(true);
}
