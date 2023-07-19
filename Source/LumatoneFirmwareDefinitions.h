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

#define ECHO_FLAG    0x5  // Used to differentiate test responses from MIDI feedback
#define TEST_ECHO    0x7F // Should not be returned by Lumatone firmware

#define SERIAL_55_KEYS "00 00 00 00 00 00"

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

// Firmware Version 1.0.12 / 1.1.0
#define GET_BOARD_THRESHOLD_VALUES 0x3A
#define GET_BOARD_SENSITIVITY_VALUES 0x3B

#define SET_PERIPHERAL_CHANNELS 0x3C
#define GET_PERIPHERAL_CHANNELS 0x3D
#define PERIPHERAL_CALBRATION_DATA 0x3E

#define SET_AFTERTOUCH_TRIGGER_DELAY 0x3F
#define GET_AFTERTOUCH_TRIGGER_DELAY 0x40

#define SET_LUMATOUCH_NOTE_OFF_DELAY 0x41
#define GET_LUMATOUCH_NOTE_OFF_DELAY 0x42
#define SET_EXPRESSION_PEDAL_THRESHOLD 0x43
#define GET_EXPRESSION_PEDAL_THRESHOLD 0x44
#define INVERT_SUSTAIN_PEDAL 0x45
#define RESET_DEFAULT_PRESETS 0x46
#define GET_PRESET_FLAGS 0x47
#define GET_EXPRESSION_PEDAL_SENSITIVIY 0x48

#define GET_MACRO_LIGHT_INTENSITY 0x49
#define RESET_MACRO_LIGHT_INTENSITY 0x4A

#define RESET_BOARD_KEYS 0x4B
#define RESET_AFTERTOUCH_TRIGGER_DELAY 0x4C

#define RESET_LUMATOUCH_NOTE_OFF_DELAY 0x4D
#define GET_PITCH_AND_MOD_BOUNDS 0x4E
#define GET_EXPRESSION_PEDAL_BOUNDS 0x4F

typedef enum
{
	NACK    = 0x00, // Not recognized
	ACK     = 0x01, // Acknowledged, OK
	BUSY    = 0x02, // Controller busy
	ERROR   = 0x03, // Error
	STATE   = 0x04, // Not in MIDI state
} TerpstraMIDIAnswerReturnCode;

typedef enum
{
    disabledDefault      = 0,
	noteOnNoteOff        = 1,
	continuousController = 2,
	lumaTouch            = 3,
	disabled             = 4
} LumatoneKeyType;

typedef enum
{
	ExpressionPedal = 0,
	PitchAndModWheels
} PeripheralCalibrationDataMode;

enum class LumatoneFirmwareVersion
{
	NO_VERSION      = 0,  // Used for instantiation
	VERSION_55_KEYS = 0,  // Used when GetSerialIdentity returns 00000000
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
    VERSION_1_1_0 = VERSION_1_0_12,
    VERSION_1_2_0,
	LAST_VERSION = VERSION_1_2_0,
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

	String toDisplayString() const 
	{ 
		String str = String(major) + "." + String(minor);
		if (revision > 0)
			str += ("." + String(revision));

		return str;
	}

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

	static FirmwareVersion fromDeterminedVersion(LumatoneFirmwareVersion versionIn)
	{
		int versionIndex = (int)versionIn;
		if (versionIn >= LumatoneFirmwareVersion::VERSION_1_0_3)
		{
			// Return special definition for future version
			if (versionIn > LumatoneFirmwareVersion::LAST_VERSION)
				return FirmwareVersion(0, 0, (int)LumatoneFirmwareVersion::FUTURE_VERSION);
			else
				return FirmwareVersion(1, 0, versionIndex - (int)LumatoneFirmwareVersion::VERSION_1_0_3);
		}
		// Return special definition for 55-keys version
		else if (versionIn == LumatoneFirmwareVersion::VERSION_55_KEYS)
			return FirmwareVersion(0, 0, 55);
		
		return FirmwareVersion(0, 0, 0);
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
        messageIsAnEcho,
		messageHasIncorrectManufacturerId,
		messageHasInvalidBoardIndex,
		messageHasInvalidStatusByte,
		messageIsNotResponseToCommand,
		messageIsNotSysEx,
		unknownCommand,
		externalError,
        commandNotImplemented
	};

	String errorToString(Error err)
	{
		switch (err)
		{
		case Error::noError:
			return "No error";
		case Error::noMidiOutputSet:
			return "No Midi output set";
		case Error::deviceIsBusy:
			return "Device is busy";
		case Error::messageTooShort:
            return "Message too short";
		case Error::messageTooLong:
            return "Message too long";
        case Error::messageIsAnEcho:
            return "Message is an echo";
		case Error::messageHasIncorrectManufacturerId:
			return "Incorrect manufacturer ID";
		case Error::messageHasInvalidBoardIndex:
			return "Invalid board index";
		case Error::messageHasInvalidStatusByte:
			return "Message has invalid status byte";
		case Error::messageIsNotResponseToCommand:
			return "Message is not a response to the command";
		case Error::messageIsNotSysEx:
			return "Message is not a valid SysEx message.";
		case Error::unknownCommand:
			return "Unknown command / Not Acknowledged";
		case Error::externalError:
			return "Error from device";
        case Error::commandNotImplemented:
            return "Command handling not implemented";
		default:
			return "Undefined error..";
		}
	}

	LumatoneFirmwareVersion getLumatoneFirmwareVersion(FirmwareVersion versionIn)
	{
		if (!(versionIn.major | versionIn.minor | versionIn.revision))
			return LumatoneFirmwareVersion::VERSION_55_KEYS;

		else if ((versionIn.major < 0) | (versionIn.minor < 0) | (versionIn.revision < 0))
			return LumatoneFirmwareVersion::UNKNOWN_VERSION;
		
        // MAJOR: 1
		else if (versionIn.major == 1)
		{
            // MINOR: 0
			if (versionIn.minor == 0)
			{
				if (versionIn.revision < 3)
					return LumatoneFirmwareVersion::VERSION_55_KEYS;

                // Computing is probably not the best thing to do but edge cases are extremely unlikely here
				else if (versionIn.revision - 3 > (int)LumatoneFirmwareVersion::LAST_VERSION - (int)LumatoneFirmwareVersion::VERSION_1_0_3)
					return LumatoneFirmwareVersion::FUTURE_VERSION;

				else if (versionIn.revision >= 3)
					return (LumatoneFirmwareVersion)((int)LumatoneFirmwareVersion::VERSION_1_0_3 + (versionIn.revision - 3));
			}
            
            // MINOR: 1
            else if (versionIn.minor == 1)
            {
                if (versionIn.revision == 0)
                    return LumatoneFirmwareVersion::VERSION_1_1_0;
            }
            
            else if (versionIn.minor == 2)
            {
                if (versionIn.revision == 0)
                    return LumatoneFirmwareVersion::VERSION_1_2_0;
            }
            
            return LumatoneFirmwareVersion::FUTURE_VERSION;
		}

        // Unsure if this is needed, or if returning FUTURE_VERSION without a condition is better
		else if (versionIn.major < 9 && versionIn.minor < 9 && versionIn.revision < 999)
			return LumatoneFirmwareVersion::FUTURE_VERSION;

		return LumatoneFirmwareVersion::UNKNOWN_VERSION;
	}

	// TODO use map instead
	// Returns the lowest version that will return ACK for a given command
	LumatoneFirmwareVersion getLowestVersionAcknowledged(unsigned int CMD)
	{
		if (CMD < CHANGE_KEY_NOTE) // 0x00
			return LumatoneFirmwareVersion::UNKNOWN_VERSION;

		else if (CMD <= GET_SERIAL_IDENTITY) // 0x23
			return LumatoneFirmwareVersion::NO_VERSION;

		else if (CMD <= DEMO_MODE) // 0x25
			return LumatoneFirmwareVersion::VERSION_1_0_5;

		else if (CMD <= CALIBRATE_PITCH_MOD_WHEEL) // 0x26
			return LumatoneFirmwareVersion::VERSION_1_0_6;

		else if (CMD <= SET_KEY_MAX_THRESHOLD) // 0x29
			return LumatoneFirmwareVersion::VERSION_1_0_7;

		else if (CMD <= GET_FIRMWARE_REVISION) // 0x31
			return LumatoneFirmwareVersion::VERSION_1_0_8;

		else if (CMD <= LUMA_PING) // 0x33
			return LumatoneFirmwareVersion::VERSION_1_0_9;

		else if (CMD <= SET_KEY_SAMPLING) // 0x35
			return LumatoneFirmwareVersion::VERSION_1_0_10;

		else if (CMD <= RESET_EXPRESSION_PEDAL_BOUNDS) // 0x39
			return LumatoneFirmwareVersion::VERSION_1_0_11;

		else if (CMD <= GET_EXPRESSION_PEDAL_BOUNDS) // 0x4F
			return LumatoneFirmwareVersion::VERSION_1_1_0;

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
    
    String serialIdentityToString(int* serialBytes)
    {
        return String::toHexString(serialBytes, 6);
    }

};

typedef enum
{
	PitchWheel = 0,
	ModWheel,
	Expression,
	Sustain
} PeripheralChannel;

struct PeripheralChannelSettings
{
	int pitchWheel = 1;
	int modWheel = 1;
	int expressionPedal = 1;
	int sustainPedal = 1;

	void setChannel(PeripheralChannel controlId, int channelIn)
	{
		if (channelIn < 1)
			channelIn = 1;

		if (channelIn > 16)
			channelIn = 16;

		switch (controlId)
		{
		case PeripheralChannel::PitchWheel:
			pitchWheel = channelIn;
			break;

		case PeripheralChannel::ModWheel:
			modWheel = channelIn;
			break;

		case PeripheralChannel::Expression:
			expressionPedal = channelIn;
			break;

		case PeripheralChannel::Sustain:
			sustainPedal = channelIn;
			break;
		}
	}

	int getChannel(PeripheralChannel controlId)
	{
		switch (controlId)
		{
		case PeripheralChannel::PitchWheel:
			return pitchWheel;

		case PeripheralChannel::ModWheel:
			return modWheel;

		case PeripheralChannel::Expression:
			return expressionPedal;

		case PeripheralChannel::Sustain:
			return sustainPedal;
		}

		return 0;
	}
};

#define ADCSCALAR 2.44140625e-4;

struct WheelsCalibrationData
{
	int centerPitch = 0;
	int minPitch = 0;
	int maxPitch = 0;

	int minMod = 0;
	int maxMod = 0;

	float getCentrePitchNorm() const { return centerPitch * ADCSCALAR; }
	float getMinPitchNorm() const { return minPitch * ADCSCALAR; }
	float getMaxPitchNorm() const { return maxPitch * ADCSCALAR; }
	float getMinModNorm() const { return minMod * ADCSCALAR; }
	float getMaxModNorm() const { return maxMod * ADCSCALAR; }

	String toString() const
	{
		String str;
		str += ("Center Pitch: " + String(centerPitch) + newLine);
		str += ("   Min Pitch: " + String(minPitch) + newLine);
		str += ("   Max Pitch: " + String(maxPitch) + newLine);
		str += ("     Min Mod: " + String(minMod) + newLine);
		str += ("     Max Mod: " + String(maxMod) + newLine);
		return str;
	}
};

struct PresetFlags
{
	bool expressionPedalInverted = false;
	bool lightsOnKeystroke = false;
	bool polyphonicAftertouch = false;
	bool sustainPedalInverted = false;
};
