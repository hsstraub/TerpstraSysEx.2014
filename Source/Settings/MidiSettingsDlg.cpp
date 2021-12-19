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

        // Help with resizing
        if (controlName.length() > longestControlName.length())
            longestControlName = controlName;
    }

    flexBox.flexWrap = FlexBox::Wrap::noWrap;
    flexBox.flexDirection = FlexBox::Direction::column;
    flexBox.justifyContent = FlexBox::JustifyContent::flexStart;
    flexBox.alignContent = FlexBox::AlignContent::flexStart;

    TerpstraSysExApplication::getApp().getLumatoneController()->addFirmwareListener(this);

    setSupportedControls(TerpstraSysExApplication::getApp().getFirmwareVersion());
}

MidiSettingsDlg::~MidiSettingsDlg()
{
    // Not good when app closes with this window open...
    TerpstraSysExApplication::getApp().getLumatoneController()->removeFirmwareListener(this);
    
    setMidiChannelHeader = nullptr;
    setMidiChannelLabels.clear();
    setMidiChannelSliders.clear();
}

void MidiSettingsDlg::paint(Graphics& g)
{

}

void MidiSettingsDlg::resized()
{
    int rowHeight = proportionOfHeight(fontHeightInBounds);

    auto setMidiChannelControlBounds = getLocalBounds().withTrimmedTop(rowHeight + margin * 2);

    controlLabelFont.setHeight(rowHeight);
    int labelWidth = controlLabelFont.getStringWidth(longestControlName);
    int sldWidth = labelWidth * 0.667f;

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
            FlexBox::JustifyContent::center
        ));

        FlexBox& flexRow = flexRows.getReference(i);

        auto lbl = setMidiChannelLabels[i];
        auto lblItem = FlexItem(labelWidth, rowHeight, *lbl).withMargin(controlMargin);
        flexRow.items.add(lblItem);

        auto sld = setMidiChannelSliders[i];
        sld->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, false, sldWidth * 0.5f, rowHeight);
        auto sldItem = FlexItem(labelWidth, rowHeight, *sld).withMargin(controlMargin);
        flexRow.items.add(sldItem);

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

void MidiSettingsDlg::setSupportedControls(FirmwareVersion version)
{
    bool setChannelsEnabled = false;

    FirmwareSupport support;
    if (support.versionAcknowledgesCommand(version, SET_PERIPHERAL_CHANNELS))
    {
        setChannelsEnabled = true;
        TerpstraSysExApplication::getApp().getLumatoneController()->getPeripheralChannels();
    }

    for (int i = 0; i < ControlNames.size(); i++)
    {
        setMidiChannelSliders[i]->setEnabled(setChannelsEnabled);

        if (!setChannelsEnabled)
        {
            setMidiChannelSliders[i]->setTooltip(translate("This feature is not supported by your Lumatone's firmware version."));
        }
    }
}

void MidiSettingsDlg::updateChannelSettings(PeripheralChannelSettings channelSettingsIn)
{
    channelSettings = channelSettingsIn;

    for (int i = 0; i < ControlNames.size(); i++)
    {
        auto channel = channelSettings.getChannel((PeripheralChannel)i);
        setMidiChannelSliders[i]->setValue(channel, dontSendNotification);
    }
}

void MidiSettingsDlg::sendChannelSettings()
{
    TerpstraSysExApplication::getApp().getLumatoneController()->setPeripheralChannels(channelSettings);
}

//=========================================================================
// LumatoneController::FirmwareListener implementation

void MidiSettingsDlg::firmwareRevisionReceived(FirmwareVersion version)
{
    setSupportedControls(version);
}

void MidiSettingsDlg::peripheralMidiChannelsReceived(PeripheralChannelSettings channelSettings)
{
    updateChannelSettings(channelSettings);
}
