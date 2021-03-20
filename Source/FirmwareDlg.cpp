/*
  ==============================================================================

    FirmwareDlg.cpp
    Created: 5 Feb 2021 7:33:07pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "FirmwareDlg.h"
#include "Main.h"

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

    //updateIsAvailable = TerpstraSysExApplication::getApp().isFirmwareUpdateAvailable();

    if (!updateIsAvailable)
    {
        //doUpdateBtn->setEnabled(false);
    }
}

FirmwareDlg::~FirmwareDlg()
{

}

void FirmwareDlg::paint(Graphics& g)
{

}

void FirmwareDlg::resized()
{
    int margin = 12;
    int buttonWidth = proportionOfWidth(0.3f);
    int buttonHeight = 30;

    //checkUpdateBtn->setBounds(margin, margin, buttonWidth, buttonHeight);

    doUpdateBtn->setBounds(getWidth() - margin - buttonWidth, margin, buttonWidth, buttonHeight);
    fileBrowser->setBounds(margin, margin, doUpdateBtn->getX() - margin * 2, buttonHeight);

    infoBox->setBounds(getLocalBounds().withTop(doUpdateBtn->getBottom()).reduced(margin));
    infoBox->applyFontToAllText(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::FranklinGothic));
}

void FirmwareDlg::lookAndFeelChanged()
{
    auto lookAndFeel = dynamic_cast<LumatoneEditorLookAndFeel*>(&getLookAndFeel());
    if (lookAndFeel)
    {
        lookAndFeel->setupTextButton(*doUpdateBtn);
        lookAndFeel->setupTextButton(*fileBrowser->getButton());
    }
}

void FirmwareDlg::buttonClicked(Button* btn)
{
    //if (btn == checkUpdateBtn.get())
    //{
    //    DBG("Version check requested");

    //}
    if (btn == doUpdateBtn.get())
    {
        DBG("Update requested");

        if (firmwareFileSelected.existsAsFile())
        {
            initializeFirmwareUpdate();
        }
        else
        {
            infoBox->setText(infoBox->getText() + "Error: Not a valid firmware file...");
        }
    }
}

void FirmwareDlg::initializeFirmwareUpdate()
{
    firmwareTransfer.reset(new FirmwareTransfer(TerpstraSysExApplication::getApp().getMidiDriver()));
    firmwareUpdateInProgress = true;
    firmwareTransfer->addListener(this);
    firmwareTransfer->requestFirmwareUpdate(firmwareFileSelected.getFullPathName());
    startTimer(infoUpdateTimeoutMs);
}

void FirmwareDlg::fileChanged(PathBrowserComponent* source, File newFile)
{
    firmwareFileSelected = newFile;
}

void FirmwareDlg::firmwareTransferUpdate(FirmwareTransfer::StatusCode statusCode)
{
    String postMsg;

    switch (statusCode)
    {
    case FirmwareTransfer::StatusCode::Initialize:
        postMsg = translate("Firmware update process initiated!");
        break;
    
    case FirmwareTransfer::StatusCode::FileIntegrityCheck:
        postMsg = translate("Checking integrity of firmware file...");
        break;

    case FirmwareTransfer::StatusCode::SessionBegin:
        postMsg = translate("Establishing connection to Lumatone...");
        break;

    case FirmwareTransfer::StatusCode::AuthBegin:
        postMsg = translate("Providing credentials...");
        break;

    case FirmwareTransfer::StatusCode::TransferBegin:
        postMsg = translate("Transferring firmware file...");
        break;

    case FirmwareTransfer::StatusCode::InstallBegin:
        postMsg = translate("Rebooting device for installation...");
        break;

    case FirmwareTransfer::StatusCode::VerificationBegin:
        postMsg = translate("Verifying installation was successful...");
        break;

    case FirmwareTransfer::StatusCode::IntegrityErr:
        postMsg = translate("Error: Not a valid Lumatone firmware file");
        break;

    case FirmwareTransfer::StatusCode::StartupErr:
        postMsg = translate("Error: Could not prepare device communication protool");
        break;

    case FirmwareTransfer::StatusCode::HostConnectErr:
        postMsg = translate("Error: Could not communicate with Lumatone");
        break;

    case FirmwareTransfer::StatusCode::SessionEstErr:
        postMsg = translate("Error: Could not verify connection with Lumatone");
        break;

    case FirmwareTransfer::StatusCode::AuthErr:
        postMsg = translate("Error: Authentication for Lumatone failed");
        break;

    case FirmwareTransfer::StatusCode::ChannelErr:
        postMsg = translate("Error: Communication channel failure");
        break;

    case FirmwareTransfer::StatusCode::ExecChnlErr:
        postMsg = translate("Error: Could not request device reboot. Try rebooting your Lumatone.");
        break;

    default: // NoErr
        postMsg = translate("Please wait for your Lumatone to install the firmware.");
    }

    if ((int)statusCode <= 0)
    {
        firmwareUpdateInProgress = false;
    }

    msgLog += (postMsg + "\n");
    infoNeedsUpdate = true;
}

void FirmwareDlg::timerCallback()
{
    if (infoNeedsUpdate)
    {
        infoBox->setText(msgLog);
        infoNeedsUpdate = false;
    }
}

void FirmwareDlg::exitSignalSent()
{
    firmwareTransfer->waitForThreadToExit(20);
    firmwareTransfer = nullptr;
    stopTimer();
}
