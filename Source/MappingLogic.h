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
#include "KBMMappingDataStructure.h"


//==============================================================================
// Logic for the transformation between an abstract index and the concrete MIDI specifications, depending on the chosen mapping
class MappingLogicBase
{
public:
	// Global number of notes in the mapping
	virtual int globalMappingSize() const = 0;
	// Returns the Terpstra key specification for the "inx"-th note
	virtual TerpstraKey indexToTerpstraKey(int inx) const = 0;
	virtual int terpstraKeyToIndex(TerpstraKey keyData) const = 0;

	// Listener class, to notify changes
	class Listener
	{
	public:
		// Destructor
		virtual ~Listener() {}

		// Called when mapping logic changes
		virtual void mappingLogicChanged(MappingLogicBase* mappingLogicThatChanged) = 0;
	};

	/** Registers a listener that will be called when the box's content changes. */
	void addListener(Listener* listener);

	/** Deregisters a previously-registered listener. */
	void removeListener(Listener* listener);

protected:
	ListenerList<Listener> listeners;
};

//#ifndef DOXYGEN
/** This typedef is just for compatibility with old code and VC6 - newer code should use Button::Listener instead. */
//typedef MappingLogicBase::Listener MappingLogicListener;
//#endif


//==============================================================================
// Mapping logic for MIDI notes, increasing order
class IncrMidiNotesMappingLogic : public MappingLogicBase
{
public:
	IncrMidiNotesMappingLogic();

    //===============================
	// Set parameters

	void setMaxMidiNote(int newMaxMIDINote);
	void setChannelInCaseOfSingleChannel(int newChannelInCaseOfSingleChannel);
	void setValues(int newMaxMIDINote, int newChannelInCaseOfSingleChannel);

    //===============================
	// Access mapping data (overrides)

	int globalMappingSize() const override;
	TerpstraKey indexToTerpstraKey(int inx) const override;
	virtual int terpstraKeyToIndex(TerpstraKey keyData) const override;

	bool isSingleChannel() const { return this->channelInCaseOfSingleChannel > 0; }

private:
	// Maximal MIDI note (global max in case of single channel. In case of multiple channel followed by note 0 of next channel.)
	int maxMIDINote;
	int channelInCaseOfSingleChannel;
};


//==============================================================================
// Mapping logic for KBM files
class KBMFilesMappingLogic: public MappingLogicBase
{
public:
    KBMFilesMappingLogic();

    //===============================
	// Set parameters

	void setMapping(int subDlgIndex, int midiChannel, KBMMappingDataStructure kbmMappingStructure);

    //===============================
	// Access mapping data (overrides)


 	int globalMappingSize() const override;
	TerpstraKey indexToTerpstraKey(int inx) const override;
	virtual int terpstraKeyToIndex(TerpstraKey keyData) const override;

    //===============================
    // Attributes

    // Number of channels that can have a KBM file specified (Limited by the amount of space on the screen)
    static const int noOfChannels = 4;

private:
    struct KBMMappingWithChannel
    {
        int channel;
        KBMMappingDataStructure mapping;
    };

    KBMMappingWithChannel mappingData[noOfChannels];

};


#endif  // MAPPINGLOGIC_H_INCLUDED
