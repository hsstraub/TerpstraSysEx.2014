/*
  ==============================================================================

    EditFunctionComponents.cpp
    Created: 12 Jul 2014 12:15:51pm
    Author:  hsstraub

  ==============================================================================
*/

#include "EditFunctionComponents.h"
#include "ViewConstants.h"


NoteAssignTab::NoteAssignTab()
{
	noteAndChannelAssGroup = new GroupComponent("noteAndChannelAssGroup", "Key Note and Channel Assignments");
	addAndMakeVisible(noteAndChannelAssGroup);
	noteAndChannelAssGroup->setBounds(5, 5, EDITAREAWIDTH-10, 200);

	editInstructionText = new Label("editInstructionText", "Assign these values to a key by clicking on the desired key-face.");
	addAndMakeVisible(editInstructionText);
	editInstructionText->setBounds(10, 35, EDITAREAWIDTH - 20, STANDARDLABELHEIGTH);

	noteLabel = new Label("noteLabel", "Note (0-127):");
	addAndMakeVisible(noteLabel);
	noteLabel->setBounds(70, 35, 70, STANDARDLABELHEIGTH);

	noteBox = new ComboBox("noteBox");
	addAndMakeVisible(noteBox);
	noteBox->setBounds(120, 70, 50, 20);
	for (int i = 0; i <= 127; i++)
		noteBox->addItem(String(i), i+1);

	noteLabel->attachToComponent(noteBox, true);

	noteAutoIncrButton = new ToggleButton("Auto Increment");
	addAndMakeVisible(noteAutoIncrButton);
	noteAutoIncrButton->setBounds(180, 70, 150, 20);

	channelLabel = new Label("channelLabel", "Channel (1-16):");
	addAndMakeVisible(channelLabel);
	channelLabel->setBounds(70, 95, 70, STANDARDLABELHEIGTH);

	channelBox = new ComboBox("channelBox");
	addAndMakeVisible(channelBox);
	channelBox->setBounds(120, 95, 50, 20);
	for (int i = 1; i <= 16; i++)
		channelBox->addItem(String(i), i);

	channelLabel->attachToComponent(channelBox, true);
}

NoteAssignTab::~NoteAssignTab()
{
	deleteAllChildren();

}

void NoteAssignTab::paint(Graphics& g)
{

}

void NoteAssignTab::resized()
{

}
