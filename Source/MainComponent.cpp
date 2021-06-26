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
	setName("MainContentComponent");

	// Midi input + output
	midiEditArea.reset(new MidiEditArea(TerpstraSysExApplication::getApp().getLookAndFeel()));
	addAndMakeVisible(midiEditArea.get());

	// All keys overview
	allKeysOverview.reset(new AllKeysOverview());
	addAndMakeVisible(allKeysOverview.get());

	// Edit function area
	noteEditArea.reset(new NoteEditArea());
	addAndMakeVisible(noteEditArea.get());
	noteEditArea->getOctaveBoardSelectorTab()->addChangeListener(this);
	noteEditArea->getColourEditComponent()->addListener(this); // Open up ColourPaletteWindow

	generalOptionsArea.reset(new GeneralOptionsDlg());
	addAndMakeVisible(generalOptionsArea.get());

	pedalSensitivityDlg.reset(new PedalSensitivityDlg());
	addAndMakeVisible(pedalSensitivityDlg.get());

	curvesArea.reset(new CurvesArea());
	addAndMakeVisible(curvesArea.get());

	globalSettingsArea.reset(new GlobalSettingsArea());
	addAndMakeVisible(globalSettingsArea.get());
	globalSettingsArea->listenToColourEditButtons(this);

	TerpstraSysExApplication::getApp().getLumatoneController().addFirmwareListener(this);

	//lblAppName.reset(new Label("lblAppName", TerpstraSysExApplication::getApp().getApplicationName()));
	lblAppName.reset(new Label("lblAppName", "lumatone editor"));
	lblAppName->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::FranklinGothic));
	lblAppName->setColour(Label::ColourIds::textColourId, Colour(0xff777777));
	addAndMakeVisible(lblAppName.get());

	lblAppVersion.reset(new Label("lblAppVersion", "v" + TerpstraSysExApplication::getApp().getApplicationVersion()));
	lblAppVersion->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::FranklinGothic));
	lblAppVersion->setColour(Label::ColourIds::textColourId, Colour(0xff777777));
	addAndMakeVisible(lblAppVersion.get());

	// Initial size
	setSize(DEFAULTMAINWINDOWWIDTH, DEFAULTMAINWINDOWHEIGHT);

	// Select first board and first key
	noteEditArea->getOctaveBoardSelectorTab()->setCurrentTabIndex(0, true);
	// The above call is supposed to update changeListener - but apparently doesn't... Call it manually then. XXX
	changeListenerCallback(noteEditArea->getOctaveBoardSelectorTab());
    noteEditArea->changeSingleKeySelection(0);

    // Initialize mapping structure
    deleteAll();
}

MainContentComponent::~MainContentComponent()
{
    //TerpstraSysExApplication::getApp().getMidiDriver().removeListener(this);

	midiEditArea = nullptr;
	allKeysOverview = nullptr;
	noteEditArea = nullptr;

	generalOptionsArea = nullptr;
	curvesArea = nullptr;
	globalSettingsArea = nullptr;
	pedalSensitivityDlg = nullptr;

	lblAppName = nullptr;
	lblAppVersion = nullptr;
}

void MainContentComponent::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	noteEditArea->restoreStateFromPropertiesFile(propertiesFile);
}

void MainContentComponent::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
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
	if (currentSetSelection >= 0 && currentSetSelection < TerpstraSysExApplication::getApp().getOctaveBoardSize()
		&& !copiedSubBoardData.isEmpty())
	{
		return new Lumatone::SectionEditAction(currentSetSelection, copiedSubBoardData);
	}
	else
		return nullptr;
}

bool MainContentComponent::setDeveloperMode(bool developerModeOn)
{
	curvesArea->setDeveloperMode(developerModeOn);
    globalSettingsArea->setDeveloperMode(developerModeOn);
    allKeysOverview->showDeveloperMode(developerModeOn);
	return true;
}

void MainContentComponent::octaveColourConfigReceived(int octaveIndex, uint8 rgbFlag, const int* colourData)
{
	for (int keyIndex = 0; keyIndex < TerpstraSysExApplication::getApp().getOctaveBoardSize(); keyIndex++)
	{
		TerpstraKey& keyData = this->mappingData.sets[octaveIndex - 1].theKeys[keyIndex];
		auto newValue = colourData[keyIndex];

		if (rgbFlag == 0)
		{
			keyData.colour = Colour(newValue, keyData.colour.getGreen(), keyData.colour.getBlue());
		}
		else if (rgbFlag == 1)
		{
			keyData.colour = Colour(keyData.colour.getRed(), newValue, keyData.colour.getBlue());
		}
		else if (rgbFlag == 2)
		{
			keyData.colour = Colour(keyData.colour.getRed(), keyData.colour.getGreen(), newValue);
		}
		else
			jassertfalse;
	}

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
	ColourEditComponent* colourEdit = dynamic_cast<ColourEditComponent*>(btn);

	if (colourEdit)
	{
		// May be better asynchronous on a timer
		TerpstraSysExApplication::getApp().reloadColourPalettes();

		ColourPaletteWindow* paletteWindow = new ColourPaletteWindow(TerpstraSysExApplication::getApp().getColourPalettes());
		paletteWindow->setSize(proportionOfWidth(popupWidth), proportionOfHeight(popupHeight));

        if (btn == noteEditArea->getColourEditComponent())
        {
            colourEdit = noteEditArea->getColourEditComponent();
            paletteWindow->listenToColourSelection(noteEditArea->getSingleNoteColourTextEditor());
        }

		Rectangle<int> componentArea = colourEdit->getScreenBounds().translated(-getScreenX(), -getScreenY());

		CallOutBox& popupBox = CallOutBox::launchAsynchronously(
			std::unique_ptr<Component>(paletteWindow),
			componentArea,
			this
		);

		// else, a preset button colour button was pressed
		paletteWindow->listenToColourSelection(colourEdit);
		popupBox.setLookAndFeel(&getLookAndFeel());
		// TODO: Set swatch # or custom colour as current colour
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
	int newKeysOverviewAreaHeight = jmax(controlsArea.getY() - midiAreaHeight, MINIMALTERPSTRAKEYSETAREAHEIGHT);
	allKeysOverview->setBounds(0, midiAreaHeight, newWidth, newKeysOverviewAreaHeight);

	// Edit function/single key field area
	noteEditArea->setSize(proportionOfWidth(assignWidth), proportionOfHeight(assignHeight));
	noteEditArea->setControlsTopLeftPosition(proportionOfWidth(assignMarginX), controlsArea.getY());
	
	generalOptionsArea->setBounds(getLocalBounds().toFloat().getProportion(generalSettingsBounds).toNearestInt());
	pedalSensitivityDlg->setBounds(getLocalBounds().toFloat().getProportion(pedalSettingsBounds).toNearestInt());

	curvesArea->setBounds(getLocalBounds().toFloat().getProportion(curvesAreaBounds).toNearestInt());

	globalSettingsArea->setBounds(getLocalBounds()
		.withTop(roundToInt(getHeight() * footerAreaY))
		.withTrimmedRight(footerHeight)
	);

	resizeLabelWithHeight(lblAppName.get(), roundToInt(footerHeight * lumatoneVersionHeight), 1.0f, " ");
	lblAppName->setTopLeftPosition(proportionOfWidth(lumatoneVersionMarginX), footerY + (footerHeight - lblAppName->getHeight()) * 0.5f);

	resizeLabelWithHeight(lblAppVersion.get(), roundToInt(lblAppName->getHeight() * 0.75f));
	lblAppVersion->setTopLeftPosition(lblAppName->getRight(), lblAppName->getBottom() - lblAppVersion->getHeight());
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

