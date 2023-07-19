/*
  ==============================================================================

    PresetSettingsDlg.h
    Created: 29 Jun 2021 9:10:00pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "../LumatoneController.h"

class PresetSettingsDlg : public Component,
    protected Button::Listener,
    protected LumatoneEditor::FirmwareListener

{
public:

    PresetSettingsDlg();
    ~PresetSettingsDlg();

    void paint(Graphics& g) override;

    void resized() override;

    void buttonClicked(Button* btn) override;

    void setSupportedControls(FirmwareVersion version);

    //=========================================================================
    // LumatoneEditor::FirmwareListener implementation

    void firmwareRevisionReceived(FirmwareVersion version) override;


private:

    std::unique_ptr<TextButton> resetPresetsBtn;
    FlexBox flexBox;

    // Style helpers
    int margin = 12;
    int buttonHeight = 30;
};