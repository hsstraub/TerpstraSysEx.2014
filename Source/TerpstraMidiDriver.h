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
		noteOnNoteOff = 1,
		fader = 2,
		afterTouch = 3
	} VelocityCurveType;

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

	// Send a value for a velocity lookup table
	void sendVelocityConfig(VelocityCurveType velocityCurveType, unsigned char velocityTable[]);

	// Save velocity config to EEPROM
	void saveVelocityConfig(VelocityCurveType velocityCurveType);

	// reset velocity config to value from EEPROM
	void resetVelocityConfig(VelocityCurveType velocityCurveType);

	void sendAfterTouchActivation(bool value);

	void sendCalibrateAfterTouch();

    void sendVelocityIntervalConfig(int velocityIntervalTable[]);

	void sendRedLEDConfigurationRequest(int boardIndex);

	void sendGreenLEDConfigurationRequest(int boardIndex);

	void sendBlueLEDConfigurationRequest(int boardIndex);

	void sendChannelConfigurationRequest(int boardIndex);

	void sendNoteConfigurationRequest(int boardIndex);

	void sendKeyTypeConfigurationRequest(int boardIndex);

	void sendVelocityConfigurationRequest(VelocityCurveType velocityCurveType);

	void sendVelocityIntervalConfigRequest();

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
    bool messageIsTerpstraVelocityConfigReceptionMessage(const MidiMessage& midiMessage, VelocityCurveType velocityCurveType);

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
