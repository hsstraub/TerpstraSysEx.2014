/*
  ==============================================================================

    ColourSelectionPanels.h
    Created: 21 Dec 2020 9:48:08pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ColourPaletteComponent.h"
#include "LumatoneEditorLookAndFeel.h"

//==============================================================================
/*
*   Panel that displays the palettes and allows users to select a swatch
*/
class ColourPalettesPanel : public Component
{
public:

    ColourPalettesPanel(
        OwnedArray<PaletteControlGroup>& paletteGroupsIn,
        ColourPaletteComponent* newPaletteIn,
        TextButton* newPaletteBtnIn
    ) :
        palettes(paletteGroupsIn),
        newPalette(newPaletteIn),
        newPaletteBtn(newPaletteBtnIn)
    {
        addMouseListener(this, true);
        setRepaintsOnMouseActivity(true);

        flexBox.flexWrap = FlexBox::Wrap::wrap;
        flexBox.justifyContent = FlexBox::JustifyContent::flexStart;
        rebuildPanel(false);
    };

    // Used to make this component resize itself depending on how many swatches there are
    void setViewUnits(int widthIn, int heightIn)
    {
        viewableWidth = widthIn;
        viewableHeight = heightIn;

        setSize(viewableWidth, getHeightFromNumRows(numRows));
    }

    int getHeightFromNumRows(int numRowsIn)
    {
        return round(viewableHeight * 0.5f * numRowsIn);
    }

    void paint(Graphics& g) override 
    {
        ////Draws rectangles around items and margins 
//        for (auto item : flexBox.items)
//        {
//            g.setColour(Colours::red);
//            g.drawRect(item.currentBounds);
//
//            g.setColour(Colours::green);
//            g.drawRect(item.currentBounds.getX(), item.currentBounds.getBottom(), item.width, item.margin.bottom, 1.0f);
//
//            g.setColour(Colours::yellow);
//            g.drawRect(item.currentBounds.getX() - item.margin.left - 1, item.currentBounds.getY(), item.margin.left - 1, item.currentBounds.getHeight(), 1.0f);
//            g.drawRect(item.currentBounds.getRight() + 1, item.currentBounds.getY(), item.margin.right - 1, item.currentBounds.getHeight(), 1.0f);
//
//            g.setColour(Colours::violet);
//            g.drawRect(item.currentBounds.getX(), item.currentBounds.getY() - item.margin.top, item.width, item.margin.top);
//        }
    };

    void resized() override
    {
        Rectangle<int> viewportBounds(viewableWidth, viewableHeight);

        float horizontalMargin  = viewportBounds.proportionOfWidth(horizontalMarginScalar);
        float itemWidth         = viewportBounds.proportionOfWidth(itemWidthScalar);
        float itemHeight        = viewportBounds.proportionOfWidth(itemHeightScalar);
        float topMargin         = viewportBounds.proportionOfHeight(topMarginScalar);
        float bottomMargin      = viewportBounds.proportionOfHeight(bottomMarginScalar);

        for (int i = 0; i < flexBox.items.size(); i++)
        {
            FlexItem& item = flexBox.items.getReference(i);
            item.width = itemWidth;
            item.height = itemHeight;
            item.margin = FlexItem::Margin(topMargin, horizontalMargin, bottomMargin, horizontalMargin);
        }

        flexBox.performLayout(viewportBounds);
        
        float bottomMarginControlHeight = roundToInt(viewportBounds.proportionOfHeight(btmMarginCtrlScalar));
        float bottomMarginControlSpace  = (bottomMargin - bottomMarginControlHeight) * 0.5f;

        for (int i = 0; i < palettes.size(); i++)
        {
            FlexItem& item = flexBox.items.getReference(i);
            Rectangle<float> bottomMarginBounds(item.currentBounds.getX(), item.currentBounds.getBottom(), itemWidth, bottomMargin);
            int halfItemWidth = bottomMarginBounds.proportionOfWidth(0.5f);

            auto label = paletteLabels[i];
            label->setBounds(item.currentBounds.withTrimmedTop(itemHeight * 0.8f).toNearestInt());

            auto group = palettes.getUnchecked(i);
            group->editButton.setSize(halfItemWidth, bottomMarginControlHeight);
            group->editButton.setTopLeftPosition(bottomMarginBounds.getPosition().translated(0, bottomMarginControlSpace).roundToInt());
            group->trashButton.setBounds(group->editButton.getBounds().translated(halfItemWidth, 0));

            Rectangle<float> controlBounds = Rectangle<float>(item.currentBounds.getTopLeft().translated(-horizontalMargin, -topMargin), bottomMarginBounds.getBottomLeft());
            controlGroupHitBoxes.set(i, controlBounds.toNearestInt());
        }

        newPaletteBtn->setSize(itemWidth, bottomMarginControlHeight);
        newPaletteBtn->setTopLeftPosition(newPalette->getX(), newPalette->getBottom() + bottomMarginControlSpace);
    }

    void mouseMove(const MouseEvent& mouse) override
    {
        int newIndex = -1;
        if (mouse.eventComponent->getProperties().contains("index"))
        {
            newIndex = mouse.eventComponent->getProperties()["index"];
        }
        else
        {
            for (int i = 0; i < controlGroupHitBoxes.size(); i++)
            {
                if (controlGroupHitBoxes[i].contains(mouse.getEventRelativeTo(this).position.roundToInt()))
                {
                    newIndex = i;
                    break;
                }
            }
        }

        if (newIndex >= 0 && newIndex != lastPaletteMouseOver)
        {
            if (lastPaletteMouseOver >= 0 && lastPaletteMouseOver < palettes.size())
            {
                setControlsVisibleForPalette(lastPaletteMouseOver, false);
            }

            if (newIndex < palettes.size())
            {
                setControlsVisibleForPalette(newIndex, true);
                lastPaletteMouseOver = newIndex;
            }
        }
    }

    // Setup panels from scratch
    void rebuildPanel(bool resize = true)
    {
        removeAllChildren();
        flexBox.items.clear();
        
        for (auto label : paletteLabels)
        {
            removeChildComponent(label);
        }
        
        paletteLabels.clear();
        controlGroupHitBoxes.clear();

        // Palettes with colour
        for (int i = 0; i < palettes.size(); i++)
        {
            auto group = palettes.getUnchecked(i);
            
            group->palette->getProperties().set("index", i);
            addAndMakeVisible(group->palette);
            flexBox.items.add(*group->palette);

            group->editButton.getProperties().set("index", i);
            addChildComponent(group->editButton);

            group->trashButton.getProperties().set("index", i);
            addChildComponent(group->trashButton);

            String name = group->palette->getPaletteName();
            auto label = paletteLabels.add(new Label("Label_" + name, name));
            label->setJustificationType(Justification::centred);
            label->getProperties().set("index", i);
            addAndMakeVisible(label);

            controlGroupHitBoxes.add(Rectangle<int>());
        }

        addAndMakeVisible(newPalette);
        flexBox.items.add(*newPalette);

        addAndMakeVisible(newPaletteBtn);

        int rows = ceil((palettes.size() + 1) * 0.333333f);

        // Set height depending on how many rows
        if (resize)
        {
            if (rows != numRows)
                setSize(viewableWidth, getHeightFromNumRows(rows));
            else
                resized();
        }

        numRows = rows;
    }

private:

    void setControlsVisibleForPalette(int paletteIndex, bool areVisible)
    {
        auto group = palettes.getUnchecked(paletteIndex);
        group->editButton.setVisible(areVisible);
        group->trashButton.setVisible(areVisible);
    }

private:

    FlexBox flexBox;

    OwnedArray<PaletteControlGroup>& palettes;
    ColourPaletteComponent* newPalette;
    TextButton* newPaletteBtn;

    OwnedArray<juce::Label> paletteLabels;

    int numRows = 1;
    int viewableWidth = 0;
    int viewableHeight = 0;


    Array<Rectangle<int>> controlGroupHitBoxes;
    int lastPaletteMouseOver = -1;

    const float itemWidthScalar         = 0.28f;
    const float itemHeightScalar        = 0.25f;

    const float topMarginScalar         = 0.042f;
    const float horizontalMarginScalar  = 0.025f;
    const float bottomMarginScalar      = 0.075f;
    const float btmMarginCtrlScalar     = 0.06f;

    const float buttonWidthScalar       = 0.333333f;
    const float buttonHeightScalar      = 0.166667f;

    const float panelLeftMarginWidth    = 0.020833f;
};

//==============================================================================
/*
*   Full RGB colour selector panel
*/
class CustomPickerPanel : public Component,
    public ChangeListener,
    public ColourSelectionBroadcaster
{
public:

    CustomPickerPanel()
    {
        colourPicker.reset(new ColourSelector(
            ColourSelector::ColourSelectorOptions::editableColour
            + ColourSelector::ColourSelectorOptions::showColourAtTop
            + ColourSelector::ColourSelectorOptions::showColourspace
        ));


        addAndMakeVisible(*colourPicker);
        colourPicker->addChangeListener(this);
    }

    ~CustomPickerPanel()
    {
        colourPicker = nullptr;
    }

    void paint(Graphics& g) override {};

    void resized() override
    {
        colourPicker->setBounds(getLocalBounds());
    }

    void setCurrentColour(Colour colourIn)
    {
        colourPicker->setCurrentColour(colourIn, dontSendNotification);
    }

    void changeListenerCallback(ChangeBroadcaster* source) override
    {
        selectorListeners.call(&ColourSelectionListener::colourChangedCallback, this, colourPicker->getCurrentColour());
    }

    //==============================================================================

    Colour getSelectedColour() override
    {
        return colourPicker->getCurrentColour();
    }

    void deselectColour() override {};

private:

    std::unique_ptr<ColourSelector> colourPicker;
};

//==============================================================================
/*
*   Colour palette edtior panel
*/
class PaletteEditPanel    : public Component, 
                            public Button::Listener, 
                            public Label::Listener,
                            public ChangeBroadcaster
{
public:

    PaletteEditPanel(const LumatoneEditorColourPalette& paletteIn)
        : colourPalette(paletteIn)
    {
        colourPicker.reset(new ColourSelector(
              ColourSelector::ColourSelectorOptions::editableColour
            + ColourSelector::ColourSelectorOptions::showColourAtTop
            + ColourSelector::ColourSelectorOptions::showColourspace
        ));
        addAndMakeVisible(*colourPicker);

        paletteControl.reset(new TenHexagonPalette());
        paletteControl->setColourPalette(*colourPalette.palette);
        paletteControl->attachColourSelector(colourPicker.get());
        addAndMakeVisible(*paletteControl);

        editPaletteLabel.reset(new Label("EditPaletteLabel", translate("EditPalette")));
        editPaletteLabel->setJustificationType(Justification::centred);
        addAndMakeVisible(*editPaletteLabel);

        String displayName = colourPalette.name;
        if (displayName == String())
        {
            paletteUnnamed = true;
            displayName = "unnamed";
        }

        paletteNameEditor.reset(new Label("PaletteNameEditor"));
        paletteNameEditor->setJustificationType(Justification::centred);
        paletteNameEditor->setEditable(true);
        paletteNameEditor->addListener(this);
        addAndMakeVisible(*paletteNameEditor);
        paletteNameEditor->setText(displayName, NotificationType::dontSendNotification);

        saveButton.reset(new TextButton("SaveButton", translate("SavePaletteTip")));
        saveButton->setButtonText("Save");
        saveButton->addListener(this);
        addAndMakeVisible(*saveButton);

        cancelButton.reset(new TextButton("CancelButton", translate("CancelPaletteTip")));
        cancelButton->setButtonText("Cancel");
        cancelButton->addListener(this);
        addAndMakeVisible(*cancelButton);
    }

    ~PaletteEditPanel()
    {
        paletteControl = nullptr;
    }

    void paint(Graphics& g) override 
    {
        g.fillAll(Colours::black);
    }

    void resized() override
    {
        float leftWidth = getWidth() * leftColumnWidth;
        float leftCenter = leftWidth * 0.5f;

        resizeLabelWithHeight(editPaletteLabel.get(), proportionOfHeight(editPaletteHeight));
        editPaletteLabel->setCentrePosition(leftCenter, round(editPaletteLabel->getHeight() * 0.5f + proportionOfHeight(editPaletteLabelY)));

        float paletteWidth = proportionOfWidth(paletteWidthScalar);
        float paletteHeight = proportionOfWidth(paletteHeightScalar);
        paletteControl->setSize(paletteWidth, paletteHeight);
        paletteControl->setCentrePosition(leftCenter, round(paletteHeight * 0.5f + proportionOfHeight(paletteY)));

        saveButton->setSize(proportionOfWidth(buttonWidth), proportionOfHeight(buttonHeight));
        saveButton->setCentrePosition(leftCenter, round(saveButton->getHeight() * 0.5f + proportionOfHeight(buttonY)));
        cancelButton->setBounds(saveButton->getBounds().translated(0, saveButton->getHeight() * 1.125f));
        
        colourPicker->setSize(proportionOfWidth(pickerWidth), proportionOfHeight(pickerHeight));
        colourPicker->setTopLeftPosition(leftWidth, round((getHeight() - colourPicker->getHeight()) * 0.5f));

        float leftMargin = colourPicker->getRight() * 0.03f * 0.5f;
        paletteNameEditor->setBounds(Rectangle<int>(
            Point<int>(roundToInt(leftMargin),  paletteControl->getBottom()),
            Point<int>(colourPicker->getX() - leftMargin, saveButton->getY())
        ));
        paletteNameEditor->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, editPaletteLabel->getHeight() / (float) paletteNameEditor->getHeight());

        if (lookAndFeel)
        {
            Font labelFont = (paletteUnnamed)
                ? lookAndFeel->getAppFont(LumatoneEditorFont::GothamNarrowItalic)
                : lookAndFeel->getAppFont(LumatoneEditorFont::GothamNarrowMedium);

            paletteNameEditor->setFont(labelFont);
        }

    }

    void lookAndFeelChanged() override
    {
        lookAndFeel = dynamic_cast<LumatoneEditorLookAndFeel*>(&getLookAndFeel());
        if (lookAndFeel)
        {
            lookAndFeel->setupTextButton(*saveButton);
            lookAndFeel->setupTextButton(*cancelButton);
        }
    }

    //==============================================================================

    void buttonClicked(Button* btn) override
    {
        if (btn == saveButton.get())
        {
            saveRequested = true;
        }

        sendChangeMessage();
    }

    //==============================================================================

    void labelTextChanged(Label* label) override
    {
        if (label == paletteNameEditor.get())
        {
            bool nameIsEmpty = paletteNameEditor->getText() == "";

            if (!nameIsEmpty)
            {
                paletteUnnamed = false;
                colourPalette.name = paletteNameEditor->getText();
            }

            else if (nameIsEmpty && !paletteUnnamed)
            {
                paletteUnnamed = true;
            }

            if (paletteUnnamed)
            {
                paletteNameEditor->setText("unnamed", NotificationType::dontSendNotification);
            }

            resized();
        }
    }

    //==============================================================================

    Array<Colour> getCurrentPalette() const
    {
        return paletteControl->getColourPalette();
    }

    void setSelectedSwatch(int selectedSwatchNumber)
    {
        if (selectedSwatchNumber >= 0 && selectedSwatchNumber < paletteControl->getNumberOfSwatches())
            paletteControl->setSelectedSwatchNumber(selectedSwatchNumber);
    }

    String getPaletteName()
    {
        return colourPalette.name;
    }

    bool wasSaveRequested() const
    {
        return saveRequested;
    }

private:

    LumatoneEditorColourPalette     colourPalette;

    std::unique_ptr<Palette>        paletteControl;
    std::unique_ptr<ColourSelector> colourPicker;

    std::unique_ptr<Label>          editPaletteLabel;
    std::unique_ptr<Label>          paletteNameEditor;
    std::unique_ptr<TextButton>     saveButton;
    std::unique_ptr<TextButton>     cancelButton;

    LumatoneEditorLookAndFeel*      lookAndFeel = nullptr;

    bool saveRequested = false;

    bool paletteUnnamed = false;

    // Drawing constants

    const float leftColumnWidth     = 0.3677f;

    const float editPaletteLabelY   = 0.1212f;
    const float editPaletteHeight   = 0.0606f;

    const float paletteY            = 0.2581f;
    const float paletteWidthScalar  = 0.2727f;
    const float paletteHeightScalar = 0.1574f;

    const float buttonY             = 0.6739f;
    const float buttonWidth         = 0.2208f;
    const float buttonHeight        = 0.0889f;

    const float pickerWidth         = 0.6f;
    const float pickerHeight        = 0.9f;
};
