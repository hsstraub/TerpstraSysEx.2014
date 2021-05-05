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
    : midiDriver(midiDriverIn)
{
    midiDriver.addListener(this);
}

DeviceActivityMonitor::~DeviceActivityMonitor()
{

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
        startTimer(inactivityTimeoutMs);
}

void DeviceActivityMonitor::pingAllDevices()
{
    jassert(!deviceDetectInProgress);
    deviceDetectInProgress = true;

    outputDevices = midiDriver.getMidiOutputList();

    outputPingIds.clear();

    waitingForTestResponse = true;

    // Impossible...but why not limit anyway?
    const unsigned int maxDevices = jmin(outputDevices.size(), (1 << 28) - 1);
    for (unsigned int i = 0; i < maxDevices; i++)
    {
        const unsigned int id = i + 1;
        midiDriver.ping(id, i);
        outputPingIds.add(id);
    }

    startTimer(responseTimeoutMs);
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
    if (detectDevicesIfDisconnected)
    {
        deviceConnectionMode = DetectConnectionMode::lookingForDevice;
        deviceDetectInProgress = false; // Don't start until first test response is sent
        startTimer(10);
    }
}

void DeviceActivityMonitor::intializeConnectionLossDetection(bool inFirmwareMode)
{
    if (checkConnectionOnInactivity)
    {
        if (inFirmwareMode)
            deviceConnectionMode = DetectConnectionMode::gettingFirmwareVersion;
        else
            deviceConnectionMode = DetectConnectionMode::waitingForInactivity;

        waitingForTestResponse = false;

        startTimer(inactivityTimeoutMs);
    }
}

void DeviceActivityMonitor::initializeFirmwareUpdateMode()
{ 
    deviceConnectionMode = DetectConnectionMode::waitingForFirmwareUpdate; 

    waitingForTestResponse = false;
}

void DeviceActivityMonitor::cancelFirmwareUpdateMode()
{
    deviceConnectionMode = DetectConnectionMode::lookingForDevice;
    waitingForTestResponse = false;
}

void DeviceActivityMonitor::stopDeviceDetection()
{
    deviceConnectionMode = DetectConnectionMode::noDeviceActivity;
    deviceDetectInProgress = false;
}

void DeviceActivityMonitor::stopMonitoringDevice()
{
    waitingForTestResponse = false;
    deviceConnectionMode = DetectConnectionMode::noDeviceMonitoring;
}

bool DeviceActivityMonitor::initializeConnectionTest(DeviceActivityMonitor::DetectConnectionMode modeToUse)
{    
    deviceConnectionMode = modeToUse;

    if (deviceConnectionMode == DetectConnectionMode::gettingFirmwareVersion)
        TerpstraSysExApplication::getApp().getLumatoneController().sendGetFirmwareRevisionRequest();
    else
        TerpstraSysExApplication::getApp().getLumatoneController().testCurrentDeviceConnection();

    waitingForTestResponse = true;

    return true;
}


void DeviceActivityMonitor::onSerialIdentityResponse(const MidiMessage& msg, int deviceIndexResponded)
{
    waitingForTestResponse = false;

    if (deviceConnectionMode == DetectConnectionMode::lookingForDevice)
    {
        confirmedOutputIndex = testOutputIndex;
        confirmedInputIndex = deviceIndexResponded;
        startTimer(10);
    }
    else if (deviceConnectionMode == DetectConnectionMode::waitingForInactivity)
    {
        startTimer(inactivityTimeoutMs);
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
            startTimer(10);
        }
    }

    else if (deviceConnectionMode == DetectConnectionMode::waitingForInactivity)
    {
        onTestResponseReceived();
    }
}

void DeviceActivityMonitor::onTestResponseReceived()
{
    stopTimer();

    waitingForTestResponse = false;

    if (deviceConnectionMode == DetectConnectionMode::waitingForFirmwareUpdate)
    {
        deviceDetectInProgress = false;
    }

    else if (deviceConnectionMode == DetectConnectionMode::lookingForDevice)
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
            // Failed ping routine
            if (outputPingIds.size() > 0)
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
                startTimer(detectRoutineTimeoutMs);
            }
        }

        // Start detection
        else
        {
            midiDriver.refreshDeviceLists();
            midiDriver.openAvailableDevicesForTesting();
            deviceConnectionMode = DetectConnectionMode::lookingForDevice;
            pingAllDevices();
        }
    }
}

void DeviceActivityMonitor::timerCallback()
{
    stopTimer();

    if (deviceConnectionMode < DetectConnectionMode::noDeviceMonitoring)
    {
        if (detectDevicesIfDisconnected)
        {
            checkDetectionStatus();
        }
        else
        {
            stopDeviceDetection();
        }
    }
    else if (deviceConnectionMode >= DetectConnectionMode::noDeviceMonitoring)
    {
        jassert(isConnectionEstablished());

        if (checkConnectionOnInactivity)
        {
            if (!waitingForTestResponse)
            {
                if (midiQueueSize == 0)
                {
                    if (deviceConnectionMode == DetectConnectionMode::noDeviceMonitoring)
                        deviceConnectionMode = DetectConnectionMode::waitingForInactivity;

                    initializeConnectionTest(deviceConnectionMode);
                }
                else
                {
                    startTimer(inactivityTimeoutMs);
                }
            }
        }
        else
        {
            stopMonitoringDevice();
        }
    }
    else
        jassertfalse;
}

//=========================================================================
// TerpstraMidiDriver::Listener Implementation

void DeviceActivityMonitor::midiMessageReceived(MidiInput* source, const MidiMessage& msg)
{
    stopTimer();
    
    if (msg.isSysEx())
    {
        if (waitingForTestResponse)
        {
            auto sysExData = msg.getSysExData();
            auto cmd = sysExData[CMD_ID];

            // Skip echos, or mark as a failed ping
            if (sysExData[MSG_STATUS] == TEST_ECHO)
            {
                switch (cmd)
                {
                case LUMA_PING:
                {
                    onFailedPing(msg);
                    break;
                }
                case GET_SERIAL_IDENTITY:
                    break;
                    
                default:
                    return;
                }
                
                waitingForTestResponse = false;
                startTimer(10);
            }
            else
            {
                int deviceIndex = midiDriver.getMidiInputList().indexOf(source->getDeviceInfo());

                switch (cmd)
                {
                case GET_SERIAL_IDENTITY:
                    onSerialIdentityResponse(msg, deviceIndex);
                    break;

                case GET_FIRMWARE_REVISION:
                    // TODO
                    break;

                case LUMA_PING:
                    onPingResponse(msg, deviceIndex);
                    break;

                default:
                    break;
                }
            }
        }
        
        // Edge case if we're disconnected but get a response
        else if (!isConnectionEstablished())
        {
            confirmedInputIndex = midiDriver.getLastMidiInputIndex();
            confirmedOutputIndex = midiDriver.getLastMidiOutputIndex();
            
            // Maybe not best solution?
            deviceConnectionMode = DetectConnectionMode::lookingForDevice;
            startTimer(10);
        }
        else
        {
            startTimer(inactivityTimeoutMs);
        }
    }
}

void DeviceActivityMonitor::noAnswerToMessage(const MidiMessage& midiMessage)
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
    
    // Confirmation test?
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

    deviceConnectionMode = DetectConnectionMode::noDeviceMonitoring;
    
    sendChangeMessage();
}

void DeviceActivityMonitor::onDisconnection()
{
    DBG("DISCONNECTION DETECTED");

    confirmedInputIndex = -1;
    confirmedOutputIndex = -1;

    waitingForTestResponse = false;
    deviceConnectionMode = DetectConnectionMode::noDeviceActivity;
    
    sendChangeMessage();
}
