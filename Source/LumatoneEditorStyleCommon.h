/*
  ==============================================================================

    LumatoneEditorStyleCommon.h
    Created: 13 Dec 2020 8:49:14pm
    Author:  Vincenzo

    Helper functions for various positioning and sizing tasks.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ImageResampling/ImageResampler.h"

//==================================================================
//
// STATIC HELPERS
//
//==================================================================

#define PHI 1.618033989f // (1 + sqrt(5)) / 2
#define HEXRADIUSTOLATERAL 0.8660254f // sqrt(3) / 2

// JUCE methods' radians are rotated -90deg with respect to trigonometry functions
#define PATH_PI_2_CW   1.5707963 // pi / 2
#define PATH_PI_2_CCW -1.5707963

// Rounded corner ratio of app neight for layout elements
static const float roundedCornerLayoutAppHeightScalar       = 1.0f / 180.0f;

static const float settingsAreaMarginHeight                 = 0.1714f;
static const float settingsLabelHeight                      = 0.14f;
static const float settingsLabelMarginWidth                 = 0.01f;

static const float settingsControlMarginParentWidthScalar   = 0.01171875f;
static const float settingsToggleButtonHeight               = 0.087f;


/// <summary>
/// Returns a path representing a rectangle shape, considering rounded corners and connected edges
/// </summary>
/// <param name="bounds"></param>
/// <param name="roundedCornerSize"></param>
/// <param name="connectedFlags"></param>
/// <returns></returns>
static Path getConnectedRoundedRectPath(Rectangle<float> bounds, float roundedCornerSize, int connectedFlags)
{
    Path rect;
    Point<float> origin = bounds.getPosition();
    Point<float> endpoint = bounds.getBottomRight();

    if (connectedFlags == 0 || connectedFlags == 3 || connectedFlags == 12 || connectedFlags == 15)
    {
        rect.addRoundedRectangle(bounds, roundedCornerSize);
        return rect;
    }

    roundedCornerSize *= 2;

    int xTo, yTo;

    if (connectedFlags & Button::ConnectedEdgeFlags::ConnectedOnTop || connectedFlags & Button::ConnectedOnLeft)
    {
        rect.startNewSubPath(origin);
    }
    else
    {
        rect.addArc(origin.x, origin.y, roundedCornerSize, roundedCornerSize, PATH_PI_2_CCW, 0, true);
    }

    if (connectedFlags & Button::ConnectedEdgeFlags::ConnectedOnTop || connectedFlags & Button::ConnectedEdgeFlags::ConnectedOnRight)
    {
        rect.lineTo(endpoint.x, origin.y);
    }
    else
    {
        xTo = endpoint.x - roundedCornerSize;
        rect.lineTo(xTo, origin.y);
        rect.addArc(xTo, origin.y , roundedCornerSize, roundedCornerSize, 0, PATH_PI_2_CW);
    }

    if (connectedFlags & Button::ConnectedEdgeFlags::ConnectedOnBottom || connectedFlags & Button::ConnectedEdgeFlags::ConnectedOnRight)
    {
        rect.lineTo(endpoint);
    }
    else
    {
        yTo = endpoint.y - roundedCornerSize;
        rect.lineTo(endpoint.x, yTo);
        rect.addArc(endpoint.x - roundedCornerSize, yTo, roundedCornerSize, roundedCornerSize, PATH_PI_2_CW, float_Pi);
    }

    if (connectedFlags & Button::ConnectedEdgeFlags::ConnectedOnBottom || connectedFlags & Button::ConnectedEdgeFlags::ConnectedOnLeft)
    {
        rect.lineTo(origin.x, endpoint.y);
    }
    else
    {
        rect.lineTo(roundedCornerSize, endpoint.y);
        rect.addArc(origin.x, endpoint.y - roundedCornerSize, roundedCornerSize, roundedCornerSize, -float_Pi, PATH_PI_2_CCW);
    }

    rect.closeSubPath();
    return rect;
}

/// <summary>
/// Adds an arc to a Path using a juce::Rectangle object for bounds.
/// </summary>
/// <param name="pathIn"></param>
/// <param name="ellipseBounds"></param>
/// <param name="fromRadians"></param>
/// <param name="toRadians"></param>
/// <param name="startAsNewSubPath"></param>
static void addArcToPath(Path& pathIn, Rectangle<float>& ellipseBounds, float fromRadians, float toRadians, bool startAsNewSubPath)
{
    pathIn.addArc(
        ellipseBounds.getX(),
        ellipseBounds.getY(),
        ellipseBounds.getWidth(),
        ellipseBounds.getHeight(),
        fromRadians,
        toRadians,
        startAsNewSubPath
    );
}

/// <summary>
/// Intended for use with components in FlexBoxes, this scales a component and sets the justifications within the FlexBox
/// </summary>
/// <param name="component"></param>
/// <param name="scaleFactorX"></param>
/// <param name="scaleFactorY"></param>
/// <param name="justification"></param>
static void scaleAndJustifyComponent(Component& component, float scaleFactorX, float scaleFactorY, Justification justification)
{
    int newWidth = round(component.getWidth() * scaleFactorX);
    int newHeight = round(component.getHeight() * scaleFactorY);

    int xOff, yOff;

    if (justification.getFlags() & Justification::left)
    {
        xOff = 0;
    }
    else
    {
        if (justification.getFlags() & Justification::right | Justification::centred)
        {
            xOff = component.getWidth() - newWidth;
        }

        if (justification.getFlags() & Justification::horizontallyCentred)
        {
            xOff = round(xOff * 0.5f);
        }
    }

    if (justification.getFlags() & Justification::top)
    {
        yOff = 0;
    }
    else
    {
        if (justification.getFlags() & Justification::bottom | Justification::centred)
        {
            yOff = component.getHeight() - newHeight;
        }

        if (justification.getFlags() & Justification::verticallyCentred)
        {
            yOff = round(yOff * 0.5f);
        }
    }

    component.setBounds(component.getX() + xOff, component.getY() + yOff, newWidth, newHeight);
}

/// <summary>
/// Intended for use with components in FlexBoxes, this scales a component and sets the justifications within the FlexBox
/// </summary>
/// <param name="component"></param>
/// <param name="scaleFactor"></param>
/// <param name="justification"></param>
static void scaleAndJustifyComponent(Component& component, float scaleFactor, Justification justification = Justification::centred)
{
    scaleAndJustifyComponent(component, scaleFactor, scaleFactor, justification);
}

/// <summary>
/// Sets the width of a component while retaining the aspect ratio of a given image
/// </summary>
/// <param name="component"></param>
/// <param name="image"></param>
/// <param name="widthIn"></param>
static void setWidthRetainingAspectRatio(Component* component, const Image& image, int widthIn)
{
    component->setSize(widthIn, round(image.getHeight() / (float) image.getWidth() * widthIn));
}

/// <summary>
/// Sets the height of a component while retaining the aspect ratio of a given image
/// </summary>
/// <param name="component"></param>
/// <param name="image"></param>
/// <param name="widthIn"></param>
static void setHeightRetainingAspectRatio(Component* component, const Image& image, int heightIn)
{
    component->setSize(round(image.getWidth() / (float) image.getHeight() * heightIn), heightIn);
}

/// <summary>
/// Sets the width of an ImageComponent while retaining the aspect ratio of its image
/// </summary>
/// <param name="component"></param>
/// <param name="image"></param>
/// <param name="widthIn"></param>
static void setWidthRetainingAspectRatio(ImageComponent* component, int widthIn)
{
    setWidthRetainingAspectRatio(component, component->getImage(), widthIn);
}

/// <summary>
/// Sets the height of an ImageComponent while retaining the aspect ratio of its image
/// </summary>
/// <param name="component"></param>
/// <param name="image"></param>
/// <param name="widthIn"></param>
static void setHeightRetainingAspectRatio(ImageComponent* component, int heightIn)
{
    setHeightRetainingAspectRatio(component, component->getImage(), heightIn);
}

/// <summary>
/// Sets the width of an ImageButton while retaining the aspect ratio of its normal image
/// </summary>
/// <param name="component"></param>
/// <param name="image"></param>
/// <param name="widthIn"></param>
static void setWidthRetainingAspectRatio(ImageButton* component, int widthIn)
{
    setWidthRetainingAspectRatio(component, component->getNormalImage(), widthIn);
}

/// <summary>
/// Sets the height of an ImageButton while retaining the aspect ratio of its normal image
/// </summary>
/// <param name="component"></param>
/// <param name="image"></param>
/// <param name="widthIn"></param>
static void setHeightRetainingAspectRatio(ImageButton* component, int heightIn)
{
    setHeightRetainingAspectRatio(component, component->getNormalImage(), heightIn);
}

static void resizeLabelWithHeight(Label* label, int height, float fontHeightScalar = 1.0f)
{
    label->setFont(label->getFont().withHeight(height * fontHeightScalar));
    label->setSize(round(label->getFont().getStringWidthFloat(label->getText() + "__")), height);
}

static void positionLabelWithHeight(Label* label, int xPosition, int yPosition, int height, float fontHeightScalar = 1.0f)
{
    label->setFont(label->getFont().withHeight(height * fontHeightScalar));
    label->setBounds(
        xPosition, yPosition,
        round(label->getFont().getStringWidthFloat(label->getText() + "__")), height
    );
}

static void resizeLabelWithWidth(Label* label, int width, float fontHeightScalar = 1.0f)
{
    float heightOverWidth = label->getFont().getHeight() / label->getFont().getStringWidthFloat(label->getText());
    label->setSize(width, round(heightOverWidth * width));
    label->setFont(label->getFont().withHeight(label->getHeight() * fontHeightScalar));
}

static void positionLabelWithWidth(Label* label, int xPosition, int yPosition, int width, float fontHeightScalar = 1.0f)
{
    float heightOverWidth = label->getFont().getHeight() / label->getFont().getStringWidthFloat(label->getText());
    label->setBounds(xPosition, yPosition, width, round(heightOverWidth * width));
    label->setFont(label->getFont().withHeight(label->getHeight() * fontHeightScalar));
}

static void resizeToggleButtonWithHeight(ToggleButton* btn, Font font, int heightIn)
{
    btn->setSize(btn->getHeight() + round(font.getStringWidth(btn->getButtonText() + "_")), heightIn);
}

static void drawPathToFillBounds(Graphics& g, const Path& path, Rectangle<float> boundsToFill)
{
    AffineTransform transform = RectanglePlacement().getTransformToFit(
        path.getBounds(),
        boundsToFill
    );

    g.strokePath(path, PathStrokeType(0.8f), transform);
}

// Creates the Lumatone logomark in 1x1 floating bit path
static Path createLogomark()
{
    Path logo;
    Point<float> center(0.5f, 0.5f);

    float phi2 = PHI * 2;
    float innerRad = 1.0f / phi2;
    float outerRad = phi2 * 0.125f;
    float ang = float_Pi * 0.083333f;
    float angOff = ang * 0.5f;

    logo.addPolygon(center, 6, innerRad, ang - angOff);
    logo.addPolygon(center, 6, HEXRADIUSTOLATERAL * innerRad, -ang - angOff);
    logo.addPolygon(center, 6, outerRad, ang - angOff);
    logo.addPolygon(center, 6, outerRad, -ang - angOff);

    return logo;
}

// Hash codes for use with ImageCache::getFromHashCode()
enum LumatoneEditorAssets
{
    LoadIcon            = 0x0002000,
    SaveIcon            = 0x0002001,
    ImportIcon          = 0x0002002,
    LumatoneGraphic     = 0x0002100,
    KeybedShadows       = 0x0002101,
    KeyShape            = 0x0002200,
    KeyShadow           = 0x0002201,
    TickBox             = 0x0003100,
    SavePalette         = 0x0005000,
    CancelPalette       = 0x0005001,
    TrashCanIcon        = 0x0005002
};

enum LumatoneEditorColourIDs
{
    LabelPink,
    LabelBlue,
    DisconnectedRed,
    ConnectedGreen,
    DarkBackground,
    MediumBackground,
    LightBackground,
    ControlAreaBackground,
    ActiveText,
    InactiveText,
    DescriptionText,
    ControlBoxBackground,
    DefaultPresetButtonActive,
    DefaultPresetButtonInactive,
    OutlineColourId,
    CurveGradientMin,
    CurveGradientMax,
    CurveGridColour,
    RotaryGradientMin,
    RotaryGradientMax
};

// Static functions for getting default Lumatone Editor fonts
struct LumatoneEditorFonts
{
    static Font UniviaPro                (float height = 12.0f)  { return Font("Univia Pro",       height, Font::plain);                                          }  
    static Font UniviaProBold            (float height = 12.0f)  { return Font("Univia Pro",       height, Font::bold );                                          }
    static Font FranklinGothic           (float height = 12.0f)  { return Font("Franklin Gothic",  height, Font::plain).withTypefaceStyle("Medium");              }
    static Font GothamNarrowMedium       (float height = 12.0f)  { return Font("Gotham",           height, Font::plain).withTypefaceStyle("Narrow Medium");       }
    static Font GothamNarrowMediumBold   (float height = 12.0f)  { return       GothamNarrowMedium(height)             .withTypefaceStyle("Narrow Bold");         }
    static Font GothamNarrowMediumItalic (float height = 12.0f)  { return       GothamNarrowMedium(height)             .withTypefaceStyle("Narrow Light Italic"); }
};

namespace LumatoneEditorStyleIDs
{    
    static Identifier fontOverride = Identifier("ComponentFont");
    static Identifier fontOverrideTypefaceStyle = Identifier("ComponentTypefaceStyle");

    // A component with text can have it scaled with this property
    static Identifier fontHeightScalar = Identifier("FontHeightScalar");

    static Identifier tabbedButtonBarDepthScalar = Identifier("TabbedButtonBarDepthScalar");

    // The Image::Cache hash code for an icon that should be displayed on a TextButton
    static Identifier textButtonIconHashCode = Identifier("TextButtonIconHashCode");

    // Choose where to put the icon on a TextButton
    static Identifier textButtonIconPlacement = Identifier("TextButtonIconPlacement");
    enum TextButtonIconPlacement
    {
        LeftOfText = 0,
        RightOfText
    };

    // A TextButton will have an invisible background and an underline if it's properties contains this
    static Identifier textButtonHyperlinkFlag = Identifier("TextButtonHyperlinkFlag");

    static Identifier popupMenuMaxColumns = Identifier("PopupMenuMaxColumns");
}