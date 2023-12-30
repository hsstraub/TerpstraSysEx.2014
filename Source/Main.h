/*
  ==============================================================================

    Main.h
    Created: 26 Jul 2015 8:39:46pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "LumatoneMenu.h"
#include "MainWindow.h"
#include "KeyboardDataStructure.h"
#include "ViewConstants.h"
#include "TerpstraMidiDriver.h"


//==============================================================================
class TerpstraSysExApplication : public JUCEApplication
{
public:
	//==============================================================================
	TerpstraSysExApplication();

	const String getApplicationName() override		{ return ProjectInfo::projectName; }
	const String getApplicationVersion() override   { return ProjectInfo::versionString; }
	bool moreThanOneInstanceAllowed() override      { return true; }

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
	RecentlyOpenedFilesList& getRecentFileList() { return recentFiles; }
	TerpstraMidiDriver& getMidiDriver() { return midiDriver; }
	Lumatone::Menu::MainMenuModel* getMainMenu() { return menuModel.get(); }

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

	bool applyLightColourScheme(bool repaintAndSave);
	bool applyDarkColourScheme(bool repaintAndSave);

	bool generalOptionsDialog();
	bool noteOnOffVelocityCurveDialog();
	bool faderVelocityCurveDialog();
	bool aftertouchVelocityCurveDialog();

	bool openRecentFile(int recentFileIndex);
	bool openFromCurrentFile();
	bool saveCurrentFile();

	void sendCurrentMappingToDevice();

	void updateMainTitle();
	void setHasChangesToSave(bool value);

	bool aboutTerpstraSysEx();

private:
	std::unique_ptr<MainWindow> mainWindow;
	std::unique_ptr<ApplicationCommandManager> commandManager;
	std::unique_ptr<Lumatone::Menu::MainMenuModel> menuModel;
	TooltipWindow				tooltipWindow;
	bool						hasChangesToSave;
	LookAndFeel_V3				lookAndFeel;

	PropertiesFile*				propertiesFile;
	File						currentFile;
	RecentlyOpenedFilesList		recentFiles;

	// MIDI connection
	TerpstraMidiDriver			midiDriver;
};
