/*
  ==============================================================================

    FirmwareDlg.cpp
    Created: 5 Feb 2021 7:33:07pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "FirmwareDlg.h"
#include "../Main.h"

FirmwareDlg::FirmwareDlg()
{
    //checkUpdateBtn.reset(new TextButton("checkUpdateBtn"));
    //checkUpdateBtn->setButtonText("Check For Updates");
    //checkUpdateBtn->addListener(this);
    //addAndMakeVisible(checkUpdateBtn.get());

    auto properties = TerpstraSysExApplication::getApp().getPropertiesFile();
    File lastFirmwareLocation = properties->getValue("LastFirmwareBinPath", properties->getValue("UserDocumentsLocation", File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getFullPathName()));

    fileBrowser.reset(new PathBrowserComponent("Lumatone Firmware Update", lastFirmwareLocation));
    fileBrowser->getEditor()->setColour(TextEditor::ColourIds::backgroundColourId, TerpstraSysExApplication::getApp().getLookAndFeel().findColour(LumatoneEditorColourIDs::ControlBoxBackground));
    fileBrowser->getEditor()->setColour(TextEditor::ColourIds::textColourId, TerpstraSysExApplication::getApp().getLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    fileBrowser->getEditor()->getProperties().set(LumatoneEditorStyleIDs::connectedEdgeFlags, Button::ConnectedEdgeFlags::ConnectedOnRight);
    fileBrowser->addListener(this);
    addAndMakeVisible(fileBrowser.get());

    doUpdateBtn.reset(new TextButton("checkUpdateBtn"));
    doUpdateBtn->setButtonText(translate("Begin Update"));
    doUpdateBtn->addListener(this);
    addAndMakeVisible(doUpdateBtn.get());

    infoBox.reset(new TextEditor("FirmwareUpdateInfoBox"));
    infoBox->setMouseClickGrabsKeyboardFocus(false);
    infoBox->setReadOnly(true);
    infoBox->setMultiLine(true);
    infoBox->insertTextAtCaret(translate("Select a firmware file and then click \"Begin Update\""));
    infoBox->setColour(TextEditor::ColourIds::backgroundColourId, TerpstraSysExApplication::getApp().getLookAndFeel().findColour(LumatoneEditorColourIDs::ControlBoxBackground));
    infoBox->setColour(TextEditor::ColourIds::textColourId, TerpstraSysExApplication::getApp().getLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    infoBox->setColour(ScrollBar::ColourIds::thumbColourId, Colour(0xff2d3135));
    infoBox->getProperties().set(LumatoneEditorStyleIDs::connectedEdgeFlags, 15);
    addAndMakeVisible(infoBox.get());

    firmwareStatusLabel.reset(new Label("FirmwareVersionStatusLabel"));
    addAndMakeVisible(firmwareStatusLabel.get());
    updateFirmwareVersionLabel();

    TerpstraSysExApplication::getApp().getLumatoneController().addFirmwareListener(this);

    //if (!updateIsAvailable)
    //{
        //doUpdateBtn->setEnabled(false);
    //}
}

FirmwareDlg::~FirmwareDlg()
{
    TerpstraSysExApplication::getApp().getLumatoneController().removeFirmwareListener(this);
}

void FirmwareDlg::paint(Graphics& g)
{

}

void FirmwareDlg::resized()
{
    int margin = 12;
    int doubleMargin = margin * 2;
    int buttonWidth = proportionOfWidth(0.3f);
    int buttonHeight = 30;

    //checkUpdateBtn->setBounds(margin, margin, buttonWidth, buttonHeight);

    doUpdateBtn->setBounds(getWidth() - margin - buttonWidth, margin, buttonWidth, buttonHeight);
    fileBrowser->setBounds(margin, margin, doUpdateBtn->getX() - doubleMargin, buttonHeight);

    firmwareStatusLabel->setBounds(margin, fileBrowser->getBottom() + margin, getWidth() - doubleMargin, buttonHeight / 2);

    infoBox->setBounds(getLocalBounds().withTop(firmwareStatusLabel->getBottom()).reduced(margin));
    infoBox->applyFontToAllText(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::FranklinGothic));
}

void FirmwareDlg::buttonClicked(Button* btn)
{
    //if (btn == checkUpdateBtn.get())
    //{
    //    DBG("Version check requested");
    //}
    if (btn == doUpdateBtn.get())
    {
        if (TerpstraSysExApplication::getApp().getLumatoneController().getMidiInputIndex() < 0 || TerpstraSysExApplication::getApp().getLumatoneController().getMidiOutputIndex() < 0)
        {
            AlertWindow::showMessageBox(AlertWindow::AlertIconType::NoIcon, "Not connected", "Please connect the Lumatone via USB before performing a firmware update.", "Ok", this);
            return;
        }

        DBG("Update requested");

        if (firmwareFileSelected.existsAsFile())
        {
            TerpstraSysExApplication::getApp().getPropertiesFile()->setValue("LastFirmwareBinPath", firmwareFileSelected.getParentDirectory().getFullPathName());
            TerpstraSysExApplication::getApp().getLumatoneController().requestFirmwareUpdate(firmwareFileSelected, this);
        }
        else
        {
            infoBox->setText(infoBox->getText() + "Error: Not a valid firmware file...");
        }
    }
}

// TODO: use LumatoneFirmwareVersion
void FirmwareDlg::updateFirmwareVersionLabel()
{
    String firmwareVersion = TerpstraSysExApplication::getApp().getFirmwareVersionStr();
    if (firmwareVersion != "0.0.0")
    {
        firmwareStatusLabel->setText("Current firmware version is: " + firmwareVersion, dontSendNotification);
    }
    else
    {
        firmwareStatusLabel->setText("Unable to determine firmware version.", dontSendNotification);
    }
}

void FirmwareDlg::fileChanged(PathBrowserComponent* source, File newFile)
{
    firmwareFileSelected = newFile;
}

void FirmwareDlg::firmwareTransferUpdate(FirmwareTransfer::StatusCode statusCode, String msg)
{
    if ((int)statusCode <= 0)
    {
        firmwareUpdateInProgress = false;
    }

    if (msg!= "")
    {
        msgLog += (msg + "\n");
        infoNeedsUpdate = true;
        startTimer(infoUpdateTimeoutMs);
    }
}

void FirmwareDlg::timerCallback()
{
    if (infoNeedsUpdate)
    {
        infoBox->setText(msgLog);
        infoNeedsUpdate = false;
    }
}

void FirmwareDlg::firmwareRevisionReceived(int major, int minor, int revision)
{
     updateFirmwareVersionLabel();
}
