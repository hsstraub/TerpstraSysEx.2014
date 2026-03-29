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
#include "KeyboardDataStructure.h"
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

		virtual void midiMessageReceived(const MidiMessage& midiMessage) = 0;
		virtual void midiMessageSent(const MidiMessage& midiMessage) = 0;
		virtual void midiSendQueueSize(int queueSize) = 0;
        virtual void generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel) = 0;
	};

	typedef enum
	{
	    NACK = 0x00,    // Not recognized
	    ACK = 0x01,     // Acknowledged, OK
	    BUSY = 0x02,    // Controller busy
	    ERROR = 0x03,   // Error
	} TerpstraMIDIAnswerReturnCode;

	enum sysExSendingMode
	{
		liveEditor = 0,
		offlineEditor = 1
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

	void setMidiInput(int deviceIndex, MidiInputCallback* callback) = delete;
	void setMidiInput(int deviceIndex);

	void setAutoSave(bool value) { this->autoSave = value; }
	void setManufacturerId(int value) { manufacturerId = value; }

	void addListener(Listener* listenerToAdd);
	void removeListener(Listener* listenerToRemove);

	sysExSendingMode getSysExSendingMode() const { return currentSysExSendingMode; }
	void setSysExSendingMode(sysExSendingMode newMode);

	//////////////////////////////////
	// Combined (hi-level) commands

	// Send all parametrizations of one sub board
	void sendAllParamsOfBoard(int boardIndex, TerpstraKeys boardData);

	// Send and save a complete key mapping
	void sendCompleteMapping(TerpstraKeyMapping mappingData);

    // Send request to receive the current mapping of one sub board on the controller
	void sendGetMappingOfBoardRequest(int boardIndex);

	// Send request to receive the complete current mapping on the controller
	void sendGetCompleteMappingRequest();

	//////////////////////////////////
	// Single (mid-level) commands

	// Send parametrization of one key to the device
	void sendKeyParam(int boardIndex, int keyIndex, TerpstraKey keyData);

	// Send expression pedal sensivity
	void sendExpressionPedalSensivity(unsigned char value);

	// Send parametrization of foot controller
	void sendInvertFootController(bool value);

	// Colour for macro button in active state
	void sendMacroButtonActiveColour(String colourAsString);

	// Colour for macro button in inactive state
	void sendMacroButtonInactiveColour(String colourAsString);

	// Send parametrization of light on keystrokes
	void sendLightOnKeyStrokes(bool value);

	// CMD 08h: Send a value for a velocity lookup table (128 7-bit values)
	void sendVelocityConfig(unsigned char velocityTable[]);

	// CMD 09h: Save velocity config to EEPROM
	void saveVelocityConfig();

	// CMD 0Ah: Reset velocity config to value from EEPROM
	void resetVelocityConfig();

	// CMD 0Bh: Adjust the internal fader look-up table (size of 128)
	void sendFaderConfig(unsigned char faderTable[]);

	// CMD 0Ch: **DEPRECATED** Save the changes made to the fader look-up table
	void saveFaderConfiguration();

	// CMD 0Dh: Reset the fader lookup table back to its factory fader settings.
	void resetFaderConfig();

	void sendAfterTouchActivation(bool value);

	void sendCalibrateAfterTouch();

	// CMD 10h: Adjust the internal aftertouch look-up table (size of 128)
	void sendAftertouchConfig(unsigned char aftertouchTable[]);

	// CMD 11h: **DEPRECATED** Save the changes made to the aftertouch look-up table
	void saveAftertouchConfig();

	// CMD 12h: Reset the aftertouch lookup table back to its factory aftertouch settings.
	void resetAftertouchConfig();

	void sendRedLEDConfigurationRequest(int boardIndex);

	void sendGreenLEDConfigurationRequest(int boardIndex);

	void sendBlueLEDConfigurationRequest(int boardIndex);

	void sendChannelConfigurationRequest(int boardIndex);

	void sendNoteConfigurationRequest(int boardIndex);

	void sendKeyTypeConfigurationRequest(int boardIndex);

	// CMD 1Dh: Read back the current velocity look up table of the keyboard.
	void sendVelocityConfigRequest();

	// CMD 1Eh: Read back the current fader look up table of the keyboard.
	void sendFaderConfigRequest();

	// CMD 1Fh: Read back the current aftertouch look up table of the keyboard.
	void sendAftertouchConfigRequest();

	// CMD 20h: Set the velocity interval table, 127 12-bit values (up to 0x7fff
    void sendVelocityIntervalConfig(int velocityIntervalTable[]);

	// CMD 21h: Sead back the velocity interval table
	void sendVelocityIntervalConfigRequest();

	// CMD 22h: Read back the fader type of all keys on the targeted board.
	void sendFaderTypeConfigRequest(uint8 boardIndex);

	// CMD 23h: This command is used to read back the serial identification number of the keyboard.
	void sendGetSerialIdentityRequest(int sendToTestDevice = -1);

	////////////////////////////////////////////////////////////////////////////
	// CMD 45h: Configure the on/off settings of the sustain pedal
	void sendInvertSustainPedal(bool value);
	// Implementation of bidirectional communication with acknowledge messages

	// MIDI input callback: handle acknowledge messages
	void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override;

	// Handle timeout
	void timerCallback() override;

	// Clear MIDI message buffer
	void clearMIDIMessageBuffer() { messageBuffer.clear(); this->listeners.call(&Listener::midiSendQueueSize, 0);  }

	// Message is an answer to a sent message yes/no
	static bool messageIsResponseToMessage(const MidiMessage& answer, const MidiMessage& originalMessage);

	// Message is SysEx message with Terpstra manufacturer ID yes/no
    bool messageIsTerpstraSysExMessage(const MidiMessage& midiMessage);

	// Message contains configuration data sent from controller yes/no
    bool messageIsTerpstraConfigurationDataReceptionMessage(const MidiMessage& midiMessage);

    // Message contains velocity curve data from controller for the specified velocity curve type yes/no
    bool messageIsTerpstraVelocityConfigReceptionMessage(const MidiMessage& midiMessage, TerpstraVelocityCurveConfig::VelocityCurveType velocityCurveType);

    bool messageIsVelocityIntervalConfigReceptionMessage(const MidiMessage& midiMessage);

private:
	// Low-level SysEx message sending
	void sendMessageWithAcknowledge(const MidiMessage& message);

	// Send the oldest message in queue and start waiting for answer
	void sendOldestMessageInQueue();

	// Send the message marked as current and start waiting for answer
    void sendCurrentMessage();

    // Send a SysEx message with standardized length
	void sendSysEx(int boardIndex, unsigned char cmd, unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4);

	// Attributes
protected:
    ListenerList<Listener> listeners;

private:
	bool autoSave;
	int manufacturerId = 0x002150;

    MidiMessage currentMsgWaitingForAck;    // std::optional would be the object of choice,once that is available...
	bool hasMsgWaitingForAck = false;       // will be obsolete when std::optional is available

	Array<MidiMessage> messageBuffer;

	// Whether SysEx messages are sent or not
	sysExSendingMode currentSysExSendingMode = sysExSendingMode::liveEditor;

	const int receiveTimeoutInMilliseconds = 2000;
	const int busyTimeDelayInMilliseconds = 20;
	TimerType timerType;
};
