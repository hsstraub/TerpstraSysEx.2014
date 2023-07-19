/*
  ==============================================================================

    DeviceActivityMonitor.h
    Created: 11 Feb 2021 9:15:43pm
    Author:  Vincenzo

    Finds a pair of devices connected to Lumatone, and monitors activity 
    to detect if the connection to the device is lost. When a change is detected
    a change signal will be broadcasted (for LumatoneController).
 
    When looking for devices, a response timeout is required, but once a device
    has been confirmed, the timing and callbacks are dependent on TerpstraMidiDriver.

  ==============================================================================
*/

#pragma once

#include "TerpstraMidiDriver.h"

class DeviceActivityMonitor : public juce::Timer, 
                              public juce::ChangeBroadcaster, 
                              protected TerpstraMidiDriver::Collector
{
    
public:
    enum class DetectConnectionMode
    {
        noDeviceActivity = -1,
        lookingForDevice,
        noDeviceMonitoring,
        waitingForInactivity
    };
    
public:

    DeviceActivityMonitor(TerpstraMidiDriver& midiDriverIn);
    ~DeviceActivityMonitor() override;

    DetectConnectionMode getMode() const { return deviceConnectionMode; }
    bool isConnectionEstablished() const { return confirmedInputIndex >= 0 && confirmedOutputIndex >= 0; }
    
    bool willDetectDeviceIfDisconnected() const { return detectDevicesIfDisconnected; }
    void setDetectDeviceIfDisconnected(bool doDetection);
    
    bool willCheckForInactivity() const { return checkConnectionOnInactivity; }
    void setCheckForInactivity(bool monitorActivity);

    int getConfirmedOutputIndex() const { return confirmedOutputIndex; }
    int getConfirmedInputIndex() const { return confirmedInputIndex; }

    // Start monitoring available MIDI devices and wait for an expected response
    // First uses the "Ping" command to send to all available devices,
    // then goes through a legacy-supported routine with individual output devices.
    void initializeDeviceDetection();

    // Set the timeout for message responses
    void setResponseTimeoutMs(int timeoutMs) { responseTimeoutMs = timeoutMs; }

    // Begin polling selected device until it stops responding. Other messages
    // will reset the inactivity timer.
    void intializeConnectionLossDetection();

    // Turn off device monitoring and idle
    void stopMonitoringDevice();

    //=========================================================================
    // juce::Timer Implementation

    void timerCallback() override;

    
private:

    /// <summary>
    /// Sends a ping to all devices to see if we get a response
    /// </summary>
    void pingAllDevices();

    /// <summary>
    /// If a MIDI device identifier is defined in properties, send a GetSerialIdentity to it.
    /// Returns whether or not a test response was sent
    /// </summary>
    bool testLastConnectedDevice();

    /// <summary>
    /// Prepares to ping devices by refreshing available devices, opening them, and starting pinging routine
    /// </summary>
    void startIndividualDetection();

    /// <summary>
    /// Checks detection routines, will move on from Ping routine if no answers, then will drive the legacy detection routine
    /// </summary>
    void checkDetectionStatus();

    /// <summary>
    /// Increments the output index and sends a Get Serial Identity message to this next output to listen for a response 
    /// </summary>
    void testNextOutput();

    // Turn off device detection and idle
    void stopDeviceDetection();

    /// <summary>
    /// Test device connectivity after a connection has previously been made.
    /// </summary>
    /// <returns>Returns false if devices are not valid, and true if it an attempt to connect was made</returns>
    bool initializeConnectionTest();

    /// <summary>
    /// Handle a response from a test or confirmed MidiInput device
    /// </summary>
    /// <param name="testInputIndex"></param>
    /// <param name="midiMessage"></param>
    void handleResponse(int inputDeviceIndex, const MidiMessage& midiMessage);


private:
    //=========================================================================
    // Callback functions

    void handleMessageQueue(const MidiBuffer& readBuffer, const Array<int, CriticalSection>& devices);

    void onSerialIdentityResponse(const MidiMessage& msg, int deviceIndexResponded);

    void onFailedPing(const MidiMessage& msg);
    void onPingResponse(const MidiMessage& msg, int deviceIndexResponded);

    void onTestResponseReceived();

    void onSuccessfulDetection();

    void onDisconnection();
    
protected:

    //=========================================================================
    // TerpstraMidiDriver::Listener Implementation

    void midiMessageReceived(MidiInput* source, const MidiMessage& midiMessage) override;
    void midiMessageSent(MidiOutput* target, const MidiMessage& midiMessage) override {}
    void midiSendQueueSize(int queueSizeIn) override { sentQueueSize = queueSizeIn; }
    void generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel) override {}
    void noAnswerToMessage(MidiInput* expectedDevice, const MidiMessage& midiMessage) override;

private:

    TerpstraMidiDriver&     midiDriver;

    DetectConnectionMode    deviceConnectionMode   = DetectConnectionMode::noDeviceActivity;
    bool                    deviceDetectInProgress = false;
    bool                    waitingForTestResponse = false;

    int                     responseTimeoutMs = 600;
    int                     detectRoutineTimeoutMs = 1000;
    int                     inactivityTimeoutMs  = 1500;

    Array<int, CriticalSection> testResponseDeviceIndices;
    std::atomic<int>        readQueueSize;
    const int               readBlockSize = 64;
    int                     sentQueueSize = 0;

    int                     testOutputIndex = -1;
    Array<MidiDeviceInfo>   outputDevices;
    Array<MidiDeviceInfo>   inputDevices;
    Array<unsigned int>     outputPingIds;

    int                     confirmedInputIndex = -1;
    int                     confirmedOutputIndex = -1;

    bool                    detectDevicesIfDisconnected = true;
    bool                    checkConnectionOnInactivity = true;
    
    bool                    sendCalibratePitchModOff = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeviceActivityMonitor)
};
