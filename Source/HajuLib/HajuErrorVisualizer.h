/*
  ==============================================================================

    HajuErrorVisualizer.h
    Created: 19 Apr 2020 11:33:04am
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


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

public:
    HajuErrorVisualizer(LookAndFeel& lookAndFeelValue);

    // Methods
public:
    void setErrorLevel(ErrorLevel errorLevel, String toolTipText);

    void setErrorLevel(
        SettableTooltipClient& tooltipClient,
        Component& component,
        ErrorLevel errorLevel,
        String toolTipText,
        int bgColourId);

    void setErrorLevel(TextEditor& textEdit, ErrorLevel errorLevel, String toolTipText) {
        return setErrorLevel(textEdit, textEdit, errorLevel, toolTipText, TextEditor::backgroundColourId);
    }

    void setErrorLevel(Label& label, ErrorLevel errorLevel, String toolTipText) {
        return setErrorLevel(label, label, errorLevel, toolTipText, Label::textColourId);
    }

    // Attributes
private:
    LookAndFeel&  lookAndFeel;
    ErrorLevel    lastErrorLevel;
    String        currentText;
    Array<Colour> errorLevelColours = { Colour(0xff84aea3), Colour(0xffc9c238), Colour(0xffd7002a) };
};