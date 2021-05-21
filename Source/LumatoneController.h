/*
  ==============================================================================

    LumatoneCommandManager.h
    Created: 17 Apr 2021 10:17:31pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "KeyboardDataStructure.h"
#include "TerpstraMidiDriver.h"
#include "DeviceActivityMonitor.h"
#include "FirmwareTransfer.h"

//==============================================================================
// Helper class for parsing and comparing (todo) firmware versions



class LumatoneController :  protected TerpstraMidiDriver::Listener, 
                            protected FirmwareTransfer::ProcessListener,
                            private juce::Thread::Listener,
                            private juce::Timer, 
                            private juce::ChangeListener
{
public:

    enum sysExSendingMode
    {
        liveEditor = 0,
        offlineEditor = 1,
        firmwareUpdate = 2
    };

public:

    LumatoneController();
    ~LumatoneController();

    //============================================================================
    // Methods to configure firmware communication parameters

    sysExSendingMode getSysExSendingMode() const { return editingMode; }
    void setSysExSendingMode(sysExSendingMode newMode);

    FirmwareVersion getFirmwareVersion() const { return firmwareVersion; }

    LumatoneFirmwareVersion getConfirmedVersion() const { return determinedVersion; }
    
    // Takes a generic firmware version and parses it into a recognized firmware version
    void setFirmwareVersion(FirmwareVersion firmwareVersionIn);

    // Takes a recognized firmware version
    void setFirmwareVersion(LumatoneFirmwareVersion lumatoneVersion, bool parseVersion = true);

    int getOctaveSize() const { return octaveSize; }

    void refreshAvailableMidiDevices();

    Array<MidiDeviceInfo> getMidiInputList() { return midiDriver.getMidiInputList(); }
    Array<MidiDeviceInfo> getMidiOutputList() { return midiDriver.getMidiOutputList(); }

    int getMidiInputIndex() const { return midiDriver.getMidiInputIndex(); }
    int getMidiOutputIndex() const { return midiDriver.getMidiOutputIndex(); }

    void setMidiInput(int deviceIndex);
    void setMidiOutput(int deviceIndex);

    bool isConnected() const { return midiDriver.hasDevicesDefined() && currentDevicePairConfirmed; }

    bool requestFirmwareUpdate(File firmwareFile, FirmwareTransfer::ProcessListener* listenerIn = nullptr);

    // Auto-connection and monitoring 

    void detectAndConnectToLumatone();
    void stopAutoConnection();

    bool isDetectingLumatone() const { return deviceMonitor.willDetectDeviceIfDisconnected() && !deviceMonitor.isConnectionEstablished(); }
    void setDeviceDetectionTimeout(int responseTimeoutMs) { deviceMonitor.setResponseTimeoutMs(responseTimeoutMs); }

    bool willCheckConnectionWhenInactvie() const { return deviceMonitor.willCheckForInactivity(); }
    void checkConnectionWhenInactive(bool checkWhenInactive) { deviceMonitor.setCheckForInactivity(checkWhenInactive); }

    //============================================================================
    // Combined (hi-level) commands

    // Send all parametrizations of one sub board
    void sendAllParamsOfBoard(int boardIndex, TerpstraKeys boardData);

    // Send and save a complete key mapping
    void sendCompleteMapping(TerpstraKeyMapping mappingData);

    // Send request to receive the current mapping of one sub board on the controller
    void sendGetMappingOfBoardRequest(int boardIndex);

    // Send request to receive the complete current mapping on the controller
    void sendGetCompleteMappingRequest();

    // Send parametrization of one key to the device
    void sendKeyParam(int boardIndex, int keyIndex, TerpstraKey keyData);

    // Send configuration of a certain look up table
    void sendTableConfig(TerpstraVelocityCurveConfig::VelocityCurveType velocityCurveType, const uint8* table);

    // Reset configuration of a certain look up table to factory settings
    void resetVelocityConfig(TerpstraVelocityCurveConfig::VelocityCurveType velocityCurveType);

    // Ping a device cached in the device list, will primarily use GetSerialIdentity, or Ping if determined version happens to be >= 1.0.9
    unsigned int sendTestMessageToDevice(int deviceIndex, unsigned int pingId);

    // Sends a generic test message to the current device pair to see if we get an expected response
    void testCurrentDeviceConnection();

    //============================================================================
    // Single (mid-level) commands

    // Send note, channel, cc, and fader polarity data
    void sendKeyConfig(int boardIndex, int keyIndex, int noteOrCCNum, int channel, LumatoneKeyType keyType, bool faderUpIsNull = true);

    void sendKeyColourConfig(int boardIndex, int keyIndex, Colour colour);

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
    void sendVelocityConfig(const uint8 velocityTable[]);

    // Save velocity config to EEPROM
    void saveVelocityConfig();

    void resetVelocityConfig();

    void setFaderConfig(const uint8 faderTable[]);

    void resetFaderConfig();

    void setAftertouchEnabled(bool isEnabled);

    void startCalibrateAftertouch();

    void setAftertouchConfig(const uint8 aftertouchTable[]);

    void resetAftertouchConfig();

    void setCalibratePitchModWheel(bool startCalibration);

    void setVelocityIntervalConfig(const int velocityIntervalTable[]);

    void getRedLEDConfig(int boardIndex);

    void getGreenLEDConfig(int boardIndex);

    void getBlueLEDConfig(int boardIndex);

    void getChannelConfig(int boardIndex);

    void getNoteConfig(int boardIndex);

    void getKeyTypeConfig(int boardIndex);

    void sendVelocityConfigRequest();

    // CMD 1Eh: Read back the current fader look up table of the keyboard.
    void sendFaderConfigRequest();

    // CMD 1Fh: Read back the current aftertouch look up table of the keyboard.
    void sendAftertouchConfigRequest();

    void sendVelocityIntervalConfigRequest();

    // This command is used to read back the serial identification number of the keyboard.
    void sendGetSerialIdentityRequest();

    void startCalibrateKeys();

    void setLumatouchConfig(const uint8 lumatouchTable[]);

    void resetLumatouchConfig();

    void getLumatouchConfig();

    // This command is used to read back the current Lumatone firmware revision.
    void sendGetFirmwareRevisionRequest();

    // Send a value from 0-127 for the Lumatone to echo back, returns actual value sent (in case of 7-bit masking); used for auto device connection and monitoring
    int pingLumatone(uint8 pingId);

    //============================================================================
    // FirmwareTransfer::Listener

    void firmwareTransferUpdate(FirmwareTransfer::StatusCode statusCode, String msg) override;

    void exitSignalSent() override;
    
    //============================================================================
    // juce::Timer implementation

    void timerCallback() override;
    
    //============================================================================
    // juce::ChangeListener implementation

    void changeListenerCallback(ChangeBroadcaster* source) override;

protected:
    //============================================================================
    // Implementation of TerpstraMidiDriver::Listener

    virtual void midiMessageReceived(MidiInput* source, const MidiMessage& midiMessage) override;

    virtual void midiMessageSent(const MidiMessage& midiMessage) override;

    virtual void midiSendQueueSize(int queueSize) override;

    virtual void generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel) override;

    virtual void noAnswerToMessage(const MidiMessage& midiMessage) override;

public:
    //============================================================================
    // Public interface for Lumatone connection status

    class StatusListener
    {
    public:

        virtual void connectionEstablished(int inputMidiDevice, int outputMidiDevice) {};
        virtual void connectionLost() {};
    };

    ListenerList<StatusListener> statusListeners;
    void    addStatusListener(StatusListener* listenerIn) { statusListeners.add(listenerIn); }
    void removeStatusListener(StatusListener* listenerIn) { statusListeners.remove(listenerIn); }

private:
    
    void testResponseReceived();

    void onConnectionConfirmed(bool sendChangeSignal);
    void onDisconnection();
    void onFirmwareUpdateReceived();

public:
    //============================================================================
    // Public interface for Lumatone firmware communication
    class FirmwareListener
    {
    public:

        // rgbFlag uses 0 for red, 1 for green, 2 for blue
        virtual void octaveColourConfigReceived(int octaveIndex, uint8 rgbFlag, const int* colourData) {};

        virtual void octaveChannelConfigReceived(int octaveIndex, const int* channelData) {};

        virtual void octaveNoteConfigReceived(int octaveIndex, const int* noteData) {};

        virtual void keyTypeConfigReceived(int octaveIndex, const int* keyTypeData) {};

        virtual void velocityConfigReceived(const int* velocityData) {};

        virtual void aftertouchConfigReceived(const int* aftertouch) {};

        virtual void velocityIntervalConfigReceived(const int* velocityData) {};

        virtual void faderConfigReceived(const int* faderData) {};

        virtual void serialIdentityReceived(int inputDeviceIndex, const int* serialBytes) {};

        virtual void lumatouchConfigReceived(const int* lumatouchData) {};

        virtual void firmwareRevisionReceived(FirmwareVersion version) {};

        virtual void pingResponseReceived(int inputDeviceIndex, unsigned int pingValue) {};

        virtual void peripheralMidiChannelsReceived(int pitchWheelChannel, int modWheelChannel, int expressionChannel, int sustainPedalChannel) {};

        virtual void pedalCalibrationDataReceived(int minBound, int maxBound, bool pedalIsActive) {};

        virtual void wheelsCalibrationDataReceived(int minPitch, int maxPitch, int minMod, int maxMod) {};

        virtual void noAnswerToCommand(int cmd) {};
    };

    ListenerList<FirmwareListener> firmwareListeners;

    void    addFirmwareListener(FirmwareListener* listenerIn) { firmwareListeners.add(listenerIn); }
    void removeFirmwareListener(FirmwareListener* listenerIn) { firmwareListeners.remove(listenerIn); }


    class EditorListener
    {
    public:

        virtual void keyFunctionConfigurationChanged(int octaveNumber, int keyNumber, int noteOrCC, int midiChannel, LumatoneKeyType keyType, bool faderUpIsNull) {};

        virtual void keyColourConfigurationChanged(int octaveNumber, int keyNumber, Colour keyColour) {};
    };

    ListenerList<EditorListener> editorListeners;

    void    addEditorListener(EditorListener* listenerIn) { editorListeners.add(listenerIn); }
    void removeEditorListener(EditorListener* listenerIn) { editorListeners.remove(listenerIn); }

    class MidiListener
    {
    public:

        virtual void handleMidiMessage(const MidiMessage& msg) = 0;
    };

    ListenerList<MidiListener> midiListeners;

    void    addMidiListener(MidiListener* listenerIn) { midiListeners.add(listenerIn); }
    void removeMidiListener(MidiListener* listenerIn) { midiListeners.remove(listenerIn); }

private:
    //============================================================================
    // Lower-level message unpacking and handling

    FirmwareSupport::Error handleOctaveConfigResponse(
        const MidiMessage& midiMessage, 
        std::function <FirmwareSupport::Error(const MidiMessage&, int&, uint8, int*)> unpackFunction,
        std::function <void(int,void*)> callbackFunctionIfNoError = {}
    );

    FirmwareSupport::Error handleTableConfigResponse(
        const MidiMessage& midiMessage,
        std::function<FirmwareSupport::Error(const MidiMessage&, int*)> unpackFunction,
        std::function <void(void*)> callbackFunctionIfNoError = {}
    );

    FirmwareSupport::Error handleLEDConfigResponse(const MidiMessage& midiMessage);

    FirmwareSupport::Error handleChannelConfigResponse(const MidiMessage& midiMessage);

    FirmwareSupport::Error handleNoteConfigResponse(const MidiMessage& midiMessage);

    FirmwareSupport::Error handleKeyTypeConfigResponse(const MidiMessage& midiMessage);

    FirmwareSupport::Error handleVelocityConfigResponse(const MidiMessage& midiMessage);

    FirmwareSupport::Error handleAftertouchConfigResponse(const MidiMessage& midiMessage);

    FirmwareSupport::Error handleVelocityIntervalConfigResponse(const MidiMessage& midiMessage);

    FirmwareSupport::Error handleFaderConfigResponse(const MidiMessage& midiMessage);
    
    FirmwareSupport::Error handleSerialIdentityResponse(const MidiMessage& midiMessage);

    FirmwareSupport::Error handleFirmwareRevisionResponse(const MidiMessage& midiMessage);

    FirmwareSupport::Error handleLumatouchConfigResponse(const MidiMessage& midiMessage);

    FirmwareSupport::Error handlePingResponse(const MidiMessage& midiMessage);

    void handleMidiDriverError(FirmwareSupport::Error errorToHandle, int commandReceived = -1);

private:

    CriticalSection             criticalSection;

    FirmwareSupport             firmwareSupport;

    String                      connectedSerialNumber;
    LumatoneFirmwareVersion     determinedVersion = LumatoneFirmwareVersion::NO_VERSION;
    FirmwareVersion             firmwareVersion = { 0, 0, 0 };
    FirmwareVersion             incomingVersion = { 0, 0, 0 };
    int                         octaveSize = 56;

    HajuErrorVisualizer         errorVisualizer;
    TerpstraMidiDriver          midiDriver;
    DeviceActivityMonitor       deviceMonitor;

    std::unique_ptr<FirmwareTransfer> firmwareTransfer;

    MidiBuffer                  responseQueue;
    int                         readSample = 0;
    int                         sampleNum = 0;
    const int                   bufferReadTimeoutMs = 50;
    const int                   bufferReadSize = 16;
    bool                        bufferReadRequested = false;

    int                         lastTestDeviceSent = -1;
    int                         lastTestDeviceResponded = -1;
    bool                        waitingForTestResponse = false;
    bool                        currentDevicePairConfirmed = false;
    
    LumatoneController::sysExSendingMode editingMode = sysExSendingMode::offlineEditor;
};
