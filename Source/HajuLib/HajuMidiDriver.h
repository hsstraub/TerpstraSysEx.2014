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
class HajuMidiDriver : public MidiInputCallback
{
public:
	HajuMidiDriver();
	~HajuMidiDriver();

	virtual void handleIncomingMidiMessage(MidiInput*, const MidiMessage&) = 0;

	//============================================================================
	// Primary functions with supported MIDI device

	// List of MIDI input device names
	const Array<MidiDeviceInfo>& getMidiInputList();
	// List of MIDI output device names
	const Array<MidiDeviceInfo>& getMidiOutputList();
    
	MidiDeviceInfo getLastMidiOutputInfo() const { return lastOutputDevice; }
	MidiDeviceInfo getLastMidiInputInfo() const { return lastInputDevice; }
    
    int getLastMidiInputIndex() const { return lastInputIndex; }
    int getLastMidiOutputIndex() const { return lastOutputIndex; }
	bool hasDevicesDefined() const { return lastInputIndex >= 0 && lastOutputIndex >= 0; }

	// Re-initializes device list in case of changes, returns true if a change was detected
	bool refreshDeviceLists();

	// Open the specified input device
	void setMidiInput(int deviceIndex);

	// Open the specified output device
	void setMidiOutput(int deviceIndex);

	// Send a MIDI message directly
	void sendMessageNow(const MidiMessage& message);

	// Higher level commands
	void sendNoteOnMessage(int noteNumber, int channelNumber, uint8 velocity);
	void sendNoteOffMessage(int noteNumber, int channelNumber, uint8 velocity);
    
    // Close current input device
    void closeMidiInput();
    
    // Close current output device
    void closeMidiOutput();

	//============================================================================
	// Device detection support

	// Refreshes and opens all available MIDI devices
	void openAvailableDevicesForTesting();

	// Send a message to a specific device
	void sendTestMessageNow(int outputDeviceIndex, const MidiMessage& message);

	// Closes all open testing devices; either setMidiInput and setMidiOutput will call this if it's not empty
	void closeTestingDevices();

	bool testIsIncomplete() const { return (testInputs.size() > 0 || testOutputs.size() > 0) && midiInput == nullptr && midiOutput == nullptr; }

	// Attributes
protected:
	Array<MidiDeviceInfo> midiInputs;
	Array<MidiDeviceInfo> midiOutputs;

    int lastOutputIndex = -1;
	int lastInputIndex = -1;

	MidiDeviceInfo lastOutputDevice;
	MidiDeviceInfo lastInputDevice;

	std::unique_ptr<MidiInput> selectedInput;
	std::unique_ptr<MidiOutput> selectedOutput;

	// Currently open MIDI output
	MidiOutput* midiOutput;

	// Last MIDI input opened
	MidiInput* midiInput;

	OwnedArray<MidiOutput> testOutputs;
	OwnedArray<MidiInput> testInputs;
};

#endif  // HAJUMIDIDRIVER_H_INCLUDED
