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

	// Open the specified device
	void setMidiOutput(int deviceIndex);

	// Attributes
private:
	StringArray midiInputs;
	StringArray midiOutputs;
	AudioDeviceManager deviceManager;
protected:
	ScopedPointer<MidiOutput> midiOutput;
};

#endif  // HAJUMIDIDRIVER_H_INCLUDED
