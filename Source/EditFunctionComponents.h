/*
  ==============================================================================

    EditFunctionComponents.h
    Created: 12 Jul 2014 12:15:51pm
    Author:  hsstraub

  ==============================================================================
*/

#ifndef EDITFUNCTIONCOMPONENTS_H_INCLUDED
#define EDITFUNCTIONCOMPONENTS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

/*
==============================================================================
Assign notes to keys edit area
==============================================================================
*/
class NoteAssignTab : public Component
{
public:
	NoteAssignTab();
	~NoteAssignTab();

	void paint(Graphics& g);
	void resized();

public:
	GroupComponent* noteAndChannelAssGroup;
	Label*			editInstructionText;
	Label*			noteLabel;
	ToggleButton*	noteAutoIncrButton;
	ComboBox*		noteBox;
	Label*			channelLabel;
	ComboBox*		channelBox;
};



#endif  // EDITFUNCTIONCOMPONENTS_H_INCLUDED
