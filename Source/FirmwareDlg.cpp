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
    checkUpdateBtn.reset(new TextButton("checkUpdateBtn"));
    checkUpdateBtn->setButtonText("Check For Updates");
    checkUpdateBtn->addListener(this);
    addAndMakeVisible(checkUpdateBtn.get());

    doUpdateBtn.reset(new TextButton("checkUpdateBtn"));
    doUpdateBtn->setButtonText("Update Firmware");
    doUpdateBtn->addListener(this);
    addAndMakeVisible(doUpdateBtn.get());

    //updateIsAvailable = TerpstraSysExApplication::getApp().isFirmwareUpdateAvailable();

    if (!updateIsAvailable)
    {
        doUpdateBtn->setEnabled(false);
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

    checkUpdateBtn->setBounds(margin, margin, buttonWidth, buttonHeight);
    doUpdateBtn->setBounds(margin, checkUpdateBtn->getBottom() + margin, buttonWidth, buttonHeight);
}

void FirmwareDlg::buttonClicked(Button* btn)
{
    if (btn == checkUpdateBtn.get())
    {
        DBG("Version check requested");
        FirmwareTransfer firmTrans(TerpstraSysExApplication::getApp().getMidiDriver());
        firmTrans.requestFirmwareDownloadAndUpdate();
    }
    else if (btn == doUpdateBtn.get())
    {
        DBG("Update requested");
    }
}