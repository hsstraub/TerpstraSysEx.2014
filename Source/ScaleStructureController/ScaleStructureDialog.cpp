/*
  ==============================================================================

    ScaleDesignWindow.cpp
    Created: 24 May 2020 7:09:24pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ScaleStructureDialog.h"

ScaleDesignWindow::ScaleDesignWindow(ScaleStructure& scaleStructure, Array<Colour>& colourTable, Colour backgroundColour)
	: DocumentWindow("Scale Designer", backgroundColour, DocumentWindow::closeButton, true)
{
	scaleDesigner.reset(new ScaleStructureComponent(scaleStructure, colourTable));
	setContentComponent(scaleDesigner.get());
	setSize(625, 625);
}

ScaleDesignWindow::~ScaleDesignWindow()
{
	scaleDesigner = nullptr;
}

void ScaleDesignWindow::setPeriod(int newPeriod)
{
	scaleDesigner->setPeriod(newPeriod);
}

void ScaleDesignWindow::addScaleDesignerListener(ScaleStructureComponent::Listener* listenerIn)
{
	scaleDesigner->addListener(listenerIn);
}

void ScaleDesignWindow::closeButtonPressed()
{
	setVisible(false);
}

void ScaleDesignWindow::resized() 
{
	DocumentWindow::resized();

	// Must be square
	int size = jmin(getWidth(), getHeight() - getTitleBarHeight());
	scaleDesigner->setBounds((getWidth() - size) / 2, getTitleBarHeight(), size, size);
}