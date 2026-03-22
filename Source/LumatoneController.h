/*
  ==============================================================================

    LumatoneController.h
    Created: 22 Mar 2026 8:54:56pm
    Author:  hsstraub

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
