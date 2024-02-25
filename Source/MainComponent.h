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
#include "PedalSensitivityDlg.h"



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent : public Component, public TerpstraMidiDriver::Listener, public ChangeListener
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

	// GUI implementation
    void paint (Graphics&);
    void resized();

	void refreshAllKeysOverview();

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

	std::unique_ptr<PedalSensitivityDlg> pedalSensitivityDlg;

	//==============================================================================
	// Data
	TerpstraKeyMapping	mappingData;

	// Buffer for copy/paste of sub board data
	TerpstraKeys		copiedSubBoardData;
};

