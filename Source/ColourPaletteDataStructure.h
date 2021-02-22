/*
  ==============================================================================

    ColourPaletteDataStructure.h
    Created: 17 Jan 2021 9:55:13pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace LumatoneEditorPaletteIds
{
    static Identifier libraryId         = Identifier("PaletteLibrary");
    static Identifier paletteId         = Identifier("ColourPalette");
    static Identifier swatchId          = Identifier("Swatch");
    static Identifier colourId          = Identifier("Colour");

    static Identifier nameId            = Identifier("Name");
    static Identifier dateCreatedId     = Identifier("DateCreated");
    static Identifier authorId          = Identifier("Author");
    static Identifier notesId           = Identifier("Notes");
}

//==============================================================================
/*
* Container for colour palette data
*/
struct LumatoneEditorColourPalette
{

    LumatoneEditorColourPalette(String paletteName="")
    {
        colourPalette.reset(new Array<Colour>());
        palette = colourPalette.get();
        name = paletteName;
        dateCreated = Time::getCurrentTime().toISO8601(false);
    }

    LumatoneEditorColourPalette(Array<Colour> colourPaletteIn, String paletteName="")
        : LumatoneEditorColourPalette(paletteName)
    {
        palette->clear();
        palette->addArray(colourPaletteIn);
    }

    LumatoneEditorColourPalette(Array<Colour> colourPaletteIn, String paletteName, String paletteAuthor, String paletteNotes="")
        : LumatoneEditorColourPalette(colourPaletteIn, paletteName)
    {
        name = paletteName;
        author = paletteAuthor;
        notes = paletteNotes;

        palette->clear();
        palette->addArray(colourPaletteIn);
    }

    LumatoneEditorColourPalette(const LumatoneEditorColourPalette& paletteToCopy) : LumatoneEditorColourPalette() { *this = paletteToCopy; }

    ~LumatoneEditorColourPalette()
    {
        palette = nullptr;
        colourPalette = nullptr;
    }

    void operator=(const LumatoneEditorColourPalette& paletteToCopy)
    {
        this->palette->clear();
        this->palette->addArray(*paletteToCopy.palette);

        this->name          = paletteToCopy.name;
        this->author        = paletteToCopy.author;
        this->notes         = paletteToCopy.notes;
        this->dateCreated   = paletteToCopy.dateCreated;

        this->pathToFile    = paletteToCopy.pathToFile;
    }

    int size() const { return palette->size(); }

    String getPathToFile() const { return pathToFile; }

    void setPathToFile(String newPathToFile) { pathToFile = newPathToFile; }

    ValueTree toValueTree() const
    {
        ValueTree node(LumatoneEditorPaletteIds::paletteId);
        node.setProperty(LumatoneEditorPaletteIds::nameId, name, nullptr);
        node.setProperty(LumatoneEditorPaletteIds::authorId, author, nullptr);
        node.setProperty(LumatoneEditorPaletteIds::dateCreatedId, dateCreated, nullptr);
        node.setProperty(LumatoneEditorPaletteIds::notesId, notes, nullptr);

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
    String name = "";
    String author = "";
    String notes = "";

    //==============================================================================
    // Palette static methods

    static LumatoneEditorColourPalette loadFromValueTree(ValueTree paletteNode)
    {
        LumatoneEditorColourPalette loadedPalette;

        if (paletteNode.hasType(LumatoneEditorPaletteIds::paletteId))
        {
            Array<Colour> palette;

            for (auto swatch : paletteNode)
            {
                if (swatch.hasType(LumatoneEditorPaletteIds::swatchId))
                    palette.add(Colour::fromString(swatch[LumatoneEditorPaletteIds::colourId].toString()));
            }

            loadedPalette = LumatoneEditorColourPalette(palette,
                paletteNode[LumatoneEditorPaletteIds::nameId],
                paletteNode[LumatoneEditorPaletteIds::authorId],
                paletteNode[LumatoneEditorPaletteIds::notesId]
            );

            if (paletteNode[LumatoneEditorPaletteIds::dateCreatedId].toString() != String())
                loadedPalette.dateCreated = paletteNode[LumatoneEditorPaletteIds::dateCreatedId];
        }

        return loadedPalette;
    }

    static LumatoneEditorColourPalette loadFromFile(const File& pathToPalette)
    {
        LumatoneEditorColourPalette loadedPalette;

        if (pathToPalette.existsAsFile())
        {
            loadedPalette = loadFromValueTree(ValueTree::fromXml(pathToPalette.loadFileAsString()));

            // Set empty palette name to file name if it does not appear to be a placeholder
            if (loadedPalette.name == String() && !pathToPalette.getFileNameWithoutExtension().contains("UnnamedPalette"))
            {
                loadedPalette.name = pathToPalette.getFileNameWithoutExtension();
            }
        }

        loadedPalette.pathToFile = pathToPalette.getFullPathName();

        return loadedPalette;
    }

    //==============================================================================
    // Palette Library based static methods, currently unused in favor of separate palette files

    //static String paletteArrayToString(const Array<LumatoneEditorColourPalette>& palettes)
    //{
    //    ValueTree libraryNode(LumatoneEditorPaletteIds::libraryId);

    //    for (int p = 0; p < palettes.size(); p++)
    //    {
    //        libraryNode.appendChild(palettes[p].toValueTree(), nullptr);
    //    }

    //    return libraryNode.toXmlString();
    //}

    //static Array<LumatoneEditorColourPalette> loadPalettesFromString(String stringIn)
    //{
    //    ValueTree libraryNode = ValueTree::fromXml(stringIn);
    //    return loadPalettesFromNode(libraryNode);
    //}

    //static Array<LumatoneEditorColourPalette> loadPalettesFromNode(ValueTree nodeIn)
    //{
    //    Array<LumatoneEditorColourPalette> library;

    //    if (nodeIn.hasType(LumatoneEditorPaletteIds::libraryId))
    //    {
    //        for (auto palette : nodeIn)
    //            library.add(LumatoneEditorColourPalette(palette));
    //    }

    //    return library;
    //}

private:
    
    std::unique_ptr<Array<Colour>> colourPalette;
    String dateCreated = "";

    // Internal use only
    String pathToFile = "";
};
