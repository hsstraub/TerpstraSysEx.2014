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
	midiInputs = MidiInput::getAvailableDevices();
	midiOutputs = MidiOutput::getAvailableDevices();

	deviceManager.initialise(midiInputs.size(), midiOutputs.size(), 0, true, String(), 0);
}

HajuMidiDriver::~HajuMidiDriver()
{
    if (lastInputIndex >= 0)
    {
        deviceManager.removeMidiInputDeviceCallback(midiInputs[lastInputIndex].identifier, lastInputCallback);
    }

	midiOutput = nullptr;
}

StringArray HajuMidiDriver::getMidiInputList() 
{
	juce::StringArray midiInputNames;

	for (auto input : midiInputs)
		midiInputNames.add(input.name);

	return midiInputNames;
}

StringArray HajuMidiDriver::getMidiOutputList() 
{ 
	juce::StringArray midiOutputNames;

	for (auto output : midiOutputs)
		midiOutputNames.add(output.name);

	return midiOutputNames;
}

void HajuMidiDriver::refreshDeviceLists()
{
	midiInputs = MidiInput::getAvailableDevices();
	midiOutputs = MidiOutput::getAvailableDevices();
}

void HajuMidiDriver::setMidiInput(int deviceIndex, MidiInputCallback* callback)
{
    if (lastInputIndex >= 0)
    {
        deviceManager.removeMidiInputDeviceCallback(midiInputs[lastInputIndex].identifier, lastInputCallback);
    }

	auto newInput = midiInputs[deviceIndex];
	if (!deviceManager.isMidiInputDeviceEnabled(newInput.identifier))
		deviceManager.setMidiInputDeviceEnabled(newInput.identifier, true);

	deviceManager.addMidiInputDeviceCallback(newInput.identifier, callback);

	lastInputIndex = deviceIndex;
	lastInputCallback = callback;
}

void HajuMidiDriver::setMidiOutput(int deviceIndex)
{
	jassert(deviceIndex >= 0 && deviceIndex < midiOutputs.size());

	midiOutput = MidiOutput::openDevice(midiOutputs[deviceIndex].identifier);
}

void HajuMidiDriver::sendMessageNow(const MidiMessage& message)
{
	// Send only if output device is there
	if (midiOutput != nullptr)
		midiOutput->sendMessageNow(message);
}

void HajuMidiDriver::sendNoteOnMessage(int noteNumber, int channelNumber, uint8 velocity)
{
	if (channelNumber > 0 && noteNumber >= 0)
		sendMessageNow(MidiMessage::noteOn(channelNumber, noteNumber, velocity));
}

void HajuMidiDriver::sendNoteOffMessage(int noteNumber, int channelNumber, uint8 velocity)
{
	if (channelNumber > 0 && noteNumber >= 0)
		sendMessageNow(MidiMessage::noteOff(channelNumber, noteNumber, velocity));
}
