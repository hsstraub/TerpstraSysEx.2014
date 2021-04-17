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

void MappingLogicBase::setPeriodSize(int newPeriodSize, bool forceRefresh)
{
	if (newPeriodSize != this->periodSize || forceRefresh)
	{
		this->periodSize = newPeriodSize;

		// Notify listeners
		this->listeners.call(&Listener::mappingLogicChanged, this);
	}
}

int MappingLogicBase::getIndexFromStartOfMap(int inx) const
{
    if (this->getPeriodSize() <= 0) {
        jassertfalse;
        return 0;
    }

    int relativeInx = (inx - getStartOfMap()) % this->getPeriodSize();
    while (relativeInx < 0)
        relativeInx += this->getPeriodSize();
    return relativeInx;

}

juce::Colour MappingLogicBase::indexToColour(int inx) const
{
	if (inx < 0 || inx >= this->globalMappingSize() || !this->assignColours)
        return juce::Colour();

    auto noteRelativeToStart = getIndexFromStartOfMap(inx);
    auto colourGroupIndex = scaleStructure.getGroupOfDegree(noteRelativeToStart);
    if (colourGroupIndex < 0 || colourGroupIndex >= colourTable.size())
    {
        jassertfalse;
        return juce::Colour();
    }
    return colourTable.getReference(colourGroupIndex);
}

void MappingLogicBase::indexToTerpstraKey(int inx, TerpstraKey& keyData) const
{
    keyData.keyType = LumatoneKeyType::noteOnNoteOff;
	keyData.channelNumber = indexToMIDIChannel(inx);
	keyData.noteNumber = indexToMIDINote(inx);

	if (this->assignColours)
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
    : MappingLogicBase(scaleStructureIn, colourTableIn)
{
}

//=============================================================
// Set parameters

void IncrMidiNotesMappingLogic::setChannelInCaseOfSingleChannel(int newChannelInCaseOfSingleChannel, bool forceRefresh)
{
	if (newChannelInCaseOfSingleChannel != this->channelInCaseOfSingleChannel || forceRefresh)
	{
		this->channelInCaseOfSingleChannel = newChannelInCaseOfSingleChannel;

		// Notify listeners
		this->listeners.call(&Listener::mappingLogicChanged, this);
	}
}

void IncrMidiNotesMappingLogic::setValues(int newPeriodSize, int newChannelInCaseOfSingleChannel, bool forceRefresh)
{
	if (newPeriodSize != this->periodSize || newChannelInCaseOfSingleChannel != this->channelInCaseOfSingleChannel
        || forceRefresh)
	{
		this->periodSize = newPeriodSize;
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
		return 128;	// notes start at 0 and go until periodSize-1
	else
		return this->periodSize * 16;
}

int IncrMidiNotesMappingLogic::indexToMIDIChannel(int inx) const
{
	if (inx < 0 || inx >= this->globalMappingSize())
        return 0;
	else if (this->isSingleChannel())
        return this->channelInCaseOfSingleChannel;
    else
        // Channel is 1-based
        return (inx / this->periodSize) + 1;
}

int IncrMidiNotesMappingLogic::indexToMIDINote(int inx) const
{
	if (inx < 0 || inx >= this->globalMappingSize())
        return 0;
	else if (this->isSingleChannel())
        return inx;
    else
        // Note is 0-based and goes until periodSize-1
        return inx % this->periodSize;

}

int IncrMidiNotesMappingLogic::terpstraKeyToIndex(TerpstraKey keyData) const
{
	if (keyData.isEmpty() || this->globalMappingSize() == 0)
		return -1;

	if (this->isSingleChannel())
	{
	    // notes start at 0 and go until periodSize-1
		if (keyData.channelNumber != this->channelInCaseOfSingleChannel || keyData.noteNumber >= this->periodSize )
			return -1;

		return keyData.noteNumber;
	}
	else
	{
		if (keyData.noteNumber >= this->periodSize)
			return -1;

		return (keyData.channelNumber - 1) * this->periodSize + keyData.noteNumber;
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

/*
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
*/

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
