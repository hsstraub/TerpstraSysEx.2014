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
//[/Headers]

/*
==============================================================================
System exclusive command bytes
==============================================================================
*/

#define CHANGE_KEY_NOTE 0x00
#define SET_KEY_COLOUR 0x01

#define SET_FOOT_CONTROLLER_SENSITIVITY 0x03
#define INVERT_FOOT_CONTROLLER 0x04

#define MACROBUTTON_COLOUR_ON 0x05
#define MACROBUTTON_COLOUR_OFF 0x06

#define SET_VELOCITY_CONFIG 0x08
#define SAVE_VELOCITY_CONFIG 0x09
#define RESET_VELOCITY_CONFIG 0x0A

#define SET_FADER_CONFIG 0x0B
#define SAVE_FADER_CONFIG 0x0C
#define RESET_FADER_CONFIG 0x0D

#define SET_AFTERTOUCH_FLAG 0x0E
#define CALIBRATE_AFTERTOUCH 0x0F
#define SET_AFTERTOUCH_CONFIG 0x10
#define SAVE_AFTERTOUCH_CONFIG 0x11
#define RESET_AFTERTOCUH_CONFIG 0x12

#define GET_RED_LED_CONFIG 0x13
#define GET_GREEN_LED_CONFIG 0x14
#define GET_BLUE_LED_CONFIG 0x15
#define GET_CHANNEL_CONFIG 0x16
#define GET_NOTE_CONFIG 0x17
#define GET_KEYTYPE_CONFIG 0x18

#define GET_MAX_THRESHOLD 0x19
#define GET_MIN_THRESHOLD 0x1A
#define GET_AFTERTOUCH_MAX 0x1B
#define GET_KEY_VALIDITY 0x1C

#define GET_VELOCITY_CONFIG 0x1D
#define GET_FADER_CONFIG 0x1E
#define GET_AFTERTOUCH_CONFIG 0x1F

#define SET_VELOCITY_INTERVALS 0x20
#define GET_VELOCITY_INTERVALS 0x21


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

	// Send a value for a velocity lookup table
	void sendVelocityConfig(VelocityCurveType velocityCurveType, unsigned char velocityTable[]);

	// Save velocity config to EEPROM
	void saveVelocityConfig(VelocityCurveType velocityCurveType);

	// reset velocity config to value from EEPROM
	void resetVelocityConfig(VelocityCurveType velocityCurveType);

	void sendAfterTouchActivation(bool value);

	void sendCalibrateAfterTouch();

	void sendRedLEDConfigurationRequest(int boardIndex);

	void sendGreenLEDConfigurationRequest(int boardIndex);

	void sendBlueLEDConfigurationRequest(int boardIndex);

	void sendChannelConfigurationRequest(int boardIndex);

	void sendNoteConfigurationRequest(int boardIndex);

	void sendKeyTypeConfigurationRequest(int boardIndex);

	void sendVelocityConfigurationRequest(VelocityCurveType velocityCurveType);

	////////////////////////////////////////////////////////////////////////////
	// Implmentation of bidirectional communication with acknowledge messages

	// MIDI input callback: handle acknowledge messages
	void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override;

	// Handle timeout
	void timerCallback() override;

	// Clear MIDI message buffer
	void clearMIDIMessageBuffer(){ messageBuffer.clear(); this->listeners.call(&Listener::midiSendQueueSize, 0); }

	// Message is an answer to a sent message yes/no
	static bool messageIsResponseToMessage(const MidiMessage& answer, const MidiMessage& originalMessage);

	// Message is SysEx message with Terpstra manufacturer ID yes/no
    bool messageIsTerpstraSysExMessage(const MidiMessage& midiMessage);

	// Message contains configuration data sent from controller yes/no
    bool messageIsTerpstraConfigurationDataReceptionMessage(const MidiMessage& midiMessage);

    // Message contains velocity curve data from controller for the specified velocity curve type yes/no
    bool messageIsTerpstraVelocityConfigReceptionMessage(const MidiMessage& midiMessage, VelocityCurveType velocityCurveType);

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

	const int receiveTimeoutInMilliseconds = 2000;
	const int busyTimeDelayInMilliseconds = 20;
	TimerType timerType;
};
