/*
  ==============================================================================

    HajuDelayMidiDriver.cpp
    Created: 17 Oct 2018 10:30:47pm
    Author:  hsstraub

  ==============================================================================
*/

#include "HajuDelayMidiDriver.h"

HajuDelayMidiDriver::HajuDelayMidiDriver() : HajuMidiDriver()
{
}

// To be done when delay is reached.
void HajuDelayMidiDriver::timerCallback()
{
	// Send first message of buffer, if there is one
	if (!messageBuffer.isEmpty())
	{
		// Send first message of buffer
		sendMessageNow(messageBuffer[0]);

		// Remove message from buffer
		messageBuffer.remove(0);
	}
	else
		// Message buffer is empty. Timer can be stopped.
		stopTimer();
}

// Send a MIDI message, delayed
void HajuDelayMidiDriver::sendMessageDelayed(const MidiMessage& message)
{
	// Do not send directly but add to buffer. Messages in buffer will be sent with a delay between each.
	messageBuffer.add(message);
	
	// Start timer, if it is not running
	if (!isTimerRunning())
		startTimer(delayInMilliseconds);
}