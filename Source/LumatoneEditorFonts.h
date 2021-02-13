/*
  ==============================================================================

    LumatoneEditorFonts.h
    Created: 12 Feb 2021 10:52:11pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#define LICENSEDFONTENVVAR "LUMATONE_EDITOR_USE_LICENSED_FONTS"

typedef enum
{
    UniviaPro = 0,
    UniviaProBold,
    FranklinGothic,
    GothamNarrowMedium,
    GothamNarrowMediumBold,
    GothamNarrowMediumItalic
} LumatoneEditorFont;

typedef Array<Font> LumatoneEditorFontCollection;

/// <summary>
/// Returns whether or not the licensed font environment variable is defined.
/// </summary>
/// <returns></returns>
static bool useLicensedFonts()
{
    return getenv(LICENSEDFONTENVVAR);
}

struct LumatoneEditorFonts
{
    LumatoneEditorFontCollection fonts;

    LumatoneEditorFonts()
    {
        // Setup default fallback fonts

        // In place of...
        // UniviaPro
        fonts.add(Font("Tahoma", 12.0f, Font::plain));

        // UniviaProBold
        fonts.add(Font("Tahoma", 12.0f, Font::bold));

        // FranklinGothic
        fonts.add(Font("Arial", 12.0f, Font::plain));

        // GothamNarrowMedium
        fonts.add(Font("Trebuchet MS", 12.0f, Font::plain));

        // GothamNarrowMediumBold
        fonts.add(Font("Trebuchet MS", 12.0f, Font::bold));

        // GothamNarrowMediumItalic
        fonts.add(Font("Trebuchet MS", 12.0f, Font::italic));


        // Define environment variable LUMATONE_EDITOR_USE_LICENSED_FONTS
        // if you have the official fonts and want to build with them
        if (useLicensedFonts())
        {
            fonts.set(LumatoneEditorFont::UniviaPro,
                Font(Typeface::createSystemTypefaceFor(BinaryData::UniviaProRegular_ttf, BinaryData::UniviaProRegular_ttfSize))
            );

            fonts.set(LumatoneEditorFont::UniviaProBold,
                Font(Typeface::createSystemTypefaceFor(BinaryData::UniviaProBold_ttf, BinaryData::UniviaProBold_ttfSize))
            );

            fonts.set(LumatoneEditorFont::FranklinGothic,
                Font(Typeface::createSystemTypefaceFor(BinaryData::framd_ttf, BinaryData::framd_ttfSize))
            );

            fonts.set(LumatoneEditorFont::GothamNarrowMedium,
                Font(Typeface::createSystemTypefaceFor(BinaryData::GothamNarrowMedium_otf, BinaryData::GothamNarrowMedium_otfSize))
            );

            fonts.set(LumatoneEditorFont::GothamNarrowMediumBold,
                Font(Typeface::createSystemTypefaceFor(BinaryData::GothamNarrowBold_otf, BinaryData::GothamNarrowBoldItalic_otfSize))
            );
            
            fonts.set(LumatoneEditorFont::GothamNarrowMediumBold,
                Font(Typeface::createSystemTypefaceFor(BinaryData::GothamNarrowLightItalic_otf, BinaryData::GothamNarrowLightItalic_otfSize))
            );
        }
    }

    Font getFont(LumatoneEditorFont fontId, float heightIn)
    {
        return fonts[fontId].withHeight(heightIn);
    }
};