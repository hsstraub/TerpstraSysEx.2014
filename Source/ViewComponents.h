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

	int						keyColour;
	TerpstraKey::KEYTYPE	keyType;
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
	enum colourComboboxOptions
	{
		DoNotAddColourToCombobox = 0,
		AddColourToComboBox
	};

	explicit ColourComboBox(const String& componentName = String());

	void setTextFieldToColourAsObject(Colour newColourAsObject,
		NotificationType notification = sendNotificationAsync);
	int getColourAsNumberFromText(colourComboboxOptions boxOptions);
	Colour getColourAsObjectFromText(colourComboboxOptions boxOptions);

	void addColourToBox(String newColourAsString);
	void addColourToBox(Colour newColourAsObject) { addColourToBox(newColourAsObject.toDisplayString(false)); }
	void addColourToBox(int newColourAsNumber) { addColourToBox(Colour(newColourAsNumber)); }
};

#endif  // VIEWCOMPOONENTS_H_INCLUDED
