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
            firmwareTransfer.reset(new FirmwareTransfer(TerpstraSysExApplication::getApp().getMidiDriver()));
            firmwareUpdateInProgress = true;
            firmwareTransfer->addListener(this);
            firmwareTransfer->requestFirmwareUpdate(firmwareFileSelected.getFullPathName());
        }
        else
        {
            infoBox->setText(infoBox->getText() + "\nError: Not a valid firmware file...");
        }
    }
}

void FirmwareDlg::fileChanged(PathBrowserComponent* source, File newFile)
{
    firmwareFileSelected = newFile;
}

void FirmwareDlg::firmwareTransferUpdate(FirmwareTransfer::StatusCode statusCode)
{
    const MessageManagerLock mml;

    String postMsg;

    switch (statusCode)
    {
    case FirmwareTransfer::StatusCode::Initialize:
        //postMsg = translate("\nFirmware update process initiated!");
        // This might be too verbose
        break;
    
    case FirmwareTransfer::StatusCode::FileIntegrityCheck:
        postMsg = translate("\nChecking integrity of firmware file...");
        break;

    case FirmwareTransfer::StatusCode::SessionBegin:
        postMsg = translate("\nEstablishing connection to Lumatone...");
        break;

    case FirmwareTransfer::StatusCode::AuthBegin:
        postMsg = translate("\nProviding credentials...");
        break;

    case FirmwareTransfer::StatusCode::TransferBegin:
        postMsg = translate("\nTransferring firmware file...");
        break;

    case FirmwareTransfer::StatusCode::InstallBegin:
        postMsg = translate("\nRebooting device for installation...");
        break;

    case FirmwareTransfer::StatusCode::VerificationBegin:
        postMsg = translate("\nVerifying installation was successful...");
        break;

    case FirmwareTransfer::StatusCode::IntegrityErr:
        postMsg = translate("\nError: Not a valid Lumatone firmware file");
        break;

    case FirmwareTransfer::StatusCode::StartupErr:
        postMsg = translate("\nError: Could not prepare device communication protool");
        break;

    case FirmwareTransfer::StatusCode::HostConnectErr:
        postMsg = translate("\nError: Could not communicate with Lumatone");
        break;

    case FirmwareTransfer::StatusCode::SessionEstErr:
        postMsg = translate("\nError: Could not verify connection with Lumatone");
        break;

    case FirmwareTransfer::StatusCode::AuthErr:
        postMsg = translate("\nError: Authentication for Lumatone failed");
        break;

    case FirmwareTransfer::StatusCode::ChannelErr:
        postMsg = translate("\nError: Communication channel failure");
        break;

    case FirmwareTransfer::StatusCode::ExecChnlErr:
        postMsg = translate("\nError: Could not request device reboot. Try manually rebooting your Lumatone.");
        break;

    default: // NoErr
        postMsg = translate("\nEverything looks good, firmware update complete!");
    }

    if ((int)statusCode <= 0)
    {
        firmwareUpdateInProgress = false;
    }

    infoBox->setText(infoBox->getText() + postMsg);
}

void FirmwareDlg::exitSignalSent()
{
    firmwareTransfer->waitForThreadToExit(20);
    firmwareTransfer = nullptr;
}
