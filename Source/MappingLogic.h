/*
  ==============================================================================

    MappingLogic.h
    Created: 27 Apr 2016 9:20:43pm
    Author:  hsstraub

  ==============================================================================
*/

#ifndef MAPPINGLOGIC_H_INCLUDED
#define MAPPINGLOGIC_H_INCLUDED

#include "KeyboardDataStructure.h"


// Logic for the transformation between an abstract index and the concrete MIDI specifications, depending on the chosen mapping
class MappingLogicBase
{
public:
	// Global number of notes in the mapping
	virtual int globalMappingSize() const = 0;
	// Returns the Terpstra key specification fo the "inx"-th note
	virtual TerpstraKey indexToTerpstraKey(int inx) const = 0;
};

// Mapping logic for MIDI notes, increasing order
class IncrMidiNotesMappingLogic : public MappingLogicBase
{
public:
	IncrMidiNotesMappingLogic(int newMaxMIDINote, int newChannelInCaseOfSingleChannel);

	int globalMappingSize() const override;
	TerpstraKey indexToTerpstraKey(int inx) const override;

	bool isSingleChannel() const { return this->channelInCaseOfSingleChannel > 0; }

private:
	// Maximal MIDI note (global max in case of single channel. In case of multiple channel followed by note 0 of next channel.)
	int maxMIDINote;
	int channelInCaseOfSingleChannel;

};


#endif  // MAPPINGLOGIC_H_INCLUDED
