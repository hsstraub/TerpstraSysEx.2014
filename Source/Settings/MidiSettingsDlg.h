/*
  ==============================================================================

    MidiSettingsDlg.h
    Created: 29 Jun 2021 10:02:43pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../LumatoneController.h"

class MidiSettingsDlg : public Component,
    protected Slider::Listener,
    protected LumatoneEditor::FirmwareListener
{
public:

    MidiSettingsDlg();
    ~MidiSettingsDlg();

    void paint(Graphics & g) override;

    void resized() override;

    void sliderValueChanged(Slider* sld) override;

    void setSupportedControls(FirmwareVersion version);

    void updateChannelSettings(PeripheralChannelSettings channelSettings);

    void sendChannelSettings();

    //=========================================================================
    // LumatoneEditor::FirmwareListener implementation

    void firmwareRevisionReceived(FirmwareVersion version) override;

    void peripheralMidiChannelsReceived(PeripheralChannelSettings channelSettings) override;



private:
    //=========================================================================

    const StringArray ControlNames = 
    {
        "Pitch Wheel",
        "Mod Wheel",
        "Expression Pedal",
        "Sustain Pedal"
    };

private:

    std::unique_ptr<Label>      setMidiChannelHeader;

    OwnedArray<Slider>          setMidiChannelSliders;
    OwnedArray<Label>           setMidiChannelLabels;
    
    PeripheralChannelSettings   channelSettings;
        
    FlexBox flexBox;
    Array<FlexBox> flexRows;

    // Style helpers
    const int margin = 12;
    const int buttonHeight = 30;
    const float fontHeightInBounds = 0.08f;

    String longestControlName;
    Font controlLabelFont;
};