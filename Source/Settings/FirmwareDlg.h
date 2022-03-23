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
    protected LumatoneEditor::FirmwareListener,
    private Timer
{
public:

    FirmwareDlg();
    ~FirmwareDlg();

    void paint(Graphics& g) override;

    void resized() override;

    void buttonClicked(Button* btn) override;

    void updateFirmwareVersionLabel();

    void postMessage(String msgForLog);
    
    //=========================================================================
    // PathBrowserComponent::Listener Implementation
    void fileChanged(PathBrowserComponent* source, File) override;

    //=========================================================================
    // FirmwareTransfer::Listener Implementation
    void firmwareTransferUpdate(FirmwareTransfer::StatusCode statusCode, String msg) override;

    //=========================================================================
    // LumatoneEditor::FirmwareListener implementation

    void firmwareRevisionReceived(FirmwareVersion version) override;
    
    //=========================================================================
    // juce::Timer Implementation
    void timerCallback() override;

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

    // Style helpers
    int margin = 12;
    int doubleMargin = margin * 2;
    float buttonWidthScalar = 0.3f;
    int buttonHeight = 30;
};
