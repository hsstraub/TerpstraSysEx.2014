/*
  ==============================================================================

    LumatoneController.h
    Created: 31 Dec 2023 3:38:41pm
    Author:  straub

  ==============================================================================
*/

#pragma once

class LumatoneController
{
public:
	int getOctaveSize() const { return octaveSize; }

private:
	int                         octaveSize = 56;
};
