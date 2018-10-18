/*
  ==============================================================================

    HajuDelayMidiDriver.h
    Created: 17 Oct 2018 10:30:47pm
    Author:  hsstraub

  ==============================================================================
*/

#ifndef HAJUDELAYMIDIDRIVER_H_INCLUDED
#define HAJUDELAYMIDIDRIVER_H_INCLUDED

#include "HajuMidiDriver.h"

// Midi driver with simple mechanism to send MIDI messages with a delay
class HajuDelayMidiDriver : public HajuMidiDriver, public Timer
{
public:
	HajuDelayMidiDriver();

	void timerCallback() override;

	void sendMessageDelayed(const MidiMessage& message);

	// Attributes
protected:
	Array<MidiMessage> messageBuffer;

	int delayInMilliseconds = 5;
};

#endif  // HAJUDELAYMIDIDRIVER_H_INCLUDED
