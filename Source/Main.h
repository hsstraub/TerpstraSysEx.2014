/*
  ==============================================================================

    Main.h
    Created: 26 Jul 2015 8:39:46pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "KeyboardDataStructure.h"
#include "ViewConstants.h"
#include "TerpstraMidiDriver.h"
#include "LumatoneEditorFonts.h"
#include "ColourPaletteDataStructure.h"
#include "LocalisationMap.h"


//==============================================================================
class TerpstraSysExApplication : public JUCEApplication
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
	LookAndFeel& getLookAndFeel() { return lookAndFeel; }
	ComponentBoundsConstrainer* getBoundsConstrainer() { return boundsConstrainer.get(); };
	RecentlyOpenedFilesList& getRecentFileList() { return recentFiles; }
	TerpstraMidiDriver& getMidiDriver() { return midiDriver; }
	Array<LumatoneEditorColourPalette>& getColourPalettes() { return colourPalettes; }
	Font getAppFont(LumatoneEditorFont fontIdIn, float height = 12.0f) { return appFonts.getFont(fontIdIn, height); }
	int getOctaveBoardSize() const { return octaveBoardSize; }

	// Menu functionality
	ApplicationCommandManager* getCommandManager() { return commandManager.get(); }
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
	bool aftertouchVelocityCurveDialog();

	bool openRecentFile(int recentFileIndex);
	bool openFromCurrentFile();
	bool saveCurrentFile();

	void sendCurrentConfigurationToDevice();
	void requestConfigurationFromDevice();

	void updateMainTitle();

	bool getHasChangesToSave() const { return hasChangesToSave; }
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
		MainWindow() : DocumentWindow("Lumatone Editor", 
			TerpstraSysExApplication::getApp().getLookAndFeel().findColour(LumatoneEditorColourIDs::MediumBackground),
			DocumentWindow::minimiseButton + DocumentWindow::closeButton)
		{
			setContentOwned(new MainContentComponent(), true);

			setResizable(true, true);
			setConstrainer(TerpstraSysExApplication::getApp().getBoundsConstrainer());
			centreWithSize(getWidth(), getHeight());
#if JUCE_ANDROID
			setFullScreen(true);
#endif
			setLookAndFeel(&TerpstraSysExApplication::getApp().getLookAndFeel());
			setVisible(true);
		}

		void closeButtonPressed()
		{
			// This is called when the user tries to close this window. Here, we'll just
			// ask the app to quit when this happens, but you can change this to do
			// whatever you need.
			JUCEApplication::getInstance()->systemRequestedQuit();
		}

		BorderSize<int> getBorderThickness() override
		{
			return BorderSize <int>(1);
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
	std::unique_ptr<MainWindow> mainWindow;

	std::unique_ptr<ComponentBoundsConstrainer> boundsConstrainer;

	std::unique_ptr<ApplicationCommandManager> commandManager;
	TooltipWindow				tooltipWindow;
	bool						hasChangesToSave;
	
	LumatoneEditorFonts			appFonts;
	LumatoneEditorLookAndFeel	lookAndFeel;

	PropertiesFile*				propertiesFile;
	File						currentFile;
	RecentlyOpenedFilesList		recentFiles;

	Array<LumatoneEditorColourPalette> colourPalettes;

	// MIDI connection
	TerpstraMidiDriver			midiDriver;

	// Size of octaver board. Usually 56, but there are a few devices with55.
	int octaveBoardSize = 56;
};

