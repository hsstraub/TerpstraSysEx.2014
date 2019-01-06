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
	// XXX open question: does controller need some delay in sending several messages

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
	// XXX open question: does controller need some delay in sending several messages

	for (int boardIndex = 1; boardIndex <= NUMBEROFBOARDS; boardIndex++)
		storeToEEPROM(boardIndex);
}

void TerpstraMidiDriver::recallAllFromEEPROM()
{
	// XXX open question: does controller need some delay in sending several messages

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

	// Colour. Values from 0x00 to 0x33 (51 decimal, 20% of 0xff == 255 decimal)
	Colour theColour(keyData.colour);
	sendSysEx(boardIndex, SET_KEY_COLOUR, keyIndex, theColour.getRed() / 5, theColour.getGreen() / 5, theColour.getBlue() / 5, '\0');
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