/*
  ==============================================================================

    LumatoneMenu.h
    Created: 8 Jan 2021 7:42:59pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

namespace Lumatone {
	namespace Menu {
		enum commandIDs {
			deleteOctaveBoard = 0x200017,
			copyOctaveBoard = 0x200018,
			pasteOctaveBoard = 0x200019
		};
	}
	namespace Debug {
		enum commandIDs {
			toggleDeveloperMode = 0xA00001
		};
	}
}