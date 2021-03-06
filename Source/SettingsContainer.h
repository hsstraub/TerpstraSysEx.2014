/*
  ==============================================================================

    SettingsContainer.h
    Created: 5 Feb 2021 7:32:32pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "LumatoneEditorLookAndFeel.h"
#include "CalibrationDlg.h"
#include "FirmwareDlg.h"

typedef enum {
    Calibration = 0,
    Firmware = 1
} LumatoneEditorSettingCategories;

class SettingsCategoryModel : public ListBoxModel, public ChangeBroadcaster
{
public:

    SettingsCategoryModel() {}

    int getNumRows() { return categories.size(); }

    void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) override;

    void selectedRowsChanged(int lastRowSelected) override { sendChangeMessage(); }

    //=========================================================================

    const Array<String> categories =
    {
        "Calibration",
        "Firmware"
    };
};

class SettingsContainer : public Component, protected ChangeListener
{
public:

    SettingsContainer();
    ~SettingsContainer();

    void paint(Graphics& g) override;

    void resized() override;

    void lookAndFeelChanged() override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

    //=========================================================================
    
    void showPanel(int editorSettingCategory);

private:

    SettingsCategoryModel model;
    std::unique_ptr<ListBox> categoryList;
    std::unique_ptr<Component> settingsPanel;

};
