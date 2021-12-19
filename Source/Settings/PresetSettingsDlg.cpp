/*
  ==============================================================================

    PresetSettingsDlg.cpp
    Created: 29 Jun 2021 9:10:00pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "PresetSettingsDlg.h"
#include "../Main.h"

PresetSettingsDlg::PresetSettingsDlg()
{
    resetPresetsBtn.reset(new TextButton(
        translate("Reset Presets To Factory Default"),
        translate("Clear the mapping presets stored on the device and replace with factory mappings.")
    ));
    addAndMakeVisible(resetPresetsBtn.get());
    resetPresetsBtn->addListener(this);

    TerpstraSysExApplication::getApp().getLumatoneController()->addFirmwareListener(this);

    setSupportedControls(TerpstraSysExApplication::getApp().getFirmwareVersion());

    flexBox.justifyContent = FlexBox::JustifyContent::flexStart;
    flexBox.alignContent = FlexBox::AlignContent::flexStart;
}

PresetSettingsDlg::~PresetSettingsDlg()
{
    TerpstraSysExApplication::getApp().getLumatoneController()->removeFirmwareListener(this);
    resetPresetsBtn = nullptr;
}

void PresetSettingsDlg::paint(Graphics& g)
{

}

void PresetSettingsDlg::resized()
{
    Font btnFont = getLookAndFeel().getTextButtonFont(*resetPresetsBtn.get(), buttonHeight);

    int btnWidth = btnFont.getStringWidth(resetPresetsBtn->getButtonText()) * 1.2f;

    flexBox.items.clear();

    FlexItem btnItem = FlexItem().withWidth(btnWidth).withHeight(buttonHeight);
    btnItem.associatedComponent = resetPresetsBtn.get();
    flexBox.items.add(btnItem);

    flexBox.performLayout(getLocalBounds().reduced(margin));
}

void PresetSettingsDlg::buttonClicked(Button* btn)
{
    if (btn == resetPresetsBtn.get())
    {
        TerpstraSysExApplication::getApp().getLumatoneController()->resetPresetsToFactoryDefault();
    }
}

void PresetSettingsDlg::firmwareRevisionReceived(FirmwareVersion version)
{
    setSupportedControls(version);
}

void PresetSettingsDlg::setSupportedControls(FirmwareVersion version)
{
    FirmwareSupport support;
    if (support.versionAcknowledgesCommand(version, RESET_DEFAULT_PRESETS))
    {
        resetPresetsBtn->setEnabled(true);
        resetPresetsBtn->setTooltip(translate("Clear the mapping presets stored on the device and replace with factory mappings."));
    }
    else
    {
        resetPresetsBtn->setEnabled(false);
        resetPresetsBtn->setTooltip(translate("This feature is not supported by your Lumatone firmware version."));
        // TODO: better approach for changing tooltips
    }
}