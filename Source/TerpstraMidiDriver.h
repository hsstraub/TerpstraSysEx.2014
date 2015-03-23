/*
  ==============================================================================

    TerpstraMidiDriver.h
    Created: 20 Feb 2015 8:04:02pm
    Author:  hsstraub

  ==============================================================================
*/

#ifndef TERPSTRAMIDIDRIVER_H_INCLUDED
#define TERPSTRAMIDIDRIVER_H_INCLUDED

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "KeyboardDataStructure.h"
//[/Headers]

/*
==============================================================================
System exclusive command bytes
==============================================================================
*/

#define MMID1 0x00
#define MMID2 0x20
#define MMID3 0xFF
#define CHANGE_KEY_NOTE 0x00
#define READ_KEY_NOTE 0x01
#define STORE_TO_EEPROM 0x02
#define RECALL_FROM_EEPROM 0x03
#define READ_KEY_POSITION 0x04
#define SET_KEY_MAX_MIN 0x05
#define READ_KEY_MAX_MIN 0x06
#define SET_KEYUP_PROXIMITY 0x07
#define SET_KEYDN_PROXIMITY 0x08
#define SET_FOOT_CONTROLLER_SENSITIVITY 0x09
#define READ_FOOT_CONTROLLER_SENSITIVITY 0x0A
#define SET_FOOT_CONTROLLER_MAX 0x0B
#define READ_FOOT_CONTROLLER_MAX 0x0C
#define SET_FOOT_CONTROLLER_MIN 0x0D
#define READ_FOOT_CONTROLLER_MIN 0x0E
#define INVERT_FOOT_CONTROLLER 0x0F
#define SEND_KEY_DWELL_TO_VELOCITY_NUMBER 0x10
#define READ_KEY_DWELL_TO_VELOCITY_NUMBER 0x11


/*
==============================================================================
Connection to midi, sending SysEx parameters to keyboard
==============================================================================
*/
class TerpstraMidiDriver
{
public:
	TerpstraMidiDriver();
	~TerpstraMidiDriver();

	// Open the specified device
	void setMidiOutput(int deviceIndex);

	//////////////////////////////////
	// Single (mid-level) commands

	// Send parametrization of one key to the device
	void sendKeyParam(int boardIndex, int keyIndex, TerpstraKey keyData);

	// Store a board's sent key parametrizations permanmently on device
	void storeToEEPROM(int boardIndex);

	// Discard edits on device
	void recallFromEEPROM(int boardIndex);

private:
	// Low-level SysEx message sending
	void sendSysEx(int boardIndex, unsigned char cmd, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4, unsigned char data5);

	// Attributes
private:
	StringArray midiInputs;
	StringArray midiOutputs;
	AudioDeviceManager deviceManager;

	ScopedPointer<MidiOutput> midiOutput;

};


#endif  // TERPSTRAMIDIDRIVER_H_INCLUDED
