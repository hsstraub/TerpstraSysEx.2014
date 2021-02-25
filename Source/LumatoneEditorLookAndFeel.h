/*
  ==============================================================================

    LumatoneEditorLookAndFeel.h
    Created: 13 Nov 2020 7:25:04pm
    Author:  Vincenzo Sicurella

    Asset loading and minimal implementation of LookAndFeel_V4 for the Lumatone Editor application.
    
    Some components also need to be passed into its respective "Setup" function 
    to apply default colours or other properties.

  ==============================================================================
*/

#pragma once

#include "LumatoneEditorStyleCommon.h"
#include "LumatoneEditorFonts.h"

class LumatoneEditorLookAndFeel : public LookAndFeel_V4
{
public:

    LumatoneEditorLookAndFeel(
        LumatoneEditorFonts::Library appFontsIn,
        bool doImageCache = false
    ) : appFonts(appFontsIn)
    {
        setupDefaultColours();

        if (doImageCache)
        {
            cacheImages();
        }
    }

    Font getAppFont(LumatoneEditorFont fontIdIn)
    {
        return appFonts[fontIdIn];
    }

public:
    //==================================================================
    // LookAndFeel_V4 Implementation

    //==================================================================
    //
    // DOCUMENT WINDOW METHODS
    //
    //==================================================================

    void drawDocumentWindowTitleBar(DocumentWindow& window, Graphics& g, 
        int w, int h, int titleSpaceX, int titleSpaceW, const Image* icon, bool drawTitleTextOnLeft) override
    {
        g.fillAll(findColour(LumatoneEditorColourIDs::MediumBackground));
        
        const float fontHeight = h * 0.5f;
        const Font font = appFonts[LumatoneEditorFont::UniviaProBold].withHeight(fontHeight);
        g.setFont(font);

        const float margin = (h - fontHeight) * 0.5f;

        g.setColour(Colours::white);
        g.drawFittedText(window.getName(), 0, margin - font.getDescent(), w, h - margin, Justification::centred, 1, 1.0f);
    }


    Button* createDocumentWindowButton(int buttonType) override
    {
        Colour btnColour = findColour(LumatoneEditorColourIDs::MediumBackground);

        // Mostly pulled from  LookAndFeel_V4::createDocumentWindowButton();
        Path shape;
        auto crossThickness = 0.15f;

        const float btnSize = 96.0f;
        const float vectorSize = btnSize * 0.333333f;
        const float vecHalfSize = vectorSize * 0.5f;

        ImageButton* btn = new ImageButton();
        Image btnImage(Image::PixelFormat::RGB, btnSize, btnSize, false);

        Graphics g(btnImage);

        g.setColour(btnColour);
        g.fillAll();

        if (buttonType == DocumentWindow::closeButton)
        {
            shape.addLineSegment({ 0.0f, 0.0f, vectorSize, vectorSize }, crossThickness);
            shape.addLineSegment({ vectorSize, 0.0f, 0.0f, vectorSize }, crossThickness);

            g.setColour(findColour(LumatoneEditorColourIDs::DisconnectedRed));
        }

        if (buttonType == DocumentWindow::minimiseButton)
        {
            shape.addLineSegment({ 0.0f, vecHalfSize, vectorSize, vecHalfSize}, crossThickness);
            g.setColour(findColour(LumatoneEditorColourIDs::CurveGradientMin));
        }

        // So far not going to be used for Lumatone Editor
        if (buttonType == DocumentWindow::maximiseButton)
        {
            shape.addLineSegment({ vecHalfSize, 0.0f, vecHalfSize, vectorSize }, crossThickness);
            shape.addLineSegment({ 0.0f, vecHalfSize, vectorSize, vecHalfSize }, crossThickness);

            g.setColour(findColour(LumatoneEditorColourIDs::ConnectedGreen));
        }

        float margin = (btnSize - vectorSize) * 0.5f;
        g.strokePath(shape, PathStrokeType(3.0f), AffineTransform::translation(margin, margin));
        
        btn->setImages(false, true, true,
            btnImage, 1.0f, Colour(),
            btnImage, 1.0f, Colours::white.withAlpha(0.1f),
            btnImage, 1.0f, Colours::black.withAlpha(0.1f)
        );

        return btn;
    }

    //==================================================================
    //
    // LABEL METHODS
    //
    //==================================================================

    Font getLabelFont(Label& l) override
    {
        Font font = l.getFont().withHeight(l.getHeight());

        NamedValueSet& properties = l.getProperties();
        if (properties.contains(LumatoneEditorStyleIDs::fontOverride))
        {
            int overrideIndex = properties[LumatoneEditorStyleIDs::fontOverride];
            if (overrideIndex >= 0 && overrideIndex < appFonts.size())
                font = appFonts[overrideIndex].withHeight(l.getHeight());
        }

        if (properties.contains(LumatoneEditorStyleIDs::fontOverrideTypefaceStyle))
        {
            font.setTypefaceStyle(properties[LumatoneEditorStyleIDs::fontOverrideTypefaceStyle]);
        }

        if (properties.contains(LumatoneEditorStyleIDs::fontHeightScalar))
        {
            font.setHeight(l.proportionOfHeight(properties[LumatoneEditorStyleIDs::fontHeightScalar]));
        }

        return font;
    }

    BorderSize<int> getLabelBorderSize(Label&) override
    {
        return BorderSize<int>(0);
    }

    void Label::LookAndFeelMethods::drawLabel(Graphics& g, Label& l)
    {
        Path roundedBounds = getConnectedRoundedRectPath(l.getBounds().toFloat(), l.getHeight() * comboBoxRoundedCornerScalar, 0);
        g.setColour(l.findColour(Label::ColourIds::backgroundColourId));
        g.fillPath(roundedBounds);

        g.setColour(l.findColour(Label::ColourIds::textColourId));
        g.setFont(getLabelFont(l));
        g.drawFittedText(l.getText(), l.getLocalBounds(), l.getJustificationType(), 3);
    }

    //==================================================================
    //
    // BUTTON METHODS
    //
    //==================================================================

    void drawButtonBackground(Graphics& g, Button& btn, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        // If it has Hyperlink flag, only draw a line under text if mouse is over
        if (btn.getProperties().contains(LumatoneEditorStyleIDs::textButtonHyperlinkFlag))
        {
            if (shouldDrawButtonAsHighlighted)
            {
                g.setColour(btn.findColour(TextButton::ColourIds::textColourOffId));
                float halfTextLength = getTextButtonFont(static_cast<TextButton&>(btn), btn.getHeight()).getStringWidthFloat(btn.getButtonText()) * 0.5f;
                float xCenter = btn.getWidth() * 0.5f;
                float yLine = btn.proportionOfHeight(0.9f);
                g.drawLine(roundToInt(xCenter - halfTextLength), yLine, roundToInt(xCenter + halfTextLength), yLine);
            }

            return;
        }

        Colour colour = (shouldDrawButtonAsDown) ? btn.findColour(TextButton::ColourIds::buttonOnColourId) : backgroundColour;

        if (!btn.isEnabled() && colour.isOpaque())
            colour = colour.overlaidWith(findColour(LumatoneEditorColourIDs::DisabledOverlay));

        else if (shouldDrawButtonAsHighlighted)
            colour = colour.brighter(0.075f);

        g.setColour(colour);
        g.fillPath(getButtonShape(btn));
    }

    void drawButtonText(Graphics& g, TextButton& btn, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        NamedValueSet properties = btn.getProperties();

        if (properties.contains(LumatoneEditorStyleIDs::textButtonIconHashCode))
        {
            int bkgdColourId = (shouldDrawButtonAsDown) ? TextButton::ColourIds::buttonOnColourId : TextButton::ColourIds::buttonColourId;
            drawButtonBackground(g, btn, btn.findColour(bkgdColourId), shouldDrawButtonAsDown, shouldDrawButtonAsHighlighted);
            
            float fontScalar = 1.0f;
#if JUCE_MAC
            fontScalar = 0.9f;
#endif
            
            Font font = getTextButtonFont(btn, btn.getHeight() * fontScalar);

            Image icon = ImageCache::getFromHashCode(properties[LumatoneEditorStyleIDs::textButtonIconHashCode]);
            int iconH = font.getHeight();
            int iconW = roundToInt(iconH * ((double)icon.getWidth() / icon.getHeight()));
            int iconY = roundToInt((btn.getHeight() - iconH) * 0.5f);

            {
                ImageProcessor ip;
                icon = ip.resizeImage(icon, iconW, iconH);
            }

            int margin = font.getStringWidth("  ");
            int textWidth = font.getStringWidth(btn.getButtonText());
            int lineStart = roundToInt((btn.getWidth() - textWidth - margin - iconW) * 0.5f);
            int secondHalf = lineStart + margin;

            int colourId = shouldDrawButtonAsDown ? TextButton::ColourIds::textColourOnId : TextButton::ColourIds::textColourOffId;
            Colour textColour = btn.findColour(colourId);
            
            if (!btn.isEnabled())
                textColour = findColour(LumatoneEditorColourIDs::InactiveText);

            else if (shouldDrawButtonAsHighlighted)
                textColour = textColour.brighter();

            g.setColour(textColour);
            g.setFont(font);

            bool iconOnRight = (bool)properties[LumatoneEditorStyleIDs::textButtonIconPlacement];
            if (iconOnRight)
            {
                secondHalf += textWidth;
                g.drawImageAt(icon, secondHalf, iconY);
                g.drawFittedText(btn.getButtonText(), btn.getLocalBounds().withLeft(lineStart).withRight(secondHalf), Justification::left, 1);
            }
            else
            {
                secondHalf += iconW;
                g.drawImageAt(icon, lineStart, iconY);
                g.drawFittedText(btn.getButtonText(), btn.getLocalBounds().withTrimmedLeft(secondHalf), Justification::left, 1);
            }
        }
        else
        {
            int bkgdColourId = (shouldDrawButtonAsDown) ? TextButton::ColourIds::buttonOnColourId : TextButton::ColourIds::buttonColourId;
            drawButtonBackground(g, btn, btn.findColour(bkgdColourId), shouldDrawButtonAsDown, shouldDrawButtonAsHighlighted);

            Font font = getTextButtonFont(btn, btn.getHeight());
            int margin = font.getStringWidth("  ");
            int textWidth = font.getStringWidth(btn.getButtonText());
            int colourId = shouldDrawButtonAsDown ? TextButton::ColourIds::textColourOnId : TextButton::ColourIds::textColourOffId;
            
            Colour textColour = btn.findColour(colourId);

            if (shouldDrawButtonAsDown)
                textColour = textColour.darker(0.5f);
            
            else
            {
                if (!btn.isEnabled())
                    textColour = findColour(LumatoneEditorColourIDs::InactiveText);

                else if (shouldDrawButtonAsHighlighted)
                    textColour = textColour.brighter();
            }

            g.setColour(textColour);
            g.setFont(font);
            g.drawFittedText(btn.getButtonText(), btn.getLocalBounds(), Justification::centred, 1);
        }
    }

    Font getTextButtonFont(TextButton& btn, int buttonHeight) override
    {
        float fontHeight = (btn.getProperties().contains(LumatoneEditorStyleIDs::textButtonHyperlinkFlag))
            ? buttonHeight
            : buttonHeight / 1.75f;

        Font font = appFonts[LumatoneEditorFont::GothamNarrowMedium].withHeight(fontHeight);

        NamedValueSet& properties = btn.getProperties();
        if (properties.contains(LumatoneEditorStyleIDs::fontOverride))
        {
            int overrideIndex = properties[LumatoneEditorStyleIDs::fontOverride];
            if (overrideIndex >= 0 && overrideIndex < appFonts.size())
                font = appFonts[overrideIndex].withHeight(btn.getHeight() * CONTROLBOXFONTHEIGHTSCALAR);
        }

        if (properties.contains(LumatoneEditorStyleIDs::fontOverrideTypefaceStyle))
        {
            font.setTypefaceStyle(properties[LumatoneEditorStyleIDs::fontOverrideTypefaceStyle]);
        }

        if (properties.contains(LumatoneEditorStyleIDs::fontHeightScalar))
        {
            font.setHeight(btn.proportionOfHeight(properties[LumatoneEditorStyleIDs::fontHeightScalar]));
        }

        return font;
    }

    void drawTickBox(Graphics& g, Component& c, float x, float y, float w, float h, bool ticked, bool isEnabled, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        Path shape;
        shape.addRoundedRectangle(x, y, w, h, w * 0.1f);
       
        Colour boxColour = Colours::white;
        Colour tickColour = Colours::black;

        if (!c.isEnabled())
        {
            boxColour = boxColour.overlaidWith(findColour(LumatoneEditorColourIDs::DisabledOverlay));
            tickColour = findColour(LumatoneEditorColourIDs::InactiveText);
        }

        g.setColour(boxColour);
        g.fillPath(shape);

        // TODO: Proper "tick" drawing
        if (ticked)
        {
            g.setColour(tickColour);
            g.setFont(appFonts[LumatoneEditorFont::GothamNarrowBold].withHeight(h).withHorizontalScale(1.333333f));
            g.drawFittedText("X", x, y, w, h, Justification::centred, 1, 1.0f);
        }
    }   

    void drawToggleButton(Graphics& g, ToggleButton& btn, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        drawTickBox(g, btn, 0, 0, btn.getHeight(), btn.getHeight(), btn.getToggleState(), btn.isEnabled(),
            shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

        g.setFont(appFonts[LumatoneEditorFont::GothamNarrowMedium].withHeight(btn.getHeight() * 1.125f));

        Colour textColour = btn.findColour(ToggleButton::ColourIds::textColourId);
       
        if (shouldDrawButtonAsDown)
            textColour = textColour.darker();

        if (!btn.isEnabled())
            textColour = findColour(LumatoneEditorColourIDs::InactiveText);

        else if (shouldDrawButtonAsHighlighted)
            textColour = textColour.brighter(0.1f);
       

        g.setColour(textColour);
        g.drawFittedText(btn.getButtonText(), btn.getLocalBounds().withLeft(btn.getHeight() * 1.5f), Justification::centredLeft, 1);
    }

    //==================================================================
    //
    // SLIDER METHODS
    //
    //==================================================================

    void drawIncDecButtonsBackground(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, Slider& sld)
    {
        Rectangle<int> bounds(x, y, width, height);
        g.setColour(Colours::red);
        g.fillRect(bounds);

        g.setColour(Colours::blue.withAlpha(0.5f));
        g.fillRect(bounds.withLeft(minSliderPos));

        g.setColour(Colours::yellow.withAlpha(0.5f));
        g.fillRect(bounds.withLeft(maxSliderPos));
    }

    void drawLinearSliderBackground(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& sld) override
    {
        if (style == Slider::SliderStyle::IncDecButtons)
        {
            drawIncDecButtonsBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, sld);
        }
        else
        {
            LookAndFeel_V4::drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, sld);
        }
    }

    Button* createSliderButton(Slider& sld, bool isIncrement) override
    {
        auto btn = new TextButton();

        btn->setColour(TextButton::ColourIds::buttonColourId, Colour());
        btn->setColour(TextButton::ColourIds::buttonOnColourId, Colour());

        Colour textColour = findColour(LumatoneEditorColourIDs::DescriptionText);
        
        if (!sld.isEnabled())
            textColour = textColour.overlaidWith(findColour(LumatoneEditorColourIDs::DisabledOverlay));

        btn->setColour(TextButton::ColourIds::textColourOnId, textColour);
        btn->setColour(TextButton::ColourIds::textColourOffId, textColour);

        if (isIncrement) 
            btn->setButtonText("+"); 
        else 
            btn->setButtonText("-");

        btn->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, 1.25f);

        return btn;
    }

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& sld) override
    {
        const float halfPi = float_Pi * 0.5f;
        const float arcThickness = 0.15f;
        const float endAngleNorm = rotaryAngleEnd - halfPi;

        const int w = sld.getWidth();
        const int h = roundToInt(sld.getHeight() * (1.5 - 0.5 * sinf(endAngleNorm))); // translated to compensate for dial radius

        const float dialRadiusFactor = 1.1f;
        const float size = jmin(w, h) / dialRadiusFactor;
        const float radiusInner = size * (1 - arcThickness);

        const int dialMargin = (size * dialRadiusFactor) - size;
        Rectangle<float> outerBounds = Rectangle<float>((w - size) * 0.5f, dialMargin, size, size);
        Rectangle<float> innerBounds = outerBounds.reduced(size * arcThickness);
        Point<float> center = outerBounds.getCentre();

        Path ring;
        addArcToPath(ring, outerBounds, rotaryAngleStart, rotaryAngleEnd, true);
        ring.lineTo(center.x + cosf(endAngleNorm) * radiusInner, center.y + sinf(endAngleNorm) * radiusInner);
        addArcToPath(ring, innerBounds, rotaryAngleEnd, rotaryAngleStart, false);
        ring.closeSubPath();

        Colour minColour = findColour(LumatoneEditorColourIDs::RotaryGradientMin);
        Colour maxColour = findColour(LumatoneEditorColourIDs::RotaryGradientMax);
        Colour dialColour = Colours::white;

        if (!sld.isEnabled())
        {
            minColour = minColour.overlaidWith(findColour(LumatoneEditorColourIDs::DisabledOverlay));
            maxColour = maxColour.overlaidWith(findColour(LumatoneEditorColourIDs::DisabledOverlay));
            dialColour = findColour(LumatoneEditorColourIDs::InactiveText);
        }

        ColourGradient grad = ColourGradient::horizontal(minColour, maxColour, outerBounds);
        g.setGradientFill(grad);
        g.fillPath(ring);

        g.setColour(Colours::white);

        float dialRadius = size * dialRadiusFactor * 0.5f;
        float dialThickness = size * 0.025f;
        float dialAng = (rotaryAngleEnd - rotaryAngleStart) * sliderPosProportional + rotaryAngleStart - halfPi;
        g.drawLine(Line<float>(center, { center.x + cosf(dialAng) * dialRadius , center.y + sinf(dialAng) * dialRadius }), dialThickness);

        // TODO: Make text box positioning more consistent when manipulating the bounds' aspect ratio
    }

    Label* createSliderTextBox(Slider& sld) override
    {
        if (sld.getSliderStyle() >= Slider::SliderStyle::Rotary && sld.getSliderStyle() < Slider::SliderStyle::IncDecButtons)
        {
            Label* label = new Label(sld.getName() + "_ValueLabel");
            label->setText(String(sld.getValue()), dontSendNotification);
            label->setJustificationType(Justification::centred);
            label->setFont(appFonts[LumatoneEditorFont::GothamNarrowMedium]);

            Colour textColour = (sld.isEnabled())
                ? findColour(LumatoneEditorColourIDs::DescriptionText)
                : findColour(LumatoneEditorColourIDs::InactiveText);

            label->setColour(Label::ColourIds::textColourId, textColour);

            int sliderSize = jmin(sld.getWidth(), sld.getHeight());
            sld.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, roundToInt(sliderSize * 0.5f), roundToInt((sld.getHeight() - sliderSize * 0.5f) * 0.5f));

            return label;
        }
        else if (sld.getSliderStyle() == Slider::SliderStyle::IncDecButtons)
        {
            Label* label = new Label(sld.getName() + "_ValueLabel");
            label->setText(String(sld.getValue()), dontSendNotification);
            label->setJustificationType(Justification::centred);
            label->setFont(appFonts[LumatoneEditorFont::GothamNarrowMedium]);
            
            if (sld.getProperties().contains(LumatoneEditorStyleIDs::fontHeightScalar))
                label->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, sld.getProperties()[LumatoneEditorStyleIDs::fontHeightScalar]);

            Colour backgroundColour = findColour(LumatoneEditorColourIDs::ControlBoxBackground);
            Colour textColour = findColour(LumatoneEditorColourIDs::DescriptionText);

            if (!sld.isEnabled())
            {
                backgroundColour = backgroundColour.overlaidWith(findColour(LumatoneEditorColourIDs::DisabledOverlay));
                textColour = findColour(LumatoneEditorColourIDs::InactiveText);
            }

            label->setColour(Label::ColourIds::backgroundColourId, backgroundColour);
            label->setColour(Label::ColourIds::backgroundWhenEditingColourId, backgroundColour);
            label->setColour(Label::ColourIds::textColourId, textColour);

            return label;
        }
        else
        {
            return LookAndFeel_V4::createSliderTextBox(sld);
        }
    }

    //==================================================================
    //
    // TEXTEDITOR METHODS
    //
    //==================================================================

    void fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& editor) override
    {
        Colour backgroundColour = editor.findColour(TextEditor::ColourIds::backgroundColourId);

        if (!backgroundColour.isTransparent())
        {
            if (!editor.isEnabled())
                backgroundColour = backgroundColour.withMultipliedSaturation(0.33f);

            Path boxShape = getConnectedRoundedRectPath(Rectangle<float>(0, 0, width, height),
                roundToInt(height * comboBoxRoundedCornerScalar), 0);

            g.setColour(backgroundColour);
            g.fillPath(boxShape);
            return;
        }
        
        g.fillAll(backgroundColour);
    }

    void drawTextEditorOutline(Graphics&, int width, int height, TextEditor&) override {}



    //==================================================================
    //
    // COMBOBOX METHODS
    //
    //==================================================================

    void drawComboBox(Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box) override
    {
        int margin = roundToInt(height * comboBoxRoundedCornerScalar);

        Colour backgroundColour = box.findColour(ComboBox::ColourIds::backgroundColourId);
        Colour textColour = findColour(LumatoneEditorColourIDs::DescriptionText);

        if (!box.isEnabled())
        {
            backgroundColour = backgroundColour.withMultipliedSaturation(0.33f);
            textColour = findColour(LumatoneEditorColourIDs::InactiveText);
        }

        else if (box.isMouseOver(true))
        {
            backgroundColour = backgroundColour.withMultipliedSaturation(1.5f);// .brighter(0.05f);
            textColour       = textColour.brighter(0.1f);
        }

        g.setColour(backgroundColour);

        Path boxShape = getConnectedRoundedRectPath(box.getLocalBounds().toFloat(), 
            roundToInt(height * comboBoxRoundedCornerScalar), 
            (box.isPopupActive()) ? Button::ConnectedEdgeFlags::ConnectedOnBottom : 0
        );
        g.fillPath(boxShape);

        g.setFont(getComboBoxFont(box));
        g.setColour(textColour);

        int realButtonX = jmax(margin, box.getWidth() - box.getHeight());

        if (buttonW > 0)
        {
            if (box.getSelectedId() == 0)
            {

                String text = (box.getNumItems()) ? box.getTextWhenNothingSelected() : box.getTextWhenNoChoicesAvailable();
                g.drawFittedText(text, margin, 0, realButtonX - margin, height, Justification::centredLeft, 1);
            }

            g.setColour(textColour);

            g.setFont(appFonts[LumatoneEditorFont::GothamNarrowLight].withHeight(buttonH * 0.5f).withHorizontalScale(2.0f));
            g.drawFittedText("v", realButtonX, 0, box.getHeight(), box.getHeight(), Justification::centred, 1);
        }
    }

    Font getComboBoxFont(ComboBox& box) override
    {
        Font font = appFonts[LumatoneEditorFont::GothamNarrowMedium].withHeight(box.getHeight() * CONTROLBOXFONTHEIGHTSCALAR);

        NamedValueSet& properties = box.getProperties();
        if (properties.contains(LumatoneEditorStyleIDs::fontOverride))
        {
            int overrideIndex = properties[LumatoneEditorStyleIDs::fontOverride];
            if (overrideIndex >= 0 && overrideIndex < appFonts.size())
                font = appFonts[overrideIndex].withHeight(box.getHeight() * CONTROLBOXFONTHEIGHTSCALAR);
        }
        
        if (properties.contains(LumatoneEditorStyleIDs::fontOverrideTypefaceStyle))
        {
            font.setTypefaceStyle(properties[LumatoneEditorStyleIDs::fontOverrideTypefaceStyle]);
        }

        if (properties.contains(LumatoneEditorStyleIDs::fontHeightScalar))
        {
            font.setHeight(font.getHeight() * (float) properties[LumatoneEditorStyleIDs::fontHeightScalar]);
        }

        return font;
    }

    Label* createComboBoxTextBox(ComboBox& box) override
    {
        Label* l = new Label(box.getName(), box.getText());
        l->setFont(getComboBoxFont(box));

        Colour textColour = box.findColour(ComboBox::ColourIds::textColourId);

        if (!box.isEnabled())
            textColour = textColour.overlaidWith(findColour(LumatoneEditorColourIDs::DisabledOverlay));

        l->setColour(Label::ColourIds::textColourId, textColour);
        
        for (auto prop : box.getProperties())
            l->getProperties().set(prop.name, prop.value);

        return l;
    }

    void positionComboBoxText(ComboBox& box, Label& labelToPosition) override
    {
        int margin = box.proportionOfHeight(comboBoxRoundedCornerScalar);
        float fontHeight = labelToPosition.getFont().getHeight();

        labelToPosition.setBounds(
            margin, roundToInt((box.getHeight() - fontHeight) * 0.5f), 
            box.getWidth() - box.getHeight() - margin, fontHeight
        );
    }

    void drawComboBoxTextWhenNothingSelected(Graphics& g, ComboBox& box, Label& l) override
    {
        drawComboBox(g, box.getWidth(), box.getHeight(), box.isMouseButtonDown(),
            box.getWidth() - box.getHeight(), 0, box.getHeight(), box.getHeight(),
            box
        );        
    }

    PopupMenu::Options getOptionsForComboBoxPopupMenu(ComboBox& box, Label& label) override
    {
        int numColumns = jmax(1, (int)box.getProperties()[LumatoneEditorStyleIDs::popupMenuMaxColumns]);

        return PopupMenu::Options()
            .withTargetComponent(box)
            .withMinimumWidth(box.getWidth())
            //.withMinimumNumColumns(numColumns)
            .withMaximumNumColumns(1)
            .withStandardItemHeight(label.getFont().getHeight() * 1.1f)
            .withPreferredPopupDirection(PopupMenu::Options::PopupDirection::downwards);
    }

    //==================================================================
    //
    // POPUPMENU METHODS
    //
    //==================================================================

    void drawPopupMenuBackgroundWithOptions(Graphics& g, int width, int height, const PopupMenu::Options& options) override
    {
        auto target = options.getTargetComponent();

        float margin = target->getHeight() * comboBoxRoundedCornerScalar;

        auto box = dynamic_cast<ComboBox*>(target);
        if (box)
            g.setColour(box->findColour(ComboBox::ColourIds::backgroundColourId).withMultipliedSaturation(1.5f)); // Box colour will always be highlighted
        else
            g.setColour(findColour(LumatoneEditorColourIDs::ControlBoxHighlighted)); 

        Path menuShape = getConnectedRoundedRectPath(Rectangle<float>(0, 0, target->getWidth(), height), margin, Button::ConnectedEdgeFlags::ConnectedOnTop);
        g.fillPath(menuShape);
    }

    void drawPopupMenuItemWithOptions(
        Graphics& g, const Rectangle<int>& area, bool isHighlighted, const PopupMenu::Item& item, const PopupMenu::Options& options) override
    {
        int width, height;
        getIdealPopupMenuItemSizeWithOptions(item.text, false, area.getHeight(), width, height, options);

        Rectangle<int> areaToUse = area.withWidth(width);

        // Only using PopupMenus with ComboBoxes so this should be fine
        auto target = dynamic_cast<ComboBox*>(options.getTargetComponent());
        Font font;
        Colour textColour;
        
        if (target)
        {
            font = getComboBoxFont(*target);
            textColour = target->findColour(ComboBox::ColourIds::textColourId);
        }
        else
        {
            font = getPopupMenuFont().withHeight(area.getHeight() * CONTROLBOXFONTHEIGHTSCALAR);
            textColour = findColour(LumatoneEditorColourIDs::DescriptionText);
        }

        int margin = target->proportionOfHeight(comboBoxRoundedCornerScalar);

        // If it's the last item, reduce size so highlight doesn't pass rounded corners
        if (item.itemID == target->getItemId(target->getNumItems() - 1))
        {
            areaToUse = areaToUse.withTrimmedBottom(roundToInt(margin * 0.5f) + 1);
        }

        if (isHighlighted)
        {
            g.setColour(Colours::white.withAlpha(0.15f));
            g.fillRect(areaToUse);
        }

        g.setColour(textColour);
        g.setFont(font);
        
        g.drawFittedText(item.text, areaToUse.withTrimmedLeft(margin), Justification::centredLeft, 1);
    }

    void getIdealPopupMenuItemSizeWithOptions(const String& text, bool isSeparator, int standardMenuItemHeight, int& idealWidth, int& idealHeight, 
        const PopupMenu::Options& options) override
    {
        auto target = dynamic_cast<ComboBox*>(options.getTargetComponent());

        idealWidth = target->getWidth();

        // If the option is the last available option, add a margin
        if (target->getItemText(target->getNumItems() - 1) == text)
        {
            idealHeight = roundToInt(target->getHeight() * (1 + comboBoxRoundedCornerScalar * 0.5f));
        }
        else
        {
            idealHeight = target->getHeight();
        }
    }

    Font getPopupMenuFont() override
    {
        return appFonts[LumatoneEditorFont::UniviaProBold];
    }

    int getMenuWindowFlags() override
    {
        return ComponentPeer::StyleFlags::windowIsSemiTransparent;
    }

    void preparePopupMenuWindow(Component& window) override
    {
        window.setOpaque(false);
    }

    //==================================================================
    //
    // TABBEDBUTTONBAR METHODS
    //
    //==================================================================

    int getTabButtonSpaceAroundImage() override { return 0; }

    int getTabButtonOverlap(int tabDepth) override { return 0; }

    int getTabButtonBestWidth(TabBarButton& tbb, int tabDepth) override 
    {
        TabbedButtonBar& bar = tbb.getTabbedButtonBar();
        return bar.getWidth() / bar.getNumTabs();
    }

    void drawTabButton(TabBarButton& tbb, Graphics& g, bool isMouseOver, bool isMouseDown) override
    {
        Colour c;

        if (tbb.isFrontTab())
            c = Colours::white;

        else if (isMouseOver)
            c = c.withAlpha(0.15f);

        g.setColour(c);
        g.drawLine(0, tbb.getHeight(), tbb.getWidth(), tbb.getHeight(), 3.0f);

        drawTabButtonText(tbb, g, isMouseOver, isMouseDown);
    }

    Font getTabButtonFont(TabBarButton& tbb, float height) override
    {
        return appFonts[LumatoneEditorFont::GothamNarrowMedium].withHeight(height).withHorizontalScale(1.05f);
    }

    void drawTabButtonText(TabBarButton& tbb, Graphics& g, bool isMouseOver, bool isMouseDown) override
    {
        Colour c = findColour(LumatoneEditorColourIDs::InactiveText); // Maybe should change this even though it's the same default colour

        if (tbb.isFrontTab())
            c = findColour(LumatoneEditorColourIDs::ActiveText);

        else if (isMouseOver)
            c = c.brighter(0.15f);

        g.setColour(c);

        const float defaultHeightScalar = 0.54545455f;
        NamedValueSet& barProperties = tbb.getTabbedButtonBar().getProperties();
        float heightScalar = (barProperties.contains(LumatoneEditorStyleIDs::tabbedButtonBarDepthScalar))
            ? (float) barProperties[LumatoneEditorStyleIDs::tabbedButtonBarDepthScalar]
            : defaultHeightScalar;

        Font font = getTabButtonFont(tbb, tbb.getHeight() * heightScalar);
        g.setFont(font);
        
        int textMargin = 0;
        Justification textJustify = Justification::centred;
        
        if (tbb.getTabbedButtonBar().getNumTabs() == 1)
        {
            textMargin = font.getStringWidth("  ");
            textJustify = Justification::centredLeft;
        }
  
        g.drawFittedText(tbb.getButtonText(), tbb.getTextArea().translated(textMargin, 0).toNearestInt(), textJustify, 0.0f);
    }

    void drawTabAreaBehindFrontButton(TabbedButtonBar& tbb, Graphics& g, int w, int h) override {}


    //==================================================================
    //
    // CALLOUTBOX METHODS
    //
    //==================================================================

    void drawCallOutBoxBackground(CallOutBox& box, Graphics& g, const Path& path, Image& img) override
    {
        //Rectangle<float> bounds = box.getLocalBounds().toFloat().reduced(getCallOutBoxBorderSize(box) / 2);
        g.setColour(Colours::black);
        g.fillPath(path);
    }

    int getCallOutBoxBorderSize(const CallOutBox& box) override
    {
        return 20;
    }

    float getCallOutBoxCornerSize(const CallOutBox& box) override
    {
        return box.getHeight() * 0.0303f;
    }

public:
    //==============================================================================================
    // Component Setup functions

    // Set colours of TextButton based on Live and Offline Editor buttons
    void setupRadioTextButton(Button& btn, int radioGroup, bool initalToggledState)
    {
        btn.setColour(TextButton::ColourIds::buttonColourId, Colour(0xff1c1c1c));
        btn.setColour(TextButton::ColourIds::buttonOnColourId, Colour(0xff383b3d));
        btn.setColour(TextButton::ColourIds::textColourOffId, Colour(0xffb5b5b5));
        btn.setColour(TextButton::ColourIds::textColourOnId, Colour(0xffffffff));
        btn.setClickingTogglesState(true);
        btn.setRadioGroupId(radioGroup, dontSendNotification);
        btn.setToggleState(initalToggledState, dontSendNotification);
    }

    void setupTextButton(Button& btn)
    {
        btn.setColour(TextButton::ColourIds::buttonColourId, Colour(0xff383b3d));
        btn.setColour(TextButton::ColourIds::textColourOffId, Colour(0xffffffff));
    }

    // Set colours of Image button based on Live and Offline Editor Buttons
    // Use the same image for Normal, Highlighted, and Down, but apply alpha layer on Highlighted
    void setupImageButton(ImageButton& btn, Image btnImage)
    {
        btn.setColour(TextButton::ColourIds::buttonColourId, Colour(0xff1c1c1c));
        btn.setColour(TextButton::ColourIds::buttonOnColourId, Colour(0xff383b3d));

        btn.setImages(false, true, true,
            btnImage, 1.0f, Colour(),
            btnImage, 1.0f, Colours::white.withAlpha(0.1f),
            btnImage, 1.0f, Colour()
        );
    }

    // Set text colour to be light gray
    void setupToggleButton(ToggleButton& btn)
    {
        btn.setColour(ToggleButton::ColourIds::textColourId, findColour(LumatoneEditorColourIDs::DescriptionText));
    }

    // Set Slider style to Rotary
    void setupRotarySlider(Slider& sld)
    {
        sld.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    }

    // Set generic ComboBox colours
    void setupComboBox(ComboBox& box)
    {
        box.setColour(ComboBox::ColourIds::backgroundColourId, findColour(LumatoneEditorColourIDs::ControlBoxBackground));
        box.setColour(ComboBox::ColourIds::textColourId,       findColour(LumatoneEditorColourIDs::DescriptionText));
    }

    // Set generic TextEditor colours
    void setupTextEditor(TextEditor& editor)
    {
        editor.setColour(TextEditor::ColourIds::backgroundColourId,      findColour(LumatoneEditorColourIDs::ControlBoxBackground));
        editor.setColour(TextEditor::ColourIds::textColourId,            findColour(LumatoneEditorColourIDs::DescriptionText));
        editor.setColour(TextEditor::ColourIds::highlightColourId,       findColour(LumatoneEditorColourIDs::ControlBoxHighlighted));
        editor.setColour(TextEditor::ColourIds::highlightedTextColourId, findColour(LumatoneEditorColourIDs::DescriptionText));
        editor.setColour(TextEditor::ColourIds::shadowColourId,          Colour());
        editor.setIndents(4, 0);
    }

public:
    //==============================================================================================
    // Non-static drawing helpers

    // Button shape path helper with default rounded corner size
    Path getButtonShape(const Button& btn)
    {
        return getConnectedRoundedRectPath(btn.getLocalBounds().toFloat(), btn.getHeight() * buttonRoundedCornerScalar, btn.getConnectedEdgeFlags());
    }

private:
    //==============================================================================================
    
    /// <summary>
    /// MUST be called a single time via the LookAndFeel constructor in the MainComponent 
    /// </summary>
    void cacheImages()
    {
        ImageCache::addImageToCache(ImageCache::getFromMemory(BinaryData::ImportIcon4x_png,   BinaryData::ImportIcon4x_pngSize),   LumatoneEditorAssets::ImportIcon);
        ImageCache::addImageToCache(ImageCache::getFromMemory(BinaryData::SaveIcon4x_png,     BinaryData::SaveIcon4x_pngSize),     LumatoneEditorAssets::SaveIcon);
        ImageCache::addImageToCache(ImageCache::getFromMemory(BinaryData::LoadIcon4x_png,     BinaryData::LoadIcon4x_pngSize),     LumatoneEditorAssets::LoadIcon);
        ImageCache::addImageToCache(ImageCache::getFromMemory(BinaryData::KeyboardBase_png,   BinaryData::KeyboardBase_pngSize),   LumatoneEditorAssets::LumatoneGraphic);
        ImageCache::addImageToCache(ImageCache::getFromMemory(BinaryData::KeybedShadows_png,  BinaryData::KeybedShadows_pngSize),  LumatoneEditorAssets::KeybedShadows);
        ImageCache::addImageToCache(ImageCache::getFromMemory(BinaryData::KeyShape2x_png,     BinaryData::KeyShape2x_pngSize),     LumatoneEditorAssets::KeyShape);
        ImageCache::addImageToCache(ImageCache::getFromMemory(BinaryData::KeyShadow2x_png,    BinaryData::KeyShadow2x_pngSize),    LumatoneEditorAssets::KeyShadow);
        ImageCache::addImageToCache(ImageCache::getFromMemory(BinaryData::SavePalette2x_png,  BinaryData::SavePalette2x_pngSize),  LumatoneEditorAssets::SavePalette);
        ImageCache::addImageToCache(ImageCache::getFromMemory(BinaryData::TrashCanIcon2x_png, BinaryData::TrashCanIcon2x_pngSize), LumatoneEditorAssets::TrashCanIcon);
    }

    /// <summary>
    /// Sets the LookAndFeel colour palette to default colours
    /// </summary>
    void setupDefaultColours()
    {
        setColour(LumatoneEditorColourIDs::LabelPink,                       Colour(0xffdfceca));
        setColour(LumatoneEditorColourIDs::LabelBlue,                       Colour(0xff60aac5));
        setColour(LumatoneEditorColourIDs::DisconnectedRed,                 Colour(0xffd7002a));
        setColour(LumatoneEditorColourIDs::ConnectedGreen,                  Colour(0xff84aea3));
        setColour(LumatoneEditorColourIDs::DarkBackground,                  Colour(0xff1a1b1c));
        setColour(LumatoneEditorColourIDs::MediumBackground,                Colour(0xff212529));
        setColour(LumatoneEditorColourIDs::LightBackground,                 Colour(0xff272b2f));
        setColour(LumatoneEditorColourIDs::ControlAreaBackground,           Colour(0xff212626));
        setColour(LumatoneEditorColourIDs::ActiveText,                      Colours::white);
        setColour(LumatoneEditorColourIDs::InactiveText,                    Colour(0xffb1b1b1));
        setColour(LumatoneEditorColourIDs::DescriptionText,                 Colour(0xffcbcbcb));
        setColour(LumatoneEditorColourIDs::ControlBoxBackground,            Colour(0xff212427));
        setColour(LumatoneEditorColourIDs::ControlBoxHighlighted,           Colour(0xff3d5a78));
        setColour(LumatoneEditorColourIDs::DefaultPresetButtonActive,       Colour(0xffff84e6));
        setColour(LumatoneEditorColourIDs::DefaultPresetButtonInactive,     Colour(0xff5c7cf2));
        setColour(LumatoneEditorColourIDs::OutlineColourId,                 Colours::white);
        setColour(LumatoneEditorColourIDs::CurveGradientMin,                Colour(0xffbf961e));
        setColour(LumatoneEditorColourIDs::CurveGradientMax,                Colour(0xffcd6f2e));
        setColour(LumatoneEditorColourIDs::CurveGridColour,                 Colour(0xff303030));
        setColour(LumatoneEditorColourIDs::RotaryGradientMin,               Colour(0xff5497b6));
        setColour(LumatoneEditorColourIDs::RotaryGradientMax,               Colour(0xff77a8b3));
        setColour(LumatoneEditorColourIDs::DisabledOverlay,                 Colour(0x601b1b1b));
    }

private:

    LumatoneEditorFonts::Library appFonts;

    // Default graphics constants
    const float buttonRoundedCornerScalar = 0.2f;

    const float comboBoxRoundedCornerScalar = 0.304878f;

    const float rotaryAngleStart = float_Pi * -0.64f; // pi * -2/3
    const float rotaryAngleEnd = -rotaryAngleStart;
};
