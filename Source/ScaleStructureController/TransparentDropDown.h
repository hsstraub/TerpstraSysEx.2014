/*
  ==============================================================================

    ComboBoxLookAndFeel.h
    Created: 13 May 2020 9:13:28pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class TransparentDropDown : public LookAndFeel_V4
{
	Colour defaultColour = Colours::darkgrey;
	Colour& baseColour;

public:

	TransparentDropDown()
		: baseColour(defaultColour) {};

	~TransparentDropDown() override {};

	void setBaseColour(Colour& baseColourIn)
	{
		baseColour = baseColourIn;
	}

	// ComboBox Overrides

	void drawComboBox(
		Graphics& g, int width, int height, bool isButtonDown, 
		int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& cb) override
	{
		Font font = getComboBoxFont(cb);
		String text = cb.getText();
		int textWidth = font.getStringWidth(text);
		int textHeight = font.getHeight();
		int marginF = 128;
		float margin = (marginF + 1) / marginF;

		if (cb.isMouseOver())
		{
			g.setColour(Colours::black.withAlpha(0.125f));
			g.fillRoundedRectangle(0, 0, width * margin, textHeight, sqrt(width * height) / 8.0f);
		}

		//g.setColour(Colours::red);
		//g.drawRect(0, 0, width, height);
	}

	Font getComboBoxFont(ComboBox& cb) override
	{
		return Font().withHeight(cb.getHeight());
	}

	Label* createComboBoxTextBox(ComboBox& cb) override
	{
		Label* l = new Label(cb.getName() + "_label", cb.getText());
		l->setInterceptsMouseClicks(false, false);
		return l;
	}

	void positionComboBoxText(ComboBox& cb, Label& labelToPosition) override
	{
		Font font = getComboBoxFont(cb);
		labelToPosition.setBounds(0, 0, cb.getWidth(), cb.getHeight());
		labelToPosition.setFont(font);
		labelToPosition.setJustificationType(Justification::centred);
	}

	PopupMenu::Options getOptionsForComboBoxPopupMenu(ComboBox& cb, Label& lbl) override
	{
		//int translateX = round(cb.getNumItems() * )
		return PopupMenu::Options()
			.withMinimumWidth(cb.getWidth() / 2)
			.withMinimumNumColumns(2)
			.withMaximumNumColumns(6)
			.withStandardItemHeight(cb.getHeight() * 2.0f / 3.0f)
			//.withPreferredPopupDirection(PopupMenu::Options::PopupDirection::upwards)
			//.withTargetScreenArea(cb.getScreenBounds().translated(-translateX, 0));
			.withTargetComponent(cb);
	}


	// PopupMenu Overrides

	void drawPopupMenuBackground(Graphics& g, int width, int height) override
	{
		g.fillAll(baseColour.contrasting(0.1f).withAlpha(0.975f));
	}

	void drawPopupMenuItem(
		Graphics& g, const Rectangle<int>& area, bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
		const String& text, const String& shortcutKeyText, const Drawable* icon, const Colour* textColour) override
	{
		float cornerSize = sqrt(area.getWidth() * area.getHeight()) * 0.125f;

		if (isHighlighted || isTicked)
		{
			if (isHighlighted)
			{
				g.setColour(Colours::black.withAlpha(0.1f));
			}
			else 
			{
				g.setColour(Colours::black.withAlpha(0.2f));
			}
			g.fillRoundedRectangle(area.toFloat(), cornerSize);
		}

		if (textColour)
			g.setColour(*textColour);
		else
			g.setColour(baseColour.withAlpha(1.0f).contrasting());
		
		g.setFont(area.getHeight() * 2 / 3.0f);
		g.drawFittedText(text, area, Justification::centred, 1);
	}

	void getIdealPopupMenuItemSize(const String& text, bool isSeparator, int standardMenuItemHeight, int& idealWidth, int& idealHeight) override
	{
		idealWidth = getPopupMenuFont().getStringWidth(text);
		idealHeight = standardMenuItemHeight;
	}

	Font getPopupMenuFont() override
	{
		return Font().boldened();
	}
};
