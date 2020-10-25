/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.4

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "ViewConstants.h"
//[/Headers]

#include "NoteEditArea.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
NoteEditArea::NoteEditArea ()
{
    //[Constructor_pre] You can add your own custom stuff here..
	//singleNoteAssign.reset(new SingleNoteAssign());
	//addAndMakeVisible(singleNoteAssign.get());
	//singleNoteAssign->setVisible(false);

	//isomorphicMassAssign.reset(new IsomorphicMassAssign());
	//addAndMakeVisible(isomorphicMassAssign.get());
	//isomorphicMassAssign->setVisible(false);
    //[/Constructor_pre]

    editFunctionsTab.reset (new juce::TabbedComponent (juce::TabbedButtonBar::TabsAtTop));
    addAndMakeVisible (editFunctionsTab.get());
    editFunctionsTab->setTabBarDepth (30);
    editFunctionsTab->addTab (TRANS("Manual Assign"), juce::Colours::lightgrey, new SingleNoteAssign(), true);
    editFunctionsTab->addTab (TRANS("Isomorphic Assign"), juce::Colours::lightgrey, new IsomorphicMassAssign(), true);
    editFunctionsTab->setCurrentTabIndex (0);

    editFunctionsTab->setBounds (8, 8, 428, 480);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (428, 480);


    //[Constructor] You can add your own custom stuff here..

	// Default selection
	// Todo: read from user settings
	//cbEditMode->setSelectedItemIndex(noteEditMode::SingleNoteAssignMode, juce::NotificationType::sendNotification);

    //[/Constructor]
}

NoteEditArea::~NoteEditArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	//singleNoteAssign = nullptr;
	//isomorphicMassAssign = nullptr;
    //[/Destructor_pre]

    editFunctionsTab = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void NoteEditArea::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(findColour(ResizableWindow::backgroundColourId));
    //[/UserPaint]
}

void NoteEditArea::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	//singleNoteAssign->setBounds(0, NOTEASSIGNSUBWINTOP, EDITAREAWIDTH, NOTEASSIGNSUBWINHEIGHT);
	//isomorphicMassAssign->setBounds(0, NOTEASSIGNSUBWINTOP, EDITAREAWIDTH, NOTEASSIGNSUBWINHEIGHT);
	editFunctionsTab->setBounds(0, 0, getWidth(), getHeight());
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void NoteEditArea::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	//singleNoteAssign->restoreStateFromPropertiesFile(propertiesFile);
	//isomorphicMassAssign->restoreStateFromPropertiesFile(propertiesFile);
	// ToDO
}

void NoteEditArea::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	//singleNoteAssign->saveStateToPropertiesFile(propertiesFile);
	//isomorphicMassAssign->saveStateToPropertiesFile(propertiesFile);
	// ToDo
}

/// <summary>Called from MainComponent when one of the keys is clicked</summary>
/// <returns>Mapping was changed yes/no</returns>
bool NoteEditArea::performMouseDown(int setSelection, int keySelection)
{
	jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS && keySelection >= 0 && keySelection < TERPSTRABOARDSIZE);

	//int editMode = cbEditMode->getSelectedItemIndex();
	//switch (editMode)
	//{
	//case noteEditMode::SingleNoteAssignMode:
	//	return singleNoteAssign->performMouseDown(setSelection, keySelection);
	//case noteEditMode::IsomorphicMassAssignMode:
	//	return isomorphicMassAssign->performMouseDown(setSelection, keySelection);
	//default:
	//	return false;
	//}
	// ToDO
	return false;
}

/// <summary>Called from MainComponent when a previously clicked key is released</summary>
/// <returns>Mapping was changed yes/no</returns>
bool NoteEditArea::performMouseUp(int setSelection, int keySelection)
{
	jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS && keySelection >= 0 && keySelection < TERPSTRABOARDSIZE);

	//int editMode = cbEditMode->getSelectedItemIndex();

	// At the moment no functionality here

	return false;
}

void NoteEditArea::onSetData(TerpstraKeyMapping& newData)
{
	// Add colours of the mapping to the colour combo box
	// singleNoteAssign->onSetData(newData);
	//ToDO
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="NoteEditArea" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="428" initialHeight="480">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <TABBEDCOMPONENT name="editFunctionsTab" id="9eb88c4dce6dede9" memberName="editFunctionsTab"
                   virtualName="" explicitFocusOrder="0" pos="8 8 428 480" orientation="top"
                   tabBarDepth="30" initialTab="0">
    <TAB name="Manual Assign" colour="ffd3d3d3" useJucerComp="0" contentClassName="SingleNoteAssign"
         constructorParams="" jucerComponentFile=""/>
    <TAB name="Isomorphic Assign" colour="ffd3d3d3" useJucerComp="0" contentClassName="IsomorphicMassAssign"
         constructorParams="" jucerComponentFile=""/>
  </TABBEDCOMPONENT>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

