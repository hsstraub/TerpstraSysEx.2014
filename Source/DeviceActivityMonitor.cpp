/*
  ==============================================================================

    DeviceActivityMonitor.cpp
    Created: 11 Feb 2021 9:15:43pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "DeviceActivityMonitor.h"
#include "Main.h"


DeviceActivityMonitor::DeviceActivityMonitor()
{
    //auto properties = TerpstraSysExApplication::getApp().getPropertiesFile();
    //responseTimeoutMs = properties->getIntValue("DetectDeviceTimout", responseTimeoutMs);
    //detectDevicesIfDisconnected = properties->getBoolValue("DetectDeviceIfDisconnected", true);
    //checkConnectionOnInactivity = properties->getBoolValue("CheckConnectionIfInactive", true);
    TerpstraSysExApplication::getApp().getLumatoneController().addFirmwareListener(this);
}

DeviceActivityMonitor::~DeviceActivityMonitor()
{
    TerpstraSysExApplication::getApp().getLumatoneController().removeFirmwareListener(this);
}

void DeviceActivityMonitor::setDetectDeviceIfDisconnected(bool doDetection)
{
    detectDevicesIfDisconnected = doDetection;
    TerpstraSysExApplication::getApp().getPropertiesFile()->setValue("DetectDeviceIfDisconnected", detectDevicesIfDisconnected);
}

void DeviceActivityMonitor::setCheckForInactivity(bool monitorActivity)
{
    checkConnectionOnInactivity = monitorActivity;
    TerpstraSysExApplication::getApp().getPropertiesFile()->setValue("CheckConnectionIfInactive", checkConnectionOnInactivity);
}

void DeviceActivityMonitor::startDetectionRoutine()
{
    jassert(!deviceDetectInProgress);

    pingOutputIndex = -1;

    if (outputDevices.size() > 0 && inputDevices.size() > 0)
    {
        expectedResponseReceived = false;
        deviceDetectInProgress = true;
        pingNextOutput();
    }
    else
    {
        DBG("No input and output MIDI device combination could be made.");
        deviceDetectInProgress = false;
        startTimer(pingRoutineTimeoutMs);
    }
}

void DeviceActivityMonitor::pingNextOutput()
{
    pingOutputIndex++;

    if (pingOutputIndex >= 0 && pingOutputIndex < outputDevices.size()) 
    {
        DBG("Pinging " + outputDevices[pingOutputIndex].name);
        TerpstraSysExApplication::getApp().getLumatoneController().pingMidiDevice(pingOutputIndex, pingOutputIndex + 1);
        startTimer(responseTimeoutMs);
    }

    // Tried all devices, set timeout for next attempt
    else
    {
        DBG("Detect device timeout.");
        deviceDetectInProgress = false;
        startTimer(pingRoutineTimeoutMs);
    }
}

void DeviceActivityMonitor::initializeDeviceDetection()
{
    deviceConnectionMode = DetectConnectionMode::lookingForDevice;
    expectedResponseReceived = false;
    deviceDetectInProgress = false;
    startTimer(pingRoutineTimeoutMs);
}

void DeviceActivityMonitor::stopDeviceDetection()
{
    deviceConnectionMode = DetectConnectionMode::noDeviceActivity;
    deviceDetectInProgress = false;
}

//void DeviceActivityMonitor::pingAvailableDevices()
//{
//    const MidiMessage pingMsg = midiDriver->getSerialIdentityRequestMessage();
//
//    for (auto output : outputDevices)
//    {
//        output
//    }
//}

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
        TerpstraSysExApplication::getApp().getLumatoneController().pingLumatone(1);
            
    waitingForTestResponse = true;
    expectedResponseReceived = false;
    startTimer(responseTimeoutMs);

    return true;
}

void DeviceActivityMonitor::serialIdentityReceived(const int* serialBytes)
{
    onTestResponseReceived();

    // Record device id

    if (deviceConnectionMode == DetectConnectionMode::lookingForDevice)
    {
        onSuccessfulDetection();
    }
}

void DeviceActivityMonitor::firmwareRevisionReceived(int major, int minor, int revision)
{
    onTestResponseReceived();

    // Record device id

    // TODO
}


void DeviceActivityMonitor::pingResponseReceived(int value)
{
    onTestResponseReceived();

    confirmedInputIndex = value;
    confirmedOutputIndex = outputPingIds.indexOf(value);
    jassert(confirmedOutputIndex >= 0);

    if (deviceConnectionMode == DetectConnectionMode::lookingForDevice)
    {
        onSuccessfulDetection();
    }
}

void DeviceActivityMonitor::onTestResponseReceived()
{
    waitingForTestResponse = false;
    expectedResponseReceived = true;

    if (deviceConnectionMode == DetectConnectionMode::waitingForFirmwareUpdate)
    {
        deviceDetectInProgress = false;
    }
}

void DeviceActivityMonitor::checkDetectionStatus()
{
    // If turned off, stop detection immediately
    // For future use
    if (!detectDevicesIfDisconnected && deviceConnectionMode == DetectConnectionMode::lookingForDevice)
    {
        stopDeviceDetection();
    }
    else
    {
        // Successful
        if (expectedResponseReceived)
        {
            onSuccessfulDetection();
        }

        // Continue trying
        else if (deviceDetectInProgress)
        {
            pingNextOutput();
        }

        // Restart detection
        else
        {
            startDetectionRoutine();
        }
    }
}

void DeviceActivityMonitor::timerCallback()
{
    stopTimer();

    if (detectDevicesIfDisconnected || checkConnectionOnInactivity)
    {
        if (deviceConnectionMode < DetectConnectionMode::testingConnection)
        {
            if (detectDevicesIfDisconnected)
            {
                if (deviceDetectInProgress)
                    checkDetectionStatus();

                else if (!isConnectionEstablished())
                    startDetectionRoutine();

                else
                    intializeConnectionLossDetection(deviceConnectionMode == DetectConnectionMode::waitingForFirmwareUpdate);
            }
        }
        else if (deviceConnectionMode == DetectConnectionMode::waitingForInactivity)
        {
            if (!waitingForTestResponse)
            {
                if (midiQueueSize == 0)
                {
                    //activitySinceLastTimeout = false;
                    //wait(inactivityTimeoutMs);
                    //if (midiQueueSize == 0 && !activitySinceLastTimeout)
                        initializeConnectionTest(deviceConnectionMode);
                }
                else
                {
                    startTimer(inactivityTimeoutMs);
                }
            }
            else if (!expectedResponseReceived)
            {
                //wait(responseTimeoutMs);
                // Disconnected!
                if (!expectedResponseReceived)
                {
                    jassert(confirmedInputIndex >= 0 && confirmedOutputIndex >= 0);
                    expectedResponseReceived = false;
                    waitingForTestResponse = false;
                    onDisconnection();
                }
            }

        }
        else if (deviceConnectionMode == DetectConnectionMode::testingConnection)
        {
            // Connection to selected devices failed
            if (waitingForTestResponse)
            {
                jassert(confirmedInputIndex < 0 && confirmedOutputIndex < 0);
                expectedResponseReceived = false;
                waitingForTestResponse = false;
                deviceConnectionMode = DetectConnectionMode::noDeviceActivity;
                DBG("No response from selected MIDI devices.");
            }
            else if (checkConnectionOnInactivity)
                intializeConnectionLossDetection();
        }
    }

    //deviceDetectInProgress = false;
    //waitingForTestResponse = false;
}

//=========================================================================
// TerpstraMidiDriver::Listener Implementation

void DeviceActivityMonitor::onSuccessfulDetection()
{
    deviceDetectInProgress = false;

    TerpstraSysExApplication::getApp().getLumatoneController().setMidiInput(confirmedInputIndex);
    TerpstraSysExApplication::getApp().getLumatoneController().setMidiOutput(confirmedOutputIndex);

    if (checkConnectionOnInactivity && deviceConnectionMode != DetectConnectionMode::waitingForFirmwareUpdate)
    {
        intializeConnectionLossDetection();
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

    if (detectDevicesIfDisconnected)
    {
        //wait(pingRoutineTimeoutMs);
        startDetectionRoutine();
        // Nullptrs are checked before opening devices but this could maybe be designed better
        // to prevent trying to open invalid (disconnected) devices altogether
    }
}

