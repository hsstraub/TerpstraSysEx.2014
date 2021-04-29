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
    if (testInputs.size() > 0)
    {
        int swapWith = -1;
        if (testInputs[deviceIndex] != nullptr)
        {
            if (midiInputs[deviceIndex].identifier == testInputs[deviceIndex]->getIdentifier())
                swapWith = deviceIndex;
        }
        else 
        {
            for (int i = 0; i < testInputs.size(); i++)
                if (testInputs[i]->getIdentifier() == midiInputs[deviceIndex].identifier)
                {
                    swapWith = i;
                    break;
                }
        }

        if (swapWith >= 0)
        {
            closeMidiInput();
            midiInput = testInputs[deviceIndex];
            lastInputIndex = swapWith;
        }

        // Clear all but confirmed input
        while (testInputs.size() > 0)
        {
            if (testInputs[0] != midiInput)
            {
                testInputs.remove(0);
            }
            else if (testInputs.size() == 1)
                break;
            else
                testInputs.swap(0, testInputs.size() - 1);
        }
    }

    // Bug if new input inserted; extra instantiation
    else if (lastInputIndex != deviceIndex)
    {
        selectedInput = MidiInput::openDevice(midiInputs[deviceIndex].identifier, this);
        midiInput = selectedInput.get();
        midiInput->start();
        lastInputIndex = deviceIndex;
    }
}

void HajuMidiDriver::setMidiOutput(int deviceIndex)
{
    jassert(deviceIndex >= 0 && deviceIndex < midiOutputs.size());
    if (testOutputs.size() > 0)
    {
        int swapWith = -1;

        if (testOutputs[deviceIndex] != nullptr)
        {
            if (testOutputs[deviceIndex]->getIdentifier() == midiOutput->getIdentifier())
                swapWith = deviceIndex;
        }
        else
        {
            for (int i = 0; i < testOutputs.size(); i++)
            {
                auto id = midiOutput->getIdentifier();
                if (testOutputs[i]->getIdentifier() == id)
                {
                    swapWith = i;
                    break;
                }

            }
        }

        if (swapWith > 0)
        {
            closeMidiOutput();
            midiOutput = testOutputs[deviceIndex];
            lastOutputIndex = deviceIndex;
        }

        while (testOutputs.size() > 0)
        {
            if (testOutputs[0] != midiOutput)
            {
                testOutputs.remove(0);
            }
            else if (testOutputs.size() == 1)
                break;
            else
                testOutputs.swap(0, testOutputs.size() - 1);
        }
    }

    // Bug if new input inserted; extra instantiation
    else if (lastOutputIndex != deviceIndex)
    {
        selectedOutput = MidiOutput::openDevice(midiOutputs[deviceIndex].identifier);

        if (selectedOutput == nullptr)
            DBG("WARNING: Could not open up output device " + midiOutputs[deviceIndex].identifier);

        else if (selectedOutput.get() != midiOutput)
        {
            midiOutput = selectedOutput.get();
            lastOutputIndex = deviceIndex;
        }
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
