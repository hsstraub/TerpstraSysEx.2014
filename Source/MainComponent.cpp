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
	midiEditArea.reset(new MidiEditArea(TerpstraSysExApplication::getApp().getLookAndFeel(), TerpstraSysExApplication::getApp().getDeviceMonitor()));
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

	TerpstraSysExApplication::getApp().getMidiDriver().addListener(this);

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
    TerpstraSysExApplication::getApp().getMidiDriver().removeListener(this);

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
		refreshKeyDataFields();
		generalOptionsArea->loadFromMapping();
		pedalSensitivityDlg->loadFromMapping();
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
	auto currentSetSelection = noteEditArea->getOctaveBoardSelectorTab()->getCurrentTabIndex();
	if (currentSetSelection >= 0 && currentSetSelection < TerpstraSysExApplication::getApp().getOctaveBoardSize())
		{
		// Delete subboard data
		mappingData.sets[currentSetSelection] = TerpstraKeys();

		// Refresh display
		refreshKeyDataFields();

		// Mark that there are changes
		TerpstraSysExApplication::getApp().setHasChangesToSave(true);

		return true;
	}
	else
		return false;
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

bool MainContentComponent::pasteCurrentSubBoardData()
{
	auto currentSetSelection = noteEditArea->getOctaveBoardSelectorTab()->getCurrentTabIndex();
	if (currentSetSelection >= 0 && currentSetSelection < TerpstraSysExApplication::getApp().getOctaveBoardSize())
		{
		if (!copiedSubBoardData.isEmpty())
		{
			mappingData.sets[currentSetSelection] = copiedSubBoardData;

			// Refresh display
			refreshKeyDataFields();

			// Mark that there are changes
			TerpstraSysExApplication::getApp().setHasChangesToSave(true);
		}
		return true;
	}
	else
		return false;
}

bool MainContentComponent::setDeveloperMode(bool developerModeOn)
{
	curvesArea->setDeveloperMode(developerModeOn);
    globalSettingsArea->setDeveloperMode(developerModeOn);
    allKeysOverview->showDeveloperMode(developerModeOn);
	return true;
}

void MainContentComponent::midiMessageReceived(const MidiMessage& midiMessage)
{
    if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsTerpstraConfigurationDataReceptionMessage(midiMessage)) {
        auto sysExData = midiMessage.getSysExData();

        int boardNo = sysExData[3];
        jassert(boardNo >= 1 && boardNo <= NUMBEROFBOARDS);
        auto midiCmd = sysExData[4];
        auto answerState = sysExData[5];

        if (answerState == TerpstraMidiDriver::ACK) {
            // Velocity curves
            if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsVelocityIntervalConfigReceptionMessage(midiMessage)) {
                // After the answer state byte there must be 254 bytes of data
                jassert(midiMessage.getSysExDataSize() >= (6 + 2 * VELOCITYINTERVALTABLESIZE)); // ToDo display error otherwise

                for (int i = 0; i < VELOCITYINTERVALTABLESIZE; i++)
                    this->mappingData.velocityIntervalTableValues[i] = (sysExData[6 + 2 * i] << 6) + sysExData[7 + 2 * i];

                curvesArea->resized();
                curvesArea->repaint();
            } else if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsTerpstraVelocityConfigReceptionMessage(midiMessage, TerpstraVelocityCurveConfig::VelocityCurveType::noteOnNoteOff)) {
                // After the answer state byte there must be 128 bytes of data
                // Values are in reverse order (shortest ticks count is the highest velocity)
                jassert(midiMessage.getSysExDataSize() >= 134); // ToDo display error otherwise
                this->mappingData.noteOnOffVelocityCurveConfig.editStrategy = TerpstraVelocityCurveConfig::EDITSTRATEGYINDEX::freeDrawing;
                for (int x = 0; x < 128; x++)
                    this->mappingData.noteOnOffVelocityCurveConfig.velocityValues[127 - x] = sysExData[6 + x];
                curvesArea->loadFromMapping();
            } else if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsTerpstraVelocityConfigReceptionMessage(midiMessage, TerpstraVelocityCurveConfig::VelocityCurveType::fader)) {
                // After the answer state byte there must be 128 bytes of data
                jassert(midiMessage.getSysExDataSize() >= 134); // ToDo display error otherwise
                this->mappingData.faderConfig.editStrategy = TerpstraVelocityCurveConfig::EDITSTRATEGYINDEX::freeDrawing;
                for (int x = 0; x < 128; x++)
                    this->mappingData.faderConfig.velocityValues[x] = sysExData[6 + x];
                curvesArea->loadFromMapping();
            } else if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsTerpstraVelocityConfigReceptionMessage(midiMessage, TerpstraVelocityCurveConfig::VelocityCurveType::afterTouch)) {
                // After the answer state byte there must be 128 bytes of data
                jassert(midiMessage.getSysExDataSize() >= 134); // ToDo display error otherwise
                this->mappingData.afterTouchConfig.editStrategy = TerpstraVelocityCurveConfig::EDITSTRATEGYINDEX::freeDrawing;
                for (int x = 0; x < 128; x++)
                    this->mappingData.afterTouchConfig.velocityValues[x] = sysExData[6 + x];
                curvesArea->loadFromMapping();
            } else if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsTerpstraVelocityConfigReceptionMessage(midiMessage, TerpstraVelocityCurveConfig::VelocityCurveType::lumaTouch)) {
                // After the answer state byte there must be 128 bytes of data
                jassert(midiMessage.getSysExDataSize() >= 134); // ToDo display error otherwise
                this->mappingData.lumaTouchConfig.editStrategy = TerpstraVelocityCurveConfig::EDITSTRATEGYINDEX::freeDrawing;
                for (int x = 0; x < 128; x++)
                    this->mappingData.lumaTouchConfig.velocityValues[x] = sysExData[6 + x];
                curvesArea->loadFromMapping();
            }

            // Key configurations
            else if (midiCmd == GET_RED_LED_CONFIG || midiCmd == GET_GREEN_LED_CONFIG || midiCmd == GET_BLUE_LED_CONFIG ||
                midiCmd == GET_CHANNEL_CONFIG || midiCmd == GET_NOTE_CONFIG || midiCmd == GET_KEYTYPE_CONFIG) {
                // After the answer state byte there must be 56 (or 55) bytes of data (one for each key)
                jassert(midiMessage.getSysExDataSize() >= TerpstraSysExApplication::getApp().getOctaveBoardSize() + 6); // ToDo display error otherwise

                for (int keyIndex = 0; keyIndex < TerpstraSysExApplication::getApp().getOctaveBoardSize(); keyIndex++) {
                    auto newValue = sysExData[6 + keyIndex];

                    TerpstraKey& keyData = this->mappingData.sets[boardNo - 1].theKeys[keyIndex];

                    switch (midiCmd) {
                        case GET_RED_LED_CONFIG:
                            newValue *= 5;
                            if (newValue > 255) {
                                jassertfalse;
                                newValue = 0;
                            }
                            keyData.colour = Colour(newValue, keyData.colour.getGreen(), keyData.colour.getBlue());	// This creates an opaque colour (alpha 0xff)
                            break;

                        case GET_GREEN_LED_CONFIG:
                            newValue *= 5;
                            if (newValue > 255) {
                                jassertfalse;
                                newValue = 0;
                            }
                            keyData.colour = Colour(keyData.colour.getRed(), newValue, keyData.colour.getBlue());
                            break;

                        case GET_BLUE_LED_CONFIG:
                            newValue *= 5;
                            if (newValue > 255) {
                                jassertfalse;
                                newValue = 0;
                            }
                            keyData.colour = Colour(keyData.colour.getRed(), keyData.colour.getGreen(), newValue);
                            break;

                        case GET_CHANNEL_CONFIG:
                            if (newValue > 16) {
                                jassertfalse;
                                newValue = 0;
                            }
                            keyData.channelNumber = newValue;
                            break;

                        case GET_NOTE_CONFIG:
                            if (newValue > 127) {
                                jassertfalse;
                                newValue = 0;
                            }
                            keyData.noteNumber = newValue;
                            break;

                        case GET_KEYTYPE_CONFIG:
                            // ToDo value validation
                            keyData.keyType = (TerpstraKey::KEYTYPE)newValue;
                            break;

                        default:
                            jassertfalse;   // Should not happen
                            break;
                    }
                }

				refreshKeyDataFields();
            }
        }
    }
    else if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsGetFirmwareRevisionResponse(midiMessage))
    {
        allKeysOverview->setFirmwareVersion(FirmwareVersion::fromGetFirmwareRevisionMsg(midiMessage));
    }
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
	// New height of subset field area, with minimal value
	int noteEditAreaWidth = noteEditArea->getWidth();
	int noteEditAreaHeight = noteEditArea->getHeight();

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
