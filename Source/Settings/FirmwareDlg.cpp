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

    //updateIsAvailable = TerpstraSysExApplication::getApp().isFirmwareUpdateAvailable();

    if (!updateIsAvailable)
    {
        //doUpdateBtn->setEnabled(false);
    }
}

FirmwareDlg::~FirmwareDlg()
{
    // Should check "who" paused it in the future
    //TerpstraSysExApplication::getApp().getDeviceMonitor().setPaused(false);
    //TerpstraSysExApplication::getApp().getMidiDriver().removeListener(this);
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
        if (TerpstraSysExApplication::getApp().getLumatoneController().getMidiInputIndex() < 0 || TerpstraSysExApplication::getApp().getLumatoneController().getMidiOutputIndex() < 0)
        {
            AlertWindow::showMessageBox(AlertWindow::AlertIconType::NoIcon, "Not connected", "Please connect the Lumatone via USB before performing a firmware update.", "Ok", this);
            return;
        }

        DBG("Update requested");

        if (firmwareFileSelected.existsAsFile())
        {
            TerpstraSysExApplication::getApp().getPropertiesFile()->setValue("LastFirmwareBinPath", firmwareFileSelected.getParentDirectory().getFullPathName());
            initializeFirmwareUpdate();
        }
        else
        {
            infoBox->setText(infoBox->getText() + "Error: Not a valid firmware file...");
        }
    }
}

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

void FirmwareDlg::initializeFirmwareUpdate()
{
    //firmwareTransfer.reset(new FirmwareTransfer(TerpstraSysExApplication::getApp().getMidiDriver()));
    //firmwareUpdateInProgress = true;
    //numberOfWaitIncrements = 0;
    //firmwareTransfer->addListener(this);
    //firmwareTransfer->requestFirmwareUpdate(firmwareFileSelected.getFullPathName());
    //startTimer(infoUpdateTimeoutMs);
}

void FirmwareDlg::initializeWaitForUpdate()
{
    waitingForUpdateConfirmation = true;
    //TerpstraSysExApplication::getApp().getDeviceMonitor().initializeFirmwareUpdateMode();
    //TerpstraSysExApplication::getApp().getDeviceMonitor().setPaused(false);
    //TerpstraSysExApplication::getApp().getMidiDriver().addListener(this);
    startTimer(updateIncrementTimeoutMs);
}

double FirmwareDlg::numIncrementsToProgress(int numberOfIncrements)
{
    return numberOfWaitIncrements / (double)maxUpdateIncrements;
}

void FirmwareDlg::fileChanged(PathBrowserComponent* source, File newFile)
{
    firmwareFileSelected = newFile;
}

void FirmwareDlg::firmwareTransferUpdate(FirmwareTransfer::StatusCode statusCode)
{
    String postMsg;

    // TODO: Revise with progress bar
    // Probably should all be handled by FirmwareTransfer
    switch (statusCode)
    {
    case FirmwareTransfer::StatusCode::Initialize:
        //postMsg = translate("Firmware update process initiated!");
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
        postMsg = translate("Rebooting device for installation");
        break;

    case FirmwareTransfer::StatusCode::VerificationBegin:
        postMsg = translate("Installing update...");
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

    case FirmwareTransfer::StatusCode::NoErr:
        postMsg = translate("Please wait for your Lumatone to install the firmware.");
        break;
    }

    if (statusCode == FirmwareTransfer::StatusCode::InstallBegin)
    {
        //TerpstraSysExApplication::getApp().getDeviceMonitor().setPaused(true);
        //TerpstraSysExApplication::getApp().getMidiDriver().closeMidiInput();
        //TerpstraSysExApplication::getApp().getMidiDriver().closeMidiOutput();
    }
    if (statusCode == FirmwareTransfer::StatusCode::VerificationBegin)
    {
        initializeWaitForUpdate();
    }

    if ((int)statusCode <= 0)
    {
        firmwareUpdateInProgress = false;
    }

    numberOfWaitIncrements += (int)(maxUpdateIncrements * FirmwareTransfer::statusCodeToProgressPercent(1) * 0.4);

    firmwareTransfer->setStatusMessage(postMsg);
    firmwareTransfer->setProgress(numIncrementsToProgress(numberOfWaitIncrements));

    if (postMsg != "")
    {
        msgLog += (postMsg + "\n");
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

        if (!waitingForUpdateConfirmation)
        {
            stopTimer();
            return;
        }
    }
    if (waitingForUpdateConfirmation)
    {
        numberOfWaitIncrements++;
        firmwareTransfer->setProgress(numIncrementsToProgress(numberOfWaitIncrements));

        if (numberOfWaitIncrements >= maxUpdateIncrements)
        {
            DBG("TIMEOUT EXCEEDED BEFORE GETTING FIRMWARE CONFIRMATION");

            stopTimer();
            firmwareTransfer->signalThreadShouldExit();
            //TerpstraSysExApplication::getApp().getDeviceMonitor().cancelFirmwareUpdateMode();
            //TerpstraSysExApplication::getApp().getMidiDriver().removeListener(this);

            AlertWindow::showMessageBox(
                AlertWindow::AlertIconType::WarningIcon,
                "Firmware update not confirmed",
                "Your Lumatone appears to still be updating, or may have been disconnected. Make sure Lumatone is powered on and connected, and the when Lumatone is ready it will connect successfully.",
                "Ok", this);
        }

        startTimer(updateIncrementTimeoutMs);
    }
}

void FirmwareDlg::exitSignalSent()
{
    firmwareTransfer->waitForThreadToExit(20);
    firmwareTransfer = nullptr;
    stopTimer();
}

void FirmwareDlg::firmwareRevisionReceived(int major, int minor, int revision)
{
    auto version = FirmwareVersion(major, minor, revision);
    if (version.isValid())
    {
        DBG("Confirmed update to firmware version " + version.toString());

        stopTimer();
        firmwareTransfer->setProgress(1.0);
        firmwareTransfer->signalThreadShouldExit();
        updateFirmwareVersionLabel();
    }
}
