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

private:
	bool isSelected;

	Label*	midiNoteLabel;
	Label*  midiChannelLabel;

	int keyColour;
};


/*
==============================================================================
Display representant of a key set of 56 keys
==============================================================================
*/
class TerpstraKeySetEdit : public ImageButton
{
public:
	TerpstraKeySetEdit();
	~TerpstraKeySetEdit();

	void setIsSelected(bool newValue) { setToggleState(newValue, dontSendNotification); };
};


/*
==============================================================================
Combo box with colours
==============================================================================
*/

#define ADDCOLOURTOCOMBOBOX true
#define DONTADDCOLOURTOCOMBOBOX false

class ColourComboBox : public ComboBox
{
public:
	explicit ColourComboBox(const String& componentName = String());

	void setTextFieldToColourObject(Colour newColour,
		NotificationType notification = sendNotificationAsync);

	int getColourIDFromText(bool addToBox);
	Colour getColourObjectFromText(bool addToBox);
};
#endif  // VIEWCOMPOONENTS_H_INCLUDED
