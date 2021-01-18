/*
  ==============================================================================

    ColourPaletteDataStructure.h
    Created: 17 Jan 2021 9:55:13pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#define COLOURPALETTESIZE 6
#define STRINGCOLOURSIZE 8

//==============================================================================
/*
* Container for colour palette data
*/
class LumatoneColourPalette
{
public:

    LumatoneColourPalette()
    {
        colourPalette.reset(new Array<Colour>());
        palette = colourPalette.get();
    }

    LumatoneColourPalette(Array<Colour> colourPaletteIn)
        : LumatoneColourPalette()
    {
        for (auto colour : colourPaletteIn)
            palette->add(colour);
    }

    LumatoneColourPalette(String paletteAsString)
        : LumatoneColourPalette()
    {
        int i = 0;
        int nextInd = STRINGCOLOURSIZE;
        while(i < paletteAsString.length())
        {
            colourPalette->add(Colour::fromString(paletteAsString.substring(i, nextInd)));
            i = nextInd;
            nextInd = i + STRINGCOLOURSIZE;
        }
    }

    LumatoneColourPalette(const LumatoneColourPalette& palette)
        : LumatoneColourPalette(*palette.palette) {}

    ~LumatoneColourPalette()
    {
        palette = nullptr;
        colourPalette = nullptr;
    }

    int size() const { return palette->size(); }

    String toString() const
    {
        String colourString;
        for (auto c : *colourPalette)
            colourString += c.toString();

        return colourString;
    }

    Array<Colour>* palette;

    // Generic key for colour palette index; update when saving to file
    String id;

public:

    // Palettes should be separated by a space
    static Array<LumatoneColourPalette> loadPalettesFromString(String stringIn)
    {
        Array<LumatoneColourPalette> palettesOut;

        int i = 0;
        while (i < stringIn.length())
        {
            int endIndex = stringIn.indexOf(i, " ");
            if (endIndex < 0)
                endIndex = stringIn.length();

            auto paletteString = stringIn.substring(i, endIndex);
            if (paletteString.length() > 0)
                palettesOut.add(LumatoneColourPalette(stringIn.substring(i, endIndex)));
            
            i += endIndex + 1;
        }
        
        return palettesOut;
    }

    static String paletteArrayToString(const Array<LumatoneColourPalette>& palettes)
    {
        String palettesString;
        for (int p = 0; p < palettes.size(); p++)
        {
            palettesString += palettes.getReference(p).toString();
            palettesString += ' ';
        }

        return palettesString;
    }

private:

    std::unique_ptr<Array<Colour>> colourPalette;

};
