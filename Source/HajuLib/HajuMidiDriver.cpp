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

	midiOutput = nullptr;
}

HajuMidiDriver::~HajuMidiDriver()
{
	midiOutput = nullptr;
}

void HajuMidiDriver::setMidiOutput(int deviceIndex)
{
	midiOutput = MidiOutput::openDevice(deviceIndex);
}
