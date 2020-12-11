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
#include "Main.h"
//[/Headers]

#include "VelocityCurveDlgBase.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
VelocityCurveDlgBase::VelocityCurveDlgBase (TerpstraMidiDriver::VelocityCurveType typeValue)
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
    cbEditMode->setTextWhenNothingSelected (TRANS("Select drawing mode"));
    cbEditMode->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cbEditMode->addItem (TRANS("Free drawing"), 1);
    cbEditMode->addItem (TRANS("Linear"), 2);
    cbEditMode->addItem (TRANS("Quadratic"), 3);
    cbEditMode->addListener (this);

    cbEditMode->setBounds (8, 8, 296, 24);

    labelCurrentBeamValue.reset (new juce::Label ("labelCurrentBeamValue",
                                                  TRANS("127")));
    addAndMakeVisible (labelCurrentBeamValue.get());
    labelCurrentBeamValue->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelCurrentBeamValue->setJustificationType (juce::Justification::centredLeft);
    labelCurrentBeamValue->setEditable (false, false, false);
    labelCurrentBeamValue->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelCurrentBeamValue->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelCurrentBeamValue->setBounds (0, 40, 31, 16);

    labelCurrentXPos.reset (new juce::Label ("labelCurrentXPos",
                                             TRANS("127")));
    addAndMakeVisible (labelCurrentXPos.get());
    labelCurrentXPos->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelCurrentXPos->setJustificationType (juce::Justification::centredLeft);
    labelCurrentXPos->setEditable (false, false, false);
    labelCurrentXPos->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelCurrentXPos->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelCurrentXPos->setBounds (40, 40, 31, 16);


    //[UserPreSize]

	drawingStrategies[TerpstraVelocityCurveConfig::freeDrawing] = &freeDrawingStrategy;
	drawingStrategies[TerpstraVelocityCurveConfig::linearSegments] = &linearDrawingStrategy;
	drawingStrategies[TerpstraVelocityCurveConfig::quadraticCurves] = &quadraticDrawingStrategy;

	for (int x = 0; x < 128; x++)
	{
		velocityBeamTable[x].reset(new  VelocityCurveBeam());
		addAndMakeVisible(velocityBeamTable[x].get());
		velocityBeamTable[x]->addMouseListener(this, true);
	}

    //[/UserPreSize]

    setSize (320, 160);


    //[Constructor] You can add your own custom stuff here..
	labelCurrentBeamValue->setVisible(false);
	labelCurrentXPos->setVisible(false);

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
    labelCurrentBeamValue = nullptr;
    labelCurrentXPos = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	deleteAllChildren();
    //[/Destructor]
}

//==============================================================================
void VelocityCurveDlgBase::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
	g.fillAll(findColour(ResizableWindow::backgroundColourId));

	g.setColour(findColour(VelocityCurveBeam::outlineColourId));
	g.strokePath(beamTableFrame, PathStrokeType(1.000f));

	auto currentDrawingStrategy = getCurrentDrawingStrategy();
	if (currentDrawingStrategy != nullptr)
	{
		//lblDescription->setText(currentCurveEditStrategy->getDescriptionText(), juce::NotificationType::dontSendNotification);
		currentDrawingStrategy->paint(g, getLookAndFeel());
	}
    //[/UserPaint]
}

void VelocityCurveDlgBase::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	int w = this->getWidth();
	int h = this->getHeight();
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..

	float graphicsXPadding = cbEditMode->getX();
	float graphicsYPos = cbEditMode->getBottom() + BEAMTABLERIMABOVE;
	float graphicsBottom = h - labelCurrentXPos->getHeight();

	beamTableFrame.clear();
	beamTableFrame.startNewSubPath(graphicsXPadding, graphicsYPos);
	beamTableFrame.lineTo(graphicsXPadding, graphicsBottom);
	beamTableFrame.lineTo(w - graphicsXPadding, graphicsBottom);
	beamTableFrame.lineTo(w - graphicsXPadding, graphicsYPos);
	beamTableFrame.closeSubPath();

	auto currentDrawingStrategy = getCurrentDrawingStrategy();
	if (currentDrawingStrategy != nullptr)
		currentDrawingStrategy->resized();

	float velocityGraphicsHeight = graphicsBottom - graphicsYPos;
	float velocityBeamXPos = graphicsXPadding;
	for (int x = 0; x < 128; x++)
	{
		auto velocityBeamWidth = beamWidth(x);
		velocityBeamTable[x]->setBounds(velocityBeamXPos, graphicsYPos, velocityBeamWidth, velocityGraphicsHeight);
		velocityBeamXPos += velocityBeamWidth;
	}

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

void VelocityCurveDlgBase::loadFromMapping()
{
	auto configInEdit = getConfigInEdit();

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

bool VelocityCurveDlgBase::showBeamValueOfMousePosition(juce::Point<float> localPoint)
{
	if (beamTableFrame.contains(localPoint))
	{
        int beamTableLeft = velocityBeamTable[0]->getX();
        int beamTableRight = velocityBeamTable[127]->getRight();
        int xpos = (localPoint.x - beamTableLeft) * 128 / (beamTableRight - beamTableLeft);
        if (xpos >= 0 && xpos < 128)
        {
			// Show the field with the current beam value
			labelCurrentBeamValue->setVisible(true);
			labelCurrentBeamValue->setBounds(
				jmin(roundToInt(localPoint.x), getWidth() - labelCurrentBeamValue->getWidth()),
				localPoint.y - labelCurrentBeamValue->getHeight(),
				labelCurrentBeamValue->getWidth(),
				labelCurrentBeamValue->getHeight());

			// Value
			labelCurrentBeamValue->setText(beamValueText(velocityBeamTable[0]->getBeamValueFromLocalPoint(localPoint)), juce::NotificationType::sendNotification);

			// Show x value (beam position)
			labelCurrentXPos->setVisible(true);
			labelCurrentXPos->setBounds(
				jmin(roundToInt(localPoint.x), getWidth() - labelCurrentXPos->getWidth()),
				velocityBeamTable[0]->getBottom(),
                labelCurrentXPos->getWidth(),
                labelCurrentXPos->getHeight());
            labelCurrentXPos->setText(beamXPosText(xpos), juce::NotificationType::sendNotification);

			return true;
        }
        else
        {
            labelCurrentBeamValue->setVisible(false);
            labelCurrentXPos->setVisible(false);

			return false;
        }
	}
	else
    {
		// Hide fields
		labelCurrentBeamValue->setVisible(false);
		labelCurrentXPos->setVisible(false);

		return false;
    }
}

void VelocityCurveDlgBase::mouseMove(const MouseEvent &event)
{
	juce::Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	bool doRepaint = showBeamValueOfMousePosition(localPoint);

	auto currentDrawingStrategy = getCurrentDrawingStrategy();
	if (currentDrawingStrategy != nullptr)
		doRepaint |= currentDrawingStrategy->mouseMove(event, localPoint);

	if (doRepaint)
		repaint();
}

void VelocityCurveDlgBase::mouseDown(const MouseEvent &event)
{
	juce::Point<float> localPoint = getLocalPoint(event.eventComponent, event.position);

	showBeamValueOfMousePosition(localPoint);

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

	showBeamValueOfMousePosition(localPoint);

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
	case TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff:
		return &mappingInEdit->noteOnOffVelocityCurveConfig;

	case TerpstraMidiDriver::VelocityCurveType::fader:
		return &mappingInEdit->faderConfig;
	case TerpstraMidiDriver::VelocityCurveType::afterTouch:
		return &mappingInEdit->afterTouchConfig;
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
  <LABEL name="labelCurrentBeamValue" id="5ddce68a8155d39e" memberName="labelCurrentBeamValue"
         virtualName="" explicitFocusOrder="0" pos="0 40 31 16" edTextCol="ff000000"
         edBkgCol="0" labelText="127" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="labelCurrentXPos" id="f2fe328e83d5b29" memberName="labelCurrentXPos"
         virtualName="" explicitFocusOrder="0" pos="40 40 31 16" edTextCol="ff000000"
         edBkgCol="0" labelText="127" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

