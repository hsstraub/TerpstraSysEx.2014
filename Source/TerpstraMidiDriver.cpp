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
    if (lastInputIndex >= 0)
        deviceManager.removeMidiInputDeviceCallback(midiInputs[lastInputIndex], this);
}

void TerpstraMidiDriver::setMidiInput(int deviceIndex, MidiInputCallback* callback)
{
    HajuMidiDriver::setMidiInput(deviceIndex, callback);

    // For the reaction to answer messages, TerpstraMidiDriver itself must be a MIDI input callback
    deviceManager.addMidiInputDeviceCallback(midiInputs[deviceIndex], this);
}

/*
==============================================================================
Combined (hi-level) commands
*/

void TerpstraMidiDriver::sendAllParamsOfBoard(int boardIndex, TerpstraKeys boardData)
{
	for (int keyIndex = 0; keyIndex < TERPSTRABOARDSIZE; keyIndex++)
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


// Send a value for a velocity lookup table
void TerpstraMidiDriver::sendVelocityConfig(TerpstraMidiDriver::VelocityCurveType velocityCurveType, unsigned char velocityTable[])
{
	if (midiOutput != nullptr)
	{
		unsigned char sysExData[133];
		sysExData[0] = (manufacturerId >> 16) & 0xff;
		sysExData[1] = (manufacturerId >> 8) & 0xff;
		sysExData[2] = manufacturerId & 0xff;
		sysExData[3] = '\0';

		switch(velocityCurveType)
		{
		    case TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff:
                sysExData[4] = SET_VELOCITY_CONFIG;
                break;
            case TerpstraMidiDriver::VelocityCurveType::fader:
                sysExData[4] = SET_FADER_CONFIG;
                break;
            case TerpstraMidiDriver::VelocityCurveType::afterTouch:
                sysExData[4] = SET_AFTERTOUCH_CONFIG;
                break;
            default:
                jassert(false);
                break;
		}

    //memcpy_s is windows specific
//    memcpy_s(&sysExData[5], 128, velocityTable, 128);  // velocityTable is supposed to contain 128 entries. ToDo security?

		memmove(&sysExData[5], velocityTable, 128);

		MidiMessage msg = MidiMessage::createSysExMessage(sysExData, 133);
		sendMessageNow(msg);
	}
}

// Save velocity config to EEPROM
void TerpstraMidiDriver::saveVelocityConfig(TerpstraMidiDriver::VelocityCurveType velocityCurveType)
{
    switch(velocityCurveType)
    {
        case TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff:
            sendSysEx(0, SAVE_VELOCITY_CONFIG, '\0', '\0', '\0', '\0');
            break;
        case TerpstraMidiDriver::VelocityCurveType::fader:
            sendSysEx(0, SAVE_FADER_CONFIG, '\0', '\0', '\0', '\0');
            break;
        case TerpstraMidiDriver::VelocityCurveType::afterTouch:
            sendSysEx(0, SAVE_AFTERTOUCH_CONFIG, '\0', '\0', '\0', '\0');
            break;
        default:
            jassert(false);
            break;
    }
}

// reset velocity config to value from EEPROM
void TerpstraMidiDriver::resetVelocityConfig(TerpstraMidiDriver::VelocityCurveType velocityCurveType)
{
    switch(velocityCurveType)
    {
        case TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff:
            sendSysEx(0, RESET_VELOCITY_CONFIG, '\0', '\0', '\0', '\0');
            break;
        case TerpstraMidiDriver::VelocityCurveType::fader:
            sendSysEx(0, RESET_FADER_CONFIG, '\0', '\0', '\0', '\0');
            break;
        case TerpstraMidiDriver::VelocityCurveType::afterTouch:
            sendSysEx(0, RESET_AFTERTOCUH_CONFIG, '\0', '\0', '\0', '\0');
            break;
        default:
            jassert(false);
            break;
    }
}

// Enable or disable aftertouch functionality
void TerpstraMidiDriver::sendAfterTouchActivation(bool value)
{
	sendSysEx(0, SET_AFTERTOUCH_FLAG, value ? '\1' : '\0', '\0', '\0', '\0');
}

// Initiate aftertouch calibration routine
void TerpstraMidiDriver::sendCalibrateAfterTouch()
{
	sendSysEx(0, CALIBRATE_AFTERTOUCH, '\0', '\0', '\0', '\0');
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

void TerpstraMidiDriver::sendVelocityConfigurationRequest(VelocityCurveType velocityCurveType)
{
    switch(velocityCurveType)
    {
        case TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff:
            sendSysEx(0, GET_VELOCITY_CONFIG, '\0', '\0', '\0', '\0');
            break;
        case TerpstraMidiDriver::VelocityCurveType::fader:
            sendSysEx(0, GET_FADER_CONFIG, '\0', '\0', '\0', '\0');
            break;
        case TerpstraMidiDriver::VelocityCurveType::afterTouch:
            sendSysEx(0, GET_AFTERTOUCH_CONFIG, '\0', '\0', '\0', '\0');
            break;
        default:
            jassert(false);
            break;
    }
}
/*
==============================================================================
Low-level SysEx calls
*/

void TerpstraMidiDriver::sendSysEx(int boardIndex, unsigned char cmd, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4)
{
	// Send only if output device is there
	if (midiOutput != nullptr)
	{
		unsigned char sysExData[9];
		sysExData[0] = (manufacturerId >> 16) & 0xff;
		sysExData[1] = (manufacturerId >> 8) & 0xff;
		sysExData[2] = manufacturerId & 0xff;
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

void TerpstraMidiDriver::sendMessageWithAcknowledge(const MidiMessage& message)
{
    // If there is no MIDI input port active: just send, without expecting acknowledge
    if ( lastInputCallback == nullptr)
    {
        sendMessageNow(message);
	    writeLog(message, MIDISendDirection::sent);
    }
    else
    {
        // Add message to queue first. The oldest message in queue will be sent.
        messageBuffer.add(message);

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

        sendCurrentMessage();
    }
}

void TerpstraMidiDriver::sendCurrentMessage()
{
    jassert(!isTimerRunning());
    jassert(hasMsgWaitingForAck);

    sendMessageNow(currentMsgWaitingForAck);        // send it
    writeLog(currentMsgWaitingForAck, MIDISendDirection::sent);

    timerType = waitForAnswer;
    startTimer(receiveTimeoutInMilliseconds);       // Start waiting for answer
}

void TerpstraMidiDriver::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message)
{
    writeLog(message, MIDISendDirection::received);

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
        writeLog("No answer from device", HajuErrorVisualizer::ErrorLevel::error);

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

void TerpstraMidiDriver::writeLog(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel)
{
    // ToDo write to a specialized log area

    // Ad hoc: display a popup if message is an error
    if ( errorLevel != HajuErrorVisualizer::ErrorLevel::noError)
    {
        AlertWindow::showNativeDialogBox(
            errorLevel == HajuErrorVisualizer::ErrorLevel::error ? "Error" : "Warning",
            textMessage,
            false);
    }
}

void TerpstraMidiDriver::writeLog(const MidiMessage& midiMessage, MIDISendDirection sendDirection)
{
    // If midiMessage is an answer: error level according to answer state
    // Currently done by MainComponent
    /*
    if ( sendDirection == MIDISendDirection::received && midiMessage.isSysEx() && midiMessage.getSysExDataSize() > 5)
    {
        auto answerState = midiMessage.getSysExData()[5];

        switch(answerState)
        {
        case TerpstraMIDIAnswerReturnCode::NACK:  // Not recognized
            writeLog("<< Not Recognized: " + midiMessage.getDescription(), HajuErrorVisualizer::ErrorLevel::error);
            break;

        case TerpstraMIDIAnswerReturnCode::ACK:  // Acknowledged, OK
            writeLog("<< Ack: " + midiMessage.getDescription(), HajuErrorVisualizer::ErrorLevel::noError);
            break;

        case TerpstraMIDIAnswerReturnCode::BUSY: // Controller busy
            writeLog("<< Busy: " + midiMessage.getDescription(), HajuErrorVisualizer::ErrorLevel::warning);
            break;

        case TerpstraMIDIAnswerReturnCode::ERROR:    // Error
            writeLog("<< Error: " + midiMessage.getDescription(), HajuErrorVisualizer::ErrorLevel::error);
            break;

        default:
            writeLog("<< " + midiMessage.getDescription(), HajuErrorVisualizer::ErrorLevel::noError);
            break;
        }
    }
    else
    {
        writeLog(
            sendDirection == MIDISendDirection::received ? "<< " : ">> " + midiMessage.getDescription(),
            HajuErrorVisualizer::ErrorLevel::noError);
    }
    */
}
