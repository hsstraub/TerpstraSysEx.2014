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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "ScaleStructureComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ScaleStructureComponent::ScaleStructureComponent (ScaleStructure& scaleStructureIn, Array<Colour>& colourTableIn)
    : scaleStructure(scaleStructureIn), colourTable(colourTableIn)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    circleComponent.reset (new GroupingCircle (scaleStructure, colourTable));
    addAndMakeVisible (circleComponent.get());
    circleComponent->setName ("circleComponent");


    //[UserPreSize]
	// Set up components
	periodSlider.reset(new NumberSelector("Period"));
	addAndMakeVisible(periodSlider.get());
	periodSlider->showNameLabel();
	periodSlider->setRange(5, 275, true, false);
	periodSlider->addListener(this);

	generatorSlider.reset(new NumberSelector("Generator", NumberSelector::SelectionType::List));
	addAndMakeVisible(generatorSlider.get());
	generatorSlider->showNameLabel();
	generatorSlider->addListener(this);

	generatorLookAndFeel.reset(new TransparentDropDown());
	// TODO: add SSC colour ids
	//generatorLookAndFeel.setBaseColour(findColour(ColourIds::backgroundColourId));
	generatorLookAndFeel->setColour(PopupMenu::ColourIds::backgroundColourId, Colour());
	generatorSlider->setLookAndFeel(generatorLookAndFeel.get());

	generatorValueLbl.reset(new Label("generatorValueLbl"));
	addAndMakeVisible(generatorValueLbl.get());
	generatorValueLbl->setFont(Font(16.0f, Font::plain).withTypefaceStyle("Regular"));
	generatorValueLbl->setJustificationType(Justification::centred);
	generatorValueLbl->setEditable(false, false, false);
	generatorValueLbl->setColour(TextEditor::textColourId, Colours::black);
	generatorValueLbl->setColour(TextEditor::backgroundColourId, Colour(0x00000000));
	generatorValueLbl->setInterceptsMouseClicks(false, false);

	sizeSelector.reset(new NumberSelector("Scale Size", NumberSelector::SelectionType::List, colourTable[0].contrasting(0.8f)));
	//sizeSelector->setColour(NumberSelector::ColourIds::valueTextColourId, colourTable[0].contrasting(1.0));
	/*sizeSelector->setColour(NumberSelector::ColourIds::, colourTable[0].contrasting(1.0));*/
	addAndMakeVisible(sizeSelector.get());
	sizeSelector->showNameLabel();
	sizeSelector->addListener(this);

	sizeLookAndFeel.reset(new TransparentDropDown());
	sizeLookAndFeel->setBaseColour(colourTable.getReference(0));
	sizeLookAndFeel->setColour(PopupMenu::ColourIds::backgroundColourId, Colour());
	sizeLookAndFeel->setColour(ComboBox::ColourIds::textColourId, sizeSelector->findColour(NumberSelector::ColourIds::valueTextColourId));
	sizeSelector->setLookAndFeel(sizeLookAndFeel.get());

	stepSizePatternLbl.reset(new Label("stepSizePatternLbl"));
	addAndMakeVisible(stepSizePatternLbl.get());
	stepSizePatternLbl->setFont(Font(16.0f, Font::plain).withTypefaceStyle("Regular"));
	stepSizePatternLbl->setJustificationType(Justification::centred);
	stepSizePatternLbl->setEditable(false, false, false);
	stepSizePatternLbl->setColour(TextEditor::textColourId, Colours::black);
	stepSizePatternLbl->setColour(TextEditor::backgroundColourId, Colour(0x00000000));
	stepSizePatternLbl->setInterceptsMouseClicks(false, false);

	offsetLabel.reset(new Label("offsetLabel"));
	offsetLabel->setJustificationType(Justification::centred);
	offsetLabel->setColour(Label::ColourIds::textColourId, Colours::white);
	addAndMakeVisible(offsetLabel.get());

	// TODO: add SSC component colour ids
	//periodFactorLookAndFeel.setBaseColour(findColour(ColourIds::backgroundColourId));
	periodFactorButton.reset(new ShapeButton("periodFactorButton", Colours::white, Colours::white.contrasting(0.125f), Colours::white.contrasting(0.25f)));
	periodFactorButton->setTooltip("Number of periods.");
	addChildComponent(periodFactorButton.get());
	periodFactorButton->addListener(this);

	periodFactorLookAndFeel.reset(new TransparentDropDown());
	periodFactorMenu.setLookAndFeel(periodFactorLookAndFeel.get());

	circle = dynamic_cast<GroupingCircle*>(circleComponent.get());
	circle->addListener(this);

	loadScaleStructureSettings();
    //[/UserPreSize]

    setSize (600, 600);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ScaleStructureComponent::~ScaleStructureComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	listeners.clear();
    //[/Destructor_pre]

    circleComponent = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	generatorSlider = nullptr;
	periodSlider = nullptr;
	generatorValueLbl = nullptr;
	stepSizePatternLbl = nullptr;
	sizeSelector = nullptr;
	periodFactorMenu.setLookAndFeel(nullptr);
    //[/Destructor]
}

//==============================================================================
void ScaleStructureComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..

	// Offset Label arrows
	g.setColour(Colours::white);
	PathStrokeType strokeType(1.0f);
	g.strokePath(offsetArrows, strokeType);
    //[/UserPaint]
}

void ScaleStructureComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    circleComponent->setBounds (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f));
    //[UserResized] Add your own custom resize handling here..

	// TODO: implement (probably ex-projucer) this so that the bounds don't have to be set twice
	periodSlider->setSize(proportionOfWidth(0.25f), proportionOfHeight(0.15f));
	periodSlider->setCentrePosition(circle->getIntPointFromCenter(circle->getInnerRadius() * 0.4f, 0));

	generatorSlider->setSize(proportionOfWidth(0.25f), proportionOfHeight(0.15f));
	generatorSlider->setCentrePosition(circle->getIntPointFromCenter(circle->getInnerRadius() * 0.125f, float_Pi));

	generatorValueLbl->setCentrePosition(circle->getIntPointFromCenter(circle->getInnerRadius() * 2.0f / 3.0f, float_Pi * 11.0f / 8.0f));
	stepSizePatternLbl->setCentrePosition(circle->getIntPointFromCenter(circle->getInnerRadius() * 2.0f / 3.0f, float_Pi * 5.0f / 8.0f));

	offsetLabel->setFont(Font().withHeight(getHeight() / 48.0f));
	offsetLabel->setSize(offsetLabel->getFont().getStringWidth("Offset") * 2, offsetLabel->getFont().getHeight() * 3);
	offsetLabel->setCentrePosition(circle->getIntPointFromCenter(circle->getInnerRadius() * 7.0f / 8.0f, 0));

	sizeSelector->setSize(proportionOfWidth(0.175f), (circle->getOuterRadius() - circle->getMiddleRadius()) * 7.0f / 8.0f);
	sizeSelector->setCentrePosition(circle->getIntPointFromCenter((circle->getOuterRadius() + circle->getMiddleRadius()) / 2.025f, 0));

	// TODO: add other part of arrow / improve drawing
	offsetArrows.clear();
	GroupingCircle::addArcToPath(offsetArrows, circle->getInnerCircleBounds().reduced(circle->getInnerRadius() / 13.0f), float_Pi / 24, float_Pi / 12, true);
	offsetArrows.lineTo(circle->getFloatPointFromCenter(circle->getInnerRadius() * 13.0f / 14.0f, float_Pi / 14));
	GroupingCircle::addArcToPath(offsetArrows, circle->getInnerCircleBounds().reduced(circle->getInnerRadius() / 13.0f), -float_Pi / 24, -float_Pi / 12, true);
	offsetArrows.lineTo(circle->getFloatPointFromCenter(circle->getInnerRadius() * 13.0f / 14.0f, -float_Pi / 14));

	float periodFBtnSize = periodSlider->getHeight() / 8.0f;
	periodFactorButtonShape.clear();
	periodFactorButtonShape.addEllipse(periodSlider->getRight(), periodSlider->getY(), periodFBtnSize, periodFBtnSize);
	periodFactorButton->setShape(periodFactorButtonShape, true, true, true);
	periodFactorButton->setTopLeftPosition(periodSlider->getPosition().translated(periodSlider->getWidth() * 4 / 5.0f, 0));

	generatorValueLbl->setSize(getWidth(), proportionOfHeight(0.15f));
	generatorValueLbl->setCentrePosition(circle->getIntPointFromCenter(circle->getInnerRadius() * 3.0f / 7.0f, float_Pi));

	stepSizePatternLbl->setSize(getWidth(), proportionOfHeight(0.15f));
	stepSizePatternLbl->setCentrePosition(circle->getIntPointFromCenter(circle->getInnerRadius() * 4.0 / 7.0f, float_Pi));

    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void ScaleStructureComponent::paintOverChildren(Graphics& g)
{
	// Period Factor label
	if (periodFactorButton->isVisible())
	{
		g.setColour(Colours::black);
		g.setFont(periodFactorButton->getHeight() * 7 / 8.0f );
		g.drawFittedText(String(scaleStructure.getPeriodFactor()), periodFactorButton->getBounds().translated(0, -1), Justification::centred, 1);
	}
}

void ScaleStructureComponent::buttonClicked(Button* buttonThatWasClicked)
{
	if (buttonThatWasClicked == periodFactorButton.get())
	{
		periodFactorMenu.clear();
		for (int i = 0; i < periodFactors.size(); i++)
		{
			periodFactorMenu.addItem(i + 1, String(periodFactors[i]), true, i == scaleStructure.getPeriodFactorIndex());
		}

		PopupMenu::Options options = PopupMenu::Options()
			.withMinimumWidth(periodSlider->getHeight() / 4)
			.withStandardItemHeight(periodSlider->getHeight() / 4)
			.withTargetComponent(periodFactorButton.get());

		periodFactorMenu.showMenuAsync(options, [=](int choice)
		{
			if (choice > 0)
				onPeriodFactorChange(choice - 1);
		});
	}
}

void ScaleStructureComponent::selectorValueChanged(NumberSelector* selectorThatHasChanged)
{
	if (selectorThatHasChanged == periodSlider.get())
	{
		onPeriodChange();
	}

	else if (selectorThatHasChanged == generatorSlider.get())
	{
		generatorSelected = generatorSlider->getIndex();
		DBG("\nSSC: Generator index set to: " + String(generatorSelected));
		scaleStructure.setAll(periodSelected, generatorSelected, -1, generatorOffset, periodFactorSelected);

		generatorOffset = scaleStructure.getGeneratorOffset();
		generatorCents = degreeCents * scaleStructure.getGenerator();

		updatePGLabel();
		updateScaleSizes();
	}

	else if (selectorThatHasChanged == sizeSelector.get())
	{
		DBG("\nSSC: Size index set to: " + String(sizeSelector->getIndex() + 1));
		scaleStructure.setSizeIndex(sizeSelector->getIndex() + 1);
	}

	listeners.call(&ScaleStructureComponent::Listener::scaleStructureStepSizesChanged,
		scaleStructure.getStepSize().x,
		scaleStructure.getStepSize().y);

	generatorOffset = scaleStructure.getGeneratorOffset();
	circle->updateGenerator();

	updateLsLabel();
	listeners.call(&ScaleStructureComponent::Listener::scaleStructureChanged);
}

void ScaleStructureComponent::offsetChanged(int newOffset)
{
	DBG("\nSSC: Generator Offset set to: " + String(newOffset));
	generatorOffset = newOffset;
	scaleStructure.setGeneratorOffset(generatorOffset);

	updateOffsetLabel();
	updateLsLabel();
	listeners.call(&ScaleStructureComponent::Listener::scaleStructureChanged);
}

void ScaleStructureComponent::degreeAltered(int degreeIndex, int chromaAmount)
{
	if (chromaAmount != 0)
		scaleStructure.setAlterationOfDegree(degreeIndex, chromaAmount);
	else
		scaleStructure.resetAlterationOfDegree(degreeIndex);

	DBG("\nSSC: Degree swapped: " + String(degreeIndex) + " Chromas: " + String(chromaAmount));
	circle->updateGenerator();
	listeners.call(&ScaleStructureComponent::Listener::scaleStructureChanged);
}

void ScaleStructureComponent::loadScaleStructureSettings()
{
	if (scaleStructure.isValid())
	{
		periodSelected = scaleStructure.getPeriod();

		periodSlider->setValue(periodSelected, false);
		periodCents = log2f(periodRatio / scaleStructure.getPeriodFactor()) * 1200;
		degreeCents = periodCents / periodSelected;

		updatePeriodFactors();
		updateGenerators();
		updatePGLabel();
		updateScaleSizes();
		updateLsLabel();
		updateOffsetLabel();

		circle->updatePeriod();
		circle->updateGenerator();
	}
}

void ScaleStructureComponent::updateGenerators()
{
	generatorSlider->setList(scaleStructure.getValidGenerators(), false, false);

	generatorSelected = scaleStructure.getGeneratorIndex();
	generatorSlider->setIndex(generatorSelected, false);

	generatorCents = degreeCents * scaleStructure.getGenerator();
}

void ScaleStructureComponent::updateScaleSizes()
{
	Array<int> sizes = scaleStructure.getScaleSizes();
	sizes.remove(0); // First size is redundant size of 1
	if (scaleStructure.getPeriodFactor() > 1)
	{
		for (int i = 0; i < sizes.size(); i++)
		{
			sizes.set(i, sizes[i] * scaleStructure.getPeriodFactor());
		}
	}

	sizeSelector->setList(sizes, false, false);
	sizeSelector->setIndex(jlimit(0, sizes.size(), scaleStructure.getScaleSizeIndex() - 1), false);
}

void ScaleStructureComponent::updatePeriodFactors()
{
	periodFactors = scaleStructure.getPeriodFactors();
	periodFactorSelected = scaleStructure.getPeriodFactorIndex();

	if (periodFactors.size() == 1)
		periodFactorButton->setVisible(false);
	else
		periodFactorButton->setVisible(true);
}

void ScaleStructureComponent::setPeriod(int newPeriod)
{
	periodSlider->setValue(newPeriod);
}

void ScaleStructureComponent::onPeriodChange(bool sendNotification)
{
	periodSelected = periodSlider->getValue();
	DBG("\nSSC: Period set to: " + String(periodSelected));
	scaleStructure.setAll(periodSelected, -1, -1, generatorOffset, 0);
	updatePeriodFactors();

	periodCents = log2f(periodRatio / scaleStructure.getPeriodFactor()) * 1200;
	degreeCents = periodCents / scaleStructure.getPeriod();

	updateGenerators();
	updateScaleSizes();
	updatePGLabel();

	circle->updatePeriod();

	if (sendNotification)
		listeners.call(&ScaleStructureComponent::Listener::scaleStructurePeriodChanged, periodSelected);
}

void ScaleStructureComponent::onPeriodFactorChange(int factorIndexIn)
{
	DBG("\nSSC: Period factor index set to: " + String(factorIndexIn));
	scaleStructure.setAll(scaleStructure.getPeriod(), -1, -1, generatorOffset, factorIndexIn);
	periodFactorSelected = scaleStructure.getPeriodFactorIndex();
	generatorOffset = scaleStructure.getGeneratorOffset();

	periodCents = log2f(periodRatio) * 1200 / scaleStructure.getPeriodFactor();

	updateGenerators();
	updateScaleSizes();
	updateOffsetLabel();
	updatePGLabel();
	circle->updateGenerator();

	listeners.call(&ScaleStructureComponent::Listener::scaleStructureChanged);
}

void ScaleStructureComponent::onGeneratorChange(bool sendNotifcation)
{

}

void ScaleStructureComponent::onScaleSizeChange(bool sendNotification)
{

}

void ScaleStructureComponent::updatePGLabel()
{
	generatorValueLbl->setText(String(roundToNDecimals(generatorCents, 3)) + "c | " + String(roundToNDecimals(periodCents, 3)) + "c", dontSendNotification);
}

void ScaleStructureComponent::updateLsLabel()
{
	stepSizePatternLbl->setText(scaleStructure.getLsSteps(), dontSendNotification);
}

void ScaleStructureComponent::updateOffsetLabel()
{
	offsetLabel->setText("Offset\n" + String(scaleStructure.getGeneratorOffset()), dontSendNotification);
}

void ScaleStructureComponent::addListener(Listener* listenerIn)
{
	listeners.add(listenerIn);
}

void ScaleStructureComponent::removeListener(Listener* listenerIn)
{
	listeners.remove(listenerIn);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ScaleStructureComponent"
                 componentName="" parentClasses="public Component, private NumberSelector::Listener, private GroupingCircle::Listener, private Button::Listener"
                 constructorParams="ScaleStructure&amp; scaleStructureIn, Array&lt;Colour&gt;&amp; colourTableIn"
                 variableInitialisers="scaleStructure(scaleStructureIn), colourTable(colourTableIn)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="600">
  <BACKGROUND backgroundColour="ffffff"/>
  <GENERICCOMPONENT name="circleComponent" id="ec9c5dc09c2f91cf" memberName="circleComponent"
                    virtualName="" explicitFocusOrder="0" pos="0 0 100% 100%" class="GroupingCircle"
                    params="scaleStructure, colourTable"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

