/*
  ==============================================================================

    SettingsContainer.cpp
    Created: 5 Feb 2021 7:32:32pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "SettingsContainer.h"

void SettingsCategoryModel::paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected)
{
    Colour backgroundColour = (rowIsSelected) ? Colours::teal : Colour();
    Rectangle<int> rowBounds(0, 0, width, height);

    if (rowIsSelected)
    {
        g.setColour(backgroundColour);
        g.fillRect(rowBounds);
    }

    g.setColour(backgroundColour.contrasting());
    g.setFont(LumatoneEditorFonts::GothamNarrowMedium(height));
    g.drawFittedText(categories[rowNumber], rowBounds, Justification::left, 1, 1.0f);
}

//=========================================================================

SettingsContainer::SettingsContainer()
    : Component("SettingsContainer")
{
    categoryList.reset(new ListBox("CategoryList", &model));
    addAndMakeVisible(categoryList.get());
    model.addChangeListener(this);

    // Find category width
    //for (auto category : model.categories)
    //{

    //}
}

SettingsContainer::~SettingsContainer()
{
    categoryList = nullptr;
}

void SettingsContainer::paint(Graphics& g)
{

}

void SettingsContainer::resized()
{
    

    categoryList->setBounds(getLocalBounds().withRight(proportionOfWidth(0.3)));
    
    if (settingsPanel.get())
    {
        settingsPanel->setBounds(getLocalBounds().withLeft(proportionOfWidth(0.3f)).reduced(0.033333f));
    }
}

void SettingsContainer::changeListenerCallback(ChangeBroadcaster* source)
{
    displayPanel(categoryList->getSelectedRow());
}

void SettingsContainer::displayPanel(int editorSettingCategory)
{
    Component* newPanel = nullptr;
    switch (editorSettingCategory)
    {
    case LumatoneEditorSettingCategories::Calibration:
        newPanel = new CalibrationDlg();
        break;

    case LumatoneEditorSettingCategories::Firmware:
        // TODO
        break;
    }

    if (newPanel)
    {
        removeChildComponent(settingsPanel.get());
        settingsPanel = nullptr;
        settingsPanel.reset(std::move(newPanel));
        addAndMakeVisible(settingsPanel.get());
        resized();
    }
}