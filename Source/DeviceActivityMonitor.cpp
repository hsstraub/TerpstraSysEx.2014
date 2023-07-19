/*
  ==============================================================================

    DeviceActivityMonitor.cpp
    Created: 11 Feb 2021 9:15:43pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "DeviceActivityMonitor.h"
#include "Main.h"


DeviceActivityMonitor::DeviceActivityMonitor(TerpstraMidiDriver& midiDriverIn)
    : midiDriver(midiDriverIn), readQueueSize(0)
{
    detectDevicesIfDisconnected = TerpstraSysExApplication::getApp().getPropertiesFile()->getBoolValue("DetectDeviceIfDisconnected", true);
    checkConnectionOnInactivity = TerpstraSysExApplication::getApp().getPropertiesFile()->getBoolValue("CheckConnectionIfInactive", true);
    responseTimeoutMs = TerpstraSysExApplication::getApp().getPropertiesFile()->getIntValue("DetectDevicesTimeout", responseTimeoutMs);

//    midiDriver.addListener(this);
    reset(readBlockSize);
    midiDriver.addMessageCollector(this);

    // avoid resizing during communication
    ensureStorageAllocated(2000);
    testResponseDeviceIndices.resize(2000);
}

DeviceActivityMonitor::~DeviceActivityMonitor()
{
    removeAllChangeListeners();
    midiDriver.removeMessageCollector(this);
}

void DeviceActivityMonitor::setDetectDeviceIfDisconnected(bool doDetection)
{
    detectDevicesIfDisconnected = doDetection;
    TerpstraSysExApplication::getApp().getPropertiesFile()->setValue("DetectDeviceIfDisconnected", detectDevicesIfDisconnected);

    if (!detectDevicesIfDisconnected)
    {
        deviceConnectionMode = DetectConnectionMode::noDeviceActivity;
        midiDriver.closeTestingDevices();
    }
    else
    {
        startTimer(detectRoutineTimeoutMs);
    }
}

void DeviceActivityMonitor::setCheckForInactivity(bool monitorActivity)
{
    checkConnectionOnInactivity = monitorActivity;
    TerpstraSysExApplication::getApp().getPropertiesFile()->setValue("CheckConnectionIfInactive", checkConnectionOnInactivity);
    
    if (checkConnectionOnInactivity && isConnectionEstablished())
    {
        startTimer(inactivityTimeoutMs);
    }
}

void DeviceActivityMonitor::pingAllDevices()
{
    deviceDetectInProgress = true;

    outputDevices = midiDriver.getMidiOutputList();

    outputPingIds.clear();

    waitingForTestResponse = true;

    int maxDevices = jmin(outputDevices.size(), 128);
    for (int i = 0; i < maxDevices; i++)
    {
        unsigned int id = (unsigned int)i + 1;
        midiDriver.ping(id, i);
        outputPingIds.add(id);
    }

    startTimer(responseTimeoutMs);
}

bool DeviceActivityMonitor::testLastConnectedDevice()
{
    String inputId = TerpstraSysExApplication::getApp().getPropertiesFile()->getValue("LastInputDeviceId");
    if (inputId.length() <= 0)
        return false;

    int inputIndex = midiDriver.getIndexOfInputDevice(inputId);
    if (inputIndex >= 0)
    {
        String outputId = TerpstraSysExApplication::getApp().getPropertiesFile()->getValue("LastOutputDeviceId");
        if (outputId.length() <= 0)
            return false;
        
        int outputIndex = midiDriver.getIndexOfOutputDevice(outputId);
        if (outputIndex >= 0)
        {
            deviceDetectInProgress = true;
            waitingForTestResponse = true;

            DBG("Testing last connected device");
            midiDriver.setMidiInput(inputIndex);
            midiDriver.setMidiOutput(outputIndex);
            midiDriver.sendGetSerialIdentityRequest();
            return true;
        }
    }

    return false;
}

void DeviceActivityMonitor::startIndividualDetection()
{
    jassert(outputPingIds.size() == 0);
    
    testOutputIndex = -1;
    outputDevices = midiDriver.getMidiOutputList();
    inputDevices = midiDriver.getMidiInputList();

    if (outputDevices.size() > 0 && inputDevices.size() > 0)
    {
        deviceDetectInProgress = true;
        testNextOutput();
    }
    else
    {
        DBG("No input and output MIDI device combination could be made.");
        deviceDetectInProgress = false;
        startTimer(detectRoutineTimeoutMs);
    }
}

void DeviceActivityMonitor::testNextOutput()
{
    testOutputIndex++;

    if (testOutputIndex >= 0 && testOutputIndex < outputDevices.size()) 
    {
        DBG("Testing " + outputDevices[testOutputIndex].name);
        if (sendCalibratePitchModOff)
            midiDriver.sendCalibratePitchModWheel(false, testOutputIndex);
        else
            midiDriver.sendGetSerialIdentityRequest(testOutputIndex);
        
        waitingForTestResponse = true;
        startTimer(responseTimeoutMs);
    }
    // failed
    else
    {
        startTimer(10);
    }
}

void DeviceActivityMonitor::initializeDeviceDetection()
{
    // Belongs somewhere else?
    if (!midiDriver.hasDevicesDefined())
    {
        confirmedInputIndex = -1;
        confirmedOutputIndex = -1;
    }

    if (detectDevicesIfDisconnected)
    {
        deviceConnectionMode = DetectConnectionMode::lookingForDevice;
        deviceDetectInProgress = false; // Don't start until first test response is sent
        startTimer(10);
    }
}

void DeviceActivityMonitor::intializeConnectionLossDetection()
{
    if (checkConnectionOnInactivity)
    {
        deviceConnectionMode = DetectConnectionMode::waitingForInactivity;
        waitingForTestResponse = false;
        startTimer(inactivityTimeoutMs);
    }
}

void DeviceActivityMonitor::stopDeviceDetection()
{
    deviceConnectionMode = DetectConnectionMode::noDeviceActivity;
    deviceDetectInProgress = false;
}

void DeviceActivityMonitor::stopMonitoringDevice()
{
    stopTimer();
    waitingForTestResponse = false;
    deviceConnectionMode = DetectConnectionMode::noDeviceMonitoring;
}

bool DeviceActivityMonitor::initializeConnectionTest()
{    
    TerpstraSysExApplication::getApp().getLumatoneController()->testCurrentDeviceConnection();
    waitingForTestResponse = true;
    startTimer(inactivityTimeoutMs);
    return true;
}


void DeviceActivityMonitor::onSerialIdentityResponse(const MidiMessage& msg, int deviceIndexResponded)
{
    waitingForTestResponse = false;

    switch (deviceConnectionMode)
    {
    case DetectConnectionMode::lookingForDevice:
        if (midiDriver.hasDevicesDefined())
        {
            confirmedOutputIndex = midiDriver.getMidiOutputIndex();
            confirmedInputIndex = midiDriver.getMidiInputIndex();
        }
        else
        {
            confirmedOutputIndex = testOutputIndex;
            confirmedInputIndex = deviceIndexResponded;
        }

        break;

    case DetectConnectionMode::waitingForInactivity:
        startTimer(inactivityTimeoutMs);
        break;

    default:
        // TODO review
        break;
    }
}

void DeviceActivityMonitor::onFailedPing(const MidiMessage& msg)
{
    unsigned int pingId = -1;
    midiDriver.unpackPingResponse(msg, pingId);
    if (pingId >= 0 && pingId < outputDevices.size())
        outputDevices.remove(outputPingIds.indexOf(pingId));
}

void DeviceActivityMonitor::onPingResponse(const MidiMessage& msg, int deviceIndexResponded)
{
    waitingForTestResponse = false;

    if (deviceConnectionMode == DetectConnectionMode::lookingForDevice && outputPingIds.size() > 0)
    {
        unsigned int pingId = 0;
        auto errorCode = midiDriver.unpackPingResponse(msg, pingId);

        if (errorCode != FirmwareSupport::Error::noError)
        {
            if (errorCode == FirmwareSupport::Error::messageIsAnEcho)
                return;
            
            DBG("WARNING: Ping response error in auto connection routine detected");
            jassertfalse;
            return;
        }

        if (pingId > 0)
        {
            confirmedOutputIndex = pingId - 1;
            confirmedInputIndex = deviceIndexResponded;
            // startTimer(10);
        }
    }

    else if (deviceConnectionMode == DetectConnectionMode::waitingForInactivity)
    {
        onTestResponseReceived();
    }
}

void DeviceActivityMonitor::onTestResponseReceived()
{
    waitingForTestResponse = false;
    
    if (sendCalibratePitchModOff)
    {
        sendCalibratePitchModOff = false;
        checkDetectionStatus();
        return;
    }

    if (deviceConnectionMode == DetectConnectionMode::lookingForDevice)
    {
        onSuccessfulDetection();
    }

    else if (deviceConnectionMode == DetectConnectionMode::waitingForInactivity)
    {
        startTimer(inactivityTimeoutMs);
    }
}

void DeviceActivityMonitor::checkDetectionStatus()
{
    // Successful
    if (isConnectionEstablished())
    {
        onTestResponseReceived();
    }
    else
    {
        if (deviceDetectInProgress)
        {
            // Failed last-device test
            if (midiDriver.hasDevicesDefined())
            {
                DBG("Unable to connect to last device.");
                waitingForTestResponse = false;
                midiDriver.closeMidiInput();
                midiDriver.closeMidiOutput();
                pingAllDevices();
            }

            // Failed ping routine
            else if (outputPingIds.size() > 0)
            {
                waitingForTestResponse = false;
                outputPingIds.clear();
                startIndividualDetection();
            }

            // Ongoing GetSerial routine
            else if (testOutputIndex >= 0 && testOutputIndex < outputDevices.size())
            {
                testNextOutput();
            }
            
            // Set timeout for next attempt
            else
            {
                DBG("Detect device timeout.");
                deviceDetectInProgress = false;
                waitingForTestResponse = false;
                startTimer(detectRoutineTimeoutMs);
            }
        }

        // Start detection
        else
        {
            midiDriver.refreshDeviceLists();
            midiDriver.openAvailableDevicesForTesting();
            deviceConnectionMode = DetectConnectionMode::lookingForDevice;

            // If there's no last-connected-device, skip to pinging
            //if (!testLastConnectedDevice())
            //{
                pingAllDevices();
            //}
        }
    }
}

void DeviceActivityMonitor::timerCallback()
{
    stopTimer();

    MidiBuffer readBuffer;
    removeNextBlockOfMessages(readBuffer, readBlockSize);
    
    handleMessageQueue(readBuffer, testResponseDeviceIndices);
    auto size = readQueueSize.load();
    readQueueSize.store(jlimit(0, 999999, size - readBlockSize));

    switch (deviceConnectionMode)
    {
    case DetectConnectionMode::noDeviceActivity:
    case DetectConnectionMode::lookingForDevice:
        if (detectDevicesIfDisconnected)
        {
            checkDetectionStatus();
            break;
        }

        stopDeviceDetection();
        break;

    case DetectConnectionMode::noDeviceMonitoring:
    case DetectConnectionMode::waitingForInactivity:
        jassert(isConnectionEstablished() && midiDriver.hasDevicesDefined());
        if (!checkConnectionOnInactivity)
        {
            stopMonitoringDevice();
            break;
        }

        if (!waitingForTestResponse)
        {
            if (sentQueueSize > 0)
            {
                startTimer(inactivityTimeoutMs);
                break;
            }

            if (deviceConnectionMode == DetectConnectionMode::noDeviceMonitoring)
                deviceConnectionMode = DetectConnectionMode::waitingForInactivity;

            initializeConnectionTest();
        }
        else
        {
            DBG("waiting for test response...");
        }

        break;

    default:
        jassertfalse;
    }
}

void DeviceActivityMonitor::handleResponse(int inputDeviceIndex, const MidiMessage& msg)
{
    if (msg.isSysEx())
    {
        auto sysExData = msg.getSysExData();
        auto cmd = sysExData[CMD_ID];

        if (cmd == PERIPHERAL_CALBRATION_DATA && !isConnectionEstablished())
        {
            sendCalibratePitchModOff = true;
            // startTimer(100);
            return;
        }

        if (waitingForTestResponse) switch (sysExData[MSG_STATUS])
        {
            // Skip echos, or mark as a failed ping
            case TEST_ECHO:
            {
                switch (cmd)
                {
                case LUMA_PING:
                {
                    DBG("Ignoring Ping Echo");
                    onFailedPing(msg);
                    break;
                }
                case GET_SERIAL_IDENTITY:
                    break;

                case GET_FIRMWARE_REVISION:
                    DBG("Ignoring Firmware Echo");
                    // onTestResponseReceived();
                    break;

                default:
                    return;
                }
                break;
            }

            case TerpstraMIDIAnswerReturnCode::ACK:
            {
                switch (cmd)
                {
                case GET_SERIAL_IDENTITY:
                    onSerialIdentityResponse(msg, inputDeviceIndex);
                    break;

                case CALIBRATE_PITCH_MOD_WHEEL:
                    if (sysExData[PAYLOAD_INIT] != TEST_ECHO)
                    {
                        if (!isConnectionEstablished())
                        {
                            sendCalibratePitchModOff = true;
                        }
                    }
                    break;

                case GET_FIRMWARE_REVISION:
                    break;

                case LUMA_PING:
                    onPingResponse(msg, inputDeviceIndex);
                    break;

                default:
                    break;
                }

                waitingForTestResponse = false;
                break;
            }

            // Consider a response from a different firmware state as successful
            case TerpstraMIDIAnswerReturnCode::STATE:
            {
                // Find 'off' message if possible
                //onTestResponseReceived();
            }
        }

        // Edge case if we're disconnected but get a response
        else if (!isConnectionEstablished())
        {
            confirmedInputIndex = midiDriver.getMidiInputIndex();
            confirmedOutputIndex = midiDriver.getMidiOutputIndex();

            // Maybe not best solution?
            deviceConnectionMode = DetectConnectionMode::lookingForDevice;
        }
        else
        {
            startTimer(inactivityTimeoutMs);
        }
    }

}

void DeviceActivityMonitor::handleMessageQueue(const MidiBuffer& readBuffer, const Array<int, CriticalSection>& devices)
{
    int smpl = 0;
    for (auto event : readBuffer)
    {
        auto msg = event.getMessage();
        handleResponse(devices[smpl], msg);
        smpl++;
    }
}

//=========================================================================
// TerpstraMidiDriver::Listener Implementation

void DeviceActivityMonitor::midiMessageReceived(MidiInput* source, const MidiMessage& msg)
{
    if (!msg.isSysEx())
        return;

    int deviceIndex = (source == nullptr) ? -1
                                          : midiDriver.getMidiInputList().indexOf(source->getDeviceInfo());

    addMessageToQueue(msg);

    auto size = readQueueSize.load();
    testResponseDeviceIndices.set(size, deviceIndex);
    readQueueSize.store(size + 1);
}

void DeviceActivityMonitor::noAnswerToMessage(MidiInput* expectedDevice, const MidiMessage& midiMessage)
{
    stopTimer();
    
    if (waitingForTestResponse && deviceConnectionMode < DetectConnectionMode::noDeviceMonitoring)
    {
        waitingForTestResponse = false;
        auto sysExData = midiMessage.getSysExData();
        if (sysExData[CMD_ID] == LUMA_PING && outputPingIds.size() > 0)
        {
            onFailedPing(midiMessage);
        }
        else
        {
            checkDetectionStatus();
        }
    }
    
    else if (isConnectionEstablished())
    {
        onDisconnection();
    }
}

void DeviceActivityMonitor::onSuccessfulDetection()
{
    jassert(isConnectionEstablished());

    deviceDetectInProgress = false;
    outputPingIds.clear();

    // LumatoneController will start this back up
    deviceConnectionMode = DetectConnectionMode::noDeviceMonitoring;
    
    sendChangeMessage();
}

void DeviceActivityMonitor::onDisconnection()
{
    DBG("DISCONNECTION DETECTED");

    confirmedInputIndex = -1;
    confirmedOutputIndex = -1;

    waitingForTestResponse = false;

    // LumatoneController will start this back up
    deviceConnectionMode = DetectConnectionMode::noDeviceActivity;
    
    sendChangeMessage();
}
