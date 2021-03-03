/*
  ==============================================================================

    LumatoneEditorFonts.h
    Created: 12 Feb 2021 10:52:11pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

typedef enum
{
    UniviaPro = 0,
    UniviaProBold,
    FranklinGothic,
    GothamNarrowMedium,
    GothamNarrowBold,
    GothamNarrowItalic,
    GothamNarrowLight
    
} LumatoneEditorFont;

struct LumatoneEditorFonts
{
    typedef Array<Font> Library;
    LumatoneEditorFonts::Library fonts;

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

        // GothamNarrowMediumLight
        fonts.add(Font("Trebuchet MS", 12.0f, Font::plain));

#if USE_LICENSED_FONTS
        
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

        fonts.set(LumatoneEditorFont::GothamNarrowBold,
              Font(Typeface::createSystemTypefaceFor(BinaryData::GothamNarrowBold_otf, BinaryData::GothamNarrowBold_otfSize))
        );

        fonts.set(LumatoneEditorFont::GothamNarrowItalic,
              Font(Typeface::createSystemTypefaceFor(BinaryData::GothamNarrowLightItalic_otf, BinaryData::GothamNarrowLightItalic_otfSize))
        );
        
        fonts.set(LumatoneEditorFont::GothamNarrowLight,
              Font(Typeface::createSystemTypefaceFor(BinaryData::GothamNarrowLight_otf, BinaryData::GothamNarrowLight_otfSize))
        );
        
#endif
    }

    Font getFont(LumatoneEditorFont fontId, float heightIn)
    {
        return fonts[fontId].withHeight(heightIn);
    }
};
