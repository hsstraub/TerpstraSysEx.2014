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

void HajuErrorVisualizer::setErrorLevel(ErrorLevel errorLevel, String toolTipText)
{
    lastErrorLevel = errorLevel;
    currentText = toolTipText;
}

void HajuErrorVisualizer::setErrorLevel(
    SettableTooltipClient& tooltipClient,
    Component& component,
    ErrorLevel errorLevel,
    String toolTipText,
    int bgColourId)
{
    setErrorLevel(errorLevel, toolTipText);
    tooltipClient.setTooltip(toolTipText);
    component.setColour(bgColourId, errorLevelColours[errorLevel]);
    component.repaint();
}
