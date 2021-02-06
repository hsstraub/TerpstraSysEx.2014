/*
  ==============================================================================

    FirmwareDlg.h
    Created: 5 Feb 2021 7:33:07pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class FirmwareDlg : public Component, protected Button::Listener
{
public:

    FirmwareDlg();
    ~FirmwareDlg();

    void paint(Graphics& g) override;

    void resized() override;

    void buttonClicked(Button* btn) override;

private:

    bool updateIsAvailable = false;

    std::unique_ptr<TextButton> checkUpdateBtn;
    std::unique_ptr<TextButton> doUpdateBtn;
};