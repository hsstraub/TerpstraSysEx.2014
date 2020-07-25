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
#include "BoardGeometry.h"
#include "KeyboardDataStructure.h"
#include "TerpstraMidiDriver.h"
#include "MidiEditArea.h"
#include "NoteEditArea.h"
#include "MidiInfoArea.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent : public Component, public TerpstraMidiDriver::Listener
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
	TerpstraBoardGeometry& getBoardGeometry() { return this->boardGeometry; }

	// Board edit operations
	bool deleteCurrentSubBoardData();
	bool copyCurrentSubBoardData();
	bool pasteCurrentSubBoardData();

	// Implementation of TerpstraNidiDriver::Listener
	void midiMessageReceived(const MidiMessage& midiMessage) override;
	void midiMessageSent(const MidiMessage& midiMessage) override {}
	void midiSendQueueSize(int queueSize) override {}
    void generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel) override {}

	// GUI implementation
    void paint (Graphics&);
    void resized();
	void mouseDown(const MouseEvent &event);
	void mouseUp(const MouseEvent &event);

private:
	void changeSetSelection(int newSelection, bool forceRefresh = false);
	void changeSingleKeySelection(int newSelection);

private:
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)

	//==============================================================================
	// GUI components

	// Sets of 55/56 keys
	std::unique_ptr<OctaveBoardComponent> terpstraSetSelectors[NUMBEROFBOARDS];

	// Editing single keys (of the selected 56-key set)
	std::unique_ptr<TerpstraKeyEdit>	terpstraKeyFields[TERPSTRABOARDSIZE];

	// Geometry settings
	TerpstraBoardGeometry	boardGeometry;

	// Midi devices
	std::unique_ptr<MidiEditArea>		midiEditArea;

	// Edit fields for setting key and button parameters
	std::unique_ptr<NoteEditArea>	noteEditArea;

	// MIDI info area: received MIDI messages, warnings, error messages
	std::unique_ptr<MidiInfoArea> midiInfoArea;

	//==============================================================================
	// Data
	TerpstraKeyMapping	mappingData;
	int					currentSetSelection;
	int					currentSingleKeySelection;

	// Buffer for copy/paste of sub board data
	TerpstraKeys		copiedSubBoardData;
};


#endif  // MAINCOMPONENT_H_INCLUDED
