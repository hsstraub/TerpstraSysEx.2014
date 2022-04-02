/*
  ==============================================================================

    MainWindow.cpp
    Created: 2 Apr 2022 3:24:02pm

  ==============================================================================
*/

#include "MainWindow.h"
#include "Main.h"

MainWindow::MainWindow(ComponentBoundsConstrainer* constrainerIn) : DocumentWindow("Lumatone Editor",
	TerpstraSysExApplication::getApp().getLookAndFeel().findColour(LumatoneEditorColourIDs::MediumBackground),
	DocumentWindow::minimiseButton + DocumentWindow::closeButton),
	constrainer(constrainerIn)
{
	setContentOwned(new MainContentComponent(), true);

	setResizable(true, true);
#if JUCE_ANDROID
	setFullScreen(true);
#else
	setConstrainer(constrainer);
	updateBoundsConstrainer();
#endif

	setLookAndFeel(&TerpstraSysExApplication::getApp().getLookAndFeel());
}

void MainWindow::closeButtonPressed()
{
	// This is called when the user tries to close this window. Here, we'll just
	// ask the app to quit when this happens, but you can change this to do
	// whatever you need.
	JUCEApplication::getInstance()->systemRequestedQuit();
}

BorderSize<int> MainWindow::getBorderThickness()
{
	return BorderSize <int>(1);
}

bool MainWindow::isLargerThanCurrentScreen() const
{
	int maxWindowHeight = Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea.getHeight();
	return getHeight() > maxWindowHeight;
}

bool MainWindow::isHandleOutOfBounds() const
{
	return getScreenY() < 0;
}

void MainWindow::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	// Save state of main window
	propertiesFile->setValue("MainWindowState", getWindowStateAsString());
	((MainContentComponent*)(getContentComponent()))->saveStateToPropertiesFile(propertiesFile);
}

void MainWindow::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	bool useSavedState = restoreWindowStateFromString(propertiesFile->getValue("MainWindowState"));

	fixWindowPositionAndSize(!useSavedState);

	setVisible(true);

	((MainContentComponent*)(getContentComponent()))->restoreStateFromPropertiesFile(propertiesFile);
}

void MainWindow::updateBoundsConstrainer()
{
	// Window aspect ratio
	constrainer->setFixedAspectRatio(DEFAULTMAINWINDOWASPECT);
	constrainer->setMinimumSize(800, round(800 / DEFAULTMAINWINDOWASPECT));

	// Don't allow to resize more than current screen height
	int maxWindowHeight = Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea.getHeight();
	constrainer->setMaximumHeight(maxWindowHeight);
}

void MainWindow::fixWindowPositionAndSize(bool setToDefault)
{
	if (setToDefault || isLargerThanCurrentScreen())
	{
		// Default window state
		setSize(DEFAULTMAINWINDOWWIDTH, DEFAULTMAINWINDOWHEIGHT);
	}
	else if (isHandleOutOfBounds())
	{
		setTopLeftPosition(getScreenX(), 0);
	}
}