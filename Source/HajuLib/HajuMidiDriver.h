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
    
	// Return the current input device index
    int getMidiInputIndex() const { return lastInputIndex; }

	// Return the current output device index
    int getMidiOutputIndex() const { return lastOutputIndex; }

	// Returns whether or not the current device indicies are valid
	bool hasDevicesDefined() const { return lastInputIndex >= 0 && lastOutputIndex >= 0; }

	// Returns the index of the given MidiDevice if found, or -1 if not present
	int getIndexOfInputDevice(MidiDeviceInfo inputDeviceInfo) const { return midiInputs.indexOf(inputDeviceInfo); }
	int getIndexOfInputDevice(String inputDeviceId) const;
	// Returns the index of the given MidiDevice if found, or -1 if not present
	int getIndexOfOutputDevice(MidiDeviceInfo outputDeviceInfo) const { return midiOutputs.indexOf(outputDeviceInfo); }
	int getIndexOfOutputDevice(String outputDeviceId) const;

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
