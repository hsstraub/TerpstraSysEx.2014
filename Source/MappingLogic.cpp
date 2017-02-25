/*
  ==============================================================================

    MappingLogic.cpp
    Created: 27 Apr 2016 9:20:43pm
    Author:  hsstraub

  ==============================================================================
*/

#include "MappingLogic.h"


// Base class

void MappingLogicBase::addListener(MappingLogicBase::Listener* listener)
{
	listeners.add(listener);
}

void MappingLogicBase::removeListener(MappingLogicBase::Listener* listener)
{
	listeners.remove(listener);
}

///////////////////////////////////////////////////////////
// Increasing MIDI notes 

IncrMidiNotesMappingLogic::IncrMidiNotesMappingLogic()
	: maxMIDINote(0), channelInCaseOfSingleChannel(0)
{

}

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