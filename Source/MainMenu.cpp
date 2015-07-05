/*
  ==============================================================================

    MainMenu.cpp
    Created: 13 Jun 2014 7:59:18pm
    Author:  hsstraub

  ==============================================================================
*/

#include "MainMenu.h"

TerpstraSysExMainMenuModel::TerpstraSysExMainMenuModel(ApplicationCommandManager* commandManager)
{
	theManager = commandManager;
	setApplicationCommandManagerToWatch(commandManager);

}

StringArray TerpstraSysExMainMenuModel::getMenuBarNames()
{
	const char* const names[] = { "File", "Help", nullptr };
	return StringArray(names);
}

void TerpstraSysExMainMenuModel::createFileMenu(PopupMenu& menu)
{
	menu.addCommandItem(theManager, openSysExMapping);
	menu.addCommandItem(theManager, saveSysExMapping);
	menu.addCommandItem(theManager, saveSysExMappingAs);
	menu.addCommandItem(theManager, resetSysExMapping);
#if ! JUCE_MAC
	menu.addSeparator();
	menu.addCommandItem(theManager, StandardApplicationCommandIDs::quit);
#endif
}

void TerpstraSysExMainMenuModel::createHelpMenu(PopupMenu& menu)
{
	menu.addCommandItem(theManager, aboutSysEx);
}

PopupMenu TerpstraSysExMainMenuModel::getMenuForIndex(int topLevelMenuIndex, const String& menuName)
{
	PopupMenu menu;

	if (menuName == "File")             createFileMenu(menu);
	else if (menuName == "Help")		createHelpMenu(menu);
	else                                jassertfalse; // names have changed?

	return menu;
}

void TerpstraSysExMainMenuModel::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
	// XXX
}
