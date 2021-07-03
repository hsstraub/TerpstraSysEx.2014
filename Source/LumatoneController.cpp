/*
  ==============================================================================

    LumatoneCommandManager.cpp
    Created: 17 Apr 2021 10:17:31pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "LumatoneController.h"
#include "Main.h"


LumatoneController::LumatoneController()
    : errorVisualizer(TerpstraSysExApplication::getApp().getLookAndFeel()), deviceMonitor(midiDriver)
{
    midiDriver.addListener(this);
    deviceMonitor.addChangeListener(this);
}

LumatoneController::~LumatoneController()
{

}

void LumatoneController::setSysExSendingMode(sysExSendingMode newMode)
{
    // Might be worth it to provide further context of commands, such as which ones are mutable or not
    // And use an automated system for determining whether or not to send a message in live or offline mode

    if (newMode != editingMode)
    {
        editingMode = newMode;
        if (editingMode == sysExSendingMode::offlineEditor)
        {
            midiDriver.clearMIDIMessageBuffer();	// ToDo remove only SysEx messages (leave NoteOn/NoteOff)?
            stopTimer();
        }
    }

    midiDriver.restrictToRequestMessages(editingMode == sysExSendingMode::offlineEditor);
}

// Takes a generic firmware version and parses it into a recognized firmware version
void LumatoneController::setFirmwareVersion(FirmwareVersion firmwareVersionIn)
{
    firmwareVersion = firmwareVersionIn;
    setFirmwareVersion(firmwareSupport.getLumatoneFirmwareVersion(firmwareVersion), false);
}

// Takes a recognized firmware version
void LumatoneController::setFirmwareVersion(LumatoneFirmwareVersion lumatoneVersion, bool parseVersion)
{
    determinedVersion = lumatoneVersion;
    octaveSize = firmwareSupport.getOctaveSize(determinedVersion);

    if (parseVersion)
        firmwareVersion = FirmwareVersion::fromDeterminedVersion(determinedVersion);

    firmwareListeners.call(&FirmwareListener::firmwareRevisionReceived, firmwareVersion);
}


void LumatoneController::setMidiInput(int deviceIndex)
{
    midiDriver.setMidiInput(deviceIndex);
    testCurrentDeviceConnection();
}

void LumatoneController::setMidiOutput(int deviceIndex)
{
    midiDriver.setMidiOutput(deviceIndex);
    testCurrentDeviceConnection();
}

void LumatoneController::detectAndConnectToLumatone()
{
    deviceMonitor.setDetectDeviceIfDisconnected(true);
}

void LumatoneController::stopAutoConnection()
{
    deviceMonitor.setDetectDeviceIfDisconnected(false);
}

void LumatoneController::refreshAvailableMidiDevices() 
{ 
    if (midiDriver.refreshDeviceLists() && midiDriver.testIsIncomplete())
        midiDriver.openAvailableDevicesForTesting();
}

bool LumatoneController::requestFirmwareUpdate(File firmwareFile, FirmwareTransfer::ProcessListener* listenerIn)
{
    if (firmwareTransfer == nullptr)
    {
        incomingVersion = FirmwareVersion(0, 0, 0);
        firmwareTransfer.reset(new FirmwareTransfer(midiDriver));
        firmwareTransfer->addTransferListener(this);
        firmwareTransfer->addListener(this);

        if (listenerIn != nullptr)
        {
            firmwareTransfer->addTransferListener(listenerIn);
        }

        return firmwareTransfer->requestFirmwareUpdate(firmwareFile.getFullPathName());
    }
    else
        jassertfalse;

    return false;
}

/*
==============================================================================
Combined (hi-level) commands
*/


void LumatoneController::sendAllParamsOfBoard(int boardIndex, TerpstraKeys boardData)
{
    if (determinedVersion >= LumatoneFirmwareVersion::VERSION_1_0_11)
    {
        for (int keyIndex = 0; keyIndex < octaveSize; keyIndex++)
        {
            auto key = &boardData.theKeys[keyIndex];
            midiDriver.sendKeyFunctionParameters(boardIndex, keyIndex, key->noteNumber, key->channelNumber, key->keyType & 0x3);
            midiDriver.sendKeyLightParameters(boardIndex, keyIndex, key->colour.getRed(), key->colour.getGreen(), key->colour.getBlue());
        }
    }
    else
    {
        for (int keyIndex = 0; keyIndex < octaveSize; keyIndex++)
        {
            auto key = &boardData.theKeys[keyIndex];
            midiDriver.sendKeyFunctionParameters(boardIndex, keyIndex, key->noteNumber, key->channelNumber, key->keyType & 0x3);
            midiDriver.sendKeyLightParameters_Version_1_0_0(boardIndex, keyIndex, key->colour.getRed() / 2, key->colour.getGreen() / 2, key->colour.getBlue() / 2);
        }
    }
}

void LumatoneController::sendCompleteMapping(TerpstraKeyMapping mappingData)
{
    for (int boardIndex = 1; boardIndex <= NUMBEROFBOARDS; boardIndex++)
        sendAllParamsOfBoard(boardIndex, mappingData.sets[boardIndex - 1]);
}

void LumatoneController::sendGetMappingOfBoardRequest(int boardIndex)
{
    getRedLEDConfig(boardIndex);
    getGreenLEDConfig(boardIndex);
    getBlueLEDConfig(boardIndex);
    getChannelConfig(boardIndex);
    getNoteConfig(boardIndex);
    getKeyTypeConfig(boardIndex);
    getFaderTypeConfig(boardIndex);
}

void LumatoneController::sendGetCompleteMappingRequest()
{
    for (int boardIndex = 1; boardIndex <= NUMBEROFBOARDS; boardIndex++)
        sendGetMappingOfBoardRequest(boardIndex);
}

void LumatoneController::resetVelocityConfig(TerpstraVelocityCurveConfig::VelocityCurveType velocityCurveType)
{
    switch (velocityCurveType)
    {
    case TerpstraVelocityCurveConfig::VelocityCurveType::noteOnNoteOff:
        resetVelocityConfig();
        break;
    case TerpstraVelocityCurveConfig::VelocityCurveType::fader:
        resetFaderConfig();
        break;
    case TerpstraVelocityCurveConfig::VelocityCurveType::afterTouch:
        resetAftertouchConfig();
        break;
    case TerpstraVelocityCurveConfig::VelocityCurveType::lumaTouch:
        resetLumatouchConfig();
        break;
    default:
        jassert(false);
        break;
    }
}

unsigned int LumatoneController::sendTestMessageToDevice(int deviceIndex, unsigned int pingId)
{
    if (!midiDriver.testIsIncomplete())
    {
        midiDriver.openAvailableDevicesForTesting();
    }

    int value = (pingId < 0)
        ? deviceIndex
        : pingId &= 0xFFFFFFF;

    if (determinedVersion >= LumatoneFirmwareVersion::VERSION_1_0_9)
    {
        midiDriver.ping(value, deviceIndex);
    }
    else
    {
        midiDriver.sendGetSerialIdentityRequest(deviceIndex);
    }

    lastTestDeviceSent = deviceIndex;
    waitingForTestResponse = true;

    return value;
}

void LumatoneController::testCurrentDeviceConnection()
{
    // On confirmed connection send connection listener message
    if (midiDriver.hasDevicesDefined())
    {
        waitingForTestResponse = true;

        if (determinedVersion >= LumatoneFirmwareVersion::VERSION_1_0_9)
        {
            pingLumatone(0xf);
        }

        else
        {
            sendGetSerialIdentityRequest();
        }
    }
    else
    {
        currentDevicePairConfirmed = false;
    }
}

// Send parametrization of one key to the device
void LumatoneController::sendKeyParam(int boardIndex, int keyIndex, TerpstraKey keyData)
{    
    // Default CC polarity = 1, Inverted CC polarity = 0
    sendKeyConfig(boardIndex, keyIndex, keyData.noteNumber, keyData.channelNumber, keyData.keyType, keyData.ccFaderDefault);
    sendKeyColourConfig(boardIndex, keyIndex, keyData.colour);
}

// Send configuration of a certain look up table
void LumatoneController::sendTableConfig(TerpstraVelocityCurveConfig::VelocityCurveType velocityCurveType, const uint8* table)
{
    switch (velocityCurveType)
    {
    case TerpstraVelocityCurveConfig::VelocityCurveType::fader:
        setFaderConfig(table);
        break;

    case TerpstraVelocityCurveConfig::VelocityCurveType::afterTouch:
        setAftertouchConfig(table);
        break;

    case TerpstraVelocityCurveConfig::VelocityCurveType::lumaTouch:
        setLumatouchConfig(table);
        break;

    default:
        sendVelocityConfig(table);
    }
}

//=============================================================================
// Mid-level firmware functions

// Send note, channel, cc, and fader polarity data
void LumatoneController::sendKeyConfig(int boardIndex, int keyIndex, int noteOrCCNum, int channel, int keyType, bool faderUpIsNull)
{
    midiDriver.sendKeyFunctionParameters(boardIndex, keyIndex, noteOrCCNum, channel, keyType & 0x3, faderUpIsNull);
}

void LumatoneController::sendKeyColourConfig(int boardIndex, int keyIndex, Colour colour)
{
    if (determinedVersion >= LumatoneFirmwareVersion::VERSION_1_0_11)
        midiDriver.sendKeyLightParameters(boardIndex, keyIndex, colour.getRed(), colour.getGreen(), colour.getBlue());
    else
        midiDriver.sendKeyLightParameters_Version_1_0_0(boardIndex, keyIndex, colour.getRed() / 2, colour.getGreen() / 2, colour.getBlue() / 2);
}


// Send expression pedal sensivity
void LumatoneController::sendExpressionPedalSensivity(unsigned char value)
{
    midiDriver.sendExpressionPedalSensivity(value);
}

// Send parametrization of foot controller
void LumatoneController::sendInvertFootController(bool value)
{
    midiDriver.sendInvertFootController(value);
}

// Colour for macro button in active state
void LumatoneController::sendMacroButtonActiveColour(String colourAsString)
{
    auto c = Colour::fromString(colourAsString);
    if (determinedVersion >= LumatoneFirmwareVersion::VERSION_1_0_11)
        midiDriver.sendMacroButtonActiveColour(c.getRed(), c.getGreen(), c.getBlue());
    else
        midiDriver.sendMacroButtonActiveColour_Version_1_0_0(c.getRed(), c.getGreen(), c.getBlue());
}

// Colour for macro button in inactive state
void LumatoneController::sendMacroButtonInactiveColour(String colourAsString)
{
    auto c = Colour::fromString(colourAsString);
    if (determinedVersion >= LumatoneFirmwareVersion::VERSION_1_0_11)
        midiDriver.sendMacroButtonInactiveColour(c.getRed(), c.getGreen(), c.getBlue());
    else
        midiDriver.sendMacroButtonInactiveColour_Version_1_0_0(c.getRed(), c.getGreen(), c.getBlue());
}

// Send parametrization of light on keystrokes
void LumatoneController::sendLightOnKeyStrokes(bool value)
{
    midiDriver.sendLightOnKeyStrokes(value);
}

// Send a value for a velocity lookup table
void LumatoneController::sendVelocityConfig(const uint8 velocityTable[])
{
    midiDriver.sendVelocityConfig(velocityTable);
}

// Save velocity config to EEPROM
void LumatoneController::saveVelocityConfig()
{
    midiDriver.saveVelocityConfig();
}

void LumatoneController::resetVelocityConfig()
{
    midiDriver.resetVelocityConfig();
}

void LumatoneController::setFaderConfig(const uint8 faderTable[])
{
    midiDriver.sendFaderConfig(faderTable);
}

void LumatoneController::resetFaderConfig()
{
    midiDriver.resetFaderConfig();
}

void LumatoneController::setAftertouchEnabled(bool value)
{
    midiDriver.sendAfterTouchActivation(value);
}

void LumatoneController::startCalibrateAftertouch()
{
    midiDriver.sendCalibrateAfterTouch();
}

void LumatoneController::setAftertouchConfig(const uint8 aftertouchTable[])
{
    midiDriver.sendAftertouchConfig(aftertouchTable);
}

void LumatoneController::resetAftertouchConfig()
{
    midiDriver.resetAftertouchConfig();
}

void LumatoneController::setVelocityIntervalConfig(const int velocityIntervalTable[])
{
    midiDriver.sendVelocityIntervalConfig(velocityIntervalTable);
}

void LumatoneController::getRedLEDConfig(int boardIndex)
{
    midiDriver.sendRedLEDConfigRequest(boardIndex);
}

void LumatoneController::getGreenLEDConfig(int boardIndex)
{
    midiDriver.sendGreenLEDConfigRequest(boardIndex);
}

void LumatoneController::getBlueLEDConfig(int boardIndex)
{
    midiDriver.sendBlueLEDConfigRequest(boardIndex);
}

void LumatoneController::getChannelConfig(int boardIndex)
{
    midiDriver.sendChannelConfigRequest(boardIndex);
}

void LumatoneController::getNoteConfig(int boardIndex)
{
    midiDriver.sendNoteConfigRequest(boardIndex);
}

void LumatoneController::getKeyTypeConfig(int boardIndex)
{
    midiDriver.sendKeyTypeConfigRequest(boardIndex);
}

void LumatoneController::sendVelocityConfigRequest()
{
    midiDriver.sendVelocityConfigRequest();
}

void LumatoneController::sendFaderConfigRequest()
{
    midiDriver.sendFaderConfigRequest();
}

void LumatoneController::sendAftertouchConfigRequest()
{
    midiDriver.sendAftertouchConfigRequest();
}

void LumatoneController::sendVelocityIntervalConfigRequest()
{
    midiDriver.sendVelocityIntervalConfigRequest();
}

void LumatoneController::getFaderTypeConfig(int boardIndex)
{
    midiDriver.sendFaderTypeConfigRequest(boardIndex);
}

// This command is used to read back the serial identification number of the keyboard.
void LumatoneController::sendGetSerialIdentityRequest()
{
    midiDriver.sendGetSerialIdentityRequest();
}

void LumatoneController::startCalibrateKeys()
{
    midiDriver.sendCalibrateKeys();
}

void LumatoneController::setCalibratePitchModWheel(bool startCalibration)
{
    midiDriver.sendCalibratePitchModWheel(startCalibration);
}

void LumatoneController::setLumatouchConfig(const uint8 lumatouchTable[])
{
    midiDriver.setLumatouchConfig(lumatouchTable);
}

void LumatoneController::resetLumatouchConfig()
{
    midiDriver.resetLumatouchConfig();
}

void LumatoneController::getLumatouchConfig()
{
    midiDriver.sendLumatouchConfigRequest();
}

// This command is used to read back the current Lumatone firmware revision.
void LumatoneController::sendGetFirmwareRevisionRequest()
{
    midiDriver.sendGetFirmwareRevisionRequest();
}

// Send a value for the Lumatone to echo back; used for auto device connection and monitoring
int LumatoneController::pingLumatone(uint8 pingId)
{
    return midiDriver.ping(pingId);
}

void LumatoneController::invertSustainPedal(bool setInverted)
{
    if (firmwareSupport.versionAcknowledgesCommand(determinedVersion, INVERT_SUSTAIN_PEDAL))
        midiDriver.sendInvertSustainPedal(setInverted);
}

void LumatoneController::resetPresetsToFactoryDefault()
{
    if (firmwareSupport.versionAcknowledgesCommand(determinedVersion, RESET_DEFAULT_PRESETS))
        midiDriver.sendResetDefaultPresetsRequest();
}

//=============================================================================
// Communication and broadcasting

void LumatoneController::midiMessageReceived(MidiInput* source, const MidiMessage& midiMessage)
{
    // Todo: no listener calling in this method
    //{
    //    const MessageManagerLock mml;
    //    midiListeners.call(&MidiListener::handleMidiMessage, midiMessage);
    //}

    if (midiMessage.isSysEx())
    {
        if (editingMode != sysExSendingMode::firmwareUpdate)
        {
            // Handle SysEx responses off of high-precision thread
            responseQueue.addEvent(midiMessage, sampleNum++);

            if (!bufferReadRequested)
            {
                bufferReadRequested = true;
                startTimer(bufferReadTimeoutMs);
            }
        }
        
        // Handle firmware update confirmation responses
        else
        {
            auto sysExData = midiMessage.getSysExData();
            switch (sysExData[CMD_ID])
            {
            case GET_FIRMWARE_REVISION:
                midiDriver.unpackGetFirmwareRevisionResponse(midiMessage, incomingVersion.major, incomingVersion.minor, incomingVersion.revision);
                if (incomingVersion.isValid())
                {
                    startTimer(UPDATETIMEOUT);
                    break;
                }

            default:
                break;
            }
        }
    }
    else
    {

    }
}

void LumatoneController::midiMessageSent(const MidiMessage& midiMessage)
{

}

void LumatoneController::midiSendQueueSize(int queueSize)
{

}

void LumatoneController::generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel)
{

}

void LumatoneController::noAnswerToMessage(const MidiMessage& midiMessage)
{
    // Safeguard for getSysExData()
    if (midiMessage.isSysEx())
    {        
        firmwareListeners.call(&FirmwareListener::noAnswerToCommand, midiMessage.getSysExData()[CMD_ID]);
    }
}

FirmwareSupport::Error LumatoneController::handleOctaveConfigResponse(
    const MidiMessage& midiMessage, 
    std::function<FirmwareSupport::Error(const MidiMessage&, int&, uint8, int*)> unpackFunction,
    std::function<void(int,void*)> callbackFunctionIfNoError)
{
    int boardId = -1;
    int channelData[56];
    
    auto errorCode = unpackFunction(midiMessage, boardId, octaveSize, channelData);
    if (errorCode == FirmwareSupport::Error::noError)
    {
        callbackFunctionIfNoError(boardId, channelData);
    }

    return errorCode;
}

FirmwareSupport::Error LumatoneController::handleTableConfigResponse(
    const MidiMessage& midiMessage,
    std::function<FirmwareSupport::Error(const MidiMessage&, int*)> unpackFunction,
    std::function<void(void*)> callbackFunctionIfNoError)
{
    int veloctiyData[128];
    auto errorCode = unpackFunction(midiMessage, veloctiyData);
    if (errorCode == FirmwareSupport::Error::noError)
        callbackFunctionIfNoError(veloctiyData);
    
    return errorCode;
}

FirmwareSupport::Error LumatoneController::handleLEDConfigResponse(const MidiMessage& midiMessage)
{
    int boardId = -1;
    int colourData[56];

    int cmd = firmwareSupport.getCommandNumber(midiMessage);
    FirmwareSupport::Error errorCode;

    // Use correct unpacking function
    if (determinedVersion < LumatoneFirmwareVersion::VERSION_1_0_11)
    {
        errorCode = midiDriver.unpackGetLEDConfigResponse_Version_1_0_0(midiMessage, boardId, octaveSize, colourData);
    }
    else
    {
        errorCode = midiDriver.unpackGetLEDConfigResponse(midiMessage, boardId, colourData);
    }

    if (errorCode == FirmwareSupport::Error::noError)
    {
        int colorCode = cmd - GET_RED_LED_CONFIG;
        firmwareListeners.call(&FirmwareListener::octaveColourConfigReceived, boardId, colorCode, colourData);
    }
    
    return errorCode;
}

FirmwareSupport::Error LumatoneController::handleChannelConfigResponse(const MidiMessage& midiMessage)
{
    auto unpack = [&](const MidiMessage& msg, int& boardId, uint8 numKeys, int* data) {
        auto errorCode = midiDriver.unpackGetChannelConfigResponse(msg, boardId, numKeys, data);
        if (errorCode == FirmwareSupport::Error::noError)
            for (int i = 0; i < numKeys; i++)
                data[i]++; // MIDI Channels are 1-based
        return errorCode;
    };
    auto callback = [&](int boardId, void* data) { firmwareListeners.call(&FirmwareListener::octaveChannelConfigReceived, boardId, (int*)data); };
    return handleOctaveConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneController::handleNoteConfigResponse(const MidiMessage& midiMessage)
{
    auto unpack = [&](const MidiMessage& msg, int& boardId, uint8 numKeys, int* data) {
        return midiDriver.unpackGetNoteConfigResponse(msg, boardId, numKeys, data);
    };
    auto callback = [&](int boardId, void* data) { firmwareListeners.call(&FirmwareListener::octaveNoteConfigReceived, boardId, (int*)data); };
    return handleOctaveConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneController::handleKeyTypeConfigResponse(const MidiMessage& midiMessage)
{
    auto unpack = [&](const MidiMessage& msg, int& boardId, uint8 numKeys, int* data) {
        return midiDriver.unpackGetTypeConfigResponse(msg, boardId, numKeys, data);
    };
    auto callback = [&](int boardId, void* data) { firmwareListeners.call(&FirmwareListener::keyTypeConfigReceived, boardId, (int*)data); };
    return handleOctaveConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneController::handleVelocityConfigResponse(const MidiMessage& midiMessage)
{
    auto unpack = [&](const MidiMessage& msg, int* data) {
        return midiDriver.unpackGetVelocityConfigResponse(msg, data);
    };
    auto callback = [&](void* data) { firmwareListeners.call(&FirmwareListener::velocityConfigReceived, (int*)data); };
    return handleTableConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneController::handleAftertouchConfigResponse(const MidiMessage& midiMessage)
{
    auto unpack = [&](const MidiMessage& msg, int* data) {
        return midiDriver.unpackGetAftertouchConfigResponse(msg, data);
    };
    auto callback = [&](void* data) { firmwareListeners.call(&FirmwareListener::aftertouchConfigReceived, (int*)data); };
    return handleTableConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneController::handleVelocityIntervalConfigResponse(const MidiMessage& midiMessage)
{
    auto unpack = [&](const MidiMessage& msg, int* data) {
        return midiDriver.unpackGetVelocityIntervalConfigResponse(msg, data);
    };
    auto callback = [&](void* data) { firmwareListeners.call(&FirmwareListener::velocityIntervalConfigReceived, (int*)data); };
    return handleTableConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneController::handleFaderConfigResponse(const MidiMessage& midiMessage)
{
    auto unpack = [&](const MidiMessage& msg, int* data) {
        return midiDriver.unpackGetFaderConfigResponse(msg, data);
    };
    auto callback = [&](void* data) { firmwareListeners.call(&FirmwareListener::faderConfigReceived, (int*)data); };
    return handleTableConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneController::handleFaderTypeConfigResponse(const MidiMessage& midiMessage)
{
    auto unpack = [&](const MidiMessage& msg, int& boardId, uint8 numKeys, int* data) {
        return midiDriver.unpackGetTypeConfigResponse(msg, boardId, numKeys, data);
    };
    auto callback = [&](int boardId, void* data) { firmwareListeners.call(&FirmwareListener::faderTypeConfigReceived, boardId, (int*)data); };
    return handleOctaveConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneController::handleSerialIdentityResponse(const MidiMessage& midiMessage)
{
    int serialBytes[6];
    auto errorCode = midiDriver.unpackGetSerialIdentityResponse(midiMessage, serialBytes);
    if (errorCode != FirmwareSupport::Error::noError)
        return errorCode;

    connectedSerialNumber = firmwareSupport.serialIdentityToString(serialBytes);
    DBG("Device serial is: " + connectedSerialNumber);

    testResponseReceived();

    firmwareListeners.call(&FirmwareListener::serialIdentityReceived, lastTestDeviceResponded, serialBytes);

    return errorCode;
}

FirmwareSupport::Error LumatoneController::handleFirmwareRevisionResponse(const MidiMessage& midiMessage)
{
    int major, minor, revision;
    auto errorCode = midiDriver.unpackGetFirmwareRevisionResponse(midiMessage, major, minor, revision);
    if (errorCode == FirmwareSupport::Error::noError)
    {
        // Listener call handled here
        setFirmwareVersion(FirmwareVersion(major, minor, revision));
    }

    return errorCode;
}

FirmwareSupport::Error LumatoneController::handleLumatouchConfigResponse(const MidiMessage& midiMessage)
{
    auto unpack = [&](const MidiMessage& msg, int* data) {
        return midiDriver.unpackGetLumatouchConfigResponse(msg, data);
    };
    auto callback = [&](void* data) { firmwareListeners.call(&FirmwareListener::lumatouchConfigReceived, (int*)data); };
    return handleTableConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneController::handlePingResponse(const MidiMessage& midiMessage)
{
    unsigned int value = 0;
    auto errorCode = midiDriver.unpackPingResponse(midiMessage, value);
    
    if (errorCode != FirmwareSupport::Error::noError)
        return errorCode;

    testResponseReceived();

    firmwareListeners.call(&FirmwareListener::pingResponseReceived, lastTestDeviceResponded, value);

    return errorCode;
}

void LumatoneController::handleMidiDriverError(FirmwareSupport::Error errorToHandle, int commandReceived)
{
    DBG("ERROR from command " + String(commandReceived) + ": " + firmwareSupport.errorToString(errorToHandle));

    // Generic handling
    if (commandReceived < CHANGE_KEY_NOTE)
    {

    }
    
    // Command specific
    else
    {
        switch (errorToHandle) {
            case FirmwareSupport::Error::messageIsAnEcho:
                return;
                
            case FirmwareSupport::Error::messageHasInvalidStatusByte:
                return;

            default:
                break;
        }
    }
    
    jassertfalse;
}

void LumatoneController::firmwareTransferUpdate(FirmwareTransfer::StatusCode statusCode, String msg)
{
    switch (statusCode)
    {
    case FirmwareTransfer::StatusCode::SessionBegin:
        midiDriver.clearMIDIMessageBuffer();
        deviceMonitor.stopMonitoringDevice();
        waitingForTestResponse = false; // In case connection test was in progress
        break;

    case FirmwareTransfer::StatusCode::InstallBegin:
        editingMode = sysExSendingMode::firmwareUpdate;
        startTimer(UPDATETIMEOUT);
        break;
            
    default:
        if (statusCode < FirmwareTransfer::StatusCode::NoErr)
        {
            // Update failed
            deviceMonitor.intializeConnectionLossDetection();
            juce::Timer::callAfterDelay(20, [&] { firmwareTransfer->signalThreadShouldExit(); });
        }
        break;
    }
}

void LumatoneController::timerCallback()
{
    stopTimer();

    if (editingMode != firmwareUpdate)
    {
        if (readSample >= sampleNum)
        {
            sampleNum = 0;
            readSample = 0;
            responseQueue.clear();
        }
        else
        {
            MidiBuffer readBuffer;
            readBuffer.addEvents(responseQueue, readSample, bufferReadSize, 0);
            int maxSample = readSample + bufferReadSize;
            for (auto event : readBuffer)
            {
                auto midiMessage = event.getMessage();
                DBG("READING: " + midiMessage.getDescription());
                if (midiMessage.isSysEx())
                {
                    auto errorCode = FirmwareSupport::Error::noError;
                    auto sysExData = midiMessage.getSysExData();

                    switch (sysExData[MSG_STATUS])
                    {
                    case TerpstraMIDIAnswerReturnCode::NACK:  // Not recognized
                        errorVisualizer.setErrorLevel(
                            HajuErrorVisualizer::ErrorLevel::error,
                            "Not Recognized");
                        errorCode = FirmwareSupport::Error::unknownCommand;
                        break;

                    case TerpstraMIDIAnswerReturnCode::ACK:  // Acknowledged, OK
                        errorVisualizer.setErrorLevel(
                            HajuErrorVisualizer::ErrorLevel::noError,
                            "Ack");
                        break;

                    case TerpstraMIDIAnswerReturnCode::BUSY: // Controller busy
                        errorVisualizer.setErrorLevel(
                            HajuErrorVisualizer::ErrorLevel::warning,
                            "Busy");
                        errorCode = FirmwareSupport::Error::deviceIsBusy;
                        break;

                    case TerpstraMIDIAnswerReturnCode::ERROR:    // Error
                        errorVisualizer.setErrorLevel(
                            HajuErrorVisualizer::ErrorLevel::error,
                            "Error from device");
                        errorCode = FirmwareSupport::Error::externalError;
                        break;

                    case TEST_ECHO:
                        errorCode = FirmwareSupport::Error::messageIsAnEcho;
                        break;

                    default:
                        errorVisualizer.setErrorLevel(
                            HajuErrorVisualizer::ErrorLevel::noError,
                            "");
                        break;
                    }

                    unsigned int cmd = sysExData[CMD_ID];

                    if (errorCode == FirmwareSupport::Error::noError)
                    {
                        switch (cmd)
                        {
                        case GET_RED_LED_CONFIG:
                            errorCode = handleLEDConfigResponse(midiMessage);
                            break;

                        case GET_GREEN_LED_CONFIG:
                            errorCode = handleLEDConfigResponse(midiMessage);
                            break;

                        case GET_BLUE_LED_CONFIG:
                            errorCode = handleLEDConfigResponse(midiMessage);
                            break;

                        case GET_CHANNEL_CONFIG:
                            errorCode = handleChannelConfigResponse(midiMessage);
                            break;

                        case GET_NOTE_CONFIG:
                            errorCode = handleNoteConfigResponse(midiMessage);
                            break;

                        case GET_KEYTYPE_CONFIG:
                            errorCode = handleKeyTypeConfigResponse(midiMessage);
                            break;

                            // TODO

                        case GET_VELOCITY_CONFIG:
                            errorCode = handleVelocityConfigResponse(midiMessage);
                            break;
                                
                        case GET_FADER_TYPE_CONFIGURATION:
                            errorCode = handleFaderTypeConfigResponse(midiMessage);
                            break;

                        case GET_SERIAL_IDENTITY:
                            errorCode = handleSerialIdentityResponse(midiMessage);
                            break;

                        case GET_LUMATOUCH_CONFIG:
                            errorCode = handleLumatouchConfigResponse(midiMessage);
                            break;

                        case GET_FIRMWARE_REVISION:
                            errorCode = handleFirmwareRevisionResponse(midiMessage);
                            break;

                        case LUMA_PING:
                            errorCode = handlePingResponse(midiMessage);
                            break;

                        default:
                            if (sysExData[MSG_STATUS] == 1)
                            {
                                if (event.numBytes > 8)
                                    DBG("WARNING: UNIMPLEMENTED RESPONSE HANDLING CMD " + String(cmd));
                            }
                            else
                            {
                                DBG("UNHANDLED EXCEPTION");
                            }
                        }
                    }

                    if (errorCode != FirmwareSupport::Error::noError)
                    {
                        handleMidiDriverError(errorCode, cmd);
                    }

                }
                readSample++;
                jassert(readSample <= sampleNum);
            }

            if (maxSample < sampleNum)
                startTimer(bufferReadTimeoutMs);
        }
    }
    else
    {
        if (firmwareTransfer != nullptr)
        {
            if (editingMode == sysExSendingMode::firmwareUpdate)
            {
                firmwareTransfer->incrementProgress();

                if (waitingForTestResponse)
                {
                    if (midiDriver.hasDevicesDefined() && incomingVersion.isValid())
                    {
                        waitingForTestResponse = true;
                        onFirmwareUpdateReceived();
                    }
                    
                    // THIS IS A KLUDGE! Something kills DeviceActivityMonitor's timer after device comes back online and I'm not yet sure why - vsicurella
                    else if (!deviceMonitor.isTimerRunning())
                    {
                         deviceMonitor.initializeDeviceDetection();
                    }
                    else
                    {
                        sendGetFirmwareRevisionRequest();
                    }
                }

                // Reset connection and start polling with GetFirmwareRevision
                else if (!incomingVersion.isValid())
                {
                    waitingForTestResponse = true;
                    midiDriver.closeMidiInput();
                    midiDriver.closeMidiOutput();
                    deviceMonitor.initializeDeviceDetection();
                }
                
                startTimer(UPDATETIMEOUT);
            }
        }
        else
        {
            AlertWindow::showMessageBox(
                AlertWindow::AlertIconType::WarningIcon,
                "Firmware update not confirmed",
                "Your Lumatone appears to still be updating, or may have been disconnected. Make sure Lumatone is powered on and connected, and the when Lumatone is ready it will connect successfully.",
                "Ok"); 

            onDisconnection();
        }
    }

    bufferReadRequested = false;
}

void LumatoneController::exitSignalSent()
{
    firmwareTransfer = nullptr;
}

void LumatoneController::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == &deviceMonitor)
    {
        if (!midiDriver.hasDevicesDefined())
        {
            int newInput = deviceMonitor.getConfirmedInputIndex();
            int newOutput = deviceMonitor.getConfirmedOutputIndex();

            currentDevicePairConfirmed = false;

            if (newInput >= 0 && newOutput >= 0)
            {
                midiDriver.setMidiInput(newInput);
                midiDriver.setMidiOutput(newOutput);
                currentDevicePairConfirmed = true;
            }
        }
        else if (deviceMonitor.isConnectionEstablished())
        {
            currentDevicePairConfirmed = true;
        }
        else
        {
            currentDevicePairConfirmed = false;
        }
        
        if (currentDevicePairConfirmed)
            onConnectionConfirmed(editingMode != sysExSendingMode::firmwareUpdate);
        else
            onDisconnection();

    }
}

void LumatoneController::testResponseReceived()
{
    if (waitingForTestResponse)
    {
        waitingForTestResponse = false;

        if (!currentDevicePairConfirmed)
        {
            currentDevicePairConfirmed = true;
            onConnectionConfirmed(true);
        }
    }
}

void LumatoneController::onConnectionConfirmed(bool sendChangeSignal)
{
    if (sendChangeSignal)
        statusListeners.call(&StatusListener::connectionEstablished, midiDriver.getMidiInputIndex(), midiDriver.getMidiOutputIndex());

    // Identity returned on 55-keys version
    if (connectedSerialNumber == SERIAL_55_KEYS)
        setFirmwareVersion(LumatoneFirmwareVersion::VERSION_55_KEYS);
    
    deviceMonitor.intializeConnectionLossDetection();

    TerpstraSysExApplication::getApp().getPropertiesFile()->setValue("LastInputDeviceId", midiDriver.getLastMidiInputInfo().identifier);
    TerpstraSysExApplication::getApp().getPropertiesFile()->setValue("LastOutputDeviceId", midiDriver.getLastMidiOutputInfo().identifier);
}

void LumatoneController::onDisconnection()
{
    midiDriver.closeMidiInput();
    midiDriver.closeMidiOutput();
    
    waitingForTestResponse = false;
    currentDevicePairConfirmed = false;
    lastTestDeviceResponded = -1;
    lastTestDeviceSent = -1;

    statusListeners.call(&StatusListener::connectionLost);
    
    deviceMonitor.initializeDeviceDetection();
}

void LumatoneController::onFirmwareUpdateReceived()
{
    jassert(firmwareTransfer != nullptr && editingMode == sysExSendingMode::firmwareUpdate);

    if (firmwareTransfer != nullptr)
    {
        firmwareTransfer->setProgress(1.0);
        auto possibleUpdate = firmwareSupport.getLumatoneFirmwareVersion(incomingVersion);
        DBG("Waiting for update, received: " + incomingVersion.toString());
        if (possibleUpdate > determinedVersion)
        {
            firmwareVersion = incomingVersion;
            DBG("Confirmed update to firmware version " + firmwareVersion.toString());
            determinedVersion = possibleUpdate;
        }
        else
        {
            DBG("Error: Firmware update appears to have failed");
            //jassertfalse;
        }

        editingMode = sysExSendingMode::liveEditor;
        firmwareListeners.call(&FirmwareListener::firmwareRevisionReceived, firmwareVersion);
        firmwareTransfer->signalThreadShouldExit();

        deviceMonitor.intializeConnectionLossDetection();
    }
}

