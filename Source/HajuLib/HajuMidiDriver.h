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

	// List of MIDI input devices
	StringArray& getMidiInputList() { return midiInputs; }
	// List of MIDI output devices
	StringArray& getMidiOutputList() { return midiOutputs; }

	// Open the specified input device
	void setMidiInput(int deviceIndex, MidiInputCallback* callback);
		
	// Open the specified output device
	void setMidiOutput(int deviceIndex);

	// Send a MIDI message directly 
	void sendMessageNow(const MidiMessage& message);

	// Attributes
private:
	StringArray midiInputs;
	StringArray midiOutputs;
	AudioDeviceManager deviceManager;

protected:
	int lastInputIndex;
	MidiInputCallback* lastInputCallback;

	// Currently open MIDI output
	ScopedPointer<MidiOutput> midiOutput;
};

#endif  // HAJUMIDIDRIVER_H_INCLUDED
