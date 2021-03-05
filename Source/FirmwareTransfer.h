/*
  ==============================================================================

    FirmwareTransfer.h
    Created: 11 Feb 2021 2:36:11am
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "TerpstraMidiDriver.h"

class FirmwareTransfer : public TerpstraMidiDriver::Listener
{
public:

    enum class ErrorCode
    {
        NoErr           = 0,
        StartupErr      = -1,
        HostConnectErr  = -2,
        SessionEstErr   = -3,
        AuthErr         = -4,
        ChannelErr      = -5,
        ExecChnlErr     = -6
    };

public:

    FirmwareTransfer(TerpstraMidiDriver& driverIn);
    ~FirmwareTransfer();

    int    initializeFirmwareUpdate(String firmwareFilePath);
    int    requestFirmwareDownloadAndUpdate();

    bool   isFirmwareUpdateAvailable() { return true; /*TODO*/ }

    String getCurrentFirmwareVersion() { return currentFirmwareVersion; }

    static String getLatestFirmwareVersion() { return ""; /*TODO*/ }

private:

    // header only in .cpp
    //static int shutdownSSHSession(LIBSSH2_SESSION*, int, FILE*, int returnCode = 0);
    //static int waitForSSHSocket(int, LIBSSH2_SESSION*); 

    void midiMessageReceived(const MidiMessage& midiMessage) override;
    void midiMessageSent(const MidiMessage& midiMessage) override {};
    void midiSendQueueSize(int queueSize) override {};
    void generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel) override {};

private:

    TerpstraMidiDriver& midiDriver;
    String currentFirmwareVersion;
};