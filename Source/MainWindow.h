/*
  ==============================================================================

    MainWindow.h
    Created: 30 Dec 2023 4:12:56pm
    Author:  straub

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
This class implements the desktop window that contains an instance of
our MainContentComponent class.
*/
class MainWindow : public DocumentWindow
{
public:
    MainWindow();

    void closeButtonPressed() override;

	/* Note: Be careful if you override any DocumentWindow methods - the base
	class uses a lot of them, so by overriding you might break its functionality.
	It's best to do all your work in your content component instead, but if
	you really have to override any DocumentWindow methods, make sure your
	subclass also calls the superclass's method.
	*/

    void saveStateToPropertiesFile(PropertiesFile* propertiesFile);
    void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};