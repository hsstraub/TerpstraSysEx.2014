/*
  ==============================================================================

    TerpstraMidiDriver.cpp
    Created: 20 Feb 2015 8:04:02pm
    Author:  hsstraub

  ==============================================================================
*/

#include "TerpstraMidiDriver.h"


TerpstraMidiDriver::TerpstraMidiDriver() : HajuDelayMidiDriver()
{
	autoSave = false;
}

TerpstraMidiDriver::~TerpstraMidiDriver() 
{
}

/*
==============================================================================
Combined (hi-level) commands
*/

void TerpstraMidiDriver::sendAndMaybeSaveKeyParam(int boardIndex, int keyIndex, TerpstraKey keyData)
{
	sendKeyParam(boardIndex, keyIndex, keyData);

	if (autoSave)
		storeToEEPROM(boardIndex);
}

void TerpstraMidiDriver::sendAllParamsOfBoard(int boardIndex, TerpstraKeys boardData, bool saveAfterSending)
{
	for (int keyIndex = 0; keyIndex < TERPSTRABOARDSIZE; keyIndex++)
		sendKeyParam(boardIndex, keyIndex, boardData.theKeys[keyIndex]);

	if (saveAfterSending)
		storeToEEPROM(boardIndex);
}

void TerpstraMidiDriver::sendCompleteMapping(TerpstraKeyMapping mappingData, bool saveAfterSending)
{
	for (int boardIndex = 1; boardIndex <= NUMBEROFBOARDS; boardIndex++)
		sendAllParamsOfBoard(boardIndex, mappingData.sets[boardIndex-1], saveAfterSending);
}

void TerpstraMidiDriver::storeAllToEEPROM()
{
	for (int boardIndex = 1; boardIndex <= NUMBEROFBOARDS; boardIndex++)
		storeToEEPROM(boardIndex);
}

void TerpstraMidiDriver::recallAllFromEEPROM()
{
	for (int boardIndex = 1; boardIndex <= NUMBEROFBOARDS; boardIndex++)
		recallFromEEPROM(boardIndex);
}


/*
==============================================================================
Single (mid-level) commands
*/

void TerpstraMidiDriver::sendKeyParam(int boardIndex, int keyIndex, TerpstraKey keyData)
{
	// boardIndex is expected 1-based
	jassert(boardIndex > 0 && boardIndex <= NUMBEROFBOARDS);

	// Channel, note, key type (note on/note off or continuous controller)
	if (keyData.channelNumber >= 0)
		sendSysEx(boardIndex, CHANGE_KEY_NOTE, keyIndex, keyData.noteNumber, keyData.channelNumber - 1, keyData.keyType, '\0');

	// Colour. Values from 0x00 to 0x7f (127 decimal, as the maximal value for data bytes is according to the MIDI standard)
	Colour theColour(keyData.colour);
	sendSysEx(boardIndex, SET_KEY_COLOUR, keyIndex, theColour.getRed() / 2, theColour.getGreen() / 2, theColour.getBlue() / 2, '\0');
}

void TerpstraMidiDriver::storeToEEPROM(int boardIndex)
{
	// boardIndex is expected 1-based
	jassert(boardIndex > 0 && boardIndex <= NUMBEROFBOARDS);

	sendSysEx(boardIndex, STORE_TO_EEPROM, '\0', '\0', '\0', '\0', '\0');
}

void TerpstraMidiDriver::recallFromEEPROM(int boardIndex)
{
	// boardIndex is expected 1-based
	jassert(boardIndex > 0 && boardIndex <= NUMBEROFBOARDS);

	sendSysEx(boardIndex, RECALL_FROM_EEPROM, '\0', '\0', '\0', '\0', '\0');
}

// Send expression pedal sensivity
void TerpstraMidiDriver::sendExpressionPedalSensivity(unsigned char value)
{
	jassert(value <= 0x7f);

	//sendSysEx(0, SET_FOOT_CONTROLLER_SENSITIVITY, value, '\0', '\0', '\0', '\0');
}

// Send parametrization of foot controller
void TerpstraMidiDriver::sendInvertFootController(bool value)
{
	sendSysEx(0, INVERT_FOOT_CONTROLLER, value ? '\1' : '\0', '\0', '\0', '\0', '\0');
}

// Send parametrization of light on key strokes
void TerpstraMidiDriver::sendLightOnKeyStroke(bool value)
{
	sendSysEx(0, SET_LIGHT_ON_KEYSTROKE, value ? '\1' : '\0', '\0', '\0', '\0', '\0');
}

// Send a value for a velocity lookup table
void TerpstraMidiDriver::sendVelocityConfig(TerpstraKey::KEYTYPE keyType, int dwellTick, int velocity)
{
	sendSysEx(0, keyType == TerpstraKey::continuousController ? SET_FADER_CONFIG : SET_VELOCITY_CONFIG, 
		dwellTick, velocity, '\0', '\0', '\0');
}


/*
==============================================================================
Low-level SysEx calls 
*/

void TerpstraMidiDriver::sendSysEx(int boardIndex, unsigned char cmd, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4, unsigned char data5)
{
	// Send only if output device is there
	if (midiOutput != nullptr)
	{
		unsigned char sysExData[10];
		sysExData[0] = MMID1;
		sysExData[1] = MMID2;
		sysExData[2] = MMID3;
		sysExData[3] = boardIndex;
		sysExData[4] = cmd;
		sysExData[5] = data1;
		sysExData[6] = data2;
		sysExData[7] = data3;
		sysExData[8] = data4;
		sysExData[9] = data5;

		MidiMessage msg = MidiMessage::createSysExMessage(sysExData, 10);
		sendMessageDelayed(msg);
	}
}