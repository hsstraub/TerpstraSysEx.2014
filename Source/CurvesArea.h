/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>

#include "KeyboardDataStructure.h"

#include "LumatoneEditorStyleCommon.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class CurvesArea  : public juce::Component,
                    public juce::Button::Listener
{
public:
    //==============================================================================
    CurvesArea ();
    ~CurvesArea() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	class CurvesTabComponent : public juce::TabbedComponent
	{
	public:
		CurvesTabComponent(TabbedButtonBar::Orientation orientation);
		void 	currentTabChanged(int newCurrentTabIndex, const String &newCurrentTabName) override;

        void    setTabsIndent(int indentWidthIn);
        void    resized() override;

    private:

        int currentTabIndent = 0;
	};

	// New mapping is loaded. Display data.
	void loadFromMapping();

	void lookAndFeelChanged() override;

	// Send curves configs to controller
	void sendConfigToController();
  //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
 
	bool showDeveloperMode = false;

    //==============================================================================
    // Size and position constants
    const float curvesLabelHeight   = 0.0667f;
    const float tabDepth            = 0.115f;
    const float tabX                = 0.333f;
    const float tabWidth            = 0.65f;
    const float tabFontHeight       = 0.058f;
    
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> labelWindowTitle;
    std::unique_ptr<CurvesTabComponent> curvesTab;
    std::unique_ptr<juce::ToggleButton> btnDeveloperMode;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CurvesArea)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

