/*
  ==============================================================================

    MappingLogic.cpp
    Created: 27 Apr 2016 9:20:43pm
    Author:  hsstraub

  ==============================================================================
*/

#include "MappingLogic.h"

IncrMidiNotesMappingLogic::IncrMidiNotesMappingLogic(int newMaxMIDINote, int newChannelInCaseOfSingleChannel)
	: maxMIDINote(newMaxMIDINote), channelInCaseOfSingleChannel(newChannelInCaseOfSingleChannel)
{

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

