/*
  ==============================================================================

    HajuErrorVisualizer.h
    Created: 19 Apr 2020 11:33:04am
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

#include "../../TerpstraSysEx.2014/JuceLibraryCode/JuceHeader.h"


class HajuErrorVisualizer
{
    // Types
public:
	typedef enum
	{
		noError = 0,
		warning = 1,
		error = 2
	} ErrorLevel;

	// Methods
public:
    static void setErrorLevel(TextEditor& textEdit, ErrorLevel errorLevel, String toolTipText);
};
