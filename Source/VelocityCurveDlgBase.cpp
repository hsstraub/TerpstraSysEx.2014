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
#include "Main.h"
//[/Headers]

#include "VelocityCurveDlgBase.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VelocityCurveDlgBase::VelocityCurveDlgBase (TerpstraVelocityCurveConfig::VelocityCurveType typeValue)
    : freeDrawingStrategy(beamTableFrame, velocityBeamTable),
      linearDrawingStrategy(beamTableFrame, velocityBeamTable),
      quadraticDrawingStrategy(beamTableFrame, velocityBeamTable)
{
    //[Constructor_pre] You can add your own custom stuff here..
	velocityCurveType = typeValue;
    //[/Constructor_pre]

    cbEditMode.reset (new juce::ComboBox ("cbEditMode"));
    addAndMakeVisible (cbEditMode.get());
    cbEditMode->setEditableText (false);
    cbEditMode->setJustificationType (juce::Justification::centredLeft);
    cbEditMode->setTextWhenNothingSelected (translate("Draw Mode"));
    cbEditMode->setTextWhenNoChoicesAvailable (translate("(no choices)"));
    cbEditMode->addItem (translate("Free"), 1);
    cbEditMode->addItem (translate("Linear"), 2);
    cbEditMode->addItem (translate("Quadratic"), 3);
    cbEditMode->addListener (this);

    cbEditMode->setBounds (8, 8, 296, 24);

    //[UserPreSize]
	cbEditMode->getProperties().set(LumatoneEditorStyleIDs::roundedDiagonalCorners, 0);

    drawingStrategies[TerpstraVelocityCurveConfig::freeDrawing] = &freeDrawingStrategy;
	drawingStrategies[TerpstraVelocityCurveConfig::linearSegments] = &linearDrawingStrategy;
	drawingStrategies[TerpstraVelocityCurveConfig::quadraticCurves] = &quadraticDrawingStrategy;

	for (int x = 0; x < 128; x++)
	{
		velocityBeamTable[x].reset(new  VelocityCurveBeam());
		addChildComponent(velocityBeamTable[x].get());
		velocityBeamTable[x]->addMouseListener(this, true);
	}

    //[/UserPreSize]

    setSize (320, 160);


    //[Constructor] You can add your own custom stuff here..

	// Initialize velocity lookup table
	for (int x = 0; x < 128; x++)
		velocityBeamTable[x]->setValue(x);

    //[/Constructor]
}

VelocityCurveDlgBase::~VelocityCurveDlgBase()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    for (int x = 0; x < 128; x++) {
		velocityBeamTable[x] = nullptr;
	}
    //[/Destructor_pre]

    cbEditMode = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	deleteAllChildren();
    //[/Destructor]
}

//==============================================================================
void VelocityCurveDlgBase::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(backgroundColour);

	int h = getHeight();

	// Build contour path
	beamTableContour.clear();
	beamTableContour.startNewSubPath(0, h);
	
	for (int x = 0; x < 128; x++)
		beamTableContour.lineTo(velocityBeamTable[x]->getX(), h - velocityBeamTable[x]->getBeamHeightFromValue());
	
	beamTableContour.lineTo(getWidth(), h);
	beamTableContour.closeSubPath();

	// Fill velocity contour
	g.setGradientFill(beamColourGradient);
	g.fillPath(beamTableContour);

	//g.setColour(findColour(VelocityCurveBeam::outlineColourId));
	//g.strokePath(beamTableFrame, PathStrokeType(1.000f));

	auto currentDrawingStrategy = getCurrentDrawingStrategy();
	if (currentDrawingStrategy != nullptr)
	{
		for (int x = 0; x < 128; x++)
		{
			auto beam = velocityBeamTable[x].get();
			beam->setTooltip(currentDrawingStrategy->getDescriptionText());
		}
		currentDrawingStrategy->paint(g, getLookAndFeel());
	}

	// Draw grid overlay
	g.setColour(gridColour);
	g.strokePath(beamTableGrid, PathStrokeType(1.0f));
    //[/UserPaint]
}

void VelocityCurveDlgBase::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	int w = this->getWidth();
	int h = this->getHeight();

	// Grid units
	float xUnit = w * CURVEGRIDXRESOLUTION;
	float x = xUnit;
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..

	//float graphicsXPadding = cbEditMode->getX();
	//float graphicsYPos = cbEditMode->getBottom() + BEAMTABLERIMABOVE;
	//float graphicsBottom = h - BEAMTABLERIMABOVE;

	//beamTableFrame.clear();
	//beamTableFrame.startNewSubPath(graphicsXPadding, graphicsYPos);
	//beamTableFrame.lineTo(graphicsXPadding, graphicsBottom);
	//beamTableFrame.lineTo(w - graphicsXPadding, graphicsBottom);
	//beamTableFrame.lineTo(w - graphicsXPadding, graphicsYPos);
	//beamTableFrame.closeSubPath();

	cbEditMode->setBounds(0, 0, xUnit * 11, xUnit * 3);

	auto currentDrawingStrategy = getCurrentDrawingStrategy();
	if (currentDrawingStrategy != nullptr)
		currentDrawingStrategy->resized();

	//float velocityGraphicsHeight = graphicsBottom - graphicsYPos;
	float velocityBeamXPos = 0;
	for (int x = 0; x < 128; x++)
	{
		auto velocityBeamWidth = beamWidth(x);
		velocityBeamTable[x]->setBounds(velocityBeamXPos, 0, velocityBeamWidth, h);
		velocityBeamXPos += velocityBeamWidth;
	}

	// Build grid
	beamTableGrid.clear();
	while (x < w)
	{
		beamTableGrid.addLineSegment(Line<float>(x, 0, x, (float)h), 0.1f);
		x += xUnit;
	}

	float y = h;
	while (xUnit > 0 && y > 0)
	{
		beamTableGrid.addLineSegment(Line<float>(0, y, (float)w, y), 0.1f);
		y -= xUnit;
	}

	beamColourGradient.point1 = { 0.0f, (float)h };
	beamColourGradient.point2 = { velocityBeamXPos, (float)h };

    //[/UserResized]
}

void VelocityCurveDlgBase::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cbEditMode.get())
    {
        //[UserComboBoxCode_cbEditMode] -- add your combo box handling code here..
		int editModeIndex = cbEditMode->getSelectedItemIndex();

		auto configInEdit = getConfigInEdit();
		if (configInEdit != nullptr)
		{
			configInEdit->editStrategy = static_cast<TerpstraVelocityCurveConfig::EDITSTRATEGYINDEX>(editModeIndex);
		}

		// Set edit config according to current values of velocity table, of possible
		auto currentDrawingStrategy = getCurrentDrawingStrategy();
		if (currentDrawingStrategy != nullptr)
			currentDrawingStrategy->setEditConfigFromVelocityTable();

		TerpstraSysExApplication::getApp().setHasChangesToSave(true);

		repaint();
        //[/UserComboBoxCode_cbEditMode]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void VelocityCurveDlgBase::paintOverChildren(juce::Graphics& g)
{
	//int roundedCornerSize = getParentHeight() * ROUNDEDCORNERTOAPPHEIGHT;
	//Rectangle<float> controlsBounds = getLocalBounds().toFloat().reduced(roundedCornerSize);
	//g.setColour(Colour(0xff272b2f));
	//g.drawRoundedRectangle(controlsBounds, roundedCornerSize, 4.0f);
}

void VelocityCurveDlgBase::lookAndFeelChanged()
{
	auto lookAndFeel = dynamic_cast<LumatoneEditorLookAndFeel*>(&getLookAndFeel());
	if (lookAndFeel)
	{
		beamColourGradient.clearColours();
		beamColourGradient.addColour(0.0, lookAndFeel->findColour(LumatoneEditorColourIDs::CurveGradientMin));
		beamColourGradient.addColour(1.0, lookAndFeel->findColour(LumatoneEditorColourIDs::CurveGradientMax));
		
		backgroundColour = lookAndFeel->findColour(LumatoneEditorColourIDs::ControlBoxBackground);
		gridColour = lookAndFeel->findColour(LumatoneEditorColourIDs::CurveGridColour);

		cbEditMode->setColour(ComboBox::ColourIds::backgroundColourId, Colours::black);
		cbEditMode->setColour(ComboBox::ColourIds::textColourId, Colours::white);
	}
}

void VelocityCurveDlgBase::loadFromMapping()
{
	auto defaultConfig = TerpstraVelocityCurveConfig(velocityCurveType);
	auto configInEdit = getConfigInEdit();
	if (configInEdit == nullptr)
		configInEdit = &defaultConfig;

	if (configInEdit != nullptr)
	{
		cbEditMode->setSelectedItemIndex(configInEdit->editStrategy, juce::NotificationType::dontSendNotification);
	}
	else
	{
		cbEditMode->setSelectedItemIndex(TerpstraVelocityCurveConfig::EDITSTRATEGYINDEX::none, juce::NotificationType::dontSendNotification);
	}

	// Set edit config according to current values of velocity table, if possible
	auto currentDrawingStrategy = getCurrentDrawingStrategy();
	if (currentDrawingStrategy != nullptr)
	{
		currentDrawingStrategy->setEditConfig(configInEdit->velocityValues);
	}
	else
	{
		// Initialize velocity lookup table
		for (int x = 0; x < 128; x++)
			velocityBeamTable[x]->setValue(x);
	}
}

void VelocityCurveDlgBase::sendVelocityTableToController()
{
	unsigned char velocityValues[128];

	for (int x = 0; x < 128; x++)
	{
		velocityValues[x] = velocityBeamTable[x]->getValue();
	}

	TerpstraSysExApplication::getApp().getMidiDriver().sendVelocityConfig(velocityCurveType, velocityValues);
}

void VelocityCurveDlgBase::mouseMove(const MouseEvent &event)
{
	juce::Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	auto currentDrawingStrategy = getCurrentDrawingStrategy();
	if (currentDrawingStrategy != nullptr && currentDrawingStrategy->mouseMove(event, localPoint))
		repaint();
}

void VelocityCurveDlgBase::mouseDown(const MouseEvent &event)
{
	juce::Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	auto currentDrawingStrategy = getCurrentDrawingStrategy();
	if (currentDrawingStrategy != nullptr)
	{
		if (currentDrawingStrategy->mouseDown(event, localPoint))
		{
			repaint();
		}
	}
}

void VelocityCurveDlgBase::mouseDrag(const MouseEvent &event)
{
	juce::Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	auto currentDrawingStrategy = getCurrentDrawingStrategy();
	if (currentDrawingStrategy != nullptr)
	{
		if (currentDrawingStrategy->mouseDrag(event, localPoint))
		{
			repaint();
		}
	}
}

void VelocityCurveDlgBase::mouseUp(const MouseEvent &event)
{
	juce::Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	auto currentDrawingStrategy = getCurrentDrawingStrategy();
	if (currentDrawingStrategy != nullptr)
	{
		currentDrawingStrategy->mouseUp(event, localPoint);

		auto configInEdit = getConfigInEdit();
		if (configInEdit != nullptr)
		{
			currentDrawingStrategy->exportEditConfig(configInEdit->velocityValues);
			TerpstraSysExApplication::getApp().setHasChangesToSave(true);
			sendVelocityTableToController();
		}
	}

	repaint();
}

TerpstraKeyMapping*	VelocityCurveDlgBase::getMappingInEdit()
{
	// Security at start of program
	if (getParentComponent() == nullptr)
		return nullptr;
	if (getParentComponent()->getParentComponent() == nullptr)
		return nullptr;
	if (getParentComponent()->getParentComponent()->getParentComponent() == nullptr)
		return nullptr;

	return &(dynamic_cast<MainContentComponent*>(getParentComponent()->getParentComponent()->getParentComponent()))->getMappingInEdit();
}

TerpstraVelocityCurveConfig* VelocityCurveDlgBase::getConfigInEdit()
{
	auto mappingInEdit = getMappingInEdit();
	if(mappingInEdit == nullptr)
		return nullptr;

	switch(velocityCurveType)
	{
	case TerpstraVelocityCurveConfig::VelocityCurveType::noteOnNoteOff:
		return &mappingInEdit->noteOnOffVelocityCurveConfig;

	case TerpstraVelocityCurveConfig::VelocityCurveType::fader:
		return &mappingInEdit->faderConfig;
	case TerpstraVelocityCurveConfig::VelocityCurveType::afterTouch:
		return &mappingInEdit->afterTouchConfig;
	case TerpstraVelocityCurveConfig::VelocityCurveType::lumaTouch:
		return &mappingInEdit->lumaTouchConfig;
	default:
		jassertfalse;
		return nullptr;
	}
}

VelocityCurveEditStrategyBase* VelocityCurveDlgBase::getCurrentDrawingStrategy()
{
	auto configInEdit = getConfigInEdit();
	if (configInEdit == nullptr)
		return nullptr;

	auto strategyItr = drawingStrategies.find(configInEdit->editStrategy);
	if (strategyItr == drawingStrategies.end())
		return nullptr;

	return strategyItr->second;
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="VelocityCurveDlgBase" componentName=""
                 parentClasses="public Component" constructorParams="TerpstraMidiDriver::VelocityCurveType typeValue"
                 variableInitialisers="freeDrawingStrategy(beamTableFrame, velocityBeamTable)&#10;linearDrawingStrategy(beamTableFrame, velocityBeamTable)&#10;quadraticDrawingStrategy(beamTableFrame, velocityBeamTable)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="320" initialHeight="160">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <COMBOBOX name="cbEditMode" id="1f22301dd42b968e" memberName="cbEditMode"
            virtualName="" explicitFocusOrder="0" pos="8 8 296 24" editable="0"
            layout="33" items="Free drawing&#10;Linear&#10;Quadratic" textWhenNonSelected="Select drawing mode"
            textWhenNoItems="(no choices)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

