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


namespace LumatoneEditorPaletteIds
{
    static Identifier libraryId = Identifier("PaletteLibrary");
    static Identifier paletteId = Identifier("ColourPalette");
    static Identifier swatchId = Identifier("Swatches");
    static Identifier colourId = Identifier("Colour");
    static Identifier nameId = Identifier("Name");
}

//==============================================================================
/*
* Container for colour palette data
*/
struct LumatoneEditorColourPalette
{
    LumatoneEditorColourPalette()
    {
        colourPalette.reset(new Array<Colour>());
        palette = colourPalette.get();
    }

    LumatoneEditorColourPalette(Array<Colour> colourPaletteIn)
        : LumatoneEditorColourPalette()
    {
        for (auto colour : colourPaletteIn)
            palette->add(colour);
    }

    LumatoneEditorColourPalette(String paletteAsString)
        : LumatoneEditorColourPalette()
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

    LumatoneEditorColourPalette(ValueTree paletteNode)
        : LumatoneEditorColourPalette()
    {
        name = paletteNode[LumatoneEditorPaletteIds::nameId];

        for (auto swatch : paletteNode)
        {
            if (swatch.hasType(LumatoneEditorPaletteIds::swatchId))
                palette->add(Colour::fromString(swatch[LumatoneEditorPaletteIds::colourId].toString()));
        }
    }

    LumatoneEditorColourPalette(const LumatoneEditorColourPalette& palette)
        : LumatoneEditorColourPalette(*palette.palette) {}

    ~LumatoneEditorColourPalette()
    {
        palette = nullptr;
        colourPalette = nullptr;
    }

    int size() const { return palette->size(); }

    ValueTree toValueTree() const
    {
        ValueTree node(LumatoneEditorPaletteIds::paletteId);
        node.setProperty(LumatoneEditorPaletteIds::nameId, name, nullptr);

        for (auto c : *colourPalette)
        {
            ValueTree swatch(LumatoneEditorPaletteIds::swatchId);
            swatch.setProperty(LumatoneEditorPaletteIds::colourId, c.toString(), nullptr);
            node.appendChild(swatch, nullptr);
        }

        return node;
    }

    String toString() const
    {
        return toValueTree().toXmlString();
    }

    //==============================================================================
    // Public members

    Array<Colour>* palette;
    String name;

    //==============================================================================
    
    static String paletteArrayToString(const Array<LumatoneEditorColourPalette>& palettes)
    {
        ValueTree libraryNode(LumatoneEditorPaletteIds::libraryId);

        for (int p = 0; p < palettes.size(); p++)
        {
            libraryNode.appendChild(palettes[p].toValueTree(), nullptr);
        }

        return libraryNode.toXmlString();
    }

    static Array<LumatoneEditorColourPalette> loadPalettesFromString(String stringIn)
    {
        ValueTree libraryNode = ValueTree::fromXml(stringIn);

        if (libraryNode.hasType(LumatoneEditorPaletteIds::libraryId))
            return loadPalettesFromNode(libraryNode);
        
        else if (!libraryNode.isValid())
            return loadPalettesFromStringOld(stringIn);
    }

    static Array<LumatoneEditorColourPalette> loadPalettesFromNode(ValueTree nodeIn)
    {
        Array<LumatoneEditorColourPalette> library;

        if (nodeIn.hasType(LumatoneEditorPaletteIds::libraryId))
        {
            for (auto palette : nodeIn)
                library.add(LumatoneEditorColourPalette(palette));
        }

        return library;
    }

private:

    // Palettes should be separated by a space
    static Array<LumatoneEditorColourPalette> loadPalettesFromStringOld(String stringIn)
    {
        Array<LumatoneEditorColourPalette> palettesOut;

        int i = 0;
        while (i < stringIn.length())
        {
            int endIndex = stringIn.indexOf(i, " ");
            if (endIndex < 0)
                endIndex = stringIn.length();

            auto paletteString = stringIn.substring(i, endIndex);
            if (paletteString.length() > 0)
                palettesOut.add(LumatoneEditorColourPalette(stringIn.substring(i, endIndex)));

            i += paletteString.length() + 1;
        }

        return palettesOut;
    }

private:

    std::unique_ptr<Array<Colour>> colourPalette;

};
