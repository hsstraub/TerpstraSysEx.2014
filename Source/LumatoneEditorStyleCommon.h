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

// Rounded corner size over app height ratio for layout elements
#define ROUNDEDCORNERTOAPPHEIGHT 0.00555556f


#define SETTINGSAREAMARGINHEIGHT 0.1714f
#define SETTINGSLABELHEIGHT      0.13f
#define SETTINGSLABELMARGINWIDTH 0.01f

#define SETTINGSCONTROLMARGINTOAPPWIDTH 0.01171875f
#define SETTINGSTOGGLEHEIGHTSCALAR      0.087f

#if JUCE_MAC
    #define GLOBALFONTSCALAR 0.9f
    #define CONTROLBOXFONTHEIGHTSCALAR 0.7f
#elif JUCE_WINDOWS
    #define GLOBALFONTSCALAR 1.0f
    #define CONTROLBOXFONTHEIGHTSCALAR 0.8f
#elif JUCE_LINUX
    #define GLOBALFONTSCALAR 1.0f
    #define CONTROLBOXFONTHEIGHTSCALAR 0.73f
#endif

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

    // TODO: finish implementing 3 and 12
    if (connectedFlags == 0 || connectedFlags == 3 || connectedFlags == 12)
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

// TODO: Integrate this into the above function somehow
static Path getDiagonalRoundedCornersPath(Rectangle<float> bounds, float roundedCornerSize, bool roundedTopRightAndBottomLeft, bool hasPopup = false)
{
    Path rect;
    roundedCornerSize *= 2;
    int xTo, yTo;

    if (roundedTopRightAndBottomLeft)
    {
        rect.startNewSubPath(bounds.getPosition());

        xTo = bounds.getRight() - roundedCornerSize;
        yTo = bounds.getY();
        rect.lineTo(xTo, yTo);
        rect.addArc(xTo, yTo, roundedCornerSize, roundedCornerSize, 0, PATH_PI_2_CW);

        xTo = bounds.getRight();
        yTo = bounds.getBottom();
        rect.lineTo(xTo, yTo);

        xTo = bounds.getX();
        if (!hasPopup)
        {
            xTo += roundedCornerSize;
            rect.lineTo(xTo, yTo);
            rect.addArc(xTo - roundedCornerSize, yTo - roundedCornerSize, roundedCornerSize, roundedCornerSize, -float_Pi, PATH_PI_2_CCW);
        }
        else
        {
            rect.lineTo(xTo, yTo);
        }
    }
    else
    {
        rect.startNewSubPath(bounds.getTopRight());

        xTo = bounds.getRight();
        yTo = bounds.getBottom();

        if (!hasPopup)
        {
            yTo -= roundedCornerSize;
            rect.lineTo(xTo, yTo);
            rect.addArc(xTo - roundedCornerSize, yTo, roundedCornerSize, roundedCornerSize, PATH_PI_2_CW, float_Pi);
        }
        else
        {
            rect.lineTo(xTo, yTo);
        }

        xTo = bounds.getX();
        yTo = bounds.getBottom();
        rect.lineTo(xTo, yTo);

        yTo = bounds.getY() + roundedCornerSize;
        rect.lineTo(xTo, yTo);

        rect.addArc(xTo, yTo - roundedCornerSize, roundedCornerSize, roundedCornerSize, PATH_PI_2_CCW, 0);
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

///// <summary>
///// Intended for use with components in FlexBoxes, this scales a component and sets the justifications within the FlexBox
///// </summary>
///// <param name="component"></param>
///// <param name="scaleFactorX"></param>
///// <param name="scaleFactorY"></param>
///// <param name="justification"></param>
//static void scaleAndJustifyComponent(Component& component, float scaleFactorX, float scaleFactorY, Justification justification)
//{
//    int newWidth = round(component.getWidth() * scaleFactorX);
//    int newHeight = round(component.getHeight() * scaleFactorY);
//
//    int xOff, yOff;
//
//    if (justification.getFlags() & Justification::left)
//    {
//        xOff = 0;
//    }
//    else
//    {
//        if (justification.getFlags() & Justification::right | Justification::centred)
//        {
//            xOff = component.getWidth() - newWidth;
//        }
//
//        if (justification.getFlags() & Justification::horizontallyCentred)
//        {
//            xOff = round(xOff * 0.5f);
//        }
//    }
//
//    if (justification.getFlags() & Justification::top)
//    {
//        yOff = 0;
//    }
//    else
//    {
//        if (justification.getFlags() & Justification::bottom | Justification::centred)
//        {
//            yOff = component.getHeight() - newHeight;
//        }
//
//        if (justification.getFlags() & Justification::verticallyCentred)
//        {
//            yOff = round(yOff * 0.5f);
//        }
//    }
//
//    component.setBounds(component.getX() + xOff, component.getY() + yOff, newWidth, newHeight);
//}
//
///// <summary>
///// Intended for use with components in FlexBoxes, this scales a component and sets the justifications within the FlexBox
///// </summary>
///// <param name="component"></param>
///// <param name="scaleFactor"></param>
///// <param name="justification"></param>
//static void scaleAndJustifyComponent(Component& component, float scaleFactor, Justification justification = Justification::centred)
//{
//    scaleAndJustifyComponent(component, scaleFactor, scaleFactor, justification);
//}

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
//static void setWidthRetainingAspectRatio(ImageComponent* component, int widthIn)
//{
//    setWidthRetainingAspectRatio(component, component->getImage(), widthIn);
//}

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

static void resizeLabelWithHeight(Label* label, int height, float fontHeightScalar = 1.0f, String textSuffix = "_")
{
    label->setFont(label->getFont().withHeight(height * fontHeightScalar * GLOBALFONTSCALAR));
    label->setSize(round(label->getFont().getStringWidthFloat(label->getText() + textSuffix)), height);
}

//static void positionLabelWithHeight(Label* label, int xPosition, int yPosition, int height, float fontHeightScalar = 1.0f, String textSuffix = "_")
//{
//    label->setFont(label->getFont().withHeight(height * fontHeightScalar));
//    label->setBounds(
//        xPosition, yPosition,
//        round(label->getFont().getStringWidthFloat(label->getText() + textSuffix)), height
//    );
//}

static void resizeLabelWithWidth(Label* label, int width, float fontHeightScalar = 1.0f)
{
    float heightOverWidth = label->getFont().getHeight() / label->getFont().getStringWidthFloat(label->getText());
    label->setSize(width, round(heightOverWidth * width));
    label->setFont(label->getFont().withHeight(label->getHeight() * fontHeightScalar * GLOBALFONTSCALAR));
}

// For scaling Labels down so they're not clipped or skewed
static float scalarToFitString(String lineOfText, Font font, float width)
{
    float scalar = 1.0f;
    float fullWidth = font.getStringWidthFloat(lineOfText);
    if (fullWidth > width)
    {
        scalar = width / fullWidth;
    }
    return scalar;
}

// For scaling Labels down so they're not clipped or skewed
static float scalarToFitString(Label& labelIn)
{
    return scalarToFitString(labelIn.getText(), labelIn.getFont(), labelIn.getWidth());
}

//static void positionLabelWithWidth(Label* label, int xPosition, int yPosition, int width, float fontHeightScalar = 1.0f)
//{
//    float heightOverWidth = label->getFont().getHeight() / label->getFont().getStringWidthFloat(label->getText());
//    label->setBounds(xPosition, yPosition, width, round(heightOverWidth * width));
//    label->setFont(label->getFont().withHeight(label->getHeight() * fontHeightScalar * GLOBALFONTSCALAR));
//}

static void resizeToggleButtonWithHeight(ToggleButton* btn, Font font, int heightIn, String textSuffix = "_")
{
    font.setHeight(font.getHeight() * GLOBALFONTSCALAR);
    btn->setSize(btn->getHeight() + round(font.getStringWidth(btn->getButtonText() + textSuffix)), heightIn);
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

static Path getArrowPath(Point<float> start, Point<float> headPoint, float headWidth, float headHeight)
{
    headWidth *= 0.5f;
    auto left = Point<float>(start.x - headWidth, headHeight);
    auto right = Point<float>(start.x + headWidth, headHeight);

    Path path;

    path.startNewSubPath(start);
    path.lineTo(headPoint);
    path.closeSubPath();
    path.startNewSubPath(headPoint);
    path.lineTo(left);
    path.closeSubPath();
    path.startNewSubPath(headPoint);
    path.lineTo(right);
    path.closeSubPath();

    return path;
}

static juce::Path getFolderIcon(float folderHeight, float tabBaseX, float tabBaseY)
{
    juce::Path path;

    float tabBaseSlopeX = tabBaseX - 0.1f;
    float folderTop = 1.0f - folderHeight;

    auto v0 = Point<float>(0.0f, folderTop);
    auto v1 = Point<float>(0.0f, folderHeight);
    auto v2 = Point<float>(1.0f, folderHeight);
    auto v3 = Point<float>(1.0f, tabBaseY);
    auto v4 = Point<float>(tabBaseX, tabBaseY);
    auto v5 = Point<float>(tabBaseSlopeX, folderTop);

    path.startNewSubPath(v0);
    path.lineTo(v1);
    path.lineTo(v2);
    path.lineTo(v3);
    path.lineTo(v4);
    path.lineTo(v5);
    path.closeSubPath();

    return path;
}


static void drawFolderIconAt(Graphics& g, int x, int y, int width, int height, Colour folderColour, Colour arrowColour)
{
    float folderHeight = 1.0f;
    Path folder = getFolderIcon(0.91f, 0.5f, 0.22f).createPathWithRoundedCorners(0.05f);

    auto transform = AffineTransform::scale(width, height).followedBy(AffineTransform::translation(x, y));
    folder.applyTransform(transform);

    g.setColour(folderColour);
    g.fillPath(folder);

    Path arrowPath = getArrowPath(Point<float>(0.5f, folderHeight - 0.267f), Point<float>(0.5f, 0.35f), 0.2f, 0.46f);
    arrowPath.applyTransform(transform);

    PathStrokeType stroke(1.25f);
    stroke.setEndStyle(PathStrokeType::EndCapStyle::rounded);
    stroke.setJointStyle(PathStrokeType::JointStyle::curved);
    
    g.setColour(arrowColour);
    g.strokePath(arrowPath, stroke);
}

static Path getSaveIconPath()
{
    float boxHeight = 0.6f;
    float boxWidth = 1.0f;
    boxWidth *= 0.5f;

    float boxLeft = 0.5f - boxWidth;
    float boxRight = 0.5f + boxWidth;

    // Box
    auto v0 = juce::Point<float>(boxLeft, boxHeight);
    auto v1 = juce::Point<float>(boxLeft, 1.0f);
    auto v2 = juce::Point<float>(boxRight, 1.0f);
    auto v3 = juce::Point<float>(boxRight, boxHeight);

    juce::Path path;
    path.startNewSubPath(v0);
    path.lineTo(v1);
    path.closeSubPath();
    path.startNewSubPath(v1);
    path.lineTo(v2);
    path.closeSubPath();
    path.startNewSubPath(v2);
    path.lineTo(v3);
    path.closeSubPath();

    Path rounded = path.createPathWithRoundedCorners(0.01f);

    Path arrow = getArrowPath(Point<float>(0.5f, 0.0f), Point<float>(0.5f, 0.667f), 0.4f, 0.5f);
    rounded.addPath(arrow);

    return rounded;
}

static void getCCPolarityIconPath(bool inverted, Path& arrowPath, Path& faderPath)
{
    float glyphWidth = 0.25f;
    
    float y1 = 0.8f;
    float y2 = 1.0f - y1;
    
    float arrowX = 0.3f;
    float arrowHeadDif = abs(y1 - y2) * 0.333;
    
    float faderMax = 0.81f;
    float faderEdge = faderMax - glyphWidth;
    
    if (inverted)
    {
        arrowPath = getArrowPath(Point<float>(arrowX, y1), Point<float>(arrowX, y2), glyphWidth, y2 + arrowHeadDif);
        
        faderPath.startNewSubPath(faderEdge, y1);
        faderPath.lineTo(faderEdge, y2);
        faderPath.lineTo(faderMax, y2);
        faderPath.closeSubPath();
    }
    else
    {
        arrowPath = getArrowPath(Point<float>(arrowX, y2), Point<float>(arrowX, y1), glyphWidth, y1 - arrowHeadDif);
        
        faderPath.startNewSubPath(faderEdge, y1);
        faderPath.lineTo(faderMax, y1);
        faderPath.lineTo(faderEdge, y2);
        faderPath.closeSubPath();
    }
}

//static void drawSaveIconAt(Graphics& g, int x, int y)


// Hash codes for use with ImageCache::getFromHashCode()
enum LumatoneEditorAssets
{
    //LoadIcon            = 0x0002000,
    //SaveIcon            = 0x0002001,
    //ImportIcon          = 0x0002002,
    LumatoneGraphic     = 0x0002100,
    KeybedShadows       = 0x0002101,
    KeyShape            = 0x0002200,
    KeyShadow           = 0x0002201,
    TickBox             = 0x0003100,
    SavePalette         = 0x0005000,
    CancelPalette       = 0x0005001,
    TrashCanIcon        = 0x0005002
};

enum LumatoneEditorIcon
{
    Checkmark   = 0x01,
    ArrowUp,
    ArrowDown,
    SaveIcon,
    LoadIcon,
    CCPolarityIcon
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
    ControlBoxHighlighted,
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
    RotaryGradientMax,
    DisabledOverlay,
    MenuBarBackground
};

namespace LumatoneEditorStyleIDs
{    
    // Index of LumatoneEditorFontCollection
    static Identifier fontOverride = Identifier("AppFontIndex");
    // TODO: Review with new Font access method
    static Identifier fontOverrideTypefaceStyle = Identifier("ComponentTypefaceStyle");

    // A component with text can have it scaled with this property
    static Identifier fontHeightScalar = Identifier("FontHeightScalar");

    // Override background colour for a Label's TextEditor when being edited
    static Identifier labelTextEditorBackgroundColour = Identifier("LabelTextEditorBackgroundColour");

    static Identifier labelMaximumLineCount = Identifier("LabelMaximumLineCount");

    static Identifier tabbedButtonBarDepthScalar = Identifier("TabbedButtonBarDepthScalar");

    // The LumatoneEditorIcon hash code for an icon that should be displayed on a TextButton
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
    // Uses the width of the target component of a menu instead of parameters
    static Identifier popupMenuTargetWidth = Identifier("PopupMenuTargetWidth");

    static Identifier popupMenuBackgroundColour = Identifier("PopupMenuBackgroundColour");

    static Identifier connectedEdgeFlags = Identifier("ConnectedEdgesFlags");

    // Odd values will have top right and bottom left rounded corners
    // Even values will have top left and bottom right rounded corners
    static Identifier roundedDiagonalCorners = Identifier("RoundedDiagonalCorners");

}
