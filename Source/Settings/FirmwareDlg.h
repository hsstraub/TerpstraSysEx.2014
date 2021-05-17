/*
  ==============================================================================

    FirmwareDlg.h
    Created: 5 Feb 2021 7:33:07pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "../FileBrowserComponent.h"
#include "../FirmwareTransfer.h"
#include "../LumatoneController.h"


class FirmwareDlg : public Component, 
    protected Button::Listener, 
    protected PathBrowserComponent::Listener,
    protected FirmwareTransfer::ProcessListener,
    protected LumatoneController::FirmwareListener,
    private Timer
{
public:

    FirmwareDlg();
    ~FirmwareDlg();

    void paint(Graphics& g) override;

    void resized() override;

    void buttonClicked(Button* btn) override;

    void updateFirmwareVersionLabel();
    
    //=========================================================================
    // PathBrowserComponent::Listener Implementation
    void fileChanged(PathBrowserComponent* source, File) override;

    //=========================================================================
    // FirmwareTransfer::Listener Implementation
    void firmwareTransferUpdate(FirmwareTransfer::StatusCode statusCode, String msg) override;

    //=========================================================================
    // LumatoneController::FirmwareListener implementation

    void firmwareRevisionReceived(int major, int minor, int revision) override;
    
    //=========================================================================
    // juce::Timer Implementation
    void timerCallback() override;

private:

    double numIncrementsToProgress(int numberOfIncrements);

private:

    bool updateIsAvailable = false;
    bool firmwareUpdateInProgress = false;

    File firmwareFileSelected;

    //std::unique_ptr<TextButton> checkUpdateBtn;
    std::unique_ptr<PathBrowserComponent> fileBrowser;
    std::unique_ptr<TextButton> doUpdateBtn;
    std::unique_ptr<TextEditor> infoBox;
    
    std::unique_ptr<Label> firmwareStatusLabel;

    String msgLog;
    bool infoNeedsUpdate = false;
    const int infoUpdateTimeoutMs = 100;
};
