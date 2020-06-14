/*
  ==============================================================================

    MappingLogic.cpp
    Created: 27 Apr 2016 9:20:43pm
    Author:  hsstraub

  ==============================================================================
*/

#include "MappingLogic.h"


//==============================================================================
// Base class

MappingLogicBase::MappingLogicBase(ScaleStructure& scaleStructureIn, Array<Colour>& colourTableIn)
    : scaleStructure(scaleStructureIn), colourTable(colourTableIn)
{
}

int MappingLogicBase::getIndexFromStartOfMap(int inx) const
{
    jassert(this->getPeriodSize() > 0);

    int relativeInx = (inx - getStartOfMap()) % this->getPeriodSize();
    while (relativeInx < 0)
        relativeInx += this->getPeriodSize();
    return relativeInx;

}

int MappingLogicBase::indexToColour(int inx) const
{
	if (inx < 0 || inx >= this->globalMappingSize())
        return 0;

    switch(this->colourAssignmentType)
    {
        case ColourAssignmentType::monochrome:
            // Ad hoc
            if (getIndexFromStartOfMap(inx) == 0)
                return 0x808080;
            else
                return 0xffffff;

        case ColourAssignmentType::fromScaleStructureEditor:

            // ToDo ScaleStructure

            if (getIndexFromStartOfMap(inx) == 0)
                return colourTable.getReference(0).getARGB();
            else
                return colourTable.getReference(1).getARGB();

        default:
            return 0;
    }
}

void MappingLogicBase::setColourAssignmentType(int value)
{
    switch(value)
    {
    case static_cast<int>(ColourAssignmentType::monochrome):
    case static_cast<int>(ColourAssignmentType::fromScaleStructureEditor):
        this->colourAssignmentType = static_cast<ColourAssignmentType>(value);
        break;
    default:
        this->colourAssignmentType = ColourAssignmentType::none;
        break;
    }
}

void MappingLogicBase::indexToTerpstraKey(int inx, TerpstraKey& keyData) const
{
	keyData.channelNumber = indexToMIDIChannel(inx);
	keyData.noteNumber = indexToMIDINote(inx);

	if (this->colourAssignmentType != ColourAssignmentType::none)
        keyData.colour = indexToColour(inx);
}

TerpstraKey MappingLogicBase::indexToTerpstraKey(int inx) const
{
	TerpstraKey keyData;
	indexToTerpstraKey(inx, keyData);
	return keyData;
}

void MappingLogicBase::addListener(MappingLogicBase::Listener* listener)
{
	listeners.add(listener);
}

void MappingLogicBase::removeListener(MappingLogicBase::Listener* listener)
{
	listeners.remove(listener);
}

//==============================================================================
// Increasing MIDI notes

IncrMidiNotesMappingLogic::IncrMidiNotesMappingLogic(ScaleStructure& scaleStructureIn, Array<Colour>& colourTableIn)
    : MappingLogicBase(scaleStructureIn, colourTableIn), maxMIDINote(0), channelInCaseOfSingleChannel(0)
{
}

//=============================================================
// Set parameters

void IncrMidiNotesMappingLogic::setMaxMidiNote(int newMaxMIDINote)
{
	if (newMaxMIDINote != this->maxMIDINote)
	{
		this->maxMIDINote = newMaxMIDINote;

		// Notify listeners
		this->listeners.call(&Listener::mappingLogicChanged, this);
	}
}

void IncrMidiNotesMappingLogic::setChannelInCaseOfSingleChannel(int newChannelInCaseOfSingleChannel)
{
	if (newChannelInCaseOfSingleChannel != this->channelInCaseOfSingleChannel)
	{
		this->channelInCaseOfSingleChannel = newChannelInCaseOfSingleChannel;

		// Notify listeners
		this->listeners.call(&Listener::mappingLogicChanged, this);
	}
}

void IncrMidiNotesMappingLogic::setValues(int newMaxMIDINote, int newChannelInCaseOfSingleChannel)
{
	if (newMaxMIDINote != this->maxMIDINote || newChannelInCaseOfSingleChannel != this->channelInCaseOfSingleChannel)
	{
		this->maxMIDINote = newMaxMIDINote;
		this->channelInCaseOfSingleChannel = newChannelInCaseOfSingleChannel;

		// Notify listeners
		this->listeners.call(&Listener::mappingLogicChanged, this);
	}
}

//============================================================
// Access mapping data (overrides)

int IncrMidiNotesMappingLogic::globalMappingSize() const
{
	if (isSingleChannel())
		return this->maxMIDINote+1;	// notes start at 0 and go until maxMIDINote
	else
		return (this->maxMIDINote+1) * 16;
}

int IncrMidiNotesMappingLogic::indexToMIDIChannel(int inx) const
{
	if (inx < 0 || inx >= this->globalMappingSize())
        return 0;
	else if (this->isSingleChannel())
        return this->channelInCaseOfSingleChannel;
    else
        // Channel is 1-based
        return (inx / (this->maxMIDINote + 1)) + 1;
}

int IncrMidiNotesMappingLogic::indexToMIDINote(int inx) const
{
	if (inx < 0 || inx >= this->globalMappingSize())
        return 0;
	else if (this->isSingleChannel())
        return inx;
    else
        // Note is 0-based and goes until maxMIDINote
        return inx % (this->maxMIDINote+1);

}

int IncrMidiNotesMappingLogic::terpstraKeyToIndex(TerpstraKey keyData) const
{
	if (keyData.isEmpty() || this->globalMappingSize() == 0)
		return -1;

	if (this->isSingleChannel())
	{
		if (keyData.channelNumber != this->channelInCaseOfSingleChannel || keyData.noteNumber > this->maxMIDINote )
			return -1;

		return keyData.noteNumber;
	}
	else
	{
		if (keyData.noteNumber > this->maxMIDINote)
			return -1;

		return (keyData.channelNumber - 1)*(this->maxMIDINote + 1) + keyData.noteNumber;
	}
}


//==============================================================================
// KBM files

KBMFilesMappingLogic::KBMFilesMappingLogic(ScaleStructure& scaleStructureIn, Array<Colour>& colourTableIn)
    : MappingLogicBase(scaleStructureIn, colourTableIn)
{
}

//==============================================================
// Set parameters

void KBMFilesMappingLogic::setMapping(int subDlgIndex, int midiChannel, KBMMappingDataStructure kbmMappingStructure)
{
    jassert(subDlgIndex >= 0 && subDlgIndex < noOfChannels);

    KBMMappingWithChannel channelMapping;
    channelMapping.channelNumber = midiChannel;
    channelMapping.mapping = kbmMappingStructure;

    channelMappingData[subDlgIndex] = channelMapping;

    createMappingTable();
}

void KBMFilesMappingLogic::createMappingTable()
{
    mappingTable.clear();

    for ( int subTableIndex = 0; subTableIndex < noOfChannels; subTableIndex++)
    {
        if (channelMappingData[subTableIndex].channelNumber > 0)
        {
            KBMMappingDataStructure::NoteAndFrequencyTable noteAndFrequencyTable =
                channelMappingData[subTableIndex].mapping.createNoteFrequencyTable();

            for ( int k = 0; k < noteAndFrequencyTable.size(); k++)
            {
                KBMMappingTableEntry tableEntry;
                tableEntry.channelNumber = channelMappingData[subTableIndex].channelNumber;
                tableEntry.noteNumber = noteAndFrequencyTable[k].noteNumber;
                tableEntry.frequency = noteAndFrequencyTable[k].frequency;

                mappingTable.add(tableEntry);   // This adds entry only if there is no other entry woth the same frequency.
            }
        }
    }

    // Notify listeners
    this->listeners.call(&Listener::mappingLogicChanged, this);
}

int KBMFilesMappingLogic::getStartOfMap() const
{
    // Start of map is supposed to be the same for all KBM files
    // ToDo display error/warning message if not so
    int subTableIndex;
    for (subTableIndex = 0; subTableIndex < noOfChannels; subTableIndex++)
    {
        if (channelMappingData[subTableIndex].channelNumber > 0)
            break;
    }

    if (subTableIndex == noOfChannels)
    {
        jassert(false);
        return 0;
    }

    TerpstraKey keyData;
    keyData.channelNumber = channelMappingData[subTableIndex].channelNumber;
    keyData.noteNumber = channelMappingData[subTableIndex].mapping.noteNrWhereMappingStarts;

    return terpstraKeyToIndex(keyData);
}

int KBMFilesMappingLogic::getPeriodSize() const
{
    // Period size is supposed to be the same for all KBM files
    // ToDo display error/warning message if not so
    int subTableIndex;
    for (subTableIndex = 0; subTableIndex < noOfChannels; subTableIndex++)
    {
        if (channelMappingData[subTableIndex].channelNumber > 0)
            break;
    }

    if (subTableIndex == noOfChannels)
    {
        jassert(false);
        return 0;
    }
    else
        return channelMappingData[subTableIndex].mapping.scaleSize;
}

//=================================================================
// Access mapping data (overrides)

int KBMFilesMappingLogic::globalMappingSize() const
{
    return mappingTable.size();
}

int KBMFilesMappingLogic::indexToMIDIChannel(int inx) const
{
	if (inx < 0 || inx >= globalMappingSize())
        return 0;
    else
        return mappingTable[inx].channelNumber;
}

int KBMFilesMappingLogic::indexToMIDINote(int inx) const
{
	if (inx < 0 || inx >= globalMappingSize())
        return 0;
    else
        return mappingTable[inx].noteNumber;
}

int KBMFilesMappingLogic::terpstraKeyToIndex(TerpstraKey keyData) const
{
	if (keyData.isEmpty() || globalMappingSize() == 0)
		return -1;

	int inx;
    for ( inx = 0; inx < globalMappingSize(); inx++)
    {
        if ( mappingTable[inx].channelNumber == keyData.channelNumber && mappingTable[inx].noteNumber == keyData.noteNumber )
            break;
    }

    return inx < globalMappingSize() ? inx : -1;
}
