/*
  ==============================================================================

    ApplicationListeners.h
    Created: 22 Mar 2022 10:11:10pm
    Author:  Vincenzo Sicurella
 
    Various listener and enums that should be shared throughout the app

  ==============================================================================
*/

#pragma once
#include  <JuceHeader.h>
#include "LumatoneFirmwareDefinitions.h"

enum sysExSendingMode
{
    liveEditor = 0,
    offlineEditor = 1,
    firmwareUpdate = 2
};

namespace LumatoneEditor
{
    //============================================================================
    // Public interface for Lumatone connection status

    class StatusListener
    {
    public:

        virtual ~StatusListener() {}

        virtual void connectionFailed() {}
        virtual void connectionEstablished(int inputMidiDevice, int outputMidiDevice) {}
        virtual void connectionLost() {}
    };

    //============================================================================
    // Public interface for Lumatone firmware communication
    class FirmwareListener
    {
    public:
        
        virtual ~FirmwareListener() {}

        // rgbFlag uses 0 for red, 1 for green, 2 for blue
        virtual void octaveColourConfigReceived(int octaveIndex, uint8 rgbFlag, const int* colourData) {};

        virtual void octaveChannelConfigReceived(int octaveIndex, const int* channelData) {};

        virtual void octaveNoteConfigReceived(int octaveIndex, const int* noteData) {};

        virtual void keyTypeConfigReceived(int octaveIndex, const int* keyTypeData) {};

        virtual void velocityConfigReceived(const int* velocityData) {};

        virtual void aftertouchConfigReceived(const int* aftertouch) {};

        virtual void velocityIntervalConfigReceived(const int* velocityData) {};

        virtual void faderConfigReceived(const int* faderData) {};

        virtual void faderTypeConfigReceived(int octaveIndex, const int* faderTypeData) {};

        virtual void serialIdentityReceived(int inputDeviceIndex, const int* serialBytes) {};
        
        virtual void calibratePitchModWheelAnswer(TerpstraMIDIAnswerReturnCode code) {};

        virtual void lumatouchConfigReceived(const int* lumatouchData) {};

        virtual void firmwareRevisionReceived(FirmwareVersion version) {};

        virtual void pingResponseReceived(int inputDeviceIndex, unsigned int pingValue) {};

        virtual void peripheralMidiChannelsReceived(PeripheralChannelSettings channelSettings) {};

        virtual void pedalCalibrationDataReceived(int minBound, int maxBound, bool pedalIsActive) {};

        virtual void wheelsCalibrationDataReceived(WheelsCalibrationData calibrationData) {};

        virtual void presetFlagsReceived(PresetFlags presetFlags) {};

        virtual void expressionPedalSensitivityReceived(int sensitivity) {};

        virtual void noAnswerToCommand(int cmd) {};
    };


     class EditorListener
     {
     public:
         
         virtual ~EditorListener() {}

         // TODO - change this to "status" and use "disconnected", "offline", "live", "firmware"
         virtual void editorModeChanged(sysExSendingMode newEditorMode) {}
    //         virtual void keyFunctionConfigurationChanged(int octaveNumber, int keyNumber, int noteOrCC, int midiChannel, LumatoneKeyType keyType, bool faderUpIsNull) {};
    //         virtual void keyColourConfigurationChanged(int octaveNumber, int keyNumber, Colour keyColour) {};
     };


        class MidiListener
        {
        public:
    
            virtual ~MidiListener() {}
    
            virtual void handleMidiMessage(const MidiMessage& msg) = 0;
        };
}
