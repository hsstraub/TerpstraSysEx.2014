/*
  ==============================================================================

    LumatoneMenu.cpp
    Created: 25 Apr 2021 12:07:58pm
    Author:  hsstraub

  ==============================================================================
*/

#include "LumatoneMenu.h"
#include "Main.h"

namespace Lumatone {
	namespace Menu {

		MainMenuModel::MainMenuModel(ApplicationCommandManager* commandManager)
		{
			theManager = commandManager;
			setApplicationCommandManagerToWatch(commandManager);

		}

		StringArray MainMenuModel::getMenuBarNames()
		{
			const char* const names[] = { "File", "Edit", "Help", nullptr };
			return StringArray(names);
		}

		void MainMenuModel::createFileMenu(PopupMenu& menu)
		{
			menu.addCommandItem(theManager, openSysExMapping);
			menu.addCommandItem(theManager, saveSysExMapping);
			menu.addCommandItem(theManager, saveSysExMappingAs);
			menu.addCommandItem(theManager, resetSysExMapping);

			menu.addSeparator();

			PopupMenu recentFilesMenu;
			TerpstraSysExApplication::getApp().getRecentFileList().createPopupMenuItems(recentFilesMenu, recentFilesBaseID, true, true);
			menu.addSubMenu("Recent Files", recentFilesMenu);

#if ! JUCE_MAC
			menu.addSeparator();
			menu.addCommandItem(theManager, StandardApplicationCommandIDs::quit);
#endif
		}

		void MainMenuModel::createEditMenu(PopupMenu& menu)
		{
			menu.addCommandItem(theManager, deleteOctaveBoard);
			menu.addCommandItem(theManager, copyOctaveBoard);
			menu.addCommandItem(theManager, pasteOctaveBoard);
			menu.addCommandItem(theManager, undo);
			menu.addCommandItem(theManager, redo);
		}

		void MainMenuModel::createHelpMenu(PopupMenu& menu)
		{
			menu.addCommandItem(theManager, aboutSysEx);
		}

		PopupMenu MainMenuModel::getMenuForIndex(int topLevelMenuIndex, const String& menuName)
		{
			PopupMenu menu;

			if (menuName == "File")             createFileMenu(menu);
			else if (menuName == "Edit")		createEditMenu(menu);
			else if (menuName == "Help")		createHelpMenu(menu);
			else                                jassertfalse; // names have changed?

			return menu;
		}

		void MainMenuModel::menuItemSelected(int menuItemID, int topLevelMenuIndex)
		{
			if (menuItemID >= recentFilesBaseID && menuItemID < recentFilesBaseID + 100)
			{
				// open a file from the "recent files" menu
				TerpstraSysExApplication::getApp().openRecentFile(menuItemID - recentFilesBaseID);
			}
		}

	}
}