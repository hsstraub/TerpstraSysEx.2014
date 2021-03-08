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
    protected FirmwareTransfer::Listener
{
public:

    FirmwareDlg();
    ~FirmwareDlg();

    void paint(Graphics& g) override;

    void resized() override;

    void lookAndFeelChanged() override;

    void buttonClicked(Button* btn) override;

    //=========================================================================
    // PathBrowserComponent::Listener Implementation
    void fileChanged(PathBrowserComponent* source, File) override;

    //=========================================================================
    // FirmwareTransfer::Listener Implementation
    void firmwareTransferUpdate(FirmwareTransfer::StatusCode statusCode) override;

private:

    bool updateIsAvailable = false;
    bool firmwareUpdateInProgress = false;

    File firmwareFileSelected;

    //std::unique_ptr<TextButton> checkUpdateBtn;
    std::unique_ptr<PathBrowserComponent> fileBrowser;
    std::unique_ptr<TextButton> doUpdateBtn;
    std::unique_ptr<TextEditor> infoBox;
};