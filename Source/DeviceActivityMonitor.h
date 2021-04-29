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

#include "LumatoneController.h"

class DeviceActivityMonitor : public juce::Timer,
                              public LumatoneController::FirmwareListener
                              //public LumatoneController::MidiListener
{
    
public:
    enum class DetectConnectionMode
    {
        noDeviceActivity = -1,
        noDeviceMonitoring,
        lookingForDevice,
        waitingForFirmwareUpdate, // "silent" lookingForDevice
        testingConnection,
        gettingFirmwareVersion, // connection tests with GetFirmwareRevision
        waitingForInactivity
    };
    
public:

    DeviceActivityMonitor();
    ~DeviceActivityMonitor() override;

    DetectConnectionMode getMode() const { return deviceConnectionMode; }
    bool isConnectionEstablished() const { return expectedResponseReceived && confirmedInputIndex >= 0 && confirmedOutputIndex >= 0; }
    
    bool willDetectDeviceIfDisconnected() const { return detectDevicesIfDisconnected; }
    void setDetectDeviceIfDisconnected(bool doDetection);
    
    bool willCheckForInactivity() const { return checkConnectionOnInactivity; }
    void setCheckForInactivity(bool monitorActivity);

    int getConfirmedOutputIndex() const { return confirmedOutputIndex; }
    int getConfirmedInputIndex() const { return confirmedInputIndex; }

    void testCurrentDevices() { initializeConnectionTest(); }

    void initializeDeviceDetection();

    void initializeFirmwareUpdateMode();
    void cancelFirmwareUpdateMode();

    void setResponseTimeoutMs(int timeoutMs) { responseTimeoutMs = timeoutMs; }

    // Begin monitoring for unresponsiveness from device
    void intializeConnectionLossDetection(bool inFirmwareMode = false);

    //=========================================================================
    // juce::Timer Implementation

    void timerCallback() override;

protected:
    //=========================================================================
    // LumatoneController::FirmwareListener Implementation

    void serialIdentityReceived(const int* serialBytes) override;

    void firmwareRevisionReceived(int major, int minor, int revision) override;

    void pingResponseReceived(int value) override;

private:

    /// <summary>
    /// Prepares to ping devices by refreshing available devices, opening them, and starting pinging routine
    /// </summary>
    void startDetectionRoutine();

    // Determines whether or not to try to continue device detection, tries next pair if so
    void checkDetectionStatus();

    /// <summary>
    /// Increments the output index and sends a Get Serial Identity message to this next output to listen for a response 
    /// </summary>
    void pingNextOutput();

    // TODO WITH ECHO FIRMWARE COMMAND
    ///// <summary>
    ///// Send a ping to all available outputs and prepare to listen to responses
    ///// </summary>
    //void pingAvailableDevices();

    // Turn off device detection and idle
    void stopDeviceDetection();

    void onTestResponseReceived();

    void onSuccessfulDetection();

    /// <summary>
    /// Test device connectivity after a connection has previously been made.
    /// </summary>
    /// <returns>Returns false if devices are not valid, and true if it an attempt to connect was made</returns>
    bool initializeConnectionTest(DetectConnectionMode modeToUse = DetectConnectionMode::testingConnection);

    void onDisconnection();

private:

    MidiMessage             detectMessage;

    DetectConnectionMode    deviceConnectionMode   = DetectConnectionMode::noDeviceMonitoring;
    bool                    deviceDetectInProgress = false;
    bool                    waitingForTestResponse = false;
    //bool                    activityIsPaused       = false;

    int                     responseTimeoutMs; // Pull from properties, currently 1000 by default
    int                     pingRoutineTimeoutMs = 1000;
    int                     inactivityTimeoutMs  = 500;
    
    int                     pingOutputIndex = -1;

    Array<MidiDeviceInfo>   outputDevices;
    Array<MidiDeviceInfo>   inputDevices;
    Array<uint8>            outputPingIds;

    int                     confirmedInputIndex = -1;
    int                     confirmedOutputIndex = -1;

    int                     midiQueueSize = 0;
    
    bool                    detectDevicesIfDisconnected = true;
    bool                    checkConnectionOnInactivity = true;

    bool                    expectedResponseReceived = false;
    //bool                    activitySinceLastTimeout = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeviceActivityMonitor)
};
