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
    ) : appFonts(appFontsIn), compactWindowStyle(this)
    {
        setupDefaultColours();

        if (doImageCache)
        {
            cacheImages();
            cacheIcons();
        }

    }
    
    LumatoneEditorLookAndFeel(const LumatoneEditorLookAndFeel& lafCopy)
        : appFonts(lafCopy.appFonts),
          saveIconPath(lafCopy.saveIconPath),
          arrowUpIconPath(lafCopy.arrowUpIconPath),
          arrowDownIconPath(lafCopy.arrowDownIconPath),
          compactWindowStyle(this)
    {
        setupDefaultColours();
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
    
    // Primarily used for palette pop-up
    class LumatoneEditorCompactWindow : public LookAndFeel_V4
    {
    public:

        LumatoneEditorCompactWindow(const LumatoneEditorLookAndFeel* parentIn) : parent(parentIn) {};

        void drawDocumentWindowTitleBar(DocumentWindow& window, Graphics& g,
            int w, int h, int titleSpaceX, int titleSpaceW, const Image* icon, bool drawTitleTextOnLeft) override
        {
            g.fillAll(parent->findColour(LumatoneEditorColourIDs::DarkBackground));

            g.setColour(parent->findColour(LumatoneEditorColourIDs::DescriptionText));
            g.setFont(parent->appFonts[LumatoneEditorFont::GothamNarrowMedium].withHeight(h * 0.75f));
            g.drawFittedText(window.getName(), 0, 0, w, h, Justification::centred, 1, 1.0f);
        }

        Button* createDocumentWindowButton(int buttonType) override
        {
            // Pulled from LookAndFeel_V4::createDocumentWindowButton();
            Path shape;
            auto crossThickness = 0.15f;

            const float btnSize = 96.0f;
            const float vectorSize = btnSize * 0.333333f;
//            const float vecHalfSize = vectorSize * 0.5f;

            ImageButton* btn = new ImageButton();
            Image btnImage(Image::PixelFormat::RGB, btnSize, btnSize, false);

            Graphics g(btnImage);
            g.fillAll(parent->findColour(LumatoneEditorColourIDs::DarkBackground));

            if (buttonType == DocumentWindow::closeButton)
            {
                shape.addLineSegment({ 0.0f, 0.0f, vectorSize, vectorSize }, crossThickness);
                shape.addLineSegment({ vectorSize, 0.0f, 0.0f, vectorSize }, crossThickness);

                g.setColour(parent->findColour(LumatoneEditorColourIDs::DescriptionText));
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

    private:

        const LumatoneEditorLookAndFeel* parent;
    };

    //==================================================================
    //
    // LABEL METHODS
    //
    //==================================================================

    Font getLabelFont(Label& l) override
    {
        // For some reason ComboBox labels aren't scaling like other components do
        auto parent = l.getParentComponent();
        if (parent)
        {
            auto comboBox = dynamic_cast<ComboBox*>(parent);
            if (comboBox)
                return getComboBoxFont(*comboBox);            
        }
        
        Font font = l.getFont().withHeight(l.getHeight());
        
        if (parent)
        {
            // Kludge - override font size for ColourSelector label so that it doesn't get cut off, and set TextEditor background colour
            // Probably need to make a bunch of changes to the way LookAndFeel is used to avoid this
            auto parentParent = parent->getParentComponent();
            auto colourPicker = dynamic_cast<ColourSelector*>(parentParent);
            if (colourPicker)
            {
                auto stringWidth = font.getStringWidth(l.getText() + "_");
                auto labelScalar = (float)l.getWidth() / stringWidth;
                font.setHeight(l.getHeight() * labelScalar);
                l.setColour(Label::ColourIds::backgroundWhenEditingColourId, colourPicker->getCurrentColour());
            }
        }

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

    void drawLabel(Graphics& g, Label& l) override	
    {
        Path roundedBounds = getConnectedRoundedRectPath(l.getBounds().toFloat(), l.getHeight() * comboBoxRoundedCornerScalar, 0);
        g.setColour(l.findColour(Label::ColourIds::backgroundColourId));
        g.fillPath(roundedBounds);

        g.setColour(l.findColour(Label::ColourIds::textColourId));

        Font font = getLabelFont(l);
        float fontScalar = 1.0f;
        int maxLines = l.getProperties().contains(LumatoneEditorStyleIDs::labelMaximumLineCount)
            ? (int)l.getProperties()[LumatoneEditorStyleIDs::labelMaximumLineCount]
            : 1;

        if (maxLines == 1)
        {
            fontScalar *= scalarToFitString(l);
        }

        font.setHeight(font.getHeight() * fontScalar * GLOBALFONTSCALAR);
        g.setFont(font);

        g.drawFittedText(l.getText(), l.getLocalBounds(), l.getJustificationType(), maxLines, 0.0f);
        g.setColour(l.findColour(Label::ColourIds::outlineColourId));
        g.drawRect(l.getLocalBounds());
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

        Colour colour = (btn.getToggleState()) ? btn.findColour(TextButton::ColourIds::buttonOnColourId) : backgroundColour;

        if (!btn.isEnabled() && colour.isOpaque())
            colour = colour.overlaidWith(findColour(LumatoneEditorColourIDs::DisabledOverlay));

        // Do not highlight if the button is a part of a radio group and is already toggled on
        if (shouldDrawButtonAsHighlighted && !(btn.getRadioGroupId() > 0 && btn.getToggleState()))
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
            drawButtonBackground(g, btn, btn.findColour(bkgdColourId), shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

            int colourId = shouldDrawButtonAsDown ? TextButton::ColourIds::textColourOnId : TextButton::ColourIds::textColourOffId;
            Colour textColour = btn.findColour(colourId);

            if (!btn.isEnabled())
                textColour = findColour(LumatoneEditorColourIDs::InactiveText);

            else if (shouldDrawButtonAsHighlighted)
                textColour = textColour.brighter();
            
            Font font = getTextButtonFont(btn, btn.getHeight() * GLOBALFONTSCALAR);
            int margin = font.getStringWidth("  ");
            int textWidth = font.getStringWidth(btn.getButtonText());
            
            auto iconCode = (int)btn.getProperties()[LumatoneEditorStyleIDs::textButtonIconHashCode];

            int iconH = font.getHeight();// *0.9f;
            int iconW = iconH;
            int iconY = roundToInt((btn.getHeight() - iconH) * 0.5f);

            // Icon specific adjustments
            switch (iconCode)
            {
            case LumatoneEditorIcon::LoadIcon:
                iconW *= 1.125f;
                iconH *= 1.125f;
                break;
            case LumatoneEditorIcon::ArrowUp:
                margin = font.getStringWidth(" ");
                break;
            }
            
            if (iconW < 1) iconW = 1;
            if (iconH < 1) iconH = 1;

            // Set up positions of icon and text
            int lineStart = roundToInt((btn.getWidth() - textWidth - margin - iconW) * 0.5f);

            int iconX = 0;
            Rectangle<int> textBounds;

            bool iconOnRight = (bool)properties[LumatoneEditorStyleIDs::textButtonIconPlacement];
            if (iconOnRight)
            {
                iconX = lineStart + textWidth + margin;
                textBounds = btn.getLocalBounds().withLeft(lineStart).withRight(iconX - margin);
            }
            else
            {
                iconX = lineStart;
                textBounds = btn.getLocalBounds().withLeft(iconX + iconW + margin);
            }

            // Needs special case for "transparent" arrow
            if (iconCode == LumatoneEditorIcon::LoadIcon)
            {
                // Make a tad bit bigger 
                auto boundsAdj = Rectangle<float>(iconX, iconY, iconW, iconH);
                boundsAdj.expand(iconW * 0.1f, iconH * 0.1f);
                drawFolderIconAt(g, boundsAdj.getX(), boundsAdj.getY(), boundsAdj.getWidth(), boundsAdj.getHeight(), textColour, btn.findColour(bkgdColourId));
            }
            else
            {
                Path iconPath;
                PathStrokeType stroke(1.5f);
                stroke.setEndStyle(PathStrokeType::EndCapStyle::rounded);
                stroke.setJointStyle(PathStrokeType::JointStyle::curved);

                switch (LumatoneEditorIcon(iconCode))
                {
                // Using X for now
                //case LumatoneEditorIcon::Checkmark:
                //    iconPath = Path(tickBoxPath);
                //    break;
                case LumatoneEditorIcon::ArrowUp:
                {
                    iconPath = arrowUpIconPath;
                    break;
                }
                case LumatoneEditorIcon::ArrowDown:
                {
                    iconPath = arrowDownIconPath;
                    break;
                }
                case LumatoneEditorIcon::SaveIcon:
                {
                    iconPath = saveIconPath;
                    break;
                }
                default:
                    break;
                }

                auto transform = AffineTransform::scale(iconW, iconH).followedBy(AffineTransform::translation(iconX, iconY));
                iconPath.applyTransform(transform);
                g.setColour(textColour);
                g.strokePath(iconPath, stroke);
            }

            g.setColour(textColour);
            g.setFont(font);
            g.drawFittedText(btn.getButtonText(), textBounds, Justification::left, 1);
        }
        else
        {
            int bkgdColourId = (btn.getToggleState()) ? TextButton::ColourIds::buttonOnColourId : TextButton::ColourIds::buttonColourId;
            drawButtonBackground(g, btn, btn.findColour(bkgdColourId), shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

            Font font = getTextButtonFont(btn, btn.getHeight());
            int colourId = (btn.getToggleState()) ? TextButton::ColourIds::textColourOnId : TextButton::ColourIds::textColourOffId;
            
            Colour textColour = btn.findColour(colourId);

            if ((btn.getClickingTogglesState() && !btn.getToggleState()) || !shouldDrawButtonAsDown)
            {
                if (!btn.isEnabled())
                    textColour = findColour(LumatoneEditorColourIDs::InactiveText);

                if (shouldDrawButtonAsHighlighted)
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
            font.setHeight(font.getHeight() * (float)properties[LumatoneEditorStyleIDs::fontHeightScalar]);
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

    void drawToggleButton(Graphics& g, ToggleButton& btn, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        drawTickBox(g, btn, 0, 0, btn.getHeight(), btn.getHeight(), btn.getToggleState(), btn.isEnabled(),
            shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

#if JUCE_MAC
        float fontScalar = 1.0675f;
#else
        float fontScalar = 1.125f;
#endif
        g.setFont(appFonts[LumatoneEditorFont::GothamNarrowMedium].withHeight(btn.getHeight() * fontScalar));

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
        
        //Rectangle<int> bounds(x, y, width, height);
        //g.setColour(Colours::red);
        //g.fillRect(bounds);

        //g.setColour(Colours::blue.withAlpha(0.5f));
        //g.fillRect(bounds.withLeft(minSliderPos));

        //g.setColour(Colours::yellow.withAlpha(0.5f));
        //g.fillRect(bounds.withLeft(maxSliderPos));
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
        auto btn = new TextButtonMouseHighlight(Colours::white.withAlpha(0.2f));

        btn->setColour(TextButton::ColourIds::buttonColourId, Colour());
        btn->setColour(TextButton::ColourIds::buttonOnColourId, Colour());

        Colour textColour = findColour(LumatoneEditorColourIDs::DescriptionText);
        btn->setColour(TextButton::ColourIds::textColourOnId, textColour);
        btn->setColour(TextButton::ColourIds::textColourOffId, textColour);

        if (isIncrement) 
            btn->setButtonText("+"); 
        else 
            btn->setButtonText("-");

        btn->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, 1.6f);

        return btn;
    }

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& sld) override
    {
        const float halfPi = MathConstants<float>::pi * 0.5f;
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
            sld.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false,
                roundToInt(sliderSize * 0.5f),
                roundToInt((sld.getHeight() - sliderSize * 0.5f) * 0.75f)
            );

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

            // This will assume all edges are connected (default TextEditor) if not "connectedEdgeFlag" property is set

            Path boxShape;
            if ((int)editor.getProperties()[LumatoneEditorStyleIDs::connectedEdgeFlags] < 15) // 15 = All edges connected
            {
                boxShape = getConnectedRoundedRectPath(Rectangle<float>(0, 0, width, height), roundToInt(height * comboBoxRoundedCornerScalar), editor.getProperties()[LumatoneEditorStyleIDs::connectedEdgeFlags]);
            }
            else
            {
                boxShape.addRectangle(0, 0, width, height);
            }

            g.setColour(backgroundColour);
            g.fillPath(boxShape);
            return;
        }
        
        g.fillAll(backgroundColour);
    }

    void drawTextEditorOutline(Graphics&, int, int, TextEditor&) override {}



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

        auto properties = box.getProperties();
        Path boxShape; 
        
        if (properties.contains(LumatoneEditorStyleIDs::roundedDiagonalCorners))
        {
            boxShape = getDiagonalRoundedCornersPath(box.getLocalBounds().toFloat(),
                roundToInt(height * comboBoxRoundedCornerScalar),
                (int)properties[LumatoneEditorStyleIDs::roundedDiagonalCorners],
                box.isPopupActive()
            );
        }
        else
        {
            boxShape = getConnectedRoundedRectPath(box.getLocalBounds().toFloat(),
                roundToInt(height * comboBoxRoundedCornerScalar),
                (box.isPopupActive()) ? Button::ConnectedEdgeFlags::ConnectedOnBottom : 0
            );
        }
        g.fillPath(boxShape);

        int realButtonX = jmax(margin, box.getWidth() - box.getHeight());

        if (buttonW > 0)
        {
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
            font.setHeight(font.getHeight() * (float)properties[LumatoneEditorStyleIDs::fontHeightScalar]);
        }
                
        return font;
    }

    Label* createComboBoxTextBox(ComboBox& box) override
    {
        Label* l = new Label(box.getName(), box.getText());

        Colour textColour = box.findColour(ComboBox::ColourIds::textColourId);

        if (!box.isEnabled())
            textColour = textColour.overlaidWith(findColour(LumatoneEditorColourIDs::DisabledOverlay));

        l->setColour(Label::ColourIds::textColourId, textColour);
        
        for (auto prop : box.getProperties())
            l->getProperties().set(prop.name, prop.value);
        
        l->setBounds(box.getLocalBounds());
        l->setFont(getComboBoxFont(box).withHeight(box.getHeight() * CONTROLBOXFONTHEIGHTSCALAR)); // Any style overrides should have been passed to Label
        
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
        String text = (box.getNumItems()) ? box.getTextWhenNothingSelected() : box.getTextWhenNoChoicesAvailable();
        l.setText(text, NotificationType::dontSendNotification);
    }

    PopupMenu::Options getOptionsForComboBoxPopupMenu(ComboBox& box, Label& label) override
    {
        int numColumns = jmax(1, (int)box.getProperties()[LumatoneEditorStyleIDs::popupMenuMaxColumns]);

        return PopupMenu::Options()
            .withTargetComponent(box)
            .withMinimumWidth(box.getWidth())
            //.withMinimumNumColumns(numColumns)
            .withMaximumNumColumns(1)
            .withStandardItemHeight(label.getFont().getHeight() * CONTROLBOXFONTHEIGHTSCALAR)
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

        int targetWidth = (target != nullptr && target->getProperties()[LumatoneEditorStyleIDs::popupMenuTargetWidth])
            ? target->getWidth()
            : width;

        int targetMargin = (target != nullptr)
            ? target->getHeight() * comboBoxRoundedCornerScalar
            : 0;

        Colour targetColour = (target != nullptr && target->getProperties().contains(LumatoneEditorStyleIDs::popupMenuBackgroundColour))
            ? Colour::fromString(target->getProperties()[LumatoneEditorStyleIDs::popupMenuBackgroundColour].toString()).withMultipliedSaturation(1.5f) // Box colour will always be highlighted
            : findColour(LumatoneEditorColourIDs::MenuBarBackground);

        g.setColour(targetColour);
		
		Path menuShape = getConnectedRoundedRectPath(Rectangle<float>(0, 0, targetWidth, height), targetMargin, Button::ConnectedEdgeFlags::ConnectedOnTop);
		g.fillPath(menuShape);
    }

    void drawPopupMenuItemWithOptions(
        Graphics& g, const Rectangle<int>& area, bool isHighlighted, const PopupMenu::Item& item, const PopupMenu::Options& options) override
    {
        int width, height;
        getIdealPopupMenuItemSizeWithOptions(item.text, false, area.getHeight(), width, height, options);

        Rectangle<int> areaToUse = area;

        // Only using PopupMenus with ComboBoxes so this should be fine
        auto target = dynamic_cast<ComboBox*>(options.getTargetComponent());
        Font font;
        Colour textColour;
        int margin = 0;
        if (target)
        {
            font = getComboBoxFont(*target);
            textColour = target->findColour(ComboBox::ColourIds::textColourId);
            margin = target->proportionOfHeight(comboBoxRoundedCornerScalar);
            areaToUse = areaToUse.withWidth(width);
        }
        else
        {
            float fontScalar = scalarToFitString(item.text, font, width);
            font = getPopupMenuFont().withHeight(area.getHeight() * fontScalar * CONTROLBOXFONTHEIGHTSCALAR);
            textColour = findColour(LumatoneEditorColourIDs::DescriptionText);
            margin = roundToInt(height * comboBoxRoundedCornerScalar);
        }

        if (!item.isEnabled)
            textColour = textColour.overlaidWith(findColour(LumatoneEditorColourIDs::DisabledOverlay));
        
        font.setHeight(font.getHeight() * GLOBALFONTSCALAR);

        // If it's the last item, reduce size so highlight doesn't pass rounded corners
        if (target != nullptr && item.itemID == target->getItemId(target->getNumItems() - 1))
        {
            areaToUse = areaToUse.withTrimmedBottom(roundToInt(margin * 0.5f) + 1);
        }

        if (isHighlighted && !item.isSeparator)
        {
            g.setColour(Colours::white.withAlpha(0.15f));
            g.fillRect(areaToUse);
        }

        g.setColour(textColour);
        g.setFont(font);
        
        g.drawFittedText(item.text, areaToUse.withTrimmedLeft(margin).withTrimmedRight(margin), Justification::centredLeft, 1);

        if (item.subMenu)
        {
            g.drawFittedText(">", areaToUse.withTrimmedRight(margin), Justification::centredRight, 1, 0.5f);
            item.subMenu->setLookAndFeel(this);
        }
    }

    void getIdealPopupMenuItemSizeWithOptions(const String& text, bool isSeparator, int standardMenuItemHeight, int& idealWidth, int& idealHeight, 
        const PopupMenu::Options& options) override
    {
        auto target = dynamic_cast<ComboBox*>(options.getTargetComponent());

		if (target != nullptr)
		{
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
		else
		  getDefaultLookAndFeel().getIdealPopupMenuItemSizeWithOptions(text, isSeparator, standardMenuItemHeight, idealWidth, idealHeight,
			  options);
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
    // MENUBAR METHODS
    //
    //==================================================================

    void drawMenuBarBackground(Graphics& g, int w, int h, bool isMouseOverBar, MenuBarComponent& menuBar) override
    {
        g.setColour(findColour(LumatoneEditorColourIDs::MenuBarBackground));
        g.fillRect(0, 0, w, h);
    }

    void drawMenuBarItem(Graphics& g, int w, int h, int index, const String& itemText, 
        bool isMouseOver, bool isMenuOpen, bool isMouseOverBar, MenuBarComponent& menuBar) override
    {
        if (isMouseOver)
        {
            Colour bkgdColour = findColour(LumatoneEditorColourIDs::MenuBarBackground).brighter(0.2f);
            g.setColour(bkgdColour);
            g.fillRect(0, 0, w, h);
        }

        g.setColour(findColour(LumatoneEditorColourIDs::ActiveText));
        g.setFont(getAppFont(LumatoneEditorFont::GothamNarrowMedium));
        g.drawFittedText(itemText, 0, 0, w, h, Justification::centred, 1);

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

        float heightScalar = 0.54545455f * GLOBALFONTSCALAR;

        Font font = getTabButtonFont(tbb, tbb.getHeight() * heightScalar);
        
        NamedValueSet& barProperties = tbb.getTabbedButtonBar().getProperties();
        if (barProperties.contains(LumatoneEditorStyleIDs::fontHeightScalar))
            font = font.withHeight(font.getHeight() * (float)barProperties[LumatoneEditorStyleIDs::fontHeightScalar]);
        
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

    //==================================================================
    //
    // ALERTWINDOW METHODS
    //
    //==================================================================

    //virtual AlertWindow* createAlertWindow(const String& title, const String& msg, const String& btn1, const String& btn2, const String& btn3, AlertWindow::AlertIconType type, int numButtons, Component* associatedComponent) override
    //{
    //    auto window = new AlertWindow(title, msg, type, associatedComponent);
    //    window->setSize(480, 240);

    //    if (numButtons > 0)
    //    {
    //        int w = window->getWidth();
    //        int h = window->getHeight();

    //        float btnW = w / 7.0f;
    //        float allMargin = w - btnW * numButtons;
    //        float eachmargin = allMargin / (numButtons + 1);
    //        
    //        float btnH = h / 6.0f;
    //        
    //        auto firstButton = new TextButton(title + btn1);
    //        firstButton->setButtonText(btn1);
    //        firstButton->setBounds(roundToInt(eachmargin), h - 32, window->getWidth() / window->getWidth() / 7.0, 24);
    //        window->addAndMakeVisible(*firstButton);
    //    }
    //    
    //    window->setOpaque(true);

    //    return window;
    //}

    //virtual void drawAlertBox(Graphics& g, AlertWindow& window, const Rectangle<int> &textArea, TextLayout& layout) override
    //{
    //    g.setColour(findColour(AlertWindow::ColourIds::backgroundColourId));
    //    //g.fillRoundedRectangle(window.getLocalBounds().toFloat(), 15);
    //    g.fillAll();

    //    g.setColour(findColour(AlertWindow::ColourIds::outlineColourId));
    //    g.drawRect(window.getLocalBounds(), 2);

    //    g.setColour(findColour(AlertWindow::ColourIds::textColourId));
    //    float widthRatio = 0.05f;
    //    float heightRatio = 0.15f;
    //    
    //    layout.draw(g, 
    //        textArea.constrainedWithin(
    //            window.getLocalBounds().reduced(window.getWidth() * widthRatio, window.getHeight() * heightRatio)
    //        ).toFloat());
    //}

    //virtual Array<int> getWidthsForTextButtons(AlertWindow& window, const Array<TextButton*>& btns) override
    //{
    //    return getDefaultLookAndFeel().getWidthsForTextButtons(window, btns);
    //}

    //virtual int getAlertWindowButtonHeight() override
    //{
    //    return 16;
    //}

    //virtual Font getAlertWindowTitleFont() override
    //{
    //    return getAppFont(LumatoneEditorFont::UniviaProBold).withHeight(18);
    //}

    //virtual Font getAlertWindowMessageFont()  override
    //{
    //    return getAppFont(LumatoneEditorFont::GothamNarrowMedium).withHeight(16);
    //}

    //virtual Font getAlertWindowFont()  override
    //{
    //    return getAppFont(LumatoneEditorFont::GothamNarrowMedium).withHeight(14);
    //}

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

    // Set colours of Image button based on Live and Offline Editor Buttons
    // Use the same image for Normal, Highlighted, and Down, but apply alpha layer on Highlighted
    //void setupImageButton(ImageButton& btn, Image btnImage)
    //{
    //    btn.setColour(TextButton::ColourIds::buttonColourId, Colour(0xff1c1c1c));
    //    btn.setColour(TextButton::ColourIds::buttonOnColourId, Colour(0xff383b3d));

    //    btn.setImages(false, true, true,
    //        btnImage, 1.0f, Colour(),
    //        btnImage, 1.0f, Colours::white.withAlpha(0.1f),
    //        btnImage, 1.0f, Colour()
    //    );
    //}

    // Set Slider style to Rotary
    void setupRotarySlider(Slider& sld)
    {
        sld.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    }

    // Set generic TextEditor colours
    void setupTextEditor(TextEditor& editor)
    {
        editor.setIndents(4, 0);
        editor.setColour(TextEditor::ColourIds::backgroundColourId, findColour(TextEditor::ColourIds::backgroundColourId));
        editor.setColour(TextEditor::ColourIds::textColourId, findColour(TextEditor::ColourIds::textColourId));
        editor.setColour(TextEditor::ColourIds::outlineColourId, Colour());

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
        ImageCache::addImageToCache(ImageCache::getFromMemory(BinaryData::KeyboardBase_png,   BinaryData::KeyboardBase_pngSize),   LumatoneEditorAssets::LumatoneGraphic);
        ImageCache::addImageToCache(ImageCache::getFromMemory(BinaryData::KeybedShadows_png,  BinaryData::KeybedShadows_pngSize),  LumatoneEditorAssets::KeybedShadows);
        ImageCache::addImageToCache(ImageCache::getFromMemory(BinaryData::KeyShape2x_png,     BinaryData::KeyShape2x_pngSize),     LumatoneEditorAssets::KeyShape);
        ImageCache::addImageToCache(ImageCache::getFromMemory(BinaryData::KeyShadow2x_png,    BinaryData::KeyShadow2x_pngSize),    LumatoneEditorAssets::KeyShadow);
        ImageCache::addImageToCache(ImageCache::getFromMemory(BinaryData::TrashCanIcon2x_png, BinaryData::TrashCanIcon2x_pngSize), LumatoneEditorAssets::TrashCanIcon);
    }
    
    void cacheIcons()
    {
        saveIconPath = getSaveIconPath();
        arrowUpIconPath = getArrowPath(Point<float>(0.5f, 0.96f), Point<float>(0.5f, 0.08f), 0.55, 0.333f);
        arrowDownIconPath = getArrowPath(Point<float>(0.5f, 0.08f), Point<float>(0.5f, 0.96f), 0.55, 0.667f);
    }

    /// <summary>
    /// Sets the LookAndFeel colour palette to default colours
    /// </summary>
    void setupDefaultColours()
    {
        // Colour references
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
        setColour(LumatoneEditorColourIDs::MenuBarBackground,               Colour(0xff1a1b1c));

        // Component defaults
        setColour(TextButton::ColourIds::buttonOnColourId, Colour(0xff383b3d));
        setColour(TextButton::ColourIds::buttonColourId, Colour(0xff383b3d));
        setColour(TextButton::ColourIds::textColourOnId, Colours::white);
        setColour(TextButton::ColourIds::textColourOffId, Colours::white);
        
        setColour(ToggleButton::ColourIds::textColourId, findColour(LumatoneEditorColourIDs::DescriptionText));

        setColour(ComboBox::ColourIds::backgroundColourId, findColour(LumatoneEditorColourIDs::ControlBoxBackground));
        setColour(ComboBox::ColourIds::textColourId, findColour(LumatoneEditorColourIDs::DescriptionText));

        setColour(TextEditor::ColourIds::backgroundColourId, findColour(LumatoneEditorColourIDs::ControlBoxBackground));
        setColour(TextEditor::ColourIds::textColourId, findColour(LumatoneEditorColourIDs::DescriptionText));
        setColour(TextEditor::ColourIds::highlightColourId, findColour(LumatoneEditorColourIDs::ControlBoxHighlighted));
        setColour(TextEditor::ColourIds::highlightedTextColourId, findColour(LumatoneEditorColourIDs::DescriptionText));
        setColour(TextEditor::ColourIds::shadowColourId, Colour());

        setColour(AlertWindow::ColourIds::backgroundColourId, findColour(LumatoneEditorColourIDs::DarkBackground));
        setColour(AlertWindow::ColourIds::textColourId, findColour(LumatoneEditorColourIDs::DescriptionText));
        setColour(AlertWindow::ColourIds::outlineColourId, findColour(LumatoneEditorColourIDs::MediumBackground));
    }

private:

    LumatoneEditorFonts::Library appFonts;
    
    Path saveIconPath;
    Path arrowUpIconPath;
    Path arrowDownIconPath;
//    Path ccPolarityDefaultIconPath;
//    Path ccPolarityInvertedIconPath;

    // Default graphics constants
    const float buttonRoundedCornerScalar = 0.2f;

    const float comboBoxRoundedCornerScalar = 0.304878f;

    const float rotaryAngleStart = MathConstants<float>::pi * -0.64f; // pi * -2/3
    const float rotaryAngleEnd = -rotaryAngleStart;

public:
    LumatoneEditorCompactWindow compactWindowStyle;

};
