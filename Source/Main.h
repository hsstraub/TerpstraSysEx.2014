/*
  ==============================================================================

    Main.h
    Created: 26 Jul 2015 8:39:46pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LumatoneMenu.h"
#include "MainWindow.h"
#include "MainComponent.h"
#include "LumatoneController.h"
#include "ViewConstants.h"
#include "LocalisationMap.h"

#define CHOOSE_FILE_NOOP [](bool) -> void {}

//==============================================================================
class TerpstraSysExApplication : public JUCEApplication
{
public:
	//==============================================================================
	TerpstraSysExApplication();

	const String getApplicationName() override		{ return ProjectInfo::projectName; }
	const String getApplicationVersion() override   { return ProjectInfo::versionString; }
	bool moreThanOneInstanceAllowed() override      { return true; }

	void initialise(const String& commandLine) override;
	void shutdown() override;
	void systemRequestedQuit() override;
	void anotherInstanceStarted(const String& commandLine) override;

	static TerpstraSysExApplication& getApp()
	{
		TerpstraSysExApplication* const app = dynamic_cast<TerpstraSysExApplication*> (JUCEApplication::getInstance());
		jassert(app != nullptr);
		return *app;
	}

	PropertiesFile* getPropertiesFile() { return propertiesFile; }
	LookAndFeel& getLookAndFeel() { return lookAndFeel; }
	RecentlyOpenedFilesList& getRecentFileList() { return recentFiles; }
	LumatoneController* getLumatoneController() { return lumatoneController.get(); }
	int getOctaveBoardSize() const { return lumatoneController->getOctaveSize(); }

	// Menu functionality
	ApplicationCommandManager* getCommandManager() { return commandManager.get(); }
	Lumatone::Menu::MainMenuModel* getMainMenu() { return menuModel.get(); }
	void getAllCommands(Array <CommandID>& commands) override;
	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
	bool perform(const InvocationInfo& info) override;

	bool openSysExMapping();
	bool saveSysExMapping(std::function<void(bool success)> saveFileCallback = CHOOSE_FILE_NOOP);
	bool saveSysExMappingAs(std::function<void(bool success)> saveFileCallback = CHOOSE_FILE_NOOP);
	bool resetSysExMapping();

	bool deleteSubBoardData();
	bool copySubBoardData();
	bool pasteSubBoardData();
    bool canPasteSubBoardData() const;

    void setEditMode(sysExSendingMode editMode);

	bool performUndoableAction(UndoableAction* editAction);
	bool undo();
	bool redo();
	bool generalOptionsDialog();
	bool noteOnOffVelocityCurveDialog();
	bool faderVelocityCurveDialog();
	bool aftertouchVelocityCurveDialog();

	bool openRecentFile(int recentFileIndex);
	bool openFromCurrentFile();
    bool setCurrentFile(File fileToOpen);
	bool saveCurrentFile(std::function<void(bool success)> saveFileCallback = CHOOSE_FILE_NOOP);

	void sendCurrentConfigurationToDevice();
	void requestConfigurationFromDevice();

	void updateMainTitle();

	bool getHasChangesToSave() const { return hasChangesToSave; }
	void setHasChangesToSave(bool value);

	bool aboutTerpstraSysEx();

	MainContentComponent* getMainContentComponent() const;

private:
	std::unique_ptr<MainWindow> mainWindow;
	std::unique_ptr<ApplicationCommandManager> commandManager;
	std::unique_ptr<Lumatone::Menu::MainMenuModel> menuModel;
	TooltipWindow				tooltipWindow;
	bool						hasChangesToSave;
	juce::UndoManager undoManager;

	LookAndFeel_V4				lookAndFeel;

	PropertiesFile*				propertiesFile;
	File						currentFile;
	RecentlyOpenedFilesList		recentFiles;

	std::unique_ptr<FileChooser> chooser;

	// MIDI connection
    std::unique_ptr<LumatoneController> lumatoneController;
};

