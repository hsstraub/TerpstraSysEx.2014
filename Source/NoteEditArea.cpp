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

//[Headers] You can add your own extra header files here...
#include "ViewConstants.h"
#include "SingleNoteAssign.h"
#include "IsomorphicMassAssign.h"
#include "Main.h"
//[/Headers]

#include "NoteEditArea.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

// DEBUG
static Random r;


//[/MiscUserDefs]

//==============================================================================
NoteEditArea::NoteEditArea ()
    : Component("NoteEditArea"),
	  currentSingleKeySelection(-1)
{
    //[Constructor_pre] You can add your own custom stuff here..;
    showIsomorphicMassAssign = TerpstraSysExApplication::getApp().getPropertiesFile()->getBoolValue("IsomorphicMassAssign", false);
    //[/Constructor_pre]

	editFunctionsTab.reset(new juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop));
	editFunctionsTab->setName("EditFunctionsTab");
	editFunctionsTab->setColour(TabbedComponent::ColourIds::outlineColourId, Colour());
	editFunctionsTab->setColour(TabbedComponent::ColourIds::backgroundColourId, Colour());
	addAndMakeVisible(editFunctionsTab.get());
	editFunctionsTab->addTab(translate("ManualAssign"), juce::Colours::lightgrey, new SingleNoteAssign(), true);
	editFunctionsTab->setCurrentTabIndex(0);
	editFunctionsTab->setIndent(0);
	editFunctionsTab->setOutline(0);

  labelWindowTitle.reset (new juce::Label ("labelWindowTitle", translate("AssignKeys")));
	labelWindowTitle->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::UniviaProBold));
  addAndMakeVisible (labelWindowTitle.get());

  //[UserPreSize]

	if (showIsomorphicMassAssign)
		editFunctionsTab->addTab(TRANS("Isomorphic Assign"), juce::Colours::lightgrey, new IsomorphicMassAssign(), true);

	// Selector for octave boards
	octaveBoardSelectorTab.reset(new TabbedButtonBar(TabbedButtonBar::Orientation::TabsAtTop));
	addAndMakeVisible(octaveBoardSelectorTab.get());

	for (int i = 0; i < NUMBEROFBOARDS; i++)
	{
		octaveBoardSelectorTab->addTab(translate("Section") + " " + String(i + 1), juce::Colours::lightgrey, i + 1);
	}

	octaveBoardSelectorTab->addChangeListener(this);

	// Single Key fields
	for (int i = 0; i < TerpstraSysExApplication::getApp().getOctaveBoardSize(); i++)
	{
		terpstraKeyFields[i].reset(new TerpstraKeyEdit());
		addAndMakeVisible(terpstraKeyFields[i].get());
		terpstraKeyFields[i]->addMouseListener(this, true);
	}

    //[/UserPreSize]


    //[Constructor] You can add your own custom stuff here..

	// First octaveboard selection, selection on first key: see MainComponent (Has to be done after change listener has been established)

    //[/Constructor]
}

NoteEditArea::~NoteEditArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    editFunctionsTab = nullptr;
    labelWindowTitle = nullptr;


    //[Destructor]. You can add your own custom destruction code here..

	octaveBoardSelectorTab = nullptr;

	for (int i = 0; i < TerpstraSysExApplication::getApp().getOctaveBoardSize(); i++)
	{
		terpstraKeyFields[i] = nullptr;
	}

    //[/Destructor]
}

//==============================================================================
void NoteEditArea::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
	g.setColour(backgroundColour);
	g.fillRoundedRectangle(contentBackground, roundedCornerLayout);

	//g.setColour(Colours::pink);
	//g.drawRect(keyEditBounds);
    //[/UserPaint]
}

void NoteEditArea::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	int contentY = round(getHeight() * contentMarginY);
	contentBackground.setBounds(0, contentY, getWidth(), getHeight() - contentY);

	assignControlsBounds.setBounds(
		getWidth() * assignMarginX, contentBackground.getHeight() * assignMarginYInContent + contentBackground.getY(),
		getWidth() * assignControlsWidth, contentBackground.getHeight() * assignControlsHeightInContent
	);

	roundedCornerLayout = round(getParentHeight() * ROUNDEDCORNERTOAPPHEIGHT);

	octaveTabsArea.setBounds(0, 0, getWidth(), round(getHeight() * 0.069f));

	editFunctionsTab->setBounds(assignControlsBounds.toNearestInt());
	editFunctionsTab->setTabBarDepth(assignTabDepthInContent * contentBackground.getHeight());

	resizeLabelWithHeight(labelWindowTitle.get(), roundToInt(octaveBoardSelectorTab->getHeight() * assignLabelTabDepthHeight));
	labelWindowTitle->setTopLeftPosition(roundToInt(getWidth() * assignLabelMarginX), roundToInt((octaveTabsArea.getHeight() - labelWindowTitle->getHeight()) * 0.5f));

	keyEditBounds.setBounds(
		getWidth() * keyEditMarginX, contentBackground.getHeight() * assignMarginYInContent + contentBackground.getY(),
		getWidth() * keyEditWidth, contentBackground.getHeight() * assignControlsHeightInContent
	);

    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..

	octaveBoardSelectorTab->setBounds(labelWindowTitle->getRight(), 0, getWidth() - labelWindowTitle->getRight(), octaveTabsArea.getHeight());

	// Single Key fields

	keyEditBounds = contentBackground.withLeft(assignControlsBounds.getRight() + assignControlsBounds.getX() / 2);

	tilingGeometry.fitTilingTo(
		keyEditBounds,
		boardGeometry.getMaxHorizontalLineSize(),
		boardGeometry.horizontalLineCount(),
		round(keyEditBounds.getWidth() * singleKeyMarginFromWidth),
		TERPSTRASINGLEKEYROTATIONANGLE, true
	);

	Array<Point<float>> keyCentres = tilingGeometry.getHexagonCentres(boardGeometry);
	jassert(keyCentres.size() == TerpstraSysExApplication::getApp().getOctaveBoardSize());

	float keySize = tilingGeometry.getKeySize();
	
	int keyIndex = 0;
	for (keyIndex = 0; keyIndex < keyCentres.size(); keyIndex++)
	{
		Point<float> centre = keyCentres[keyIndex];

		terpstraKeyFields[keyIndex]->setKeySize(keySize);
		terpstraKeyFields[keyIndex]->setCentrePosition(centre.roundToInt());
	}

	jassert(TerpstraSysExApplication::getApp().getOctaveBoardSize() == keyIndex);

    //[/UserResized]
}

void NoteEditArea::mouseDown (const juce::MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
	bool mappingChanged = false;

	// Selection of single key fields
	for (int keyIndex = 0; keyIndex < TerpstraSysExApplication::getApp().getOctaveBoardSize(); keyIndex++)
	{
		if (e.eventComponent == terpstraKeyFields[keyIndex].get() || e.eventComponent->getParentComponent() == terpstraKeyFields[keyIndex].get())
		{
			// Select field
			changeSingleKeySelection(keyIndex);

			// Perform the edit, according to edit mode. Including sending to device
			auto setSelection = octaveBoardSelectorTab->getCurrentTabIndex();
			jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS && keyIndex >= 0 && keyIndex < TerpstraSysExApplication::getApp().getOctaveBoardSize());

			int editMode = editFunctionsTab->getCurrentTabIndex();
			switch (editMode)
			{
			case noteEditMode::SingleNoteAssignMode:
				mappingChanged = dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(editMode))->performMouseDown(setSelection, keyIndex);
				break;
			case noteEditMode::IsomorphicMassAssignMode:
				mappingChanged = dynamic_cast<IsomorphicMassAssign*>(editFunctionsTab->getTabContentComponent(editMode))->performMouseDown(setSelection, keyIndex);
				break;
			default:
				break;
			}

			break;
		}
	}

	// Mark that there are changes
	if (mappingChanged)
	{
		TerpstraSysExApplication::getApp().setHasChangesToSave(true);

		// Refresh key fields (all may be affected)
		// repaint();	That should be enough - but is not. apparently...XXX
		refreshKeyFields();

		((MainContentComponent*)getParentComponent())->refreshAllKeysOverview();
	}

    //[/UserCode_mouseDown]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void NoteEditArea::lookAndFeelChanged()
{
	LookAndFeel& lookAndFeel = getLookAndFeel();
	backgroundColour = lookAndFeel.findColour(LumatoneEditorColourIDs::ControlAreaBackground);
	labelWindowTitle->setColour(Label::ColourIds::textColourId, lookAndFeel.findColour(LumatoneEditorColourIDs::LabelBlue));
}

void NoteEditArea::setControlsTopLeftPosition(int controlsAreaX, int controlsAreaY)
{
	setTopLeftPosition(controlsAreaX, controlsAreaY - octaveTabsArea.getHeight());
}


void NoteEditArea::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->restoreStateFromPropertiesFile(propertiesFile);
	
	if (showIsomorphicMassAssign)
		dynamic_cast<IsomorphicMassAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::IsomorphicMassAssignMode))->restoreStateFromPropertiesFile(propertiesFile);
}

void NoteEditArea::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->saveStateToPropertiesFile(propertiesFile);

	if (showIsomorphicMassAssign)
		dynamic_cast<IsomorphicMassAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::IsomorphicMassAssignMode))->saveStateToPropertiesFile(propertiesFile);
}

void NoteEditArea::changeListenerCallback(ChangeBroadcaster *source)
{
	if (source == octaveBoardSelectorTab.get())
	{
		auto setSelection = octaveBoardSelectorTab->getCurrentTabIndex();
		jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS);

		setKeyFieldValues(((MainContentComponent*)getParentComponent())->getMappingInEdit().sets[setSelection]);
	}
}


void NoteEditArea::onSetData(TerpstraKeyMapping& newData)
{
	// Add colours of the mapping to the colour combo box
	return dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->onSetData(newData);
}

void NoteEditArea::setKeyFieldValues(const TerpstraKeys& keySet)
{
	for (int i = 0; i < TerpstraSysExApplication::getApp().getOctaveBoardSize(); i++)
		terpstraKeyFields[i]->setValue(keySet.theKeys[i]);
}

ColourEditComponent* NoteEditArea::getColourEditComponent()
{
	return dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->getColourEditComponent();
}

ColourTextEditor* NoteEditArea::getSingleNoteColourTextEditor() 
{
	return dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->getColourTextEditor();;
}

void NoteEditArea::changeSingleKeySelection(int newSelection)
{
	// Unselect previous key
	if (currentSingleKeySelection >= 0 && currentSingleKeySelection < TerpstraSysExApplication::getApp().getOctaveBoardSize())
		terpstraKeyFields[currentSingleKeySelection]->setIsSelected(false);

	// Select new key
	currentSingleKeySelection = newSelection;
	if (currentSingleKeySelection >= 0 && currentSingleKeySelection < TerpstraSysExApplication::getApp().getOctaveBoardSize())
		terpstraKeyFields[currentSingleKeySelection]->setIsSelected(true);
}

void NoteEditArea::refreshKeyFields()
{
	auto setSelection = octaveBoardSelectorTab->getCurrentTabIndex();
	jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS);
	setKeyFieldValues(((MainContentComponent*)getParentComponent())->getMappingInEdit().sets[setSelection]);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="NoteEditArea" componentName=""
                 parentClasses="public Component, public ChangeListener" constructorParams=""
                 variableInitialisers="currentSingleKeySelection(-1)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="0"
                 initialWidth="760" initialHeight="470">
  <METHODS>
    <METHOD name="mouseDown (const juce::MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffbad0de"/>
  <TABBEDCOMPONENT name="editFunctionsTab" id="9eb88c4dce6dede9" memberName="editFunctionsTab"
                   virtualName="" explicitFocusOrder="0" pos="8 48 320 422" orientation="top"
                   tabBarDepth="30" initialTab="0">
    <TAB name="Manual Assign" colour="ffd3d3d3" useJucerComp="0" contentClassName="SingleNoteAssign"
         constructorParams="" jucerComponentFile=""/>
  </TABBEDCOMPONENT>
  <LABEL name="labelWindowTitle" id="afc0b85c8e03b3d6" memberName="labelWindowTitle"
         virtualName="" explicitFocusOrder="0" pos="8 8 104 24" textCol="ff61acc8"
         edTextCol="ff000000" edBkgCol="0" labelText="Assign Keys" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="18.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

