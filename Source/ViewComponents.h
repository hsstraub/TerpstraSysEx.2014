/*
  ==============================================================================

    ViewComponents.h
    Created: 16 Jun 2014 7:20:34pm
    Author:  hsstraub

  ==============================================================================
*/

#ifndef VIEWCOMPOONENTS_H_INCLUDED
#define VIEWCOMPOONENTS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "KeyboardDataStructure.h"
#include "ViewConstants.h"


/*
==============================================================================
Display of the data of one key
==============================================================================
*/
class TerpstraKeyEdit : public Component
{
public:
	TerpstraKeyEdit();
	~TerpstraKeyEdit();

	TerpstraKey getValue() const;
	void setValue(TerpstraKey newValue);

	void setIsSelected(bool newValue);

	void paint(Graphics& g);
	void resized();

	void setKeySize(float keySizeIn);

	enum ColourIds
	{
		backgroundColourId = 0x2000100,
		outlineColourId = 0x2000101,
		selectedKeyOutlineId = 0x2000102
	};

private:
	bool isSelected;

    Path    hexPath;
	Label*	midiNoteLabel;
	Label*  midiChannelLabel;

	int						keyColour;
	TerpstraKey::KEYTYPE	keyType;
	
	float					keySize;

	const float				channelLabelRadiusScalar = 0.4347826f;
};


#endif  // VIEWCOMPOONENTS_H_INCLUDED
