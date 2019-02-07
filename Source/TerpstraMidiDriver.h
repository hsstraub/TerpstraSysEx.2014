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
#include "Hajulib/HajuDelayMidiDriver.h"
//[/Headers]

/*
==============================================================================
System exclusive command bytes
==============================================================================
*/

/*
Old definitions, for the first generation keyboard only

#define READ_KEY_NOTE 0x01
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

*/

#define MMID1 0x00
#define MMID2 0x20
#define MMID3 0xFF
#define CHANGE_KEY_NOTE 0x00
#define SET_KEY_COLOUR 0x01
#define STORE_TO_EEPROM 0x02
#define RECALL_FROM_EEPROM 0x03

#define SET_LIGHT_ON_KEYSTROKE 0x07


/*
==============================================================================
Connection to midi, sending SysEx parameters to keyboard
==============================================================================
*/
class TerpstraMidiDriver : public HajuDelayMidiDriver
{
public:
	TerpstraMidiDriver();
	~TerpstraMidiDriver();

	void setAutoSave(bool value) { this->autoSave = value; }

	//////////////////////////////////
	// Combined (hi-level) commands

	// Send parametrization of one key to the device and save it there, if autosave is active
	void sendAndMaybeSaveKeyParam(int boardIndex, int keyIndex, TerpstraKey keyData);

	// Send and save all parametrizations of one sub board
	void sendAllParamsOfBoard(int boardIndex, TerpstraKeys boardDa, bool saveAfterSending);

	// Send and save a complete key mapping
	void sendCompleteMapping(TerpstraKeyMapping mappingData, bool saveAfterSending);
	void sendAndSaveCompleteMapping(TerpstraKeyMapping mappingData) { sendCompleteMapping(mappingData, true); }

	// Store a sub board's sent key parametrizations permanently on device
	void storeAllToEEPROM();

	// Discard edits of a sub-board on device
	void recallAllFromEEPROM();

	//////////////////////////////////
	// Single (mid-level) commands

	// Send parametrization of one key to the device
	void sendKeyParam(int boardIndex, int keyIndex, TerpstraKey keyData);

	// Send expression pedal sensivity
	void sendExpressionPedalSensivity(unsigned char value);

	// Send parametrization of foot controller
	void sendInvertFootController(bool value);

	// Send parametrization of light on key strokes
	void sendLightOnKeyStroke(bool value);

	// Store a sub board's sent key parametrizations permanently on device
	void storeToEEPROM(int boardIndex);

	// Discard edits of a sub-board on device
	void recallFromEEPROM(int boardIndex);

private:
	// Low-level SysEx = valuemessage sending
	void sendSysEx(int boardIndex, unsigned char cmd, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4, unsigned char data5);

	// Attributes
private:
	bool autoSave;
};

#endif  // TERPSTRAMIDIDRIVER_H_INCLUDED
