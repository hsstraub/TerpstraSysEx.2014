/*
  ==============================================================================

    DeviceActivityMonitor.cpp
    Created: 11 Feb 2021 9:15:43pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "DeviceActivityMonitor.h"
#include "Main.h"


DeviceActivityMonitor::DeviceActivityMonitor(TerpstraMidiDriver& midiDriverIn, int responseTimeoutMsIn)
    : midiDriver(&midiDriverIn), responseTimeoutMs(responseTimeoutMsIn),
      Thread("DeviceActivityMonitor")
{
    detectDevicesIfDisconnected = TerpstraSysExApplication::getApp().getPropertiesFile()->getBoolValue("DetectDeviceIfDisconnected", true);
    checkConnectionOnInactivity = TerpstraSysExApplication::getApp().getPropertiesFile()->getBoolValue("CheckConnectionIfInactive", true);
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
    if (threadShouldExit())
        return;

    pingOutputIndex = -1;

    if (outputsToPing.size() > 0 && inputsListening.size() > 0)
    {
        expectedResponseReceived = false;
        deviceDetectInProgress = true;
        pingNextOutput();
    }
    else
    {
        DBG("No input and output MIDI device combination could be made.");
        deviceDetectInProgress = false;
        wait(pingRoutineTimeoutMs);
    }
}

void DeviceActivityMonitor::pingNextOutput()
{
    pingOutputIndex++;

    if (pingOutputIndex >= 0 && pingOutputIndex < outputsToPing.size() && outputsToPing[pingOutputIndex])
    {
        DBG("Pinging " + outputsToPing[pingOutputIndex]->getName());
        outputsToPing[pingOutputIndex]->sendMessageNow(detectMessage);
        wait(responseTimeoutMs);
    }

    // Tried all devices, set timeout for next attempt
    else
    {
        DBG("Detect device timeout.");
        deviceDetectInProgress = false;
        wait(pingRoutineTimeoutMs);
    }
}

void DeviceActivityMonitor::initializeDeviceDetection()
{
    deviceConnectionMode = DetectConnectionMode::lookingForDevice;
    expectedResponseReceived = false;
    deviceDetectInProgress = false;
    startThread();
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
//    for (auto output : outputsToPing)
//    {
//        output
//    }
//}

void DeviceActivityMonitor::intializeConnectionLossDetection(bool inFirmwareMode)
{
    if (inFirmwareMode)
        deviceConnectionMode = DetectConnectionMode::gettingFirmwareVersion;
    else
        deviceConnectionMode = DetectConnectionMode::waitingForInactivity;

    waitingForTestResponse = false;
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
    if (isThreadRunning() && threadShouldExit())
        return false;

    MidiDeviceInfo inputInfo = midiDriver->getMidiOutputInfo();
    MidiDeviceInfo outputInfo = midiDriver->getMidiInputInfo();
    
    if (inputInfo.identifier != "" && outputInfo.identifier != "")
    {
        DBG("Testing connection of input " + inputInfo.name + " and output " + outputInfo.name);
        midiDriver->addListener(this);
        {
            //ScopedLock lock(criticalSection);
            deviceConnectionMode = modeToUse;

            if (deviceConnectionMode == DetectConnectionMode::gettingFirmwareVersion)
                midiDriver->sendGetFirmwareRevisionRequest(true);
            else
                midiDriver->sendGetSerialIdentityRequest(true);
            
            waitingForTestResponse = true;
            expectedResponseReceived = false;
        }
        
        return true;
    }

    return false;
}

void DeviceActivityMonitor::pingResponseReceived(int value)
{
    onTestResponseReceived();

    confirmedInputIndex = value;
    confirmedOutputIndex = outputPingIds.indexOf(value);
    jassert(confirmedOutputIndex >= 0);

    // TODO from old function
    if (!isThreadRunning())
    {
        int currentInput = midiDriver->getLastMidiInputIndex();
        int currentOutput = midiDriver->getLastMidiOutputIndex();
        if (confirmedInputIndex != currentInput || confirmedOutputIndex != currentOutput)
        {
            confirmedInputIndex = currentInput;
            confirmedOutputIndex = currentOutput;
        }

        //sendChangeMessage();
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

void DeviceActivityMonitor::run()
{
    while (!threadShouldExit() && (detectDevicesIfDisconnected || checkConnectionOnInactivity))
    {
        while (activityIsPaused) { if (threadShouldExit()) return; };

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
                    activitySinceLastTimeout = false;
                    wait(inactivityTimeoutMs);
                    if (midiQueueSize == 0 && !activitySinceLastTimeout)
                        initializeConnectionTest(deviceConnectionMode);
                }
            }
            else if (!expectedResponseReceived)
            {
                wait(responseTimeoutMs);
                // Disconnected!
                if (!expectedResponseReceived)
                {
                    jassert(confirmedInputIndex >= 0 && confirmedOutputIndex >= 0);
                    {
                        ScopedLock lock(criticalSection);
                        expectedResponseReceived = false;
                        waitingForTestResponse = false;
                    }
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
                {
                    ScopedLock lock(criticalSection);
                    expectedResponseReceived = false;
                    waitingForTestResponse = false;
                    deviceConnectionMode = DetectConnectionMode::noDeviceActivity;
                }
                DBG("No response from selected MIDI devices.");
                sendChangeMessage();
            }
            else if (checkConnectionOnInactivity)
                intializeConnectionLossDetection();
        }
    }

    deviceDetectInProgress = false;
    waitingForTestResponse = false;
}

//=========================================================================
// TerpstraMidiDriver::Listener Implementation

void DeviceActivityMonitor::onSuccessfulDetection()
{
    {
        ScopedLock lock(criticalSection);

        deviceDetectInProgress = false;

        midiDriver->setMidiInput(confirmedInputIndex);
        midiDriver->setMidiOutput(confirmedOutputIndex);
    }

    if (checkConnectionOnInactivity && deviceConnectionMode != DetectConnectionMode::waitingForFirmwareUpdate)
    {
        intializeConnectionLossDetection();
    }
}

void DeviceActivityMonitor::onDisconnection()
{
    {
        ScopedLock lock(criticalSection);
        DBG("DISCONNECTION DETECTED");

        confirmedInputIndex = -1;
        confirmedOutputIndex = -1;

        waitingForTestResponse = false;
        expectedResponseReceived = false;
        deviceConnectionMode = DetectConnectionMode::noDeviceActivity;
    }

    if (detectDevicesIfDisconnected)
    {
        wait(pingRoutineTimeoutMs);
        startDetectionRoutine();
        // Nullptrs are checked before opening devices but this could maybe be designed better
        // to prevent trying to open invalid (disconnected) devices altogether
    }
}

