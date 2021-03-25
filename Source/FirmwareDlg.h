/*
  ==============================================================================

    FirmwareDlg.h
    Created: 5 Feb 2021 7:33:07pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "FileBrowserComponent.h"
#include "FirmwareTransfer.h"


class FirmwareDlg : public Component, 
    protected Button::Listener, 
    protected PathBrowserComponent::Listener,
    protected FirmwareTransfer::ProcessListener,
    protected Thread::Listener,
    protected TerpstraMidiDriver::Listener, // Probably could be done in FirmwareTransfer
    private Timer
{
public:

    FirmwareDlg();
    ~FirmwareDlg();

    void paint(Graphics& g) override;

    void resized() override;

    void lookAndFeelChanged() override;

    void buttonClicked(Button* btn) override;

    void updateFirmwareVersionLabel();
    
    //=========================================================================
    // PathBrowserComponent::Listener Implementation
    void fileChanged(PathBrowserComponent* source, File) override;

    //=========================================================================
    // FirmwareTransfer::Listener Implementation
    void firmwareTransferUpdate(FirmwareTransfer::StatusCode statusCode) override;

    //=========================================================================
    // TerpstraMidiDriver::Listener implementation

    void midiMessageReceived(const MidiMessage& midiMessage) override;
    void midiMessageSent(const MidiMessage& midiMessage) override {};
    void midiSendQueueSize(int queueSize) override {};
    void generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel) override {};


    //=========================================================================
    // juce::Thread::Listener Implementation
    void exitSignalSent() override;
    
    //=========================================================================
    // juce::Timer Implementation
    void timerCallback() override;

private:
    
    void initializeFirmwareUpdate();

    void initializeWaitForUpdate();

    double numIncrementsToProgress(int numberOfIncrements);
    
private:

    bool updateIsAvailable = false;
    bool firmwareUpdateInProgress = false;

    File firmwareFileSelected;

    std::unique_ptr<FirmwareTransfer> firmwareTransfer;

    //std::unique_ptr<TextButton> checkUpdateBtn;
    std::unique_ptr<PathBrowserComponent> fileBrowser;
    std::unique_ptr<TextButton> doUpdateBtn;
    std::unique_ptr<TextEditor> infoBox;
    
    std::unique_ptr<Label> firmwareStatusLabel;

    String msgLog;
    bool infoNeedsUpdate = false;
    const int infoUpdateTimeoutMs = 100;

    bool waitingForUpdateConfirmation = false;
    int numberOfWaitIncrements = 0;
    const int updateIncrementTimeoutMs = 5000;
    // Estimation based on boot time of ~85 seconds, plus transfer time, and overhead
    int maxUpdateIncrements = 220000 / updateIncrementTimeoutMs;
};
