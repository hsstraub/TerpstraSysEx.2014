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
      responseTimeoutMs(TerpstraSysExApplication::getApp().getPropertiesFile()->getIntValue("DetectDeviceTimeout", 1000))
{
    monitorMessage = midiDriver->getSerialIdentityRequestMessage();
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
    
    midiDriver->removeListener(this);

    deviceConnectionMode = DetectConnectionMode::lookingForDevice;

    pingOutputIndex = -1;

    closeInputDevices();
    inputsListening.clear();
    closeOutputDevices();
    
    // Refresh available devices
    midiDriver->refreshDeviceLists();

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
        startTimer(pingRoutineTimeoutMs);
    }
}

void DeviceActivityMonitor::pingNextOutput()
{
    pingOutputIndex++;

    if (pingOutputIndex >= 0 && pingOutputIndex < outputsToPing.size() && outputsToPing[pingOutputIndex])
    {
        DBG("Pinging " + outputsToPing[pingOutputIndex]->getName());

        outputsToPing[pingOutputIndex]->sendMessageNow(monitorMessage);
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

void DeviceActivityMonitor::stopDeviceDetection()
{
    deviceConnectionMode = DetectConnectionMode::noDeviceActivity;
    deviceDetectInProgress = false;
    stopTimer();
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
    startTimer(inactivityTimeoutMs);
}

void DeviceActivityMonitor::openAvailableOutputDevices()
{
    for (auto outputDeviceInfo : MidiOutput::getAvailableDevices())
    {
        outputsToPing.add(MidiOutput::openDevice(outputDeviceInfo.identifier));
    }
}

void DeviceActivityMonitor::openAvailableInputDevices()
{
    for (auto inputDeviceInfo : MidiInput::getAvailableDevices())
    {
        auto input = inputsListening.add(MidiInput::openDevice(inputDeviceInfo.identifier, this));
        input->start();
    }
}

void DeviceActivityMonitor::closeInputDevices()
{
    for (auto input : inputsListening)
    {
        input->stop();
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
    MidiDeviceInfo inputInfo = midiDriver->getMidiOutputInfo();
    MidiDeviceInfo outputInfo = midiDriver->getMidiInputInfo();
    
    if (inputInfo.identifier != "" && outputInfo.identifier != "")
    {
        deviceConnectionMode = modeToUse;
        //DBG("Testing connection of input " + inputInfo.name + " and output " + outputInfo.name);
        midiDriver->addListener(this);
        waitingForTestResponse = true;
        midiDriver->sendMessageNow(monitorMessage);
        startTimer(responseTimeoutMs);
        return true;
    }

    return false;
}

void DeviceActivityMonitor::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& response)
{
    if (midiDriver->messageIsResponseToMessage(response, monitorMessage))
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
        deviceDetectInProgress = false;
    }
    else
    {
        // Successful
        if (expectedResponseReceived)
        {
            deviceDetectInProgress = false;
            
            closeOutputDevices();
            closeInputDevices(); // Connection loss is handled in TerpstraMidiDriver::Listener callback
            
            midiDriver->setMidiInput(confirmedInputIndex);
            midiDriver->setMidiOutput(confirmedOutputIndex);
        
            sendChangeMessage();
            
            if (checkConnectionOnInactivity)
            {
                intializeConnectionLossDetection();
            }
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

void DeviceActivityMonitor::timerCallback()
{
    stopTimer();

    if (deviceConnectionMode < DetectConnectionMode::testingConnection)
    {
        if (detectDevicesIfDisconnected)
        {
            if (deviceDetectInProgress)
                checkDetectionStatus();

            else if (!isConnectionEstablished())
                initializeDeviceDetection();
        }
    }
    else if (deviceConnectionMode >= DetectConnectionMode::testingConnection)
    {
        if (waitingForTestResponse)
        {
            waitingForTestResponse = false;
            
            // If we don't receive the response soon and if a connection was previously made
            if (   deviceConnectionMode == DetectConnectionMode::waitingForInactivity
                && confirmedInputIndex >= 0 && confirmedOutputIndex >= 0
               )
            {
                DBG("DISCONNECTION DETECTED");
                
                midiDriver->closeMidiInput();
                midiDriver->closeMidiOutput();
                
                confirmedInputIndex = -1;
                confirmedOutputIndex = -1;

                expectedResponseReceived = false;
                sendChangeMessage();

                if (detectDevicesIfDisconnected)
                    initializeDeviceDetection();
            }
            
            // Connection to selected devices failed
            else
            {
                DBG("No response from selected MIDI devices.");
                deviceConnectionMode = DetectConnectionMode::noDeviceActivity;
                sendChangeMessage();
            }
        }
        else
        {
            initializeConnectionTest(deviceConnectionMode);
        }
    }
}

//=========================================================================
// TerpstraMidiDriver::Listener Implementation

void DeviceActivityMonitor::midiMessageReceived(const MidiMessage& midiMessage)
{
    if (midiMessage.isSysEx() && midiMessage.getSysExData()[5] == TerpstraMidiDriver::ACK)
    {
        waitingForTestResponse = false;
        expectedResponseReceived = true;
        
        int currentInput = midiDriver->getLastMidiInputIndex();
        int currentOutput = midiDriver->getLastMidiOutputIndex();
        
        if (confirmedInputIndex != currentInput || confirmedOutputIndex != currentOutput)
        {
            confirmedInputIndex = currentInput;
            confirmedOutputIndex = currentOutput;
            sendChangeMessage();
        }

        if (checkConnectionOnInactivity)
        {
            deviceConnectionMode = DetectConnectionMode::waitingForInactivity;
            startTimer(inactivityTimeoutMs);
        }
        else
        {
            stopTimer();
        }
    }
}
