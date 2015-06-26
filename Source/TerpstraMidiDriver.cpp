/*
  ==============================================================================

    TerpstraMidiDriver.cpp
    Created: 20 Feb 2015 8:04:02pm
    Author:  hsstraub

  ==============================================================================
*/

#include "TerpstraMidiDriver.h"


TerpstraMidiDriver::TerpstraMidiDriver()
{
	midiInputs = MidiInput::getDevices();
	midiOutputs = MidiOutput::getDevices();

	deviceManager.initialise(midiInputs.size(), midiOutputs.size(), 0, true, String::empty, 0);

	midiOutput = nullptr;
	autoSave = false;
}

TerpstraMidiDriver::~TerpstraMidiDriver()
{
	midiOutput = nullptr;
}

void TerpstraMidiDriver::setMidiOutput(int deviceIndex)
{
	midiOutput = MidiOutput::openDevice(deviceIndex);
}

/*
==============================================================================
Combined (hi-level) commands
*/

void TerpstraMidiDriver::sendAndMaybeSaveKeyParam(int boardIndex, int keyIndex, TerpstraKey keyData)
{
	// Send only if data are not empty
	if (!keyData.isEmpty())
	{
		sendKeyParam(boardIndex, keyIndex, keyData);

		if (autoSave)
			storeToEEPROM(boardIndex);
	}
}

void TerpstraMidiDriver::sendAndSaveAllParamsOfBoard(int boardIndex, TerpstraKeys boardData)
{
	// XXX open question: does controller need some delay in sending several messages

	for (int keyIndex = 0; keyIndex < TERPSTRABOARDSIZE; keyIndex++)
		sendKeyParam(boardIndex, keyIndex, boardData.theKeys[keyIndex]);

	storeToEEPROM(boardIndex);
}

void TerpstraMidiDriver::sendAndSaveCompleteMapping(TerpstraKeyMapping mappingData)
{
	for (int boardIndex = 1; boardIndex <= NUMBEROFBOARDS; boardIndex++)
		sendAndSaveAllParamsOfBoard(boardIndex, mappingData.sets[boardIndex-1]);
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

	// Send only if data are not empty
	if (!keyData.isEmpty())
	{
		sendSysEx(boardIndex, CHANGE_KEY_NOTE, keyIndex, keyData.noteNumber, keyData.channelNumber - 1, '\0', '\0');
	}
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
		midiOutput->sendMessageNow(msg);
	}
}