/*
  ==============================================================================

    TerpstraMidiDriver.cpp
    Created: 20 Feb 2015 8:04:02pm
    Author:  hsstraub

  ==============================================================================
*/

#include "TerpstraMidiDriver.h"


TerpstraMidiDriver::TerpstraMidiDriver() : HajuMidiDriver()
{
	autoSave = false;
}

TerpstraMidiDriver::~TerpstraMidiDriver()
{
}

void TerpstraMidiDriver::setMidiInput(int deviceIndex)
{
    // For the reaction to answer messages, TerpstraMidiDriver itself must be a MIDI input callback
    HajuMidiDriver::setMidiInput(deviceIndex, this);
}

void TerpstraMidiDriver::addListener(TerpstraMidiDriver::Listener* listener)
{
	listeners.add(listener);
}

void TerpstraMidiDriver::removeListener(TerpstraMidiDriver::Listener* listener)
{
	listeners.remove(listener);
}

void TerpstraMidiDriver::setSysExSendingMode(sysExSendingMode newMode)
{
	if (newMode != currentSysExSendingMode)
	{
		currentSysExSendingMode = newMode;
		if (currentSysExSendingMode == sysExSendingMode::offlineEditor)
		{
			clearMIDIMessageBuffer();	// ToDo remove only SysEx messages (leave NoteOn/NoteOff)?
			stopTimer();
			hasMsgWaitingForAck = false;
		}
	}
}

/*
==============================================================================
Combined (hi-level) commands
*/

void TerpstraMidiDriver::sendAllParamsOfBoard(int boardIndex, TerpstraKeys boardData)
{
	for (int keyIndex = 0; keyIndex < 56; keyIndex++)
		sendKeyParam(boardIndex, keyIndex, boardData.theKeys[keyIndex]);
}

void TerpstraMidiDriver::sendCompleteMapping(TerpstraKeyMapping mappingData)
{
	for (int boardIndex = 1; boardIndex <= NUMBEROFBOARDS; boardIndex++)
		sendAllParamsOfBoard(boardIndex, mappingData.sets[boardIndex-1]);
}

void TerpstraMidiDriver::sendGetMappingOfBoardRequest(int boardIndex)
{
    sendRedLEDConfigurationRequest(boardIndex);
    sendGreenLEDConfigurationRequest(boardIndex);
    sendBlueLEDConfigurationRequest(boardIndex);
    sendChannelConfigurationRequest(boardIndex);
    sendNoteConfigurationRequest(boardIndex);
    sendKeyTypeConfigurationRequest(boardIndex);
}

void TerpstraMidiDriver::sendGetCompleteMappingRequest()
{
	for (int boardIndex = 1; boardIndex <= NUMBEROFBOARDS; boardIndex++)
		sendGetMappingOfBoardRequest(boardIndex);
}

/*
==============================================================================
Single (mid-level) commands
*/

void TerpstraMidiDriver::sendKeyParam(int boardIndex, int keyIndex, TerpstraKey keyData)
{
	// boardIndex is expected 1-based
	jassert(boardIndex > 0 && boardIndex <= NUMBEROFBOARDS);

	// Channel, note, key type (note on/note off or continuous controller)
	if (keyData.channelNumber >= 0)
		sendSysEx(boardIndex, CHANGE_KEY_NOTE, keyIndex, keyData.noteNumber, keyData.channelNumber - 1, keyData.keyType);

	// Colour. Values from 0x00 to 0x7f (127 decimal, as the maximal value for data bytes is according to the MIDI standard)
	Colour theColour(keyData.colour);
	sendSysEx(boardIndex, SET_KEY_COLOUR, keyIndex, theColour.getRed() / 2, theColour.getGreen() / 2, theColour.getBlue() / 2);
}

// Send expression pedal sensivity
void TerpstraMidiDriver::sendExpressionPedalSensivity(unsigned char value)
{
	jassert(value <= 0x7f);

	sendSysEx(0, SET_FOOT_CONTROLLER_SENSITIVITY, value, '\0', '\0', '\0');
}

// Send parametrization of foot controller
void TerpstraMidiDriver::sendInvertFootController(bool value)
{
	sendSysEx(0, INVERT_FOOT_CONTROLLER, value ? '\1' : '\0', '\0', '\0', '\0');
}

// Colour for macro button in active state
void TerpstraMidiDriver::sendMacroButtonActiveColour(String colourAsString)
{
	int colourAsNumber = colourAsString.getHexValue32();
	Colour theColour = Colour(colourAsNumber);
	sendSysEx(0, MACROBUTTON_COLOUR_ON, theColour.getRed() / 2, theColour.getGreen() / 2, theColour.getBlue() / 2, '\0');
}

// Colour for macro button in inactive state
void TerpstraMidiDriver::sendMacroButtonInactiveColour(String colourAsString)
{
	int colourAsNumber = colourAsString.getHexValue32();
	Colour theColour = Colour(colourAsNumber);
	sendSysEx(0, MACROBUTTON_COLOUR_OFF, theColour.getRed() / 2, theColour.getGreen() / 2, theColour.getBlue() / 2, '\0');
}

void TerpstraMidiDriver::sendLightOnKeyStrokes(bool value)
{
	sendSysEx(0, SET_LIGHT_ON_KEYSTROKES, value ? '\1' : '\0', '\0', '\0', '\0');
}


// CMD 08h: Send a value for a velocity lookup table
void TerpstraMidiDriver::sendVelocityConfig(unsigned char velocityTable[])
{
	if (midiOutput != nullptr)
	{
        // Values are in reverse order (shortest ticks count is the highest velocity)
        uint8 reversedTable[128];
        for (uint8 x = 0; x < 128; x++)
        {
            reversedTable[x] = velocityTable[127 - x] & 0x7f;
        }

        auto msg = createTableSysEx(SET_VELOCITY_CONFIG, reversedTable);
		sendMessageNow(msg);
	}
}

// CMD 09h: Save velocity config to EEPROM
void TerpstraMidiDriver::saveVelocityConfig()
{
    sendSysEx(0, SAVE_VELOCITY_CONFIG, '\0', '\0', '\0', '\0');
}

// CMD 0Ah: Reset velocity config to value from EEPROM
void TerpstraMidiDriver::resetVelocityConfig()
{
    sendSysEx(0, RESET_VELOCITY_CONFIG, '\0', '\0', '\0', '\0');
}

// CMD 0Bh: Adjust the internal fader look-up table (128 7-bit values)
void TerpstraMidiDriver::sendFaderConfig(unsigned char faderTable[])
{
	if (midiOutput != nullptr)
	{
        MidiMessage msg = createTableSysEx(SET_FADER_CONFIG, faderTable);
		sendMessageNow(msg);
	}
}

// CMD 0Ch: **DEPRECATED** Save the changes made to the fader look-up table
void TerpstraMidiDriver::saveFaderConfiguration()
{
    sendSysEx(0, SAVE_FADER_CONFIG, '\0', '\0', '\0', '\0');
}

// CMD 0Dh: Reset the fader lookup table back to its factory fader settings.
void TerpstraMidiDriver::resetFaderConfig()
{
    sendSysEx(0, RESET_FADER_CONFIG, '\0', '\0', '\0', '\0');
}

// CMD 0Eh: Enable or disable aftertouch functionality
void TerpstraMidiDriver::sendAfterTouchActivation(bool value)
{
	sendSysEx(0, SET_AFTERTOUCH_FLAG, value ? '\1' : '\0', '\0', '\0', '\0');
}

// CMD 0Fh: Initiate aftertouch calibration routine
void TerpstraMidiDriver::sendCalibrateAfterTouch()
{
	sendSysEx(0, CALIBRATE_AFTERTOUCH, '\0', '\0', '\0', '\0');
}

// CMD 10h: Adjust the internal aftertouch look-up table (size of 128)
void TerpstraMidiDriver::sendAftertouchConfig(unsigned char aftertouchTable[])
{
	if (midiOutput != nullptr)
	{
        MidiMessage msg = createTableSysEx(SET_AFTERTOUCH_CONFIG, aftertouchTable);
		sendMessageNow(msg);
	}
}

// CMD 11h: **DEPRECATED** Save the changes made to the aftertouch look-up table
void TerpstraMidiDriver::saveAftertouchConfig()
{
    sendSysEx(0, SAVE_AFTERTOUCH_CONFIG, '\0', '\0', '\0', '\0');
}

// CMD 12h: Reset the aftertouch lookup table back to its factory aftertouch settings.
void TerpstraMidiDriver::resetAftertouchConfig()
{
    sendSysEx(0, RESET_AFTERTOUCH_CONFIG, '\0', '\0', '\0', '\0');
}


void TerpstraMidiDriver::sendRedLEDConfigurationRequest(int boardIndex)
{
    sendSysEx(boardIndex, GET_RED_LED_CONFIG, '\0', '\0', '\0', '\0');
}

void TerpstraMidiDriver::sendGreenLEDConfigurationRequest(int boardIndex)
{
    sendSysEx(boardIndex, GET_GREEN_LED_CONFIG, '\0', '\0', '\0', '\0');
}

void TerpstraMidiDriver::sendBlueLEDConfigurationRequest(int boardIndex)
{
    sendSysEx(boardIndex, GET_BLUE_LED_CONFIG, '\0', '\0', '\0', '\0');
}

void TerpstraMidiDriver::sendChannelConfigurationRequest(int boardIndex)
{
    sendSysEx(boardIndex, GET_CHANNEL_CONFIG, '\0', '\0', '\0', '\0');
}

void TerpstraMidiDriver::sendNoteConfigurationRequest(int boardIndex)
{
    sendSysEx(boardIndex, GET_NOTE_CONFIG, '\0', '\0', '\0', '\0');
}

void TerpstraMidiDriver::sendKeyTypeConfigurationRequest(int boardIndex)
{
    sendSysEx(boardIndex, GET_KEYTYPE_CONFIG, '\0', '\0', '\0', '\0');

}

// CMD 1Dh: Read back the current velocity look up table of the keyboard.
void TerpstraMidiDriver::sendVelocityConfigRequest()
{
    sendSysEx(0, GET_VELOCITY_CONFIG, '\0', '\0', '\0', '\0');
}

// CMD 1Eh: Read back the current fader look up table of the keyboard.
void TerpstraMidiDriver::sendFaderConfigRequest()
{
    sendSysEx(0, GET_FADER_CONFIG, '\0', '\0', '\0', '\0');
}

// CMD 1Fh: Read back the current aftertouch look up table of the keyboard.
void TerpstraMidiDriver::sendAftertouchConfigRequest()
{
    sendSysEx(0, GET_AFTERTOUCH_CONFIG, '\0', '\0', '\0', '\0');
}

// CMD 20h: Set the velocity interval table, 127 12-bit values
void TerpstraMidiDriver::sendVelocityIntervalConfig(int velocityIntervalTable[])
{
	if (midiOutput != nullptr)
	{
		unsigned char sysExData[259];
        fillManufacturerId(sysExData);
		sysExData[3] = '\0';
        sysExData[4] = SET_VELOCITY_INTERVALS;

        // Interval table contains 127 values!
        for ( int i = 0; i<127; i++)
        {
            sysExData[5 + 2*i] = velocityIntervalTable[i] >> 6;
            sysExData[6 + 2*i] = velocityIntervalTable[i] & 0x3f;
        }

		MidiMessage msg = MidiMessage::createSysExMessage(sysExData, 261);
		sendMessageNow(msg);
	}
}

// CMD 21h: Sead back the velocity interval table
void TerpstraMidiDriver::sendVelocityIntervalConfigRequest()
{
    sendSysEx(0, GET_VELOCITY_INTERVALS, '\0', '\0', '\0', '\0');
}

// CMD 45h: Configure the on/off settings of the sustain pedal
void TerpstraMidiDriver::sendInvertSustainPedal(bool value)
{
    sendSysEx(0, INVERT_SUSTAIN_PEDAL, value ? '\1' : '\0', '\0', '\0', '\0');
}

/*
==============================================================================
Low-level SysEx calls
*/

void TerpstraMidiDriver::fillManufacturerId(unsigned char* data) const
{
    data[0] = (manufacturerId >> 16) & 0xff;
    data[1] = (manufacturerId >> 8) & 0xff;
    data[2] = manufacturerId & 0xff;
}

MidiMessage TerpstraMidiDriver::createTableSysEx(unsigned char cmd, unsigned char table[])
{
    unsigned char sysExData[133];
    fillManufacturerId(sysExData);
    sysExData[3] = '\0';

    sysExData[4] = cmd;

    memmove(&sysExData[5], table, 128);

    auto msg = MidiMessage::createSysExMessage(sysExData, 133);
    return msg;
}

void TerpstraMidiDriver::sendSysEx(int boardIndex, unsigned char cmd, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4)
{
	// Send only if output device is there and SysEx sending is meant to be active
	if (midiOutput != nullptr & currentSysExSendingMode == sysExSendingMode::liveEditor)
	{
		unsigned char sysExData[9];
        fillManufacturerId(sysExData);
		sysExData[3] = boardIndex;
		sysExData[4] = cmd;
		sysExData[5] = data1;
		sysExData[6] = data2;
		sysExData[7] = data3;
		sysExData[8] = data4;

		MidiMessage msg = MidiMessage::createSysExMessage(sysExData, 9);
		sendMessageWithAcknowledge(msg);
	}
}

bool TerpstraMidiDriver::messageIsResponseToMessage(const MidiMessage& answer, const MidiMessage& originalMessage)
{
    // Only for SysEx messages
    if (answer.isSysEx() != originalMessage.isSysEx())
        return false;

    auto answerSysExData = answer.getSysExData();
    auto originalSysExData = originalMessage.getSysExData();

    // Manufacturer Id, board index, command coincide?
    if (answerSysExData[0] != originalSysExData[0] ||
        answerSysExData[1] != originalSysExData[1] ||
        answerSysExData[2] != originalSysExData[2] ||
        answerSysExData[3] != originalSysExData[3] ||
        answerSysExData[4] != originalSysExData[4])
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool TerpstraMidiDriver::messageIsTerpstraSysExMessage(const MidiMessage& midiMessage)
{
    if (!midiMessage.isSysEx())
        return false;

    auto sysExData = midiMessage.getSysExData();

    return midiMessage.getSysExDataSize() >= 3 &&
        (sysExData[0] == (manufacturerId >> 16) & 0xff) &&
        (sysExData[1] == (manufacturerId >> 8) & 0xff) &&
		(sysExData[2] == manufacturerId & 0xff);
}

bool TerpstraMidiDriver::messageIsTerpstraConfigurationDataReceptionMessage(const MidiMessage& midiMessage)
{
    if (!messageIsTerpstraSysExMessage(midiMessage))
        return false;

    // sysExData, positions 0-2: manufacturer Id. position 3: board index.
    auto midiCmd = midiMessage.getSysExData()[4];

    return
        midiCmd == GET_RED_LED_CONFIG ||
        midiCmd == GET_GREEN_LED_CONFIG ||
        midiCmd == GET_BLUE_LED_CONFIG ||
        midiCmd == GET_CHANNEL_CONFIG ||
        midiCmd == GET_NOTE_CONFIG ||
        midiCmd == GET_KEYTYPE_CONFIG;
}

bool TerpstraMidiDriver::messageIsTerpstraVelocityConfigReceptionMessage(const MidiMessage& midiMessage, TerpstraVelocityCurveConfig::VelocityCurveType velocityCurveType)
{
    if (!messageIsTerpstraSysExMessage(midiMessage))
        return false;

    // sysExData, positions 0-2: manufacturer Id. position 3: board index.
    auto midiCmd = midiMessage.getSysExData()[4];

    return
        (velocityCurveType == TerpstraVelocityCurveConfig::VelocityCurveType::noteOnNoteOff && midiCmd == GET_VELOCITY_CONFIG) ||
        (velocityCurveType == TerpstraVelocityCurveConfig::VelocityCurveType::fader && midiCmd == GET_FADER_CONFIG) ||
        (velocityCurveType == TerpstraVelocityCurveConfig::VelocityCurveType::afterTouch && midiCmd == GET_AFTERTOUCH_CONFIG);
}

bool TerpstraMidiDriver::messageIsVelocityIntervalConfigReceptionMessage(const MidiMessage& midiMessage)
{
    if (!messageIsTerpstraSysExMessage(midiMessage))
        return false;

    // sysExData, positions 0-2: manufacturer Id. position 3: board index.
    auto midiCmd = midiMessage.getSysExData()[4];

    return midiCmd == GET_VELOCITY_INTERVALS;
}

void TerpstraMidiDriver::sendMessageWithAcknowledge(const MidiMessage& message)
{
    // If there is no MIDI input port active: just send, without expecting acknowledge
	// ToDo Or do nothing?
    if ( lastInputCallback == nullptr)
    {
        sendMessageNow(message);

	    // Notify listeners
		const MessageManagerLock mmLock;
		this->listeners.call(&Listener::midiMessageSent, message);
    }
    else
    {
        // Add message to queue first. The oldest message in queue will be sent.
		{
			messageBuffer.add(message);
			const MessageManagerLock mmLock;
			this->listeners.call(&Listener::midiSendQueueSize, messageBuffer.size());
		}

        // If there is no message waiting for acknowledge: send oldest message of queue
       	if (!isTimerRunning())
        {
            sendOldestMessageInQueue();
        }
    }
}

void TerpstraMidiDriver::sendOldestMessageInQueue()
{
    if (!messageBuffer.isEmpty())
    {
        jassert(!isTimerRunning());
        jassert(!hasMsgWaitingForAck);

        currentMsgWaitingForAck = messageBuffer[0];     // oldest element in buffer
        hasMsgWaitingForAck = true;
		messageBuffer.remove(0);                        // remove from buffer
		{
			const MessageManagerLock mmLock;
			this->listeners.call(&Listener::midiSendQueueSize, messageBuffer.size());
		}

        sendCurrentMessage();
    }
}

void TerpstraMidiDriver::sendCurrentMessage()
{
    jassert(!isTimerRunning());
    jassert(hasMsgWaitingForAck);

    sendMessageNow(currentMsgWaitingForAck);        // send it

    // Notify listeners
	{
		const MessageManagerLock mmLock;
		this->listeners.call(&Listener::midiMessageSent, currentMsgWaitingForAck);
	}

    timerType = waitForAnswer;
    startTimer(receiveTimeoutInMilliseconds);       // Start waiting for answer
}

void TerpstraMidiDriver::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message)
{
    // Notify listeners
	{
		const MessageManagerLock mmLock;
		this->listeners.call(&Listener::midiMessageReceived, message);
	}

    // Check whether received message is an answer to the previously sent one
    if (hasMsgWaitingForAck && messageIsResponseToMessage(message, currentMsgWaitingForAck))
    {
        jassert(timerType == waitForAnswer);

        // Answer has come, we can stop the timer
        stopTimer();

        // Check answer state (error yes/no)
        auto answerState = message.getSysExData()[5];
        // if answer state is "busy": resend message after a little delay
        if ( answerState == TerpstraMIDIAnswerReturnCode::BUSY)
        {
            // Start delay timer, after which message will be sent again
            timerType = delayWhileDeviceBusy;
            startTimer(busyTimeDelayInMilliseconds);
        }
        else
        {
            // In case of error, NACK: ?
            // For now: Remove from buffer in any case
            hasMsgWaitingForAck = false;

            // If there are more messages waiting in the queue: send the next one
            sendOldestMessageInQueue();
        }
    }

    // Other incoming messages are ignored
}

void TerpstraMidiDriver::timerCallback()
{
    stopTimer();

    if (timerType == waitForAnswer)
    {
        // No answer came from MIDI input
		{
			const MessageManagerLock mmLock;
			this->listeners.call(&Listener::generalLogMessage, "No answer from device", HajuErrorVisualizer::ErrorLevel::error);
		}

        // For now: Remove from buffer, try to send next one
        hasMsgWaitingForAck = false;
        sendOldestMessageInQueue();
    }
    else if (timerType == delayWhileDeviceBusy)
    {
        // Resend current message and start waiting for answer again
        sendCurrentMessage();
    }
    else
        jassertfalse;
}
