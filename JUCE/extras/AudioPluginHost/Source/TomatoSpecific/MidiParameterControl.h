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

#pragma once

#include "InternalFilters.h"
#include <map>

//==============================================================================
/**
    Internal plugin that maps incoming MidiCC to every plugin parameter.
*/
class MidiParameterControl : public InternalPlugin
{
public:
	MidiParameterControl(const PluginDescription& descr);

	void SetAudioProcessorGraph(AudioProcessorGraph*);
	AudioProcessorGraph* GetAudioProcessorGraph() { return graph; };

	static String getIdentifier()
	{
		return "MidiParameterControl";
	}

	static PluginDescription getPluginDescription()
	{
		return InternalPlugin::getPluginDescription(getIdentifier(), false, false);
	}

	void prepareToPlay(double newSampleRate, int) override;
	void reset() override;
	void releaseResources() override;
	void processBlock(AudioBuffer<float>& buffer, MidiBuffer&) override;

	AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	class ControllableParameter
	{
	public:
		AudioProcessor &processorParameterOwner;
		int parameterIndex;

		ControllableParameter(AudioProcessor &newprocessorParameterOwner, int newparameterIndex) :
			processorParameterOwner(newprocessorParameterOwner),
			parameterIndex(newparameterIndex)
		{
		}

		ControllableParameter(ControllableParameter &copy) :
			processorParameterOwner(copy.processorParameterOwner),
			parameterIndex(copy.parameterIndex)
		{
		}
	};

	std::map<int, ControllableParameter*> const &getParameterMappings() { return mappings; };
	void addMapping(int MidiCC, AudioProcessor &processorParameterOwner, int parameterIndex);

protected:
	AudioProcessorGraph *graph;
	std::map<int, ControllableParameter*> mappings;

private:
	long int samplesSinceStart;
};

