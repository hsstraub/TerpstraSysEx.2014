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

    ColourPalettesPanel(Array<LumatoneEditorColourPalette> palettesIn, ColourSelectionGroup* colourSelectionGroup = nullptr)
        : selectionGroup(colourSelectionGroup)
    {
        addMouseListener(this, true);
        setRepaintsOnMouseActivity(true);

        newPalette.reset(new ColourPaletteComponent("EmptyPalette"));
        newPaletteBtn.reset(new TextButton("NewButton", translate("NewPaletteTip")));
        newPaletteBtn->setButtonText(translate("NewPalette"));
        newPaletteBtn->getProperties().set(LumatoneEditorStyleIDs::textButtonHyperlinkFlag, 1);
        newPaletteBtn->onClick = [&] { listeners.call(&ColourPalettesPanel::Listener::newPaletteRequested); };

    };

    ~ColourPalettesPanel()
    {
        for (auto palette : allPalettes)
            selectionGroup->removeSelector(palette);

        paletteLabels.clear();
        controlGroups.clear();
        newPaletteBtn = nullptr;
        newPalette = nullptr;
    }

    // Used to make this component resize itself depending on how many swatches there are
    //void setViewUnits(int widthIn, int heightIn)
    //{
    //    viewableWidth = widthIn;
    //    viewableHeight = heightIn;

    //    setSize(viewableWidth, getHeightFromNumRows(numRows));
    //}

    int getHeightFromNumRows(int widthIn, int numRowsIn)
    {
        float rowHeight = widthIn * (itemHeightScalar + topMarginScalar + bottomMarginScalar);
        auto height = roundToInt(numRowsIn * rowHeight);
        return height;
    }

    void paint(Graphics& g) override 
    {
        //Draws rectangles around items and margins 
        //for (auto item : dbgItems)
        //{
        //    g.setColour(Colours::red);
        //    g.drawRect(item.currentBounds);

        //    g.setColour(Colours::green);
        //    g.drawRect(item.currentBounds.getX(), item.currentBounds.getBottom(), item.width, item.margin.bottom, 1.0f);

        //    g.setColour(Colours::yellow);
        //    g.drawRect(item.currentBounds.getX() - item.margin.left - 1, item.currentBounds.getY(), item.margin.left - 1, item.currentBounds.getHeight(), 1.0f);
        //    g.drawRect(item.currentBounds.getRight() + 1, item.currentBounds.getY(), item.margin.right - 1, item.currentBounds.getHeight(), 1.0f);

        //    g.setColour(Colours::violet);
        //    g.drawRect(item.currentBounds.getX(), item.currentBounds.getY() - item.margin.top, item.width, item.margin.top);
        //}
    };

    void resized() override
    {
        Rectangle<int> viewportBounds(getWidth(), viewableHeight);
        FlexBox flexBox(FlexBox::Direction::row, FlexBox::Wrap::wrap, FlexBox::AlignContent::flexStart, FlexBox::AlignItems::center, FlexBox::JustifyContent::flexStart);

        float itemWidth         = viewportBounds.proportionOfWidth(itemWidthScalar);
        float itemHeight        = viewportBounds.proportionOfWidth(itemHeightScalar);
        float topMargin         = viewportBounds.proportionOfWidth(topMarginScalar);
        float bottomMargin      = viewportBounds.proportionOfWidth(bottomMarginScalar);
        float horizontalMargin  = (viewportBounds.getWidth() - (3 * itemWidth)) * 0.143f;

        for (auto palette : allPalettes)
        {
            FlexItem item(itemWidth, itemHeight, *palette);
            item.margin = FlexItem::Margin(topMargin, horizontalMargin, bottomMargin, horizontalMargin);
            flexBox.items.add(item);
        }

        flexBox.performLayout(viewportBounds);
        
        float bottomMarginControlHeight = roundToInt(viewportBounds.proportionOfHeight(btmMarginCtrlScalar));
        float bottomMarginControlSpace  = (bottomMargin - bottomMarginControlHeight) * 0.5f;

        for (int i = 0; i < controlGroups.size(); i++)
        {
            FlexItem& item = flexBox.items.getReference(i + 1);
            Rectangle<float> bottomMarginBounds(item.currentBounds.getX(), item.currentBounds.getBottom(), itemWidth, bottomMargin);
            int halfItemWidth = bottomMarginBounds.proportionOfWidth(0.5f);

            auto label = paletteLabels[i];
            label->setBounds(item.currentBounds.withTrimmedTop(itemHeight * 0.8f).toNearestInt());

            auto group = controlGroups.getUnchecked(i);
            group->getEditButton()->setSize(halfItemWidth, bottomMarginControlHeight);
            group->getEditButton()->setTopLeftPosition(bottomMarginBounds.getPosition().translated(0, bottomMarginControlSpace).roundToInt());
            group->getTrashButton()->setBounds(group->getEditButton()->getBounds().translated(halfItemWidth, 0));

            Rectangle<float> controlBounds = Rectangle<float>(item.currentBounds.getTopLeft().translated(-horizontalMargin, -topMargin), bottomMarginBounds.getBottomRight());
            controlGroupHitBoxes.set(i, controlBounds.toNearestInt());
        }

        bottom = controlGroupHitBoxes.getLast().getBottom();

        dbgItems = flexBox.items;

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

        if (newIndex != lastPaletteMouseOver && lastPaletteMouseOver >= 0 && lastPaletteMouseOver < controlGroups.size())
            setControlsVisibleForPalette(lastPaletteMouseOver, false);

        if (newIndex >= 0 && newIndex < controlGroups.size())
        {
            setControlsVisibleForPalette(newIndex, true);
            lastPaletteMouseOver = newIndex;
        }
    }

    // Setup panels from scratch
    void rebuildPanel(Array<LumatoneEditorColourPalette> palettesIn, int width = 0, bool resize = true)
    {
        for (auto group : controlGroups)
            selectionGroup->removeSelector(group->getPaletteComponent());

        removeAllChildren();
        addAndMakeVisible(newPaletteBtn.get());
        addAndMakeVisible(newPalette.get());

        controlGroups.clear();
        paletteLabels.clear();
        controlGroupHitBoxes.clear();
        allPalettes.clear();

        // Palettes with colour
        for (int i = 0; i < palettesIn.size(); i++)
        {
            auto group = controlGroups.add(new PaletteControlGroup(palettesIn.getReference(i)));

            auto paletteComponent = group->getPaletteComponent();
            paletteComponent->getProperties().set("index", i);
            addAndMakeVisible(paletteComponent);

            if (selectionGroup)
                selectionGroup->addSelector(paletteComponent);

            group->getEditButton()->getProperties().set("index", i);
            group->getEditButton()->onClick = [&, i, paletteComponent] { listeners.call(&ColourPalettesPanel::Listener::editPaletteRequested, i, paletteComponent->getSelectedSwatchNumber()); };
            addChildComponent(group->getEditButton());

            group->getTrashButton()->getProperties().set("index", i);
            group->getTrashButton()->onClick = [&, group, i] { listeners.call(&ColourPalettesPanel::Listener::deletePaletteRequested, i); };
            addChildComponent(group->getTrashButton());

            String name = palettesIn[i].getName();
            auto label = paletteLabels.add(new Label("Label_" + name, name));
            label->setJustificationType(Justification::centred);
            label->getProperties().set("index", i);
            label->getProperties().set(LumatoneEditorStyleIDs::labelMaximumLineCount, 2);
            addAndMakeVisible(label);

            controlGroupHitBoxes.add(Rectangle<int>());
        }

        allPalettes = Array<ColourPaletteComponent*>(newPalette.get());
        for (auto group : controlGroups)
            allPalettes.add(group->getPaletteComponent());

        int rows = ceil((palettesIn.size() + 1) * 0.333333f);

        // Set height depending on how many rows
        width = (width < 1) ? getWidth() : width;
        viewableHeight = getHeightFromNumRows(width, rows);

        if (resize)
            setSize(width, viewableHeight);

        numRows = rows;
    }

private:

    void setControlsVisibleForPalette(int paletteIndex, bool areVisible)
    {
        auto group = controlGroups.getUnchecked(paletteIndex);
        group->getEditButton()->setVisible(areVisible);
        group->getTrashButton()->setVisible(areVisible);
    }

private:

    ColourSelectionGroup* selectionGroup;

    std::unique_ptr<ColourPaletteComponent> newPalette;
    std::unique_ptr<TextButton> newPaletteBtn;
    OwnedArray<PaletteControlGroup> controlGroups;
    Array<ColourPaletteComponent*> allPalettes;
    OwnedArray<juce::Label> paletteLabels;

    Array<FlexItem> dbgItems;

    int numRows = 1;
    //int viewableWidth = 0;
    int viewableHeight = 0;
    int bottom = 0;

    Array<Rectangle<int>> controlGroupHitBoxes;
    int lastPaletteMouseOver = -1;

    const float itemWidthScalar         = 0.265f;
    const float itemHeightScalar        = 0.24f;

    const float topMarginScalar         = 0.04f;
    const float horizontalMarginScalar  = 0.0367f;
    const float bottomMarginScalar      = 0.06f;
    const float btmMarginCtrlScalar     = 0.02f;

    const float buttonWidthScalar       = 0.333333f;
    const float buttonHeightScalar      = 0.166667f;

    const float panelLeftMarginWidth    = 0.020833f;

    //==============================================================================

public:

    class Listener
    {
    public:

        virtual void editPaletteRequested(int paletteIndex, int selectedSwatchIndex) = 0;
        virtual void deletePaletteRequested(int paletteIndex) = 0;
        virtual void newPaletteRequested() = 0;
    };

    void addListener(Listener* listenerIn) { listeners.add(listenerIn); }
    void removeListener(Listener* listenerIn) { listeners.remove(listenerIn); }

protected:

    ListenerList<ColourPalettesPanel::Listener> listeners;
};

//==============================================================================
/*
*   Full RGB colour selector panel
*/
class CustomPickerPanel : public Component,
    public ChangeListener,
    public ColourSelectionBroadcaster,
    public ColourSelectionListener
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

    void colourChangedCallback(ColourSelectionBroadcaster* source, Colour newColour) override
    {
        if (this != source)
            colourPicker->setCurrentColour(newColour, dontSendNotification);
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
class PaletteEditPanel : public Component,
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
        paletteControl->setColourPalette(*colourPalette.getColours());
        paletteControl->attachColourSelector(colourPicker.get());
        addAndMakeVisible(*paletteControl);

        editPaletteLabel.reset(new Label("EditPaletteLabel", translate("EditPalette")));
        editPaletteLabel->setJustificationType(Justification::centred);
        addAndMakeVisible(*editPaletteLabel);

        paletteNameEditor.reset(new Label("PaletteNameEditor"));
        paletteNameEditor->setJustificationType(Justification::centred);
        paletteNameEditor->setEditable(true);
        paletteNameEditor->addListener(this);
        paletteNameEditor->setColour(Label::ColourIds::backgroundColourId, Colour());
        paletteNameEditor->setText(colourPalette.getName(), dontSendNotification);
        paletteNameEditor->getProperties().set(LumatoneEditorStyleIDs::labelMaximumLineCount, 3);
        addAndMakeVisible(*paletteNameEditor);
        labelTextChanged(paletteNameEditor.get()); // force update


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
        editPaletteLabel->setCentrePosition(leftCenter, roundToInt(editPaletteLabel->getHeight() * 0.5f + proportionOfHeight(editPaletteLabelY)));

        float paletteWidth = proportionOfWidth(paletteWidthScalar);
        float paletteHeight = proportionOfHeight(paletteHeightScalar);
        paletteControl->setSize(paletteWidth, paletteHeight);
        paletteControl->setCentrePosition(leftCenter, roundToInt(paletteHeight * 0.5f + proportionOfHeight(paletteY)));

        saveButton->setSize(proportionOfWidth(buttonWidth), proportionOfHeight(buttonHeight));
        saveButton->setCentrePosition(leftCenter, roundToInt(saveButton->getHeight() * 0.5f + proportionOfHeight(buttonY)));
        cancelButton->setBounds(saveButton->getBounds().translated(0, saveButton->getHeight() * 1.125f));

        colourPicker->setSize(proportionOfWidth(pickerWidth), proportionOfHeight(pickerHeight));
        colourPicker->setTopLeftPosition(leftWidth, roundToInt((getHeight() - colourPicker->getHeight()) * 0.5f));

        float leftMargin = colourPicker->getRight() * 0.03f * 0.5f;
        paletteNameEditor->setBounds(Rectangle<int>(
            Point<int>(roundToInt(leftMargin), paletteControl->getBottom()),
            Point<int>(colourPicker->getX() - leftMargin, saveButton->getY())
            ));
        paletteNameEditor->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, editPaletteLabel->getHeight() / (float)paletteNameEditor->getHeight());

        //if (lookAndFeel)
        //{
        //    Font labelFont = (paletteUnnamed)
        //        ? lookAndFeel->getAppFont(LumatoneEditorFont::UniviaProBold)
        //        : lookAndFeel->getAppFont(LumatoneEditorFont::GothamNarrowMedium);

        //    paletteNameEditor->setFont(labelFont);
        //}

    }

    void lookAndFeelChanged() override
    {
        lookAndFeel = dynamic_cast<LumatoneEditorLookAndFeel*>(&getLookAndFeel());
        if (lookAndFeel)
        {
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
        // Italics and font override here is a bit of a hack
        if (label == paletteNameEditor.get())
        {
            bool nameIsEmpty = paletteNameEditor->getText() == "";

            if (!nameIsEmpty)
            {
                paletteUnnamed = false;
                colourPalette.setName(paletteNameEditor->getText());
                paletteNameEditor->getProperties().remove(LumatoneEditorStyleIDs::fontOverrideTypefaceStyle);
                paletteNameEditor->getProperties().set(LumatoneEditorStyleIDs::fontOverride, LumatoneEditorFont::GothamNarrowMedium);
            }

            else if (nameIsEmpty && !paletteUnnamed)
            {
                paletteUnnamed = true;
            }

            if (paletteUnnamed)
            {
                paletteNameEditor->setText("unnamed", NotificationType::dontSendNotification);
                paletteNameEditor->getProperties().set(LumatoneEditorStyleIDs::fontOverrideTypefaceStyle, "Italic");
                paletteNameEditor->getProperties().remove(LumatoneEditorStyleIDs::fontOverride);
            }

            //resized();
        }
    }

    void editorShown(Label* label, TextEditor& editor) override
    {
        editor.setColour(TextEditor::ColourIds::backgroundColourId, Colours::black);
        editor.setJustification(Justification::centred);
        editor.setMultiLine(true);
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
        return colourPalette.getName();
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

    const float paletteY            = 0.26f;
    const float paletteWidthScalar  = 0.27f;
    const float paletteHeightScalar = 0.25f;

    const float buttonY             = 0.6739f;
    const float buttonWidth         = 0.2208f;
    const float buttonHeight        = 0.0889f;

    const float pickerWidth         = 0.6f;
    const float pickerHeight        = 0.9f;
};
