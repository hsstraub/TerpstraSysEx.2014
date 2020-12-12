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
#include "BoardGeometry.h"
#include "ViewComponents.h"
#include "MainComponent.h"
#include "Main.h"

//[/Headers]

#include "AllKeysOverview.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

// Geometry settings
static TerpstraBoardGeometry	boardGeometry;


//==============================================================================
KeyMiniDisplayInsideAllKeysOverview::KeyMiniDisplayInsideAllKeysOverview(int newBoardIndex, int newKeyIndex)
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
	boardIndex = newBoardIndex;
	keyIndex = newKeyIndex;

}

KeyMiniDisplayInsideAllKeysOverview::~KeyMiniDisplayInsideAllKeysOverview()
{
}

void KeyMiniDisplayInsideAllKeysOverview::paint(Graphics& g)
{
	jassert(getParentComponent() != nullptr);
	bool boardIsSelected = boardIndex == dynamic_cast<AllKeysOverview*>(getParentComponent())->getCurrentSetSelection();

	Colour hexagonColour = findColour(TerpstraKeyEdit::backgroundColourId).overlaidWith(getKeyColour()
		.withAlpha(boardIsSelected ? TERPSTRASINGLEKEYCOLOURALPHA : TERPSTRASINGLEKEYCOLOURUNSELECTEDMINIALPHA));
	// ToDo if highlighted: even different alpha?
	g.setColour(hexagonColour);
	g.fillPath(hexPath);

	// Key highlighted or not: color and thickness of the line
	float lineWidth = isHighlighted ? 1.5 : 1;
	Colour lineColour = findColour(isHighlighted ? TerpstraKeyEdit::selectedKeyOutlineId : TerpstraKeyEdit::outlineColourId);
	g.setColour(lineColour);
	g.strokePath(hexPath, PathStrokeType(1));
}

void KeyMiniDisplayInsideAllKeysOverview::resized()
{
	float w = this->getWidth();
	float h = this->getHeight();

	float marginOffset = 1.5;

	// recalculate position and size of hexagon
	hexPath.clear();
	hexPath.startNewSubPath(w / 2.0f, 0);
	hexPath.lineTo(w, h / 4.0f);
	hexPath.lineTo(w, 3.0f * h / 4.0f);
	hexPath.lineTo(w / 2.0f, h);
	hexPath.lineTo(0, 3.0f * h / 4.0f);
	hexPath.lineTo(0, h / 4.0f);
	hexPath.closeSubPath();

	// Rotate slightly counterclockwise around the center
	AffineTransform transform = AffineTransform::translation(-w / 2.0f, -h / 2.0f);
	transform = transform.rotated(TERPSTRASINGLEKEYROTATIONANGLE);
	transform = transform.translated(w / 2.0f, h / 2.0f);

	hexPath.applyTransform(transform);
	hexPath.scaleToFit(marginOffset, marginOffset, w - 2 * marginOffset, h - 2 * marginOffset, true);
}

void KeyMiniDisplayInsideAllKeysOverview::mouseDown(const MouseEvent& e)
{
	Component::mouseDown(e);

	// Select this octave board
	jassert(getParentComponent() != nullptr);
	jassert(getParentComponent()->getParentComponent() != nullptr);
	dynamic_cast<MainContentComponent*>(getParentComponent()->getParentComponent())->
		getOctaveBoardSelectorTab()->setCurrentTabIndex(boardIndex);

	isHighlighted = true;
	repaint();

	// NoteOn MIDI message
	auto keyData = getKeyData();
	if (keyData != nullptr && keyData->channelNumber > 0)
	{
		if (keyData->keyType == TerpstraKey::noteOnNoteOff)
		{
			// Send "note on" event
			TerpstraSysExApplication::getApp().getMidiDriver().sendNoteOnMessage(keyData->noteNumber, keyData->channelNumber, 60);
		}
		// ToDo if keyType is "continuous controller": send controller event?
	}
}

void KeyMiniDisplayInsideAllKeysOverview::mouseUp(const MouseEvent& e)
{
	Component::mouseDown(e);

	isHighlighted = false;
	repaint();

	// NoteOff MIDI message
	auto keyData = getKeyData();
	if (keyData != nullptr && keyData->channelNumber > 0)
	{
		if (keyData->keyType == TerpstraKey::noteOnNoteOff)
		{
			// Send "note off" event
			TerpstraSysExApplication::getApp().getMidiDriver().sendNoteOffMessage(keyData->noteNumber, keyData->channelNumber, 60);
		}
	}
}

const TerpstraKey* KeyMiniDisplayInsideAllKeysOverview::getKeyData() const
{
	if (boardIndex >= 0 && boardIndex < NUMBEROFBOARDS && keyIndex >= 0 && keyIndex < TERPSTRABOARDSIZE)
	{
		jassert(getParentComponent() != nullptr);
		jassert(getParentComponent()->getParentComponent() != nullptr);
		return &dynamic_cast<MainContentComponent*>(getParentComponent()->getParentComponent())
			->getMappingInEdit().sets[boardIndex].theKeys[keyIndex];
	}

	return nullptr;
}

Colour KeyMiniDisplayInsideAllKeysOverview::getKeyColour() const
{
	auto keyData = getKeyData();
	if ( keyData != nullptr)
		return Colour(keyData->colour);
	else
		return findColour(TerpstraKeyEdit::backgroundColourId);
}

//[/MiscUserDefs]

//==============================================================================
AllKeysOverview::AllKeysOverview ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    btnLoadFile.reset (new juce::TextButton ("btnLoadFile"));
    addAndMakeVisible (btnLoadFile.get());
    btnLoadFile->setButtonText (TRANS("Load File"));
    btnLoadFile->addListener (this);

    btnLoadFile->setBounds (368, 8, 96, 24);

    btnSaveFile.reset (new juce::TextButton ("btnSaveFile"));
    addAndMakeVisible (btnSaveFile.get());
    btnSaveFile->setButtonText (TRANS("Save File"));
    btnSaveFile->addListener (this);

    btnSaveFile->setBounds (472, 8, 96, 24);


    //[UserPreSize]

	for (int subBoardIndex = 0; subBoardIndex < NUMBEROFBOARDS; subBoardIndex++)
	{
		for (int keyIndex = 0; keyIndex < TERPSTRABOARDSIZE; keyIndex++)
		{
			octaveBoards[subBoardIndex].keyMiniDisplay[keyIndex].reset(new KeyMiniDisplayInsideAllKeysOverview(subBoardIndex, keyIndex));
			addAndMakeVisible(octaveBoards[subBoardIndex].keyMiniDisplay[keyIndex].get());
		}
	}

    //[/UserPreSize]

    setSize (928, 214);


    //[Constructor] You can add your own custom stuff here..
	currentSetSelection = -1;
    //[/Constructor]
}

AllKeysOverview::~AllKeysOverview()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    btnLoadFile = nullptr;
    btnSaveFile = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	for (int subBoardIndex = 0; subBoardIndex < NUMBEROFBOARDS; subBoardIndex++)
	{
		for (int i = 0; i < TERPSTRABOARDSIZE; i++)
		{
			octaveBoards[subBoardIndex].keyMiniDisplay[i] = nullptr;
		}
	}
    //[/Destructor]
}

//==============================================================================
void AllKeysOverview::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(findColour(ResizableWindow::backgroundColourId));

	// Draw a line under the selected sub board
	if (currentSetSelection >= 0 && currentSetSelection < NUMBEROFBOARDS)
	{
		Path selectionMarkPath;
		auto yPos = getHeight() - TERPSTRAKEYSETVERTICALRIM / 2;
		selectionMarkPath.startNewSubPath(octaveBoards[currentSetSelection].leftPos, yPos);
		selectionMarkPath.lineTo(octaveBoards[currentSetSelection].rightPos, yPos);

		Colour lineColour = findColour(TerpstraKeyEdit::outlineColourId);
		g.setColour(lineColour);
		g.strokePath(selectionMarkPath, PathStrokeType(4));
	}

    //[/UserPaint]
}

void AllKeysOverview::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	int newHeight = getHeight();
	int newWidth = getWidth();

	// Single key field size
	int newSingleKeySize = jmin(newWidth / 33, newHeight / 10);

	// Transformation Rotate slightly counterclockwise
	AffineTransform transform = AffineTransform::rotation(TERPSTRASINGLEKEYROTATIONANGLE);

    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..

	int rowCount = boardGeometry.horizontalLineCount();
	float x = 0.0f;
	float y = 0.0f;
	int subBoardIndex, keyIndex;
	int mostBottomKeyPos = 0;

	// All key fields of all sub boards, according to board geometry
	for (subBoardIndex = 0; subBoardIndex < NUMBEROFBOARDS; subBoardIndex++)
	{
		keyIndex = 0;
		octaveBoards[subBoardIndex].leftPos = newWidth;
		octaveBoards[subBoardIndex].rightPos = 0;

		for (int rowIndex = 0; rowIndex < rowCount; rowIndex++)
		{
			float xbasepos;
			if (rowIndex % 2 == 0)
				xbasepos = 6.0f * subBoardIndex * newSingleKeySize;
			else
				xbasepos = 6.0f * subBoardIndex * newSingleKeySize + newSingleKeySize / 2.0f;

			float ybasepos = btnLoadFile->getBottom() + TERPSTRAKEYSETVERTICALRIM + subBoardIndex * newSingleKeySize * 3.0f / 2.0f + rowIndex * newSingleKeySize * 3.0f / 4.0f;

			int subBoardRowSize = boardGeometry.horizontalLineSize(rowIndex);
			for (int posInRow = 0; posInRow < subBoardRowSize; posInRow++)
			{
				x = xbasepos + (boardGeometry.firstColumnOffset(rowIndex) + posInRow)*newSingleKeySize;
				y = ybasepos;
				transform.transformPoint(x, y);
				octaveBoards[subBoardIndex].keyMiniDisplay[keyIndex]->setBounds(roundToInt(x), roundToInt(y), newSingleKeySize, newSingleKeySize);

				mostBottomKeyPos = jmax(mostBottomKeyPos, octaveBoards[subBoardIndex].keyMiniDisplay[keyIndex]->getBottom());

				octaveBoards[subBoardIndex].leftPos = jmin(octaveBoards[subBoardIndex].leftPos, octaveBoards[subBoardIndex].keyMiniDisplay[keyIndex]->getX());
				octaveBoards[subBoardIndex].rightPos = jmax(octaveBoards[subBoardIndex].rightPos, octaveBoards[subBoardIndex].keyMiniDisplay[keyIndex]->getRight());

				keyIndex++;
			}
		}
	}

	// Move key fields to bottom
	if (mostBottomKeyPos < newHeight - TERPSTRAKEYSETVERTICALRIM)
	{
		int ydispacement = (newHeight - TERPSTRAKEYSETVERTICALRIM - mostBottomKeyPos)/2;
		for (subBoardIndex = 0; subBoardIndex < NUMBEROFBOARDS; subBoardIndex++)
		{
			for (keyIndex = 0; keyIndex < TERPSTRABOARDSIZE; keyIndex++)
			{
				octaveBoards[subBoardIndex].keyMiniDisplay[keyIndex]->setTopLeftPosition(
					juce::Point<int>(octaveBoards[subBoardIndex].keyMiniDisplay[keyIndex]->getX(), octaveBoards[subBoardIndex].keyMiniDisplay[keyIndex]->getY() + ydispacement));
			}
		}
	}

    //[/UserResized]
}

void AllKeysOverview::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnLoadFile.get())
    {
        //[UserButtonCode_btnLoadFile] -- add your button handler code here..
		TerpstraSysExApplication::getApp().openSysExMapping();
        //[/UserButtonCode_btnLoadFile]
    }
    else if (buttonThatWasClicked == btnSaveFile.get())
    {
        //[UserButtonCode_btnSaveFile] -- add your button handler code here..
		TerpstraSysExApplication::getApp().saveSysExMappingAs();
        //[/UserButtonCode_btnSaveFile]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AllKeysOverview" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="928" initialHeight="214">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTBUTTON name="btnLoadFile" id="6c0c074c9f137f23" memberName="btnLoadFile"
              virtualName="" explicitFocusOrder="0" pos="368 8 96 24" buttonText="Load File"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnSaveFile" id="abbc33d699ba1e52" memberName="btnSaveFile"
              virtualName="" explicitFocusOrder="0" pos="472 8 96 24" buttonText="Save File"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

