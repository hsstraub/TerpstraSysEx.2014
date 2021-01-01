/*
  ==============================================================================

    LocalisationMap.h
    Created: 1 Jan 2021 2:09:55pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

static const char* findLocalisation(String languageCode)
{
    if (languageCode.startsWith("en"))
    {
        if (languageCode.endsWith("us"))
            return BinaryData::enus_txt;

        return BinaryData::engb_txt;
    }

    if (languageCode.startsWith("es"))
    {
        return BinaryData::eses_txt;
    }

    if (languageCode.startsWith("fr"))
    {
        return BinaryData::frca_txt;
    }

    // Fallback to en-gb
    return BinaryData::engb_txt;
}

