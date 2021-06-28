/*
  ==============================================================================

    FirmwareTransfer.h
    Created: 11 Feb 2021 2:36:11am
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "TerpstraMidiDriver.h"

#define UPDATETIMEOUT 5000

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
    bool   requestFirmwareDownloadAndUpdate(); // TODO - probably better moved elsewhere

    bool   isFirmwareUpdateAvailable() { return true; /*TODO*/ }

    // TODO - member should be filled on instantiation
    String getCurrentFirmwareVersion() { return currentFirmwareVersion; }

    static String getLatestFirmwareVersion() { return ""; /*TODO*/ }

    bool isDownloadInProgress() const { return downloadRequested; }
    bool isTransferInProgress() const { return transferRequested; }

    void incrementProgress();

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
        virtual void firmwareTransferUpdate(FirmwareTransfer::StatusCode statusCode, String msg)=0;
    };

    void addTransferListener(ProcessListener* listenerIn) { listeners.add(listenerIn); }

    void removeTransferListener(ProcessListener* listenerIn) { listeners.remove(listenerIn); }

protected:

    ListenerList<ProcessListener> listeners;


private:

    // Return true if update was successful
    bool       prepareAndRunUpdate();
    StatusCode performFirmwareUpdate();

    // header only in .cpp
    //static int shutdownSSHSession(LIBSSH2_SESSION*, int, FILE*, int returnCode = 0);
    //static int waitForSSHSocket(int, LIBSSH2_SESSION*); 

private:

    void postUpdate(StatusCode codeIn);

    double numIncrementsToProgress(int numberOfIncrements) { return numberOfIncrements / (double)maxUpdateIncrements; }

private:

    TerpstraMidiDriver& midiDriver;
    String currentFirmwareVersion;

    String selectedFileToTransfer;

    bool downloadRequested = false;
    bool transferRequested = false;

    // For shortening the built-in libssh2 timeout period
    bool progressMadeSinceUpdate = false;

    int numberOfWaitIncrements = 0;
    // Estimation based on boot time of ~85 seconds, plus transfer time, and overhead
    const int maxUpdateIncrements = 300000 / UPDATETIMEOUT;

public:

    static String statusCodeToMessage(FirmwareTransfer::StatusCode statusCode)
    {
        switch (statusCode)
        {
        case FirmwareTransfer::StatusCode::Initialize:
            return translate("Firmware update process initiated!");

        case FirmwareTransfer::StatusCode::FileIntegrityCheck:
            return translate("Checking integrity of firmware file...");

        case FirmwareTransfer::StatusCode::SessionBegin:
            return translate("Establishing connection to Lumatone...");

        case FirmwareTransfer::StatusCode::AuthBegin:
            return translate("Providing credentials...");

        case FirmwareTransfer::StatusCode::TransferBegin:
            return translate("Transferring firmware file...");

        case FirmwareTransfer::StatusCode::InstallBegin:
            return translate("Rebooting device for installation");

        case FirmwareTransfer::StatusCode::VerificationBegin:
            return translate("Installing update...");

        case FirmwareTransfer::StatusCode::IntegrityErr:
            return translate("Error: Not a valid Lumatone firmware file");

        case FirmwareTransfer::StatusCode::StartupErr:
            return translate("Error: Could not prepare device communication protool");

        case FirmwareTransfer::StatusCode::HostConnectErr:
            return translate("Error: Could not communicate with Lumatone");

        case FirmwareTransfer::StatusCode::SessionEstErr:
            return translate("Error: Could not verify connection with Lumatone");

        case FirmwareTransfer::StatusCode::AuthErr:
            return translate("Error: Authentication for Lumatone failed");

        case FirmwareTransfer::StatusCode::ChannelErr:
            return translate("Error: Communication channel failure");

        case FirmwareTransfer::StatusCode::ExecChnlErr:
            return translate("Error: Could not request device reboot. Try rebooting your Lumatone.");

        case FirmwareTransfer::StatusCode::NoErr:
            return translate("Please wait for your Lumatone to install the firmware.");

        default:
            return translate("Unspecified error.");
        }
    }

    static double statusCodeToProgressPercent(int codeIn)
    {
        return (double)codeIn / (int)FirmwareTransfer::StatusCode::VerificationBegin;
    }
};
