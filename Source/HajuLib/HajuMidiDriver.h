/*
  ==============================================================================

    HajuMidiDriver.h
    Created: 23 Feb 2018 11:30:58pm
    Author:  hsstraub

  ==============================================================================
*/

#ifndef HAJUMIDIDRIVER_H_INCLUDED
#define HAJUMIDIDRIVER_H_INCLUDED

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
//[/Headers]


/*
==============================================================================
Driver encapsulating MIDI functionality, extension of JUCE
==============================================================================
*/
class HajuMidiDriver
{
public:
	HajuMidiDriver();
	~HajuMidiDriver();

	// List of MIDI input device names
	StringArray getMidiInputList();
	// List of MIDI output device names
	StringArray getMidiOutputList();

	// Re-initializes device list in case of changes
	void refreshDeviceLists();

	// Open the specified input device
	void setMidiInput(int deviceIndex, MidiInputCallback* callback);

	// Open the specified output device
	void setMidiOutput(int deviceIndex);

	// Send a MIDI message directly
	void sendMessageNow(const MidiMessage& message);

	// Higher level commands
	void sendNoteOnMessage(int noteNumber, int channelNumber, uint8 velocity);
	void sendNoteOffMessage(int noteNumber, int channelNumber, uint8 velocity);


	// Attributes
protected:
	Array<MidiDeviceInfo> midiInputs;
	Array<MidiDeviceInfo> midiOutputs;
	AudioDeviceManager deviceManager;

	int lastInputIndex = -1;
	MidiInputCallback* lastInputCallback = nullptr;

	// Currently open MIDI output
  std::unique_ptr<MidiOutput> midiOutput = nullptr;
};

#endif  // HAJUMIDIDRIVER_H_INCLUDED
