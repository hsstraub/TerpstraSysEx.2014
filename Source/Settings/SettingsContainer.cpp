/*
  ==============================================================================

    SettingsContainer.cpp
    Created: 5 Feb 2021 7:32:32pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "SettingsContainer.h"
#include "../Main.h"

void SettingsCategoryModel::paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected)
{
    Colour backgroundColour = (rowIsSelected) ? Colours::teal : Colour();
    Rectangle<int> rowBounds(0, 0, width, height);

    if (rowIsSelected)
    {
        g.setColour(backgroundColour);
        g.fillRect(rowBounds);
    }

    g.setColour(Colours::white);
    g.setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::GothamNarrowMedium, height));
    g.drawFittedText(categories[rowNumber], rowBounds.withLeft(8), Justification::left, 1, 1.0f);
}

//=========================================================================

SettingsContainer::SettingsContainer()
    : Component("SettingsContainer"),
      model({"Calibrate", "Firmware"})
{
    categoryList.reset(new ListBox("CategoryList"));
    categoryList->setModel(&model);

    addAndMakeVisible(categoryList.get());
    model.addChangeListener(this);

    auto lastPanelIndex = TerpstraSysExApplication::getApp().getPropertiesFile()->getIntValue("LastSettingsPanel", 0);
    categoryList->selectRow(lastPanelIndex);
}

SettingsContainer::~SettingsContainer()
{
    categoryList = nullptr;
}

void SettingsContainer::paint(Graphics& g)
{
    g.fillAll(findColour(ResizableWindow::ColourIds::backgroundColourId));
}

void SettingsContainer::resized()
{
    categoryList->setBounds(getLocalBounds().withRight(proportionOfWidth(0.2)));
    
    if (settingsPanel.get())
    {
        settingsPanel->setBounds(getLocalBounds().withLeft(categoryList->getRight()).reduced(0.033333f));
    }
}

void SettingsContainer::lookAndFeelChanged()
{
    auto* lookAndFeel = dynamic_cast<LumatoneEditorLookAndFeel*>(&getLookAndFeel());
    if (lookAndFeel)
    {
        setColour(ResizableWindow::ColourIds::backgroundColourId, lookAndFeel->findColour(LumatoneEditorColourIDs::LightBackground));
        categoryList->setColour(ListBox::ColourIds::backgroundColourId, lookAndFeel->findColour(LumatoneEditorColourIDs::MediumBackground));
    }
}

void SettingsContainer::changeListenerCallback(ChangeBroadcaster* source)
{
    auto panelIndex = categoryList->getSelectedRow();
    showPanel(panelIndex);
}

void SettingsContainer::showPanel(int editorSettingCategory)
{
    Component* newPanel = nullptr;
    switch (editorSettingCategory)
    {
    case LumatoneEditorSettingCategories::Calibration:
        newPanel = new CalibrationDlg();
        break;

    case LumatoneEditorSettingCategories::Firmware:
        newPanel = new FirmwareDlg();
        break;
    }

    if (newPanel)
    {
        TerpstraSysExApplication::getApp().getPropertiesFile()->setValue("LastSettingsPanel", editorSettingCategory);

        removeChildComponent(settingsPanel.get());
        settingsPanel = nullptr;
        settingsPanel.reset(std::move(newPanel));
        addAndMakeVisible(settingsPanel.get());
        settingsPanel->setLookAndFeel(&getLookAndFeel());
        resized();
    }
}
