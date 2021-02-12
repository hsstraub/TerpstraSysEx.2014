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

}

DeviceActivityMonitor::~DeviceActivityMonitor()
{
    closeInputDevices();
    closeOutputDevices();
}

void DeviceActivityMonitor::initializeDeviceDetection()
{
    TerpstraMidiDriver& midiDriver = TerpstraSysExApplication::getApp().getMidiDriver();

    if (deviceConnectionMode != DetectConnectionMode::lookingForDevice)
    {
        deviceConnectionMode = DetectConnectionMode::lookingForDevice;
        monitorMessage = midiDriver.getSerialIdentityRequestMessage();
    }
    
    // Refresh available devices
    midiDriver.refreshDeviceLists();

    closeInputDevices();
    closeOutputDevices();
    pingOutputIndex = -1;

    for (auto outputDeviceInfo : MidiOutput::getAvailableDevices())
    {
        outputsToPing.add(MidiOutput::openDevice(outputDeviceInfo.identifier));
    }



    for (auto inputDeviceInfo : MidiInput::getAvailableDevices())
    {
        auto input = inputsListening.add(MidiInput::openDevice(inputDeviceInfo.identifier, this));
        input->start();
    }

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

    if (pingOutputIndex >= 0 && pingOutputIndex < outputsToPing.size())
    {
        DBG("Pinging " + outputsToPing[pingOutputIndex]->getName());
        TerpstraSysExApplication::getApp().getMidiDriver().sendMessageNow(monitorMessage);
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

//void DeviceActivityMonitor::pingAvailableDevices()
//{
//    const MidiMessage pingMsg = TerpstraSysExApplication::getApp().getMidiDriver().getSerialIdentityRequestMessage();
//
//    for (auto output : outputsToPing)
//    {
//        output
//    }
//}

void DeviceActivityMonitor::intializeConnectionLossDetection()
{
    deviceConnectionMode = DetectConnectionMode::waitingForConnectionLoss;

    // TODO
}

void DeviceActivityMonitor::closeInputDevices()
{
    for (auto inputs : inputsListening)
    {
        inputs->stop();
    }

    inputsListening.clear();
}

void DeviceActivityMonitor::closeOutputDevices()
{
    // juce::MidiOutput stops background threads in its destructor
    outputsToPing.clear();
}

void DeviceActivityMonitor::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& response)
{
    // Check if SysEx and expected manufacturer ID, board ID, and command ID
    bool isExpectedResponseType = TerpstraSysExApplication::getApp().getMidiDriver().messageIsResponseToMessage(response, monitorMessage);
    if (isExpectedResponseType)
    {
        auto data = response.getSysExData();

        // Is a duplicate of the request
        if (data[5] == TerpstraMidiDriver::TerpstraMIDIAnswerReturnCode::NACK)
        {
            return;
        }

        // Is an acknowledged answer and success connection
        else
        {
//            const MessageManagerLock mml;
//            DBG("Response received from: " + source->getName());
            confirmedOutputIndex = pingOutputIndex;
            confirmedInputIndex = inputsListening.indexOf(source);
            expectedResponseReceived = true;
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

    if (deviceConnectionMode < DetectConnectionMode::waitingForConnectionLoss)
    {
        if (detectDevicesIfDisconnected)
        {
            if (deviceDetectInProgress)
                checkDetectionStatus();

            else
                initializeDeviceDetection();
        }
    }
    else if (deviceConnectionMode == DetectConnectionMode::waitingForConnectionLoss)
    {
        // TODO
        // Maybe look at the last message time stamp and only check if there hasn't been activity
        // Then stop if there is
    }
}

//=========================================================================
// TerpstraMidiDriver::Listener Implementation, only for connection loss detection

void DeviceActivityMonitor::midiMessageReceived(const MidiMessage& midiMessage)
{
    // TODO
}

void DeviceActivityMonitor::generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel)
{
    // TODO
}
