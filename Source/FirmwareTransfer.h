/*
  ==============================================================================

    FirmwareTransfer.h
    Created: 11 Feb 2021 2:36:11am
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "TerpstraMidiDriver.h"

#define SSHCONNECTIMEOUT 5000

class FirmwareTransfer : public juce::ThreadWithProgressWindow
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
        ExecChnlErr     = -7,       // Couldn't prepare channel for reboot execution
        ThreadKillErr   = -8        // Thread was requested to shutdown before finishing
    };

public:

    FirmwareTransfer(TerpstraMidiDriver& driverIn);
    ~FirmwareTransfer();

    bool   requestFirmwareUpdate(String firmwareFilePath);
    bool   requestFirmwareDownloadAndUpdate(); // TODO


    bool   isFirmwareUpdateAvailable() { return true; /*TODO*/ }

    // TODO - member should be filled on instantiation
    String getCurrentFirmwareVersion() { return currentFirmwareVersion; }

    static String getLatestFirmwareVersion() { return ""; /*TODO*/ }


    bool isDownloadInProgress() const { return downloadRequested; }
    bool isTransferInProgress() const { return transferRequested; }

    //=========================================================================
    //juce::Thread implementation

    // Begin transfer process with currently set file
    // Will exit if file doesn't exit or upon error
    void run() override;

    //=========================================================================
    
public:

    // TODO use error codes
    static bool checkFirmwareFileIntegrity(String filePathIn);


public:

    class ProcessListener
    {
    public:
        virtual void firmwareTransferUpdate(FirmwareTransfer::StatusCode statusCode)=0;
    };

    void addListener(ProcessListener* listenerIn) { listeners.add(listenerIn); }

    void removeListener(ProcessListener* listenerIn) { listeners.remove(listenerIn); }


public:

    static double statusCodeToProgressPercent(int codeIn)
    {
        return (double)codeIn / (int)StatusCode::VerificationBegin;
    }

protected:

    ListenerList<ProcessListener> listeners;


private:

    // Return true if update was successful
    bool       prepareForUpdate();
    StatusCode performFirmwareUpdate();

    // header only in .cpp
    //static int shutdownSSHSession(LIBSSH2_SESSION*, int, FILE*, int returnCode = 0);
    //static int waitForSSHSocket(int, LIBSSH2_SESSION*); 

private:

    void postUpdate(StatusCode codeIn);

private:

    TerpstraMidiDriver& midiDriver;
    String currentFirmwareVersion;

    String selectedFileToTransfer;

    bool downloadRequested = false;
    bool transferRequested = false;

    // For shortening the built-in libssh2 timeout period
    bool progressMadeSinceUpdate = false;
};
