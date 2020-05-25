/*
  ==============================================================================

    ScaleDesignWindow.h
    Created: 24 May 2020 7:09:24pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ScaleStructureComponent.h"

class ScaleDesignWindow : public DocumentWindow
{
public:

	ScaleDesignWindow(ScaleStructure& scaleStructure, Array<Colour>& colourTable, Colour backgroundColour);
	~ScaleDesignWindow();

	void addScaleDesignerListener(ScaleStructureComponent::Listener* listenerIn);
	void setPeriod(int newPeriod);

	void closeButtonPressed() override;
	void resized() override;

private:

	std::unique_ptr<ScaleStructureComponent> scaleDesigner;
};