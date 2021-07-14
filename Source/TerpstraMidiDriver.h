/*
  ==============================================================================

    TerpstraMidiDriver.h
    Created: 20 Feb 2015 8:04:02pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "HajuLib/HajuMidiDriver.h"
#include "HajuLib/HajuErrorVisualizer.h"
#include "LumatoneFirmwareDefinitions.h"
//[/Headers]

/*
==============================================================================
Connection to midi, sending SysEx parameters to keyboard
==============================================================================
*/
class TerpstraMidiDriver : public HajuMidiDriver, public MidiInputCallback, public Timer
{
    // Types
public:
	// Listener class, to notify changes
	class Listener
	{
	public:
		// Destructor
		virtual ~Listener() {}

		virtual void midiMessageReceived(MidiInput* source, const MidiMessage& midiMessage) = 0;
		virtual void midiMessageSent(const MidiMessage& midiMessage) = 0;
		virtual void midiSendQueueSize(int queueSize) = 0;
        virtual void generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel) = 0;
		virtual void noAnswerToMessage(const MidiMessage& midiMessage) = 0;
	};

private:
    typedef enum
    {
        waitForAnswer,
        delayWhileDeviceBusy
    } TimerType;

public:
	TerpstraMidiDriver();
	~TerpstraMidiDriver();

	void addListener(Listener* listenerToAdd);
	void removeListener(Listener* listenerToRemove);

	void restrictToTestMessages(bool testMessagesOnly) { sendTestMessagesOnly = testMessagesOnly; }

	//============================================================================
	// Single (mid-level) commands, firmware specific

	// CMD 00h: Send a single key's functionctional configuration
	void sendKeyFunctionParameters(uint8 boardIndex, uint8 keyIndex, uint8 noteOrCCNum, uint8 midiChannel, uint8 keyType, bool faderUpIsNull = true);

	// CMD 01h: Send a single key's LED channel intensities, three 8-bit values
	void sendKeyLightParameters(uint8 boardIndex, uint8 keyIndex, uint8 red, uint8 green, uint8 blue);
	// CMD 01h: Send a single key's LED channel intensities, three pairs of 4-bit values for each channel
	void sendKeyLightParameters(uint8 boardIndex, uint8 keyIndex, uint8 redUpper, uint8 redLower, uint8 greenUpper, uint8 greenLower, uint8 blueUpper, uint8 blueLower);
	// CMD 01h: Send a single key's LED channel intensities, three 7-bit values
	void sendKeyLightParameters_Version_1_0_0(uint8 boardIndex, uint8 keyIndex, uint8 red, uint8 green, uint8 blue);

	// CMD 02h: Save current configuration to specified preset index
	void saveProgram(uint8 presetNumber);

	// CMD 03h: Send expression pedal sensivity
	void sendExpressionPedalSensivity(uint8 value);

	// CMD 04h: Send parametrization of foot controller
	void sendInvertFootController(bool value);

	// CMD 05h: Colour for macro button in active state, 3 8-bit values
	void sendMacroButtonActiveColour(uint8 red, uint8 green, uint8 blue);
	// CMD 05h: Colour for macro button in active state, 3 pairs for 4-bit values for each LED channel
	void sendMacroButtonActiveColour(uint8 redUpper, uint8 redLower, uint8 greenUpper, uint8 greenLower, uint8 blueUpper, uint8 blueLower);
	// CMD 05h: Colour for macro button in active state, 3 7-bit values
	void sendMacroButtonActiveColour_Version_1_0_0(uint8 red, uint8 green, uint8 blue);

	// CMD 06h: Colour for macro button in inactive state
	void sendMacroButtonInactiveColour(int red, int green, int blue);
	// CMD 05h: Colour for macro button in active state, 3 pairs for 4-bit values for each LED channel
	void sendMacroButtonInactiveColour(uint8 redUpper, uint8 redLower, uint8 greenUpper, uint8 greenLower, uint8 blueUpper, uint8 blueLower);
	// CMD 05h: Colour for macro button in active state, 3 7-bit values
	void sendMacroButtonInactiveColour_Version_1_0_0(uint8 red, uint8 green, uint8 blue);

	// CMD 07h: Send parametrization of light on keystrokes
	void sendLightOnKeyStrokes(bool value);

	// CMD 08h: Send a value for a velocity lookup table (128 7-bit values)
	void sendVelocityConfig(const uint8 velocityTable[]);

	// CMD 09h: Save velocity config to EEPROM
	void saveVelocityConfig();

	// CMD 0Ah: Reset velocity config to value from EEPROM
	void resetVelocityConfig();

	// CMD 0Bh: Adjust the internal fader look-up table (size of 128)
	void sendFaderConfig(const uint8 faderTable[]);

	// CMD 0Ch: **DEPRECATED** Save the changes made to the fader look-up table
	void saveFaderConfiguration();

	// CMD 0Dh: Reset the fader lookup table back to its factory fader settings.
	void resetFaderConfig();

	// CMD 0Eh: Enable or disable aftertouch functionality
	void sendAfterTouchActivation(bool value);

	// CMD 0Fh: Initiate aftertouch calibration routine
	void sendCalibrateAfterTouch();

	// CMD 10h: Adjust the internal aftertouch look-up table (size of 128)
	void sendAftertouchConfig(const uint8 aftertouchTable[]);

	// CMD 11h: **DEPRECATED** Save the changes made to the aftertouch look-up table
	void saveAftertouchConfig();

	// CMD 12h: Reset the aftertouch lookup table back to its factory aftertouch settings.
	void resetAftertouchConfig();

	// CMD 13h: Read back the current red intensity of all the keys of the target board.
	void sendRedLEDConfigRequest(uint8 boardIndex);

	// CMD 14h: Read back the current green intensity of all the keys of the target board.
	void sendGreenLEDConfigRequest(uint8 boardIndex);

	// CMD 15h: Read back the current blue intensity of all the keys of the target board.
	void sendBlueLEDConfigRequest(uint8 boardIndex);

	// CMD 16h: Read back the current channel configuration of all the keys of the target board.
	void sendChannelConfigRequest(uint8 boardIndex);

	// CMD 17h: Read back the current note configuration of all the keys of the target board.
	void sendNoteConfigRequest(uint8 boardIndex);

	// CMD 18h: Read back the current key type configuration of all the keys of the target board.
	void sendKeyTypeConfigRequest(uint8 boardIndex);

	// CMD 19h: Read back the maximum fader (note on) threshold of all the keys of the target board.
	void sendMaxFaderThresholdRequest(uint8 boardIndex);

	// CMD 1Ah: Read back the maximum fader (note on) threshold of all the keys of the target board
	void sendMinFaderThresholdRequest(uint8 boardIndex);

	// CMD 1Bh: Read back the aftertouch maximum threshold of all the keys of the target board
	void sendMaxAftertouchThresholdRequest(uint8 boardIndex);

	// CMD 1Ch: Get back flag whether or not each key of target board meets minimum threshold
	void sendKeyValidityParametersRequest(uint8 boardIndex);
	
	// CMD 1Dh: Read back the current velocity look up table of the keyboard.
	void sendVelocityConfigRequest();

	// CMD 1Eh: Read back the current fader look up table of the keyboard.
	void sendFaderConfigRequest();

	// CMD 1Fh: Read back the current aftertouch look up table of the keyboard.
	void sendAftertouchConfigRequest();

	// CMD 20h: Set the velocity interval table, 127 12-bit values (up to 0x7fff
    void sendVelocityIntervalConfig(const int velocityIntervalTable[]);

	// CMD 21h: Sead back the velocity interval table
	void sendVelocityIntervalConfigRequest();

	// CMD 22h: Read back the fader type of all keys on the targeted board.
	void sendFaderTypeConfigRequest(uint8 boardIndex);

	// CMD 23h: This command is used to read back the serial identification number of the keyboard.
	void sendGetSerialIdentityRequest(int sendToTestDevice = -1);

	// CMD 24h: Initiate the key calibration routine; each pair of macro buttons  
	// on each octave must be pressed to return to normal state
	void sendCalibrateKeys();

	// CMD 25h: Pass in true to enter Demo Mode, or false to exit
	void startDemoMode(bool turnOn);

	// CMD 26h: Initiate the pitch and mod wheel calibration routine, pass in false to stop
	void sendCalibratePitchModWheel(bool startCalibration);

	// CMD 27h: Set the sensitivity value of the mod wheel, 0x01 to 0x07f
	void setModWheelSensitivity(uint8 sensitivity);

	// CMD 28h: Set the sensitivity value of the pitch bend wheel between 0x01 and 0x3FFF
	void setPitchBendSensitivity(int sensitivity);

	// CMD 29h: Set abs. distance from max value to trigger CA-004 submodule key events, ranging from 0x00 to 0xFE
	void setKeyMaximumThreshold(uint8 boardIndex, uint8 maxThreshold, uint8 aftertouchMax);

	// CMD 2Ah: Set abs. distance from min value to trigger CA-004 submodule key events, ranging from 0x00 to 0xFE
	void setKeyMinimumThreshold(uint8 boardIndex, uint8 minThresholdHigh, uint8 minThresholdLow);

	// CMD 2Bh: Set the sensitivity for CC events, ranging from 0x00 to 0xFE
	void setFaderKeySensitivity(uint8 boardIndex, uint8 sensitivity);

	// CMD 2Ch: Set the target board sensitivity for aftertouch events, ranging from 0x00 to 0xFE
	void setAftertouchKeySensitivity(uint8 boardIndex, uint8 sensitivity);

	// CMD 2Dh: Adjust the Lumatouch table, 127 7-bit values, where 127 is a result of a fully pressed key
	void setLumatouchConfig(const uint8 lumatouchTable[]);

	// CMD 2Eh: **DEPRECATED** Save Lumatouch table changes
	void saveLumatoneConfig();

	// CMD 2Fh: Reset the Lumatouch table back to factory settings
	void resetLumatouchConfig();

	// CMD 30h: Read back the Lumatouch table
	void sendLumatouchConfigRequest();

	// CMD 31h: This command is used to read back the current Lumatone firmware revision.
	// The firmware version format is in the form {Major}.{Minor}.{Revision}
	// If the board has not been initialized, the Beaglebone will contain a firmware revision of 0.0.0 for the board
	void sendGetFirmwareRevisionRequest(int sendToTestDevice = -1);

	// CMD 32h: Set the thresold from key’s min value to trigger CA - 004 submodule CC events, ranging from 0x00 to 0xFE
	void setCCActiveThreshold(uint8 boardIndex, uint8 sensitivity);

	// CMD 33h: Echo the payload, 3 7-bit values, for use in connection monitoring
    // the first 7-bit value is reserved for echo differentiation
	void ping(uint8 value1, uint8 value2, uint8 value3, int sendToTestDevice = -1);
	unsigned int ping(unsigned int value, int sendToTestDevice = -1);

	// CMD 34h: Reset the thresholds for events and sensitivity for CC & aftertouch on the target board
	void resetBoardThresholds(uint8 boardIndex);

	// CMD 35h: Enable/disable key sampling over SSH for the target key and board
	void setKeySampling(uint8 boardIndex, uint8 keyIndex, bool turnSamplingOn);

	// CMD 36h: Set thresholds for the pitch and modulation wheel to factory settings
	void resetWheelsThresholds();

	// CMD 37h: Set the bounds from the calibrated zero adc value of the pitch wheel, 0x00 to 0x7f
	void setPitchWheelZeroThreshold(uint8 threshold);

	// CMD 38h: Pass in true to initiate the expression pedal calibration routine, or false to stop
	void calibrateExpressionPedal(bool startCalibration);

	// CMD 39h: Reset expression pedal minimum and maximum bounds to factory settings
	void resetExpressionPedalBounds();

	// CMD 3Ah: Retrieve the threshold values of target board
	void getBoardThresholdValues(uint8 boardIndex);

	// CMD 3Bh: Retrieve the sensitivity values of target board
	void getBoardSensitivityValues(uint8 boardIndex);

	// CMD 3Ch: Set the MIDI channels for peripheral controllers
	void setPeripheralChannels(uint8 pitchWheelChannel, uint8 modWheelChannel, uint8 expressionChannel, uint8 sustainChannel);

	// CMD 3Dh: Retrieve the MIDI channels for peripheral controllers
	void getPeripheralChannels();

	// CMD 3Eh: This command is only used for automatic responses containing calibration data every 100ms while in calibration mode
	// Sending a SysEx message containing this command will result in a NACK response

	// CMD 3Fh: Set the 8-bit aftertouch trigger delay value, the time between a note on event and the initialization of aftertouch events
	void setAfterTouchTriggerDelay(uint8 boardIndex, uint8 aftertouchTriggerValue);
	void setAfterTouchTriggerDelay(uint8 boardIndex, uint8 triggerValueUpperNibble, uint8 triggerValueLowerNibble);

	// CMD 40h: Retrieve the aftertouch trigger delay of the given board
	void sendGetAftertouchTriggerDelayRequest(uint8 boardIndex);

	// CMD 41h: Set the Lumatouch note-off delay value, an 11-bit integer representing the amount of 1.1ms ticks before
	// sending a note-off event after a Lumatone-configured key is released. 
	void setLumatouchNoteOffDelay(uint8 boardIndex, int delayValue);
	void setLumatouchNoteOffDelay(uint8 boardIndex, uint8 valueBits8_11, uint8 valueBits4_7, uint8 valueBits0_3);

	// CMD 42h: Retrieve the note-off delay value of the given board
	void sendGetLumatouchNoteOffDelayRequest(uint8 boardIndex);

	// CMD 43h: Set expression pedal ADC threshold value, a 12-bit integer
	void setExpressionPedalADCThreshold(int thresholdValue);
	// CMD 43h: Set expression pedal ADC threshold value with 3 4-bit integers representing a 12-bit integer
	void setExpressionPedalADCThreshold(uint8 valueBits8_12, uint8 valueBits4_7, uint8 valueBits0_3);

	// CMD 44h: Get the current expression pedal ADC threshold value
	void sendGetExpressionPedalADCThresholdRequest();

	// CMD 45h: Configure the on/off settings of the sustain pedal
	void sendInvertSustainPedal(bool setInverted);

	//============================================================================
	// Implementation of bidirectional communication with acknowledge messages

	// MIDI input callback: handle acknowledge messages
	void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override;

	// Handle timeout
	void timerCallback() override;

	// Clear MIDI message buffer
	void clearMIDIMessageBuffer();

	// Message is an answer to a sent message yes/no
	static bool messageIsResponseToMessage(const MidiMessage& answer, const MidiMessage& originalMessage);

	// Message is SysEx message with Lumatone Inc. manufacturer ID, valid board ID byte, and acknowledged status
	FirmwareSupport::Error messageIsValidLumatoneResponse(const MidiMessage& midiMessage);

	// Determines if response is too short, too long, or expected
	FirmwareSupport::Error responseIsExpectedLength(const MidiMessage& midiMessage, size_t numPayloadBytes);

	//============================================================================
	// Specific commands for unpacking responses, returns error code

	// For CMD 13h response: unpacks 8-bit key data for red LED intensity. 112 bytes, lower and upper nibbles for 56 values
	FirmwareSupport::Error unpackGetLEDConfigResponse(const MidiMessage& response, int& boardId, int* keyData);
	
	// For CMD 13h response: unpacks 7-bit key data for red LED intensity. 56 bytes, each value must be multiplied by 5
	FirmwareSupport::Error unpackGetLEDConfigResponse_Version_1_0_0(const MidiMessage& response, int& boardId, uint8 numKeys, int* keyData);

	// For CMD 16h response: unpacks 7-bit channel data for note configuration. 55 or 56 bytes
	FirmwareSupport::Error unpackGetChannelConfigResponse(const MidiMessage& response, int& boardId, uint8 numKeys, int* keyData);

	// For CMD 17h response: unpacks 7-bit key data for note configuration. 55 or 56 bytes
	FirmwareSupport::Error unpackGetNoteConfigResponse(const MidiMessage& response, int& boardId, uint8 numKeys, int* keyData);

	// For CMD 18h response: unpacks 7-bit key type data for key configuration. 55 or 56 bytes
	FirmwareSupport::Error unpackGetTypeConfigResponse(const MidiMessage& response, int& boardId, uint8 numKeys, int* keyData);

	// For CMD 19h response: unpacks 8-bit key data for maximums of adc threshold. 55 or 56 bytes
	FirmwareSupport::Error unpackGetKeyMaxThresholdsResponse(const MidiMessage& response, int& boardId, uint8 numKeys, int* keyData);

	// For CMD 1Ah response: unpacks 8-bit key data for minimums of adc threshold. 55 or 56 bytes
	FirmwareSupport::Error unpackGetKeyMinThresholdsResponse(const MidiMessage& response, int& boardId, uint8 numKeys, int* keyData);

	// For CMD 1Bh response: unpacks 8-bit key data for maximums of adc threshold for aftertouch triggering. 55 or 56 bytes
	FirmwareSupport::Error unpackGetAftertouchMaxThresholdsResponse(const MidiMessage& response, int& boardId, uint8 numKeys, int* keyData);

	// For CMD 1Ch response: unpacks boolean key validity data for board, whether or not each key meets threshold specs
	FirmwareSupport::Error unpackGetKeyValidityResponse(const MidiMessage& response, int& boardId, uint8 numKeys, bool* keyValidityData);

	// For CMD 1Dh response: unpacks 7-bit velocity configuration of keyboard, 128 bytes
	FirmwareSupport::Error unpackGetVelocityConfigResponse(const MidiMessage& response, int* velocityData);

	// For CMD 1Eh response: unpacks 7-bit fader configuration of keyboard, 128 bytes
	FirmwareSupport::Error unpackGetFaderConfigResponse(const MidiMessage& response, int* faderData);

	// For CMD 1Fh response: unpacks 7-bit aftertouch configuration of keyboard, 128 bytes
	FirmwareSupport::Error unpackGetAftertouchConfigResponse(const MidiMessage& response, int* aftertouchData);

	// For CMD 21h response: unpacks 12-bit velocity interval configuration of keyboard, 254 bytes encoding 127 values
	FirmwareSupport::Error unpackGetVelocityIntervalConfigResponse(const MidiMessage& response, int* intervalData);

	// For CMD 22h response: unpacks 7-bit fader type configuration of board, 56 bytes
	FirmwareSupport::Error unpackGetFaderConfigResponse(const MidiMessage& response, int& boardId, uint8 numKeys, int* faderData);

	// For CMD 23h response: unpacks serial ID number of keyboard, 12 7-bit values encoding 6 bytes
	FirmwareSupport::Error unpackGetSerialIdentityResponse(const MidiMessage& response, int* serialBytes);

	// For CMD 30h response: unpacks 7-bit Lumatouch configuration of keyboard, 128 bytes
	FirmwareSupport::Error unpackGetLumatouchConfigResponse(const MidiMessage& response, int* lumatouchData);

	// For CMD 31h response: unpacks firmware revision running on the keyboard
	FirmwareSupport::Error unpackGetFirmwareRevisionResponse(const MidiMessage& response, int& majorVersion, int& minorVersion, int& revision);

	// For CMD 33h response: echo payload
	FirmwareSupport::Error unpackPingResponse(const MidiMessage& response, int& value1, int& value2, int& value3, int& value4);
	FirmwareSupport::Error unpackPingResponse(const MidiMessage& response, unsigned int& value);

	// For CMD 3Ah response: retrieve all 8-bit threshold values of a certain board
	FirmwareSupport::Error unpackGetBoardThresholdValuesResponse(const MidiMessage& response, int& boardId, int& minHighThreshold, int& minLowThreshold, int& maxThreshold, int& aftertouchThreshold, int& ccThreshold);

	// For CMD 3Bh response: retrieve all threshold values of a certain board
	FirmwareSupport::Error unpackGetBoardSensitivityValuesResponse(const MidiMessage& response, int& boardId, int& ccSensitivity, int& aftertouchSensitivity);

	// For CMD 3Dh response: retrieve MIDI channels of which peripherals are configured
	FirmwareSupport::Error unpackGetPeripheralChannelsResponse(const MidiMessage& response, int& pitchWheelChannel, int& modWheelChannel, int& expressionChannel, int& sustainPedalChannel);

	// For CMD 3Eh response: retrieve 12-bit expression pedal calibration status values in respective mode, automatically sent every 100ms
	FirmwareSupport::Error unpackExpressionPedalCalibrationPayload(const MidiMessage& response, int& minBound, int& maxBound, bool& valid);

	// For CMD 3Eh response: retrieve 12-bit pitch & mod wheel calibration status values in respective mode, automatically sent every 100ms
	FirmwareSupport::Error unpackWheelsCalibrationPayload(const MidiMessage& response, int& centerPitch, int& minPitch, int& maxPitch, int& minMod, int& maxMod);

	// For CMD 40h response: retrieve 8-bit aftertouch trigger delay of a certain board
	FirmwareSupport::Error unpackGetAftertouchTriggerDelayResponse(const MidiMessage& response, int& boardId, int& triggerDelay);

	// For CMD 42h response: retrieve 12-bit Lumatouch note off delay of a certain board
	FirmwareSupport::Error unpackGetLumatouchNoteOffDelayResponse(const MidiMessage& response, int& boardId, int& delay);

	// For CMD 44h response: retrieve 12-bit expression pedal adc threshold
	FirmwareSupport::Error unpackGetExpressionPedalThresholdResponse(const MidiMessage& response, int& thresholdValue);


private:
	// Low-level SysEx message sending
	void sendMessageWithAcknowledge(const MidiMessage& message);

	// Send the oldest message in queue and start waiting for answer
	void sendOldestMessageInQueue();

	// Send the message marked as current and start waiting for answer
    void sendCurrentMessage();

	// Fill a buffer with 3-bytes representing the manufacturer's ID
	void fillManufacturerId(unsigned char* data) const;

	// Create a SysEx message with standardized length
	MidiMessage createTerpstraSysEx(uint8 boardIndex, uint8 cmd, uint8 data1, uint8 data2, uint8 data3, uint8 data4) const;

	// Create a SysEx message to send 8-bit colour precision to a Lumatone key
	MidiMessage createExtendedKeyColourSysEx(uint8 boardIndex, uint8 cmd, uint8 keyIndex, uint8 redUpper, uint8 redLower, uint8 greenUpper, uint8 greenLower, uint8 blueUpper, uint8 blueLower) const;
	MidiMessage createExtendedKeyColourSysEx(uint8 boardIndex, uint8 cmd, uint8 keyIndex, int red, int green, int blue) const;

	// Create a SysEx message to send 8-bit colour precision to macro buttons
	MidiMessage createExtendedMacroColourSysEx(uint8 cmd, uint8 redUpper, uint8 redLower, uint8 greenUpper, uint8 greenLower, uint8 blueUpper, uint8 blueLower) const;
	MidiMessage createExtendedMacroColourSysEx(uint8 cmd, int red, int green, int blue) const;

	// Create a SysEx message encoding a table with a defined size
	MidiMessage sendTableSysEx(uint8 boardIndex, uint8 cmd, uint8 tableSize, const uint8 table[]);

    // Send a SysEx message with standardized length
	void sendSysEx(uint8 boardIndex, uint8 cmd, uint8 data1, uint8 data2, uint8 data3, uint8 data4, bool overrideEditMode = false);

	// Send a SysEx message without parameters
	void sendSysExRequest(uint8 boardIndex, uint8 cmd);

	// Send a SysEx message to toggle a state
	void sendSysExToggle(uint8 boardIndex, uint8 cmd, bool turnStateOn);

	// Checks if message is a valid Lumatone firmware response and is expected length, then runs supplied unpacking function or returns an error code 
	FirmwareSupport::Error unpackIfValid(const MidiMessage& response, size_t numBytes, std::function<FirmwareSupport::Error(const uint8*)> unpackFunction);

	// Generic unpacking of octave data from a SysEx message
	FirmwareSupport::Error unpackOctaveConfig(const MidiMessage& msg, int& boardId, size_t numBytes, int* keyData, std::function<FirmwareSupport::Error(const MidiMessage&, size_t, int*)> nBitUnpackFunction);

	// Generic unpacking of 7-bit data from a SysEx message
	FirmwareSupport::Error unpack7BitData(const MidiMessage& msg, size_t numBytes, int* unpackedData);

	// Unpacking of octave-based 7-bit key configuration data
	FirmwareSupport::Error unpack7BitOctaveData(const MidiMessage& msg, int& boardId, uint8 numKeys, int* keyData);

	// Generic unpacking of 8-bit data from a SysEx message
	FirmwareSupport::Error unpack8BitData(const MidiMessage& msg, size_t numBytes, int* unpackedData);

	// Unpacking of octave-based 8-bit data
	FirmwareSupport::Error unpack8BitOctaveData(const MidiMessage& msg, int& boardId, uint8 numKeys, int* keyData);

	// Generic unpacking of 12-bit data from a SysEx message, when packed with two 7-bit values
	FirmwareSupport::Error unpack12BitDataFrom7Bit(const MidiMessage& msg, size_t numBytes, int* unpackedData);

	// Generic unpacking of 12-bit data from a SysEx message, when packed with three 4-bit values
	FirmwareSupport::Error unpack12BitDataFrom4Bit(const MidiMessage& msg, size_t numBytes, int* unpackedData);

	// Attributes
protected:
    ListenerList<Listener> listeners;

private:

    MidiMessage currentMsgWaitingForAck;    // std::optional would be the object of choice,once that is available...
	bool hasMsgWaitingForAck = false;       // will be obsolete when std::optional is available

	Array<MidiMessage> messageBuffer;

	bool      sendTestMessagesOnly = false; // Only send GetSerialIdentity, GetFirmwareRevision, and Ping commands

	const int receiveTimeoutInMilliseconds = 2000;
	const int busyTimeDelayInMilliseconds = 500;
	TimerType timerType;
};
