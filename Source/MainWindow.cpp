/*
  ==============================================================================

    MainWindow.cpp
    Created: 10 Feb 2024 9:23:42pm
    Author:  straub

  ==============================================================================
*/

#include "MainWindow.h"
#include "Main.h"

MainWindow::MainWindow() : DocumentWindow("Lumatone Keyboard Setup Utility",
	Colour(TerpstraSysExApplication::getApp().getLookAndFeel().findColour(DocumentWindow::backgroundColourId)),
	DocumentWindow::allButtons)
{
	setContentOwned(new MainContentComponent(), true);

	setResizable(true, false);
	centreWithSize(getWidth(), getHeight());
#if JUCE_ANDROID
	setFullScreen(true);
#endif
	setLookAndFeel(&TerpstraSysExApplication::getApp().getLookAndFeel());
	setVisible(true);
}

void MainWindow::closeButtonPressed()
{
	// This is called when the user tries to close this window. Here, we'll just
	// ask the app to quit when this happens, but you can change this to do
	// whatever you need.
	JUCEApplication::getInstance()->systemRequestedQuit();
}