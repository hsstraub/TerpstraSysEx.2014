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
    : midiDriver(&TerpstraSysExApplication::getApp().getMidiDriver()),
      responseTimeoutMs(TerpstraSysExApplication::getApp().getPropertiesFile()->getIntValue("DetectDeviceTimeout", 1000)),
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

void DeviceActivityMonitor::initializeDeviceDetection()
{
    jassert(!deviceDetectInProgress);
    if (threadShouldExit())
        return;

    midiDriver->removeListener(this);

    deviceConnectionMode = DetectConnectionMode::lookingForDevice;

    pingOutputIndex = -1;

    closeInputDevices();
    inputsListening.clear();
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

void DeviceActivityMonitor::intializeConnectionLossDetection()
{
    deviceConnectionMode = DetectConnectionMode::waitingForInactivity;
    midiDriver->addListener(this);
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
            midiDriver->sendGetSerialIdentityRequest();
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
    if (!detectDevicesIfDisconnected)
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
            initializeDeviceDetection();
        }
    }
}

void DeviceActivityMonitor::run()
{
    while (!threadShouldExit() && (detectDevicesIfDisconnected || checkConnectionOnInactivity))
    {
        if (deviceConnectionMode < DetectConnectionMode::testingConnection)
        {
            if (detectDevicesIfDisconnected)
            {
                if (deviceDetectInProgress)
                    checkDetectionStatus();

                else if (!isConnectionEstablished())
                    initializeDeviceDetection();

                else
                    intializeConnectionLossDetection();
            }
        }
        else if (deviceConnectionMode == DetectConnectionMode::waitingForInactivity)
        {
            if (!waitingForTestResponse)
            {
                activitySinceLastTimeout = false;
                wait(inactivityTimeoutMs);
                if (!activitySinceLastTimeout)
                    initializeConnectionTest(deviceConnectionMode);
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
                    //sendChangeMessage();
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

    // Ignore any non-SysEx related messages
    if (!midiMessage.isSysEx())
        return;
 
    auto sysExData = midiMessage.getSysExData();
    if (sysExData[5] > TerpstraMidiDriver::NACK && midiDriver->messageIsGetSerialIdentityResponse(midiMessage))
    {
        waitingForTestResponse = false;
        expectedResponseReceived = true;
        activitySinceLastTimeout = true;

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
}

void DeviceActivityMonitor::generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel)
{
    // If we don't receive the response soon and if a connection was previously made
    //if  (  errorLevel == HajuErrorVisualizer::ErrorLevel::error 
    //    && waitingForTestResponse
    //    && textMessage.startsWith("No answer")
    //    )
    //{
    //    if (deviceConnectionMode == DetectConnectionMode::waitingForInactivity)
    //    {
    //        jassert(confirmedInputIndex >= 0 && confirmedOutputIndex >= 0);
    //        onDisconnection();
    //    }
    //    else
    //    {
    //        jassert(confirmedInputIndex < 0 && confirmedOutputIndex < 0);
    //        // Connection to selected devices failed
    //        DBG("No response from selected MIDI devices.");
    //        deviceConnectionMode = DetectConnectionMode::noDeviceActivity;
    //        sendChangeMessage();
    //    }
    //}
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

    if (checkConnectionOnInactivity)
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
    }

    sendChangeMessage();

    if (detectDevicesIfDisconnected)
    {
        wait(pingRoutineTimeoutMs);
        initializeDeviceDetection();
        // Nullptrs are checked before opening devices but this could maybe be designed better
        // to prevent trying to open invalid (disconnected) devices altogether
    }
}

