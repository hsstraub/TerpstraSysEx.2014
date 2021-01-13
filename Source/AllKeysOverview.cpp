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
static Random r;//DEBUG

//==============================================================================
KeyMiniDisplayInsideAllKeysOverview::KeyMiniDisplayInsideAllKeysOverview(int newBoardIndex, int newKeyIndex)
	: Component("AllKeysOverview_Key" + String(newKeyIndex) + "," + String(newBoardIndex))
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
	boardIndex = newBoardIndex;
	keyIndex = newKeyIndex;

	// DEBUG
	keyColour = Colour(r.nextFloat(), r.nextFloat(), 1.0f, 1.0f);
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
	//// ToDo if highlighted: even different alpha?
	//g.setColour(hexagonColour);
	//g.fillPath(hexPath);

	//// Key highlighted or not: color and thickness of the line
	//float lineWidth = isHighlighted ? 1.5 : 1;
	//Colour lineColour = findColour(isHighlighted ? TerpstraKeyEdit::selectedKeyOutlineId : TerpstraKeyEdit::outlineColourId);
	//g.setColour(lineColour);
	//g.strokePath(hexPath, PathStrokeType(1));

	g.setColour(hexagonColour);

	if (colourGraphic && shadowGraphic)
	{
		int w = getWidth();
		int h = getHeight();
		int x = round((getWidth() - colourGraphic->getWidth()) / 2.0f);
		int y = round((getHeight() - colourGraphic->getHeight()) / 2.0f);

		g.drawImageAt(*colourGraphic, x, y, true);
		g.drawImageAt(*shadowGraphic, x, y);
	}
}

void KeyMiniDisplayInsideAllKeysOverview::resized()
{

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

void KeyMiniDisplayInsideAllKeysOverview::setKeyGraphics(Image& colourGraphicIn, Image& shadowGraphicIn)
{
	colourGraphic = &colourGraphicIn;
	shadowGraphic = &shadowGraphicIn;
}

//[/MiscUserDefs]

//==============================================================================
AllKeysOverview::AllKeysOverview()
	: Component("AllKeysOverview")
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    btnLoadFile.reset (new juce::TextButton ("btnLoadFile"));
    addAndMakeVisible (btnLoadFile.get());
    btnLoadFile->setButtonText (translate("LoadFile"));
    btnLoadFile->addListener (this);

    btnSaveFile.reset (new juce::TextButton ("btnSaveFile"));
    addAndMakeVisible (btnSaveFile.get());
    btnSaveFile->setButtonText (translate("SaveFile"));
    btnSaveFile->addListener (this);

    //[UserPreSize]

	for (int subBoardIndex = 0; subBoardIndex < NUMBEROFBOARDS; subBoardIndex++)
	{
		OctaveBoard* board = octaveBoards.add(new OctaveBoard());

		for (int keyIndex = 0; keyIndex < TERPSTRABOARDSIZE; keyIndex++)
		{
			auto key = board->keyMiniDisplay.add(new KeyMiniDisplayInsideAllKeysOverview(subBoardIndex, keyIndex));
			addAndMakeVisible(key);
		}

		jassert(board->keyMiniDisplay.size() == TERPSTRABOARDSIZE);
	}

	jassert(octaveBoards.size() == NUMBEROFBOARDS);

    //[/UserPreSize]


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
	//for (int subBoardIndex = 0; subBoardIndex < NUMBEROFBOARDS; subBoardIndex++)
	//{
	//	for (int i = 0; i < TERPSTRABOARDSIZE; i++)
	//	{
	//		octaveBoards[subBoardIndex]->keyMiniDisplay[i] = nullptr;
	//	}
	//}
    //[/Destructor]
}

//==============================================================================
void AllKeysOverview::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..

	g.drawImageAt(lumatoneGraphic, lumatoneBounds.getX(), lumatoneBounds.getY());

	// Draw a line under the selected sub board
	if (currentSetSelection >= 0 && currentSetSelection < NUMBEROFBOARDS)
	{
		Path selectionMarkPath;
		auto yPos = getHeight() - TERPSTRAKEYSETVERTICALRIM / 2;
		selectionMarkPath.startNewSubPath(octaveBoards[currentSetSelection]->leftPos, yPos);
		selectionMarkPath.lineTo(octaveBoards[currentSetSelection]->rightPos, yPos);

		Colour lineColour = findColour(TerpstraKeyEdit::outlineColourId);
		g.setColour(lineColour);
		g.strokePath(selectionMarkPath, PathStrokeType(4));
	}

    //[/UserPaint]
}

void AllKeysOverview::resized()
{
    //[UserPreResize] Add your own custom resize code here..

	// Prepare position helpers for graphics
	int graphicHeight = roundToInt(getHeight() * imageHeight);
	int graphicWidth  = roundToInt(imageAspect * graphicHeight);

	lumatoneBounds.setBounds(
		roundToInt((getWidth() - graphicWidth) / 2.0f), roundToInt(getHeight() * imageY),
		graphicWidth, graphicHeight
	);

	lumatoneGraphic = ImageCache::getFromHashCode(LumatoneEditorAssets::LumatoneGraphic).rescaled(graphicWidth, graphicHeight, Graphics::highResamplingQuality);
	
	keybedBounds = Rectangle<float>(
		Point<float>(keybedX * lumatoneBounds.getWidth() + lumatoneBounds.getX(), keybedY * lumatoneBounds.getHeight() + lumatoneBounds.getY()),
		Point<float>(keybedBottomX * lumatoneBounds.getWidth() + lumatoneBounds.getX(), keybedBottomY * lumatoneBounds.getHeight() + lumatoneBounds.getY())
	);

	oct1Key1  = Point<float>(oct1Key1X  * lumatoneBounds.getWidth() + lumatoneBounds.getX(), oct1Key1Y  * lumatoneBounds.getHeight() + lumatoneBounds.getY());
	oct1Key56 = Point<float>(oct1Key56X * lumatoneBounds.getWidth() + lumatoneBounds.getX(), oct1Key56Y * lumatoneBounds.getHeight() + lumatoneBounds.getY());
	oct5Key7  = Point<float>(oct5Key7X  * lumatoneBounds.getWidth() + lumatoneBounds.getX(), oct5Key7Y  * lumatoneBounds.getHeight() + lumatoneBounds.getY());

	int boardLineLength = boardGeometry.getMaxHorizontalLineSize() * NUMBEROFBOARDS;
	int boardNumLines = boardGeometry.horizontalLineCount() + BOARDROWOFFSET * (NUMBEROFBOARDS - 1);
	tilingGeometry.fitSkewedTiling(oct1Key1, oct1Key56, 10, oct5Key7, 24);

	Array<Point<float>> keyCentres = tilingGeometry.getHexagonCentresSkewed(boardGeometry, 0, NUMBEROFBOARDS);
	jassert(keyCentres.size() == TERPSTRABOARDSIZE * NUMBEROFBOARDS);

	int keyWidth  = round(lumatoneBounds.getWidth() * keyW);
	int keyHeight = round(lumatoneBounds.getHeight() * keyH);

	// Scale key graphics once
	keyShapeGraphic = ImageCache::getFromHashCode(LumatoneEditorAssets::KeyShape).rescaled(keyWidth, keyHeight);
	keyShadowGraphic = ImageCache::getFromHashCode(LumatoneEditorAssets::KeyShadow).rescaled(keyWidth, keyHeight);

	int octaveIndex = 0;
	octaveBoards[octaveIndex]->leftPos = keybedBounds.getX();

	for (int keyIndex = 0; keyIndex < keyCentres.size(); keyIndex++)
	{
		int keyOctaveIndex = keyIndex % TERPSTRABOARDSIZE;

		// Apply rotational transform
		Point<int> centre = keyCentres[keyIndex].roundToInt();
			
		auto key = octaveBoards[octaveIndex]->keyMiniDisplay[keyOctaveIndex];
		key->setSize(keyWidth, keyHeight);
		key->setCentrePosition(centre);
		key->setKeyGraphics(keyShapeGraphic, keyShadowGraphic);


		if (keyOctaveIndex + 1 == TERPSTRABOARDSIZE)
		{
			octaveBoards[octaveIndex]->rightPos = key->getRight();
			octaveIndex++;

			if (octaveIndex < NUMBEROFBOARDS)
				octaveBoards[octaveIndex]->leftPos = key->getRight();
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

