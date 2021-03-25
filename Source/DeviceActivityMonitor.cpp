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
    detectMessage = midiDriver->getSerialIdentityRequestMessage();
    detectDevicesIfDisconnected = TerpstraSysExApplication::getApp().getPropertiesFile()->getBoolValue("DetectDeviceIfDisconnected", true);
    checkConnectionOnInactivity = TerpstraSysExApplication::getApp().getPropertiesFile()->getBoolValue("CheckConnectionIfInactive", true);
    midiDriver->addListener(this);
}

DeviceActivityMonitor::~DeviceActivityMonitor()
{
    closeInputDevices();
    closeOutputDevices();
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

    midiDriver->removeListener(this);

    pingOutputIndex = -1;

    closeInputDevices();
    closeOutputDevices();

    openAvailableInputDevices();
    openAvailableOutputDevices();

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
    midiDriver->addListener(this);
}

void DeviceActivityMonitor::initializeFirmwareUpdateMode()
{ 
    deviceConnectionMode = DetectConnectionMode::waitingForFirmwareUpdate; 
    midiDriver->closeMidiInput();
    midiDriver->closeMidiOutput();
    sendChangeMessage();
    expectedResponseReceived = false;
    waitingForTestResponse = false;
    midiDriver->addListener(this);
}

void DeviceActivityMonitor::cancelFirmwareUpdateMode()
{
    deviceConnectionMode = DetectConnectionMode::lookingForDevice;
    waitingForTestResponse = false;
    midiDriver->removeListener(this);
}

void DeviceActivityMonitor::openAvailableOutputDevices()
{
    for (auto outputDeviceInfo : midiDriver->getMidiOutputList())
    {
        outputsToPing.add(MidiOutput::openDevice(outputDeviceInfo.identifier));
    }
}

void DeviceActivityMonitor::openAvailableInputDevices()
{
    for (auto inputDeviceInfo : midiDriver->getMidiInputList())
    {
        auto input = inputsListening.add(MidiInput::openDevice(inputDeviceInfo.identifier, this));
        if (input) input->start();
    }
}

void DeviceActivityMonitor::closeInputDevices()
{
    for (auto input : inputsListening)
    {
        if (input) input->stop();
    }

    inputsListening.clear();
}

void DeviceActivityMonitor::closeOutputDevices()
{
    // juce::MidiOutput stops background threads in its destructor
    outputsToPing.clear();
}

bool DeviceActivityMonitor::initializeConnectionTest(DeviceActivityMonitor::DetectConnectionMode modeToUse)
{
    if (isThreadRunning() && threadShouldExit())
        return false;

    MidiDeviceInfo inputInfo = midiDriver->getMidiOutputInfo();
    MidiDeviceInfo outputInfo = midiDriver->getMidiInputInfo();
    
    if (inputInfo.identifier != "" && outputInfo.identifier != "")
    {
        //DBG("Testing connection of input " + inputInfo.name + " and output " + outputInfo.name);
        midiDriver->addListener(this);
        {
            ScopedLock lock(criticalSection);
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

void DeviceActivityMonitor::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& response)
{
    if (midiDriver->messageIsResponseToMessage(response, detectMessage))
    {
        auto data = response.getSysExData();

        // Is an echo of the request
        if (data[5] == TerpstraMidiDriver::TerpstraMIDIAnswerReturnCode::NACK)
        {
            return;
        }

        // Is an acknowledged answer and successful connection
        else
        {
            expectedResponseReceived = true;
            confirmedInputIndex = inputsListening.indexOf(source);
            confirmedOutputIndex = pingOutputIndex;
        }
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

void DeviceActivityMonitor::midiMessageReceived(const MidiMessage& midiMessage)
{
    ScopedLock lock(criticalSection);

    activitySinceLastTimeout = true;

    // Ignore any non-SysEx related messages
    if (!midiMessage.isSysEx())
        return;
 
    auto sysExData = midiMessage.getSysExData();

    if (sysExData[5] > TerpstraMidiDriver::NACK)
    {
        if (midiDriver->messageIsGetSerialIdentityResponse(midiMessage))
        {
            waitingForTestResponse = false;
            expectedResponseReceived = true;

            if (!isThreadRunning())
            {
                int currentInput = midiDriver->getLastMidiInputIndex();
                int currentOutput = midiDriver->getLastMidiOutputIndex();
                if (confirmedInputIndex != currentInput || confirmedOutputIndex != currentOutput)
                {
                    confirmedInputIndex = currentInput;
                    confirmedOutputIndex = currentOutput;
                }

                sendChangeMessage();
            }
        }
        else if (deviceConnectionMode == DetectConnectionMode::waitingForFirmwareUpdate && midiDriver->messageIsGetFirmwareRevisionResponse(midiMessage))
        {
            expectedResponseReceived = true;
            deviceDetectInProgress = false;
        }
    }
}

void DeviceActivityMonitor::onSuccessfulDetection()
{
    {
        ScopedLock lock(criticalSection);

        deviceDetectInProgress = false;

        closeOutputDevices();
        closeInputDevices(); // Connection loss is handled in TerpstraMidiDriver::Listener callback

        midiDriver->setMidiInput(confirmedInputIndex);
        midiDriver->setMidiOutput(confirmedOutputIndex);
    }

    sendChangeMessage();

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

        midiDriver->closeMidiInput();
        midiDriver->closeMidiOutput();

        confirmedInputIndex = -1;
        confirmedOutputIndex = -1;

        waitingForTestResponse = false;
        expectedResponseReceived = false;
        deviceConnectionMode = DetectConnectionMode::noDeviceActivity;
    }

    sendChangeMessage();

    if (detectDevicesIfDisconnected)
    {
        wait(pingRoutineTimeoutMs);
        startDetectionRoutine();
        // Nullptrs are checked before opening devices but this could maybe be designed better
        // to prevent trying to open invalid (disconnected) devices altogether
    }
}

