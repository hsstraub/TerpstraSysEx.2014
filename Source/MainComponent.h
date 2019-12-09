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

#include "ViewComponents.h"
#include "BoardGeometry.h"
#include "KeyboardDataStructure.h"
#include "TerpstraMidiDriver.h"
#include "MidiEditArea.h"
#include "NoteEditArea.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent : public Component, public Button::Listener, public MidiInputCallback
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);

	// Transfer of data
	void setData(TerpstraKeyMapping& newData);
	void getData(TerpstraKeyMapping& newData);
	TerpstraKeyMapping&	getMappingInEdit() { return this->mappingData; }

	// Board ddit operations
	bool deleteCurrentSubBoardData();
	bool copyCurrentSubBoardData();
	bool pasteCurrentSubBoardData();

	// MIDI input callback
	void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override;
	
	// GUI implementation
    void paint (Graphics&);
    void resized();
	void buttonClicked(Button *button);
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

	// Macro buttons
	TerpstraMacroButton* macroButtons[NUMBEROFBOARDS];

	// Sets of 56 keys
	TerpstraKeySetEdit* terpstraSetSelectors[NUMBEROFBOARDS];

	// Editing single keys (of the selected 56-key set)
	TerpstraKeyEdit*	terpstraKeyFields[TERPSTRABOARDSIZE];

	// Geometry settings 
	TerpstraBoardGeometry	boardGeometry;

	// Midi devices
	MidiEditArea*		midiEditArea;

	// Edit fields for setting key and button parameters
	NoteEditArea*	noteEditArea;

	//==============================================================================
	// Data
	TerpstraKeyMapping	mappingData;
	int					currentSetSelection;
	int					currentSingleKeySelection;

	// Buffer for copy/paste of sub board data
	TerpstraKeys		copiedSubBoardData;
};


#endif  // MAINCOMPONENT_H_INCLUDED
