/*
  ==============================================================================

    MainComponent_New.cpp
    Created: 20 Oct 2020 9:40:33pm
    Author:  hsstraub

  ==============================================================================
*/

#include "MainComponent_New.h"

//==============================================================================
MainComponent_New::MainComponent_New()
{
    setSize (600, 400);
}

MainComponent_New::~MainComponent_New()
{
}

//==============================================================================
void MainComponent_New::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainComponent_New::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
