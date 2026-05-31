/*
  ==============================================================================

    LocalisationMap.h
    Created: 1 Jan 2021 2:09:55pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

static String getLocalisation(String languageCode)
{
    languageCode = languageCode.toLowerCase();

    if (languageCode.startsWith("en"))
    {
        if (languageCode.endsWith("us"))
            return String::createStringFromData(BinaryData::enus_txt, BinaryData::enus_txtSize);
    }

    if (languageCode.startsWith("es"))
    {
        return String::createStringFromData(BinaryData::eses_txt, BinaryData::eses_txtSize);
    }

    if (languageCode.startsWith("fr"))
    {
        return String::createStringFromData(BinaryData::frca_txt, BinaryData::frca_txtSize);
    }

    // Fallback to en-gb
    return String::createStringFromData(BinaryData::engb_txt, BinaryData::engb_txtSize);
}
