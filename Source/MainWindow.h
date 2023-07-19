/*
  ==============================================================================

    MainWindow.h
    Created: 2 Apr 2022 3:24:02pm

  ==============================================================================
*/

#pragma once
#include "MainComponent.h"

//==============================================================================
/*
This class implements the desktop window that contains an instance of
our MainContentComponent class.
*/
class MainWindow : public DocumentWindow, public Timer
{
public:
    MainWindow(ComponentBoundsConstrainer* constrainerIn);

    virtual ~MainWindow();
    
    void closeButtonPressed() override;

    BorderSize<int> getBorderThickness() override;

    /* Note: Be careful if you override any DocumentWindow methods - the base
    class uses a lot of them, so by overriding you might break its functionality.
    It's best to do all your work in your content component instead, but if
    you really have to override any DocumentWindow methods, make sure your
    subclass also calls the superclass's method.
    */

    // Returns true if the height is larger than current screen
    bool isLargerThanCurrentScreen() const;

    // Returns true if the top of the window is out of the screen
    bool isOutOfVerticalBounds() const;

    // Returns true if left side of window is too far right, or if right side of window is too far left
    bool isOutOfHorizontalBounds() const;

    void saveStateToPropertiesFile(PropertiesFile* propertiesFile);

    void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
    
    // Checks size of current display and updates constraint
    void updateBounds();

    void fixWindowPositionAndSize(bool setToDefault=false);
    
    void timerCallback() override;

private:

    ComponentBoundsConstrainer* constrainer;
    
    int maxWindowWidth = 0;
    int maxWindowHeight = 0;

    int horizontalBoundsThreshold = 10;
    int verticalBoundsThreshold = 10;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};
