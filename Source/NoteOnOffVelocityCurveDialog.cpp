/*
  ==============================================================================

    NoteOnOffVelocityCurveDialog.cpp
    Created: 13 Nov 2020 10:20:07pm
    Author:  hsstraub

  ==============================================================================
*/

#include "NoteOnOffVelocityCurveDialog.h"
#include "TerpstraMidiDriver.h"
#include "Main.h"

//==============================================================================
NoteOnOffVelocityCurveDialog::NoteOnOffVelocityCurveDialog()
	: VelocityCurveDlgBase(TerpstraMidiDriver::VelocityCurveType::noteOnNoteOff)
{
	labelCurrentXPos->setSize(128, labelCurrentXPos->getHeight());

	// Default interval table: equal division
	for (int i = 0; i < VELOCITYINTERVALTABLESIZE; i++)
	{
		velocityIntervalTableValues[i] = ticksCountFromXPos(i + 1);
	}

	restoreIntervalTableFromPropertiesFile(TerpstraSysExApplication::getApp().getPropertiesFile());
}

NoteOnOffVelocityCurveDialog::~NoteOnOffVelocityCurveDialog()
{
	saveIntervalTableToPropertiesFile(TerpstraSysExApplication::getApp().getPropertiesFile());
}

void NoteOnOffVelocityCurveDialog::restoreIntervalTableFromPropertiesFile(PropertiesFile* propertiesFile)
{
	String intervalTableString = propertiesFile->getValue("NoteOnOffVelocityIntervalTable");
	// ToDo Saved string from older version, with drawing stategy and values -1: to be ignored 

	StringArray intervalTableValueArray = StringArray::fromTokens(intervalTableString, false);
	if (intervalTableValueArray.size() > 0)
	{
		jassert(intervalTableValueArray.size() >= VELOCITYINTERVALTABLESIZE);

		for (int i = 0; i < VELOCITYINTERVALTABLESIZE; i++)
		{
			velocityIntervalTableValues[i] = intervalTableValueArray[i].getIntValue();
		}
	}
	else
	{
		// Default interval table: equal division
		for (int i = 0; i < VELOCITYINTERVALTABLESIZE; i++)
		{
			velocityIntervalTableValues[i] = ticksCountFromXPos(i + 1);
		}
	}
}

void NoteOnOffVelocityCurveDialog::saveIntervalTableToPropertiesFile(PropertiesFile* propertiesFile)
{
	String intervalTableString;
	for (auto intervalTableValue : velocityIntervalTableValues)
		intervalTableString += String(intervalTableValue) + " ";

	propertiesFile->setValue("NoteOnOffVelocityIntervalTable", intervalTableString);
}

void NoteOnOffVelocityCurveDialog::sendVelocityTableToController()
{
	TerpstraSysExApplication::getApp().getMidiDriver().sendVelocityIntervalConfig(velocityIntervalTableValues);

	__super::sendVelocityTableToController();
}

void NoteOnOffVelocityCurveDialog::sendVelocityConfigurationRequest()
{
	TerpstraSysExApplication::getApp().getMidiDriver().sendVelocityIntervalConfigRequest();

	__super::sendVelocityConfigurationRequest();
}

void NoteOnOffVelocityCurveDialog::midiMessageReceived(const MidiMessage& midiMessage)
{
	if (TerpstraSysExApplication::getApp().getMidiDriver().messageIsVelocityIntervalConfigReceptionMessage(midiMessage))
	{
		auto sysExData = midiMessage.getSysExData();
		auto answerState = sysExData[5];

		if (answerState == TerpstraMidiDriver::ACK)
		{
			// After the answer state byte there must be 254 bytes of data
			jassert(midiMessage.getSysExDataSize() >= (6 + 2 * VELOCITYINTERVALTABLESIZE)); // ToDo display error otherwise

			for (int i = 0; i < VELOCITYINTERVALTABLESIZE; i++)
				velocityIntervalTableValues[i] = (sysExData[6 + 2 * i] << 6) + sysExData[7 + 2 * i];
		}

		resized();
		repaint();
	}
	else
		__super::midiMessageReceived(midiMessage);
}

String NoteOnOffVelocityCurveDialog::beamXPosText(int xPos) const
{
	auto ticksCount = ticksCountFromXPos(xPos);

	{ return String(ticksCount) + " ticks (" + String(milliSecondsFromTicksCount(ticksCount)) + " ms)"; }
}

float NoteOnOffVelocityCurveDialog::beamWidth(int xPos) const
{
	if (xPos == 0)
	{
		return (getWidth() - 2.0f * labelEditMode->getX()) * velocityIntervalTableValues[0] / 2048.0f;
	}
	else if (xPos < VELOCITYINTERVALTABLESIZE)
	{
		return (getWidth() - 2.0f * labelEditMode->getX()) * (velocityIntervalTableValues[xPos] - velocityIntervalTableValues[xPos-1]) / 2048.0f;
	}
	else if (xPos == VELOCITYINTERVALTABLESIZE)
	{
		return (getWidth() - 2.0f * labelEditMode->getX()) * (2048.0f - velocityIntervalTableValues[xPos-1]) / 2048.0f;
	}
	else
	{
		jassertfalse;
		return 1;
	}
}