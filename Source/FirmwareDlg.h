/*
  ==============================================================================

    FirmwareDlg.h
    Created: 5 Feb 2021 7:33:07pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "FileBrowserComponent.h"


class FirmwareDlg : public Component, protected Button::Listener, protected PathBrowserComponent::Listener
{
public:

    FirmwareDlg();
    ~FirmwareDlg();

    void paint(Graphics& g) override;

    void resized() override;

    void lookAndFeelChanged() override;

    void buttonClicked(Button* btn) override;

    void fileChanged(PathBrowserComponent* source, File) override;

private:

    bool updateIsAvailable = false;
    
    File firmwareFileSelected;

    //std::unique_ptr<TextButton> checkUpdateBtn;
    std::unique_ptr<PathBrowserComponent> fileBrowser;
    std::unique_ptr<TextButton> doUpdateBtn;
    std::unique_ptr<TextEditor> infoBox;
};