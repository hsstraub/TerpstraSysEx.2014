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
        //for (auto item : flexBox.items)
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
        Rectangle<int> viewportBounds(viewableWidth, viewableHeight);

        float horizontalMargin  = viewportBounds.proportionOfWidth(horizontalMarginScalar);
        float itemWidth         = viewportBounds.proportionOfWidth(itemWidthScalar);
        float topMargin         = viewportBounds.proportionOfHeight(topMarginScalar);
        float bottomMargin      = viewportBounds.proportionOfHeight(bottomMarginScalar);

        for (int i = 0; i < flexBox.items.size(); i++)
        {
            FlexItem& item = flexBox.items.getReference(i);
            item.width = itemWidth;
            item.height = itemWidth;
            item.margin = FlexItem::Margin(topMargin, horizontalMargin, bottomMargin, horizontalMargin);
        }

        flexBox.performLayout(viewportBounds);

        for (int i = 0; i < palettes.size(); i++)
        {
            FlexItem& item = flexBox.items.getReference(i);
            Rectangle<int> bottomMarginBounds(item.currentBounds.getX(), item.currentBounds.getBottom(), itemWidth, bottomMargin);
            int halfItemWidth = bottomMarginBounds.proportionOfWidth(0.5f);

            auto group = palettes.getUnchecked(i);

            group->editButton.setSize(halfItemWidth, bottomMarginBounds.proportionOfHeight(0.5f));
            group->editButton.setTopLeftPosition(bottomMarginBounds.getPosition());
            group->trashButton.setBounds(group->editButton.getBounds().translated(halfItemWidth, 0));
        }

        newPaletteBtn->setSize(itemWidth, round(bottomMargin * 0.5f));
        newPaletteBtn->setTopLeftPosition(newPalette->getX(), newPalette->getBottom());
    }

    // Setup panels from scratch
    void rebuildPanel(bool resize = true)
    {
        removeAllChildren();
        flexBox.items.clear();

        // Palettes with colour
        for (int i = 0; i < palettes.size(); i++)
        {
            auto group = palettes.getUnchecked(i);
            
            addAndMakeVisible(group->palette);
            flexBox.items.add(group->palette);

            addAndMakeVisible(group->editButton);
            addAndMakeVisible(group->trashButton);
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

    FlexBox flexBox;

    OwnedArray<PaletteControlGroup>& palettes;
    ColourPaletteComponent* newPalette;
    TextButton* newPaletteBtn;

    int numRows = 1;
    int viewableWidth = 0;
    int viewableHeight = 0;

    const float itemWidthScalar         = 0.25f;

    const float topMarginScalar         = 1.0f / 24.0f;
    const float bottomMarginScalar      = 1.0f / 9.0f;
    const float horizontalMarginScalar  = 1.0f / 24.0f;

    const float buttonWidthScalar       = 1.0f / 3.0f;
    const float buttonHeightScalar      = 1.0f / 6.0f;

    const float panelLeftMarginWidth    = 1.0f / 48.0f;
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
class PaletteEditPanel : public Component, public Button::Listener, public ChangeBroadcaster
{
public:

    PaletteEditPanel(Array<Colour> paletteColours)
    {
        colourPicker.reset(new ColourSelector(
              ColourSelector::ColourSelectorOptions::editableColour
            + ColourSelector::ColourSelectorOptions::showColourAtTop
            + ColourSelector::ColourSelectorOptions::showColourspace
        ));
        addAndMakeVisible(*colourPicker);

        palette.reset(new TenHexagonPalette());
        palette->setColourPalette(paletteColours);
        palette->attachColourSelector(colourPicker.get());
        addAndMakeVisible(*palette);

        editPaletteLabel.reset(new Label("EditPaletteLabel", translate("EditPalette")));
        editPaletteLabel->setJustificationType(Justification::centred);
        addAndMakeVisible(*editPaletteLabel);

        saveImage = ImageCache::getFromHashCode(LumatoneEditorAssets::SavePalette);
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
        palette = nullptr;
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

        float paletteSize = proportionOfWidth(paletteWidthSize);
        palette->setSize(paletteSize, paletteSize);
        palette->setCentrePosition(leftCenter, round(paletteSize * 0.5f + proportionOfHeight(paletteY)));

        saveButton->setSize(proportionOfWidth(buttonWidth), proportionOfHeight(buttonHeight));
        saveButton->setCentrePosition(leftCenter, round(saveButton->getHeight() * 0.5f + proportionOfHeight(buttonY)));
        cancelButton->setBounds(saveButton->getBounds().translated(0, saveButton->getHeight() * 1.125f));

        colourPicker->setSize(proportionOfWidth(pickerWidth), proportionOfHeight(pickerHeight));
        colourPicker->setTopLeftPosition(leftWidth, round((getHeight() - colourPicker->getHeight()) * 0.5f));
    }

    void lookAndFeelChanged() override
    {
        auto lookAndFeel = dynamic_cast<LumatoneEditorLookAndFeel*>(&getLookAndFeel());
        if (lookAndFeel)
        {
            lookAndFeel->setupTextButton(*saveButton);
            lookAndFeel->setupTextButton(*cancelButton);
        }
    }

    Array<Colour> getCurrentPalette() const
    {
        return palette->getColourPalette();
    }

    void setSelectedSwatch(int selectedSwatchNumber)
    {
        if (selectedSwatchNumber >= 0 && selectedSwatchNumber < palette->getNumberOfSwatches())
            palette->setSelectedSwatchNumber(selectedSwatchNumber);
    }

    void buttonClicked(Button* btn) override
    {
        if (btn == saveButton.get())
        {
            saveRequested = true;
        }

        sendChangeMessage();
    }

    bool wasSaveRequested() const
    {
        return saveRequested;
    }

private:

    std::unique_ptr<Palette> palette;
    std::unique_ptr<ColourSelector> colourPicker;

    Image saveImage;

    std::unique_ptr<Label> editPaletteLabel;
    std::unique_ptr<TextButton> saveButton;
    std::unique_ptr<TextButton> cancelButton;

    bool saveRequested = false;

    // Drawing constants

    const float leftColumnWidth     = 0.3677f;

    const float editPaletteLabelY   = 0.1212f;
    const float editPaletteHeight   = 0.0606f;

    const float paletteY            = 0.2581f;
    const float paletteWidthSize    = 0.2727f;

    const float buttonY             = 0.6739f;
    const float buttonWidth         = 0.2208f;
    const float buttonHeight        = 0.0889f;

    const float pickerWidth         = 0.6f;
    const float pickerHeight        = 0.9f;
};