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

void TerpstraMidiDriver::sendAllParamsOfBoard(int boardIndex, TerpstraKeys boardData)
{
	for (int keyIndex = 0; keyIndex < TERPSTRABOARDSIZE; keyIndex++)
		sendKeyParam(boardIndex, keyIndex, boardData.theKeys[keyIndex]);

	//if (saveAfterSending)
	//	storeToEEPROM(boardIndex);
}

void TerpstraMidiDriver::sendCompleteMapping(TerpstraKeyMapping mappingData)
{
	for (int boardIndex = 1; boardIndex <= NUMBEROFBOARDS; boardIndex++)
		sendAllParamsOfBoard(boardIndex, mappingData.sets[boardIndex-1]);
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

// Send expression pedal sensivity
void TerpstraMidiDriver::sendExpressionPedalSensivity(unsigned char value)
{
	jassert(value <= 0x7f);

	sendSysEx(0, SET_FOOT_CONTROLLER_SENSITIVITY, value, '\0', '\0', '\0', '\0');
}

// Send parametrization of foot controller
void TerpstraMidiDriver::sendInvertFootController(bool value)
{
	sendSysEx(0, INVERT_FOOT_CONTROLLER, value ? '\1' : '\0', '\0', '\0', '\0', '\0');
}

// Colour for macro button in active state
void TerpstraMidiDriver::sendMacroButtonActiveColour(String colourAsString)
{
	int colourAsNumber = colourAsString.getHexValue32();
	Colour theColour = Colour(colourAsNumber);
	sendSysEx(0, MACROBUTTON_COLOUR_ON, theColour.getRed() / 2, theColour.getGreen() / 2, theColour.getBlue() / 2, '\0', '\0');
}

// Colour for macro button in inactive state
void TerpstraMidiDriver::sendMacroButtonInactiveColour(String colourAsString)
{
	int colourAsNumber = colourAsString.getHexValue32();
	Colour theColour = Colour(colourAsNumber);
	sendSysEx(0, MACROBUTTON_COLOUR_OFF, theColour.getRed() / 2, theColour.getGreen() / 2, theColour.getBlue() / 2, '\0', '\0');
}


// Send parametrization of light on key strokes
void TerpstraMidiDriver::sendLightOnKeyStroke(bool value)
{
	sendSysEx(0, SET_LIGHT_ON_KEYSTROKE, value ? '\1' : '\0', '\0', '\0', '\0', '\0');
}

// Send a value for a velocity lookup table
void TerpstraMidiDriver::sendVelocityConfig(TerpstraKey::KEYTYPE keyType, unsigned char velocityTable[])
{
	if (midiOutput != nullptr)
	{
		unsigned char sysExData[133];
		sysExData[0] = MMID1;
		sysExData[1] = MMID2;
		sysExData[2] = MMID3;
		sysExData[3] = '\0';
		sysExData[4] = keyType == TerpstraKey::continuousController ? SET_FADER_CONFIG : SET_VELOCITY_CONFIG;

    //memcpy_s is windows specific
//    memcpy_s(&sysExData[5], 128, velocityTable, 128);  // velocityTable is supposed to contain 128 entries. ToDo security?

		memmove(&sysExData[5], velocityTable, 128);

		MidiMessage msg = MidiMessage::createSysExMessage(sysExData, 133);
		sendMessageDelayed(msg);
	}
}

// Save velocity config to EEPROM
void TerpstraMidiDriver::saveVelocityConfig(TerpstraKey::KEYTYPE keyType)
{
	sendSysEx(0, keyType == TerpstraKey::continuousController ? SAVE_FADER_CONFIG : SAVE_VELOCITY_CONFIG,
		'\0', '\0', '\0', '\0', '\0');
}

// reset velocity config to value from EEPROM
void TerpstraMidiDriver::resetVelocityConfig(TerpstraKey::KEYTYPE keyType)
{
	// XXX keyType == TerpstraKey::continuousController ?
	sendSysEx(0, keyType == TerpstraKey::continuousController ? RESET_FADER_CONFIG : RESET_VELOCITY_CONFIG,
		'\0', '\0', '\0', '\0', '\0');
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
