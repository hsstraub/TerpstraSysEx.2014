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

Array<MidiDeviceInfo> HajuMidiDriver::getMidiInputList()
{
	return midiInputs;
}

Array<MidiDeviceInfo> HajuMidiDriver::getMidiOutputList()
{ 
	return midiOutputs;
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

//============================================================================
// Device detection support

void HajuMidiDriver::openAvailableDevicesForTesting()
{
    closeTestingDevices();
    refreshDeviceLists();

    int i = 0;
    for (auto device : midiOutputs)
        testOutputs.set(i++, MidiOutput::openDevice(device.identifier), true);

    i = 0;
    for (auto device : midiInputs)
        if (testInputs.getUnchecked(i) != nullptr && testInputs.getUnchecked(i)->getIdentifier() != device.identifier)
            testInputs.set(i++, MidiInput::openDevice(device.identifier, this), true);
        else i++;
}

void HajuMidiDriver::sendTestMessageNow(int outputDeviceIndex, const MidiMessage& message)
{
    // Return some error code?
    if (testOutputs.size() > 0 && outputDeviceIndex >= 0 && outputDeviceIndex < testOutputs.size())
    {
        testOutputs.getUnchecked(outputDeviceIndex)->sendMessageNow(message);
    }
}

// Closes all open testing devices; either setMidiInput and setMidiOutput will call this if it's not empty
void HajuMidiDriver::closeTestingDevices()
{
    if (testOutputs.size() > 0)
        testOutputs.clear();

    int errors = 0;
    for (int i = 0; i < testInputs.size(); i++)
    {
        auto input = testInputs[i];
        try
        {
            input->stop();
            input = nullptr;
        }
        catch (int err)
        {
            errors++;
            DBG("Err in closing MIDI Input: " + input->getName());
        }
    }

    try
    {
        testInputs.clear();
    }
    catch (int err)
    {
        errors++;
        DBG("Could not clear test inputs");
    }
}
