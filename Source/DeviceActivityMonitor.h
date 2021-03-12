/*
  ==============================================================================

    DeviceActivityMonitor.h
    Created: 11 Feb 2021 9:15:43pm
    Author:  Vincenzo

    Finds a pair of devices connected to Lumatone, and monitors activity 
    to detect if the connection to the device is lost.

    Sends a change message when a change in the connection status is detected;

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

    bool isSearchingForLumatone() const { return deviceDetectInProgress; }

    bool isConnectionEstablished() const { return expectedResponseReceived && confirmedMidiInput.identifier != "" && confirmedMidiOutput.identifier != ""; }

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

    void stopDeviceDetection();

    // Determines whether or not to try to continue device detection, tries next pair if so
    void checkDetectionStatus();

    // TODO
    void intializeConnectionLossDetection();

    int getConfirmedOutputIndex() const { return confirmedOutputIndex; }
    int getConfirmedInputIndex() const { return confirmedInputIndex; }

    void openAvailableOutputDevices();
    void openAvailableInputDevices();

    void closeInputDevices();
    void closeOutputDevices();

    /// <summary>
    /// Test device connectivity after already having confirmed the connection.
    /// </summary>
    /// <returns>Returns false if connection not previously confirmed, and true if it an attempt to connect was made</returns>
    bool initializeConnectionTest();

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
        waitingForInactivity,
        testingConnection
    };

    TerpstraMidiDriver*     midiDriver;

    DetectConnectionMode    deviceConnectionMode   = noActivity;
    bool                    deviceDetectInProgress = false;

    const int               responseTimeoutMs    = 1000; // Will have to try to adjust when we can test with hardware
    const int               pingRoutineTimeoutMs = 1000;
    const int               inactivityTimeoutMs   = 2000; // TODO: set to 30000
    
    int                     pingOutputIndex = -1;

    OwnedArray<MidiOutput>  outputsToPing;
    OwnedArray<MidiInput>   inputsListening;

    int                     confirmedInputIndex = -1;
    int                     confirmedOutputIndex = -1;
    
    bool                    detectDevicesIfDisconnected = true;
    bool                    checkConnectionOnInactivity = true;
    bool                    waitingForTestResponse = false;

    MidiMessage             monitorMessage;
    bool                    expectedResponseReceived = false;

    MidiDeviceInfo          confirmedMidiInput;
    MidiDeviceInfo          confirmedMidiOutput;
};
