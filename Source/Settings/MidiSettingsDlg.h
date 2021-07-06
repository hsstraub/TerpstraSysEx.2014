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
    protected ToggleButton::Listener,
    protected LumatoneController::FirmwareListener
{
public:

    MidiSettingsDlg();
    ~MidiSettingsDlg();

    void paint(Graphics & g) override;

    void resized() override;

    void sliderValueChanged(Slider* sld) override;

    void buttonClicked(Button* btn) override;

    void setSupportedControls(FirmwareVersion version);

    void updateChannelSettings(int pitchWheelChannel, int modWheelChannel, int expressionChannel, int sustainPedalChannel);

    void sendChannelSettings();

    //=========================================================================
    // LumatoneController::FirmwareListener implementation

    void firmwareRevisionReceived(FirmwareVersion version) override;

    void peripheralMidiChannelsReceived(int pitchWheelChannel, int modWheelChannel, int expressionChannel, int sustainPedalChannel) override;



private:
    //=========================================================================

    const StringArray ControlNames = 
    {
        "Pitch Wheel",
        "Mod Wheel",
        "Expression Pedal",
        "Sustain Pedal"
    };

    // TODO: Move these somewhere more common?

    typedef enum
    {
        PitchWheel = 0,
        ModWheel,
        Expression,
        Sustain
    } PeripheralChannel;

    struct PeripheralChannelSettings
    {
        int pitchWheel = 1;
        int modWheel = 1;
        int expressionPedal = 1;
        int sustainPedal = 1;

        void setChannel(PeripheralChannel controlId, int channelIn)
        {
            if (channelIn < 1)
                channelIn = 1;

            if (channelIn > 16)
                channelIn = 17; // Set to Omni

            switch (controlId)
            {
            case PeripheralChannel::PitchWheel:
                pitchWheel = channelIn;
                break;

            case PeripheralChannel::ModWheel:
                modWheel = channelIn;
                break;

            case PeripheralChannel::Expression:
                expressionPedal = channelIn;
                break;

            case PeripheralChannel::Sustain:
                sustainPedal = channelIn;
                break;
            }
        }

        int getChannel(PeripheralChannel controlId)
        {
            switch (controlId)
            {
            case PeripheralChannel::PitchWheel:
                return pitchWheel;

            case PeripheralChannel::ModWheel:
                return modWheel;

            case PeripheralChannel::Expression:
                return expressionPedal;

            case PeripheralChannel::Sustain:
                return sustainPedal;
            }

            return 0;
        }
    };

private:

    std::unique_ptr<Label>      setMidiChannelHeader;

    OwnedArray<Slider>          setMidiChannelSliders;
    OwnedArray<Label>           setMidiChannelLabels;
    OwnedArray<Button>          setOmniChannelButtons;
    
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