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

			deleteOctaveBoard = 0x200100,
			copyOctaveBoard = 0x200101,
			pasteOctaveBoard = 0x200102,
            pasteOctaveBoardNotes = 0x200103,
            pasteOctaveBoardChannels = 0x200104,
            pasteOctaveBoardColours = 0x200105,
            pasteOctaveBoardTypes = 0x200106,
            
			undo = 0x200200,
			redo = 0x200201,

			recentFilesBaseID = 0x200300,

			aboutSysEx = 0x200400
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
