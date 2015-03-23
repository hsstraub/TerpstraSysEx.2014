/*
  ==============================================================================

  MainComponent.cpp
  Created: XXX.2014
  Author:  hsstraub

  ==============================================================================
*/

#include "MainComponent.h"
#include "ViewConstants.h"


//==============================================================================
MainContentComponent::MainContentComponent()
: currentSetSelection(-1), currentSingleKeySelection(-1)
{
	setSize(MAINWINDOWWIDTH, MAINWINDOWHEIGHT);

	// SysEx command buttons
	// XXX

	// Key set fields
	for (int i = 0; i < NUMBEROFBOARDS; i++)
	{
		// Paint them from right to left
		// (This will not matter any more when the images' backgrounds are transparent)
		// Width and heigth: were taken from image
		terpstraSetSelectors[4-i] = new TerpstraKeySetEdit();
		addAndMakeVisible(terpstraSetSelectors[4-i]);
		terpstraSetSelectors[4 - i]->setBounds(MAINWINDOWFIRSTCOLPOS + (4 - i)*TERPSTRAKEYSETFLDSPAN, TERPSTRAKEYSETFLDFIRSTYPOS, terpstraSetSelectors[4 - i]->getWidth(), terpstraSetSelectors[4 - i]->getHeight());
		terpstraSetSelectors[4-i]->addListener(this);
	}

	// Single Key fields

	// Transformation Rotate slightly counterclockwise
	int x = MAINWINDOWFIRSTCOLPOS;
	int y = TERPSTRASINGLEKEYFIELDFIRSTYPOS;
	AffineTransform transform = AffineTransform::translation(-x, -y);
	transform = transform.rotated(TERPSTRASINGLEKEYROTATIONANGLE);
	transform = transform.translated(x, y);

	// Rows from right to left
	// first row
	x = MAINWINDOWFIRSTCOLPOS + TERPSTRASINGLEKEYFLDSIZE;
	y = TERPSTRASINGLEKEYFIELDFIRSTYPOS;
	transform.transformPoint(x, y);
	terpstraKeyFields[0] = new TerpstraKeyEdit();
	addAndMakeVisible(terpstraKeyFields[0]);
	terpstraKeyFields[0]->setBounds(x, y, TERPSTRASINGLEKEYFLDSIZE, TERPSTRASINGLEKEYFLDSIZE);

	terpstraKeyFields[1] = new TerpstraKeyEdit();
	addAndMakeVisible(terpstraKeyFields[1]);
	x = MAINWINDOWFIRSTCOLPOS;
	y = TERPSTRASINGLEKEYFIELDFIRSTYPOS;
	transform.transformPoint(x, y);
	terpstraKeyFields[1]->setBounds(x, y, TERPSTRASINGLEKEYFLDSIZE, TERPSTRASINGLEKEYFLDSIZE);

	// 8 identical rows
	for (int row = 0; row < 8; row++)
	{
		int xbasepos;
		if (row % 2 == 0)
			xbasepos = MAINWINDOWFIRSTCOLPOS + TERPSTRASINGLEKEYFLDSIZE / 2;
		else
			xbasepos = MAINWINDOWFIRSTCOLPOS;

		int ybasepos = TERPSTRASINGLEKEYFIELDFIRSTYPOS + 3 * (row + 1) * TERPSTRASINGLEKEYFLDSIZE / 4;

		for (int i = 0; i < 6; i++)
		{
			terpstraKeyFields[2 + 6 * row + i] = new TerpstraKeyEdit();
			addAndMakeVisible(terpstraKeyFields[2 + 6 * row + i]);
			x = xbasepos + (5-i)*TERPSTRASINGLEKEYFLDSIZE;
			y = ybasepos;
			transform.transformPoint(x, y);
			terpstraKeyFields[2 + 6 * row + i]->setBounds(x, y, TERPSTRASINGLEKEYFLDSIZE, TERPSTRASINGLEKEYFLDSIZE);
		}
	}

	// 9th row
	for (int i = 0; i < 5; i++)
	{
		terpstraKeyFields[50 + i] = new TerpstraKeyEdit();
		addAndMakeVisible(terpstraKeyFields[50 + i]);
		x = MAINWINDOWFIRSTCOLPOS + (2 * (4-i) + 3)*TERPSTRASINGLEKEYFLDSIZE / 2;
		y = TERPSTRASINGLEKEYFIELDFIRSTYPOS + 3 * 9 * TERPSTRASINGLEKEYFLDSIZE / 4;
		transform.transformPoint(x, y);
		terpstraKeyFields[50 + i]->setBounds(x, y, TERPSTRASINGLEKEYFLDSIZE, TERPSTRASINGLEKEYFLDSIZE);
	}

	// 10th row
	terpstraKeyFields[55] = new TerpstraKeyEdit();
	addAndMakeVisible(terpstraKeyFields[55]);
	x = MAINWINDOWFIRSTCOLPOS + 5 * TERPSTRASINGLEKEYFLDSIZE;
	y = TERPSTRASINGLEKEYFIELDFIRSTYPOS + 3 * 10 * TERPSTRASINGLEKEYFLDSIZE / 4;
	transform.transformPoint(x, y);
	terpstraKeyFields[55]->setBounds(x, y, TERPSTRASINGLEKEYFLDSIZE, TERPSTRASINGLEKEYFLDSIZE);

	for (int i = 0; i < TERPSTRABOARDSIZE; i++)
		terpstraKeyFields[i]->addMouseListener(this, true);

	// Midi input + output
	lblMidiInput = new Label("Label", "MIDI Input Device");
	addAndMakeVisible(lblMidiInput);
	lblMidiInput->setBounds(EDITAREAFIRSTCOLPOS, 250, MIDIPORTWIDTH, STANDARDLABELHEIGTH);

	cbMidiInput = new ComboBox();
	addAndMakeVisible(cbMidiInput);
	cbMidiInput->setBounds(EDITAREAFIRSTCOLPOS, 280, MIDIPORTWIDTH, 20);
	cbMidiInput->addItemList(MidiInput::getDevices(), 1);
	cbMidiInput->addListener(this);

	lblMidiOutput = new Label("Label", "MIDI Output Device");
	addAndMakeVisible(lblMidiOutput);
	lblMidiOutput->setBounds(EDITAREAFIRSTCOLPOS + EDITAREAWIDTH - MIDIPORTWIDTH, 250, MIDIPORTWIDTH, STANDARDLABELHEIGTH);

	cbMidiOutput = new ComboBox();
	addAndMakeVisible(cbMidiOutput);
	cbMidiOutput->setBounds(EDITAREAFIRSTCOLPOS+EDITAREAWIDTH - MIDIPORTWIDTH, 280, MIDIPORTWIDTH, 20);
	cbMidiOutput->addItemList(MidiOutput::getDevices(), 1);
	cbMidiOutput->addListener(this);

	editArea = new TabbedComponent(TabbedButtonBar::Orientation::TabsAtTop);
	addAndMakeVisible(editArea);
	editArea->setBounds(EDITAREAFIRSTCOLPOS, 310, EDITAREAWIDTH, EDITAREAHEIGHT);

	noteAssignTab = new NoteAssignTab();
	editArea->addTab("Assign Notes to Keys", Colour(MAINWINDOWBGCOLOUR), noteAssignTab, true);


	// Select first board and first key
	changeSetSelection(0);
	changeSingleKeySelection(0);
}

MainContentComponent::~MainContentComponent()
{
	deleteAllChildren();
}

// Transfer of data
void MainContentComponent::setData(TerpstraKeyMapping& newData)
{
	mappingData = newData;

	changeSetSelection(-1);
	changeSetSelection(0);
}

void MainContentComponent::getData(TerpstraKeyMapping& newData)
{
	newData = mappingData;
}


void MainContentComponent::paint (Graphics& g)
{
	g.fillAll(Colour(MAINWINDOWBGCOLOUR));

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainContentComponent::buttonClicked(Button *button)
{
	for (int i = 0; i < NUMBEROFBOARDS; i++)
	{
		if (button == terpstraSetSelectors[i])
		{
			changeSetSelection(i);
			break;
		}
	}
}

void MainContentComponent::mouseDown(const MouseEvent &event)
{
	// Selection of single key fields
	for (int i = 0; i < TERPSTRABOARDSIZE; i++)
	{
		if (event.eventComponent == terpstraKeyFields[i] || event.eventComponent->getParentComponent() == terpstraKeyFields[i])
		{
			// Select field
			changeSingleKeySelection(i);

			// Edit
			// XXX encapsulate in noteAssignTab?
			TerpstraKey keyData;
			keyData.noteNumber = noteAssignTab->noteBox->getSelectedItemIndex(); //-1 for no selection or 0-127
			if (keyData.noteNumber < 0) keyData.noteNumber = 0;
			keyData.channelNumber = noteAssignTab->channelBox->getSelectedId();	// 0 for no selection or 1-16

			mappingData.sets[currentSetSelection].theKeys[i] = keyData;		// Save data
			terpstraKeyFields[i]->setValue(keyData);						// Display

			// Send to device, if "sending at once" is specified
			// XXX

			// Auto increment
			if (noteAssignTab->noteAutoIncrButton->getToggleState())
				noteAssignTab->noteBox->setSelectedItemIndex(noteAssignTab->noteBox->getSelectedItemIndex() + 1);

			break;
		}
	}
}

void MainContentComponent::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	if (comboBoxThatHasChanged == cbMidiInput)
	{
		//[UserComboBoxCode_comboBoxMidiInput] -- add your combo box handling code here..
		//[/UserComboBoxCode_comboBoxMidiInput]
	}
	else if (comboBoxThatHasChanged == cbMidiOutput)
	{
		//[UserComboBoxCode_comboBoxMidiOutput] -- add your combo box handling code here..
		midiDriver.setMidiOutput(cbMidiOutput->getSelectedItemIndex());
		//[/UserComboBoxCode_comboBoxMidiOutput]
	}
}

void MainContentComponent::changeSetSelection(int newSelection)
{
	if (newSelection != currentSetSelection)
	{
		// Unselect previous set
		// saving the data was done in click event on the single key fields
		if (currentSetSelection >= 0 && currentSetSelection < NUMBEROFBOARDS)
			terpstraSetSelectors[currentSetSelection]->setIsSelected(false);

		// Set data of new selection
		if (newSelection >= 0 && newSelection < NUMBEROFBOARDS )
		{
			for (int i = 0; i < TERPSTRABOARDSIZE; i++)
				terpstraKeyFields[i]->setValue(mappingData.sets[newSelection].theKeys[i]);
		}

		currentSetSelection = newSelection;
	}

	// Set toggle state in any case (override default imagebutton functionality)
	if ( currentSetSelection >= 0 && currentSetSelection < NUMBEROFBOARDS )
		terpstraSetSelectors[currentSetSelection]->setIsSelected(true);
}

void MainContentComponent::changeSingleKeySelection(int newSelection)
{
	// Unselect previous key
	if (currentSingleKeySelection >= 0 && currentSingleKeySelection < TERPSTRABOARDSIZE )
		terpstraKeyFields[currentSingleKeySelection]->setIsSelected(false);

	// Select new key
	currentSingleKeySelection = newSelection;
	if (currentSingleKeySelection >= 0 && currentSingleKeySelection < TERPSTRABOARDSIZE)
		terpstraKeyFields[currentSingleKeySelection]->setIsSelected(true);
}