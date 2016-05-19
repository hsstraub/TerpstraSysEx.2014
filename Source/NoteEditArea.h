/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_8469F5B08F113FE2__
#define __JUCE_HEADER_8469F5B08F113FE2__

//[Headers]     -- You can add your own extra header files here --
#include "../../JuceLibraryCode/JuceHeader.h"
#include "IncrMidiNotesMapping.h"
#include "SingleNoteAssign.h"
#include "IsomorphicMassAssign.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class NoteEditArea  : public Component,
                      public ComboBoxListener
{
public:
    //==============================================================================
    NoteEditArea ();
    ~NoteEditArea();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	// Edit operation when a key field has been clicked
	void PerformMouseClickEdit(TerpstraKeyMapping& mappingData, int setSelection, int keySelection);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	ScopedPointer<IncrMidiNotesMapping>	incrMidiNotesMapping;

	ScopedPointer<SingleNoteAssign> singleNoteAssign;
	ScopedPointer<IsomorphicMassAssign> isomorphicMassAssign;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ComboBox> cbMappingStyle;
    ScopedPointer<Label> labelMappingStyle;
    ScopedPointer<ComboBox> cbEditMode;
    ScopedPointer<Label> labelEditMode;
    ScopedPointer<GroupComponent> groupMapping;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteEditArea)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_8469F5B08F113FE2__
