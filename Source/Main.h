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
#include "MainComponent.h"
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
	bool saveCurrentFile();

	void sendCurrentConfigurationToDevice();
	void requestConfigurationFromDevice();

	void updateMainTitle();

	bool getHasChangesToSave() const { return hasChangesToSave; }
	void setHasChangesToSave(bool value);

	void setOpenDialogWindow(DialogWindow* dialogWindowIn);

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
#if JUCE_ANDROID
			setFullScreen(true);
#else
			setConstrainer(TerpstraSysExApplication::getApp().getBoundsConstrainer());
#endif

			setLookAndFeel(&TerpstraSysExApplication::getApp().getLookAndFeel());
		}

		void closeButtonPressed() override
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

		void saveStateToPropertiesFile(PropertiesFile* propertiesFile)
		{
			// Save state of main window
			propertiesFile->setValue("MainWindowState", getWindowStateAsString());
			((MainContentComponent*)(getContentComponent()))->saveStateToPropertiesFile(propertiesFile);
		}

		void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
		{
			// Check if stored height is larger than display area
            int maxWindowHeight = Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea.getHeight(); 
            String windowState = propertiesFile->getValue("MainWindowState");
            auto windowProperties = StringArray::fromTokens(windowState, false);
            bool useSavedState = windowProperties[windowProperties.size() - 1].getIntValue() <= maxWindowHeight;
            if (useSavedState)
            {
                useSavedState = restoreWindowStateFromString(propertiesFile->getValue("MainWindowState"));
            }

            if (!useSavedState)
            {
                // Default window state
                setSize(DEFAULTMAINWINDOWWIDTH, DEFAULTMAINWINDOWHEIGHT);
            }
            
			setVisible(true);

			((MainContentComponent*)(getContentComponent()))->restoreStateFromPropertiesFile(propertiesFile);
		}

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
	};

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
