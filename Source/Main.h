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
#include "LumatoneController.h"

#include "ViewConstants.h"
#include "LumatoneEditorFonts.h"
#include "ColourPaletteDataStructure.h"
#include "LocalisationMap.h"
#include "FirmwareTransfer.h"

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
	LumatoneEditorLookAndFeel& getLookAndFeel() { return lookAndFeel; }
	ComponentBoundsConstrainer* getBoundsConstrainer() { return boundsConstrainer.get(); };
	RecentlyOpenedFilesList& getRecentFileList() { return recentFiles; }
	LumatoneController* getLumatoneController() { return lumatoneController.get(); }
	Array<LumatoneEditorColourPalette>& getColourPalettes() { return colourPalettes; }
	Font getAppFont(LumatoneEditorFont fontIdIn, float height = 12.0f) { return appFonts.getFont(fontIdIn, height); }
	int getOctaveBoardSize() const { return lumatoneController->getOctaveSize(); }
    int getNumBoards() const { return lumatoneController->getNumBoards(); }

	FirmwareVersion getFirmwareVersion() const { return lumatoneController->getFirmwareVersion(); }
	String getFirmwareVersionStr() const { return lumatoneController->getFirmwareVersion().toDisplayString(); }

	void setFirmwareUpdatePerformed(bool updateWasRun) { firmwareUpdateWasPerformed = true; }

	void reloadColourPalettes();
	bool saveColourPalette(LumatoneEditorColourPalette& palette, File pathToPalette=File());
	bool deletePaletteFile(File pathToPalette);

	// Menu functionality
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
    bool pasteModifiedSubBoardData(CommandID commandID);
    bool canPasteSubBoardData() const;
    
    void setEditMode(sysExSendingMode editMode);

	void setCalibrationMode(bool calibrationStarted) { inCalibrationMode = calibrationStarted; }
	bool getInCalibrationMode() const { return inCalibrationMode; }

	bool performUndoableAction(UndoableAction* editAction);
	bool undo();
	bool redo();

	bool toggleDeveloperMode();

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

	void setOpenDialogWindow(DialogWindow* dialogWindowIn);

	bool aboutTerpstraSysEx();


	MainContentComponent* getMainContentComponent() const;

private:
	std::unique_ptr<MainWindow> mainWindow;

	std::unique_ptr<ComponentBoundsConstrainer> boundsConstrainer;

	std::unique_ptr<ApplicationCommandManager> commandManager;
	std::unique_ptr<Lumatone::Menu::MainMenuModel> menuModel;
	TooltipWindow				tooltipWindow;
	bool						hasChangesToSave;

	bool						inCalibrationMode = false;

	juce::UndoManager undoManager;
	
	LumatoneEditorFonts			appFonts;
	LumatoneEditorLookAndFeel	lookAndFeel;

	PropertiesFile*				propertiesFile;
	File						currentFile;
	RecentlyOpenedFilesList		recentFiles;

	File						userDocumentsDirectory;
	File						userMappingsDirectory;
	File						userPalettesDirectory;

	Array<LumatoneEditorColourPalette> colourPalettes;

	// Make sure an open dialog window is deleted on shutdown
	std::unique_ptr<DialogWindow> dialogWindow;

	// Communication with Lumatone
	std::unique_ptr<LumatoneController> lumatoneController;

	std::unique_ptr<FileChooser> chooser;

	bool firmwareUpdateWasPerformed = false; // Allows us to deinitialize libssh2 a single time
};
