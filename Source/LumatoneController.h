/*
  ==============================================================================

    LumatoneCommandManager.h
    Created: 17 Apr 2021 10:17:31pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "ApplicationListeners.h"
#include "KeyboardDataStructure.h"
#include "TerpstraMidiDriver.h"
#include "DeviceActivityMonitor.h"
#include "FirmwareTransfer.h"

//==============================================================================
// Helper class for parsing and comparing (todo) firmware versions



class LumatoneController :  private TerpstraMidiDriver::Collector,
                            protected FirmwareTransfer::ProcessListener, // Firmware transfer updates
                            private juce::Thread::Listener,              // Firmware thread exited
                            private juce::Timer,
                            private juce::ChangeListener
{

public:

    LumatoneController();
    ~LumatoneController();
    
    //============================================================================
    // Methods to configure firmware communication parameters

    sysExSendingMode getEditorMode() const { return editingMode; }
    void setSysExSendingMode(sysExSendingMode newMode);

    FirmwareVersion getFirmwareVersion() const { return firmwareVersion; }

    LumatoneFirmwareVersion getConfirmedVersion() const { return determinedVersion; }

    const FirmwareSupport& getFirmwareSupport() const { return firmwareSupport; }

    int getOctaveSize() const { return octaveSize; }
    
    int getNumBoards() const { return BOARD_OCT_5; } // TODO: Set variable based on serial/version

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

    bool isDetectingLumatone() const { return deviceMonitor->willDetectDeviceIfDisconnected() && !deviceMonitor->isConnectionEstablished(); }
    void setDeviceDetectionTimeout(int responseTimeoutMs) { deviceMonitor->setResponseTimeoutMs(responseTimeoutMs); }

    bool willCheckConnectionWhenInactvie() const { return deviceMonitor->willCheckForInactivity(); }
    void checkConnectionWhenInactive(bool checkWhenInactive) { deviceMonitor->setCheckForInactivity(checkWhenInactive); }

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
    void sendKeyConfig(int boardIndex, int keyIndex, int noteOrCCNum, int channel, int keyType, bool faderUpIsNull = true);

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

    // CMD 22h: Read back the fader type of all keys on the targeted board.
	void getFaderTypeConfig(int boardIndex);

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

    // Set MIDI Channels of peripheral controllers, pitch & mod wheels, expression & sustain pedals
    void setPeripheralChannels(int pitchWheelChannel, int modWheelChannel, int expressionChannel, int sustainChannel);
    void setPeripheralChannels(PeripheralChannelSettings channelSettings);

    // Get MIDI Channels of peripheral controllers, pitch & mod wheels, expression & sustain pedals
    void getPeripheralChannels();

    // Invert the polarity of the sustain pedal
    void invertSustainPedal(bool setInverted);

    // Reset preset mappings to factory mappings
    void resetPresetsToFactoryDefault();
    void resetPresetToFactoryDefault(int presetIndex);

    // Get interaction flags of current preset
    void getPresetFlags();

    // Get sensitivity setting of expression pedal
    void getExpressionPedalSensitivity();

    //============================================================================
    // FirmwareTransfer::Listener

    void firmwareTransferUpdate(FirmwareTransfer::StatusCode statusCode, String msg) override;

    void exitSignalSent() override;
    
    //============================================================================
    // juce::Timer implementation

    void timerCallback() override;
    
    // Buffer read helpers
    FirmwareSupport::Error getBufferErrorCode(const uint8* sysExData);
    FirmwareSupport::Error handleBufferCommand(const MidiMessage& midiMessage);
    
    //============================================================================
    // juce::ChangeListener implementation

    void changeListenerCallback(ChangeBroadcaster* source) override;


    //============================================================================
    // Test functions

    void loadRandomMapping(int testTimeoutMs, int maxIterations, int i = 0);
    
private:
    
    // Takes a generic firmware version and parses it into a recognized firmware version
    void setFirmwareVersion(FirmwareVersion firmwareVersionIn);

    // Takes a recognized firmware version
    void setFirmwareVersion(LumatoneFirmwareVersion lumatoneVersion, bool parseVersion = true);

protected:
    //============================================================================
    // Implementation of TerpstraMidiDriver::Listener

    virtual void midiMessageReceived(MidiInput* source, const MidiMessage& midiMessage) override;
    virtual void midiMessageSent(MidiOutput* target, const MidiMessage& midiMessage) override;
    virtual void midiSendQueueSize(int queueSize) override;
    virtual void generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel) override;
    virtual void noAnswerToMessage(MidiInput* expectedDevice, const MidiMessage& midiMessage) override;

private:
    
    void confirmAutoConnection();
    void onConnectionConfirm(bool sendChangeSignal);
    void onDisconnection();
    void onFirmwareUpdateReceived();
    
    
private:
    ListenerList<LumatoneEditor::StatusListener> statusListeners;
public:
    void    addStatusListener(LumatoneEditor::StatusListener* listenerIn) { statusListeners.add(listenerIn); }
    void removeStatusListener(LumatoneEditor::StatusListener* listenerIn) { statusListeners.remove(listenerIn); }

private:
    ListenerList<LumatoneEditor::FirmwareListener> firmwareListeners;
public:
    void    addFirmwareListener(LumatoneEditor::FirmwareListener* listenerIn) { firmwareListeners.add(listenerIn); }
    void removeFirmwareListener(LumatoneEditor::FirmwareListener* listenerIn) { firmwareListeners.remove(listenerIn); }

private:
     ListenerList<LumatoneEditor::EditorListener> editorListeners;
public:
     void    addEditorListener(LumatoneEditor::EditorListener* listenerIn) { editorListeners.add(listenerIn); }
     void removeEditorListener(LumatoneEditor::EditorListener* listenerIn) { editorListeners.remove(listenerIn); }

//private:
//    ListenerList<MidiListener> midiListeners;
//public:
//    void    addMidiListener(MidiListener* listenerIn) { midiListeners.add(listenerIn); }
//    void removeMidiListener(MidiListener* listenerIn) { midiListeners.remove(listenerIn); }

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

    FirmwareSupport::Error handleFaderTypeConfigResponse(const MidiMessage& midiMessage);
    
    FirmwareSupport::Error handleSerialIdentityResponse(const MidiMessage& midiMessage);

    FirmwareSupport::Error handleFirmwareRevisionResponse(const MidiMessage& midiMessage);

    FirmwareSupport::Error handleLumatouchConfigResponse(const MidiMessage& midiMessage);

    FirmwareSupport::Error handlePingResponse(const MidiMessage& midiMessage);

    FirmwareSupport::Error handlePeripheralCalibrationData(const MidiMessage& midiMessage);
    FirmwareSupport::Error handleExpressionPedalCalibrationData(const MidiMessage& midiMessage);
    FirmwareSupport::Error handleWheelsCalibrationData(const MidiMessage& midiMessage);

    FirmwareSupport::Error handleGetPeripheralChannelResponse(const MidiMessage& midiMessage);

    FirmwareSupport::Error handleGetPresetFlagsResponse(const MidiMessage& midiMessage);

    FirmwareSupport::Error handleGetExpressionPedalSensitivityResponse(const MidiMessage& midiMessage);

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
    
    std::unique_ptr<DeviceActivityMonitor>  deviceMonitor;

    std::unique_ptr<FirmwareTransfer>       firmwareTransfer;

//    MidiBuffer                  responseQueue;
//    int                         readSample = 0;
//    int                         sampleNum = 0;
    const int                   bufferReadTimeoutMs = 30;
    const int                   bufferReadSize = 16;
    bool                        bufferReadRequested = false;

    std::atomic<int>            readQueueSize;
    int                         sendQueueSize = 0;

    int                         lastTestDeviceSent = -1;
    int                         lastTestDeviceResponded = -1;
    bool                        waitingForTestResponse = false;
    bool                        currentDevicePairConfirmed = false;
    
    sysExSendingMode editingMode = sysExSendingMode::offlineEditor;
};
