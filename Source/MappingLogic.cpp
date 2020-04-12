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
    channelMapping.channel = midiChannel;
    channelMapping.mapping = kbmMappingStructure;

    channelMappingData[subDlgIndex] = channelMapping;

    createMappingTable();
}

void KBMFilesMappingLogic::createMappingTable()
{
    mappingTable.clear();

    for ( int subTableIndex = 0; subTableIndex < noOfChannels; subTableIndex++)
    {
        if (channelMappingData[subTableIndex].channel > 0)
        {
            KBMMappingDataStructure::NoteAndFrequencyTable noteAndFrequencyTable =
                channelMappingData[subTableIndex].mapping.createNoteFrequencyTable();

            for ( int k = 0; k < noteAndFrequencyTable.size(); k++)
            {
                // Key in mapping table must be integer
                int keyInTable = roundToInt(noteAndFrequencyTable[k].frequency*1000);
                if (!mappingTable.contains(keyInTable))
                {
                    TerpstraKey keyData;
                    keyData.channelNumber = channelMappingData[subTableIndex].channel;
                    keyData.noteNumber = noteAndFrequencyTable[k].midiNote;

                    mappingTable.set(keyInTable, keyData);
                }
                // else: more than one key are defined for the same frequency. We just choose one of them
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
	if (inx < 0 || inx >= this->globalMappingSize())
		return TerpstraKey();	// Empty value

    HashMap<int, TerpstraKey>::Iterator itr(mappingTable);
    for ( int i = 0; i < inx; i++)
        itr.next();

    return itr.getValue();
}

int KBMFilesMappingLogic::terpstraKeyToIndex(TerpstraKey keyData) const
{
	if (keyData.isEmpty() || this->globalMappingSize() == 0)
		return -1;

    int resultInx = -1;
    int localInx = -1;
    HashMap<int, TerpstraKey>::Iterator itr(mappingTable);
    while (itr.next() && resultInx < 0)
    {
        localInx++;
        if ( itr.getValue().channelNumber == keyData.channelNumber && itr.getValue().noteNumber == keyData.noteNumber )
            resultInx = localInx;
    }

    return resultInx;
}
