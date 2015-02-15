#ifndef MIDCONST_INCLUDED
#define MIDCONST_INCLUDED

// State Constants
#define MIDISTATE_CLOSED 0
#define MIDISTATE_OPEN 1
#define MIDISTATE_STARTED  2
#define MIDISTATE_STOPPED 3
#define MIDISTATE_PAUSED  4

// MidiOut actions

#define MIDIOUT_NONE  0
#define MIDIOUT_OPEN 1
#define MIDIOUT_CLOSE 2
#define MIDIOUT_RESET 3
#define MIDIOUT_START 4
#define MIDIOUT_STOP 5
#define MIDIOUT_QUEUE 6
#define MIDIOUT_SEND 7
#define MIDIOUT_TIMER 8
#define MIDIOUT_PAUSE 9

// MidiIn actions

#define MIDIIN_NONE 0
#define MIDIIN_OPEN 1
#define MIDIIN_CLOSE 2
#define MIDIIN_RESET 3
#define MIDIIN_START 4
#define MIDIIN_STOP 5
#define MIDIIN_REMOVE 6

// MidiOut device types

#define MIDIOUT_PORT 0
#define MIDIOUT_SQUARESYNTH 1
#define MIDIOUT_FMSYNTH 2
#define MIDIOUT_MIDIMAPPER 3

// MidiFile actions

#define MIDIFILE_NONE = 0
#define MIDIFILE_OPEN = 1
#define MIDIFILE_CLOSE = 2
#define MIDIFILE_CREATE = 3
#define MIDIFILE_SAVE = 4
#define MIDIFILE_CLEAR = 5
#define MIDIFILE_INSERT_MESSAGE = 6
#define MIDIFILE_MODIFY_MESSAGE = 7
#define MIDIFILE_DELETE_MESSAGE = 8
#define MIDIFILE_INSERT_TRACK = 9
#define MIDIFILE_DELETE_TRACK = 10
#define MIDIFILE_SAVE_AS = 11

// Standard MIDI File Meta Event Constants

#define META 0xFF
#define META_SEQUENCE_NUMBER 0x00
#define META_TEXT 0x01
#define META_COPYRIGHT 0x02
#define META_NAME 0x03
#define META_INST_NAME 0x04
#define META_LYRIC 0x05
#define META_MARKER 0x06
#define META_CUE_POINT 0x07
#define META_CHAN_PREFIX 0x20
#define META_EOT 0x2F
#define META_TEMPO 0x51
#define META_SMPTE_OFFSET 0x54
#define META_TIME_SIG 0x58
#define META_KEY_SIG 0x59
#define META_SEQ_SPECIFIC 0x7F

// MIDI System Command Filter
#define FILTER_MTC 0xF1         // filter MTC
#define FILTER_SNGPTR 0xF2       // filter Song Position Pointer F2
#define FILTER_SNGSEL 0xF3       // filter Song Select F3
#define FILTER_F4 0xF4           // filter undefined F4
#define FILTER_F5 0xF5           // filter undefined F5
#define FILTER_TUNE 0xF6         // filter Tune Request F6
#define FILTER_CLOCK 0xF8        // filter MIDI Clock F8
#define FILTER_F9 0xF9           // filter undefined F9
#define FILTER_START 0xFA        // filter MIDI Start FA
#define FILTER_CONT 0xFB         // filter MIDI Continue FB
#define FILTER_STOP 0xFC         // filter MIDI Stop FC
#define FILTER_FD 0xFD           // filter undefined FD
#define FILTER_ACTIVE_SENSE 0xFE // filter Active Sensing FE
#define FILTER_RESET 0xFF        // filter System Reset FF

// MIDI status messages

#define NOTE_OFF 0x80
#define NOTE_ON 0x90
#define POLY_KEY_PRESS 0xA0
#define CONTROLLER_CHANGE 0xB0
#define PROGRAM_CHANGE 0xC0
#define CHANNEL_PRESSURE 0xD0
#define PITCH_BEND 0xE0
#define SYSEX 0xF0
#define MTC_QFRAME 0xF1
#define EOX 0xF7
#define MIDI_CLOCK 0xF8
#define MIDI_START 0xFA
#define MIDI_CONTINUE 0xFB
#define MIDI_STOP 0xFC


// CONTROLLER_CHANGE, MIDI Controller Numbers Constants

#define MOD_WHEEL 1
#define BREATH_CONTROLLER 2
#define FOOT_CONTROLLER 4
#define PORTAMENTO_TIME 5
#define MAIN_VOLUME 7
#define BALANCE 8
#define PAN 10
#define EXPRESS_CONTROLLER 11
#define DAMPER_PEDAL 64
#define PORTAMENTO 65
#define SOSTENUTO 66
#define SOFT_PEDAL 67
#define HOLD_2 69
#define EXTERNAL_FX_DEPTH 91
#define TREMELO_DEPTH 92
#define CHORUS_DEPTH 93
#define DETUNE_DEPTH 94
#define PHASER_DEPTH 95
#define DATA_INCREMENT 96
#define DATA_DECREMENT 97

// Constants to simplify MIDI

#define TOTAL_MIDI_CHANNELS 16

// System exclusive command bytes

#define MMID1 0x00
#define MMID2 0x20
#define MMID3 0xFF
#define CHANGE_KEY_NOTE 0x00
#define READ_KEY_NOTE 0x01
#define STORE_TO_EEPROM 0x02
#define RECALL_FROM_EEPROM 0x03
#define READ_KEY_POSITION 0x04
#define SET_KEY_MAX_MIN 0x05
#define READ_KEY_MAX_MIN 0x06
#define SET_KEYUP_PROXIMITY 0x07
#define SET_KEYDN_PROXIMITY 0x08
#define SET_FOOT_CONTROLLER_SENSITIVITY 0x09
#define READ_FOOT_CONTROLLER_SENSITIVITY 0x0A
#define SET_FOOT_CONTROLLER_MAX 0x0B
#define READ_FOOT_CONTROLLER_MAX 0x0C
#define SET_FOOT_CONTROLLER_MIN 0x0D
#define READ_FOOT_CONTROLLER_MIN 0x0E
#define INVERT_FOOT_CONTROLLER 0x0F
#define SEND_KEY_DWELL_TO_VELOCITY_NUMBER 0x10
#define READ_KEY_DWELL_TO_VELOCITY_NUMBER 0x11

#endif