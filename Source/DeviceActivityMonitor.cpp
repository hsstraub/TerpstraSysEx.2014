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
    : midiDriver(&TerpstraSysExApplication::getApp().getMidiDriver())
{
    monitorMessage = midiDriver->getSerialIdentityRequestMessage();
}

DeviceActivityMonitor::~DeviceActivityMonitor()
{
    closeInputDevices();
    closeOutputDevices();
}

void DeviceActivityMonitor::initializeDeviceDetection()
{ 
    midiDriver->removeListener(this);

    deviceConnectionMode = DetectConnectionMode::lookingForDevice;

    pingOutputIndex = -1;

    closeInputDevices();
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
    if (deviceConnectionMode == DetectConnectionMode::lookingForDevice)
    {
        deviceDetectInProgress = false;
        stopTimer();
    }
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
    jassert(confirmedMidiInput.identifier != "" && confirmedMidiOutput.identifier != "");

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

bool DeviceActivityMonitor::initializeConnectionTest()
{
    deviceConnectionMode = DetectConnectionMode::testingConnection;

    if (confirmedMidiInput.identifier != "" && confirmedMidiOutput.identifier != "")
    {
        DBG("Testing connection to " + confirmedMidiInput.name);
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

            confirmedMidiOutput = outputsToPing[pingOutputIndex]->getDeviceInfo();
            confirmedOutputIndex = pingOutputIndex;

            confirmedMidiInput = source->getDeviceInfo();
            confirmedInputIndex = inputsListening.indexOf(source);

            closeOutputDevices();
            closeInputDevices(); // Connection loss is handled in TerpstraMidiDriver::Listener callback
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
            sendChangeMessage();

            if (checkConnectionOnInactivity)
            {
                intializeConnectionLossDetection(); // TODO
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

    if (deviceConnectionMode < DetectConnectionMode::waitingForInactivity)
    {
        if (detectDevicesIfDisconnected)
        {
            if (deviceDetectInProgress)
                checkDetectionStatus();

            else
                initializeDeviceDetection();
        }
    }
    else if (deviceConnectionMode >= DetectConnectionMode::waitingForInactivity)
    {
        if (waitingForTestResponse)
        {
            DBG("DISCONNECTION DETECTED");

            midiDriver->closeMidiInput();
            midiDriver->closeMidiOutput();

            expectedResponseReceived = false;
            sendChangeMessage();

            if (detectDevicesIfDisconnected)
                initializeDeviceDetection();
        }
        else
        {
            waitingForTestResponse = true;
            initializeConnectionTest();
        }
    }
}

//=========================================================================
// TerpstraMidiDriver::Listener Implementation, only for connection loss detection

void DeviceActivityMonitor::midiMessageReceived(const MidiMessage& midiMessage)
{
    jassert(confirmedMidiInput.identifier != "" && confirmedMidiOutput.identifier != "");
    if (deviceConnectionMode >= DetectConnectionMode::waitingForInactivity)
    {
        waitingForTestResponse = false;
        expectedResponseReceived = true;

        if (checkConnectionOnInactivity)
        {
            deviceConnectionMode = DetectConnectionMode::waitingForInactivity;
            startTimer(inactivityTimeoutMs);
        }
    }
}

void DeviceActivityMonitor::generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel)
{
    // TODO
}
