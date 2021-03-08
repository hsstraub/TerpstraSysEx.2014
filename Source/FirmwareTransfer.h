/*
  ==============================================================================

    FirmwareTransfer.h
    Created: 11 Feb 2021 2:36:11am
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "TerpstraMidiDriver.h"

class FirmwareTransfer : public TerpstraMidiDriver::Listener
{
public:

    enum class StatusCode
    {
        /* NON ERRORS */
        NoErr           = 0x0000,   // Firmware update was performed successfully
        Initialize,                 // Firmware update process was requested
        FileIntegrityCheck,         // Checking firmware file integrity before transfer
        SessionBegin,               // Preparing communication with Lumatone
        AuthBegin,                  // Authenticating connection
        TransferBegin,              // File transfer started
        InstallBegin,               // Reboot request sent
        VerificationBegin,          // Sent GetFirmwareVersion sysex and waiting for answer
        
        /* ERRORS */
        IntegrityErr    = -1,       // Firmware file integrity check is not successful
        StartupErr      = -2,       // Couldn't initialize libssh2 or open firmware file
        HostConnectErr  = -3,       // Couldn't create socket or session
        SessionEstErr   = -4,       // Session handshake failed
        AuthErr         = -5,       // Password authentication failed
        ChannelErr      = -6,       // Couldn't open a channel (for file transfer or reboot execution)
        ExecChnlErr     = -7        // Couldn't prepare channel for reboot execution
    };

public:

    FirmwareTransfer(TerpstraMidiDriver& driverIn);
    ~FirmwareTransfer();

    /// <summary>
    /// Return 0 on success, nonzero on error
    /// </summary>
    /// <param name="firmwareFilePath"></param>
    /// <returns></returns>
    int    checkFirmwareFileIntegrity(String firmwareFilePath); /***TODO****/

    bool   startFirmwareUpdate(String firmwareFilePath);
    bool   requestFirmwareDownloadAndUpdate();


    bool   isFirmwareUpdateAvailable() { return true; /*TODO*/ }

    String getCurrentFirmwareVersion() { return currentFirmwareVersion; }

    static String getLatestFirmwareVersion() { return ""; /*TODO*/ }


    //=========================================================================

    class Listener
    {
    public:
        virtual void firmwareTransferUpdate(FirmwareTransfer::StatusCode statusCode)=0;
    };

    void addListener(Listener* listenerIn) { listeners.add(listenerIn); }

    void removeListener(Listener* listenerIn) { listeners.remove(listenerIn); }

    
protected:

    ListenerList<Listener> listeners;


private:

    StatusCode performFirmwareUpdate(String firmwareFilePath);

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