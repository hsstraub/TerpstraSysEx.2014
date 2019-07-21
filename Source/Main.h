/*
  ==============================================================================

    Main.h
    Created: 26 Jul 2015 8:39:46pm
    Author:  hsstraub

  ==============================================================================
*/

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainMenu.h"
#include "MainComponent.h"
#include "KeyboardDataStructure.h"
#include "ViewConstants.h"
#include "TerpstraMidiDriver.h"


//==============================================================================
class TerpstraSysExApplication : public JUCEApplication,
    public MidiInputCallback
{
public:
	//==============================================================================
	TerpstraSysExApplication();

	const String getApplicationName()       { return ProjectInfo::projectName; }
	const String getApplicationVersion()    { return ProjectInfo::versionString; }
	bool moreThanOneInstanceAllowed()       { return true; }

	void initialise(const String& commandLine);
	void shutdown();
	void systemRequestedQuit();
	void anotherInstanceStarted(const String& commandLine);

	static TerpstraSysExApplication& getApp()
	{
		TerpstraSysExApplication* const app = dynamic_cast<TerpstraSysExApplication*> (JUCEApplication::getInstance());
		jassert(app != nullptr);
		return *app;
	}

	PropertiesFile* getPropertiesFile() { return propertiesFile; }
	RecentlyOpenedFilesList& getRecentFileList() { return recentFiles; }
	TerpstraMidiDriver& getMidiDriver() { return midiDriver; }
	TerpstraSysExMainMenuModel* getMainMenu() { return menuModel; }

	// Menu functionality
	void getAllCommands(Array <CommandID>& commands) override;
	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
	bool perform(const InvocationInfo& info) override;

	bool openSysExMapping();
	bool saveSysExMapping();
	bool saveSysExMappingAs();
	bool resetSysExMapping();

	bool deleteSubBoardData();
	bool copySubBoardData();
	bool pasteSubBoardData();

	bool generalOptionsDialog();
	bool noteOnOffVelocityCurveDialog();
	bool faderVelocityCurveDialog();

	bool openRecentFile(int recentFileIndex);
	bool openFromCurrentFile();
	bool saveCurrentFile();

	void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override;

	void updateMainTitle();
	void setHasChangesToSave(bool value);

	bool aboutTerpstraSysEx();

	//==============================================================================
	/*
	This class implements the desktop window that contains an instance of
	our MainContentComponent class.
	*/
	class MainWindow : public DocumentWindow
	{
	public:
		MainWindow() : DocumentWindow("Terpstra Keyboard SysEx Utility",
			Colour(MAINWINDOWBGCOLOUR),
			DocumentWindow::allButtons)
		{
			setContentOwned(new MainContentComponent(), true);

			setResizable(true, false);
			centreWithSize(getWidth(), getHeight());
#if JUCE_ANDROID
			setFullScreen(true);
#endif
			setVisible(true);
		}

		void closeButtonPressed()
		{
			// This is called when the user tries to close this window. Here, we'll just
			// ask the app to quit when this happens, but you can change this to do
			// whatever you need.
			JUCEApplication::getInstance()->systemRequestedQuit();
		}

		/* Note: Be careful if you override any DocumentWindow methods - the base
		class uses a lot of them, so by overriding you might break its functionality.
		It's best to do all your work in your content component instead, but if
		you really have to override any DocumentWindow methods, make sure your
		subclass also calls the superclass's method.
		*/

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
	};

private:
	ScopedPointer<MainWindow> mainWindow;
	ScopedPointer<ApplicationCommandManager> commandManager;
	ScopedPointer<TerpstraSysExMainMenuModel> menuModel;
	TooltipWindow				tooltipWindow;
	bool						hasChangesToSave;

	PropertiesFile*				propertiesFile;
	File						currentFile;
	RecentlyOpenedFilesList		recentFiles;

	// MIDI connection
	TerpstraMidiDriver			midiDriver;
};


#endif  // MAIN_H_INCLUDED
