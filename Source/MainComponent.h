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
#include "TerpstraMidiDriver.h"
#include "MidiEditArea.h"
#include "NoteEditArea.h"
#include "GeneralOptionsDlg.h"
#include "CurvesArea.h"
#include "GlobalSettingsArea.h"
#include "LumatoneEditorLookAndFeel.h"
#include "ColourPaletteWindow.h"



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent : public Component, 
							 public TerpstraMidiDriver::Listener, 
							 public ChangeListener,
							 public Button::Listener,
	public Slider::Listener
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

	// Board edit operations
	bool deleteCurrentSubBoardData();
	bool copyCurrentSubBoardData();
	bool pasteCurrentSubBoardData();

	// Implementation of TerpstraNidiDriver::Listener
	void midiMessageReceived(const MidiMessage& midiMessage) override;
	void midiMessageSent(const MidiMessage& midiMessage) override {}
	void midiSendQueueSize(int queueSize) override {}
    void generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel) override {}

	// Implementation of ChangeListener
	void changeListenerCallback(ChangeBroadcaster *source) override;

	// Implementation of Button::Listener
	void buttonClicked(Button* btn) override;

	// GUI implementation
    void paint (Graphics&);
    void resized();

	void refreshAllKeysOverview();

	void sliderValueChanged(Slider* slider) override;

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

	std::unique_ptr<GeneralOptionsDlg> generalOptionsArea;
	
	std::unique_ptr<CurvesArea> curvesArea;

	std::unique_ptr<GlobalSettingsArea> globalSettingsArea;

	std::unique_ptr<ColourPaletteWindow> palettePopup;

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

    const float popupWidth                  = 0.2879f;
    const float popupHeight                 = 0.2615f;

    const float lumatoneVersionMarginX      = 1.0f / 46.0f;
    const float lumatoneVersionWidth        = 1.0f / 5.0f;

    const float presetButtonColoursX        = 0.7237f;
    const float presetButtonColoursHeight   = 0.2258f;

    const float settingsColumnX             = 0.597826f;
    const float settingsAreaY               = 0.546875f;
    const float settingsAreaHeight          = 0.148148f;

    const Rectangle<float> generalSettingsBounds = { settingsColumnX, settingsAreaY, 0.17f, settingsAreaHeight };
    const Rectangle<float>   pedalSettingsBounds = { 7.0f / 9.0f,     settingsAreaY, 0.26f, settingsAreaHeight };
    const Rectangle<float>      curvesAreaBounds = { settingsColumnX, 0.7174f,       0.3626f, 0.21f };

	//DEBUG

	std::unique_ptr<Slider> marginRatioSlider;
	std::unique_ptr<Slider> keyRotationSlider;
};