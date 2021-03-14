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
}

HajuMidiDriver::~HajuMidiDriver()
{
    if (midiInput.get() != nullptr)
    {
        midiInput->stop();
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

MidiDeviceInfo HajuMidiDriver::getMidiInputInfo() const
{
    return midiInputs[lastInputIndex];
}

MidiDeviceInfo HajuMidiDriver::getMidiOutputInfo() const
{
    if (midiOutput.get())
        return midiOutput->getDeviceInfo();
    return MidiDeviceInfo();
}

void HajuMidiDriver::refreshDeviceLists()
{
    // Reset last opened device to be the same index
    // so we close the correct one if that's requested
    // order change is probably rare but can't be guaranteed
    
    if (lastInputIndex >= 0)
    {
        String lastInputId = midiInputs[lastInputIndex].identifier;
        midiInputs = MidiInput::getAvailableDevices();

        for (int i = 0; i < midiInputs.size(); i++)
            if (midiInputs[i].identifier == lastInputId)
                lastInputIndex = i;
    }
    else
        midiInputs = MidiInput::getAvailableDevices();
    
    if (lastOutputIndex >= 0)
    {
        String lastOutputId = midiOutputs[lastOutputIndex].identifier;
        midiOutputs = MidiOutput::getAvailableDevices();
        
        for (int i = 0; i < midiOutputs.size(); i++)
            if (midiOutputs[i].identifier == lastOutputId)
                lastOutputIndex = i;
    }
    else
        midiOutputs = MidiOutput::getAvailableDevices();
}

void HajuMidiDriver::setMidiInput(int deviceIndex)
{
    if (midiInput.get())
    {
        midiInput->stop();
    }

    auto newInput = MidiInput::openDevice(midiInputs[deviceIndex].identifier, this);
	midiInput.swap(newInput);
    midiInput->start();
    
    lastInputIndex = deviceIndex;
}

void HajuMidiDriver::setMidiOutput(int deviceIndex)
{
	jassert(deviceIndex >= 0 && deviceIndex < midiOutputs.size());
    
    auto newOutput = MidiOutput::openDevice(midiOutputs[deviceIndex].identifier);
    
    if (newOutput == nullptr)
        DBG("WARNING: Could not open up output device " + midiOutputs[deviceIndex].identifier);
    
    else if (newOutput != midiOutput)
    {
        midiOutput.swap(newOutput);
        lastOutputIndex = deviceIndex;
    }
}

void HajuMidiDriver::sendMessageNow(const MidiMessage& message)
{
	// Send only if output device is there
	if (midiOutput != nullptr)
    {
		midiOutput->sendMessageNow(message);
    }
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

void HajuMidiDriver::closeMidiInput()
{
    if (midiInput.get())
    {
        midiInput->stop();
        midiInput = nullptr;

        lastInputIndex = -1;
    }
}

void HajuMidiDriver::closeMidiOutput()
{
    midiOutput = nullptr;
    lastOutputIndex = -1;
}
