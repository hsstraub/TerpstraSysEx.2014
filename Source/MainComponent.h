/*
  ==============================================================================

    This file was auto-generated!
	Created: XXX.2014
	Author:  hsstraub

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "AllKeysOverview.h"
#include "ViewComponents.h"
#include "KeyboardDataStructure.h"
#include "LumatoneController.h"
#include "MidiEditArea.h"
#include "NoteEditArea.h"
#include "GeneralOptionsDlg.h"
#include "CurvesArea.h"
#include "GlobalSettingsArea.h"
#include "PedalSensitivityDlg.h"
#include "LumatoneEditorLookAndFeel.h"
#include "ColourPaletteWindow.h"



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent : public Component, 
							 public LumatoneController::FirmwareListener,
							 public ChangeListener,
							 public Button::Listener
{
public:
	//==============================================================================
	MainContentComponent();
	~MainContentComponent();

	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);

	// Transfer of data
	void setData(TerpstraKeyMapping& newData, bool withRefresh = true);
	void deleteAll(bool withRefresh = true);

	void getData(TerpstraKeyMapping& newData);
	TerpstraKeyMapping&	getMappingInEdit() { return this->mappingData; }

	TabbedButtonBar* getOctaveBoardSelectorTab() { return  noteEditArea->getOctaveBoardSelectorTab(); }
	CurvesArea* getCurvesArea() { return curvesArea.get(); }

	// Board edit operations
	UndoableAction* createDeleteCurrentSectionAction();
	bool copyCurrentSubBoardData();
	UndoableAction* createPasteCurrentSectionAction();
    UndoableAction* createModifiedPasteCurrentSectionAction(CommandID commandID);
    bool canPasteCopiedSubBoard() const;

	bool setDeveloperMode(bool developerModeOn);

	// Implementation of ChangeListener
	void changeListenerCallback(ChangeBroadcaster *source) override;

	// Implementation of Button::Listener
	void buttonClicked(Button* btn) override;

	// GUI implementation
    void paint (Graphics&);
    void resized();

	void refreshKeyDataFields();
	void refreshAllFields();

	// Implementation of LumatoneController::FirmwareListener

	void octaveColourConfigReceived(int octaveIndex, uint8 rgbFlag, const int* colourData) override;

	void octaveChannelConfigReceived(int octaveIndex, const int* channelData) override;

	void octaveNoteConfigReceived(int octaveIndex, const int* noteData) override;

	void keyTypeConfigReceived(int octaveIndex, const int* keyTypeData) override;

	void velocityConfigReceived(const int* velocityData) override;

	void aftertouchConfigReceived(const int* aftertouchData) override;

	void velocityIntervalConfigReceived(const int* velocityData) override;

	void faderConfigReceived(const int* faderData) override;

	void faderTypeConfigReceived(int octaveIndex, const int* faderTypeData) override;

	void lumatouchConfigReceived(const int* lumatouchData) override;

	void firmwareRevisionReceived(FirmwareVersion version) override;

private:
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)

	//==============================================================================
	// GUI components

	// Midi devices and connection state
	std::unique_ptr<MidiEditArea>		midiEditArea;

	// Sets of 55/56 keys
	std::unique_ptr<AllKeysOverview> allKeysOverview;

	// Edit fields for setting key and button parameters, and edits for single keys
	std::unique_ptr<NoteEditArea>	noteEditArea;

	std::unique_ptr<GeneralOptionsDlg>	generalOptionsArea;
	
	std::unique_ptr<CurvesArea> curvesArea;

	std::unique_ptr<GlobalSettingsArea> globalSettingsArea;

	std::unique_ptr<PedalSensitivityDlg> pedalSensitivityDlg;

	// Version signature in bottom left of window
	std::unique_ptr<Label> lblAppName;

	std::unique_ptr<Label> lblAppVersion;

	//==============================================================================
	// Data
	TerpstraKeyMapping	mappingData;

	// Buffer for copy/paste of sub board data
	TerpstraKeys		copiedSubBoardData;

	//==============================================================================
	// Position and Size helpers

    Rectangle<int> controlsArea;

    const float headerHeight                = 0.0776f;

    const float controlsAreaY               = 0.5273f;
    const float controlsAreaHeight          = 0.43f;

    const float assignMarginX               = 0.043f;
    const float assignWidth                 = 0.5385f;
    const float assignHeight                = 0.44f;

    const float footerAreaY                 = 0.96f;

    const float popupWidth                  = 0.33f;
    const float popupHeight                 = 0.273f;

    const float lumatoneVersionMarginX      = 0.02f;
	const float lumatoneVersionWidth        = 0.2f;
	const float lumatoneVersionHeight       = 0.5f;

    const float presetButtonColoursX        = 0.7237f;
    const float presetButtonColoursHeight   = 0.2258f;

    const float settingsColumnX             = 0.597826f;
    const float settingsAreaY               = 0.546875f;
    const float settingsAreaHeight          = 0.148148f;

    const Rectangle<float> generalSettingsBounds = { settingsColumnX, settingsAreaY, 0.17f, settingsAreaHeight };
    const Rectangle<float>   pedalSettingsBounds = { 0.777778f,       settingsAreaY, 0.18f, settingsAreaHeight };
    const Rectangle<float>      curvesAreaBounds = { settingsColumnX, 0.7174f,       0.3626f, 0.21f };

};
