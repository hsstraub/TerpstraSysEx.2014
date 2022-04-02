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
class MainWindow : public DocumentWindow
{
public:
	MainWindow(ComponentBoundsConstrainer* constrainerIn);

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
	bool isHandleOutOfBounds() const;

	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);

	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);

	void updateBoundsConstrainer();

	void fixWindowPositionAndSize(bool setToDefault=false);

private:

	ComponentBoundsConstrainer* constrainer;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};
