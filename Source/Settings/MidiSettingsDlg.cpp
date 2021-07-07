/*
  ==============================================================================

    MidiSettingsDlg.cpp
    Created: 29 Jun 2021 10:02:43pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "MidiSettingsDlg.h"
#include "../Main.h"

MidiSettingsDlg::MidiSettingsDlg()
{
    setMidiChannelHeader.reset(new Label("SetMidiChannelHeader", "Set Controller MIDI Channel"));
    setMidiChannelHeader->setJustificationType(Justification::centred);
    setMidiChannelHeader->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::UniviaPro));
    addAndMakeVisible(setMidiChannelHeader.get());

    controlLabelFont = TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::GothamNarrowMedium);

    for (auto controlName : ControlNames)
    {
        auto lbl = setMidiChannelLabels.add(new Label(controlName + " Label", controlName + ": "));
        lbl->setJustificationType(Justification::centredRight);
        lbl->setFont(controlLabelFont);
        addAndMakeVisible(*lbl);

        auto sld = setMidiChannelSliders.add(new Slider(Slider::SliderStyle::IncDecButtons, Slider::TextEntryBoxPosition::TextBoxLeft));
        sld->setRange(1, 16, 1);
        sld->setName(controlName);
        sld->setTooltip("Set MIDI Channel for " + controlName + " messages.");
        sld->addListener(this);
        addAndMakeVisible(*sld);

        auto btn = setOmniChannelButtons.add(new ToggleButton("All Channels"));
        btn->setName(controlName);
        btn->setTooltip("Send " + controlName + " messages on all MIDI channels.");
        btn->addListener(this);
        addAndMakeVisible(*btn);

        // Help with resizing
        if (controlName.length() > longestControlName.length())
            longestControlName = controlName;
    }

    flexBox.flexWrap = FlexBox::Wrap::noWrap;
    flexBox.flexDirection = FlexBox::Direction::column;
    flexBox.justifyContent = FlexBox::JustifyContent::flexStart;
    flexBox.alignContent = FlexBox::AlignContent::flexStart;

    TerpstraSysExApplication::getApp().getLumatoneController().addFirmwareListener(this);
    
    setSupportedControls(TerpstraSysExApplication::getApp().getFirmwareVersion());
}

MidiSettingsDlg::~MidiSettingsDlg()
{
    // Not good when app closes with this window open...
    TerpstraSysExApplication::getApp().getLumatoneController().removeFirmwareListener(this);
    
    setMidiChannelHeader = nullptr;
    setMidiChannelLabels.clear();
    setMidiChannelSliders.clear();
    setOmniChannelButtons.clear();
}

void MidiSettingsDlg::paint(Graphics& g)
{

}

void MidiSettingsDlg::resized()
{
    int rowHeight = proportionOfHeight(fontHeightInBounds);

    auto setMidiChannelControlBounds = getLocalBounds().reduced(margin)
                                                        .withTrimmedTop(rowHeight + margin)
                                                        .withTrimmedLeft(margin * 1.5f); // This one is a hack

    controlLabelFont.setHeight(rowHeight);
    int labelWidth = controlLabelFont.getStringWidth(longestControlName);
    int sldWidth = labelWidth * 0.667f;
    int btnWidth = controlLabelFont.getStringWidth("All Channels");
    int btnHeight = rowHeight * 0.6f;

    flexRows.clear();
    flexBox.items.clear();

    auto controlMargin = FlexItem::Margin(0, margin * 0.5f, 0, 0);
    int toggleMargins = margin * 0.2f;

    for (int i = 0; i < ControlNames.size(); i++)
    {
        flexRows.add(FlexBox(
            FlexBox::Direction::row,
            FlexBox::Wrap::noWrap,
            FlexBox::AlignContent::center,
            FlexBox::AlignItems::center,
            FlexBox::JustifyContent::flexStart
        ));

        FlexBox& flexRow = flexRows.getReference(i);

        auto lbl = setMidiChannelLabels[i];
        auto lblItem = FlexItem(labelWidth, rowHeight, *lbl).withMargin(controlMargin);
        flexRow.items.add(lblItem);

        auto sld = setMidiChannelSliders[i];
        sld->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, false, sldWidth * 0.5f, rowHeight);
        auto sldItem = FlexItem(sldWidth, rowHeight, *sld).withMargin(controlMargin);
        flexRow.items.add(sldItem);

        auto btn = setOmniChannelButtons[i];
        auto btnItem = FlexItem(btnWidth, btnHeight, *btn).withMargin(FlexItem::Margin(toggleMargins, 0, toggleMargins, 0));
        
        flexRow.items.add(btnItem.withFlex(0.0f, 1.0f));

        auto rowItem = FlexItem(getWidth(), rowHeight);
        rowItem.associatedFlexBox = &flexRow; 
        rowItem.margin = FlexItem::Margin(0, 0, margin, 0);

        flexBox.items.add(rowItem);
    }

    flexBox.performLayout(setMidiChannelControlBounds);

    setMidiChannelHeader->setBounds(setMidiChannelControlBounds.withY(margin).withHeight(rowHeight));
}

void MidiSettingsDlg::sliderValueChanged(Slider* sld)
{
    int controlIndex = setMidiChannelSliders.indexOf(sld);

    if (controlIndex >= 0 && controlIndex < ControlNames.size())
    {
        channelSettings.setChannel((PeripheralChannel)controlIndex, sld->getValue());
        sendChannelSettings();
    }
}

void MidiSettingsDlg::buttonClicked(Button* btn)
{
    int controlIndex = setOmniChannelButtons.indexOf(static_cast<ToggleButton*>(btn));

    if (controlIndex >= 0 && controlIndex < ControlNames.size())
    {
        // TODO: make omni channel explicit
        channelSettings.setChannel((PeripheralChannel)controlIndex, 17);
        sendChannelSettings();

        // Disable slider if all channels is enabled
        setMidiChannelSliders[controlIndex]->setEnabled(!btn->getToggleState());
    }
}

void MidiSettingsDlg::setSupportedControls(FirmwareVersion version)
{
    bool setChannelsEnabled = false;

    FirmwareSupport support;
    if (support.versionAcknowledgesCommand(version, SET_PERIPHERAL_CHANNELS))
    {
        setChannelsEnabled = true;
        TerpstraSysExApplication::getApp().getLumatoneController().getPeripheralChannels();
    }

    for (int i = 0; i < ControlNames.size(); i++)
    {
        setMidiChannelSliders[i]->setEnabled(setChannelsEnabled);
        setOmniChannelButtons[i]->setEnabled(setChannelsEnabled);

        if (!setChannelsEnabled)
        {
            setMidiChannelSliders[i]->setTooltip(translate("This feature is not supported by your Lumatone's firmware version."));
            setOmniChannelButtons[i]->setTooltip(translate("This feature is not supported by your Lumatone's firmware version."));
        }
    }
}

void MidiSettingsDlg::updateChannelSettings(int pitchWheelChannel, int modWheelChannel, int expressionChannel, int sustainPedalChannel)
{
    channelSettings.setChannel(PeripheralChannel::PitchWheel, pitchWheelChannel);
    channelSettings.setChannel(PeripheralChannel::ModWheel, modWheelChannel);
    channelSettings.setChannel(PeripheralChannel::Expression, expressionChannel);
    channelSettings.setChannel(PeripheralChannel::Sustain, sustainPedalChannel);

    for (int i = 0; i < ControlNames.size(); i++)
    {
        if (channelSettings.getChannel((PeripheralChannel)i) > 16)
        {
            setMidiChannelSliders[i]->setEnabled(false);
            setOmniChannelButtons[i]->setToggleState(true, dontSendNotification);
        }
        else
        {
            setMidiChannelSliders[i]->setEnabled(true);
            setOmniChannelButtons[i]->setToggleState(false, dontSendNotification);
        }
    }
}

void MidiSettingsDlg::sendChannelSettings()
{
    TerpstraSysExApplication::getApp().getLumatoneController().setPeripheralChannels(
        channelSettings.pitchWheel,
        channelSettings.modWheel,
        channelSettings.expressionPedal,
        channelSettings.sustainPedal
    );
}

//=========================================================================
// LumatoneController::FirmwareListener implementation

void MidiSettingsDlg::firmwareRevisionReceived(FirmwareVersion version)
{
    setSupportedControls(version);
}

void MidiSettingsDlg::peripheralMidiChannelsReceived(int pitchWheelChannel, int modWheelChannel, int expressionChannel, int sustainPedalChannel)
{
    updateChannelSettings(pitchWheelChannel, modWheelChannel, expressionChannel, sustainPedalChannel);
}
