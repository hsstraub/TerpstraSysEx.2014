/*
  ==============================================================================

    BoardGeometry.h
    Created: 9 May 2016 9:04:35pm
    Author:  hsstraub

  ==============================================================================
*/

#ifndef BOARDGEOMETRY_H_INCLUDED
#define BOARDGEOMETRY_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
// Manages the geometry of the board - used to create isomorphic key mappings 
class TerpstraBoardGeometry
{
	// Types
public:
	typedef Array<int> StraightLine;

	// Construction
public:
	TerpstraBoardGeometry();

	// Methods
public:
	StraightLine horizontalLineOfField(int fieldIndex);

	// Attributes
private:
	Array<StraightLine>	horizontalLines;
	Array<StraightLine>	rightUpwardLines;

};


#endif  // BOARDGEOMETRY_H_INCLUDED
