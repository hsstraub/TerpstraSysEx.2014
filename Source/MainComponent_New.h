/*
  ==============================================================================

    MainComponent_New.h
    Created: 20 Oct 2020 9:40:33pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent_New   : public Component
{
public:
    //==============================================================================
    MainComponent_New();
    ~MainComponent_New();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent_New)
};
