/*
  ==============================================================================

    LumatoneMenu.h
    Created: 8 Jan 2021 7:42:59pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


namespace Lumatone {
	namespace Menu {
		enum commandIDs {
			openSysExMapping = 0x200010,
			saveSysExMapping = 0x200011,
			saveSysExMappingAs = 0x200012,
			resetSysExMapping = 0x200013,

			deleteOctaveBoard = 0x200017,
			copyOctaveBoard = 0x200018,
			pasteOctaveBoard = 0x200019,

			recentFilesBaseID = 0x200100,

			aboutSysEx = 0x200040
		};

		class MainMenuModel : public MenuBarModel
		{
		public:
			MainMenuModel(ApplicationCommandManager* commandManager);

			virtual StringArray getMenuBarNames();
			PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName);
			void menuItemSelected(int menuItemID, int topLevelMenuIndex);

			virtual void createFileMenu(PopupMenu& menu);
			virtual void createEditMenu(PopupMenu& menu);
			virtual void createHelpMenu(PopupMenu& menu);

		private:
			ApplicationCommandManager * theManager;
		};

	}
	namespace Debug {
		enum commandIDs {
			toggleDeveloperMode = 0xA00001
		};
	}
}