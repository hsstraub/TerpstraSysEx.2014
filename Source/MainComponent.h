/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "ViewComponents.h"
#include "EditFunctionComponents.h"
#include "KeyboardDataStructure.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component, public ButtonListener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

	// Transfer of data
	void setData(TerpstraKeyMapping& newData);
	void getData(TerpstraKeyMapping& newData);


	// GUI implementation
    void paint (Graphics&);
    void resized();
	void buttonClicked(Button *button);
	void mouseDown(const MouseEvent &event);

private:
	void changeSetSelection(int newSelection);
	void changeSingleKeySelection(int newSelection);

private:
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)

	//==============================================================================
	// GUI components

	// Buttons for SysEx commands
	// XXX

	// Sets of 56 krys
	TerpstraKeySetEdit* terpstraSetSelectors[NUMBEROFBOARDS];

	// Editing single keys (of the selected 56-key set)
	TerpstraKeyEdit*	terpstraKeyFields[TERPSTRABOARDSIZE];

	// Midi devices
	Label*		lblMidiInput;
	ComboBox*	cbMidiInput;
	Label*		lblMidiOutput;
	ComboBox*	cbMidiOutput;

	// Edit fields for setting key and button parameters
	TabbedComponent*	editArea;
	NoteAssignTab*		noteAssignTab;

	//==============================================================================
	// Data
	TerpstraKeyMapping	mappingData;
	int					currentSetSelection;
	int					currentSingleKeySelection;
};


#endif  // MAINCOMPONENT_H_INCLUDED
