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

    auto outputDevices = midiDriver.getMidiOutputList();

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

    failedPings = 0;
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
        expectedResponseReceived = false;
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

    // Tried all devices, set timeout for next attempt
    else
    {
        DBG("Detect device timeout.");
        deviceDetectInProgress = false;
        startTimer(detectRoutineTimeoutMs);
    }
}

void DeviceActivityMonitor::initializeDeviceDetection()
{
    if (detectDevicesIfDisconnected)
    {
        deviceConnectionMode = DetectConnectionMode::lookingForDevice;
        expectedResponseReceived = false;
        deviceDetectInProgress = false;
        startTimer(detectRoutineTimeoutMs);
    }
}

void DeviceActivityMonitor::stopDeviceDetection()
{
    deviceConnectionMode = DetectConnectionMode::noDeviceActivity;
    deviceDetectInProgress = false;
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

    expectedResponseReceived = false;
    waitingForTestResponse = false;
}

void DeviceActivityMonitor::cancelFirmwareUpdateMode()
{
    deviceConnectionMode = DetectConnectionMode::lookingForDevice;
    waitingForTestResponse = false;
}

bool DeviceActivityMonitor::initializeConnectionTest(DeviceActivityMonitor::DetectConnectionMode modeToUse)
{    
    deviceConnectionMode = modeToUse;

    if (deviceConnectionMode == DetectConnectionMode::gettingFirmwareVersion)
        TerpstraSysExApplication::getApp().getLumatoneController().sendGetFirmwareRevisionRequest();
    else
        TerpstraSysExApplication::getApp().getLumatoneController().testCurrentDeviceConnection();

    waitingForTestResponse = true;
    expectedResponseReceived = false;
    startTimer(responseTimeoutMs);

    return true;
}


void DeviceActivityMonitor::onSerialIdentityResponse(const MidiMessage& msg, int deviceIndexResponded)
{
    if (deviceConnectionMode == DetectConnectionMode::lookingForDevice)
    {
        confirmedOutputIndex = testOutputIndex;
        confirmedInputIndex = deviceIndexResponded;
    }

    onTestResponseReceived();
}

void DeviceActivityMonitor::onPingResponse(const MidiMessage& msg, int deviceIndexResponded)
{
    if (deviceConnectionMode == DetectConnectionMode::lookingForDevice && outputPingIds.size() > 0)
    {
        unsigned int pingId = 0;
        auto errorCode = midiDriver.unpackPingResponse(msg, pingId);

        if (errorCode != FirmwareSupport::Error::noError)
        {
            DBG("WARNING: Ping response error in auto connection routine detected");
            jassertfalse;
            return;
        }

        if (pingId > 0)
        {
            confirmedOutputIndex = pingId - 1;
            confirmedInputIndex = deviceIndexResponded;

            onTestResponseReceived();
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
    expectedResponseReceived = true;

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
    if (expectedResponseReceived && isConnectionEstablished())
    {
        onSuccessfulDetection();
    }
    else
    {
        if (deviceDetectInProgress)
        {
            // Failed ping routine
            if (outputPingIds.size() > 0)
            {
                outputPingIds.clear();
                startIndividualDetection();
            }

            // Ongoing GetSerial routine
            else
            {
                testNextOutput();
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
    DBG("DAM TIMER");

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
                if (midiQueueSize == 0) //TODO
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
            else
            {
                // Disconnected!
                if (isConnectionEstablished())
                {
                    onDisconnection();
                }
            }
        }
        else
        {
            waitingForTestResponse = false;
        }
    }
    else
        jassertfalse;
    //// Connection to selected devices failed
    //if (waitingForTestResponse)
    //{
    //    jassert(confirmedInputIndex < 0 && confirmedOutputIndex < 0);
    //    expectedResponseReceived = false;
    //    waitingForTestResponse = false;
    //    deviceConnectionMode = DetectConnectionMode::noDeviceActivity;
    //    DBG("No response from selected MIDI devices.");
    //}
    //else if (checkConnectionOnInactivity)
    //    intializeConnectionLossDetection();
    
}

//=========================================================================
// TerpstraMidiDriver::Listener Implementation

void DeviceActivityMonitor::midiMessageReceived(MidiInput* source, const MidiMessage& msg)
{
    if (msg.isSysEx())
    {
        if (waitingForTestResponse)
        {
            auto sysExData = msg.getSysExData();
            auto cmd = sysExData[CMD_ID];

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
        else
        {
            if (!isConnectionEstablished())
            {
                confirmedInputIndex = midiDriver.getLastMidiInputIndex();
                confirmedOutputIndex = midiDriver.getLastMidiOutputIndex();
            }

            if (deviceConnectionMode <= DetectConnectionMode::lookingForDevice);
            {
                deviceConnectionMode = DetectConnectionMode::noDeviceMonitoring;
            }
            
            startTimer(inactivityTimeoutMs);
        }
    }
}

void DeviceActivityMonitor::onSuccessfulDetection()
{
    jassert(isConnectionEstablished());

    deviceDetectInProgress = false;
    outputPingIds.clear();

    midiDriver.setMidiInput(confirmedInputIndex);
    midiDriver.setMidiOutput(confirmedOutputIndex);

    if (checkConnectionOnInactivity)
    {
        deviceConnectionMode = DetectConnectionMode::waitingForInactivity;
        startTimer(inactivityTimeoutMs);
    }
    else
    {
        deviceConnectionMode = DetectConnectionMode::noDeviceMonitoring;
    }
}

void DeviceActivityMonitor::onDisconnection()
{
    DBG("DISCONNECTION DETECTED");

    confirmedInputIndex = -1;
    confirmedOutputIndex = -1;

    waitingForTestResponse = false;
    expectedResponseReceived = false;
    deviceConnectionMode = DetectConnectionMode::noDeviceActivity;

    initializeDeviceDetection();
}
