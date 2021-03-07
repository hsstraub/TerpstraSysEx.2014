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
            FirmwareTransfer firmTrans(TerpstraSysExApplication::getApp().getMidiDriver());
            firmTrans.initializeFirmwareUpdate(firmwareFileSelected.getFullPathName());
        }
        else
        {
            infoBox->insertTextAtCaret("\nError: Not a valid firmware file...");
        }
    }
}

void FirmwareDlg::fileChanged(PathBrowserComponent* source, File newFile)
{
    firmwareFileSelected = newFile;
    // TODO: valid firmware file detection
}
