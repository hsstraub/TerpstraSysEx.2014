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
#include "HajuLib/HajuDelayMidiDriver.h"
//[/Headers]

/*
==============================================================================
System exclusive command bytes
==============================================================================
*/

/*
Old definitions, for the first generation keyboard only

// Manufacturer Id
#define MMID1 0x00
#define MMID2 0x20
#define MMID3 0xff

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

*/

#define CHANGE_KEY_NOTE 0x00
#define SET_KEY_COLOUR 0x01

#define SET_FOOT_CONTROLLER_SENSITIVITY 0x03
#define INVERT_FOOT_CONTROLLER 0x04

#define MACROBUTTON_COLOUR_ON 0x05
#define MACROBUTTON_COLOUR_OFF 0x06

#define SET_LIGHT_ON_KEYSTROKE 0x07

#define SET_VELOCITY_CONFIG 0x08
#define SAVE_VELOCITY_CONFIG 0x09
#define RESET_VELOCITY_CONFIG 0x0A

#define SET_FADER_CONFIG 0x0B
#define SAVE_FADER_CONFIG 0x0C
#define RESET_FADER_CONFIG 0x0D

#define SET_AFTERTOUCH_FLAG 0x0E
#define CALIBRATE_AFTERTOUCH 0x0F
#define SET_AFTERTOUCH_CONFIG 0x10
#define SAVE_AFTERTOUCH_CONFIG 0x11
#define RESET_AFTERTOCUH_CONFIG 0x12

/*
==============================================================================
Connection to midi, sending SysEx parameters to keyboard
==============================================================================
*/
class TerpstraMidiDriver : public HajuDelayMidiDriver
{
    // Types
public:
 	typedef enum
	{
		noteOnNoteOff = 1,
		fader = 2,
		afterTouch = 3
	} VelocityCurveType;

public:
	TerpstraMidiDriver();
	~TerpstraMidiDriver();

	void setAutoSave(bool value) { this->autoSave = value; }

	void setManufacturerId(int value) { manufacturerId = value; }

	//////////////////////////////////
	// Combined (hi-level) commands

	// Send all parametrizations of one sub board
	void sendAllParamsOfBoard(int boardIndex, TerpstraKeys boardData);

	// Send and save a complete key mapping
	void sendCompleteMapping(TerpstraKeyMapping mappingData);

	//////////////////////////////////
	// Single (mid-level) commands

	// Send parametrization of one key to the device
	void sendKeyParam(int boardIndex, int keyIndex, TerpstraKey keyData);

	// Store a sub board's sent key parametrizations permanently on device
	//void storeToEEPROM(int boardIndex);

	// Discard edits of a sub-board on device
	//void recallFromEEPROM(int boardIndex);

	// Send expression pedal sensivity
	void sendExpressionPedalSensivity(unsigned char value);

	// Send parametrization of foot controller
	void sendInvertFootController(bool value);

	// Colour for macro button in active state
	void sendMacroButtonActiveColour(String colourAsString);

	// Colour for macro button in inactive state
	void sendMacroButtonInactiveColour(String colourAsString);

	// Send parametrization of light on key strokes
	void sendLightOnKeyStroke(bool value);

	// Send a value for a velocity lookup table
	void sendVelocityConfig(VelocityCurveType velocityCurveType, unsigned char velocityTable[]);

	// Save velocity config to EEPROM
	void saveVelocityConfig(VelocityCurveType velocityCurveType);

	// reset velocity config to value from EEPROM
	void resetVelocityConfig(VelocityCurveType velocityCurveType);

	void sendAfterTouchActivation(bool value);

	void sendCalibrateAfterTouch();

private:
	// Low-level SysEx = valuemessage sending
	void sendSysEx(int boardIndex, unsigned char cmd, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4, unsigned char data5);

	// Attributes
private:
	bool autoSave;

	int manufacturerId = 0x002150;
};

#endif  // TERPSTRAMIDIDRIVER_H_INCLUDED
