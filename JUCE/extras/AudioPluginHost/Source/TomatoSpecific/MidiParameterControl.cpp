/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#include "MidiParameterControlEditor.h"
#include "MidiParameterControl.h"

//==============================================================================
MidiParameterControl::MidiParameterControl(const PluginDescription& descr) : InternalPlugin(descr, AudioChannelSet::disabled())
{
	graph = nullptr;
	samplesSinceStart = 0;
}

void MidiParameterControl::SetAudioProcessorGraph(AudioProcessorGraph* arg_graph)
{
	graph = arg_graph;
}

void MidiParameterControl::prepareToPlay(double newSampleRate, int buffer)
{
	_CRT_UNUSED(newSampleRate);
	_CRT_UNUSED(buffer);
}

void MidiParameterControl::reset() 
{

}

void MidiParameterControl::releaseResources() 
{

}

void MidiParameterControl::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	int samplePos;
	double seconds;
	MidiMessage message(0xf0);
	MidiBuffer::Iterator it(midiMessages);
	const double sampleRate = getSampleRate();

	jassert(sampleRate > 0.0);

	int numMess = midiMessages.getNumEvents();

	if (numMess > 0)
		numMess = numMess;

	while (it.getNextEvent(message, samplePos))
	{
		seconds = (double)(samplesSinceStart + samplePos) / sampleRate;

		if (message.isController())
		{
			int cc = message.getControllerNumber();
			int value = message.getControllerValue();
			int messageChan = message.getChannel();
			
			/*if ((messageChan == 1) ||
				(messageChan == 2))
			{*/
				float normalizedValue = (float)value / 127.0f;

				if (mappings.find(cc) == mappings.end()) {
					// not found, sorry
				}
				else {
					ControllableParameter *map = mappings.find(cc)->second;
					map->processorParameterOwner.setParameter(map->parameterIndex, normalizedValue);
				}
			//}

		}

	}

	samplesSinceStart += buffer.getNumSamples();

	midiMessages.clear();
}

bool MidiParameterControl::hasEditor() const
{
	return true; 
}

AudioProcessorEditor* MidiParameterControl::createEditor()
{
	return new MidiParameterControlEditor(*this);
}

void MidiParameterControl::addMapping(int MidiCC, AudioProcessor &processorParameterOwner, int parameterIndex)
{
	mappings.insert({ MidiCC, new ControllableParameter(processorParameterOwner, parameterIndex) });
}