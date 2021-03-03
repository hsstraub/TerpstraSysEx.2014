/*
  ==============================================================================

    DeviceActivityMonitor.h
    Created: 11 Feb 2021 9:15:43pm
    Author:  Vincenzo

    Uses methods from TerpstraMidiDriver in parallel to detect the MIDI device the Lumatone is connected to
    and monitors activity listening to TerpstraMidiDriver to detect if the connection to the device is lost.

  ==============================================================================
*/

#pragma once

#include "TerpstraMidiDriver.h"

class DeviceActivityMonitor : public juce::MidiInputCallback,
                              public TerpstraMidiDriver::Listener, 
                              public juce::Timer,
                              public juce::ChangeBroadcaster
{
public:

    DeviceActivityMonitor();
    ~DeviceActivityMonitor() override;

    /// <summary>
    /// Prepares to ping devices by refreshing available devices, opening them, and starting pinging routine
    /// </summary>
    void initializeDeviceDetection();

    /// <summary>
    /// Increments the output index and sends a Get Serial Identity message to this next output to listen for a response 
    /// </summary>
    void pingNextOutput();

    ///// <summary>
    ///// Send a ping to all available outputs and prepare to listen to responses
    ///// </summary>
    //void pingAvailableDevices();

    // Determines whether or not to try to continue device detection, tries next pair if so
    void checkDetectionStatus();

    // TODO
    void intializeConnectionLossDetection();


    int getConfirmedOutputIndex() const { return confirmedOutputIndex; }

    int getConfirmedInputIndex() const { return confirmedInputIndex; }


    void closeInputDevices();

    void closeOutputDevices();


    //=========================================================================
    // juce::MidiInputCallback Implementation
    
    void handleIncomingMidiMessage(MidiInput*, const MidiMessage&) override;


    //=========================================================================
    // juce::Timer Implementation
    
    void timerCallback() override;


protected:

    //=========================================================================
    // TerpstraMidiDriver::Listener Implementation, only for connection loss detection

    void midiMessageReceived(const MidiMessage& midiMessage) override;
    void midiMessageSent(const MidiMessage& midiMessage) override {}
    void midiSendQueueSize(int queueSize) override {}
    void generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel) override;


private:

    enum DetectConnectionMode
    {
        noActivity = 0,
        lookingForDevice,
        waitingForConnectionLoss
    };

    DetectConnectionMode    deviceConnectionMode   = noActivity;
    bool                    deviceDetectInProgress = false;

    const int               responseTimeoutMs    = 1000; // Will have to try to adjust when we can test with hardware
    const int               pingRoutineTimeoutMs = 1000;
    
    int                     pingOutputIndex = -1;

    OwnedArray<MidiOutput>  outputsToPing;
    OwnedArray<MidiInput>   inputsListening;

    int                     confirmedInputIndex;
    int                     confirmedOutputIndex;
    
    bool                    detectDevicesIfDisconnected = true;
    bool                    checkConnectionOnInactivity = true;


    MidiMessage                               monitorMessage;
    std::function<bool(const MidiMessage&)>   messageIsExpectedResponse = [&](const MidiMessage&) { return false; };
    bool                                      expectedResponseReceived = false;

};
