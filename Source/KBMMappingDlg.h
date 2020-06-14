/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../../TerpstraSysEx.2014/JuceLibraryCode/JuceHeader.h"
#include "MappingLogic.h"
#include "KBMForOneChannel.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class KBMMappingDlg  : public Component
{
public:
    //==============================================================================
    KBMMappingDlg (int& periodSizeReference, ScaleStructure& scaleStructureIn, Array<Colour>& colourTableIn);
    ~KBMMappingDlg() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	MappingLogicBase* getMappingLogic() { return &mappingLogic; }
	void onUpdatePeriodSize();

	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void visibilityChanged() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    std::unique_ptr<KBMForOneChannel>	channelMappingComponents[KBMFilesMappingLogic::noOfChannels];
   	KBMFilesMappingLogic	mappingLogic;
   	int& periodSize;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Label> lblChannel;
    std::unique_ptr<Label> lblMappingFile;
    std::unique_ptr<Label> lblChannel2;
    std::unique_ptr<Label> lblMappingFile2;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KBMMappingDlg)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

