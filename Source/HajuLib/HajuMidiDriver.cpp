/*
  ==============================================================================

    HajuMidiDriver.cpp
    Created: 23 Feb 2018 11:30:58pm
    Author:  hsstraub

  ==============================================================================
*/

#include "HajuMidiDriver.h"


HajuMidiDriver::HajuMidiDriver()
{
	midiInputs = MidiInput::getDevices();
	midiOutputs = MidiOutput::getDevices();

	deviceManager.initialise(midiInputs.size(), midiOutputs.size(), 0, true, String::empty, 0);

	lastInputIndex = -1;
	lastInputCallback = nullptr;
	midiOutput = nullptr;
}

HajuMidiDriver::~HajuMidiDriver()
{
	deviceManager.removeMidiInputCallback(midiInputs[lastInputIndex], lastInputCallback);
	midiOutput = nullptr;
}

void HajuMidiDriver::setMidiInput(int deviceIndex, MidiInputCallback* callback)
{
	deviceManager.removeMidiInputCallback(midiInputs[lastInputIndex], lastInputCallback);

	auto newInput = midiInputs[deviceIndex];
	if (!deviceManager.isMidiInputEnabled(newInput))
		deviceManager.setMidiInputEnabled(newInput, true);
	deviceManager.addMidiInputCallback(newInput, callback);

	lastInputIndex = deviceIndex;
	lastInputCallback = callback;
}

void HajuMidiDriver::setMidiOutput(int deviceIndex)
{
	midiOutput = MidiOutput::openDevice(deviceIndex);
}

void HajuMidiDriver::sendMessageNow(const MidiMessage& message)
{
	// Send only if output device is there
	if (midiOutput != nullptr)
		midiOutput->sendMessageNow(message);
}
