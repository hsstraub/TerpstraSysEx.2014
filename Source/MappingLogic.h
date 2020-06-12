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
#include "ScaleStructureController/ScaleStructure.h"


//==============================================================================
// Logic for the transformation between an abstract index and the concrete MIDI specifications, depending on the chosen mapping
class MappingLogicBase
{
public:
    enum ColourAssignmentType
    {
        none = 0,
        monochrome,
        fromScaleStructureEditor
    };

    void setColourAssignmentType(int value);

	// Global number of notes in the mapping
	virtual int globalMappingSize() const = 0;

	virtual int indexToMIDIChannel(int inx) const = 0;
	virtual int indexToMIDINote(int inx) const = 0;
	virtual int indexToColour(int inx) const;

	// sets the Terpstra key specification for the "inx"-th note
	virtual void indexToTerpstraKey(int inx, TerpstraKey& keyData) const;

	// Returns the Terpstra key specification for the "inx"-th note
	virtual TerpstraKey indexToTerpstraKey(int inx) const;

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
    ColourAssignmentType colourAssignmentType = ColourAssignmentType::none;
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

	int indexToMIDIChannel(int inx) const override;
	int indexToMIDINote(int inx) const override;

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
    //===============================
    // Types
private:
    struct KBMMappingWithChannel
    {
        int channelNumber = 0;
        KBMMappingDataStructure mapping;
    };

    class KBMMappingTableEntry : public KBMMappingDataStructure::noteAndFrequency
    {
    public:
        int channelNumber;

    public:
        // Comparison operators, so a list of it can be sorted
        bool operator==(const KBMMappingTableEntry& second) const { return this->frequency == second.frequency; }
        bool operator<(const KBMMappingTableEntry& second) const { return this->frequency < second.frequency; }
        bool operator>(const KBMMappingTableEntry& second) const { return this->frequency > second.frequency; }
        bool operator<=(const KBMMappingTableEntry& second) const { return this->frequency <= second.frequency; }
        bool operator>=(const KBMMappingTableEntry& second) const { return this->frequency >= second.frequency; }
    };

public:
    KBMFilesMappingLogic();

    //===============================
	// Set parameters

	void setMapping(int subDlgIndex, int midiChannel, KBMMappingDataStructure kbmMappingStructure);
protected:
    void createMappingTable();

    //===============================
	// Access mapping data (overrides)
public:
 	int globalMappingSize() const override;

	int indexToMIDIChannel(int inx) const override;
	int indexToMIDINote(int inx) const override;

	virtual int terpstraKeyToIndex(TerpstraKey keyData) const override;

    //===============================
    // Attributes

    // Number of channels that can have a KBM file specified (Limited by the amount of space on the screen)
    static const int noOfChannels = 4;

private:
    KBMMappingWithChannel channelMappingData[noOfChannels];

    // Frequency to key definition mapping. Key must be integer for the default hash funciton to work
    SortedSet<KBMMappingTableEntry> mappingTable;

};


#endif  // MAPPINGLOGIC_H_INCLUDED
