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
    static Identifier libraryId         = Identifier("PaletteLibrary"); // Currently unused
    static Identifier paletteId         = Identifier("ColourPalette");
    static Identifier swatchId          = Identifier("Swatch");
    static Identifier colourId          = Identifier("Colour");

    static Identifier nameId            = Identifier("Name");
    static Identifier dateCreatedId     = Identifier("DateCreated");
    static Identifier authorId          = Identifier("Author");
    static Identifier notesId           = Identifier("Notes");
}

#define PALETTEFILEEXTENSION ".ltp"
#define DEFAULTPALETTESIZE 10

//==============================================================================
/*
* Container for colour palette data
*/
class LumatoneEditorColourPalette
{
public:
    LumatoneEditorColourPalette(String paletteName="")
    {
        colourPalette.resize(DEFAULTPALETTESIZE);
        colourPalette.fill(Colour(0xff1b1b1b));
        name = paletteName;
        dateCreated = Time::getCurrentTime().toISO8601(false);
    }

    LumatoneEditorColourPalette(Array<Colour> colourPaletteIn, String paletteName="")
        : LumatoneEditorColourPalette(paletteName)
    {
        setColours(colourPaletteIn);
    }

    LumatoneEditorColourPalette(Array<Colour> colourPaletteIn, String paletteName, String paletteAuthor, String paletteNotes="")
        : LumatoneEditorColourPalette(colourPaletteIn, paletteName)
    {
        author = paletteAuthor;
        notes = paletteNotes;
    }
    
    LumatoneEditorColourPalette clone() const { return LumatoneEditorColourPalette(colourPalette, name, author, notes); }

    int size() const { return colourPalette.size(); }

    Array<Colour>* getColours() { return &colourPalette; }

    String getName() const { return name; }

    String getAuthor() const { return author; }

    String getNotes() const { return notes; }

    bool hasBeenModified() const { return modifiedSinceLastSave; }

    /// <summary>
    /// The timestamp from when this was first created
    /// </summary>
    /// <returns>String created from Time::toISO8601()</returns>
    String getDateCreated() const { return dateCreated; }

    String getPathToFile() const { return pathToFile; }

    void setColours(const Array<Colour>& newColourPalette)
    {
        setModifiedIfChanged(colourPalette, newColourPalette);

        colourPalette.clear();
        colourPalette.addArray(newColourPalette);
    }

    void setName(String nameIn)     { setModifiedIfChanged(name, nameIn); name = nameIn; }

    void setAuthor(String authorIn) { setModifiedIfChanged(author, authorIn); author = authorIn; }

    void setNotes(String notesIn)   { setModifiedIfChanged(notes, notesIn); notes = notesIn; }

    /// <summary>
    /// Save palette to a .ltp file (and update internal file path)
    /// Warning: deletes old version regardless if file path is not different from the previous version
    /// </summary>
    /// <param name="fileToSaveTo"></param>
    /// <param name="deleteOldVersion"></param>
    bool saveToFile(File fileToSaveTo, bool deleteOldVersion = true)
    {
        // Make sure it has proper extension
        fileToSaveTo = fileToSaveTo.withFileExtension(PALETTEFILEEXTENSION);

        if (fileToSaveTo.getFullPathName() == pathToFile)
            deleteOldVersion = true;

        if (deleteOldVersion)
        {
            File originalFile = pathToFile;

            if (originalFile.existsAsFile())
                originalFile.deleteFile();
        }

        if (!fileToSaveTo.existsAsFile())
            fileToSaveTo.create();

        if (fileToSaveTo.replaceWithText(toValueTree().toXmlString()))
        {
            modifiedSinceLastSave = false;
            pathToFile = fileToSaveTo.getFullPathName();

            return true;
        }

        return false;
    }
    bool saveToFile()
    {
        return saveToFile(File(pathToFile));
    }

    ValueTree toValueTree() const
    {
        ValueTree node(LumatoneEditorPaletteIds::paletteId);
        node.setProperty(LumatoneEditorPaletteIds::nameId, name, nullptr);
        node.setProperty(LumatoneEditorPaletteIds::authorId, author, nullptr);
        node.setProperty(LumatoneEditorPaletteIds::dateCreatedId, dateCreated, nullptr);
        node.setProperty(LumatoneEditorPaletteIds::notesId, notes, nullptr);

        for (auto c : colourPalette)
        {
            ValueTree swatch(LumatoneEditorPaletteIds::swatchId);
            swatch.setProperty(LumatoneEditorPaletteIds::colourId, c.toString(), nullptr);
            node.appendChild(swatch, nullptr);
        }

        return node;
    }

    String toString() const { return toValueTree().toXmlString(); }

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

            loadedPalette.modifiedSinceLastSave = false;
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

private:

    template <class T>
    void setModifiedIfChanged(const T& originalValue, const T& newValue) { if (originalValue != newValue) modifiedSinceLastSave = true; }

private:
    
    Array<Colour> colourPalette;
    
    String name = "";
    String author = "";
    String notes = "";

    String dateCreated = "";

    // Internal use only
    String pathToFile = "";
    bool modifiedSinceLastSave = true;
};

class LumatoneEditorPaletteSorter : juce::DefaultElementComparator<const LumatoneEditorColourPalette&>
{
public:
    static int compareElements(const LumatoneEditorColourPalette& first, const LumatoneEditorColourPalette& second)
    {
        // Reverse chronological sort
        Time t0 = Time::fromISO8601(first.getDateCreated());
        Time t1 = Time::fromISO8601(second.getDateCreated());
        return (t1 < t0) ? -1 : ((t0 < t1) ? 1 : 0);
    }
};
