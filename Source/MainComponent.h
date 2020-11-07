/*
  ==============================================================================

    This file was auto-generated!
	Created: XXX.2014
	Author:  hsstraub

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "OctaveBoardComponent.h"
#include "ViewComponents.h"
#include "KeyboardDataStructure.h"
#include "TerpstraMidiDriver.h"
#include "MidiEditArea.h"
#include "NoteEditArea.h"


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
	int getCurrentSetSelection() const  { return currentSetSelection; }

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

private:
	void changeSetSelection(int newSelection, bool forceRefresh = false);

private:
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)

	//==============================================================================
	// GUI components

	// Sets of 55/56 keys
	std::unique_ptr<OctaveBoardComponent> terpstraSetSelectors[NUMBEROFBOARDS];

	// Midi devices
	std::unique_ptr<MidiEditArea>		midiEditArea;

	// Edit fields for setting key and button parameters, and edits for single keys
	std::unique_ptr<NoteEditArea>	noteEditArea;

	//==============================================================================
	// Data
	TerpstraKeyMapping	mappingData;
	int					currentSetSelection;

	// Buffer for copy/paste of sub board data
	TerpstraKeys		copiedSubBoardData;
};


#endif  // MAINCOMPONENT_H_INCLUDED
