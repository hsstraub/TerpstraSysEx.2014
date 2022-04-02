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
    // Window aspect ratio
    constrainer->setFixedAspectRatio(DEFAULTMAINWINDOWASPECT);
    constrainer->setMinimumSize(800, round(800 / DEFAULTMAINWINDOWASPECT));

    setConstrainer(constrainer);
    updateBounds();
    startTimer(2000);
#endif

    setLookAndFeel(&TerpstraSysExApplication::getApp().getLookAndFeel());
}

MainWindow::~MainWindow()
{
    stopTimer();
    setConstrainer(nullptr);
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
    if (maxWindowHeight > 0)
        return getHeight() > maxWindowHeight;
    
    return false; // shouldn't happen
}

bool MainWindow::isOutOfVerticalBounds() const
{
    return getScreenY() < 0 
        || getScreenY() >= (maxWindowHeight - verticalBoundsThreshold);
}

bool MainWindow::isOutOfHorizontalBounds() const
{
    return getScreenBounds().getRight() < horizontalBoundsThreshold 
        || getScreenX() >= (maxWindowWidth - horizontalBoundsThreshold);
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

void MainWindow::updateBounds()
{
    auto thisDisplay = Desktop::getInstance().getDisplays().getDisplayForRect(getScreenBounds());
    
    if (thisDisplay != nullptr)
    {
        int screenWidth = thisDisplay->userArea.getWidth();
        int screenHeight = thisDisplay->userArea.getHeight();
        if (screenWidth != maxWindowWidth || screenHeight != maxWindowHeight)
        {
            maxWindowWidth = screenWidth;
            maxWindowHeight = screenHeight;
            constrainer->setMaximumHeight(maxWindowHeight);
            
            fixWindowPositionAndSize();
            return;
        }
    }

    if (isOutOfVerticalBounds())
        fixWindowPositionAndSize();
}

void MainWindow::fixWindowPositionAndSize(bool setToDefault)
{
    if (setToDefault || isLargerThanCurrentScreen())
    {
        // Default window state
        setSize(DEFAULTMAINWINDOWWIDTH, DEFAULTMAINWINDOWHEIGHT);
        return;
    }

    if (isOutOfVerticalBounds())
    {
        int correctedY = (getScreenY() < 0) ? 0
                                            : maxWindowHeight - verticalBoundsThreshold;
        setTopLeftPosition(getScreenX(), correctedY);
    }
    
    if (isOutOfHorizontalBounds())
    {
        auto bounds = getScreenBounds();
        int correctedX = (bounds.getX() < 0) ? horizontalBoundsThreshold - maxWindowWidth
                                             : maxWindowWidth - horizontalBoundsThreshold;
        setTopLeftPosition(correctedX, getScreenY());
    }
}

void MainWindow::timerCallback()
{
    // Set threshold to be a quarter of the window handle height
    verticalBoundsThreshold = round(getTitleBarHeight() * 0.25f); 

    updateBounds();
}
