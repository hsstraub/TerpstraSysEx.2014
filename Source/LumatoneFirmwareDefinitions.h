/*
  ==============================================================================

    LumatoneFirmwareDefinitions.h
    Created: 7 Apr 2024 3:53:44pm
    Author:  straub

  ==============================================================================
*/

#pragma once

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

// Firmware Version 1.0.12 / 1.1.0
#define INVERT_SUSTAIN_PEDAL 0x45

typedef enum
{
    disabledDefault      = 0,
	noteOnNoteOff        = 1,
	continuousController = 2,
	lumaTouch            = 3,
	disabled             = 4
} LumatoneKeyType;

