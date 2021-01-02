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
	: copiedSubBoardData()
{ 
	setName("MainContentComponent");

	// Midi input + output
	midiEditArea.reset(new MidiEditArea());
	addAndMakeVisible(midiEditArea.get());

	// All keys overview
	allKeysOverview.reset(new AllKeysOverview());
	addAndMakeVisible(allKeysOverview.get());

	// Edit function area
	noteEditArea.reset(new NoteEditArea());
	addAndMakeVisible(noteEditArea.get());
	noteEditArea->getOctaveBoardSelectorTab()->addChangeListener(this);

	generalOptionsArea.reset(new GeneralOptionsDlg());
	addAndMakeVisible(generalOptionsArea.get());

	curvesArea.reset(new CurvesArea());
	addAndMakeVisible(curvesArea.get());

	globalSettingsArea.reset(new GlobalSettingsArea());
	addAndMakeVisible(globalSettingsArea.get());

	TerpstraSysExApplication::getApp().getMidiDriver().addListener(this);

	// Initial size
	setSize(DEFAULTMAINWINDOWWIDTH, DEFAULTMAINWINDOWHEIGHT);

	// Select first board and first key
	noteEditArea->getOctaveBoardSelectorTab()->setCurrentTabIndex(0, true);
	// The above call is supposed to update changeListener - but apaprently doesn't... Call it manually then. XXX
	changeListenerCallback(noteEditArea->getOctaveBoardSelectorTab());

	noteEditArea->changeSingleKeySelection(0);
}

MainContentComponent::~MainContentComponent()
{
    TerpstraSysExApplication::getApp().getMidiDriver().removeListener(this);

	midiEditArea = nullptr;
	allKeysOverview = nullptr;
	noteEditArea = nullptr;

	generalOptionsArea = nullptr;
	curvesArea = nullptr;
	globalSettingsArea = nullptr;
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
	globalSettingsArea->saveStateToPropertiesFile(propertiesFile);
}

// Set the current mapping to be edited to the value passed in parameter
void MainContentComponent::setData(TerpstraKeyMapping& newData, bool withRefresh)
{
	mappingData = newData;

	noteEditArea->onSetData(newData);

	if (withRefresh)
	{
		refreshAllKeysOverview();
		noteEditArea->refreshKeyFields();
		generalOptionsArea->loadFromMapping();
		curvesArea->loadFromMapping();
		curvesArea->repaint();
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
	//if (currentSetSelection >= 0 && currentSetSelection < NUMBEROFBOARDS)
	//{
	//	// Delete subboard data
	//	mappingData.sets[currentSetSelection] = TerpstraKeys();

	//	// Refresh display
	//	changeSetSelection(currentSetSelection, true);

	//	// Mark that there are changes
	//	TerpstraSysExApplication::getApp().setHasChangesToSave(true);

	//	return true;
	//}
	//else
	//todo
		return false;
}

bool MainContentComponent::copyCurrentSubBoardData()
{
	//if (currentSetSelection >= 0 && currentSetSelection < NUMBEROFBOARDS)
	//{
	//	copiedSubBoardData = mappingData.sets[currentSetSelection];
	//	return true;
	//}
	//else
	//ToDo
		return false;
}

bool MainContentComponent::pasteCurrentSubBoardData()
{
	//if (currentSetSelection >= 0 && currentSetSelection < NUMBEROFBOARDS)
	//{
	//	if (!copiedSubBoardData.isEmpty())
	//	{
	//		mappingData.sets[currentSetSelection] = copiedSubBoardData;

	//		// Refresh display
	//		changeSetSelection(currentSetSelection, true);

	//		// Mark that there are changes
	//		TerpstraSysExApplication::getApp().setHasChangesToSave(true);
	//	}
	//	return true;
	//}
	//else
	// ToDO
		return false;
}

void MainContentComponent::midiMessageReceived(const MidiMessage& midiMessage)
{
    if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsTerpstraConfigurationDataReceptionMessage(midiMessage))
    {
        auto sysExData = midiMessage.getSysExData();

        int boardNo = sysExData[3];
        jassert(boardNo >= 1 && boardNo <= NUMBEROFBOARDS);
        auto midiCmd = sysExData[4];
        auto answerState = sysExData[5];

        if (answerState == TerpstraMidiDriver::ACK)
        {
			// ToDo General options

			// Velocity curves
			if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsVelocityIntervalConfigReceptionMessage(midiMessage))
			{
				// After the answer state byte there must be 254 bytes of data
				jassert(midiMessage.getSysExDataSize() >= (6 + 2 * VELOCITYINTERVALTABLESIZE)); // ToDo display error otherwise

				for (int i = 0; i < VELOCITYINTERVALTABLESIZE; i++)
					this->mappingData.velocityIntervalTableValues[i] = (sysExData[6 + 2 * i] << 6) + sysExData[7 + 2 * i];

				curvesArea->resized();
				curvesArea->repaint();
			}
			else if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsTerpstraVelocityConfigReceptionMessage(midiMessage, TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff))
			{
				// After the answer state byte there must be 128 bytes of data
				jassert(midiMessage.getSysExDataSize() >= 134); // ToDo display error otherwise
				this->mappingData.noteOnOffVelocityCurveConfig.editStrategy = TerpstraVelocityCurveConfig::EDITSTRATEGYINDEX::freeDrawing;
				for (int x = 0; x < 128; x++)
					this->mappingData.noteOnOffVelocityCurveConfig.velocityValues[x] = sysExData[6 + x];
				curvesArea->loadFromMapping();
			}
			else if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsTerpstraVelocityConfigReceptionMessage(midiMessage, TerpstraMidiDriver::VelocityCurveType::fader))
			{
				// After the answer state byte there must be 128 bytes of data
				jassert(midiMessage.getSysExDataSize() >= 134); // ToDo display error otherwise
				this->mappingData.faderConfig.editStrategy = TerpstraVelocityCurveConfig::EDITSTRATEGYINDEX::freeDrawing;
				for (int x = 0; x < 128; x++)
					this->mappingData.faderConfig.velocityValues[x] = sysExData[6 + x];
				curvesArea->loadFromMapping();
			}
			else if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsTerpstraVelocityConfigReceptionMessage(midiMessage, TerpstraMidiDriver::VelocityCurveType::afterTouch))
			{
				// After the answer state byte there must be 128 bytes of data
				jassert(midiMessage.getSysExDataSize() >= 134); // ToDo display error otherwise
				this->mappingData.afterTouchConfig.editStrategy = TerpstraVelocityCurveConfig::EDITSTRATEGYINDEX::freeDrawing;
				for (int x = 0; x < 128; x++)
					this->mappingData.afterTouchConfig.velocityValues[x] = sysExData[6 + x];
				curvesArea->loadFromMapping();
			}

			// Key configurations
			else if (midiCmd == GET_RED_LED_CONFIG || midiCmd == GET_GREEN_LED_CONFIG || midiCmd == GET_BLUE_LED_CONFIG ||
				midiCmd == GET_CHANNEL_CONFIG || midiCmd == GET_NOTE_CONFIG || midiCmd == GET_KEYTYPE_CONFIG)
			{
				// After the answer state byte there must be 55 bytes of data (one for each key)
				jassert(midiMessage.getSysExDataSize() >= TERPSTRABOARDSIZE + 6); // ToDo display error otherwise

				for (int keyIndex = 0; keyIndex < TERPSTRABOARDSIZE; keyIndex++)
				{
					auto newValue = sysExData[6 + keyIndex];

					TerpstraKey& keyData = this->mappingData.sets[boardNo - 1].theKeys[keyIndex];

					switch (midiCmd)
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

				refreshAllKeysOverview();
			}
		}
    }
}

void MainContentComponent::changeListenerCallback(ChangeBroadcaster *source)
{
	if (source == noteEditArea->getOctaveBoardSelectorTab())
	{
		allKeysOverview->setCurrentSetSelection(noteEditArea->getOctaveBoardSelectorTab()->getCurrentTabIndex());
	}
}

void MainContentComponent::paint (Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(LumatoneEditorColourIDs::MediumBackground));

	g.setColour(getLookAndFeel().findColour(LumatoneEditorColourIDs::LightBackground));
	g.fillRect(controlsArea);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	int newWidth = getWidth();
	int newHeight = getHeight();

	// Logo, MIDI edit area and connection state
	int midiAreaHeight = proportionOfHeight(headerHeight);
	midiEditArea->setBounds(0, 0, getWidth(), midiAreaHeight);

	// Bounds for controls, where background is darker
	int footerY = proportionOfHeight(footerAreaY);
	int footerHeight = getHeight() - footerY;
	controlsArea = getBounds().withTop(proportionOfHeight(controlsAreaY)).withBottom(footerY);

	// All keys overview/virtual keyboard playing
	// New height of subset field area, with minimal value
	int noteEditAreaWidth = noteEditArea->getWidth();
	int noteEditAreaHeight = noteEditArea->getHeight();

	int newKeysOverviewAreaHeight = jmax(newHeight - midiAreaHeight - controlsArea.getY(), MINIMALTERPSTRAKEYSETAREAHEIGHT);

	allKeysOverview->setBounds(0, midiAreaHeight, newWidth, newKeysOverviewAreaHeight);


	// Edit function/single key field area
	noteEditArea->setSize(proportionOfWidth(assignWidth), proportionOfHeight(assignHeight));
	noteEditArea->setControlsTopLeftPosition(proportionOfWidth(assignMarginX), controlsArea.getY());
	
	generalOptionsArea->setBounds(getBounds().toFloat().getProportion(generalSettingsBounds).toNearestInt());
	curvesArea->setBounds(getBounds().toFloat().getProportion(curvesAreaBounds).toNearestInt());

	globalSettingsArea->setBounds(getLocalBounds().withLeft(proportionOfWidth(footerControlsX)).withTop(proportionOfHeight(footerAreaY)));
}

void MainContentComponent::refreshAllKeysOverview()
{
	allKeysOverview->repaint();
}