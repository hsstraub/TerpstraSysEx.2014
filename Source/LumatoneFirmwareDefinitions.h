/*
  ==============================================================================

    LumatoneFirmwareDefinitions.h
    Created: 17 Apr 2021 10:19:39pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

/*
==============================================================================
System exclusive board definitions
==============================================================================
*/

#define BOARD_SERVER 0x0
#define BOARD_OCT_1  0x1
#define BOARD_OCT_2  0x2
#define BOARD_OCT_3  0x3
#define BOARD_OCT_4  0x4
#define BOARD_OCT_5  0x5

/*
==============================================================================
System exclusive command structure
==============================================================================
*/

#define MANUFACTURER_ID_0 0x00
#define MANUFACTURER_ID_1 0x21
#define MANUFACTURER_ID_2 0x50

#define MANU_0       0x0
#define MANU_1       0x1
#define MANU_2       0x2
#define BOARD_IND    0x3
#define CMD_ID       0x4
#define MSG_STATUS   0x5
#define CALIB_MODE   0x5
#define PAYLOAD_INIT 0x6
#define PING_FLAG    0x7

#define PING_ECHO    0x7F // Should not be returned by Lumatone firmware

/*
==============================================================================
System exclusive command bytes
==============================================================================
*/

// Start support at 55-keys firmware version, Developmental versions
#define CHANGE_KEY_NOTE 0x00
#define SET_KEY_COLOUR 0x01

#define SAVE_PROGRAM 0x02

#define SET_FOOT_CONTROLLER_SENSITIVITY 0x03
#define INVERT_FOOT_CONTROLLER 0x04

#define MACROBUTTON_COLOUR_ON 0x05
#define MACROBUTTON_COLOUR_OFF 0x06

#define SET_LIGHT_ON_KEYSTROKES 0x07

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
#define RESET_AFTERTOUCH_CONFIG 0x12

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

// Firmware Version 1.0.3
#define SET_VELOCITY_INTERVALS 0x20
#define GET_VELOCITY_INTERVALS 0x21

// Firmware Version 1.0.4
#define GET_FADER_TYPE_CONFIGURATION 0x22

// Start 56-keys, Firmware Version 1.0.5
#define GET_SERIAL_IDENTITY 0x23
// 0x23 will acknowledge in 55-keys but will not return serial number

#define CALIBRATE_KEYS 0x24

#define DEMO_MODE 0x25

// Firmware Version 1.0.6
#define CALIBRATE_PITCH_MOD_WHEEL 0x26
#define SET_MOD_WHEEL_SENSITIVITY 0x27
#define SET_PITCH_WHEEL_SENSITIVITY 0x28

// Firmware Version 1.0.7, Start shipping backers and batches
#define SET_KEY_MAX_THRESHOLD 0x29
#define SET_KEY_MIN_THRESHOLD 0x2A
#define SET_KEY_FADER_SENSITIVITY 0x2B
#define SET_KEY_AFTERTOUCH_SENSITIVITY 0x2C

#define SET_LUMATOUCH_CONFIG 0x2D
#define SAVE_LUMATOUCH_CONFIG 0x2E
#define RESET_LUMATOUCH_CONFIG 0x2F
#define GET_LUMATOUCH_CONFIG 0x30

// Firmware Version 1.0.8
#define GET_FIRMWARE_REVISION 0x31

// Firmware Version 1.0.9
#define SET_CC_ACTIVE_THRESHOLD 0x32
#define LUMA_PING 0x33

// Firmware Version 1.0.10
#define RESET_BOARD_THRESHOLDS 0x34
#define SET_KEY_SAMPLING 0x35

// Firmware Version 1.0.11
#define RESET_WHEELS_THRESHOLD 0x36
#define SET_PITCH_WHEEL_CENTER_THRESHOLD 0x37
#define CALLIBRATE_EXPRESSION_PEDAL 0x38
#define RESET_EXPRESSION_PEDAL_BOUNDS 0x39

// Firmware Version 1.0.12
#define GET_BOARD_THRESHOLD_VALUES 0x3A
#define GET_BOARD_SENSITIVITY_VALUES 0x3B

// Firmware Version 1.0.13
#define SET_PERIPHERAL_CHANNELS 0x3C
#define GET_PERIPHERAL_CHANNELS 0x3D
#define PERIPHERAL_CALBRATION_DATA 0x3E

// Firmware Version 1.0.14
#define SET_AFTERTOUCH_TRIGGER_DELAY 0x3F
#define GET_AFTERTOUCH_TRIGGER_DELAY 0x40

// Firmware Version 1.0.15
#define SET_LUMATOUCH_NOTE_OFF_DELAY 0x41
#define GET_LUMATOUCH_NOTE_OFF_DELAY 0x42
#define SET_EXPRESSION_PEDAL_THRESHOLD 0x43
#define GET_EXPRESSION_PEDAL_THRESHOLD 0x44

typedef enum
{
	NACK    = 0x00, // Not recognized
	ACK     = 0x01, // Acknowledged, OK
	BUSY    = 0x02, // Controller busy
	ERROR   = 0x03, // Error
} TerpstraMIDIAnswerReturnCode;

typedef enum
{
	disabled             = 0,
	noteOnNoteOff        = 1,
	continuousController = 2,
	lumaTouch            = 3
} LumatoneKeyType;

enum class LumatoneFirmwareVersion
{
	NO_VERSION      = 0,  // Used for instantiation
	VERSION_55_KEYS = 0,  // Used when version retrieved is 0.0.0
	UNKNOWN_VERSION = 0,  // Used when no other version applies
	VERSION_1_0_3 = 0x10,
	VERSION_1_0_4,
	VERSION_1_0_5,
	VERSION_1_0_6,
	VERSION_1_0_7,
	VERSION_1_0_8,
	VERSION_1_0_9,
	VERSION_1_0_10,
	VERSION_1_0_11,
	VERSION_1_0_12,
	VERSION_1_0_13,
	VERSION_1_0_14,
	VERSION_1_0_15,
	LAST_VERSION = VERSION_1_0_15,
	FUTURE_VERSION = 0xFFFF  // Used when version is nonnegative and below 9.9.999
} ;

struct FirmwareVersion
{
	int major = 0;
	int minor = 0;
	int revision = 0;

	FirmwareVersion(int majorVersion, int minorVersion, int revisionNumber)
		: major(majorVersion), minor(minorVersion), revision(revisionNumber) {}

	bool isValid() { return major > 0 || minor > 0 || revision > 0; }

	String toString() const { return String(major) + "." + String(minor) + "." + String(revision); }

	//============================================================================

	static FirmwareVersion fromString(String firmwareVersion)
	{
		FirmwareVersion version(0, 0, 0);

		String afterFirstDecimal = firmwareVersion.fromFirstOccurrenceOf(".", false, true);

		// Just check if it contains at least two decimals
		if (firmwareVersion.contains(".") && afterFirstDecimal.contains("."))
		{
			String majorNum = firmwareVersion.upToFirstOccurrenceOf(".", false, true);

			String minorNum = afterFirstDecimal.upToFirstOccurrenceOf(".", false, true);

			if (minorNum == afterFirstDecimal)
			{
				// This means there was only one decimal, don't try to parse
				return version;
			}

			String revisionNum = firmwareVersion.fromLastOccurrenceOf(".", false, true);
			if (revisionNum != revisionNum.upToFirstOccurrenceOf(".", false, true))
			{
				// This means there's an additional decimal, don't try to parse
				return version;
			}

			version.major = majorNum.getIntValue();
			version.minor = majorNum.getIntValue();
			version.revision = majorNum.getIntValue();
		}

		return version;
	}
};

struct FirmwareSupport
{
	enum class Error
	{
		noError = 0,
		noMidiOutputSet,
		deviceIsBusy,
		messageTooShort,
		messageTooLong,
		messageHasIncorrectManufacturerId,
		messageHasInvalidBoardIndex,
		messageHasInvalidStatusByte,
		messageIsNotResponseToCommand,
		messageIsNotSysEx,
		unknownCommand,
		externalError
	};

	LumatoneFirmwareVersion getLumatoneFirmwareVersion(FirmwareVersion versionIn)
	{
		if (!(versionIn.major | versionIn.minor | versionIn.revision))
			return LumatoneFirmwareVersion::VERSION_55_KEYS;

		else if ((versionIn.major < 0) | (versionIn.minor < 0) | (versionIn.revision < 0))
			return LumatoneFirmwareVersion::UNKNOWN_VERSION;
		
		else if (versionIn.major == 1)
		{
			if (versionIn.minor == 0)
			{
				if (versionIn.revision < 3)
					return LumatoneFirmwareVersion::VERSION_55_KEYS;

				else if (versionIn.revision - 3 > (int)LumatoneFirmwareVersion::LAST_VERSION - (int)LumatoneFirmwareVersion::VERSION_1_0_3)
					return LumatoneFirmwareVersion::FUTURE_VERSION;

				else if (versionIn.revision >= 3)
					return (LumatoneFirmwareVersion)((int)LumatoneFirmwareVersion::VERSION_1_0_3 + (versionIn.revision - 3));
			}
		}

		else if (versionIn.major < 9 && versionIn.minor < 9 && versionIn.revision < 999)
			return LumatoneFirmwareVersion::FUTURE_VERSION;

		return LumatoneFirmwareVersion::UNKNOWN_VERSION;
	}

	// TODO use map instead
	// Returns the lowest version that will return ACK for a given command
	LumatoneFirmwareVersion getLowestVersionAcknowledged(unsigned int CMD)
	{
		if (CMD < CHANGE_KEY_NOTE) // 0x00
			LumatoneFirmwareVersion::UNKNOWN_VERSION;

		else if (CMD <= GET_SERIAL_IDENTITY) // 0x23
			LumatoneFirmwareVersion::NO_VERSION;

		else if (CMD <= DEMO_MODE) // 0x25
			LumatoneFirmwareVersion::VERSION_1_0_5;

		else if (CMD <= CALIBRATE_PITCH_MOD_WHEEL) // 0x26
			LumatoneFirmwareVersion::VERSION_1_0_6;

		else if (CMD <= SET_KEY_MAX_THRESHOLD) // 0x29
			LumatoneFirmwareVersion::VERSION_1_0_7;

		else if (CMD <= GET_FIRMWARE_REVISION) // 0x31
			LumatoneFirmwareVersion::VERSION_1_0_8;

		else if (CMD <= LUMA_PING) // 0x33
			return LumatoneFirmwareVersion::VERSION_1_0_9;

		else if (CMD <= SET_KEY_SAMPLING) // 0x35
			return LumatoneFirmwareVersion::VERSION_1_0_10;

		else if (CMD <= RESET_EXPRESSION_PEDAL_BOUNDS) // 0x39
			return LumatoneFirmwareVersion::VERSION_1_0_11;

		else if (CMD <= GET_BOARD_SENSITIVITY_VALUES) // 0x3B
			return LumatoneFirmwareVersion::VERSION_1_0_12;

		else if (CMD <= PERIPHERAL_CALBRATION_DATA) // 0x3E
			return LumatoneFirmwareVersion::VERSION_1_0_13;

		else if (CMD <= SET_AFTERTOUCH_TRIGGER_DELAY) // 0x3F
			return LumatoneFirmwareVersion::VERSION_1_0_14;

		else if (CMD <= GET_EXPRESSION_PEDAL_THRESHOLD) //0x44
			return LumatoneFirmwareVersion::VERSION_1_0_15;

		else
			return LumatoneFirmwareVersion::FUTURE_VERSION;
	}

	bool versionAcknowledgesCommand(LumatoneFirmwareVersion VERSION, unsigned int CMD)
	{
		return getLowestVersionAcknowledged(CMD) <= VERSION;
	}

	bool versionAcknowledgesCommand(FirmwareVersion versionIn, unsigned int CMD)
	{
		auto VERSION = getLumatoneFirmwareVersion(versionIn);
		return versionAcknowledgesCommand(VERSION, CMD);
	}

	// Should always be 56 in production
	int getOctaveSize(LumatoneFirmwareVersion VERSION)
	{
		if (VERSION >= LumatoneFirmwareVersion::VERSION_1_0_3)
			return 56;
		else
			return 55;
	}

	int getCommandNumber(const MidiMessage& msg)
	{
		return msg.getSysExData()[CMD_ID];
	}
};
