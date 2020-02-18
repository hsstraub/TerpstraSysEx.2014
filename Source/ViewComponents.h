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

	enum ColourIds
	{
		backgroundColourId = 0x2000100,
		outlineColourId = 0x2000101,
		selectedKeyOutlineId = 0x2000102
	};

private:
    int getLineWidth() { return isSelected ? TERPSTRASELECTEDKEYFLDLINEWIDTH : TERPSTRASINGLEKEYFLDLINEWIDTH; }

	bool isSelected;

    Path    hexPath;
	Label*	midiNoteLabel;
	Label*  midiChannelLabel;

	int						keyColour;
	TerpstraKey::KEYTYPE	keyType;
};


/*
==============================================================================
Macro button component (holds two controller buttons)
==============================================================================
*/

class TerpstraMacroButton : public ImageButton
{
public:
	TerpstraMacroButton();
	~TerpstraMacroButton();

	typedef enum
	{
		leftbutton,
		rightbutton
	} MACROBUTTONSUBINDEX;

	void setIsSelected(MACROBUTTONSUBINDEX subIndex, bool newValue);
};


#endif  // VIEWCOMPOONENTS_H_INCLUDED
