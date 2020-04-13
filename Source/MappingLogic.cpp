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

IncrMidiNotesMappingLogic::IncrMidiNotesMappingLogic()
	: maxMIDINote(0), channelInCaseOfSingleChannel(0)
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

TerpstraKey IncrMidiNotesMappingLogic::indexToTerpstraKey(int inx) const
{
	if (inx < 0 || inx >= this->globalMappingSize())
		return TerpstraKey();	// Empty value

	TerpstraKey keyData;

	if (this->isSingleChannel())
	{
		keyData.channelNumber = this->channelInCaseOfSingleChannel;
		keyData.noteNumber = inx;
	}
	else
	{
		// Channel is 1-based, note is 0-based and goes until maxMIDINote
		keyData.channelNumber = (inx / (this->maxMIDINote + 1)) + 1;
		keyData.noteNumber = inx % (this->maxMIDINote+1);
	}

	return keyData;
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

KBMFilesMappingLogic::KBMFilesMappingLogic()
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


//=================================================================
// Access mapping data (overrides)

int KBMFilesMappingLogic::globalMappingSize() const
{
    return mappingTable.size();
}

TerpstraKey KBMFilesMappingLogic::indexToTerpstraKey(int inx) const
{
	if (inx < 0 || inx >= globalMappingSize())
		return TerpstraKey();	// Empty value

	TerpstraKey keyData;
    keyData.channelNumber = mappingTable[inx].channelNumber;
    keyData.noteNumber = mappingTable[inx].noteNumber;

    return keyData;
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
