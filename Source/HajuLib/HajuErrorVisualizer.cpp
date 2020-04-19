/*
  ==============================================================================

    HajuErrorVisualizer.cpp
    Created: 19 Apr 2020 11:33:04am
    Author:  hsstraub

  ==============================================================================
*/

#include "HajuErrorVisualizer.h"

HajuErrorVisualizer::HajuErrorVisualizer(LookAndFeel& lookAndFeelValue)
: lookAndFeel(lookAndFeelValue)
{
}

void HajuErrorVisualizer::setErrorLevel(TextEditor& textEdit, ErrorLevel errorLevel, String toolTipText)
{
    Colour bgColour = lookAndFeel.findColour(TextEditor::backgroundColourId);

    switch(errorLevel)
    {
    case ErrorLevel::warning:
        bgColour = bgColour.overlaidWith(Colours::yellow).withAlpha(0.5f);
        break;
    case ErrorLevel::error:
        bgColour = bgColour.overlaidWith(Colours::red).withAlpha(0.5f);
        break;
    default:
        break;
    }

    textEdit.setTooltip(toolTipText);
    textEdit.setColour(TextEditor::backgroundColourId, bgColour);

}
