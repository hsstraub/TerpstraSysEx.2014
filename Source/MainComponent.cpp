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
#include "EditActions.h"


//==============================================================================
MainContentComponent::MainContentComponent()
	: copiedSubBoardData()
{
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

	pedalSensitivityDlg.reset(new PedalSensitivityDlg());
	addAndMakeVisible(pedalSensitivityDlg.get());

	curvesArea.reset(new CurvesArea());
	addAndMakeVisible(curvesArea.get());

	globalSettingsArea.reset(new GlobalSettingsArea());
	addAndMakeVisible(globalSettingsArea.get());

	// Initial size
	setSize(DEFAULTMAINWINDOWWIDTH, DEFAULTMAINWINDOWHEIGHT);

	// Select first board and first key
	noteEditArea->getOctaveBoardSelectorTab()->setCurrentTabIndex(0, true);
	// The above call is supposed to update changeListener - but apaprently doesn't... Call it manually then. XXX
	changeListenerCallback(noteEditArea->getOctaveBoardSelectorTab());

	noteEditArea->changeSingleKeySelection(0);

    // Initialize mapping structure
    deleteAll();
}

MainContentComponent::~MainContentComponent()
{
	midiEditArea = nullptr;
	allKeysOverview = nullptr;
	noteEditArea = nullptr;

	generalOptionsArea = nullptr;
	curvesArea = nullptr;
	globalSettingsArea = nullptr;
	pedalSensitivityDlg = nullptr;
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
		refreshAllFields();
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

UndoableAction* MainContentComponent::createDeleteCurrentSectionAction()
{
	auto currentSetSelection = noteEditArea->getOctaveBoardSelectorTab()->getCurrentTabIndex();
	if (currentSetSelection >= 0 && currentSetSelection < TerpstraSysExApplication::getApp().getOctaveBoardSize())
		{
		// Delete subboard data
        auto keySet = TerpstraKeys();
        return new Lumatone::SectionEditAction(currentSetSelection, keySet);
	}
	else
		return nullptr;
}

bool MainContentComponent::copyCurrentSubBoardData()
{
	auto currentSetSelection = noteEditArea->getOctaveBoardSelectorTab()->getCurrentTabIndex();
	if (currentSetSelection >= 0 && currentSetSelection < TerpstraSysExApplication::getApp().getOctaveBoardSize())
	{
		copiedSubBoardData = mappingData.sets[currentSetSelection];
		return true;
	}
	else
		return false;
}

UndoableAction* MainContentComponent::createPasteCurrentSectionAction()
{
	auto currentSetSelection = noteEditArea->getOctaveBoardSelectorTab()->getCurrentTabIndex();
	if (currentSetSelection >= 0 && currentSetSelection < NUMBEROFBOARDS
		&& !copiedSubBoardData.isEmpty())
	{
		return new Lumatone::SectionEditAction(currentSetSelection, copiedSubBoardData);
	}
	else
		return nullptr;
}

bool MainContentComponent::canPasteCopiedSubBoard() const
{
    return !copiedSubBoardData.isEmpty();
}

void MainContentComponent::octaveColourConfigReceived(int octaveIndex, uint8 rgbFlag, const int* colourData)
{
    /*
	for (int keyIndex = 0; keyIndex < TerpstraSysExApplication::getApp().getOctaveBoardSize(); keyIndex++)
	{
		TerpstraKey& keyData = this->mappingData.sets[octaveIndex - 1].theKeys[keyIndex];
		auto newValue = colourData[keyIndex];

		if (rgbFlag == 0)
		{
-		    auto theColour = juce::Colour(keyData.colour);
-			theColour = juce::Colour(newValue, theColour.getGreen(), theColour.getBlue());
-			keyData.colour = theColour.toDisplayString(false).getHexValue32();
		}
		else if (rgbFlag == 1)
		{
-			auto theColour = juce::Colour(keyData.colour);
-			theColour = juce::Colour(theColour.getRed(), theColour.getGreen(), newValue);
-			keyData.colour = theColour.toDisplayString(false).getHexValue32();
		}
		else if (rgbFlag == 2)
		{
-			auto theColour = juce::Colour(keyData.colour);
-			theColour = juce::Colour(theColour.getRed(), newValue, theColour.getBlue());
-			keyData.colour = theColour.toDisplayString(false).getHexValue32();
		}
		else
			jassertfalse;
	}
	*/

	refreshKeyDataFields();
}

void MainContentComponent::octaveChannelConfigReceived(int octaveIndex, const int* channelData)
{
	for (int keyIndex = 0; keyIndex < TerpstraSysExApplication::getApp().getOctaveBoardSize(); keyIndex++)
	{
		// Check channel values?
		this->mappingData.sets[octaveIndex - 1].theKeys[keyIndex].channelNumber = channelData[keyIndex];
	}

	refreshKeyDataFields();
}

void MainContentComponent::octaveNoteConfigReceived(int octaveIndex, const int* noteData)
{
	for (int keyIndex = 0; keyIndex < TerpstraSysExApplication::getApp().getOctaveBoardSize(); keyIndex++)
	{
		// Check note values?
		this->mappingData.sets[octaveIndex - 1].theKeys[keyIndex].noteNumber = noteData[keyIndex];
	}

	refreshKeyDataFields();
}

void MainContentComponent::keyTypeConfigReceived(int octaveIndex, const int* keyTypeData)
{
	for (int keyIndex = 0; keyIndex < TerpstraSysExApplication::getApp().getOctaveBoardSize(); keyIndex++)
	{
		// Check type values?
		this->mappingData.sets[octaveIndex - 1].theKeys[keyIndex].keyType = LumatoneKeyType(keyTypeData[keyIndex]);
	}

	refreshKeyDataFields();
}

void MainContentComponent::velocityConfigReceived(const int* velocityData)
{
	this->mappingData.noteOnOffVelocityCurveConfig.editStrategy = TerpstraVelocityCurveConfig::EDITSTRATEGYINDEX::freeDrawing;
	for (int i = 0; i < 128; i++)
		this->mappingData.noteOnOffVelocityCurveConfig.velocityValues[i] = velocityData[127 - i]; // Reversed
	curvesArea->loadFromMapping();
}

void MainContentComponent::aftertouchConfigReceived(const int* aftertouch)
{
	this->mappingData.afterTouchConfig.editStrategy = TerpstraVelocityCurveConfig::EDITSTRATEGYINDEX::freeDrawing;
	memmove(this->mappingData.afterTouchConfig.velocityValues, aftertouch, sizeof(int) * 128);
	curvesArea->loadFromMapping();
}

void MainContentComponent::velocityIntervalConfigReceived(const int* velocityData)
{
	memmove(this->mappingData.velocityIntervalTableValues, velocityData, sizeof(int) * VELOCITYINTERVALTABLESIZE);
	curvesArea->loadFromMapping();
}

void MainContentComponent::faderConfigReceived(const int* faderData)
{
	this->mappingData.faderConfig.editStrategy = TerpstraVelocityCurveConfig::EDITSTRATEGYINDEX::freeDrawing;
	memmove(this->mappingData.faderConfig.velocityValues, faderData, sizeof(int) * 128);
	curvesArea->loadFromMapping();
}

void MainContentComponent::faderTypeConfigReceived(int octaveIndex, const int* faderTypeData)
{
	for (int keyIndex = 0; keyIndex < TerpstraSysExApplication::getApp().getOctaveBoardSize(); keyIndex++)
	{
		this->mappingData.sets[octaveIndex - 1].theKeys[keyIndex].ccFaderDefault = faderTypeData[keyIndex];
	}
}

void MainContentComponent::lumatouchConfigReceived(const int* lumatouchData)
{
	this->mappingData.lumaTouchConfig.editStrategy = TerpstraVelocityCurveConfig::EDITSTRATEGYINDEX::freeDrawing;
	memmove(this->mappingData.lumaTouchConfig.velocityValues, lumatouchData, sizeof(int) * 128);
	curvesArea->loadFromMapping();
}

void MainContentComponent::firmwareRevisionReceived(FirmwareVersion version)
{
	// Make sure changes happen in proper order
	noteEditArea->resetOctaveSize();
	allKeysOverview->resetOctaveSize();
}

void MainContentComponent::changeListenerCallback(ChangeBroadcaster *source)
{
	if (source == noteEditArea->getOctaveBoardSelectorTab())
	{
		allKeysOverview->setCurrentSetSelection(noteEditArea->getOctaveBoardSelectorTab()->getCurrentTabIndex());
	}
}

void MainContentComponent::buttonClicked(Button* btn)
{
    // Todo colour palette
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

	int settingsAreaWidth = globalSettingsArea->getWidth();

	int newNoteEditAreaWidth = jmax(newWidth - settingsAreaWidth, MINIMALNOTEEDITAREAWIDTH);
	int noteEditAreaHeight = noteEditArea->getHeight();

	// All keys overview/virtual keyboard playing
	// New height of subset field area, with minimal value
	int newKeysOverviewAreaHeight = jmax(newHeight - midiAreaHeight - noteEditAreaHeight, MINIMALTERPSTRAKEYSETAREAHEIGHT);

	allKeysOverview->setBounds(0, midiAreaHeight, newWidth, newKeysOverviewAreaHeight);

	// Edit function/single key field area
	noteEditArea->setBounds(0, midiAreaHeight + newKeysOverviewAreaHeight, newNoteEditAreaWidth, noteEditAreaHeight);

	int generalOptionsYPos = allKeysOverview->getBottom() + OCTAVEBOARDTABHEIGHT;
	generalOptionsArea->setBounds(newNoteEditAreaWidth, generalOptionsYPos, generalOptionsArea->getWidth(), generalOptionsArea->getHeight());
	pedalSensitivityDlg->setBounds(newNoteEditAreaWidth + generalOptionsArea->getWidth(), generalOptionsYPos, pedalSensitivityDlg->getWidth(), pedalSensitivityDlg->getHeight());

	int curvesAreaYPos = generalOptionsArea->getBottom();
	int curvesAreaHeight = jmax(newHeight - curvesAreaYPos - globalSettingsArea->getHeight(), MINIMALCURVESAREAHEIGHT);
	curvesArea->setBounds(newNoteEditAreaWidth, curvesAreaYPos, settingsAreaWidth, curvesAreaHeight);

	globalSettingsArea->setBounds(newNoteEditAreaWidth, curvesArea->getBottom(), settingsAreaWidth, globalSettingsArea->getHeight());
}

void MainContentComponent::refreshKeyDataFields()
{
	allKeysOverview->repaint();
	noteEditArea->refreshKeyFields();
}

void MainContentComponent::refreshAllFields()
{
	refreshKeyDataFields();
	generalOptionsArea->loadFromMapping();
	pedalSensitivityDlg->loadFromMapping();
	curvesArea->loadFromMapping();
	curvesArea->repaint();
}

