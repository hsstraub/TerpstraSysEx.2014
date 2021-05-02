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
    midiInput = nullptr;
    midiOutput = nullptr;
}

HajuMidiDriver::~HajuMidiDriver()
{
    if (midiInput != nullptr)
    {
        midiInput->stop();
        midiInput = nullptr;
    }

	midiOutput = nullptr;
}

const Array<MidiDeviceInfo>& HajuMidiDriver::getMidiInputList()
{
	return midiInputs;
}

const Array<MidiDeviceInfo>& HajuMidiDriver::getMidiOutputList()
{ 
	return midiOutputs;
}

MidiDeviceInfo HajuMidiDriver::getMidiInputInfo() const
{
    return midiInputs[lastInputIndex];
}

MidiDeviceInfo HajuMidiDriver::getMidiOutputInfo() const
{
    if (midiOutput != nullptr)
        return midiOutput->getDeviceInfo();
    return MidiDeviceInfo();
}

bool HajuMidiDriver::refreshDeviceLists()
{
    bool inputsChanged = false;
    auto newInputs = MidiInput::getAvailableDevices();
    if (newInputs.size() == midiInputs.size())
    {
        for (int i = 0; i < newInputs.size(); i++)
            if (newInputs[i] != midiInputs[i])
                inputsChanged = true;
    }
    else
        inputsChanged = true;

    // Reset last opened device to be the same index
    // so we close the correct one if that's requested
    // order change is probably rare but can't be guaranteed

    if (inputsChanged)
    {
        if (lastInputIndex >= 0)
        {
            String lastInputId = midiInputs[lastInputIndex].identifier;

            for (int i = 0; i < newInputs.size(); i++)
                if (newInputs[i].identifier == lastInputId)
                    lastInputIndex = i;
        }
        
        midiInputs = newInputs;
    }

    bool outputsChanged = false;
    auto newOutputs = MidiOutput::getAvailableDevices();
    if (newOutputs.size() == midiOutputs.size())
    {
        for (int i = 0; i < newOutputs.size(); i++)
            if (newOutputs[i] != midiOutputs[i])
                outputsChanged = true;
    }
    else
        outputsChanged = true;

    if (outputsChanged)
    {
        if (lastOutputIndex >= 0)
        {
            String lastOutputId = midiOutputs[lastOutputIndex].identifier;

            for (int i = 0; i < newOutputs.size(); i++)
                if (newOutputs[i].identifier == lastOutputId)
                    lastOutputIndex = i;
        }
        
        midiOutputs = newOutputs;
    }

    return inputsChanged || outputsChanged;
}

void HajuMidiDriver::setMidiInput(int deviceIndex)
{
    jassert(deviceIndex >= 0 && deviceIndex < midiInputs.size());

    testInputs.clear();

    if (midiInput != nullptr)
    {
        midiInput->stop();
    }

    selectedInput = MidiInput::openDevice(midiInputs[deviceIndex].identifier, this);

    if (selectedInput != nullptr)
    {
        midiInput = selectedInput.get();
        midiInput->start();

        lastInputIndex = deviceIndex;
    }
    else
    {
        DBG("WARNING: Could not open up input device " + String(deviceIndex));
    }
}

void HajuMidiDriver::setMidiOutput(int deviceIndex)
{
    jassert(deviceIndex >= 0 && deviceIndex < midiOutputs.size());

    testOutputs.clear();

    selectedOutput = MidiOutput::openDevice(midiOutputs[deviceIndex].identifier);

    if (selectedOutput != nullptr)
    {
        midiOutput = selectedOutput.get();
        lastOutputIndex = deviceIndex;
    }
    else
    {
        DBG("WARNING: Could not open up output device " + String(deviceIndex));
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
    if (midiInput != nullptr)
    {
        midiInput->stop();
        midiInput = nullptr;

        lastInputIndex = -1;
    }

    if (selectedInput.get() != nullptr)
    {
        selectedInput = nullptr;
    }
}

void HajuMidiDriver::closeMidiOutput()
{
    if (midiOutput != nullptr)
    {
        midiOutput = nullptr;
        lastOutputIndex = -1;
    }

    if (selectedOutput.get() != nullptr)
    {
        selectedOutput = nullptr;
    }
}

//============================================================================
// Device detection support

void HajuMidiDriver::openAvailableDevicesForTesting()
{
    // May be preferable to only close and open input devices when necessary
    closeTestingDevices();

    for (auto device : midiOutputs)
    {
        auto newOutput = testOutputs.add(MidiOutput::openDevice(device.identifier));
        if (newOutput == nullptr)
            testOutputs.removeObject(newOutput);
    }

    for (auto device : midiInputs)
    {
        auto newInput = testInputs.add(MidiInput::openDevice(device.identifier, this));
        if (newInput != nullptr)
            newInput->start();
        else
            testInputs.removeObject(newInput);
    }
}

void HajuMidiDriver::sendTestMessageNow(int outputDeviceIndex, const MidiMessage& message)
{
    // Return some error code?
    if (outputDeviceIndex >= 0 && outputDeviceIndex < testOutputs.size())
    {
        DBG("Sending test message to " + testOutputs[outputDeviceIndex]->getName());
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
        if (input != nullptr)
        {
            input->stop();
            input = nullptr;
        }
    }

    testInputs.clear();
}
